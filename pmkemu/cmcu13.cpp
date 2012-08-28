//
//  cmcu13.cpp
//  m145ik13
//
//  Created by Felix Lazarev on 12/30/11.
//  Copyright 2011 Felix Lazarev. All rights reserved.
//
#include "cmcu13.h"
#include "cdebugdlg.h"
#include "QFile"

const unsigned char jrom[42]={
    0,1,2,3,4,5,3,4,5,3,4,5,3,4,5,3,4,5,3,4,5,3,4,5,6,7,8,
    0,1,2,3,4,5,6,7,8,
    0,1,2,3,4,5
};


cMCU::cMCU(QWidget *parent, QString name,bool debug)
{
    
    int i;
    for(i=0;i<MCU_BITLEN;i++)
    {
        rm[i]=false;
        rr[i]=false;
        rs[i&3]=false;
        rs1[i&3]=false;
        rst[i]=false;
        rh[i&3]=false;

    }

    debugme=debug;
    myname=name;
    if(debugme)
    {
        dbg=new cDebugDlg(parent);
        dbg->setWindowTitle(name);
        dbg->show();
    }
}

cMCU::~cMCU()
{
    if(debugme)
    {
        dbg->hide();
        delete dbg;
    }
}

void cMCU::init()
{
    int i;

    icount=0;
    dcount=0;
    ecount=0;
    ucount=0;
    sigma=0;
    carry=0;
    rl=0;
    rt=0;
    
    cptr=0;

#if 0
    //Test code for display
    for(i=0;i<12;i++)
    {
        rr[i*3*4+0]=(i+1)&1;
        rr[i*3*4+1]=((i+1)&2)?true:false;
        rr[i*3*4+2]=((i+1)&4)?true:false;
        rr[i*3*4+3]=((i+1)&8)?true:false;
    }
    cptr=0x64;
    rr[144+0]=(cptr)&1;
    rr[144+1]=(cptr&2)?true:false;
    rr[144+2]=(cptr&4)?true:false;
    rr[144+3]=(cptr&8)?true:false;

    rr[156+0]=(cptr>>4)&1;
    rr[156+1]=((cptr>>4)&2)?true:false;
    rr[156+2]=((cptr>>4)&4)?true:false;
    rr[156+3]=((cptr>>4)&8)?true:false;

#endif

    if(debugme)
        dbg->setI(icount);
    

    command=cmdrom[cptr];

    if(debugme)
    {
        dbg->setCMD(command);
        disassemble();
    }

    if(debugme)
    {
        dbg->setS(rs[0]|rs[1]<<1|rs[2]<<2|rs[3]<<3);
        dbg->setS1(rs1[0]|rs1[1]<<1|rs1[2]<<2|rs1[3]<<3);
        dbg->setL(rl);
        dbg->setPC(cptr);
        dbg->setH(rh[0]|rh[1]<<1|rh[2]<<2|rh[3]<<3);
        dbg->setREGS(rm,rr,rst,MCU_BITLEN,icount,ucount);
    }
}

void cMCU::pretick(bool rin)
{
    rm[MCU_BITLEN-1]=rin;
}

bool cMCU::tick(bool rin,bool k1, bool k2, unsigned int * dcycle, bool * syncout, unsigned char *segment)
{
    int i;
    unsigned int ret;
    int a;
    int b;
    int g;
    int newr0;
    int newm0;
    bool x,y,z;
    unsigned char ucmd;
    bool temp;
    bool oldrm0;

    a=0;
    b=0;
    g=0;

    rm[MCU_BITLEN-1]=rin; //shift in the data

    if(icount<27)
    {
        ucmd=asprom[command&0x7f][jrom[icount]];
    }else
        if((icount>=27)&&(icount<36))
        {
            ucmd=asprom[(command>>8)&0x7f][jrom[icount]];
        }
        else
        {
            // special case
            if(((command>>16)&0xff)>=0x20)
            {
                //r0 points to i=36
                //we need to store ASP field to R1[D14:D13]
                if(icount==36)
                {
                    rr[4*1]=((((command>>16)&0xf)>>ucount)&1)?true:false;
                    rr[4*4]=((((command>>20)&0xf)>>ucount)&1)?true:false;
                }
                ucmd=asprom[0x5f][jrom[icount]];
            }
            else
                ucmd=asprom[(command>>16)&0x3f][jrom[icount]]; 
        }
   
    ucmd&=0x3f; //fool's proof
    if(ucmd>0x3b)
    {
        ucmd=(ucmd-0x3c)*2;
        ucmd+=!rl?1:0;
        ucmd+=0x3c;
    }
    
    u_command=ucrom[ucmd];
    

    
    if(u_command.bits.a_r)
        a|=rr[0];

    if(u_command.bits.a_m)
        a|=rm[0];

    if(u_command.bits.a_st)
        a|=rst[0];

    if(u_command.bits.a_nr)
        a|=!rr[0];

    if(u_command.bits.a_10nl)
        a|=((10>>ucount)&1)&!rl;

    if(u_command.bits.a_s)
        a|=rs[0];

    if(u_command.bits.a_4)
        a|=(4>>ucount)&1;

  
    
    
    
    if(u_command.bits.b_1)
        b|=(1>>ucount)&1;
    if(u_command.bits.b_6)
        b|=(6>>ucount)&1;
    if(u_command.bits.b_s)
        b|=rs[0];
    if(u_command.bits.b_s1)
        b|=rs1[0];
    if(u_command.bits.b_ns)
        b|=!rs[0];
    
    if(u_command.bits.g_l)
        g|=rl;
    if(u_command.bits.g_nl)
        g|=!rl;

    if(k1|k2)
    {
        rt=1;
        latchk1=k1;
        latchk2=k2;
    }
    else
    if(command&0xfc0000)
    {
        latchk1=false;
        latchk2=false;
        rt=0;
    }

    if(((command&0xfc0000)==0)&&(rt))
    {
        rs1[0]=((((latchk2?1:0)<<3|(latchk1?1:0))>>ucount)&1)?true:false;
    }

    if(u_command.bits.g_nt)
         g|=!rt;

    
    if(ucount!=0) //gamma input -- 1 bit data or carry only. 
        g=carry;
    
    sigma=a+b+g;
    carry=(sigma>>1)&1;
    sigma&=1;
    
    switch(u_command.bits.r0)
    {
        case 0:
            newr0=rr[0];
            break;
        case 1:
            newr0=rr[3*4];
            break;
        case 2:
            newr0=sigma;
            break;
        case 3:
            newr0=rs[0];
            break;
        case 4:
            newr0=rr[0]|rs[0]|sigma;
            break;
        case 5:
            newr0=rs[0]|sigma;
            break;
        case 6:
            newr0=rr[0]|rs[0];
            break;
        case 7:
            newr0=rr[0]|sigma;
            break;
    }
    if(u_command.bits.r_1)
    {
        if(icount<36)
        {
            if((command&0xff000000)==0)
                rr[MCU_BITLEN-1*4]=sigma;
        }
        else
            rr[MCU_BITLEN-1*4]=sigma;
    }
    if(u_command.bits.r_2)
    {
        if(icount<36)
        {
            if((command&0xff000000)==0)
                rr[MCU_BITLEN-2*4]=sigma;
        }
        else
            rr[MCU_BITLEN-2*4]=sigma;

    }
    if(u_command.bits.l)
        //if(ucount==3)
            rl=carry;

    if(u_command.bits.m)
        newm0=rs[0];
    else
        newm0=rm[0];
    
    switch(u_command.bits.s)
    {
        case 0:
            temp=rs[0];
            rs[0]=rs[1];rs[1]=rs[2];rs[2]=rs[3];
            rs[3]=temp;
            break;
        case 1:
            rs[0]=rs[1];rs[1]=rs[2];rs[2]=rs[3];
            rs[3]=rs1[0];
            break;
        case 2:
            rs[0]=rs[1];rs[1]=rs[2];rs[2]=rs[3];
            rs[3]=sigma;
            break;
        case 3:
            temp=rs1[0];
            rs1[0]=rs1[1];rs1[1]=rs1[2];rs1[2]=rs1[3];
            rs1[3]=sigma|temp;
            break;
    }
    
    switch(u_command.bits.s1)
    {
        case 0:
            temp=rs1[0];
            rs1[0]=rs1[1];rs1[1]=rs1[2];rs1[2]=rs1[3];
            rs1[3]=temp;
            break;
        case 1:
            rs1[0]=rs1[1];rs1[1]=rs1[2];rs1[2]=rs1[3];
            rs1[3]=sigma;
            break;
        case 2:
            temp=rs1[0];
            rs1[0]=rs1[1];rs1[1]=rs1[2];rs1[2]=rs1[3];
            rs1[3]=temp|rh[0];
            break;
        case 3:
            temp=rs1[0];
            rs1[0]=rs1[1];rs1[1]=rs1[2];rs1[2]=rs1[3];
            rs1[3]=temp|rh[0]|sigma;
            break;
    }
    
    switch(u_command.bits.st)
    {
        case 1:
            rst[8]=rst[4];
            rst[4]=rst[0];
            rst[0]=sigma;
            break;
        case 2:
            temp=rst[0];
            rst[0]=rst[4];
            rst[4]=rst[8];
            rst[8]=temp;
            break;
        case 3:
            x=rst[0*4];
            y=rst[1*4];
            z=rst[2*4];
            rst[0*4]=sigma|y;
            rst[1*4]=x|z;
            rst[2*4]=x|y;
            break;
    }
    
    
    ret=newm0;

    for(i=0;i<(MCU_BITLEN-1);i++)
        rm[i]=rm[i+1];


    

    if((icount<36)&&(command&0xff000000))//mod flag -- do not modify R!!!
        newr0=rr[0];
    
    for(i=0;i<(MCU_BITLEN-1);i++)
        rr[i]=rr[i+1];
    rr[MCU_BITLEN-1]=newr0;

    if((dcount<13)&&(ecount==0))
    {
        for(i=0;i<3;i++)
            dispout[i]=dispout[i+1];
        dispout[3]=newr0;
    }
    
    temp=rst[0];
    for(i=0;i<(MCU_BITLEN-1);i++)
        rst[i]=rst[i+1];
    rst[MCU_BITLEN-1]=temp;
 
    if(debugme)
    {
        dbg->setS(rs[0]|rs[1]<<1|rs[2]<<2|rs[3]<<3);
        dbg->setS1(rs1[0]|rs1[1]<<1|rs1[2]<<2|rs1[3]<<3);
        dbg->setL(rl);
        dbg->setPC(cptr);
        dbg->setH(rh[0]|rh[1]<<1|rh[2]<<2|rh[3]<<3);
    }
    ucount++;
    if(ucount>=4)
    {
        ucount=0;
        icount++;

        if((debugme)&&(icount<42))
            dbg->setI(icount);
        if(debugme)
        {
            dbg->setCMD(command);
            disassemble();
        }
        ecount++;
    }
    if(icount>=42)
    {
        icount=0;
        if(debugme)
            dbg->setI(icount);


        cptr=rr[39*4+0]|rr[39*4+1]<<1|rr[39*4+2]<<2|rr[39*4+3]<<3;
        cptr=cptr<<4|rr[36*4+0]|rr[36*4+1]<<1|rr[36*4+2]<<2|rr[36*4+3]<<3;
        command=cmdrom[cptr];

#if 0
        QFile * log=new QFile("/Users/admin/src/emu145/emu145/ik1302.log");
        log->open(QFile::Append);
        QString str;
        str.sprintf("%2.2X    %2.2X %2.2X %2.2X %1.1X\n",cptr,command&0xff,(command>>8)&0xff,(command>>16)&0xff,(command>>24)&0xff);
        log->write(str.toAscii());
        log->close();
#endif

        rt=false;
        latchk1=false;
        latchk2=false;

        if(debugme)
        {
            dbg->setS(rs[0]|rs[1]<<1|rs[2]<<2|rs[3]<<3);
            dbg->setS1(rs1[0]|rs1[1]<<1|rs1[2]<<2|rs1[3]<<3);
            dbg->setL(rl);
            dbg->setPC(cptr);
            dbg->setH(rh[0]|rh[1]<<1|rh[2]<<2|rh[3]<<3);
        }

        if(debugme)
        {
            dbg->setCMD(command);
            disassemble();
        }

    }
    if(ecount>=3)
    {
        ecount=0;
        dcount++;
     }
    if(dcount>=14)
    {
        dcount=0;
    }
    
    //only needed for master - 1302
    if((dcycle)&&(syncout))
    {
        *dcycle=(((command&0xfc0000)==0)&&(ecount==0)&&(ucount==0))?(dcount+1):0;   //return value is D strobe idx for keyboard/display scan
        *syncout=((dcount==13)&&(ecount==2)&&(ucount==3))?true:false;
    }

    if(segment)
    {

        *segment=(((command&0xfc0000)==0)&&(ecount==0)&&(ucount==0))?(dispout[0]|dispout[1]<<1|dispout[2]<<2|dispout[3]<<3):0;
        temp=((command&0xfc0000)==0)?rl:0;
        *segment|=(temp)?0x80:0;
    }

    if(debugme)
    {
        dbg->setREGS(rm,rr,rst,MCU_BITLEN,icount,ucount);
    }

    return (ret&1)?true:false;
}

void cMCU::disassemble()
{
    QString cmd;
    QString tmp;
    unsigned char ucmd;
    unsigned char masp;
    ucmd_u mcmd;


    if(icount<27)
    {
        ucmd=asprom[command&0x7f][jrom[icount]];
        masp=command&0x7f;
    }else
        if((icount>=27)&&(icount<36))
        {
            ucmd=asprom[(command>>8)&0x7f][jrom[icount]];
            masp=(command>>8)&0x7f;
        }
        else
        {
            // special case
            if(((command>>16)&0xff)>=0x20)
            {
                ucmd=asprom[0x5f][jrom[icount]];
                masp=0x5f;
            }
            else
            {
                ucmd=asprom[(command>>16)&0x3f][jrom[icount]];
                masp=(command>>16)&0x3f;
            }
        }

    ucmd&=0x3f; //fool's proof
    if(ucmd>0x3b)
    {
        ucmd=(ucmd-0x3c)*2;
        ucmd+=!rl?1:0;
        ucmd+=0x3c;
    }

    mcmd=ucrom[ucmd];

    if(mcmd.raw==0)
    {
        dbg->setASP(masp);
        dbg->setUCMD(ucmd,"NOP");
        return;
    }
    cmd="";
    tmp="";
    if(mcmd.bits.a_r)
        tmp+="R[i]";
    if(mcmd.bits.a_m)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="M[i]";
    }
    if(mcmd.bits.a_st)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="ST[i]";
    }
    if(mcmd.bits.a_nr)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="~R[i]";
    }
    if(mcmd.bits.a_10nl)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="0xA*!L";
    }
    if(mcmd.bits.a_s)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="S";
    }
    if(mcmd.bits.a_4)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="0x4";
    }
    if(mcmd.bits.b_1|mcmd.bits.b_6|mcmd.bits.b_ns|mcmd.bits.b_s|mcmd.bits.b_s1)
        cmd="("+tmp+")+";
    else
        cmd=tmp;
    tmp="";
    if(mcmd.bits.b_1)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="0x1";
    }
    if(mcmd.bits.b_6)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="0x6";
    }
    if(mcmd.bits.b_ns)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="~S";
    }
    if(mcmd.bits.b_s)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="S";
    }
    if(mcmd.bits.b_s1)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="S1";
    }
    if(tmp.count())
    {
        if(cmd.count())
            cmd+="("+tmp+")";
        else
            cmd=tmp;
    }
    if(cmd.count())
    {
        if(mcmd.bits.g_l|mcmd.bits.g_nl|mcmd.bits.g_nt)
            cmd+="+";
    }
    tmp="";
    if(mcmd.bits.g_l)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="L";
    }
    if(mcmd.bits.g_nl)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="!L";
    }
    if(mcmd.bits.g_nt)
    {
        if(tmp.count())
            tmp+="|";
        tmp+="!T";
    }
    if(tmp.count())
    {
        if(cmd.count())
            cmd+="("+tmp+")";
    }
    tmp="";
    if(cmd.count())
        cmd="sum="+cmd+"; ";

    switch(mcmd.bits.r0)
    {
        case 1:
        cmd+="R[i]=R[i+3]; ";
        break;
    case 2:
        cmd+="R[i]=sum; ";
        break;
    case 3:
        cmd+="R[i]=S; ";
        break;
    case 4:
        cmd+="R[i]=R[i]|S|sum; ";
        break;
    case 5:
        cmd+="R[i]=S|sum; ";
        break;
    case 6:
        cmd+="R[i]=R[i]|S; ";
        break;
    case 7:
        cmd+="R[i]=R[i]|sum; ";
        break;
    }
    if(mcmd.bits.r_1)
        cmd+="R[i-1]=sum; ";
    if(mcmd.bits.r_2)
        cmd+="R[i-2]=sum; ";
    if(mcmd.bits.m)
        cmd+="M[i]=S; ";
    switch(mcmd.bits.s)
    {
    case 1:
        cmd+="S=S1; ";
        break;
    case 2:
        cmd+="S=sum; ";
        break;
    case 3:
        cmd+="S=S1|sum; ";
        break;
    }
    if(mcmd.bits.l)
        cmd+="L=cry; ";
    switch(mcmd.bits.s1)
    {
        case 1:
        cmd+="S1=sum; ";
        break;
    case 2:
        cmd+="S1=S1|H; ";
        break;
    case 3:
        cmd+="S1=S1|H|sum; ";
        break;
    }
    switch(mcmd.bits.st)
    {
        case 1:
        cmd+="ST[i]=sum;";
        break;
    case 2:
        cmd+="ST[i]=ST[i+1];";
        break;
    case 3:
        cmd+="ST[]=XYZsum;";
       break;
    }

    dbg->setASP(masp);
    dbg->setUCMD(ucmd,cmd);
}
