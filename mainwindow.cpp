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
    setWindowTitle("Контроль установленной версии Весты");

    trayIcon = new QVestaUpdaterTrayIcon(this);
    trayIcon->setMenu(ui->menuVestaUpdater);

    performConnections();

    vu.deserialize();
}

void MainWindow::performConnections() {
    //---------------------- Меню --------------------------------
    //QObject::connect(ui->exitAction, SIGNAL(triggered()),
    //                 this, SLOT(close()) );
    //QObject::connect(ui->toggleWindowVisibilityAction, SIGNAL(triggered()),
    //                 this, SLOT(toggleVisibility()) );
    //QObject::connect(ui->checkForNewVersionAction, SIGNAL(triggered()),
    //                 &vu, SLOT(checkLatestVersionInDropbox()) );
    //QObject::connect(ui->installNewVersionAction, SIGNAL(triggered()),
    //                 &vu, SLOT(installLatestVersionIfReady() ));


    //---------------------- Кнопки ------------------------------
    //QObject::connect(ui->checkLatestVersionButton, SIGNAL(clicked()),
    //                 &vu, SLOT(checkLatestVersionInDropbox()) );
    //QObject::connect(ui->installLatestVersionButton, SIGNAL(clicked()),
    //                 &vu, SLOT(installLatestVersionIfReady()) );

    //---------------------- Метки -------------------------------
    /*QObject::connect(&vu, SIGNAL(latestVersionUpdated(QString)),
                    ui->latestVersionLabel, SLOT(setText(QString)) );
    QObject::connect(&vu, SIGNAL(installedVersionUpdated(QString)),
                     ui->installedVersionLabel, SLOT(setText(QString)) );
    QObject::connect(&vu, SIGNAL(installedVersionUpdated(QDateTime)),
                     this, SLOT(fillReadyToUpdateLabel()) );
    QObject::connect(&vu, SIGNAL(latestVersionUpdated(QDateTime)),
                     this, SLOT(fillReadyToUpdateLabel()) );

    //---------------------- Иконка в трее -----------------------
    QObject::connect(&vu, SIGNAL(latestVersionUpdated(QDateTime)),
                     trayIcon, SLOT(showNewVersionReleased(QDateTime)) );
    QObject::connect(&vu, SIGNAL(installedVersionUpdated(QDateTime)),
                     trayIcon, SLOT(showNewVersionInstalled(QDateTime)) );
    */


    /*QObject::connect(ui->pathToInstalerEdit, &(QLineEdit::textEdited),
                    &vu, &(VestaUpdater::setPathToVestaInstaller) );
    QObject::connect(ui->pathToInstalerEdit, &(QLineEdit::editingFinished),
                    &vu, &(VestaUpdater::serialize ) );*/
}

void MainWindow::toggleVisibility() {
    if(this->isHidden()) {
        this->show();
    } else {
        this->hide();
    }
}

void MainWindow::fillReadyToUpdateLabel() {
/*    ui->readyToUpdateLabel->setText(
        vu.needToUpdate() ?
            "<p style=\"color:red\">Есть обновление!</p>" :
            "<p style=\"color:green\">Уставновлена последняя версия</p>"
    );*/
}

MainWindow::~MainWindow()
{
    vu.serialize();
    delete ui;
}
