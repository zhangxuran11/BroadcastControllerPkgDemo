#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"broadcastmanager.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    broadcastManager_r = new BroadcastManager("/dev/ttyUSB0");
    broadcastManager_s = new BroadcastManager("/dev/ttyUSB1");
    connect(broadcastManager_r,SIGNAL(readyRead()),this,SLOT(fun()));
}
void MainWindow::fun()
{
    CallStateParser callState = broadcastManager_r->getOneCallState();
    callState.print();
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    CallStateParser call;
    call.hasBroadcast = ui->hasBroadcastCall->checkState() == Qt::Checked ? true:false;
    call.hasCall1 = ui->hasCall1->checkState() == Qt::Checked ? true:false;
    call.hasCall2 = ui->hasCall2->checkState() == Qt::Checked ? true:false;
    call.hasCall3 = ui->hasCall3->checkState() == Qt::Checked ? true:false;
    call.hasCall4 = ui->hasCall4->checkState() == Qt::Checked ? true:false;
    broadcastManager_s->sendBroadcastPkg(call);
}
