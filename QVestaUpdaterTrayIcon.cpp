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
    trayIcon->setToolTip(
        "Установлена последняя версия от " + dt.toString(kDateFormat));
}

void QVestaUpdaterTrayIcon::onInstallStarted(QDateTime dt) {
    setIconWaitForUpdate();
    trayIcon->setToolTip("Идёт установка новой версии");
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
    trayIcon->setToolTip("Ожидается закрытие Весты");
    trayIcon->showMessage("Ожидается закрытие Весты",
                          "Установка отложена");
}

//---------------------------------------------------------------------
void QVestaUpdaterTrayIcon::setIconUpToDate()
{ trayIcon->setIcon(QIcon(":/img/VestaIcon.png")); }

void QVestaUpdaterTrayIcon::setIconWaitForUpdate()
{ trayIcon->setIcon(QIcon(":/img/VestaIconUpdateReady")); }
