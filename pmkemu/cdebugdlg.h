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

private:
    Ui::cDebugDlg *ui;
    QTableWidgetItem * items[3*42];

};

#endif // CDEBUGDLG_H
