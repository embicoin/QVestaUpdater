#include "QVestaUpdater.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QMetaProperty>
#include <QVariant>
#include <QDebug>
#include <QProcess>
#include <QtConcurrent/QtConcurrentRun>
#include <QStateMachine>
#include <QState>

QVestaUpdater::QVestaUpdater() {
    deserialize();
    setupMachine();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(checkStatus()) );
}

QVestaUpdater::~QVestaUpdater() {
    serialize();
}

//------------------------- StateMachine stuff ------------------------------
void QVestaUpdater::setupMachine() {
    QState *sInitialCheck = new QState();
    QState *sUpToDate     = new QState();
    QState *sUpdateReady  = new QState();
    QState *sWaitVestaExit= new QState();
    QState *sInstallBegin = new QState();

    sInitialCheck->addTransition(this,
        SIGNAL(upToDate       (QDateTime)), sUpToDate     );
    sInitialCheck->addTransition(this,
        SIGNAL(updateReady    (QDateTime)), sUpdateReady  );
    sUpToDate    ->addTransition(this,
        SIGNAL(updateReady    (QDateTime)), sUpdateReady  );
    sUpdateReady ->addTransition(this,
        SIGNAL(vestaRunning            ()), sWaitVestaExit);
    sUpdateReady ->addTransition(this,
        SIGNAL(vestaNotRunning         ()), sInstallBegin );
    sInstallBegin->addTransition(this,
        SIGNAL(installFinished(QDateTime)), sUpToDate     );
    sWaitVestaExit->addTransition(this,
        SIGNAL(vestaNotRunning         ()), sInstallBegin );

    QObject::connect(
        sInitialCheck, SIGNAL(entered()), this, SLOT(onEnterInitialCheck()));
    QObject::connect(
        sUpToDate,     SIGNAL(entered()), this, SLOT(onEnterUpToDate    ()));
    QObject::connect(
        sUpdateReady,  SIGNAL(entered()), this, SLOT(onEnterUpdateReady ()));
    QObject::connect(
        sInstallBegin, SIGNAL(entered()), this, SLOT(onEnterInstallBegin()));
    QObject::connect(
        sWaitVestaExit,SIGNAL(entered()), this, SLOT(onEnterWaitVestaExit()));

    machine.addState(sInitialCheck );
    machine.addState(sUpToDate     );
    machine.addState(sUpdateReady  );
    machine.addState(sWaitVestaExit);
    machine.addState(sInstallBegin );

    machine.setInitialState(sInitialCheck);
    machine.start();
}

void QVestaUpdater::onEnterInitialCheck() {
    emit enterInitialCheck();
    checkStatus();
}
void QVestaUpdater::onEnterUpToDate() {
    emit enterUpToDate(getInstalledVersion());
    checkStatus();
    startTimer();
}
void QVestaUpdater::onEnterUpdateReady() {
    emit enterUpdateReady(getLatestVersion());
    checkVestaRunning();
}
void QVestaUpdater::onEnterInstallBegin() {
    emit enterInstallBegin(getLatestVersion());
    doInstall();
}
void QVestaUpdater::onEnterWaitVestaExit() {
    emit enterWaitVestaExit();
}

//----------------------------- Helpers ---------------------------------------
void QVestaUpdater::checkStatus() {
    QDateTime dropboxVersion = checkLatestVersionInDropbox();
    if ( dropboxVersion > getInstalledVersion().addSecs(60) ) {
        setLatestVersion(dropboxVersion);
        emit updateReady(dropboxVersion);
    } else {
        emit upToDate(getInstalledVersion());
    }
    checkVestaRunning();
}

void QVestaUpdater::checkVestaRunning() {
    QtConcurrent::run(this,
                      &QVestaUpdater::checkVestaRunningWithTasklist );
}

void QVestaUpdater::checkVestaRunningWithTasklist() {
    const QString vestaProcessName = "Vesta.exe";
    QProcess tasklist;
    tasklist.start(
        "tasklist",
        QStringList()<< "/NH"          // без заголовка
                     << "/FO" << "CSV" // в формате CSV
                     << "/FI"          // с фильтром по названию
                     << QString("IMAGENAME eq %1").arg(vestaProcessName));
    tasklist.waitForFinished();
    QString output = tasklist.readAllStandardOutput();
    bool isRunning =
            output.startsWith(QString("\"%1").arg(vestaProcessName));
    if(isRunning) { emit vestaRunning();    }
    else          { emit vestaNotRunning(); }
}

void QVestaUpdater::doInstall() {
    stopTimer();
    QtConcurrent::run(this, &QVestaUpdater::installLatestVersionThread);
}

void QVestaUpdater::installLatestVersionThread() {
    emit installStarted(getLatestVersion());
    // Вызываем скрипт, который должен
    //   создать папку VestaDizLite в каталоге приложения,
    //   разархивировать туда архив VestaDizLite.zip из Dropbox
    QProcess unzipScript;
    QString unzipCommand = "VestaDizLiteCopyAndUnzip.bat";
    unzipScript.start(unzipCommand);
    unzipScript.waitForFinished();
    setInstalledVersion(getLatestVersion());
    emit installFinished(getInstalledVersion());
}

void QVestaUpdater::startTimer()
{ timer->start(60 * 1000); }

void QVestaUpdater::stopTimer()
{ timer->stop(); }

QDateTime QVestaUpdater::checkLatestVersionInDropbox() {
    //QFileInfo fileInfo("C:/Users/1/Dropbox/VestaLite (2)/VestaDizLite.zip");
    QFileInfo fileInfo("C:/Temp/VestaDizLite.zip");
    return fileInfo.lastModified();
}

void QVestaUpdater::startVesta() {
	QProcess::startDetached("\"C:/Program Files (x86)/Vesta/Vesta.exe\"");
}

// ----------------------- Serialization ---------------------------------
bool QVestaUpdater::serialize() {
    QFile file("settings.dat");
    if(!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QDataStream ds(&file);   // we will serialize the data into the file
    for(int i = 0; i < this->metaObject()->propertyCount(); ++i) {
        QMetaProperty p = this->metaObject()->property(i);
        if(p.isStored(this)) {
            ds << this->property(p.name());
        }
    }
    return true;
}

bool QVestaUpdater::deserialize() {
    QFile file("settings.dat");
    if(!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open settings file.");
        return false;
    }
    QDataStream ds(&file);   // we will read data from file
    for(int i = 0; i < this->metaObject()->propertyCount(); ++i) {
        QMetaProperty p = this->metaObject()->property(i);
        if(p.isStored(this)) {
            QVariant v;
            ds >> v;
            this->setProperty(p.name(), v);
        }
    }
    return true;
}

//-------------- Setters and Getters below -------------------------------
void QVestaUpdater::setPathToVestaInstaller(QString path)
{ pathToVestaInstaller = path; }

void QVestaUpdater::setLatestVersion(QDateTime dateTime)
{ latestVersion = dateTime; }

void QVestaUpdater::setInstalledVersion(QDateTime dateTime)
{ installedVersion = dateTime; }

QString QVestaUpdater::getPathToVestaInstaller()
{ return pathToVestaInstaller; }

QDateTime QVestaUpdater::getLatestVersion()
{ return latestVersion; }

QDateTime QVestaUpdater::getInstalledVersion()
{ return installedVersion; }
