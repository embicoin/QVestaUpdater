#ifndef QVESTAUPDATERTRAYICON_H
#define QVESTAUPDATERTRAYICON_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QDateTime>
#include <QMenu>

class QVestaUpdaterTrayIcon : public QObject
{
    Q_OBJECT
public:
    explicit QVestaUpdaterTrayIcon(QObject *parent = 0);

signals:
	void iconDoubleClicked();
	void balloonClicked();
public slots:
    void setMenu(QMenu*);
    void onUpToDate(QDateTime);
    void onInstallStarted(QDateTime);
    void onInstallFinished(QDateTime);
    void onWaitVestaExit();

private:
    const QString kDateFormat;
    QSystemTrayIcon *trayIcon;
    void setIconUpToDate();
    void setIconWaitForUpdate();
private slots:
	void onIconActivated(QSystemTrayIcon::ActivationReason reason);
};

#endif // QVESTAUPDATERTRAYICON_H
