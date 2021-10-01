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
 
#ifndef CVALUEDLG_H
#define CVALUEDLG_H

#include <QDialog>

namespace Ui {
class cValueDlg;
}

class cValueDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit cValueDlg(QWidget *parent = 0);
    ~cValueDlg();

    void setReg(QString str, unsigned char val);


    int changed;
    int newval;
    
private slots:
    void on_cancelBtn_clicked();

    void on_okBtn_clicked();



private:
    Ui::cValueDlg *ui;
};

#endif // CVALUEDLG_H
