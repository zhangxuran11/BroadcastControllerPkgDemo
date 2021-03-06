#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"broadcastmanager.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    broadcastManager_r = new BroadcastManager("/dev/ttymxc2");
    broadcastManager_r->speed = 10;
    broadcastManager_r->trainId =23;
    broadcastManager_r->carId =07;
    broadcastManager_r->startStationEN =   "shanghaibeijingtianjin";
    broadcastManager_r->startStationThai = "นึ่รู้รู้รู้รู้รู้รู้นึ่นึ่นึ่นึ่รู้นึ่";
    broadcastManager_r->endStationEN =     "guangdongshanghaibeiji";
    broadcastManager_r->endStationThai = "นึ่รู้รู้นึ่";
    broadcastManager_r->dateTime = QDateTime::currentDateTime();

//        broadcastManager_r->startStationEN =   "shanghai";
//        broadcastManager_r->startStationThai = "นึ่รู้รู้รู้รู้รู้รู้";
//        broadcastManager_r->endStationEN =     "guaeiji";
//        broadcastManager_r->endStationThai = "นึ่รู้รู้รู้รู้รู้รู้รู้นึ่";

    //broadcastManager_s = new BroadcastManager("/dev/ttyUSB0");
    connect(broadcastManager_r,SIGNAL(readyRead()),this,SLOT(fun()));
    broadcastManager_r->start();
}
void MainWindow::fun()
{
    CallStateParser callState = broadcastManager_r->getOneCallState();
    if(callState.hasBroadcast_lc)
    {
        int speed = ui->train_sp__line->text().toInt()+1;
        if(speed >=40 )
            speed = 0;
        else
            speed++;
        ui->train_sp__line->setText(QString::number(speed));
    }
    callState.print();
}
MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_sendcall_clicked()
{
    CallStateParser call;
    call.hasBroadcast = ui->hasBroadcastCall->checkState() == Qt::Checked ? true:false;
    call.hasBroadcast_lc = ui->hasBroadcastCall_LC->checkState() == Qt::Checked ? true:false;
    call.hasCall1 = ui->hasCall1->checkState() == Qt::Checked ? true:false;
    call.hasCall2 = ui->hasCall2->checkState() == Qt::Checked ? true:false;
    call.hasCall3 = ui->hasCall3->checkState() == Qt::Checked ? true:false;
    call.hasCall4 = ui->hasCall4->checkState() == Qt::Checked ? true:false;
    broadcastManager_s->sendBroadcastPkg(call);
}

void MainWindow::on_rail_clicked()
{
    RailwayStateParser rail;
    //rail.startStationEN = ui->rail_s_en_line->text();
    //rail.startStationThai = ui->rail_s_th_line->text();
    //rail.endStationEN = ui->rail_e_en_line->text();
    //rail.endStationThai = ui->rail_e_th_line->text();
    broadcastManager_s->sendBroadcastPkg(rail);
    rail.print();
}

void MainWindow::on_sendtrain_clicked()
{
    TrainStateParser train;
    train.trainID = ui->train_trainID__line->text().toInt();
    train.carID = ui->train_carID__line->text().toInt();
    train.speed = ui->train_sp__line->text().toInt();
    broadcastManager_s->sendBroadcastPkg(train);
    train.print();

}
