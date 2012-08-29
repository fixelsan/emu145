#ifndef CMEM_H
#define CMEM_H

#define MEM_BITLEN  1008

class cMem
{
public:
    cMem();
    bool pretick(void);
    bool tick(bool rm);

private:
    bool memarray[MEM_BITLEN];
};

#endif // CMEM_H
