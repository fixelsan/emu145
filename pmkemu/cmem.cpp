#include "cmem.h"

cMem::cMem()
{
    int i;
    for(i=0;i<MEM_BITLEN;i++)
        memarray[i]=false;
}

bool cMem::pretick()
{
    return memarray[0];
}

bool cMem::tick(bool rm)
{
    bool ret;
    int i;

    ret=memarray[0];
    for(i=0;i<(MEM_BITLEN-1);i++)
        memarray[i]=memarray[i+1];
    memarray[MEM_BITLEN-1]=rm;
    return ret;
}
