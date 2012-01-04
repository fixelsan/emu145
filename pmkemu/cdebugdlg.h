#ifndef CDEBUGDLG_H
#define CDEBUGDLG_H

#include <QDialog>
#include <QTableWidgetItem>


namespace Ui {
    class cDebugDlg;
}

class cDebugDlg : public QDialog
{
    Q_OBJECT

public:
    explicit cDebugDlg(QWidget *parent = 0);
    ~cDebugDlg();
    void setI(unsigned int i);
    void setH(unsigned char h);
    void setL(bool l);
    void setPC(unsigned char pc);
    void setCMD(unsigned int cmd);
    void setS(unsigned char s);
    void setS1(unsigned char s1);
    void setASP(unsigned char asp);
    void setUCMD(unsigned char ucmd,QString disassm);
    void setREGS(bool rm[],bool rr[],bool rst[],unsigned int rlen, unsigned int curi, unsigned int curu);

signals:
    void REGSChanged(unsigned int idx,unsigned char val);

private slots:
    void onItemChange(QTableWidgetItem* itm);



private:
    Ui::cDebugDlg *ui;
    QTableWidgetItem * items[3*42];


};

#endif // CDEBUGDLG_H
