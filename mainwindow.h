#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>
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
    void on_sendcall_clicked();
    void on_rail_clicked();
    void on_sendtrain_clicked();
};

#endif // MAINWINDOW_H
