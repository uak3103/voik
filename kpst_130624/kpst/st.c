#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>

#include "../common/candev.h"
#include "../common/elements.h"
#include "sys/sys.h"
#include "rv/rv.h"
#include "vu/vu.h"
#include "apoi/apoi.h"
#include "csn/csn.h"
#include "inter.h"
#include "globst.h"
#include "st.h"
#include "ki/ki.h"


UPRST uprst;
int RABTK[5];
int NUMRABTK;
  
  

long myversion=3;
char *myversion_date = "24/06/13";

long version_KP54;
long jd_KP54;


int CVS=0;       // 1 - работа с ЦВС

int chv_bezA=0;  //ЧВ без оси А

int dumpizmer = 0;

void rabOK(void)
{
     rabst=rabvu=rabcsn=rabfmk=rabapoi=rabasev=0;
     rabksio=4;
}

void check_KP54_date(void)
{
  double jd1,jd2;
  int D,M,Y;
  char s[10];
  static char checked=0;

  if (version_KP54==0) return;
  jd1=JDsea;
  jd2=MJD_JD(jd_KP54);
  if (jd2<DMYtoJD(1,1,2004) || jd2>DMYtoJD(1,1,2025)) return;
  if (jd1!=jd2 && !checked) {
     checked=1;
     printf(
     "\n\nВнимание!\n"
     "Системная дата на ВС-С не совпадает с датой на ВС-К.\n"
     "Установите правильную дату.\n");
     gets(s);
  }
}


#include "inter.c"
#include "uprctk.c"
#include "uprst.c"
#include "uprki.c"
#include "uprtest.c"
#include "config.c"
#include "obrdump.c"
#include "proverki.c"
#include "voik/voik.c"

void print_bz(char *bz)
{
   printf("-------------------------------------------------\n"
		  "%02d:%02d:%02.0f Получена БЗ ", rvhour,rvmin,rvsec);
   switch (TYPEKV(bz)) {
     case POISK_KV :
       printf("на поиск в зоне "); goto zona;
     case PRPOISK_KV:
       printf("на продолжение поиска ");
zona:
       if (AVOIK_LZ(bz)==1) printf("(с АВОИК!)");
       printf("\n");
       if (TYPESK_LZ(bz)==NEPODV_SK)
         printf("  ДПТ "); else printf(" альфа");
       printf("   дельта   длина    ширина   наклон\n");
//       ***.**   ***.**   ***.**   ***.**   ***.**
       printf("%6.2f   %6.2f   %6.2f   %6.2f   %6.2f\n",
	  ALFA_LZ(bz),DELTA_LZ(bz),DELALFA_LZ(bz),DELDEL_LZ(bz),R_LZ(bz));
       break;

     case SBORKI_KV:
       printf("на сбор КИ");
       if (NI_KI(bz)!=NO_NI) printf(" и НИ");
       if (AVOIK_KI(bz)==1) printf(" (с АВОИК!)");
       printf("\n");
       long Nko;
       double t;
       int h,m; double s;
       Nko=NKO_KI(bz);
       t=T_KI(bz);
       h=t/3600.; m=(t-h*3600.)/60; s=t-h*3600.-m*60.;
       printf(
 "   Nко      A       H      VA      VH         T       блеск\n");
//******   ***.**  ***.**  **.**** **.****  **:**:**.**  **.*
       if (Nko==0) printf("   ***"); else printf("%6ld",Nko);
       printf("   %6.2f  %6.2f  %7.4f %7.4f  %02d:%02d:%05.2f %5.1f\n",
	      A_KI(bz),H_KI(bz),VA_KI(bz),VH_KI(bz),h,m,s,BLESK_KI(bz));
       if (NI_KI(bz)!=NO_NI) {
	 printf("Время фотометрирования %5.1f",TIMEFOT_NI(bz));
	 if (TVFOT_KI(bz)==1) printf(" (телевизионным методом)");
         printf("\n");
       }
//     double *kep = KEP_KI(bz);
//     printf("Кеплеровские параметры:\n");
//     for(int i=0;i<8;i++) printf("%f ",kep[i]);
//     printf("\n");
       break;

     case USLVID_KV:
       printf("\"Измерение условий видимости\"\n"
	      "призн.фона=%d призн.прозрач.=%d\n",
	      FON_IMT(bz),PROZR_IMT(bz));
       break;

     case POISKVDOL_KV:
       printf("на поиск ВДОЛЬ ТРАЕКТОРИИ ");
       if (AVOIK_LZV(bz)==1) printf(" (с АВОИК!)");
       printf("\n");
       void print_bz_VdolTr(char *bz);
       print_bz_VdolTr(bz);
       break;
   }
}


int paramzona(char *bz, ZONA &z)
{
  double t, alf, del, size1, size2, razvorot;
  double Az,H, 
         sizeX_grad, sizeY_grad, sizeL, sizeR,
         A, R,RN,RC,RK,DR, L,LN,LC,LK,DL,
         perekrL=0.1,
         perekrR=0.1, maxsinR, dop_razv=0, napr=1;
  int ntk_naved=1;
  int pz, NL, NR;
#ifdef  BL051
perekrL=0.01; perekrR=0.01;
#endif
  switch(TIPST) {
    case 56:
    case 58:
      if (NUMRABTK>1) 
        ntk_naved = 0;
      else {
        for(int i=1; i<=MAXTK; i++)
          if (RABTK[i]) { ntk_naved=i; break; }
      }
      break;
    case 60:
      ntk_naved=1;
      break;
  }

  if (TYPESK_LZ(bz)==NEPODV_SK) {
    DPTdelta_tdelta( DPT_LZ(bz)*GR, DELTA_LZ(bz)*GR, &t, &del );
    del=DELTA_LZ(bz)*GR;
    size1 = DELDPT_LZ(bz);
    size2 = DELDEL_LZ(bz);
    razvorot=R_LZ(bz);
  } else {
    alf = ALFA_LZ(bz)*GR;
    del = DELTA_LZ(bz)*GR;
    size1 = DELALFA_LZ(bz);
    size2 = DELDEL_LZ(bz);
    t=atsea(alf,rvtime);
    razvorot=R_LZ(bz);
  }
  size1=fabs(size1); if (size1<0.001) size1=0.001;
  size2=fabs(size2); if (size2<0.001) size2=0.001;
  //printf("дельта=%lf° час.уг.= %lf h\n",del*(180/M_PI),t*(12/M_PI));
  tdAH(t,del,&Az,&H);
  //printf("Азимут %lf°, угол места %lf°\n",Az*(180/M_PI),H*(180/M_PI));
povtor:
  Naved(ntk_naved,Az,H,POLUS,180+razvorot+dop_razv*napr,0,0,&A,&LC,&RC);
  
  
  if (TIPST==56) {
    z.A = A; 
    z.LN = z.LK = LC; 
    z.RN = z.RK= z.R = RC;
    z.RN=RN; z.RK=RK; z.DR=DR;
    z.NL=  z.NR = 1; 
    z.DR = z.DL = z.iL = z.iR = z.percent = 0;
    //z.NTK=NTK; //!!!
    return 1;
  }
  
  /* датчики центра зоны поиска */
  //printf("A=%f L=%f R=%f\n",A,LC,RC);

  RN=RC-size1/2; /* датчик оси R начала дуги */
  RK=RN+size1;   /* датчик оси R конца дуги */
  if (RK<90) maxsinR=sin(RK*GR);
  if (RN>90) maxsinR=sin(RN*GR);
  if (RN<90&&RK>90) maxsinR=1;
  LN=LC-size2/2/maxsinR; /* датчик оси L первой дуги */
  LK=LN+size2/maxsinR;   /* датчик оси L последней дуги */

  if(LK>MAXCD(OS_L)-0.5 || LN<MINCD(OS_L)+0.5 ||
     RK>MAXCD(OS_R)-0.5 || RN<MINCD(OS_R)+0.5) {
     if (dop_razv>180) {
         printf("\nНаведение невозможно.Дуга заходит в зону ограничения ОПУ\n");
         return 0;
     }
     dop_razv+=0.5;
     napr*=-1;
     goto povtor;
  }

  sizeX_grad = TKsizeXmm(ntk_naved) / pasF(ntk_naved) * RG;
  sizeY_grad = TKsizeYmm(ntk_naved) / pasF(ntk_naved) * RG;

  if (TIPST==58 && NUMRABTK==2) sizeY_grad += 70./60;

  if (TIPST==60 || TIPST==59) {
    sizeR = sizeY_grad;
    sizeL = sizeX_grad;
  } else if (TIPST==58) {
    sizeR = sizeX_grad;
    sizeL = sizeY_grad;
  }

  DL = (1-perekrL) * sizeL / maxsinR;
  NL = (size2/maxsinR - 0.001) / DL + 1;
  LN = LC - DL*(NL-1)/2; LK = LC + DL*(NL-1)/2;
  DR = (1-perekrR) * sizeR; NR = (size1-0.001) / DR +1;

  RN = RC - DR*(NR-1)/2; RK = RC + DR*(NR-1)/2;

  printf("Углы наведения: A=%7.2f L=%6.2f...%6.2f R=%6.2f...%6.2f\n",
         A,LN,LK,RN,RK);

  z.A=A; z.LN=LN; z.LK=LK; z.DL=DL; z.RN=RN; z.RK=RK; z.DR=DR;
  z.R=RN;
  z.NL=NL; z.NR=NR; z.iL=0; z.iR=0;
  z.percent=0;
  //z.NTK=NTK;
  return 1;
}

int sravn_zona(ZONA &z1, ZONA &z2)
{
  if (fabs(z1.A-z2.A)<0.5 &&
      fabs(z1.LN-z2.LN)<0.5 &&
      fabs(z1.LK-z2.LK)<0.5 &&
      fabs(z1.RN-z2.RN)<0.5 &&
      fabs(z1.RK-z2.RK)<0.5 ) return 1;
  else return 0;
}

void ispoln_bz(char *bz)
{
  UPRST &u=uprst;
//ZONA &z=u.current_zona;
  double T,Az,H,VAz,VH,A,L,R; int ntk,nko,pz; char s[10];
  int fotflag;

  switch(TYPEKV(bz)) {
   case POISK_KV:
   {
     if (!paramzona(bz,u.zona)) return;

restore_poisk:

     if (TIPST == 56) { 
       pz=BARIER56(u.zona); 
       break; 
     }

     pz=SCANDISKR(u.zona);

     if (pz==0) { //если просмотр зоны не закончен, сохранить зону
       u.zona_flag=1;
       if (DELDPT_LZ(bz)*DELDEL_LZ(bz) > 25) {
         u.saved_zona=u.zona;
         u.saved_zona_flag=1;
       }
     } else
       u.zona_flag=0;

     break;
   }

   case PRPOISK_KV:
   {
     ZONA z;
     if (!paramzona(bz,z)) return;

     if (u.zona_flag && sravn_zona(u.zona,z)==1)
        goto restore_poisk;

     if (u.saved_zona_flag && sravn_zona(u.saved_zona,z)==1) {
        u.zona=u.saved_zona;
        u.saved_zona_flag=0;
        u.zona_flag=1;
        goto restore_poisk;
     }

     u.zona=z;
     goto restore_poisk;

     break;
   }

   case SBORKI_KV :
   {
     static double T_last_bz=0;
     static long Nko_last_bz=0;
     int povtor_bz=0;

     if (NKO_KI(bz)==Nko_last_bz && rvtime-T_last_bz<10) povtor_bz=1;
     Nko_last_bz=NKO_KI(bz);

     T = time_kbp2kpst(T_KI(bz));
     Az=A_KI(bz); H=H_KI(bz); VAz=VA_KI(bz); VH=VH_KI(bz);

     double *kep=KEP_KI(bz);
     if (kep[0]!=0) {
       estglobkep=1;
       memcpy(globkep,kep,8*sizeof(double));
       globkep[0]=MJD_JD(globkep[0]);
     } else
       estglobkep=0;

     fotflag=0;

     if (NI_KI(bz)!=NO_NI && TIMEFOT_NI(bz)>0)
     {
       if (estglobkep) fotflag=1;
       else printf("Нет параметров от КП54. "
		   "Фотометрирование не будет выполняться.\n");
     }

     if (H==0) { //При Н=0 БЗ выполняется без переброса. 23.04.03
	pz=1;
	vxapr[1]=vxapr[2]=vyapr[1]=vyapr[2]=0;
	A=L=R=999;
     } else if (Az<0||Az>360||H<0||H>90) pz=0;
     else {
       for(ntk=1; ntk<=MAXTK; ntk++)
          if (RABTK[ntk]) break;
       pz=NavedKO(ntk,fotflag,T, Az*GR,H*GR,VAz*GR,VH*GR, &A,&L,&R);
       getvxvy(A,L,R,Az*GR,H*GR,VAz*GR,VH*GR);
     }
     if (pz==0) {
	printf("Наведение невозможно\n");
	sterror=0;//1;//krikunov
	    return;
     }
//printf("\n BEFORE OBNTCHK!");
     pz=OBNTCHK(A,L,R);
//printf("\n AFTER OBNTCHK");
     T_last_bz = rvtime;

     break;
   }
   
   case USLVID_KV :
     break;

   case POISKVDOL_KV:
     void ispoln_bz_VdolTr(char *bz);
     ispoln_bz_VdolTr(bz);
     break;

  }

  if (pz==1) printf("БЗ выполнена\n");
}


int kpstmain (void)
{
 int i; char s[80];
 int num_errors=0;
/*
 if (!readCONFIG()) 
   return 0;

 if (!initrv())  goto quit;

 initvu();

 initcsn();      //CSN

 if (!initglobst(TIPST,NPORST,rvday,rvmonth,rvyear,rvhour))
   goto quit;

 init_ctk();

 INITAPOI(MAXTK);

 initinter();    //IPX

 printf(
"\n"
"╔═версия от %-8s════════════════════════════════════════════════╗\n"
"║      *** Запущена комплексная программа станции %dЖ6-%d ***        ║\n"
"║                 Дата: %02d/%02d/%4d  Время: %02d:%02d:%02.0f                 ║\n",
 myversion_date,
 TIPST,NPORST,rvday,rvmonth,rvyear,rvhour,rvmin,rvsec);
 if (IMIT) printf(
"║                        РЕЖИМ ИМИТАЦИИ!                            ║\n");
 printf(
"╚═══════════════════════════════════════════════════════════════════╝\n");
*/
ReadKadr();

// 16.06.2013 tsarev
 CVS=1;		//послать сообщение Попову.
// goto cvs;

dial:

// void dialog(void);
// dialog();

cvs:
 /* подготовка к работе модуля связи с ЦВС */
/*
 if (!initinter()) { CVS=2; goto dial; }

 while(1) { // цикл ожидания БЗ 
   clearsterror();
   waitbz_cvs();
   if (breakcode!=0) {
	 if (breakcode==1) { CVS=2; kon_sv(); goto dial; } else goto quit;
   }

   print_bz(bz);
   flush_text_dump();
   ispoln_bz(bz);

   if (breakcode!=4) estbz_cvs = 0;

   printsterror();

   if (!sterror&&!breakcode) rabOK();

   if (CVS==1) {
	 if (!sterror&&!breakcode)
	   output_msg(BZEND_TS,BZVIP_PZ); // послать сбщ."БЗ выполнена"
	 if (sterror) {
	   output_msg(OTKAZST_TS,BZVIP_PZ); // послать сбщ."отказ станции"
	   if (++num_errors>=3) { num_errors=0; CVS=2; kon_sv(); goto dial; }
	 } else
	   num_errors=0;
   }
   flush_text_dump();

 } // конец основного цикла 

quit: 
 if (CVS==1) output_msg(RABZ_TS);

 printf( "\n*** Сеанс завершен ***\n" );
*/
 if (ftextdump!=NULL) fclose(ftextdump);
 
 return 0;
}

