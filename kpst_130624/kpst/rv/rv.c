#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "../common/candev.h" 
#include "../common/elements.h"
#include "sys/sys.h"
#include "globst.h"
#include "rv/rv.h"
#include "inter.h"
#include "rv/dump.c"
#include "rv/candev.c"
#include "rv/ipx.c"


#define INTERVAL 100000

long nomertakta;
int sterror;
char sterrmsg1[80];
char sterrmsg2[80];
char sterrmsg3[80];
int rabst=2,
    rabvu=2, rabcsn=2,
    rabksio=2, rabfmk=2,
    rabapoi=2, rabasev=2;

int rvday, rvmonth, rvyear, rvhour, rvmin; 
double rvsec;

double rvtime;
unsigned timer_takt;

int IMIT, IMITVU, IMITOS[3], IMITAPOI, IMITCTK;


void get_realtime(int *day, int *month, int *year,
             int *hour, int *min, double *sec)
{
  struct timeval tv;
  struct tm *ptm;

  gettimeofday( &tv, NULL);

  ptm=localtime(&tv.tv_sec);
  *day   = ptm->tm_mday;
  *month = ptm->tm_mon+1;
  *year  = ptm->tm_year+1900;
  *hour  = ptm->tm_hour;
  *min   = ptm->tm_min;
  *sec   = ptm->tm_sec + tv.tv_usec/1000000.;
}

double hms_to_rvtime(int h, int m, double s)
{
  double t;
  t = h * 3600.0 + m * 60 + s;
  if (h>=12) t-=43200.; else t+=43200.;
  return t;
}


void taktvu(void);
void taktcsn(void);
void taktst(void);


int taktflag;


void taktovaya(void)
{
 int i,j;
 unsigned t,t1;

 taktflag=1;
 nomertakta++;

 get_realtime(&rvday, &rvmonth, &rvyear, &rvhour, &rvmin, &rvsec);
 rvtime = hms_to_rvtime(rvhour, rvmin, rvsec);

 taktvu();
 taktcsn();
 taktst();

 void inter_takt(void);

//  if ( ( IMIT && (nomertakta%25)==0 ) ||
//        ( asev && // (ms_asev/40)==(TIPST-56)*2+(NPORST-1)*10 ||
//                  ((long(t_asev*25))%24)==(TIPST-56)*2+(NPORST-1)*10 ||
//       !asev && (nomertakta%25)==0 ) )
//     inter_takt();

 if ( (nomertakta%10)==0 )
     inter_takt();
}


int timer_pipe[2];
int takt_sync_pipe[2];
int takt_sync_flag=0;

void timer_proc(int sig)
{ int masw=sig;
  write(timer_pipe[1],&masw,sizeof(masw));
}

void handl_timer(void (*sig_proc)(int))
{ struct sigaction sig_act;
  sigset_t         sig_set;
  sigprocmask(SIG_SETMASK,0,&sig_set);
  sig_act.sa_handler=sig_proc;
  sig_act.sa_mask   =sig_set;
  sig_act.sa_flags  =SA_RESTART;
  sigaction(SIGALRM,&sig_act,0);
}

void setup_timer(int inter,int value)
{ struct itimerval my_timer;
  my_timer.it_value   .tv_sec  = 0;
  my_timer.it_value   .tv_usec = value;
  my_timer.it_interval.tv_sec  = 0;
  my_timer.it_interval.tv_usec = inter;
  setitimer(ITIMER_REAL,&my_timer,0);
}

void* takt_thread(void *arg)
{ int masr;
  for(;;) {
    read(timer_pipe[0],&masr,sizeof(masr));
    taktovaya();
    if (takt_sync_flag) write(takt_sync_pipe[1],&nomertakta,sizeof(nomertakta));
  }
}

void start_takt_thread(void)
{
  pthread_t tid;
  if (pipe(timer_pipe)<0) {
    printf("Cannot open pipe. Exit!\n");
    exit(0);
  }
  if (pipe(takt_sync_pipe)<0) {
    printf("Cannot open pipe. Exit!\n");
    exit(0);
  } 
  handl_timer(timer_proc);
  setup_timer(INTERVAL,INTERVAL);
  pthread_create(&tid,0,takt_thread,0);
}

int initrv(void)
{
  pthread_t tid;

  get_realtime(&rvday, &rvmonth, &rvyear, &rvhour, &rvmin, &rvsec);
  rvtime = hms_to_rvtime(rvhour, rvmin, rvsec);

  standard_dump_names(rvday,rvmonth,rvyear,rvhour);
  if (!open_text_dump()) return 0;

  initsys(0,ftextdump);

  for(int ncan=0; ncan<=1; ncan++) {
    char *driver;
  
    if (ncan==0) 
       driver="/dev/can0"; 
    else
       driver="/dev/can1"; 

    fcan[ncan]=open(driver,O_RDWR);
    if (fcan[ncan] <= 0) { 
       printf("Не найден драйвер CAN%d!\n", ncan);
    } else {
      CAN_STRUC cfg;
      int res;
      cfg.code=0x0000;
      cfg.mask=0x07FF;
/*      
  switch (rate)
  {
   case  125: cfg.btr0=0x03; cfg.btr1=0x1C; break;
// case  250: cfg.btr0=0x01; cfg.btr1=0x1C; break;
   case  250: cfg.btr0=0x01; cfg.btr1=0x3A; break; // omega ctk
   case  500: cfg.btr0=0x00; cfg.btr1=0x1C; break;
   case 1000: cfg.btr0=0x00; cfg.btr1=0x14; break;
  }
*/  
     cfg.btr0=0x01; cfg.btr1=0x3A;  // omega ctk
  
      res=ioctl(fcan[ncan],CAN_CNFG,&cfg);
      printf("CAN_CNFG %d res=%d\n", ncan, res);

      res=ioctl(fcan[ncan],CAN_START,0);
      printf("CAN_START %d res=%d\n", ncan, res);

      pthread_create(&tid, 0, can_thread, (void*)ncan);
   } 
 }

 start_takt_thread();
 
 return 1;
}


void waittakt(long n)
{
  if (nomertakta>=n) return; 

  set_key_mode();
  
  takt_sync_flag=1;
    
  while(nomertakta<n) {
    long x; int k;
    char buf[10];
//  printf("Before pipe t=%ld     ",nomertakta);
    k=read(takt_sync_pipe[0],&x,sizeof(long));
//  printf("%d bytes read    ",k);
//  printf("After pipe t=%ld\n",nomertakta);

    memset(buf,0,10);
    k=read(0,buf,8);
    if (k>0) {
//    printf("key:");
//    for(int i=0;i<k;i++) printf("%02X ",buf[i]);
//    printf("'%s'",buf);
//    printf("\n");
      if (memcmp(buf,"\x1B\[21~",5)==0) {
         extern int breakcode;
         breakcode=1;
//	 printf("break!\n");
      }
    }
  }
  takt_sync_flag=0;
}

void delaytakt(int n)
{ 
  waittakt(nomertakta+n);
}

