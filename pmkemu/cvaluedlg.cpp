#include "cvaluedlg.h"
#include "ui_cvaluedlg.h"

cValueDlg::cValueDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cValueDlg)
{
    ui->setupUi(this);
}

cValueDlg::~cValueDlg()
{
    delete ui;
}

void cValueDlg::on_cancelBtn_clicked()
{

}

void cValueDlg::on_okBtn_clicked()
{

}
