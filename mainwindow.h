#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class BroadcastManager;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    BroadcastManager* broadcastManager_r;
    BroadcastManager* broadcastManager_s;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
private slots:
    void fun();
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
