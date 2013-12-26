#include "VestaUpdater.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QFileInfo>
#include <QString>
#include <QMetaProperty>
#include <QVariant>
#include <QDebug>
#include <QProcess>

VestaUpdater::VestaUpdater()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),
            this, SLOT(installLatestVersionIfReady()) );
    startTimer();
}

void VestaUpdater::startTimer()
{ timer->start(60 * 1000); }

void VestaUpdater::stopTimer()
{ timer->stop(); }

bool VestaUpdater::needToUpdate() {
    return getLatestVersion() > getInstalledVersion().addSecs(60);
}

void VestaUpdater::checkLatestVersionInDropbox() {
    //QFileInfo fileInfo("C:/Users/1/Dropbox/VestaLite (2)/VestaDizLite.zip");
    QFileInfo fileInfo("C:/Temp/VestaDizLite.zip");
    QDateTime dropboxVersion = fileInfo.lastModified();
    if (dropboxVersion > getLatestVersion()) {
        setLatestVersion(dropboxVersion);
    }
}

bool VestaUpdater::installLatestVersion() {
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

bool VestaUpdater::installLatestVersionIfReady() {
    checkLatestVersionInDropbox();
    if(!needToUpdate()) {
        return false;
    }
    installLatestVersion();
    return true;
}

// ----------------------- Serialization ---------------------------------
bool VestaUpdater::serialize() {
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

bool VestaUpdater::deserialize() {
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
void VestaUpdater::setPathToVestaInstaller(QString path)
{ pathToVestaInstaller = path; }

void VestaUpdater::setLatestVersion(QDateTime dateTime) {
    latestVersion = dateTime;
    emit latestVersionUpdated(dateTime);
    emit latestVersionUpdated(dateTime.toString("dd.MM.yyyy hh:mm"));
}

void VestaUpdater::setInstalledVersion(QDateTime dateTime) {
    installedVersion = dateTime;
    emit installedVersionUpdated(dateTime);
    emit installedVersionUpdated(dateTime.toString("dd.MM.yyyy hh:mm"));
}

QString VestaUpdater::getPathToVestaInstaller()
{ return pathToVestaInstaller; }

QDateTime VestaUpdater::getLatestVersion()
{ return latestVersion; }

QDateTime VestaUpdater::getInstalledVersion()
{ return installedVersion; }
