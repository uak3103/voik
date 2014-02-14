#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <bios.h>
#include "sys/sys.h"
#include "rv/rv.h"

#ifndef BYTE
  typedef unsigned char BYTE;
  typedef unsigned int  WORD;
#endif

#include "vu/vu.h"



void initvu(void)
{
}



void uprvu(int par1, int par2, ...)
{
//   can_req6(fcan,CAN_ABN_KVU,CAN_CMD_PUTP,el,sost); // CAN only
//   vu.status = 1;
}

void uprvu_nocheck(int el, int sost)
{
}

void err(int nel)
{
  //vu.status=-1;
  if (sterror==0) {
    rabst=rabvu=2;
    sterror=1;
    strcpy(sterrmsg1,"Ошибка ВУ");
    //p=sterrmsg2;
    //strcpy(p,nameelvu(nel,s));
    //strcat(p," не переведен в положение \"");
    //strcat(p,namesostelvu(nel,sost,s));
    //strcat(p,"\"");
  }
}

void taktvu(void)
{
}

int statusvu(void)
{ return 1; }


int sostelvu(int nel)
{
  return 1;
}

char* nameelvu(int nel, char *s)
{
  return NULL;
}

char* namesostelvu(int nel, int sost, char *s)
{
  return NULL;
}

void hndl_kvu(CAN_MSG *msg)
{ int val,elem;
  switch (CAN_CMD(msg->idn))
  {
   case CAN_CMD_GETP:
    elem=MSG_T(msg);
    val=MSG_V(msg);
    break;
  }
}

