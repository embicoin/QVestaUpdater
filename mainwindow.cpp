#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QPixmap>
#include <QProcess>
#include <QDateTime>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QUrl>
#include "QVestaUpdaterTrayIcon.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    kDateTimeFormat("dd.MM, hh:mm"),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);    
    setWindowTitle("QVestaUpdater v0.2");

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
		QObject::connect(ui->startVestaAction, SIGNAL(triggered()),
										 &vu, SLOT(startVesta()) );

    //---------------------- Кнопки ------------------------------
    QObject::connect(ui->checkLatestVersionButton, SIGNAL(clicked()),
                     &vu, SLOT(checkStatus()) );

    //---------------------- Иконка в трее -----------------------
    QObject::connect(&vu,       SIGNAL(enterUpToDate     (QDateTime)),
                     trayIcon,  SLOT  (onUpToDate        (QDateTime))    );
    QObject::connect(&vu,       SIGNAL(enterInstallBegin (QDateTime)),
                     trayIcon,  SLOT  (onInstallStarted  (QDateTime))    );
    QObject::connect(&vu,       SIGNAL(installFinished   (QDateTime)),
                     trayIcon,  SLOT  (onInstallFinished (QDateTime))    );
    QObject::connect(&vu,       SIGNAL(enterWaitVestaExit(         )),
                     trayIcon,  SLOT  (onWaitVestaExit   (         ))    );
		QObject::connect(
			trayIcon,							SIGNAL(iconDoubleClicked()),
			ui->startVestaAction, SIGNAL(triggered					()) );
		QObject::connect(
			trayIcon,							SIGNAL(balloonClicked   ()),
			this,									SLOT  (onBalloonClicked	()) );

    //----------------- Обработка сигналов QVestaUpdater --------------------
    QObject::connect(&vu,       SIGNAL(enterUpToDate     (QDateTime)),
                     this,      SLOT  (onUpToDate        (         ))    );
    QObject::connect(&vu,       SIGNAL(enterUpdateReady  (QDateTime)),
                     this,      SLOT  (onUpdateReady     (         ))    );
    QObject::connect(&vu,       SIGNAL(enterInstallBegin (QDateTime)),
                     this,      SLOT  (onInstallStarted  (         ))    );
    QObject::connect(&vu,       SIGNAL(enterWaitVestaExit(         )),
                     this,      SLOT  (onWaitVestaExit   (         ))    );
    QObject::connect(&vu,       SIGNAL(vestaRunning      (         )),
                     this,      SLOT  (onVestaRunning    (         ))    );
    QObject::connect(&vu,       SIGNAL(vestaNotRunning   (         )),
                     this,      SLOT  (onVestaNotRunning (         ))    );

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
    ui->installedVersionLabel->setText(
                vu.getInstalledVersion().toString(kDateTimeFormat));
    ui->latestVersionLabel->setText(
                vu.getLatestVersion().toString(kDateTimeFormat));
		
		QFile file("VestaDizLite/svn.history.txt");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			ui->historyText->setPlainText("Can't open history file");
		}
		else {
			ui->historyText->setPlainText( file.readAll() 	);
		}
}

void MainWindow::onBalloonClicked() {
	this->show();
	ui->tabWidget->setCurrentWidget(ui->tabSvn);
}

void MainWindow::onUpToDate() {
    ui->installedVersionLabel->setText(
                vu.getInstalledVersion().toString(kDateTimeFormat));
    ui->statusLabel->setText(
                "<p style=\"color:green\">Установлена последняя версия</p>");
}

void MainWindow::onUpdateReady() {
    ui->latestVersionLabel->setText(
                vu.getLatestVersion().toString(kDateTimeFormat));
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
