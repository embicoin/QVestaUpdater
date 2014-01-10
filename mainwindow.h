#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QIcon>
#include "QVestaUpdater.h"
#include "QVestaUpdaterTrayIcon.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void toggleVisibility();

    void initInterface();
    void onUpToDate();
    void onUpdateReady();
    void onWaitVestaExit();
    void onInstallStarted();
    void onVestaRunning();
    void onVestaNotRunning();
		void onBalloonClicked();

private:
    void performConnections();
    const QString kDateTimeFormat;

    Ui::MainWindow *ui;
    QVestaUpdater vu;
    QVestaUpdaterTrayIcon *trayIcon;
    QIcon *vestaIcon;
};

#endif // MAINWINDOW_H
