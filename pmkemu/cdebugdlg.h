/*
 * Emulator Copyright (c) 2012 Felix Lazarev <me@felixl.com>
 *
 * ROM Content Copyright (c) 198x Trokhimenko Y.K. and others
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
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
