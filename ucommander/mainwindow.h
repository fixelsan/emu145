#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}


typedef union{
    struct{
        unsigned int l:1;
        unsigned int a_10nl:1;
        unsigned int a_4:1;
        unsigned int a_s:1;
        unsigned int b_6:1;
        unsigned int b_s:1;
        unsigned int b_ns:1;
        unsigned int b_s1:1;
        unsigned int b_1:1;
        unsigned int g_l:1;
        unsigned int g_nl:1;
        unsigned int g_nt:1;
        unsigned int r_2:1;
        unsigned int a_st:1;
        unsigned int r_1:1;
        unsigned int s:2;
        unsigned int s1_lo:1;
        unsigned int st_hi:1;
        unsigned int st_lo:1;
        unsigned int a_m:1;
        unsigned int a_nr:1;
        unsigned int a_r:1;
        unsigned int r0:3;
        unsigned int m:1;
        unsigned int s1_hi:1;
        unsigned int pad:4;
    }bits;
    unsigned int raw;
}rcmd_u;

typedef union{
    struct{
        unsigned int a_r:1;
        unsigned int a_m:1;
        unsigned int a_st:1;
        unsigned int a_nr:1;
        unsigned int a_10nl:1;
        unsigned int a_s:1;
        unsigned int a_4:1;

        unsigned int b_s:1;
        unsigned int b_ns:1;
        unsigned int b_s1:1;
        unsigned int b_6:1;
        unsigned int b_1:1;

        unsigned int g_l:1;
        unsigned int g_nl:1;
        unsigned int g_nt:1;

        unsigned int r0:3;
        unsigned int r_1:1;
        unsigned int r_2:1;
        unsigned int m:1;
        unsigned int l:1;

        unsigned int s:2;
        unsigned int s1:2;

        unsigned int st:2;

        unsigned int pad:4;



    }bits;
    unsigned int raw;
}ucmd_u;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void OnChange();
    void OnClear();
    void OnConv();


private:
    Ui::MainWindow *ui;
    ucmd_u ucomm;
    void disassm();
};

#endif // MAINWINDOW_H
