#ifndef QVESTAUPDATER_H
#define QVESTAUPDATER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QTimer>

class QVestaUpdater : public QObject
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
    QVestaUpdater();
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
    void installProcessStarted();
    void installProcessFinished();
    void latestVersionUpdated(QDateTime);
    void latestVersionUpdated(QString);
    void installedVersionUpdated(QDateTime);
    void installedVersionUpdated(QString);

private:
    bool installLatestVersion();

    QTimer *timer;
    QString   pathToVestaInstaller;
    QDateTime latestVersion;
    QDateTime installedVersion;
};

#endif // QVESTAUPDATER_H
