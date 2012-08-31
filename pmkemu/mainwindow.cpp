#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmcu13.h"
#include "cmem.h"



const char segments[16]={
    '0','1','2','3','4','5','6','7','8','9',
    '-','L','C','r','E',' '
};



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
   int i;

    ustep=0;
    btnpressed=0;

    ui->setupUi(this);

    btns.clear();
    btns.append(ui->b_1_2);
    btns.append(ui->b_1_3);
    btns.append(ui->b_1_4);
    btns.append(ui->b_1_5);
    btns.append(ui->b_1_6);
    btns.append(ui->b_1_7);
    btns.append(ui->b_1_8);
    btns.append(ui->b_1_9);
    btns.append(ui->b_1_10);
    btns.append(ui->b_1_11);


    btns.append(ui->b_2_2);
    btns.append(ui->b_2_3);
    btns.append(ui->b_2_4);
    btns.append(ui->b_2_5);
    btns.append(ui->b_2_6);
    btns.append(ui->b_2_7);
    btns.append(ui->b_2_8);
    btns.append(ui->b_2_9);
    btns.append(ui->b_2_10);
    btns.append(ui->b_2_11);


    btns.append(ui->b_3_2);
    btns.append(ui->b_3_3);
    btns.append(ui->b_3_4);
    btns.append(ui->b_3_5);
    btns.append(ui->b_3_6);
    btns.append(ui->b_3_7);
    btns.append(ui->b_3_8);
    btns.append(ui->b_3_9);
    btns.append(ui->b_3_10);
    btns.append(ui->b_3_11);


    for(i=0;i<btns.count();i++)
    {
        connect(btns.at(i),SIGNAL(pressed()),this,SLOT(on_keypad_clicked()));
        connect(btns.at(i),SIGNAL(released()),this,SLOT(on_keypad_clicked()));
    }

   // QPushButton::pressed()

    display_blank=0;

    for(i=0;i<12;i++)
        display[i]=0x0f;

    display[7]=0x80;


    emu=new emulator(this);

    connect(emu,SIGNAL(on_sync(QByteArray*)),this,SLOT(on_sync(QByteArray*)));
    connect(this,SIGNAL(step(int)),emu,SLOT(step(int)));
    connect(this,SIGNAL(keypad(uint)),emu,SLOT(keypad(uint)));
    connect(this,SIGNAL(enable(bool)),emu,SLOT(enable(bool)));
    connect(this,SIGNAL(set_mode(mode_e)),emu,SLOT(set_mode(mode_e)));
    connect(this,SIGNAL(debug(bool)),emu,SLOT(debug(bool)));
    connect(ui->runCheck,SIGNAL(clicked()),this,SLOT(on_run_clicked()));
    connect(ui->debugCheck,SIGNAL(clicked()),this,SLOT(on_debug_clicked()));
    emu->start();

    ui->modeSlider->setValue(1);
    connect(ui->modeSlider,SIGNAL(valueChanged(int)),this,SLOT(on_mode_changed(int)));
    emit set_mode(e_rad);


    timer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(OnTimer()));
    //timer->start(25);
    ui->lcdNumber->display("-8.Err0r  -99");

    ui->gutsSplitter->setStretchFactor(0, 10);
    ui->gutsSplitter->setStretchFactor(1, 1);
    ui->mainToolBar->hide();
    ui->menuBar->setVisible(false);
    ui->statusBar->setVisible(false);
    this->layout()->setSizeConstraint(QLayout::SetFixedSize);
}



MainWindow::~MainWindow()
{

    emu->enable(false);
    disconnect(this,SLOT(on_sync(QByteArray*)));
    timer->stop();
    emu->deleteLater();
    //delete emu;
    timer->deleteLater();
    //delete timer;
    delete ui;
}

void MainWindow::on_debug_clicked()
{
    emit debug(ui->debugCheck->isChecked());
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

void MainWindow::on_sync(QByteArray *disp)
{
    for(int i=0;i<disp->count();i++)
        display[i]=disp->at(i);
    //int i;
    //for(i=0;i<12;i++)
      //  display[i]=disp[i];
    updatedisp();
    display_blank=1;
}

void MainWindow::on_mode_changed(int i)
{
    switch(ui->modeSlider->value())
    {
        case 1:
            emit set_mode(e_rad);
            break;
        case 2:
            emit set_mode(e_grd);
            break;
        case 3:
            emit set_mode(e_deg);
            break;
    }
}

void MainWindow::OnTimer()
{
    if(display_blank)
    {
        display_blank--;
        updatedisp();
    }
    else
        ui->lcdNumber->display("");
  #if 0
    switch(ui->modeSlider->value())
    {
        case 1:
            mode=e_rad;
            break;
        case 2:
            mode=e_grd;
            break;
        case 3:
            mode=e_deg;
            break;
    }
    #endif

#if 0
    //updatedisp();
    ui->ik1302_d->setText(QString().sprintf("d=%d %d   ",ik1302->dcount,dcycle));
    ui->ik1302_e->setText(QString().sprintf("e=%d   ",ik1302->ecount));
    ui->ik1302_i->setText(QString().sprintf("i=%d   ",ik1302->icount));
    ui->ik1302_sy->setText(QString().sprintf("sync= %s  ",sync?"T":" "));
    ui->ic1302_cp->setText(QString().sprintf("pc=%2.2X    ",ik1302->cptr));

    ui->ik1303_d->setText(QString().sprintf("d=%d   ",ik1303->dcount));
    ui->ik1303_e->setText(QString().sprintf("e=%d   ",ik1303->ecount));
    ui->ik1303_i->setText(QString().sprintf("i=%d   ",ik1303->icount));
    ui->ic1303_cp->setText(QString().sprintf("pc=%2.2X    ",ik1303->cptr));


    ui->ik1306_d->setText(QString().sprintf("d=%d   ",ik1306->dcount+1));
    ui->ik1306_e->setText(QString().sprintf("e=%d   ",ik1306->ecount+1));
    ui->ik1306_i->setText(QString().sprintf("i=%d   ",ik1306->icount));
#endif

}

void MainWindow::on_ustepBtn_clicked()
{
    emit step(1);
}

void MainWindow::on_istepBtn_clicked()
{
    emit step(4);
}

void MainWindow::on_cycleBtn_clicked()
{
    emit step(4*42);
}

void MainWindow::on_keypad_clicked()
{
    int i;
    //cycle

    //if(btnpressed) //one button is still processing
      //  return;

    for(i=0;i<btns.count();i++)
    {
        if(btns.at(i)->isDown())
        {
            btnpressed=(i/10)+1;
            btnpressed<<=8;
            btnpressed|=i%10;
            ui->ik1302_d->setText(QString().sprintf("d=%x  ",btnpressed));
            emit keypad(btnpressed);
            return;
        }
    }
    btnpressed=0;
    emit keypad(0);
}

void MainWindow::on_run_clicked()
{
    emit enable(ui->runCheck->isChecked());
}

void MainWindow::on_showVariables_toggled(bool checked)
{
    ui->gutsSplitter->setVisible(checked);
    ui->centralWidget->updateGeometry();
}
