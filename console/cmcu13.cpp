//
//  cmcu13.cpp
//  m145ik13
//
//  Created by Felix Lazarev on 12/30/11.
//  Copyright 2011 Felix Lazarev. All rights reserved.
//
#include "cmcu13.h"

const unsigned char jrom[42]={
    0,1,2,3,4,5,3,4,5,3,4,5,3,4,5,3,4,5,3,4,5,3,4,5,6,7,8,
    0,1,2,3,4,5,6,7,8,
    0,1,2,3,4,5
};


cMCU::cMCU()
{
    
    int i;
    for(i=0;i<MCU_BITLEN;i++)
    {
        m[i]=0;
        r[i]=0;
        s[i&2]=0;
        s1[i&2]=0;
        st[i%12]=0;
        h[i&2]=0;
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
    l=0;
    
    cptr=0;
    
    command=cmdrom[cptr];
    
}

bool cMCU::tick(bool rin,bool k1, bool k2, unsigned int * dcycle, bool * syncout)
{
    int i;
    unsigned int ret;
    int t;
    int a;
    int b;
    int g;
    int newr0;
    int newm0;
    int x,y,z;
    unsigned char ucmd;
    
    a=0;
    b=0;
    g=0;
    
    if(ecount==0)
    {
        //sample new H!!!!
        h[0]=k2;h[3]=k1;
        h[1]=false;h[2]=false;
    }
    
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
                r[4*1]=((((command>>16)&0xf)>>ucount)&1)?true:false;
                r[4*4]=((((command>>20)&0xf)>>ucount)&1)?true:false;
                ucmd=asprom[0x5f][jrom[icount]];
            }
            else
                ucmd=asprom[(command>>16)&0x3f][jrom[icount]]; 
        }
   
    ucmd&=0x3f; //fool's proof
    if(ucmd>0x3b)
    {
        ucmd=(ucmd-0x3c)*2;
        ucmd+=!l;
    }
    
    u_command=ucrom[ucmd];
    
 
    
    if(u_command.bits.a_r)
        a|=r[icount*4];

    if(u_command.bits.a_m)
        a|=m[icount*4];

    if(u_command.bits.a_st)
        a|=st[icount*4];

    if(u_command.bits.a_nr)
        a|=!r[icount*4];

    if(u_command.bits.a_10nl)
        a|=((10>>ucount)&1)&!l;

    if(u_command.bits.a_s)
        a|=s[0];

    if(u_command.bits.a_4)
        a|=(4>>ucount)&1;

  
    
    
    
    if(u_command.bits.b_1)
        b|=(1>>ucount)&1;
    if(u_command.bits.b_6)
        b|=(6>>ucount)&1;
    if(u_command.bits.b_s)
        b|=s[0];
    if(u_command.bits.b_s1)
        b|=s1[0];
    if(u_command.bits.b_ns)
        b|=!s[0];
    
    if(u_command.bits.g_l)
        g|=l;
    if(u_command.bits.g_nl)
        g|=!l;
    if(u_command.bits.g_nt)
        g|=!(h[0]|h[1]|h[2]|h[3]);
    
    if(ucount!=0) //gamma input -- 1 bit data or carry only. 
        g=carry;
    
    sigma=a+b+g;
    carry=(sigma>>1)&1;
    sigma&=1;
    
    switch(u_command.bits.r0)
    {
        case 0:
            newr0=r[0];
            break;
        case 1:
            newr0=r[3*4];
            break;
        case 2:
            newr0=sigma;
            break;
        case 3:
            newr0=s[0];
            break;
        case 4:
            newr0=r[0]|s[0]|sigma;
            break;
        case 5:
            newr0=s[0]|sigma;
            break;
        case 6:
            newr0=r[0]|s[0];
            break;
        case 7:
            newr0=r[0]|sigma;
            break;
    }
    if(u_command.bits.r_1)
        r[MCU_BITLEN-1*4]=sigma;
    if(u_command.bits.r_2)
        r[MCU_BITLEN-2*4]=sigma;
    if(u_command.bits.l)
        l=carry;
    if(u_command.bits.m)
        newm0=s[0];
    else
        newm0=rin;
    
    switch(u_command.bits.s)
    {
        case 0:
            t=s[0];
            s[0]=s[1];s[1]=s[2];s[2]=s[3];
            s[3]=t;
            break;
        case 1:
            s[0]=s[1];s[1]=s[2];s[2]=s[3];
            s[3]=s1[0];
            break;
        case 2:
            s[0]=s[1];s[1]=s[2];s[2]=s[3];
            s[3]=sigma;
            break;
        case 3:
            t=s[0];
            s[0]=s[1];s[1]=s[2];s[2]=s[3];
            s[3]=sigma|t;
            break;
    }
    
    switch(u_command.bits.s1)
    {
        case 0:
            t=s1[0];
            s1[0]=s1[1];s1[1]=s1[2];s1[2]=s1[3];
            s1[3]=t;
            break;
        case 1:
            s1[0]=s1[1];s1[1]=s1[2];s1[2]=s1[3];
            s1[3]=sigma;
            break;
        case 2:
            t=s1[0];
            s1[0]=s1[1];s1[1]=s1[2];s1[2]=s1[3];
            s1[3]=t|h[0];
            break;
        case 3:
            t=s1[0];
            s1[0]=s1[1];s1[1]=s1[2];s1[2]=s1[3];
            s1[3]=t|h[0]|sigma;
            break;
    }
    
    switch(u_command.bits.st)
    {
        case 1:
            for(i=0;i<11;i++)
                st[i+1]=st[i];
            st[0]=sigma;
            break;
        case 2:
            t=st[0];
            for(i=0;i<11;i++)
                st[i]=st[i+1];
            st[11]=t;
            break;
        case 3:
            x=st[0*4+ucount];
            y=st[1*4+ucount];
            z=st[2*4+ucount];
            st[0*4+ucount]=sigma|y;
            st[1*4+ucount]=x|z;
            st[2*4+ucount]=x|y;
            break;
    }
    
    
    ret=newm0;
    for(i=0;i<(MCU_BITLEN-1);i++)
        m[i]=m[i+1];
    m[MCU_BITLEN-1]=rin;
    

    if((icount<36)&&(command&0xff000000))//mod flag -- do not modify R!!!
        newr0=r[0];
    
    for(i=0;i<(MCU_BITLEN-1);i++)
        r[i]=r[i+1];
    r[MCU_BITLEN-1]=newr0;
    
    t=st[0];
    for(i=0;i<(MCU_BITLEN-1);i++)
        st[i]=st[i+1];
    st[MCU_BITLEN-1]=newr0;
 
    
    ucount++;
    if(ucount>=4)
    {
        ucount=0;
        icount++;
        ecount++;
    }
    if(icount>=42)
    {
        icount=0;
        cptr=r[39*4+0]|r[39*4+1]<<1|r[39*4+2]<<2|r[39*4+3]<<3;
        cptr=cptr<<4|r[36*4+0]|r[36*4+1]<<1|r[36*4+2]<<2|r[36*4+3]<<3;
        command=cmdrom[cptr];
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
        *dcycle=dcount;
        *syncout=((dcount==13)&&(ecount==3))?true:false;
    }
    

    return (ret&1)?true:false;
}
