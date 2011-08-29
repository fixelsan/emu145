#include <cstdlib>
#include <string.h>
#include <iostream>
#include "cascade.h"
#include "chargemem.h"

CHARGE_MEM::CHARGE_MEM(int SIZE_MEM)
: CASCADE(), IN(NULL)
{
   SIZE = SIZE_MEM;
   C = new char[SIZE_MEM];
   Cbuff = new char[SIZE_MEM];
   memset(C, '0', SIZE_MEM);
}

CHARGE_MEM::~CHARGE_MEM()
{
	delete C;
	delete Cbuff;
}

void CHARGE_MEM::ClearBuff(void)
{
	memset(Cbuff, '0', SIZE);
}

void CHARGE_MEM::ClearData(void)
{
	memset(C, '0', SIZE);
}

void CHARGE_MEM::CopyBuff(void)
{
	memcpy(Cbuff, C, SIZE);
}

void CHARGE_MEM::CopyData(void)
{
	memcpy(&C[1], Cbuff, SIZE-1);
	if(IN != NULL) C[0] = IN->get();
}

void CHARGE_MEM::clk(int phase)
{
	switch(phase)
	{
		case 0: ClearBuff(); break;
		case 1: CopyBuff(); break;
		case 2: ClearData(); break;
		case 3: CopyData();
	}
}

char CHARGE_MEM::get(void)
{
	return Cbuff[SIZE-1];
}

void CHARGE_MEM::cascade(CASCADE *object)
{
	IN = object;
}

void CHARGE_MEM::set(int A, char D)
{
  C[A] = D;
}

int CHARGE_MEM::Size(void)
{
    return SIZE;
}

void CHARGE_MEM::show(void)
{
 char tmp = C[SIZE-1];
 C[SIZE-1] = 0;
 printf("%s%c\n",C,tmp);
 C[SIZE-1] = tmp;
}
