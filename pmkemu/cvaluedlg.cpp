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
 
#include "cvaluedlg.h"
#include "ui_cvaluedlg.h"

cValueDlg::cValueDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cValueDlg)
{
    ui->setupUi(this);
    changed=0;
}

cValueDlg::~cValueDlg()
{
    delete ui;
}

void cValueDlg::on_cancelBtn_clicked()
{
    changed=0;
    hide();
}

void cValueDlg::on_okBtn_clicked()
{
    changed=1;
    newval=ui->regValue->text().toInt(NULL,16);
    hide();
}

void cValueDlg::setReg(QString str, unsigned char val)
{
    ui->regText->setText(str);
    ui->regValue->setText(QString().sprintf("%1.1X",val&0xf));
}

