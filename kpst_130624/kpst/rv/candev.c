extern void hndl_kdpo(CAN_MSG *msg);
extern void hndl_kep (CAN_MSG *msg);
extern void hndl_ctk(CAN_MSG *msg);
extern void hndl_kvu (CAN_MSG *msg);


int fcan[2]; // 2 канала CAN
int q_www=0; //для совместимости с common.cpp
int can_verbose_mode=0; 

/*
int ncan_vu   =0, 
    ncan_kdpo =0,
    ncan_kep  =0,
    ncan_ctk  =0;
*/

#define fcan_vu   fcan[ncan_vu]
#define fcan_kdpo fcan[ncan_kdpo]
#define fcan_kep  fcan[ncan_kep]
#define fcan_ctk  fcan[ncan_ctk]


CAN_MSG CAN_last_msg[16];
int CAN_msg_flag[16];

int CAN_abn_info[16][8];


void print_can_msg(int ncan, CAN_MSG *msg)
{
  printf("CAN%d %s%d %s %s", 
    ncan,
    (CAN_MOD(msg->idn)? "ret":"req"), 
    msg->len,
    can_abn_name(CAN_ADR(msg->idn)), 
    can_cmd_name(CAN_CMD(msg->idn)));

  if (msg->len >= 2) {
    int par = MSG_T(msg), 
        c1 = par&0xFF, 
        c2 = (par>>8)&0xFF;
    if (c1>=0x20 && c1<=0x7F && c2>=0x20 && c2<=0x7F)
      printf(" '%c%c'", par&0xFF, (par>>8)&0xFF );
    else 
      printf(" %04X", par);
  }
     
  if (msg->len == 6) {
    printf(" %08X", MSG_V(msg) );
  }

  printf("\n");
}

/***********************************************************************/
void* can_thread(void *arg)
{ CAN_MSG msg; int res,kkk;
  int ncan = (int)arg;
  int adr,cmd;

  for (kkk=0;;kkk++)
  {
   res=read(fcan[ncan],&msg,sizeof(CAN_MSG));
   if (res<0) break;

   adr = CAN_ADR(msg.idn);
   cmd = CAN_CMD(msg.idn);

   CAN_last_msg[adr] = msg;
   CAN_msg_flag[adr] = 1;

   if (can_verbose_mode) {
      if (! (adr == CAN_ABN_KDPO && 
             cmd == CAN_CMD_GETP &&
             MSG_T(&msg) >=0 && MSG_T(&msg)<=3 ) )
         print_can_msg(ncan,&msg);
   }

   if (cmd==CAN_CMD_GETP && msg.len == 6) {
     if (MSG_T(&msg) == m2b('#','V'))  CAN_abn_info[adr][0] = MSG_V(&msg);
     if (MSG_T(&msg) == m2b('#','D'))  CAN_abn_info[adr][1] = MSG_V(&msg);
     if (MSG_T(&msg) == m2b('T','X'))  CAN_abn_info[adr][2] = MSG_V(&msg);
     if (MSG_T(&msg) == m2b('R','X'))  CAN_abn_info[adr][3] = MSG_V(&msg);
     if (MSG_T(&msg) == m2b('E','R'))  CAN_abn_info[adr][4] = MSG_V(&msg);
   }
   
   switch (adr)
   {
     case CAN_ABN_KEPA:
     case CAN_ABN_KEPL:
     case CAN_ABN_KEPR:
       hndl_kep (&msg); break;
     case CAN_ABN_KDPO:
       hndl_kdpo(&msg); break;
     case CAN_ABN_KVU:
       hndl_kvu (&msg); break;
     case CAN_ABN_CTK1:
     case CAN_ABN_CTK2:
     case CAN_ABN_CTK3:
     case CAN_ABN_CTK4:
       hndl_ctk (&msg); break;
   }
  }
  return 0;
}
/***********************************************************************/
