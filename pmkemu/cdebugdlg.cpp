#include "cdebugdlg.h"
#include "ui_cdebugdlg.h"
#include <QTableWidgetItem>
#include <iostream>
#include "cvaluedlg.h"


cDebugDlg::cDebugDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cDebugDlg)
{
    QColor rg;
    QBrush rt;
    int i,j;

    ui->setupUi(this);
    ui->rcmd->setText("CMD: 00 00 00 0");
    ui->rh->setText("H: 0");
    ui->rl->setText("L: 0");
    ui->rpc->setText("PC: 00");
    ui->rs->setText("S: 0");
    ui->rs1->setText("S1: 0");
    ui->ucmdaddr->setText("uCMD: 00");
    ui->ucmddis->setText("uCMD: NOP");
    ui->aspid->setText("ASP: 00");


#if 1
    i=ui->regTable->rowCount();
    j=ui->regTable->columnCount();

    rg.setRgb(200,200,200);

    for(i=0;i<9;i++)
        for(j=0;j<14;j++)
        {
            items[i*14+j]=new QTableWidgetItem(QString().sprintf("%X",(i*14+j)%16));
            items[i*14+j]->setFlags(Qt::ItemIsEnabled);
            ui->regTable->setItem(i,j,items[i*14+j]);
            //ui->regTable->item(i,j)->setBackgroundColor(QColor().setRgb(128,128,128));

            rt=ui->regTable->item(i,j)->background();

            ui->regTable->item(i,j)->setBackgroundColor(rg);

        }
    setI(0);
#endif

    connect(ui->regTable,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(onItemChange(QTableWidgetItem*)));

}

cDebugDlg::~cDebugDlg()
{
    delete ui;
}

void cDebugDlg::onItemChange(QTableWidgetItem *itm)
{
    cValueDlg * dlg=new cValueDlg(this);
    unsigned int i;

    i=itm->text().toInt(NULL,16);

    switch(itm->row())
    {
        case 0:
        case 1:
        case 2:
            dlg->setReg(QString().sprintf("M%d[%d]=",itm->row()+1,itm->column()),i&0xf);
            break;
        case 3:
        case 4:
        case 5:
            dlg->setReg(QString().sprintf("R%d[%d]=",itm->row()+1,itm->column()),i&0xf);
            break;
        case 6:
        case 7:
        case 8:
            dlg->setReg(QString().sprintf("ST%d[%d]=",itm->row()+1,itm->column()),i&0xf);
            break;
    }

    dlg->exec();

    if(dlg->changed)
        itm->setText(QString().sprintf("%1.1X",dlg->newval&0xf));

    delete dlg;

}

void cDebugDlg::setI(unsigned int i)
{
    int r,c,j;
    QColor rg;
#if 1
    rg.setRgb(200,200,200);
    for(j=0;j<14;j++)
    {
        ui->regTable->item(0,j)->setBackgroundColor(rg);
        ui->regTable->item(1,j)->setBackgroundColor(rg);
        ui->regTable->item(2,j)->setBackgroundColor(rg);
        ui->regTable->item(3,j)->setBackgroundColor(rg);
        ui->regTable->item(4,j)->setBackgroundColor(rg);
        ui->regTable->item(5,j)->setBackgroundColor(rg);
        ui->regTable->item(6,j)->setBackgroundColor(rg);
        ui->regTable->item(7,j)->setBackgroundColor(rg);
        ui->regTable->item(8,j)->setBackgroundColor(rg);

    }
    r=i%3;
    c=i/3;
    rg.setRgb(128,255,128);
    ui->regTable->item(0+r,c)->setBackgroundColor(rg);
    ui->regTable->item(3+r,c)->setBackgroundColor(rg);
    ui->regTable->item(6+r,c)->setBackgroundColor(rg);
#endif
}


void cDebugDlg::setH(unsigned char h)
{
    ui->rh->setText(QString().sprintf("H: %1.1X",h&0xf));
}
void cDebugDlg::setL(bool l)
{
    ui->rl->setText(QString().sprintf("L: %c",l?'1':'0'));
}
void cDebugDlg::setPC(unsigned char pc)
{
    ui->rpc->setText(QString().sprintf("PC: %2.2X",pc));
}
void cDebugDlg::setCMD(unsigned int cmd)
{
    ui->rcmd->setText(QString().sprintf("CMD: %2.2X %2.2X %2.2X %c",cmd&0x7f,(cmd>>8)&0x7f,(cmd>>16)&0xff,cmd&0xff000000?'1':'0'));
}
void cDebugDlg::setS(unsigned char s)
{
    ui->rs->setText(QString().sprintf("S: %1.1X",s&0xf));
}
void cDebugDlg::setS1(unsigned char s1)
{
    ui->rs1->setText(QString().sprintf("S1: %1.1X",s1&0xf));
}
void cDebugDlg::setASP(unsigned char asp)
{
    ui->aspid->setText(QString().sprintf("ASP: %2.2X",asp));
}
void cDebugDlg::setUCMD(unsigned char ucmd,QString disassm)
{
    ui->ucmdaddr->setText(QString().sprintf("uCMD: %2.2X",ucmd));
    QString str;

    str="uCMD: "+disassm;
    ui->ucmddis->setText(str);
}

void cDebugDlg::setREGS(bool rm[],bool rr[],bool rst[],unsigned int rlen, unsigned int curi, unsigned int curu)
{
    unsigned char curm;
    unsigned char curr;
    unsigned char curst;
    unsigned int bitcnt;
    unsigned int ptr;
    int r,c;

#if 1
    ptr=0;
    while(curu!=0)
    {
        ptr++;
        curu++;
        if(curu>=4)
        {
            curu=0;
            curi++;
            if(curi>=(rlen/4)){
                curi=0;
            }
        }
        if(ptr>=rlen)
            ptr=0;
    }

    //now ptr is aligned at bit0 of r/m/st next i
    for(bitcnt=0;bitcnt<rlen;bitcnt++)
    {
        if(curu==0)
        {
            curm=0;
            curr=0;
            curst=0;
        }

        curm|=rm[ptr]<<curu;
        curr|=rr[ptr]<<curu;
        curst|=rst[ptr]<<curu;

        curu++;
        if(curu>=4)
        {
            r=curi%3;
            c=curi/3;
            ui->regTable->item(r,c)->setText(QString().sprintf("%1.1X",curm));
            ui->regTable->item(3+r,c)->setText(QString().sprintf("%1.1X",curr));
            ui->regTable->item(6+r,c)->setText(QString().sprintf("%1.1X",curst));
            curu=0;
            curi++;
            if(curi>=(rlen/4))
                curi=0;
        }
        ptr++;
        if(ptr>=rlen)
            ptr=0;
    }

#endif




}
