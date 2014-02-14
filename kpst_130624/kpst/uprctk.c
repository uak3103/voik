#define CAN_ABN_CTK(ntk) (CAN_ABN_CTK1 + ntk - 1)

int readyCTK[5];

double TexpCTK[5] = { 1.0, 1.0, 1.0, 1.0, 1.0};
int binCTK[5]     = {   1,   1,   1,   1,   1};
int rateCTK[5]    = {   2,   2,   2,   2,   2};

int TexpCTK_set[5];
int binCTK_set[5];
int rateCTK_set[5];

enum { 
  CTK_RATE_LOW=0,
  CTK_RATE_MEDIUM=1,
  CTK_RATE_HIGH=2,
};


//установка даты и времени на камере
void CTK_set_datetime(int ntk) 
{
  int D,M,Y, h,m; double s;
  double drob, wait;
  char mas[6];

  get_realtime(&D, &M, &Y, &h, &m, &s);
  drob = s - (int)s;
  wait=0;
  if (drob<0.25) wait=0.25-drob;
  if (drob>0.75) wait=1.25-drob;
  if (wait>0) usleep((int)(wait*1000000));

  get_realtime(&D, &M, &Y, &h, &m, &s);

  mas[0] = Y%100;
  mas[1] = M;
  mas[2] = D;
  mas[3] = h;
  mas[4] = m;
  mas[5] = (int)s;
  can_req8(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP, m2b('D','T'), mas);
  usleep(200);
  printf("settime %02d-%02d-%02d %02d:%02d:%02d\n",Y,M,D,h,m,(int)s);
}


void CTK_pusk(int ntk)
{
  can_req0(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUSK); 
  usleep(200);
}

void CTK_stop(int ntk)
{
  can_req0(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_STOP); 
  usleep(200);
  printf("STOP!\n");
}


//проверка включения камеры и начальные установки
int CTK_check_and_setup(int ntk)
{
  int flag;
  int adr = CAN_ABN_CTK(ntk);

  if (IMITCTK) {
    readyCTK[ntk]=1; return 1;
  }

  CAN_msg_flag[adr] = 0;
  can_req0(fcan_ctk,adr,CAN_CMD_STAT); 
  usleep(1000);
  flag = CAN_msg_flag[adr];
// can_req0(fcan_ctk, CAN_ABN_CTK(ntk),m2b(0x22,0x81));// Включить камеру
  if (flag==0) {
     readyCTK[ntk]=0; return 0;
  }

  if (readyCTK[ntk]==1 && flag==1)  return 1;


  CTK_set_datetime(ntk);   

  if (TexpCTK_set[ntk]) {
    can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('E','T'), (int)(TexpCTK[ntk]*1000)); 
    usleep(200);
  }

  if (binCTK_set[ntk]) {
    int b;
    switch(binCTK[ntk]) {
      case 1: b=0; break;
      case 2: b=1; break;
      case 4: b=2; break;
      case 8: b=3; break;
    }
    can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('B','X'), b); 
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('B','Y'), b); 
    usleep(200);
  }

  if (rateCTK_set[ntk]) {
    can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('T','R'), rateCTK[ntk]); 
    usleep(200);
  }
 // krikunov
  can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('K','A'),3846 ); //analog amplifier only for SHPK!
 //can_req0(fcan_ctk, CAN_ABN_CTK(ntk),m2b(0x16,0x80));//  cooler off!!! 
usleep(2000000);
  can_req2(fcan_ctk,CAN_ABN_CTK(ntk),CAN_CMD_EXTRA,0x8116); // cooler on!!! 
usleep(10000);
 // can_req0(fcan_ctk, CAN_ABN_CTK(ntk),m2b(0x16,0x81));//  cooler on!!!
usleep(20000);
//can_req0(fcan_ctk, CAN_ABN_CTK(ntk),0x8116);
usleep(20000);
//can_req0(fcan_ctk, CAN_ABN_CTK(ntk),0x1681);
//can_req0(fcan_ctk, CAN_ABN_CTK(ntk),m2b(0x81,0x16));//  cooler on!!! only for BIK !! ! ! 
  CTK_pusk(ntk);
  readyCTK[ntk]=1;

  return 1;
}
void CTK_reconnect(int ntk)
{

can_req2(fcan_ctk,CAN_ABN_CTK(ntk),CAN_CMD_EXTRA,0xC010);  // close ethernet connect
usleep(200000);
can_req2(fcan_ctk,CAN_ABN_CTK(ntk),CAN_CMD_EXTRA,0x8116); // cooler on!!!

can_req2(fcan_ctk,CAN_ABN_CTK(ntk),CAN_CMD_EXTRA,0xC110);  // ethernet connect enable
usleep(200);
}

void CTK_reset(int ntk)
{
 printf("\n CTK%d Reset begin \n",ntk);
readyCTK[ntk]=0;
can_req0(fcan_ctk,CAN_ABN_CTK(ntk),CAN_CMD_RESET);  // reset ctk
usleep(2000000);

CTK_set_datetime(ntk);

if (TexpCTK_set[ntk]) {
  can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('E','T'), (int)(TexpCTK[ntk]*1000));
  usleep(200);
}

if (binCTK_set[ntk]) {
  int b;
  switch(binCTK[ntk]) {
    case 1: b=0; break;
    case 2: b=1; break;
    case 4: b=2; break;
    case 8: b=3; break;
  }
  can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('B','X'), b);
  usleep(200);
  can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('B','Y'), b);
  usleep(200);
}

if (rateCTK_set[ntk]) {
  can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('T','R'), rateCTK[ntk]);
  usleep(200);
}

can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('K','A'),3846 ); //analog amplifier only for SHPK!
usleep(2000000);
can_req2(fcan_ctk,CAN_ABN_CTK(ntk),CAN_CMD_EXTRA,0x8116); // cooler on!!!
usleep(10000);
CTK_pusk(ntk);
readyCTK[ntk]=1;
printf("\n CTK%d Reset end \n",ntk);
}

void CTK_get_param(int ntk)
{
    int adr = CAN_ABN_CTK(ntk);
    CAN_msg_flag[adr] = 0;
    can_req2(fcan_ctk, adr, CAN_CMD_GETP,  m2b('B','X')); 
    usleep(1000);
    if (CAN_msg_flag[adr]==1) {
      CAN_MSG *msg = &CAN_last_msg[adr];
      if (CAN_CMD(msg->idn)==CAN_CMD_GETP && MSG_T(msg)==m2b('B','X')) {
        int v=MSG_V(msg), bin;
        switch(v) {
          case 0: bin=1; break;
          case 1: bin=2; break;
          case 2: bin=4; break;
          case 3: bin=8; break;
        }
        pasbin(ntk) = bin;
      }
    }
}


void print_ctk_msg(CAN_MSG &msg)
{
  int i;
  switch(CAN_CMD(msg.idn)) {
  case CAN_CMD_STAT:
    printf("STATUS ");
    for(i=0; i<msg.len; i++) printf("%02X ", msg.dat[i]);
    printf("\n");
    break;
  case CAN_CMD_GETP:
    printf("%c%c  ", msg.dat[0], msg.dat[1]);
    for(i=2; i<msg.len; i++) printf("%02X ",msg.dat[i]);
    printf("\n");
    break;
  }
}

int CTK_codes[] = { 
  'CT', 'DT', 'KA', 'ET', 'XT', 'EM', 
  'TR', 'CR', 'BS', 'QQ', 'WW', 'MM', 
  'BX', 'BY', 'X0', 'Y0', 'DX', 'DY',
  'T1', 'T2', 'T3', 'T4',
  'A0', 'P0', 'A1', 'P1'
};

void hndl_ctk(CAN_MSG *msg)
{
/*
   CAN_MSG ctk_msg=*msg;
   int ntk=CAN_ADR(ctk_msg.idn)-CAN_ABN_CTK1+1;
   char status[4];

   if(readyCTK[ntk]!=1) return;
   if(CAN_CMD(ctk_msg.idn)==CAN_CMD_STAT)
   {
        memcpy(status,ctk_msg.dat,4);
       if((status[1] & 0x20) != 0x20)
        {
          printf("\n CTK%d Ethernet on in status \n",ntk);
         }
    }
*/
}


void CTK_info(int ntk)
{
  int i;  
  int adr = CAN_ABN_CTK(ntk);

  CTK_check_and_setup(ntk);
  if (!readyCTK[ntk]) {
    printf("Камера не отвечает\n"); 
    return;
  }

  CAN_msg_flag[adr] = 0;
  can_req0(fcan_ctk,CAN_ABN_CTK(ntk),CAN_CMD_STAT); 
  usleep(2000);
  if (CAN_msg_flag[adr]) {
     printf("STATUS --> ");
     print_ctk_msg(CAN_last_msg[adr]);       
  }    

  for(i=0; i<sizeof(CTK_codes)/sizeof(int); i++) {
    int code = CTK_codes[i];
    int xchg = m2b(code>>8, code&0xFF);    

    CAN_msg_flag[adr] = 0;
    can_req2(fcan_ctk,CAN_ABN_CTK(ntk),CAN_CMD_GETP,xchg); 
    usleep(2000);

    if (CAN_msg_flag[adr]) {
       printf("%c%c --> ", code>>8, code&0xFF);
       print_ctk_msg(CAN_last_msg[adr]);       
    }    

  }
}


void CTK_checktime(int ntk)
{
  int D1,M1,Y1, h1,m1; double s1;
  int D2,M2,Y2, h2,m2,s2;
  int i;
  unsigned char *dan;
  int adr = CAN_ABN_CTK(ntk);

  CTK_check_and_setup(ntk);
  if (!readyCTK[ntk]) {
    printf("Камера не отвечает\n"); return;
  }

  printf("        CKS time              CTK %d time\n", ntk);
        //YYYY-MM-DD hh:mm:ss.ssss  YY-MM-DD hh:mm:ss

  for(int i=0; i<50; i++) {
    get_realtime(&D1, &M1, &Y1, &h1, &m1, &s1);

    CAN_msg_flag[adr] = 0;
    can_req2(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_GETP, m2b('D','T'));
    usleep(1000);

    if (CAN_msg_flag[adr]==0) continue;
    dan = CAN_last_msg[adr].dat;
    if (dan[0]!='D' || dan[1]!='T') continue;
    Y2 = dan[2];
    M2 = dan[3];
    D2 = dan[4];
    h2 = dan[5];
    m2 = dan[6];
    s2 = dan[7];
    printf("%4d-%02d-%02d %02d:%02d:%07.4f  %02d-%02d-%02d %02d:%02d:%02d\n",  
             Y1,M1,D1,h1,m1,s1, Y2,M2,D2,h2,m2,s2);

    usleep(20000);
  }
}

/*
void CTK_dialog_command(int ntk, int argc, char **argv)
{
   int i=0, flag=0;

   while(i<argc) {
      if (strcmp(argv[i],"pusk")==0) {
         CTK_pusk(ntk);
         i++;
      } else if (strcmp(argv[i],"stop")==0) {
         CTK_stop(ntk);
         i++;
      } else if (strcmp(argv[i],"texp")==0) {
         if (i+1<argc) TexpCTK[ntk]=atof(argv[i+1]);
         i+=2; flag=1;
      } else if (strcmp(argv[i],"bin")==0) {
         if (i+1<argc) binCTK[ntk]=atoi(argv[i+1]);
         i+=2; flag=1;
      } else if (strcmp(argv[i],"rate")==0) {
         if (i+1<argc) {
             if (strcmp(argv[i+1],"low")==0) {
               rateCTK[ntk]=CTK_RATE_LOW;
             } else if (strcmp(argv[i+1],"medium")==0) {
               rateCTK[ntk]=CTK_RATE_MEDIUM;
             } else if (strcmp(argv[i+1],"high")==0) {
               rateCTK[ntk]=CTK_RATE_HIGH;
             } else
               rateCTK[ntk]=atoi(argv[i+1]);
         } 
         i+=2; flag=1;
       } else
         i++;
   }

   if (flag) {
     CTK_set_param(ntk, TexpCTK[ntk], binCTK[ntk], rateCTK[ntk]);
   }
}
*/


void init_ctk(void)
{
  int ntk;
  for(ntk=1;ntk<=MAXTK;ntk++) {
    CTK_check_and_setup(ntk);     
  }
}
