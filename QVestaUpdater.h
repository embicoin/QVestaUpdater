#ifndef QVESTAUPDATER_H
#define QVESTAUPDATER_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QTimer>
#include <QStateMachine>

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
    virtual ~QVestaUpdater();
    QString   getPathToVestaInstaller();
    QDateTime getLatestVersion();
    QDateTime getInstalledVersion();

public slots:
    bool serialize();
    bool deserialize();

    void startTimer();
    void stopTimer();

    void setPathToVestaInstaller(QString);
    void setLatestVersion(QDateTime);
    void setInstalledVersion(QDateTime);

signals:
// Used by machine:
    void upToDate        (QDateTime version         );
    void updateReady     (QDateTime newVersion      );
    void installStarted  (QDateTime installedVersion);
    void installFinished (QDateTime installedVersion);
    void waitVestaExit   (                          );
    void vestaRunning    (                          );
    void vestaNotRunning (                          );
// on state enter:
    void enterInitialCheck  (                          );
    void enterUpToDate      (QDateTime version         );
    void enterUpdateReady   (QDateTime newVersion      );
    void enterInstallBegin  (QDateTime installedVersion);
    void enterWaitVestaExit (                          );

private:
    QStateMachine machine;

    QTimer   *timer;
    QString   pathToVestaInstaller;
    QDateTime latestVersion;
    QDateTime installedVersion;

    void checkVestaRunningWithTasklist();

private slots:
// state enter handlers:
    void onEnterInitialCheck  ();
    void onEnterUpToDate      ();
    void onEnterUpdateReady   ();
    void onEnterInstallBegin  ();
    void onEnterWaitVestaExit ();

    void setupMachine();
    void checkStatus();
    void checkVestaRunning();
    void doInstall();
    void installLatestVersionThread();
    QDateTime checkLatestVersionInDropbox();
};

#endif // QVESTAUPDATER_H
