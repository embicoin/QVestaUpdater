#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QIcon>
#include "VestaUpdater.h"
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
    void fillReadyToUpdateLabel();
    void toggleVisibility();

private:
    void performConnections();

    Ui::MainWindow *ui;
    VestaUpdater vu;
    QVestaUpdaterTrayIcon *trayIcon;
    QIcon *vestaIcon;
};

#endif // MAINWINDOW_H
