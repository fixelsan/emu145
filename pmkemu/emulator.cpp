#include "emulator.h"
#include "ucommands.h"
#include "synchro.h"
#include "mcommands.h"

emulator::emulator(QObject *parent) :
    QThread(parent)
{
        int i,j;
        enabled=false;

        ik1302=new cMCU(this, "IK1302");
        ik1303=new cMCU(this, "IK1303");
        ik1306=new cMCU(this, "IK1306");
        ir2_1=new cMem();
        ir2_2=new cMem();

        //load memory
        for(i=0;i<68;i++)
        {
            ik1302->ucrom[i].raw=ik1302_urom[i];
            ik1303->ucrom[i].raw=ik1303_urom[i];
            ik1306->ucrom[i].raw=ik1306_urom[i];
        }
        for(i=0;i<128;i++)
            for(j=0;j<9;j++)
            {
                ik1302->asprom[i][j]=ik1302_srom[i][j];
                ik1303->asprom[i][j]=ik1303_srom[i][j];
                ik1306->asprom[i][j]=ik1306_srom[i][j];
            }
        for(i=0;i<256;i++)
        {
            ik1302->cmdrom[i]=ik1302_mrom[i];
            ik1303->cmdrom[i]=ik1303_mrom[i];
            ik1306->cmdrom[i]=ik1306_mrom[i];
        }


        ik1302->init();
        ik1303->init();
        ik1306->init();


        chain=false;
        sync=false;
        ustep=0;
        k1=false;
        k2=false;
        btnpressed=0;
        debugme=false;
        dbg1302=new cDebugDlg();
        dbg1303=new cDebugDlg();
        dbg1306=new cDebugDlg();

        dbg1302->setWindowTitle("IK1302");
        dbg1303->setWindowTitle("IK1303");
        dbg1306->setWindowTitle("IK1306");

        dbg1302->hide();
        dbg1303->hide();
        dbg1306->hide();

        dbg1302->setASP(ik1302->asp);
        dbg1302->setREGS(ik1302->rm,ik1302->rr,ik1302->rst,MCU_BITLEN,ik1302->icount,ik1302->ucount);
        dbg1302->setPC(ik1302->cptr);
        dbg1302->setCMD(ik1302->command);
        ik1302->disassemble();
        dbg1302->setUCMD(ik1302->cur_ucmd,ik1302->disassm);


        dbg1302->setS(ik1302->rs[0]|ik1302->rs[1]<<1|ik1302->rs[2]<<2|ik1302->rs[3]<<3);
        dbg1302->setS1(ik1302->rs1[0]|ik1302->rs1[1]<<1|ik1302->rs1[2]<<2|ik1302->rs1[3]<<3);
        dbg1302->setL(ik1302->rl);
        dbg1302->setH(ik1302->rh[0]|ik1302->rh[1]<<1|ik1302->rh[2]<<2|ik1302->rh[3]<<3);
        dbg1302->setI(ik1302->icount);

        dbg1303->setASP(ik1303->asp);
        dbg1303->setREGS(ik1303->rm,ik1303->rr,ik1303->rst,MCU_BITLEN,ik1303->icount,ik1303->ucount);
        dbg1303->setPC(ik1303->cptr);
        dbg1303->setCMD(ik1303->command);
        ik1303->disassemble();
        dbg1303->setUCMD(ik1303->cur_ucmd,ik1303->disassm);


        dbg1303->setS(ik1303->rs[0]|ik1303->rs[1]<<1|ik1303->rs[2]<<2|ik1303->rs[3]<<3);
        dbg1303->setS1(ik1303->rs1[0]|ik1303->rs1[1]<<1|ik1303->rs1[2]<<2|ik1303->rs1[3]<<3);
        dbg1303->setL(ik1303->rl);
        dbg1303->setH(ik1303->rh[0]|ik1303->rh[1]<<1|ik1303->rh[2]<<2|ik1303->rh[3]<<3);
        dbg1303->setI(ik1303->icount);

        dbg1306->setASP(ik1306->asp);
        dbg1306->setREGS(ik1306->rm,ik1306->rr,ik1306->rst,MCU_BITLEN,ik1306->icount,ik1306->ucount);
        dbg1306->setPC(ik1306->cptr);
        dbg1306->setCMD(ik1306->command);
        ik1306->disassemble();
        dbg1306->setUCMD(ik1306->cur_ucmd,ik1306->disassm);


        dbg1306->setS(ik1306->rs[0]|ik1306->rs[1]<<1|ik1306->rs[2]<<2|ik1306->rs[3]<<3);
        dbg1306->setS1(ik1306->rs1[0]|ik1306->rs1[1]<<1|ik1306->rs1[2]<<2|ik1306->rs1[3]<<3);
        dbg1306->setL(ik1306->rl);
        dbg1306->setH(ik1306->rh[0]|ik1306->rh[1]<<1|ik1306->rh[2]<<2|ik1306->rh[3]<<3);
        dbg1306->setI(ik1306->icount);



}

void emulator::enable(bool en)
{
    enabled=en;
}
void emulator::step(int count)
{
    ustep=count;
}
void emulator::debug(bool en)
{
    if(en!=debugme)
    {
        debugme=en;
        if(debugme)
        {
            dbg1302->show();
            dbg1303->show();

        }else
        {
            dbg1302->hide();
            dbg1303->hide();
        }
    }



}
void emulator::set_mode(mode_e mod)
{
    mode=mod;
}

void emulator::keypad(unsigned int key)
{

   // if((key!=0)&&(btnpressed))
    if(btnpressed)
        return; //already processing one

    btnpressed=key;
}

void emulator::run()
{
    unsigned char seg;

    unsigned int cycle;
    unsigned int maxcycle;
    bool grd;
    int buttondly;

    //2 ms = real time
    buttondly=0;

    while(1)
    {

        msleep(1);
        if(!enabled)
        {
            maxcycle=1;
            if(ustep==0)
                continue;
            ustep--;
        }
        else
        {
            maxcycle=168;
        }

        for(cycle=0;cycle<maxcycle;cycle++)
        {
            k1=false;
            k2=false;
            if(ik1302->strobe())
            {
                if(ik1302->dcount==12) //d13
                    k2=true;
                if(btnpressed)
                {
                       // qDebug("STB D=%d",ik1302->dcount);
                        if((ik1302->dcount+1)==(2+(btnpressed&0xff)))
                        {

                            switch(btnpressed>>8)
                            {
                                case 0x1:   k1=true;k2=false;break;
                                case 0x2:   k2=true;k1=false;break;
                                case 0x3:   k1=true;k2=true;break;
                            }
                            //qDebug("STB found D");
                            btnpressed=0; // found D
                        }
                 }



            }
            /*
            else
            {
                k1=false;
                k2=false;
            }*/



            chain=ik1302->tick(chain,k1,k2,&dcycle,&sync,&seg);
            if(debugme && !enabled)
            {
                dbg1302->setASP(ik1302->asp);
                dbg1302->setREGS(ik1302->rm,ik1302->rr,ik1302->rst,MCU_BITLEN,ik1302->icount,ik1302->ucount);
                dbg1302->setPC(ik1302->cptr);
                dbg1302->setCMD(ik1302->command);
                ik1302->disassemble();
                dbg1302->setUCMD(ik1302->cur_ucmd,ik1302->disassm);


                dbg1302->setS(ik1302->rs[0]|ik1302->rs[1]<<1|ik1302->rs[2]<<2|ik1302->rs[3]<<3);
                dbg1302->setS1(ik1302->rs1[0]|ik1302->rs1[1]<<1|ik1302->rs1[2]<<2|ik1302->rs1[3]<<3);
                dbg1302->setL(ik1302->rl);
                dbg1302->setH(ik1302->rh[0]|ik1302->rh[1]<<1|ik1302->rh[2]<<2|ik1302->rh[3]<<3);
                dbg1302->setI(ik1302->icount);

            }

            if(ik1302->strobe())
            {

                if((dcycle>1)&&(dcycle<14))
                {
                 display[dcycle-2]=seg;
                }



                if(sync)
                {

                    QByteArray * arr=new QByteArray((const char*)display,12);
                    emit on_sync(arr);
                }
            }
            else
            {
                if(sync)
                {
                    QByteArray * arr=new QByteArray(12,0xf);
                    emit on_sync(arr);
                }
            }

            switch(mode)
            {
                case e_rad:
                if(ik1303->dcount==9) //D10
                        grd=false;
                    else
                        grd=true;
                    break;
                case e_deg:
                if(ik1303->dcount==10) //D11
                    grd=false;
                else
                    grd=true;
                break;
                case e_grd:
                if(ik1303->dcount==11) //D12
                    grd=false;
                else
                    grd=true;
                break;
            }

            chain=ik1303->tick(chain,grd,false,NULL,NULL,NULL);

            if(debugme & !enabled)
            {
                dbg1303->setASP(ik1303->asp);
                dbg1303->setREGS(ik1303->rm,ik1303->rr,ik1303->rst,MCU_BITLEN,ik1303->icount,ik1303->ucount);
                dbg1303->setPC(ik1303->cptr);
                dbg1303->setCMD(ik1303->command);
                ik1303->disassemble();
                dbg1303->setUCMD(ik1303->cur_ucmd,ik1303->disassm);


                dbg1303->setS(ik1303->rs[0]|ik1303->rs[1]<<1|ik1303->rs[2]<<2|ik1303->rs[3]<<3);
                dbg1303->setS1(ik1303->rs1[0]|ik1303->rs1[1]<<1|ik1303->rs1[2]<<2|ik1303->rs1[3]<<3);
                dbg1303->setL(ik1303->rl);
                dbg1303->setH(ik1303->rh[0]|ik1303->rh[1]<<1|ik1303->rh[2]<<2|ik1303->rh[3]<<3);
                dbg1303->setI(ik1303->icount);
            }

            chain=ik1306->tick(chain,false,false,NULL,NULL,NULL);

            if(debugme & !enabled)
            {
                dbg1306->setASP(ik1306->asp);
                dbg1306->setREGS(ik1306->rm,ik1306->rr,ik1306->rst,MCU_BITLEN,ik1306->icount,ik1306->ucount);
                dbg1306->setPC(ik1306->cptr);
                dbg1306->setCMD(ik1306->command);
                ik1306->disassemble();
                dbg1306->setUCMD(ik1306->cur_ucmd,ik1306->disassm);


                dbg1306->setS(ik1306->rs[0]|ik1306->rs[1]<<1|ik1306->rs[2]<<2|ik1306->rs[3]<<3);
                dbg1306->setS1(ik1306->rs1[0]|ik1306->rs1[1]<<1|ik1306->rs1[2]<<2|ik1306->rs1[3]<<3);
                dbg1306->setL(ik1306->rl);
                dbg1306->setH(ik1306->rh[0]|ik1306->rh[1]<<1|ik1306->rh[2]<<2|ik1306->rh[3]<<3);
                dbg1306->setI(ik1306->icount);
            }


            chain=ir2_1->tick(chain);
            chain=ir2_2->tick(chain);

            ik1302->pretick(chain);
        }
        //msleep(20);
    }

}
