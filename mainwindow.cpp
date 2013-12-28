#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QPixmap>
#include <QProcess>
#include <QDateTime>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include "QVestaUpdaterTrayIcon.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    setWindowTitle("QVestaUpdater v0.1");

    trayIcon = new QVestaUpdaterTrayIcon(this);
    trayIcon->setMenu(ui->menuVestaUpdater);

    performConnections();
    initInterface();
}

void MainWindow::performConnections() {
    //---------------------- Меню --------------------------------
    QObject::connect(ui->exitAction, SIGNAL(triggered()),
                     this, SLOT(close()) );
    QObject::connect(ui->toggleWindowVisibilityAction, SIGNAL(triggered()),
                     this, SLOT(toggleVisibility()) );
    QObject::connect(ui->checkForNewVersionAction, SIGNAL(triggered()),
                     &vu, SLOT(checkStatus()) );

    //---------------------- Кнопки ------------------------------
    QObject::connect(ui->checkLatestVersionButton, SIGNAL(clicked()),
                     &vu, SLOT(checkStatus()) );

    //---------------------- Иконка в трее -----------------------
    QObject::connect(&vu,       SIGNAL(upToDate         (QDateTime)),
                     trayIcon,  SLOT  (onUpToDate       (QDateTime))    );
    QObject::connect(&vu,       SIGNAL(installStarted   (QDateTime)),
                     trayIcon,  SLOT  (onInstallStarted (QDateTime))    );
    QObject::connect(&vu,       SIGNAL(installFinished  (QDateTime)),
                     trayIcon,  SLOT  (onInstallFinished(QDateTime))    );
    QObject::connect(&vu,       SIGNAL(waitVestaExit    (         )),
                     trayIcon,  SLOT  (onWaitVestaExit  (         ))    );

    //----------------- Обработка сигналов QVestaUpdater --------------------
    QObject::connect(&vu,       SIGNAL(upToDate         (QDateTime)),
                     this,      SLOT  (onUpToDate       (         ))    );
    QObject::connect(&vu,       SIGNAL(updateReady      (QDateTime)),
                     this,      SLOT  (onUpdateReady    (         ))    );
    QObject::connect(&vu,       SIGNAL(installStarted   (QDateTime)),
                     this,      SLOT  (onInstallStarted (         ))    );
    QObject::connect(&vu,       SIGNAL(waitVestaExit    (         )),
                     this,      SLOT  (onWaitVestaExit  (         ))    );
    QObject::connect(&vu,       SIGNAL(vestaRunning     (         )),
                     this,      SLOT  (onVestaRunning   (         ))    );
    QObject::connect(&vu,       SIGNAL(vestaNotRunning  (         )),
                     this,      SLOT  (onVestaNotRunning(         ))    );

    //----------------------- Настройки --------------------------
    /*QObject::connect(ui->pathToInstalerEdit, &(QLineEdit::textEdited),
                    &vu, &(VestaUpdater::setPathToVestaInstaller) );
    QObject::connect(ui->pathToInstalerEdit, &(QLineEdit::editingFinished),
                    &vu, &(VestaUpdater::serialize ) );*/
}

void MainWindow::toggleVisibility() {
    if(this->isHidden()) { this->show(); } else { this->hide(); }
}

void MainWindow::initInterface() {
    ui->installedVersionLabel->setText(vu.getInstalledVersion().toString());
    ui->latestVersionLabel->setText(vu.getLatestVersion().toString());
}

void MainWindow::onUpToDate() {
    ui->installedVersionLabel->setText(vu.getInstalledVersion().toString());
    ui->statusLabel->setText(
                "<p style=\"color:green\">Установлена последняя версия</p>");
}

void MainWindow::onUpdateReady() {
    ui->latestVersionLabel->setText(vu.getLatestVersion().toString());
    ui->statusLabel->setText(
                "<p style=\"color:orange\">Доступно обновление</p>");
}

void MainWindow::onInstallStarted() {
    ui->statusLabel->setText(
                "<p style=\"color:orange\">Идёт установка новой версии</p>");
}

void MainWindow::onWaitVestaExit() {
    ui->statusLabel->setText(
                "<p style=\"color:red\">Ожидание завершения Весты</p>");
}

void MainWindow::onVestaRunning() {
    ui->isVestaRunningLabel->setText(
                "<p style=\"color:orange\">Веста работает</p>");
}

void MainWindow::onVestaNotRunning() {
    ui->isVestaRunningLabel->setText(
                "<p style=\"color:green\">Веста не работает</p>");
}

MainWindow::~MainWindow()
{
    delete ui;
}
