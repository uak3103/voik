#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/ioctl.h> 
#include <ctype.h>
#include </usr/include/time.h>
#include <pthread.h>
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
#include "my_ip2.c"

#define CAN_ABN_CTK(ntk) (CAN_ABN_CTK1 + ntk - 1)

//=================krikunov=========================//
double dt =  0 ,dt_luft=0, dtt=0;
struct timeval tv1,  tv2,  dtv;
struct timezone tz;
struct timespec delay;
void time_start() { gettimeofday(&tv1, &tz); } // for calculate speed

long time_stop()                               // for calculate speed

{ gettimeofday(&tv2, &tz);

  dtv.tv_sec= tv2.tv_sec -tv1.tv_sec;

  dtv.tv_usec=tv2.tv_usec-tv1.tv_usec;

  if(dtv.tv_usec<0) { dtv.tv_sec--; dtv.tv_usec+=1000000; }

  return dtv.tv_sec*1000 + dtv.tv_usec/1000;//mlsek
delay.tv_sec = 0; delay.tv_nsec = 1000;
//delay.tv_nsec = 1000000000L;// 1 sec!
 nanosleep(&delay,NULL);// 1 mlsek
}
//extern int kodCD[3]; //= {0,524288/4,524288/4);//krikunov код ЦД
extern double maxskor[3] ;

extern void poiskv(void);
extern void max_osh_naved(int);
extern void max_osh_chv(int);
extern void max_SKO_izm(int);
extern void pozition();
extern int blesk(int);
extern  int ModSTAR();
extern int blesk();
extern  int ModSTAR();
extern void nepr_job();
extern void CTK_reconnect(int ntk);
extern void CTK_reset(int ntk);
//================krikunov==========================//



/*************** Деление командной строки на "слова" ***************/
void param(char *s, int *argc, char **argv)
{
 *argc=0;
 while(1) {
   while(*s==' ') { if(*s==0) return; s++; }
   if (*s==0) return;
   argv[(*argc)++]=s;
   while(*s!=' ') { if(*s==0) return; s++; }
   *s=0; s++;
 }
}


void strlwr(char *s);
//{
//  while(*s) { *s = tolower(*s); s++; }
//} 


int askTK(void)
{
  char s[100];
  int ntk, n;

  if (NUMRABTK==1) {
     for(ntk=1; ntk<=MAXTK; ntk++)
       if (RABTK[ntk]) return ntk;
  }
  printf("Введите номер ТК (%d-%d):", 1, MAXTK);
  gets(s);
  n=sscanf(s,"%d",&ntk);
  if (n==0) return -1;
  if (ntk<1 || ntk>MAXTK) {
    printf("Неверный номер ТК: %d\n", ntk); return -1;
  }
  if (RABTK[ntk]==0) {
    printf("ТК %d не используется в текущей конфигурации\n", ntk); return -1;
  }
  return ntk;
}


int gsko_proc(double dpt);
int zvezda_proc(void);
void testpasp(int n1, int n2);
void current_jd_t(double *jd, double *t);
void print_stat_kp54(void);


/* Главный диалог */
void dialog(void)
{
 char line[80]; int argc; char *argv[40]; char* name; int n;


 for(n=0;;n++) {
   int i,j;

   set_line_mode();

   printf("->"); gets(line);
   strlwr(line);

//   i=strlen(line);
//   if (i>0 && i<80 && line[i-1]=='\n') line[i-1]=0;

   param(line,&argc,argv);
   if (argc==0) continue;
   name=argv[0];
   clearsterror();

   /* Команда выхода */
   if (strcmp(name,"q")==0) {
	 flush_text_dump();
     exit(0);

   /* Команда вывода инструкции */
   } else if (strcmp(name,"?")==0) {
printf(
   "q    - выход из программы\n"
   "cvs  - переход под управление КП54\n"
   "\n");

printf(
   "candrv - инф.от драйвера CAN\n"
   "canv <0|1> - вкл/выкл печать сообщений CAN\n"
   "abinfo - опрос абонентов CAN\n"
   "tk <N> - указать номер рабочего ТК\n"
   "\n");

printf(
   "Приказы управления ВУ:\n"
   "vu <код элемента> - опрос элемента\n"
   "vu <код элемента> <код состояния> - переключение элемента\n"
   "vo - открыть затвор\n"
   "vz - закрыть затвор\n"
   "\n");
printf(
   "Приказы управления ЦСН:\n"
   "cd  -  вывести текущие показания датчиков (и энкодеров)\n"
   "cp <A> <L> <R> - переброс по трем осям\n"
   "cpa <A> - переброс по A\n"
   "cpl <L> - переброс по L\n"
   "cpr <R> - переброс по R\n"
   "cv <va> <vl> <vr> - задать скорость по трем осям (град/с)\n"
   "cva <va> - задать скорость по A (град/с)\n"
   "cvl <vl> - задать скорость по L (град/с)\n"
   "cvr <vr> - задать скорость по R (град/с)\n"
   "cc - начать отработку ЧВ\n"
   "cs  -  остановить движение\n"
   "\n");
printf(
   "Прочие приказы\n"
   "obn   - выполнить обнаружение\n"
// "omr   - выполнить распознавание звезд по каталогу\n"
   "zvezda - наведение на звезду из списка\n"
   "gsko <дпт> - наведение на ГСКО по ДПТ\n"
   "ah    - выведение по заданному азимуту и высоте\n"
   "ad    - выведение по заданным альфа,дельта\n"
   "koord - выдать угловые координаты полей зрения ОТК\n"
   "bz    - выполнить БЗ в автономном режиме\n"
   );


   } else if (strcmp(name,"candrv")==0) {
      CAN_STATS sta;
      int ncan, res;
      printf("CAN driver info\n");
      for(ncan=0; ncan<=1; ncan++) {
        printf("CHANNEL %d", ncan);
        if (fcan[ncan]<0) {
          printf(" -- no driver\n");
          continue;
        }
        printf("\n");
        res=ioctl(fcan[ncan],CAN_STAT,&sta);
        printf("recv:%6d read:%6d send:%6d writ:%6d ovrn:%6d errs:%6d\n"
          ,sta.q_recv,sta.q_read
          ,sta.q_send,sta.q_writ
          ,sta.q_ovrn,sta.q_errs);
      }

  } else if (strcmp(name,"canv")==0) {
     extern int can_verbose_mode;
     if (argc>=2) can_verbose_mode = atoi(argv[1]);
     printf("can_verbose_mode = %d\n", can_verbose_mode);

  } else if (strcmp(name,"abinfo")==0) {

     int ncan, minadr=1, maxadr=15, adr, i;
     
//   if (argc > 1)
//     minadr = maxadr = atoi(argv[1]);
       
     for(ncan=0; ncan<=1; ncan++) {

       printf("CHANNEL %d", ncan);

       if (fcan[ncan]<0) {
          printf(" -- no driver\n");
          continue;
       }

       printf("\n");

       for(adr=minadr; adr<=maxadr; adr++) 
         for(i=0; i<5; i++) 
           CAN_abn_info[adr][i]=0;
       
       for (adr=minadr; adr<=maxadr ; adr++) {
         can_req2(fcan[ncan],adr,CAN_CMD_GETP,m2b('T','X')); // can-send
         usleep(1000);
         can_req2(fcan[ncan],adr,CAN_CMD_GETP,m2b('R','X')); // can-recv
         usleep(1000);
         can_req2(fcan[ncan],adr,CAN_CMD_GETP,m2b('E','R')); // can-error
         usleep(1000);
         can_req2(fcan[ncan],adr,CAN_CMD_GETP,m2b('#','V')); // version-numb
         usleep(1000);
         can_req2(fcan[ncan],adr,CAN_CMD_GETP,m2b('#','D')); // version-date
         usleep(1000);
       }
     
       printf("ABON  VERSION   DATE      N_SEND    N_RECV    N_ERR\n");
             //ssss  XXXXXXXX  XXXXXXXX  XXXXXXXX  XXXXXXXX  XXXXXXXX
       for (adr=minadr; adr<=maxadr ; adr++) {
         if (CAN_abn_info[adr][0] != 0 || CAN_abn_info[adr][1] != 0) {
            printf("%4s  %08X  %08X  %08X  %08X  %08X\n",
                   can_abn_name(adr),
                   CAN_abn_info[adr][0],
                   CAN_abn_info[adr][1],
                   CAN_abn_info[adr][2],
                   CAN_abn_info[adr][3],
                   CAN_abn_info[adr][4]);
         }
       }
     }

   } else if (strcmp(name,"tk")==0) {
	int maska[5], ntk;

        if (argc==1) {
          printf("Используются ТК:"); 
          for(ntk=1; ntk<=MAXTK; ntk++) 
            if (RABTK[ntk]) printf(" %d",ntk);
          printf("\n"); 
          goto end_tk;
        }

        for(ntk=1; ntk<=MAXTK; ntk++) 
          maska[ntk]=0;

        for(i=1;i<argc;i++) {
          ntk=atoi(argv[i]);
          if (ntk<1 || ntk>MAXTK) {
            printf("Неверный номер ТК: %d\n",ntk); goto end_tk;
          }
          maska[ntk]=1;
        }
        
        NUMRABTK=0;
        for(ntk=1; ntk<=MAXTK; ntk++) {
          RABTK[ntk] = maska[ntk];
          NUMRABTK += RABTK[ntk];
        }
end_tk: ;

   } else if (strcmp(name,"test")==0) {
     void test_proc(void);
     test_proc();

   } else if (strcmp(name,"testkdpo")==0) {
     void testkdpo(void);
     testkdpo();
   } else if (strcmp(name,"ctkinfo")==0) {
     int ntk=askTK();
     if (ntk<0) continue;
     CTK_info(ntk);

   } else if (strcmp(name,"timectk")==0) {
     int ntk=askTK();
     if (ntk<0) continue;
     CTK_checktime(ntk);

  } else if (strcmp(name,"ctkreconnect")==0) {int ntk=askTK();CTK_reconnect(ntk);
  
  } else if (strcmp(name,"ctkreset")==0) {int ntk=askTK();CTK_reset(ntk);
  
   /* Команды управления ВУ */
   
   } else if (strcmp(name,"vu")==0) {
     int elem, sost;
     if (argc <= 1) goto badkom;

     elem = atoi(argv[1]);
     
     if (argc == 2) {
       can_req2(fcan_vu, CAN_ABN_KVU, CAN_CMD_GETP, elem);
     } else {
       sost = atoi(argv[2]);
       can_req6(fcan_vu, CAN_ABN_KVU, CAN_CMD_PUTP, elem, sost);
     }
   

   } else if (strcmp(name,"vo")==0) {
      if (VU_OTKR(RABTK))
        printf("Затвор открыт\n");

   } else if (strcmp(name,"vz")==0) {
      if (VU_ZAKR(RABTK))
        printf("Затвор закрыт\n");

   /* Команды управления ЦСН */

  } else if (strcmp(name,"csnv")==0) {
     extern int csn_verbose_mode;
     if (argc>=2) csn_verbose_mode = atoi(argv[1]);
     printf("csn_verbose_mode = %d\n", csn_verbose_mode);

   } else if (strcmp(name,"cs")==0) {
      stopcsn();
      if (RESCSN())
        printf("ЦСН переведен в режим ожидания\n");

   } else if (strcmp(name,"cd")==0) {
      printf("ЦД:  A=%8.4f  L=%8.4f  R=%8.4f\n", cd[0], cd[1], cd[2]);
      ask_encoder(OS_A);
      ask_encoder(OS_L);
      ask_encoder(OS_R);
      usleep(10000);
      printf("encoder:  A=%d  L=%d  R=%d\n", encoder[0], encoder[1], encoder[2]);


   } else if (strcmp(name,"cp")==0) {
      double A,L,R;
      if (argc>=2) A=atof(argv[1]); else A=999.0;
      if (argc>=3) L=atof(argv[2]); else L=999.0;
      if (argc>=4) R=atof(argv[3]); else R=999.0;
      
      docum_perebroscsn(A,L,R);


   } else if (strcmp(name,"cpa")==0) {
      double A=atof(argv[1]);
      docum_perebroscsn(A,999,999);

   } else if (strcmp(name,"cpl")==0) {
      double L=atof(argv[1]);
      docum_perebroscsn(999,L,999);

   } else if (strcmp(name,"cpr")==0) {
      double R=atof(argv[1]);
      docum_perebroscsn(999,999,R);

   } else if (strcmp(name,"cv")==0) {
      double va, vl,vr;
      if (argc>=2) va=atof(argv[1]); else va=999;
      if (argc>=3) vl=atof(argv[2]); else vl=999;
      if (argc>=4) vr=atof(argv[3]); else vr=999;

      docum_skorcsn(va,vl,vr);
              
   } else if (strcmp(name,"cva")==0) {
      double va =atof(argv[1]);
      docum_skorcsn(va,999,999);
              
   } else if (strcmp(name,"cvl")==0) {
      double vl =atof(argv[1]);
      docum_skorcsn(999,vl,999);

   } else if (strcmp(name,"cvr")==0) {
      double vr =atof(argv[1]);
      docum_skorcsn(999,999,vr);

   } else if (strcmp(name,"cc")==0) {
      double va,vl,vr;
      skorchvcsn(_cdf(0),_cdf(1),_cdf(2),&va,&vl,&vr);
      printf("Скорости отработки ЧВ: va=%7.5f vl=%7.5f vr=%7.5f (град/с)\n", 
            va,vl,vr);
      docum_skorcsn(va,vl,vr);
      //skorcsn(va,vl,vr);
 } else if (strcmp(name,"modstar")==0) {int pz =  ModSTAR(); printf("\n pzModSTAR:%d",pz);
   } else if (strcmp(name,"zkt")==0) { CATSTAR  star;//=new [sizeof(CATSTAR)];CAT c;
       //for(long ij=0;ij<100000;ij++){
     findstar( zkt,44946191,star )  ;
    // if(star.mag>=6.5 &&star.mag<=7)
      printf("\nПосле findstar: N=%ld alfa=%lf delta=%lf mag=%lf sp=%c%c\n",star.N,star.alfa,star.delta,star.mag,star.sp[0],star.sp[1]);
       //}
startreadcircle(zkt,0,0,4);

      while(1) {
        if (!readstar(zkt,star)) break;
        if (star.mag > 14 && star.mag < 17) {
          //printf("alfa=%f delta=%f mag=%f\n", star.alfa, star.delta,      star.mag);
            printf("\nПосле readstar: N=%ld alfa=%lf delta=%lf mag=%lf sp=%c%c\n",star.N,star.alfa,star.delta,star.mag,star.sp[0],star.sp[1]);
        }
      }
       //readstar(zkt,star);
       
   } else if (strcmp(name,"maxskora")==0) {
       if (argc>=2) maxskor[OS_A] = atof(argv[1]);
       printf("max skor A = %6.3f\n", maxskor[OS_A]);

   } else if (strcmp(name,"maxskorl")==0) {
       if (argc>=2) maxskor[OS_L] = atof(argv[1]);
       printf("max skor L = %6.3f\n", maxskor[OS_L]);

   } else if (strcmp(name,"maxskorr")==0) {
       if (argc>=2) maxskor[OS_R] = atof(argv[1]);
       printf("max skor R = %6.3f\n", maxskor[OS_R]);

   } else if (strcmp(name,"maxskor")==0) {
       if (argc>=2) 
         maxskor[OS_A] = maxskor[OS_L] = maxskor[OS_R] = 
           atof(argv[1]);
       printf("max skor A = %6.3f\n", maxskor[OS_A]);
       printf("max skor L = %6.3f\n", maxskor[OS_L]);
       printf("max skor R = %6.3f\n", maxskor[OS_R]);

   /* Команды управления АПОИ (=спецвычислителем) */
   } else if (strcmp(name,"apoiinfo")==0) {

       APOI_connect();
      
       printf("Спецвычислитель\n");
       printf("ip: %s\n", APOI_ADDR);
       printf("port: %d\n", APOI_PORT);

       if (APOI_connected) {
         printf("Соединение установлено\n");
         printf("Принято байт: %d\n",APOI_nbytes_read);
         printf("Отправлено байт: %d\n",APOI_nbytes_write);
       } else {
 
        printf("Соединение не установлено\n");
       }
 
   } else if (strcmp(name,"obn")==0) {
     int ntk, res;
     res = IZMERAPOI(RABTK);
     if (res) {
       for(ntk=1; ntk<=MAXTK; ntk++) {
         if (RABTK[ntk])
           printf("ЦТК %d  обнаружено %d обьектов (%d звезд, %d КО)\n", 
                  ntk, KolobjAPOI[ntk], KZVAPOI[ntk], KKOAPOI[ntk]); 
       }
     }

   } else if (strcmp(name,"resobn")==0) {
       int ntk, i;
       for(ntk=1; ntk<=MAXTK; ntk++)
         if (RABTK[ntk]) {
           printf("ЦТК %d   обнаружено %d звезд, %d КО\n", ntk, KZVAPOI[ntk], KKOAPOI[ntk]); 
           for(i=0; i<KolobjAPOI[ntk]; i++) {
             nObj *obj = &ObjAPOI[ntk][i];
             printf("N=%3d T=%10.4f X=%6.2f Y=%6.2f VX=%6.2f VY=%6.2f S=%6.2f V=%6.2f KO=%2d\n",
                 i, obj->T, obj->X, obj->Y, obj->DX, obj->DY, obj->S, obj->V, obj->KO); 
           } 
         }
     
    /* Команды выведения */

    } else if (strcmp(name,"ad")==0||strcmp(name,"ah")==0) {
      double g1=0,h1=0,m1=0,s1=0, g2=0,m2=0,s2=0,
          va,vl,vr, alfa, delta, A, L, R, Az, H;
        char s[80];
        int ntk=askTK();
        if (ntk<0) continue;
        if (strcmp(name,"ad")==0) {
          printf("введите альфа(ч м с) дельта(г м с):\n");
          gets(s);
          sscanf(s,"%lf%lf%lf%lf%lf%lf",&h1,&m1,&s1,&g2,&m2,&s2);
          alfa=hmshour(h1,m1,s1); delta=gmsgrad(g2,m2,s2);
          printf("альфа= "); printhms(alfa,5);
          printf("  дельта= "); printgms(delta,3,4); printf("\n");
          alfa*=M_PI/12; delta*=M_PI/180;
          adAHsea(alfa,delta,rvtime,&Az,&H);
        } else {
          printf("введите азимут(г м с) угол места(г м с):\n");
          gets(s);
          sscanf(s,"%lf%lf%lf%lf%lf%lf",&g1,&m1,&s1,&g2,&m2,&s2);
          Az=gmsgrad(g1,m1,s1); H=gmsgrad(g2,m2,s2);
          Az*=M_PI/180; H*=M_PI/180;
        }
	printf ("Азимут=%6.2f Угол места=%6.2f\n",
                Az*(180/M_PI),H*(180/M_PI));
        //if (H<10*M_PI/180) goto fig;
        if (!NavedShort(ntk,Az,H,0,0,&A,&L,&R)) goto fig;
        printf ("A=%7.2f L=%6.2f R=%6.2f\n",A,L,R);
        printf("навести? (y/n) ");
        gets(s);
        if (s[0]!='y'&&s[0]!='Y') continue;
        perebroscsn(A,L,R);
        if (!RESCSN()) goto otkaz;
        skorchvcsn(A,L,R,&va,&vl,&vr);
        skorcsn(va,vl,vr);
        continue;
fig:    printf("Наведение невозможно\n");

   } else if (strcmp(name,"zvezda")==0) {
     zvezda_proc();

   } else if (strcmp(name,"gsko")==0) {
     double dpt;
 
     if (argc<2) goto badkom;
     dpt = atof(argv[1]);
     gsko_proc(dpt);


  /* Команды определения координат */
   } else if (strcmp(name,"koord")==0) {
      int ntk, ntkmin, ntkmax; double A, L, R;

      A=_cdf(0); L=_cdf(1); R=_cdf(2);
      printf ("A=%7.2f L=%7.2f R=%7.2f\n",A,L,R);

      for(ntk=1; ntk<=MAXTK; ntk++) 
        if (RABTK[ntk]) {
  	  double Az, H, alfa, delta, t, dpt, a2000, d2000;
          absmet(ntk,A,L,R,0,0,&Az,&H);
          MinusRefraction(&H);
          AHtd(Az,H,&t,&delta);
          alfa=tasea(t,rvtime);
          dpt=t_DPT(t);
	  printf ("TK %d:  Az=%7.2f H=%7.2f alfa=%7.2f delta=%7.2f chasug=%7.2f dpt=%7.2f\n",
		 ntk, Az*RG, H*RG, alfa*RH, delta*RG, t*RH, dpt*RG);

          int ha,ma; double sa; char sign='+'; int gd, md; double sd;
          a2000=alfa; d2000=delta;
          TekJ2000sea(rvtime,&a2000,&d2000);
          hourhms(a2000*RH, &ha,&ma,&sa);
          if (d2000<0) { sign='-'; d2000*=-1; }
          gradgms(d2000*RG, &gd,&md,&sd);
      	  printf ("       alfa2000=%02d %02d %04.1f  delta2000=%c%02d %02d %02.0f\n",
                   ha,ma,sa, sign,gd,md,sd);
       }

   } else if (strcmp(name,"omr")==0) {
     int ntk, res;
     if (!IZMERAPOI(RABTK)) goto otkaz;
     for(ntk=1; ntk<=MAXTK; ntk++) {
       if (!RABTK[ntk]) continue;
       printf("ЦТК %d  обнаружено %d обьектов (%d звезд, %d КО)\n", 

              ntk, KolobjAPOI[ntk], KZVAPOI[ntk], KKOAPOI[ntk]); 
       if (KZVAPOI[ntk]>0) {
         MetodOMR(ntk);
         print2OMR(ntk);
       }
     }

   } else if (strcmp(name,"izmer")==0) {
     UPRST &u=uprst;
     int ntk, res;

     if (!IZMERAPOI(RABTK)) goto otkaz;
     for(ntk=1; ntk<=MAXTK; ntk++) {
       if (!RABTK[ntk]) continue;
       printf("ЦТК %d  обнаружено %d обьектов (%d звезд, %d КО)\n", 
              ntk, KolobjAPOI[ntk], KZVAPOI[ntk], KKOAPOI[ntk]); 
     }

     for(ntk=1; ntk<=MAXTK; ntk++) {
       if (!RABTK[ntk]) continue;
       if (KKOAPOI[ntk]>0)
         MetodOMR(ntk);
     }
     IZMERKO(RABTK);
     if (u.kolko>0 || (IMITAPOI && CVS==1))
        OBRKO(RABTK,1);

   } else if (strcmp(name,"obrdump")==0) {
     int obrdump(char *filename);
     char fname[100] = "dump/input";
     
     if (argc>=2) strcpy(fname,argv[1]);
     obrdump(fname);
     

#ifdef  BL051
  } else if (strcmp(name,"poiskv")==0) {poiskv(); //POISKV
#endif


#ifdef  BL051
}   else if (strcmp(name,"oshnaved")==0) {int tk=1;
       printf("\n Задайте номер ТК    (1 или 2) \n");
                   gets(line); sscanf(line,"%d",&tk);
                             max_osh_naved(tk);

#endif
#ifdef  BL051
}   else if (strcmp(name,"oshchv")==0) {int tk=1;
       printf("\n Задайте номер ТК    (1 или 2) \n");
                   gets(line); sscanf(line,"%d",&tk);
                  max_osh_chv(tk);

#endif

#ifdef  BL051
}   else if (strcmp(name,"skoizm")==0) {int tk=1;
       printf("\n Задайте номер ТК    (1 или 2) \n");
                   gets(line); sscanf(line,"%d",&tk);
                    max_SKO_izm(tk);

#endif


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Baikov
#ifdef  BL051
}   else if (strcmp(name,"modlw")==0) {modlw();
#endif

#ifdef  BL051
}   else if (strcmp(name,"modkw")==0) {modkw();
#endif

#ifdef  BL051
}   else if (strcmp(name,"modcd3")==0) {modcd3();
#endif
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! Baikov

#ifdef  BL051
 }   else if (strcmp(name,"blesk")==0) {int tk=1;
       printf("\n Задайте номер ТК    (1 или 2) \n");
                   gets(line); sscanf(line,"%d",&tk); blesk(tk);
 }   else if (strcmp(name,"nepr")==0) {  nepr_job();
#endif



   } else if (strcmp(name,"pasp")==0) {
     int n0=1, n1=100;
     if (argc>=2) n0=atoi(argv[1]);
     if (argc>=3) n1=atoi(argv[2]);
     testpasp(n0,n1);
//   } else if (strcmp(name,"poiskv")==0) {poiskv();

} else if (strcmp(name,"poz")==0) { //proverka pozition проверка позиционирования



   } else if (strcmp(name,"bz")==0) {
     int tip;

     printf("введите тип БЗ: 0 - сканирование, 1 - обн. в точке, "
            "2 - продолж. сканир.\n"
            "                3 - поиск вдоль траектории\n");
     gets(line); sscanf(line,"%d",&tip);

     if (tip==0 || tip==2) { //сканирование
       if (tip==0) TYPEKV(bz)=POISK_KV;
       if (tip==2) TYPEKV(bz)=PRPOISK_KV;
       TYPESK_LZ(bz)=NEPODV_SK;

       printf("ДПТ центра зоны (град.):"); gets(line);
       sscanf(line,"%lf",&DPT_LZ(bz));
       printf("ДЕЛЬТА центра зоны (град.):"); gets(line);
       sscanf(line,"%lf",&DELTA_LZ(bz));
       printf("длина зоны (град.):"); gets(line);
       sscanf(line,"%lf",&DELALFA_LZ(bz));
       printf("ширина зоны (град.):"); gets(line);
       sscanf(line,"%lf",&DELDEL_LZ(bz));
       printf("разворот зоны (град.):"); gets(line);
       sscanf(line,"%lf",&R_LZ(bz));
       ISFON_LZ(bz)=0;

     } else if (tip==1) { //обн.в точке
       TYPEKV(bz)=SBORKI_KV;
       printf("Азимут (град.):"); gets(line);
       sscanf(line,"%lf",&A_KI(bz));
       printf("Угол места (град.):"); gets(line);
       sscanf(line,"%lf",&H_KI(bz));
       printf("Требуется фотометрирование? (y/n)"); gets(line);
       if (line[0]=='Y'||line[0]=='y') {
         NI_KI(bz)=REALIZ_NI;
         printf("Время фотометрирования (с):"); gets(line);
         sscanf(line,"%lf",&TIMEFOT_NI(bz));
       } else
         NI_KI(bz)=NO_NI;
       T_KI(bz)         = time_kpst2kbp(rvtime); 
       VA_KI(bz)        = 0;
       VH_KI(bz)        = 0;
       BLESK_KI(bz)     = 0;
       MAXPOL_KI(bz)    = 0;
       MAXTIME_KI(bz)   = 0;
       ISFON_KI(bz)     = 0;

     } else if (tip==3) { //поиск вдоль траектории
       int vvod_bz_VdolTr(char *bz);
       if (!vvod_bz_VdolTr(bz)) continue;
     }

     print_bz(bz);

     printf("выполнять? (y/n)"); gets(line);
     if (line[0]!='y'&&line[0]!='Y') continue;

     ispoln_bz(bz);

   } else if (strcmp(name,"vsk")==0||strcmp(name,"cvs")==0) {

     CVS=1; return;

   } else if (strcmp(name,"ko")==0) {
       int nspis=0;
       long nko; double shift_t=0, tf=0;
       int nrep=1,silent=0,switch_f=0,i;
       if (argc>1) {
         if (memcmp(argv[1],"sp",2)==0) goto select;

	 nko=atol(argv[1]);
	 if (argc>=3) shift_t=atof(argv[2]);
         for(i=2;i<argc;i++) {
           if (strcmp(argv[i],"f")==0) {
             if (i+1<argc) tf=atof(argv[i+1]);
             if (tf==0) tf=20;
           }
           if (strcmp(argv[i],"r")==0) {
             if (i+1<argc) nrep=atoi(argv[i+1]);
             if (nrep==0) nrep=10;
           }
           if (strcmp(argv[i],"sw")==0) switch_f=1;
         }
         if (nrep>1) silent=1;

         for(i=0;i<nrep;i++) {
            if (nrep>1) {
              printf("Измерение N%d\n",i+1);
            }
	    if (!koproc(nko,shift_t,tf,silent)) break;
            if (sterror||breakcode) break;
         }

       } else {
select:
         nspis=atoi(&argv[1][2]);
	 select_ko(nspis);
       }

   } else if (strcmp(name,"polus")==0||strcmp(name,"pronic")==0) {
     int ntk=askTK();

     int nabl_polus(int ntk);

    if(TIPST==59)
     {
     can_req6(fcan_vu,CAN_ABN_KVU,CAN_CMD_PUTP,_PEREKLUCH,ntk);   // switch between ctk ntk=1 to SHPK NTK=2 to BIK
     usleep(3000000);
     if(ntk==1)  //SHPK
     {
          can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('E','T'), 1000);
          usleep(200);
          can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('B','S'), 255);
          usleep(200);
          can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('B','X'), 1);
          usleep(200);
          can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('B','Y'), 1);
          usleep(200);
          can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('T','R'), 0);
          usleep(200);
          can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('K','A'),3846 ); //analog amplifier
          usleep(200000);
     }
     else if(ntk==2)  //BIK
     {
         can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('E','T'), 400);
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('B','S'), 255);
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('B','X'), 1);
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('B','Y'), 1);
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('T','R'), 2);
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('K','A'),3846 ); //analog amplifier 15 db
         usleep(200);
         can_req2(fcan_ctk,CAN_ABN_CTK(ntk),CAN_CMD_EXTRA,0x8011); // EM on
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('E','M'),10240 ); //EM amplifier 40db

         usleep(100000);
     }
    }

     nabl_polus(ntk);


   } else if (strcmp(name,"stat")==0) {
     print_stat_kp54();

   } else if (strcmp(name,"takt")==0) {
      printf("nomertakta=%ld\n",nomertakta);
    
   } else {
badkom: printf("Неверный приказ:'%s'\n",line); continue;
   }

otkaz:   
   printsterror();
 }

}


//===================================================================//
//===================================================================//

enum { 
  RES_OK    = 1, 
  RES_IMIT  = 2, 
  RES_NODRV = 3, 
  RES_DEAD  = 4
};

int test_kdpo(void)
{
  if (IMITOS[OS_A] && IMITOS[OS_L] && IMITOS[OS_R]) return RES_IMIT;
  if (fcan_kdpo<=0) return RES_NODRV;
  if (timecd[OS_A] < rvtime-1 || timecd[OS_L] < rvtime-1 || timecd[OS_R] < rvtime-1) 
    return RES_DEAD; 
  return RES_OK;
}

int test_kep(int os)
{
  int adr = CAN_ABN_KEPA + os;
  if (IMITOS[os]) return RES_IMIT;
  if (fcan_kep<=0) return RES_NODRV;
  CAN_msg_flag[adr] = 0;
  can_req2(fcan_kep, adr, CAN_CMD_GETP, m2b('P',' '));
  usleep(1000);
  if (CAN_msg_flag[adr]==0) return RES_DEAD;
  return RES_OK;
}

int test_ctk(int nctk)
{
  int adr = CAN_ABN_CTK1 + nctk - 1;
  if (IMITCTK) return RES_IMIT;
  if (fcan_ctk<=0) return RES_NODRV;
  CAN_msg_flag[adr] = 0;
  can_req2(fcan_ctk, adr, CAN_CMD_GETP, m2b('B','X')); 
  usleep(1000);
  if (CAN_msg_flag[adr]==0) return RES_DEAD;
  return RES_OK;
}

int test_kvu(void)
{
  int adr = CAN_ABN_KVU;
  if (IMITVU) return RES_IMIT;
  if (fcan_vu<=0) return RES_NODRV;
  CAN_msg_flag[adr] = 0;
  can_req2(fcan_vu, CAN_ABN_KVU, CAN_CMD_GETP, _ZATVOR1);
  usleep(1000);
  if (CAN_msg_flag[adr]==0) return RES_DEAD;
  return RES_OK;
}

int test_apoi(void)
{
  if (IMITAPOI) return RES_IMIT;
  APOI_connect();
  if (!APOI_connected) return RES_DEAD;
  return RES_OK;
}

void print_res_test(int res)
{
  char *txt;
  switch(res) {
    case RES_IMIT:  txt = "включена имитация"; break;
    case RES_NODRV: txt = "нет драйвера CAN";  break;
    case RES_DEAD:  txt = "не отвечает";       break;
    case RES_OK:    txt = "вроде жив";         break;
  }
  printf("%s\n", txt);
}

void test_proc(void)
{
  int res, i;
  printf("КДПО     ");  res=test_kdpo();     print_res_test(res);
  printf("КЭП-A    ");  res=test_kep(OS_A);  print_res_test(res);
  printf("КЭП-L    ");  res=test_kep(OS_L);  print_res_test(res);
  printf("КЭП-R    ");  res=test_kep(OS_R);  print_res_test(res);
  for(i=1;i<=MAXTK;i++) {
    printf("ЦТК %d    "); res=test_ctk(i);   print_res_test(res);
  }
  printf("КВУ      ");  res = test_kvu();    print_res_test(res);
  printf("Спецвыч. ");  res = test_apoi();   print_res_test(res);
}

void testkdpo(void){
docum_perebroscsn(190,999,999);
delaytakt(30);
docum_perebroscsn(-190,999,999);
delaytakt(30);
docum_perebroscsn(0,90,90);
}
//===================================================================//
//===================================================================//

int gsko_proc(double dpt)
{
  int ntk;
  double chasug, delta, Az, H, A, L, R;
  char s[100];

  DPTdelta_tdelta(dpt*GR, 0, &chasug, &delta);
  tdAH(chasug, delta, &Az, &H);
  printf("Az=%6.2f H=%6.2f\n", Az*RG, H*RG);
  if (H<0) { printf("Под горизонтом\n"); goto bad; }
  if (H<5*GR) { printf("Низко над горизонтом\n"); goto bad; }

  ntk=askTK();
  if (ntk<0) return 0;

  Naved(ntk, Az, H, POLUS, 180, 0, 0, &A, &L, &R);
  if (L<MINCD(OS_L)+0.5 || L>MAXCD(OS_L)-0.5 || 
      R<MINCD(OS_R)+0.5 || R>MAXCD(OS_R)-0.5) {
    if (!NavedShort(ntk,Az,H, 0,0, &A,&L,&R)) goto bad;
  }
  printf ("A=%7.2f L=%6.2f R=%6.2f\n",A,L,R);
  printf("навести? (y/n) ");
  gets(s);
  if (s[0]!='y'&&s[0]!='Y') return 0;
  perebroscsn(A,L,R);
  if (!RESCSN()) return 0;
  return 1;
bad:
  printf("Наведение невозможно\n");
  return 0;
}


//===================================================================//
//===================================================================//


int read_stars_file(int printall, int p_nstar, double *p_alfa, double *p_delta)
{
  char *fname = "dan/stars.txt";
  FILE *f;
  char str[200];
  char *p;
  double alfa, delta;
  double g, h, m, s; int sign;
  int nstar, n;

  if ((f=myfopen(fname,"rt"))==NULL) return 0;
  
  while (fgets(str,200,f)!=NULL) {

    p = strstr(str,"//"); if (p!=NULL) *p=0;
    p = strchr(str,'#');  if (p!=NULL) *p=0;
    p=str;
    while(*p==' ' || *p=='\t') p++;
    if (*p=='\n' || *p=='\r' || *p==0) continue;

    n = sscanf(str, "%d", &nstar);
    if (n==0) goto skip;
    
    p = strstr(str,"alfa");
    if (p==NULL) goto skip;
    h=0; m=0; s=0;
    n = sscanf(p, "alfa=%lf%lf%lf", &h, &m, &s);
    if (n==0) goto skip;
    alfa = (h + m/60 + s/3600) * HR;

    p = strstr(str,"delta");
    if (p==NULL) goto skip;
    g=0; m=0; s=0; sign=1;
    n = sscanf(p, "delta=%lf%lf%lf", &g, &m, &s);
    if (n==0) goto skip;
    if (g<0) { sign=-1; g=-g; }
    delta = sign * (g + m/60 + s/3600) * GR;

    if (nstar==p_nstar) {
      if (p_alfa!=NULL) *p_alfa = alfa;
      if (p_delta!=NULL) *p_delta = delta;
      printf("%s", str);
      return 1;
    }

skip:
    if (printall) printf("%s", str);
  }

  return nstar;    
}


int naved_zvezda(int ntk, double alfa2000, double delta2000)
{
  double alfa, delta, Az, H, A, L, R, 
     A1, L1, R1, Az1, H1, alfa1, delta1, r,
     va, vl, vr;
  int i;

  alfa=alfa2000; delta=delta2000;
  J2000Teksea(rvtime, &alfa, &delta);

  for(i=0; i<3; i++) {
    adAHsea(alfa,delta,rvtime,&Az,&H);
    if (H<5*GR) goto bad;
    PlusRefraction(&H);
    Naved(ntk, Az, H, POLUS, 90, 0, 0, &A, &L, &R); //ориентация "север сверху"
    if (L<MINCD(OS_L)+0.5 || L>MAXCD(OS_L)-0.5 ||   //если не удалось, то произвольная
        R<MINCD(OS_R)+0.5 || R>MAXCD(OS_R)-0.5) {
      if (!NavedShort(ntk,Az,H, 0,0, &A,&L,&R)) goto bad;
    }
    printf("\n               Az=%7.3f           H=%7.3f",Az/GR,H/GR);
    printf("\n  A=%7.3f L=%7.3f R=%7.3f\n",A,L,R);  //#avm#
    perebroscsn(A,L,R);
    if (!RESCSN()) return 0;
//    stopcsn();                 //19.06.13  //#avm# 10.07.2013
    A1=cd[OS_A]; L1=cd[OS_L]; R1=cd[OS_R];
    absmet(ntk,A1,L1,R1,0,0,&Az1,&H1);
    MinusRefraction(&H1);
    AHadsea(Az1,H1, rvtime, &alfa1,&delta1);
    r = duga(alfa, delta, alfa1, delta1);
    //printf("i=%d r=%f\n", i, r*RG);
    if (r < 1./60*GR) break;
  }

  if (delta < 89*GR) {
    skorchvcsn(A1,L1,R1,&va,&vl,&vr);
    skorcsn(va,vl,vr);
  }
  return 1;
bad:
  printf("Наведение невозможно\n");
  return 0;
}


int zvezda_proc(void) 
{
  int ntk;
  char str[100];
  double alfa2000=0, delta2000=0, alfa, delta, Az, H;
  int nstar, ori, res;

  res=read_stars_file(1,0,0,0);
  if (res==0) return 0;

  printf("введите номер:");
  gets(str);
  res=sscanf(str,"%d",&nstar);
  if (res==0) return 0;

  res=read_stars_file(0,nstar,&alfa2000,&delta2000);
  if (alfa2000==0 && delta2000==0) {
    printf("Неверный номер\n"); return 0;
  }

  ntk=askTK();
  if (ntk<0) return 0;
  alfa=alfa2000; delta=delta2000;
  J2000Teksea(rvtime, &alfa, &delta);
  adAHsea(alfa,delta,rvtime,&Az,&H);
  printf("Az=%6.2f H=%6.2f\n", Az*RG, H*RG);
  if (H<0) { printf("Под горизонтом\n"); return 0; }
  if (H<5*GR) { printf("Низко над горизонтом\n"); return 0; }

  res = naved_zvezda(ntk, alfa2000, delta2000);
  if (res==0) return 0;
  printf("Выполнено наведение ТК %d на звезду %d\n", ntk, nstar);
  return 1;

}
