#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h> 

#include "sys/sys.h"
#include "rv/rv.h"
#include "globst.h"

enum { MAXKKO=10, MAXKZV=3000, MAXKOLOBJ=3000 };

extern void CTK_reset(int ntk) ;

typedef struct {
          int nvyd;
	  float x,y,//- координаты объекта (мм);
                e,//- яркость объекта (фотометрич.ед.);
                vx,vy;//вектор скорости объекта (мм/с);
	  long takt;//- номер такта измерения для привязки координат;
	  float dt;//поправка ко времени относительно начала такта для привязки координат (с);
	}
	  KOtype;

typedef struct {
  	  float x,y, //x, y   - координаты звезды (мм);Диапазон изменения координат x и y - от -14 до 14; центр поля зрения имеет  
                      //координаты 0,0; координата y увеличивается вниз, x увеличивается вправо.
                e;    //     - яркость звезды (фотометрич.ед.);
	}
	  ZVtype;


//char APOI_ADDR[20] = "127.0.0.1";
//int APOI_PORT = 7216;

int APOI_sock = -1;
int APOI_connected = 0;
int APOI_nbytes_read = 0;
int APOI_nbytes_write = 0;


int NkadrAPOI[5];

int KolobjAPOI[5];
nObj *(ObjAPOI[5]);

int       KKOAPOI[5];//- количество обнаруженных КО
KOtype   *(KOAPOI[5]);

int       KZVAPOI[5];
long   TAKTZVAPOI[5];
ZVtype   *(ZVAPOI[5]);//структура, содержащая параметры звезды с номером i ; ZVAPOI[NTK] - указатель на массив параметров звезд;

int    STATUSAPOI[5];


/*
typedef struct {
    int Nkadr;
    float VXzv;
    float VYzv;
    int userv[10];
} puskprm;
*/

int APOI_pusk(int ntk, int Nkadr, double VXzv, double VYzv)
{  
  puskprm PP;
  int mas[20];
  int res;

  if (!APOI_connected) {
    sterror=1; rabst=rabapoi=2;
    strcpy(sterrmsg1,"ОЕФ УПЕДЙОЕОЙС У брпй");
    return 0;
  }

  PP.Nkadr = Nkadr;
  PP.VXzv  = VXzv;
  PP.VYzv  = VYzv;

  mas[0]=CAN_CMD_PUSK;
  memcpy (&mas[1], &PP, sizeof(PP));

  res = apoi_send(APOI_sock, ntk,'CMND',mas,sizeof(int)+sizeof(puskprm));//8);
  if (res<=0) {
    sterror=1; rabst=rabapoi=2;
    strcpy(sterrmsg1,"SP error");
    return 0;
  }

  APOI_nbytes_write += res;
  STATUSAPOI[ntk] = 1;
  return 1;
}



void* APOI_thread(void* notused)
{
  int res;

  while(1) {
    APOI_MSG msg;

    res=recv(APOI_sock,&msg,sizeof(APOI_MSG),0);
    if (res<=0) goto disconnected;
    APOI_nbytes_read += res;

//  printf("%d bytes read\n",res);
//  for(int i=0;i<res;i++) printf("%02X ",((char*)(&msg))[i]);
//  printf("\n");
//  printf("mark = %c%c%c%c  type = %c%c%c%c  nctk = %d  size = %d\n", 
//      msg.mark[3], msg.mark[2], msg.mark[1], msg.mark[0], 
//      msg.type[3], msg.type[2], msg.type[1], msg.type[0], 
//      msg.nctk,
//      msg.size);

    if (*(int*)msg.mark!='APOI') {
//    perror("bad_mark"); 
      continue;
    }

    switch (*(int*)msg.type)
    {
      case 'CRST':
      {
       int ntk = msg.nctk;
        KKOAPOI[ntk] = 0;
        KZVAPOI[ntk] = 0;
        STATUSAPOI[ntk]=0;
        CTK_reset(ntk);
       break;
      }
      case 'CONV':
      {
      int ntk = msg.nctk , 
          size = msg.size,
          nnn = size/sizeof(nObj),
          kko=0, 
          kzv=0,
          i;
      nObj *buf;

//      printf("size=%d Nobj=%d\n",size,nnn);

      if (size==0) {
        KKOAPOI[ntk] = 0;
        KZVAPOI[ntk] = 0;
        STATUSAPOI[ntk]=0;//krikunov
        continue;
      }

      buf = (nObj*)malloc(size);

      int s=size;
      char *p = (char*)buf;
      while (s>0) {
        res=recv(APOI_sock,p,s,0);
        if (res<=0) goto disconnected;
        s-=res;
        p+=res;
      }

      APOI_nbytes_read += size;

      if (nnn>MAXKOLOBJ) 
        ObjAPOI[ntk] = (nObj*)realloc(ObjAPOI[ntk], sizeof(nObj)*nnn);

      memcpy(ObjAPOI[ntk], buf, size);
      KolobjAPOI[ntk] = nnn;

      for(i=0; i<nnn; i++) {
        nObj *obj = &buf[i];
        double x,y;

//      printf("N=%3d X=%6.2f Y=%6.2f VX=%6.2f VY=%6.2f S=%6.2f KO=%2d\n",
//            i, obj->X, obj->Y, obj->DX, obj->DY, obj->S, obj->KO); 

        if (obj->KO == +1) {
          KOtype *ko = &KOAPOI[ntk][kko];//указатель на массив параметров КО;
          if (kko>=MAXKKO) continue;
          coord_pix2mm(ntk, obj->X, obj->Y, &x, &y);
          ko->x  = x;// mm
          ko->y  = y;//mm
          coord_pix2mm(ntk, obj->DX, obj->DY, &x, &y);
          ko->vx = x;// speed
          ko->vy = y;
          ko->e  = obj->V;
          kko++;
        } else if (obj->KO == -1) {
          ZVtype *zv = &ZVAPOI[ntk][kzv];
          if (kzv>=MAXKZV) continue;
          coord_pix2mm(ntk, obj->X, obj->Y, &x, &y);
          zv->x  = x;
          zv->y  = y;
          zv->e  = obj->V;
          kzv++;
        }
      }  

      free(buf);

      KKOAPOI[ntk] = kko;
      KZVAPOI[ntk] = kzv;
      STATUSAPOI[ntk]=0;
      
   //   printf("Nzv = %d  Nko = %d\n", KZVAPOI[ntk], KKOAPOI[ntk]); //krikunov 07.06.13
      }
      break;

      default:
      if (msg.size>0) {
        void *buf = malloc(msg.size);
        int s=msg.size;

        while (s>0) {
          res=recv(APOI_sock,buf,s,0);
          if (res<=0) goto disconnected;
          s-=res;
        }

        APOI_nbytes_read += msg.size;
//      printf("%d bytes read\n", res);
        free(buf);
      }
      break;
    }
     
  }

disconnected:
  APOI_connected = 0;
  sterror=1; rabst=rabapoi=2;
  strcpy(sterrmsg1,"ОЕФ УПЕДЙОЕОЙС У брпй");
  return 0;
}


int APOI_connect(void)
{
  int res;
  pthread_t tid;

  if (APOI_connected) return 1;

  APOI_sock = socket(AF_INET, SOCK_STREAM, 0); 
  res = make_connect(APOI_sock, APOI_ADDR, APOI_PORT);

  if (res<0) return 0;

  APOI_connected = 1;
  APOI_nbytes_read = 0;
  APOI_nbytes_write = 0;
  pthread_create(&tid,0,&APOI_thread,NULL);

  return 1;
}



int INITAPOI(int maxTK)
{
  int ntk;
  char *msg;

  for ( ntk=1; ntk<=maxTK; ntk++ ) {
	ObjAPOI[ntk] = (nObj*)malloc(sizeof(nObj)*MAXKOLOBJ);
	KOAPOI[ntk] = (KOtype*)malloc(sizeof(KOtype)*MAXKKO);
	ZVAPOI[ntk] = (ZVtype*)malloc(sizeof(ZVtype)*MAXKZV);
	STATUSAPOI[ntk]=0;
  }

  APOI_connect();
  return 1;
}
