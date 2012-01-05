#include "cvaluedlg.h"
#include "ui_cvaluedlg.h"

cValueDlg::cValueDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cValueDlg)
{
    ui->setupUi(this);
    changed=0;
}

cValueDlg::~cValueDlg()
{
    delete ui;
}

void cValueDlg::on_cancelBtn_clicked()
{
    changed=0;
    hide();
}

void cValueDlg::on_okBtn_clicked()
{
    changed=1;
    newval=ui->regValue->text().toInt(NULL,16);
    hide();
}

void cValueDlg::setReg(QString str, unsigned char val)
{
    ui->regText->setText(str);
    ui->regValue->setText(QString().sprintf("%1.1X",val&0xf));
}

