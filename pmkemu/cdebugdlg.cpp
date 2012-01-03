#include "cdebugdlg.h"
#include "ui_cdebugdlg.h"
#include <QTableWidgetItem>

cDebugDlg::cDebugDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cDebugDlg)
{
    QColor rg;
    QBrush rt;
    int i,j;

    ui->setupUi(this);

    i=ui->regTable->rowCount();
    j=ui->regTable->columnCount();

    rg.setRgb(200,200,200);

    for(i=0;i<3;i++)
        for(j=0;j<42;j++)
        {
            items[i*42+j]=new QTableWidgetItem(QString().sprintf("%X",(i*42+j)%16));
            items[i*42+j]->setFlags(Qt::ItemIsEnabled);
            ui->regTable->setItem(i,j,items[i*42+j]);
            //ui->regTable->item(i,j)->setBackgroundColor(QColor().setRgb(128,128,128));

            rt=ui->regTable->item(i,j)->background();

            ui->regTable->item(i,j)->setBackgroundColor(rg);

        }
    setI(2);
}

cDebugDlg::~cDebugDlg()
{
    delete ui;
}

void cDebugDlg::setI(unsigned int i)
{
    int j;
    QColor rg;

    rg.setRgb(200,200,200);
    for(j=0;j<42;j++)
    {
        ui->regTable->item(0,j)->setBackgroundColor(rg);
        ui->regTable->item(1,j)->setBackgroundColor(rg);
        ui->regTable->item(2,j)->setBackgroundColor(rg);
    }
    rg.setRgb(128,255,128);
    ui->regTable->item(0,i)->setBackgroundColor(rg);
    ui->regTable->item(1,i)->setBackgroundColor(rg);
    ui->regTable->item(2,i)->setBackgroundColor(rg);

}
