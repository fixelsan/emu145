#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QList>
#include "cmcu13.h"
#include "cmem.h"

namespace Ui {
class MainWindow;
}

typedef enum{
    e_rad,
    e_deg,
    e_grd
}mode_e;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void OnTimer();

private slots:
    void on_ustepBtn_clicked();

    void on_istepBtn_clicked();

    void on_cycleBtn_clicked();

    void on_keypad_clicked();

private:
    Ui::MainWindow *ui;
    QTimer * timer;
    cMCU * ik1302;
    cMCU * ik1303;
    cMCU * ik1306;
    cMem * ir2_1;
    cMem * ir2_2;
    unsigned char display[12];
    unsigned int olddcycle;
    bool chain;
    bool sync;
    bool k1;
    bool k2;
    bool fast;
    unsigned int dcycle;
    mode_e mode;


    QList<QPushButton*> btns;
    unsigned int btnpressed;

    int ustep;

    void updatedisp();
};

#endif // MAINWINDOW_H
