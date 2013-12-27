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

QVestaUpdater::QVestaUpdater()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(installLatestVersionIfReady()) );
    startTimer();
}

void QVestaUpdater::startTimer()
{ timer->start(60 * 1000); }

void QVestaUpdater::stopTimer()
{ timer->stop(); }

bool QVestaUpdater::needToUpdate() {
    return getLatestVersion() > getInstalledVersion().addSecs(60);
}

void QVestaUpdater::checkLatestVersionInDropbox() {
    //QFileInfo fileInfo("C:/Users/1/Dropbox/VestaLite (2)/VestaDizLite.zip");
    QFileInfo fileInfo("C:/Temp/VestaDizLite.zip");
    QDateTime dropboxVersion = fileInfo.lastModified();
    if (dropboxVersion > getLatestVersion()) {
        setLatestVersion(dropboxVersion);
    }
}

bool QVestaUpdater::installLatestVersion() {
    // Вызываем скрипт, который должен
    //   создать папку VestaDizLite в каталоге приложения,
    //   разархивировать туда архив VestaDizLite.zip из Dropbox,
    //   запустиь инсталлятор в silent режиме.
    QProcess *unzipScript = new QProcess(this);
    QString unzipCommand = "VestaDizLiteCopyAndUnzip.bat";
    unzipScript->start(unzipCommand);

    setInstalledVersion(getLatestVersion());
    return true;
}

bool QVestaUpdater::installLatestVersionIfReady() {
    checkLatestVersionInDropbox();
    if(!needToUpdate()) {
        return false;
    }
    installLatestVersion();
    return true;
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

void QVestaUpdater::setLatestVersion(QDateTime dateTime) {
    latestVersion = dateTime;
    emit latestVersionUpdated(dateTime);
    emit latestVersionUpdated(dateTime.toString("dd.MM.yyyy hh:mm"));
}

void QVestaUpdater::setInstalledVersion(QDateTime dateTime) {
    installedVersion = dateTime;
    emit installedVersionUpdated(dateTime);
    emit installedVersionUpdated(dateTime.toString("dd.MM.yyyy hh:mm"));
}

QString QVestaUpdater::getPathToVestaInstaller()
{ return pathToVestaInstaller; }

QDateTime QVestaUpdater::getLatestVersion()
{ return latestVersion; }

QDateTime QVestaUpdater::getInstalledVersion()
{ return installedVersion; }
