#ifndef VESTAUPDATER_H
#define VESTAUPDATER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QTimer>

class VestaUpdater : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString pathToVestaInstaller
        READ   getPathToVestaInstaller
        WRITE  setPathToVestaInstaller
        STORED true)
    Q_PROPERTY(QDateTime latestVersion
        READ   getLatestVersion
        WRITE  setLatestVersion
        STORED true)
    Q_PROPERTY(QDateTime installedVersion
        READ   getInstalledVersion
        WRITE  setInstalledVersion
        STORED true)
public:
    VestaUpdater();
    QString   getPathToVestaInstaller();
    QDateTime getLatestVersion();
    QDateTime getInstalledVersion();

public slots:
    void startTimer();
    void stopTimer();

    void checkLatestVersionInDropbox();    
    bool installLatestVersionIfReady();

    void setPathToVestaInstaller(QString);
    void setLatestVersion(QDateTime);
    void setInstalledVersion(QDateTime);

    bool serialize();
    bool deserialize();
    bool needToUpdate();
signals:
    void latestVersionUpdated(QDateTime);
    void latestVersionUpdated(QString);
    void installedVersionUpdated(QDateTime);
    void installedVersionUpdated(QString);

private:
    QTimer *timer;
    bool installLatestVersion();
    QString   pathToVestaInstaller;
    QDateTime latestVersion;
    QDateTime installedVersion;
};

#endif // VESTAUPDATER_H
