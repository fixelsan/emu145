#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QList>
#include "emulator.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
    void OnTimer();
    void on_keypad_clicked();

    void on_sync(QByteArray * disp);

signals:
    void enable(bool en);
    void step(int count);
    void debug(bool en);
    void set_mode(mode_e mod);
    void keypad(unsigned int key);

private slots:
    void on_ustepBtn_clicked();
    void on_istepBtn_clicked();
    void on_cycleBtn_clicked();
    void on_run_clicked();
    void on_mode_changed(int i);
    void on_debug_clicked();


    void on_showVariables_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QTimer * timer;
    emulator * emu;

    unsigned char display[12];
    int display_blank;

    QList<QPushButton*> btns;
    unsigned int btnpressed;

    int ustep;

    void updatedisp();
};

#endif // MAINWINDOW_H
