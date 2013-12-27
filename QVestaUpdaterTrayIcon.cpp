#include "QVestaUpdaterTrayIcon.h"

QVestaUpdaterTrayIcon::QVestaUpdaterTrayIcon(QObject *parent) :
    QObject(parent)
{
    trayIcon = new QSystemTrayIcon(parent);
    setIconUpToDate();
    trayIcon->show();
}

void QVestaUpdaterTrayIcon::setMenu(QMenu *menu) {
    trayIcon->setContextMenu(menu);
}

void QVestaUpdaterTrayIcon::setIconUpToDate()
{ trayIcon->setIcon(QIcon(":/img/VestaIcon.png")); }

void QVestaUpdaterTrayIcon::setIconWaitForUpdate()
{ trayIcon->setIcon(QIcon(":/img/VestaIconUpdateReady")); }

void QVestaUpdaterTrayIcon::showNewVersionReleased(QDateTime dt) {
    setIconWaitForUpdate();
    trayIcon->showMessage("Доступно обновление",
                          "Новая версия от " + dt.toString() );
}

void QVestaUpdaterTrayIcon::showNewVersionInstalled(QDateTime dt) {
    setIconUpToDate();
    trayIcon->showMessage("Установлена последняя версия",
                          "Последняя версия от " + dt.toString() );
}
