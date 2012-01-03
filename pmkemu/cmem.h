#ifndef CMEM_H
#define CMEM_H

#define MEM_BITLEN  2048

class cMem
{
public:
    cMem();
    bool tick(bool rm);

private:
    bool memarray[MEM_BITLEN];
};

#endif // CMEM_H
