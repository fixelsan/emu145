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
 
#ifndef EMULATOR_H
#define EMULATOR_H

#include <QThread>
#include "cmcu13.h"
#include "cmem.h"
#include "cdebugdlg.h"

typedef enum{
    e_rad,
    e_deg,
    e_grd
}mode_e;

class emulator : public QThread
{
    Q_OBJECT
public:
    explicit emulator(QObject *parent = 0);
    virtual void run();

signals:
    void on_sync(QByteArray * disp);

    
public slots:
    void enable(bool en);
    void step(int count);
    void debug(bool en);
    void set_mode(mode_e mod);
    void keypad(unsigned int key);


private:
    cMCU * ik1302;
    cMCU * ik1303;
    cMCU * ik1306;
    cMem * ir2_1;
    cMem * ir2_2;
    unsigned char display[12];
    bool chain;
    bool sync;
    bool k1;
    bool k2;
    unsigned int dcycle;
    unsigned int btnpressed;

    mode_e mode;

    int ustep;
    bool enabled;
    bool debugme;
    cDebugDlg * dbg1302;
    cDebugDlg * dbg1303;
    cDebugDlg * dbg1306;

};

#endif // EMULATOR_H
