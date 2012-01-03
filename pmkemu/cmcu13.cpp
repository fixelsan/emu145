//
//  cmcu13.cpp
//  m145ik13
//
//  Created by Felix Lazarev on 12/30/11.
//  Copyright 2011 Felix Lazarev. All rights reserved.
//
#include "cmcu13.h"
#include "cdebugdlg.h"

const unsigned char jrom[42]={
    0,1,2,3,4,5,3,4,5,3,4,5,3,4,5,3,4,5,3,4,5,3,4,5,6,7,8,
    0,1,2,3,4,5,6,7,8,
    0,1,2,3,4,5
};


cMCU::cMCU(QString name,bool debug)
{
    
    int i;
    for(i=0;i<MCU_BITLEN;i++)
    {
        rm[i]=0;
        rr[i]=0;
        rs[i&2]=0;
        rs1[i&2]=0;
        rst[i%12]=0;
        rh[i&2]=0;
    }
    debugme=debug;
    myname=name;
    if(debugme)
    {
        dbg=new cDebugDlg();
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
    icount=0;
    dcount=0;
    ecount=0;
    ucount=0;
    sigma=0;
    carry=0;
    rl=0;
    rt=0;
    
    cptr=0;

    if(debugme)
        dbg->setI(icount);
    
    command=cmdrom[cptr];
    
}

bool cMCU::tick(bool rin,bool k1, bool k2, unsigned int * dcycle, bool * syncout, bool *segment, bool *point)
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
    
    a=0;
    b=0;
    g=0;
    
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
    if(u_command.bits.g_nt)
    {
        if(k1|k2)
        {
            rt=1;
        }
        else
        if(command&0xfc0000)
            rt=0;
    
        if(((command&0xfc0000)==0)&&(rt))
        {
            rs1[0]=((((k2?1:0)<<8|(k1?1:0))>>ucount)&1)?true:false;
        }
        
        g|=!rt;
    }
    
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
        rr[MCU_BITLEN-1*4]=sigma;
    if(u_command.bits.r_2)
        rr[MCU_BITLEN-2*4]=sigma;
    if(u_command.bits.l)
        rl=carry;
    if(u_command.bits.m)
        newm0=rs[0];
    else
        newm0=rin;
    
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
            temp=rs[0];
            rs[0]=rs[1];rs[1]=rs[2];rs[2]=rs[3];
            rs[3]=sigma|temp;
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
            for(i=0;i<11;i++)
                rst[i+1]=rst[i];
            rst[0]=sigma;
            break;
        case 2:
            temp=rst[0];
            for(i=0;i<11;i++)
                rst[i]=rst[i+1];
            rst[11]=temp;
            break;
        case 3:
            x=rst[0*4+ucount];
            y=rst[1*4+ucount];
            z=rst[2*4+ucount];
            rst[0*4+ucount]=sigma|y;
            rst[1*4+ucount]=x|z;
            rst[2*4+ucount]=x|y;
            break;
    }
    
    
    ret=newm0;
    for(i=0;i<(MCU_BITLEN-1);i++)
        rm[i]=rm[i+1];
    rm[MCU_BITLEN-1]=rin;
    

    if((icount<36)&&(command&0xff000000))//mod flag -- do not modify R!!!
        newr0=rr[0];
    
    for(i=0;i<(MCU_BITLEN-1);i++)
        rr[i]=rr[i+1];
    rr[MCU_BITLEN-1]=newr0;
    
    temp=rst[0];
    for(i=0;i<(MCU_BITLEN-1);i++)
        rst[i]=rst[i+1];
    rst[MCU_BITLEN-1]=temp;
 
    
    ucount++;
    if(ucount>=4)
    {
        ucount=0;
        icount++;

        if((debugme)&&(icount<42))
            dbg->setI(icount);

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
        if(command&0xfc0000)
        {
            rt=0;
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
        *dcycle=(command&0xfc0000)?0:dcount+1;   //return value is D strobe idx for keyboard/display scan
        *syncout=((dcount==13)&&(ecount==2)&&(ucount==3))?true:false;
    }

    if((segment)&&(point))
    {
        *segment=(command&0xfc0000)?0:rr[0];
        *point=(command&0xfc0000)?0:rl;
    }

    return (ret&1)?true:false;
}
