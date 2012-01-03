#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
#if 0
    ui->r_destbox->addItem("---");
    ui->r_destbox->addItem("R[i+3]");
    ui->r_destbox->addItem("sum");
    ui->r_destbox->addItem("S");
    ui->r_destbox->addItem("R[i]|S| sum");
    ui->r_destbox->addItem("S| sum");
    ui->r_destbox->addItem("R[i]|S");
    ui->r_destbox->addItem("R[i]| sum");



    ui->s_destbox->addItem("---");
    ui->s_destbox->addItem("S1");
    ui->s_destbox->addItem("sum");
    ui->s_destbox->addItem("S1|sum");

    ui->s1_destbox->addItem("---");
    ui->s1_destbox->addItem("sum");
    ui->s1_destbox->addItem("S1|H");
    ui->s1_destbox->addItem("S1|H|sum");

    ui->st_destbox->addItem("---");
    ui->st_destbox->addItem("sum");
    ui->st_destbox->addItem("ROL");
    ui->st_destbox->addItem("Compute");
#endif
    connect(ui->st_destbox,SIGNAL(currentIndexChanged(int)),this,SLOT(OnChange()));
    connect(ui->s1_destbox,SIGNAL(currentIndexChanged(int)),this,SLOT(OnChange()));
    connect(ui->s_destbox,SIGNAL(currentIndexChanged(int)),this,SLOT(OnChange()));
    connect(ui->r_destbox,SIGNAL(currentIndexChanged(int)),this,SLOT(OnChange()));

    ui->r_destbox->setCurrentIndex(0);
    ui->s1_destbox->setCurrentIndex(0);
    ui->st_destbox->setCurrentIndex(0);
    ui->s_destbox->setCurrentIndex(0);

    ui->opcodeOut->setText("");
    ui->adderSym->setText("");
    ui->opcodeOut->setReadOnly(true);
    ui->adderSym->setReadOnly(true);

    ui->a_10nlsel->setChecked(false);   connect(ui->a_10nlsel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));
    ui->a_4sel->setChecked(false);connect(ui->a_4sel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));
    ui->a_msel->setChecked(false);connect(ui->a_msel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));
    ui->a_nrsel->setChecked(false);connect(ui->a_nrsel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));
    ui->a_rsel->setChecked(false);connect(ui->a_rsel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));
    ui->a_ssel->setChecked(false);connect(ui->a_ssel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));
    ui->a_stsel->setChecked(false);connect(ui->a_stsel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));

    ui->b_1sel->setChecked(false);connect(ui->b_1sel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));
    ui->b_6sel->setChecked(false);connect(ui->b_6sel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));
    ui->b_nssel->setChecked(false);connect(ui->b_nssel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));
    ui->b_s1sel->setChecked(false);connect(ui->b_s1sel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));
    ui->b_ssel->setChecked(false);connect(ui->b_ssel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));

    ui->g_lsel->setChecked(false);connect(ui->g_lsel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));
    ui->g_nlsel->setChecked(false);connect(ui->g_nlsel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));
    ui->g_ntsel->setChecked(false);connect(ui->g_ntsel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));

    ui->r_i1sel->setChecked(false);connect(ui->r_i1sel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));
    ui->r_i2sel->setChecked(false);connect(ui->r_i2sel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));
    ui->m_isel->setChecked(false);connect(ui->m_isel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));
    ui->l_carsel->setChecked(false);connect(ui->l_carsel,SIGNAL(stateChanged(int)),this,SLOT(OnChange()));

    connect(ui->clearBtn,SIGNAL(clicked()),this,SLOT(OnClear()));
    ucomm.raw=0;
    QString str;
    str.sprintf("0x%7.7X",ucomm.raw);
    ui->opcodeOut->setText(str);
}

void MainWindow::OnChange()
{
    QString str;
    ucomm.raw=0;
    ucomm.bits.a_10nl=ui->a_10nlsel->isChecked();
    ucomm.bits.a_4=ui->a_4sel->isChecked();
    ucomm.bits.a_m=ui->a_msel->isChecked();
    ucomm.bits.a_nr=ui->a_nrsel->isChecked();
    ucomm.bits.a_r=ui->a_rsel->isChecked();
    ucomm.bits.a_s=ui->a_ssel->isChecked();
    ucomm.bits.a_st=ui->a_stsel->isChecked();

    ucomm.bits.b_1=ui->b_1sel->isChecked();
    ucomm.bits.b_6=ui->b_6sel->isChecked();
    ucomm.bits.b_ns=ui->b_nssel->isChecked();
    ucomm.bits.b_s=ui->b_ssel->isChecked();
    ucomm.bits.b_s1=ui->b_s1sel->isChecked();
    ucomm.bits.g_l=ui->g_lsel->isChecked();
    ucomm.bits.g_nl=ui->g_nlsel->isChecked();
    ucomm.bits.g_nt=ui->g_ntsel->isChecked();

    ucomm.bits.r0=ui->r_destbox->currentIndex();
    ucomm.bits.r_1=ui->r_i1sel->isChecked();
    ucomm.bits.r_2=ui->r_i2sel->isChecked();
    ucomm.bits.m=ui->m_isel->isChecked();
    ucomm.bits.l=ui->l_carsel->isChecked();
    ucomm.bits.s=ui->s_destbox->currentIndex();
    ucomm.bits.s1=ui->s1_destbox->currentIndex();
    ucomm.bits.st=ui->st_destbox->currentIndex();
    ucomm.bits.pad=0;

    this->disassm();


}

void MainWindow::OnClear()
{
    ui->r_destbox->setCurrentIndex(0);
    ui->s1_destbox->setCurrentIndex(0);
    ui->st_destbox->setCurrentIndex(0);
    ui->s_destbox->setCurrentIndex(0);

    ui->opcodeOut->setText("");
    ui->adderSym->setText("");

    ui->a_10nlsel->setChecked(false);
    ui->a_4sel->setChecked(false);
    ui->a_msel->setChecked(false);
    ui->a_nrsel->setChecked(false);
    ui->a_rsel->setChecked(false);
    ui->a_ssel->setChecked(false);
    ui->a_stsel->setChecked(false);

    ui->b_1sel->setChecked(false);
    ui->b_6sel->setChecked(false);
    ui->b_nssel->setChecked(false);
    ui->b_s1sel->setChecked(false);
    ui->b_ssel->setChecked(false);

    ui->g_lsel->setChecked(false);
    ui->g_nlsel->setChecked(false);
    ui->g_ntsel->setChecked(false);

    ui->r_i1sel->setChecked(false);
    ui->r_i2sel->setChecked(false);
    ui->m_isel->setChecked(false);
    ui->l_carsel->setChecked(false);
    ucomm.raw=0;
    QString str;
    str.sprintf("0x%7.7X",ucomm.raw);
    ui->opcodeOut->setText(str);
    QClipboard *cb = QApplication::clipboard();
    cb->setText(str);


}

void MainWindow::disassm()
{
    QString cmd;
    QString tmp;

    cmd="";
    tmp="";
    if(ucomm.bits.a_r)
        tmp+="R[i]";
    if(ucomm.bits.a_m)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="M[i]";
    }
    if(ucomm.bits.a_st)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="ST[i]";
    }
    if(ucomm.bits.a_nr)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="~R[i]";
    }
    if(ucomm.bits.a_10nl)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="0xA*!L";
    }
    if(ucomm.bits.a_s)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="S";
    }
    if(ucomm.bits.a_4)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="0x4";
    }
    if(ucomm.bits.b_1|ucomm.bits.b_6|ucomm.bits.b_ns|ucomm.bits.b_s|ucomm.bits.b_s1)
        cmd="("+tmp+")+";
    else
        cmd=tmp;
    tmp="";
    if(ucomm.bits.b_1)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="0x1";
    }
    if(ucomm.bits.b_6)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="0x6";
    }
    if(ucomm.bits.b_ns)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="~S";
    }
    if(ucomm.bits.b_s)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="S";
    }
    if(ucomm.bits.b_s1)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="S1";
    }
    if(tmp.count())
    {
        if(cmd.count())
            cmd+="("+tmp+")";
        else
            cmd=tmp;
    }
    if(cmd.count())
    {
        if(ucomm.bits.g_l|ucomm.bits.g_nl|ucomm.bits.g_nt)
            cmd+="+";
    }
    tmp="";
    if(ucomm.bits.g_l)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="L";
    }
    if(ucomm.bits.g_nl)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="!L";
    }
    if(ucomm.bits.g_nt)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="!T";
    }
    if(tmp.count())
    {
        if(cmd.count())
            cmd+="("+tmp+")";
    }
    tmp="";
    if(cmd.count())
        cmd="sum="+cmd+"; ";

    switch(ucomm.bits.r0)
    {
        case 1:
        cmd+="R[i]=R[i+3]; ";
        break;
    case 2:
        cmd+="R[i]=sum; ";
        break;
    case 3:
        cmd+="R[i]=S; ";
        break;
    case 4:
        cmd+="R[i]=R[i]|S|sum; ";
        break;
    case 5:
        cmd+="R[i]=S|sum; ";
        break;
    case 6:
        cmd+="R[i]=R[i]|S; ";
        break;
    case 7:
        cmd+="R[i]=R[i]|sum; ";
        break;
    }
    if(ucomm.bits.r_1)
        cmd+="R[i-1]=sum; ";
    if(ucomm.bits.r_2)
        cmd+="R[i-2]=sum; ";
    if(ucomm.bits.m)
        cmd+="M[i]=S; ";
    switch(ucomm.bits.s)
    {
    case 1:
        cmd+="S=S1; ";
        break;
    case 2:
        cmd+="S=sum; ";
        break;
    case 3:
        cmd+="S=S1|sum; ";
        break;
    }
    if(ucomm.bits.l)
        cmd+="L=cry; ";
    switch(ucomm.bits.s1)
    {
        case 1:
        cmd+="S1=sum; ";
        break;
    case 2:
        cmd+="S1=S1|H; ";
        break;
    case 3:
        cmd+="S1=S1|H|sum; ";
        break;
    }
    switch(ucomm.bits.st)
    {
        case 1:
        cmd+="ST[i]=sum;";
        break;
    case 2:
        cmd+="ST[i]=ST[i+1];";
        break;
    case 3:
        cmd+="ST[]=XYZsum;";
       break;
    }

    ui->adderSym->setText(cmd);

    tmp.sprintf("0x%7.7X, ",ucomm.raw);
    ui->opcodeOut->setText(tmp);
    tmp+="//"+cmd;

    QClipboard *cb = QApplication::clipboard();
    cb->setText(tmp);


}

MainWindow::~MainWindow()
{
    delete ui;
}
