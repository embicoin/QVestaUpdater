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

public slots:
    void setMenu(QMenu*);
    void showNewVersionReleased(QDateTime);
    void showNewVersionInstalled(QDateTime);

private:
    QSystemTrayIcon *trayIcon;
    void setIconUpToDate();
    void setIconWaitForUpdate();
};

#endif // QVESTAUPDATERTRAYICON_H
