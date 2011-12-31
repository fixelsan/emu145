//
//  cmcu13.h
//  m145ik13
//
//  Created by Felix Lazarev on 12/30/11.
//  Copyright 2011 Felix Lazarev. All rights reserved.
//

#ifndef m145ik13_cmcu13_h
#define m145ik13_cmcu13_h

#define MCU_BITLEN  168

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

class cMCU 
{
    
public:
    cMCU();
    void init();
    bool tick(bool rin,bool k1, bool k2, unsigned int * dcycle, bool * syncout);    //execute 1 tick of cpu, 
                    // parameters:  rin - input register; 
                    //              k1 -- input pin K1
                    //              k2 -- input pin K2
                    //              *dcycle  -- current D cycle, output
                    //              *syncout -- synchronization output for slaves
                    //return value = output register
    
    ucmd_u  ucrom[68];              //ucommand rom
    unsigned char asprom[128][9];   //synchro sequence rom
    unsigned int cmdrom[256];       //macrocommand rom   -- format (asp0)|(asp1<<8)|(asp2<<16)|(modflag<<24)

private:
    bool m[MCU_BITLEN];
    bool r[MCU_BITLEN];
    bool st[MCU_BITLEN];
    bool s[4];
    bool s1[4];
    bool h[4];
    bool l;
    unsigned int sigma; //so we can calculate normally
    bool carry;
    
    unsigned int icount;
    unsigned int dcount;
    unsigned int ecount;
    unsigned int ucount;
    ucmd_u  u_command;
    unsigned int command;
    unsigned char cptr;
};


#endif
