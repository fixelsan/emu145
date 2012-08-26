#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmcu13.h"
#include "cmem.h"

#include "ucommands.h"
#include "synchro.h"
#include "mcommands.h"

const char segments[16]={
    '0','1','2','3','4','5','6','7','8','9',
    '-','L','C','r','E',' '
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    int i,j;

    ustep=0;

    ui->setupUi(this);

    for(i=0;i<12;i++)
        display[i]=0x0f;

    display[7]=0x80;

    ik1302=new cMCU(this, "IK1302", true);
    ik1303=new cMCU(this, "IK1303");
    ik1306=new cMCU(this, "IK1306");
    ir2_1=new cMem();
    ir2_2=new cMem();

    //load memory
    for(i=0;i<68;i++)
        ik1302->ucrom[i].raw=ik1302_urom[i];
    for(i=0;i<128;i++)
        for(j=0;j<9;j++)
            ik1302->asprom[i][j]=ik1302_srom[i][j];
    for(i=0;i<256;i++)
        ik1302->cmdrom[i]=ik1302_mrom[i];


    ik1302->init();
    ik1303->init();
    ik1306->init();


    chain=false;
    sync=false;
    olddcycle=0;
    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
    timer->start(10);
    ui->lcdNumber->display("-8.Err0r  -99");


}

MainWindow::~MainWindow()
{
    timer->stop();
    delete timer;
    delete ik1302;
    delete ik1303;
    delete ik1306;
    delete ir2_1;
    delete ir2_2;
    delete ui;
}

void MainWindow::updatedisp()
{
    QString disp;

    int i;

    disp="";
    for(i=0;i<9;i++)
    {
        disp+=segments[display[8-i]&0xf];
        if(display[8-i]&0x80)
            disp+=".";
    }
    for(i=0;i<3;i++)
    {
        disp+=segments[display[11-i]&0xf];
        if(display[11-i]&0x80)
            disp+=".";
    }
    ui->lcdNumber->display(disp);

}

void MainWindow::OnTimer()
{
    //unsigned int dcycle;
    unsigned char seg;
    bool k1;
    bool k2;


    k1=false;
    k2=false;
    if(ui->runCheck->isChecked()==false)
    {
        if(ustep==0)
            return;
        ustep--;
    }

    if(dcycle==13)
        k2=true;


    chain=ik1302->tick(chain,k1,k2,&dcycle,&sync,&seg);
#if 1
    if((dcycle>1)&&(dcycle<14))
        display[dcycle-2]=seg;


#endif

    if(sync)
    {
        updatedisp();
    }

    //chain=ik1303->tick(chain,false,false,NULL,NULL,NULL);
    //chain=ik1306->tick(chain,false,false,NULL,NULL,NULL);
    chain=ir2_1->tick(chain);
    chain=ir2_2->tick(chain);

    ik1302->pretick(chain);

    ui->ik1302_d->setText(QString().sprintf("d=%d %d   ",ik1302->dcount+1,dcycle));
    ui->ik1302_e->setText(QString().sprintf("e=%d   ",ik1302->ecount+1));
    ui->ik1302_i->setText(QString().sprintf("i=%d   ",ik1302->icount));
    ui->ik1302_sy->setText(QString().sprintf("sync= %s  ",sync?"T":" "));

    ui->ik1303_d->setText(QString().sprintf("d=%d   ",ik1303->dcount+1));
    ui->ik1303_e->setText(QString().sprintf("e=%d   ",ik1303->ecount+1));
    ui->ik1303_i->setText(QString().sprintf("i=%d   ",ik1303->icount));

    ui->ik1306_d->setText(QString().sprintf("d=%d   ",ik1306->dcount+1));
    ui->ik1306_e->setText(QString().sprintf("e=%d   ",ik1306->ecount+1));
    ui->ik1306_i->setText(QString().sprintf("i=%d   ",ik1306->icount));
}

void MainWindow::on_ustepBtn_clicked()
{
    ustep=1;
}

void MainWindow::on_istepBtn_clicked()
{
    ustep=4;
}

void MainWindow::on_cycleBtn_clicked()
{
    ustep=4*42;
}
