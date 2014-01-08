#include "QVestaUpdaterTrayIcon.h"

QVestaUpdaterTrayIcon::QVestaUpdaterTrayIcon(QObject *parent) :
    QObject(parent), kDateFormat("dd.MM, hh:mm")
{
    trayIcon = new QSystemTrayIcon(parent);
    setIconUpToDate();
    trayIcon->show();
}

void QVestaUpdaterTrayIcon::setMenu(QMenu *menu) {
    trayIcon->setContextMenu(menu);
}

//---------------------- Обработка сигналов VestaUpdater --------------------
void QVestaUpdaterTrayIcon::onUpToDate(QDateTime dt) {
    setIconUpToDate();
}

void QVestaUpdaterTrayIcon::onInstallStarted(QDateTime dt) {
    setIconWaitForUpdate();
    trayIcon->showMessage("Начата установка",
                          "Версия от " + dt.toString(kDateFormat));
}

void QVestaUpdaterTrayIcon::onInstallFinished(QDateTime dt) {
    trayIcon->showMessage("Установлена последняя версия",
                          "Версия от " + dt.toString(kDateFormat));
    onUpToDate(dt);
}

void QVestaUpdaterTrayIcon::onWaitVestaExit() {
    setIconWaitForUpdate();
    trayIcon->showMessage("Ожидается закрытие Весты",
                          "Установка отложена");
}

//---------------------------------------------------------------------
void QVestaUpdaterTrayIcon::setIconUpToDate()
{ trayIcon->setIcon(QIcon(":/img/VestaIcon.png")); }

void QVestaUpdaterTrayIcon::setIconWaitForUpdate()
{ trayIcon->setIcon(QIcon(":/img/VestaIconUpdateReady")); }
