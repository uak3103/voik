#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

#include "sys/sys.h"
#include "globst.h"
#include "coord.c"
#include "reductio.c"
#include "paspst.c"
#include "am.c"
#include "naved.c"
#include "zkt.c"
#include "omr.c"


int tipst;   //тип станции (56-60)
int nporst;  //порядковый номер станции (1-2)

int MAXTK; //макс.номер ТК

int ncan_vu   =1, //номер линии CAN для КВУ
    ncan_kdpo =0, //номер линии CAN для КДПО
    ncan_kep  =1, //номер линии CAN для КЭП-A, КЭП-L, КЭП-R 
    ncan_ctk  =0; //номер линии CAN для ЦТК-1 ... ЦТК-4

double
  mincd[3],    //мин.угол по осям (программное ограничение)
  maxcd[3],    //макс.угол по осям (программное ограничение)
  maxskor[3],  //макс.скорость по осям, град/с
  maxuskor[3]; //макс.ускорение по осям, град/с^2

char APOI_ADDR[20] = "127.0.0.1";  // IP-адрес спецвычислителя
int APOI_PORT = 7216;              // порт спецвычислителя


double JDsea;
double DAVLENIE=680, //мм.рт.ст.
       TEMPERATURA=+10; //°C

char zktpath[80]="dan/tycho2.zkt";
CAT zkt;
int zktflag;



double MZVsea(double time)
{ double s=GZV(JDsea,time+(12-TZ)*3600.)*(M_PI/43200)+DOLGOTA;
  if (s>2*M_PI) s-=2*M_PI;
  return s;
}

void AHtd(double A, double H, double *t, double *delta)
{ GorEkv1(A,H,SHIROTA,t,delta); }

void tdAH(double t, double delta, double *A, double *H)
{ Ekv1Gor(t,delta,SHIROTA,A,H); }

void DPTdelta_tdelta(double dpt, double deltageo,
		       double *t, double *deltatopo)
{
  double x0,y0,z0,x1,y1,z1,
  RZ = 6378.137,
  a  = 42164.14;
  x0 =  RZ * cos(SHIROTA);
  y0 =  0;
  z0 =  RZ * sin(SHIROTA);
  x1 =   a * cos(dpt-DOLGOTA) * cos(deltageo);
  y1 = - a * sin(dpt-DOLGOTA) * cos(deltageo);
  z1 =   a * sin(deltageo);
  DekSfer(x1-x0,y1-y0,z1-z0,t,deltatopo);
}

double t_DPT(double t)
{
 double dpt1=DOLGOTA-M_PI/2, dpt2=DOLGOTA+M_PI/2,dpt,t1,delta;
 while(dpt2-dpt1>1e-5) {
  dpt=(dpt2+dpt1)/2;
  DPTdelta_tdelta(dpt,0,&t1,&delta);
  if (t1-t>0&&t1-t<M_PI || t1-t<-M_PI)
    dpt1=dpt;
  else
    dpt2=dpt;
 }
 return dpt;
}

double atsea(double alfa, double time)
{ double t=MZVsea(time)-alfa;
  if(t<0) t+=2*M_PI;
  return t;
}
#define tasea atsea

void AHadsea(double A, double H, double time,
             double *alfa, double *delta)
{ GorEkv2(A,H,SHIROTA,MZVsea(time),alfa,delta); }

void adAHsea(double alfa, double delta, double time,
             double *A, double *H)
{ Ekv2Gor(alfa,delta,SHIROTA,MZVsea(time),A,H); }

void AHad(double jd58, double utc, double A, double H,
             double *alfa, double *delta)
{ GorEkv2(A,H,SHIROTA,MZV(jd58,utc),alfa,delta); }

void adAH(double jd58, double utc, double alfa, double delta,
             double *A, double *H)
{ Ekv2Gor(alfa,delta,SHIROTA,MZV(jd58,utc),A,H); }


void KatVidsea(double time, double *alfa, double *delta)
{ time=time;
  VECT3 v=SferVect(*alfa,*delta); v=NP*v+Vaber; VectSfer(v,alfa,delta);
}

void VidKatsea(double time, double *alfa, double *delta)
{ time=time;
  VECT3 v=SferVect(*alfa,*delta); v=NPT*(v-Vaber); VectSfer(v,alfa,delta);
}

void TekJ2000sea (double time, double *alfa, double *delta)
{ time=time;
  VECT3 v=SferVect(*alfa,*delta); v=NPT*v; VectSfer(v,alfa,delta);
}

void J2000Teksea(double time, double *alfa, double *delta)
{ time=time;
  VECT3 v=SferVect(*alfa,*delta); v=NP*v; VectSfer(v,alfa,delta);
}

void J1950J2000(double *alfa, double *delta)
{ double B1950=2433282.42346;
  MATR33 M=~PrecMatr(B1950);
  VECT3 v=SferVect(*alfa,*delta); v=M*v; VectSfer(v,alfa,delta);
}


void PlusRefraction(double *H)
{ *H+=refraction(M_PI/2-(*H),DAVLENIE,TEMPERATURA); }

void MinusRefraction(double *H)
{ double h1=*H,h2,h3,h4,h5;
  h2=*H-refraction(M_PI/2-h1,DAVLENIE,TEMPERATURA);
  h3=*H-refraction(M_PI/2-h2,DAVLENIE,TEMPERATURA);
  h4=*H-refraction(M_PI/2-h3,DAVLENIE,TEMPERATURA);
  h5=*H-refraction(M_PI/2-h4,DAVLENIE,TEMPERATURA);
  *H=h5;
  //PlusRefraction(&h5);
}

double DMYtoJD(double D, int M, int Y)
{ return 367L*Y-7*(Y+(M+9)/12)/4+(275*M)/9+D+1721013.5; }

int JDtoDMY(double JD, int *D, int *M, int *Y)
{
  double jd; int d,m,y;
  if (JD<2415079.5 || JD>=2488128.5) { (*D)=(*M)=(*Y)=0; return 0; }
  y=1900+(JD-2415019.5)/365.25;
  for(m=12;m>=1;m--)
   { jd=DMYtoJD(1,m,y); if (jd<=JD) { d=JD-jd+1; break; } }
  *D=d; *M=m; *Y=y;
  return 1;
}


double MZV(double JD, double time)
{ double s=GZV(JD,time)*(M_PI/43200)+DOLGOTA;
  if (s>2*M_PI) s-=2*M_PI;
  return s;
}


//****************************************************************
// Расчет геоцентрических координат точки на поверхности Земли
// Вх. параметры:
//   fi  - широта места (рад)
//   h   - высота над уровнем моря (км)
//   mzv - местное звездное время (рад)
// Вых. параметры:
//   X,Y,Z - прямоугольные геоцентрические координаты во
//           второй экваториальной системе (км)
// Источник: Бюракан
//****************************************************************
void gnskkoord(double fi, double h, double mzv,
	  double *x, double *y, double *z)
{
#define ekvradz 6378.137
#define fcompr (1./298.257222101 )
  double sinfi,cosfi,pa,sk,g1,g2,f=fcompr;
  sinfi=sin(fi);
  cosfi=cos(fi);
  pa=1-f*(2.-f)*sinfi*sinfi;
  sk=ekvradz/sqrt(pa);
  g1=sk+h;
  g2=(1-f)*(1-f)*sk+h;
  *x=g1*cosfi*cos(mzv);
  *y=g1*cosfi*sin(mzv);
  *z=g2*sinfi;
}

//****************************************************************
// Расчет геоцентрических координат комплекса
// на заданный момент
//****************************************************************
void cksn(double JD, double t, double *x, double *y, double *z)
{ gnskkoord(SHIROTA,VYSOTA/1000.,MZV(JD,t),x,y,z); }


typedef struct {
		double t;
		double a, l, r;
		float x, y, e, vx, vy;
	       } TKO;

typedef struct {
	  float x,y,e;
	} ZV;

// расчет скоростей ЧВ (град/сек )

void skorchvcsn_A (double A, double L, double R,
	       double* VA, double* VL, double* VR )
{
 #define VCHV (360/(23*3600.+56*60+4)) // скорость вращения Земли (ГРАД/СЕК)
 double sinshir=sin(SHIROTA), cosshir=cos(SHIROTA);
 R=R;
 *VA = (sinshir+cosshir*sin(A*(M_PI/180))/tan(L*(M_PI/180)))*VCHV;
 *VL = -cosshir*cos(A*(M_PI/180)) * VCHV;
 *VR = -cosshir*sin(A*(M_PI/180))/sin(L*(M_PI/180)) * VCHV;
}


void skorchvcsn_bezA (double A, double L, double R,
	       double* VA, double* VL, double* VR )
{
  double t=0,Az,H,a,d,X,Y;
  int ntk=1;
  absmet(ntk,A,L,R,0,0,&Az,&H);
  AHadsea(Az,H,t,&a,&d);
  adAHsea(a,d,t+1,&Az,&H);
  AHXY(ntk,A,L,R,Az,H,&X,&Y);
  *VA=0;
  *VL=-X/pasF(ntk)/sin(R*(M_PI/180))*(180/M_PI);
  *VR=Y/pasF(ntk)*(180/M_PI);
}

/************************************************************************/
/* решение системы линейных уравнений методом Гаусса                    */
/* с выбором главного элемента по столбцу                               */
/* входные параметры : n - размерность системы                          */
/*                     a - коэффициенты системы (массив размера n*n)    */
/*                     b - свободные члены (массив размера n)           */
/* выходной параметр : x - решение системы (массив размера n)           */
/* Содержимое массивов a и b не сохраняется.                            */
/* результат: 1 - решение найдено,                                      */
/*            0 - система вырождена                                     */
/************************************************************************/
/*
 struct date d ;    //текущая дата .
 char  pathfot[80] ;//путь для файлов фотометрирования
*/

int gauss (int n,  double* a, double* b, double* x )
{
#define el(i,j) a[n*i+j]
#define xchg(x,y) { t=x; x=y; y=t; }
 int i,j,k,j1;
 double t,d,dmax;

 for(j=0;j<n;j++) {
   dmax=0;
   for(i=j;i<n;i++)
     { d=fabs(el(i,j)); if (d>dmax) { dmax=d; k=i; } }
   if (dmax<1e-17) return 0;
   for(j1=j;j1<n;j1++) xchg(el(j,j1),el(k,j1));
   xchg(b[j],b[k]);
   for(i=j+1;i<n;i++) {
     t=-el(i,j)/el(j,j);
     for(j1=j+1;j1<n;j1++) el(i,j1)+=t*el(j,j1);
     b[i]+=t*b[j];
   }
 }

 for(j=n-1;j>=0;j--) {
   b[j]/=el(j,j);
   for(i=j-1;i>=0;i--) b[i]-=b[j]*el(i,j);
 }

 for(i=0;i<n;i++) x[i]=b[i];

 return 1;
}

/************************************************************************/
/* Аппроксимация функциональной зависимости полиномом степени 2         */
/* по методу наименьших квадратов.                                      */
/* входные параметры : n - количество отсчетов                          */
/*                     Т - массив значений аргумента (длины n)          */
/*                     Х - массив значений функции (длины n)            */
/*   (в точке T[i] функция принимает значение X[i], i=0,...,n-1)        */
/* выходные параметры : a0,a1,a2 - коэффициенты полинома:               */
/*                                 X(T) = a0 + a1*T + a2*T*T            */
/************************************************************************/
void approx2(int n, double *T, double *X,
            double *a0, double *a1, double *a2)
{
 double st=0, st2=0, st3=0, st4=0, sx=0, stx=0, st2x=0;
 double a[9], b[3], r[3];
 for(int i=0;i<n;i++) {
   double t,t2,t3,t4,x;
   t=T[i]; t2=t*t; t3=t2*t; t4=t3*t;
   x=X[i];
   st+=t; st2+=t2; st3+=t3; st4+=t4; sx+=x; stx+=t*x; st2x+=t2*x;
 }
 a[0]=n;   a[1]=st;  a[2]=st2;  b[0]=sx;
 a[3]=st;  a[4]=st2; a[5]=st3;  b[1]=stx;
 a[6]=st2; a[7]=st3; a[8]=st4;  b[2]=st2x;
 gauss(3,a,b,r);
 *a0=r[0]; *a1=r[1]; *a2=r[2];
}


char otk_letter(int tipst, int ntk)
{
  char c;
  if (tipst==58) c='0'+ntk;
    else c="УШ"[ntk-1];
  return c;
}


double gmsgrad(double g, double m, double s)
{ int sign=1;
  if (g<0) { sign=-1; g=-g; }
  return sign*(g+m/60+s/3600);
}
#define hmshour gmsgrad


void gradgms(double x, int* g, int* m, double *s)
{
 int sign=1;
 x*=3600;
 if (x<0) {sign=-1;x=-x;}
 *g=(int)(x/3600); x-=*g*3600.;
 *m=(int)(x/60); *s=x-*m*60;
 *g *= sign;
}

#define hourhms gradgms


void printgms(double x, int posg, int poss)
{
 int sign=1,g,m; double s;
 x*=3600;
 if (x<0) {sign=-1;x=-x;}
 g=(int)(x/3600); x-=g*3600.;
 m=(int)(x/60); s=x-m*60;
 if (g==0&&sign==-1) printf("%*s",posg,"-0");
 else printf("%*d",posg,g*sign);
 printf("°%02d'",m);
 if (poss>0) {
   if (poss>3) printf("%0*.*lf\"",poss,poss-3,s);
   else printf("%02d\"",(int)s);
 }
}

void printhms(double x, int poss)
{
 int h,m; double s;
 x*=3600;
 h=(int)(x/3600); x-=h*3600.;
 m=(int)(x/60); s=x-m*60;
 printf("%2dh %02dm",h,m);
 if (poss>0) {
   if (poss>3) printf(" %0*.*lfs",poss,poss-3,s);
   else printf(" %02ds",(int)s);
 }
}

double duga(double A1, double H1, double A2, double H2)
{
  return acos(cos(H1)*cos(H2)*cos(A1-A2)+sin(H1)*sin(H2));
}


int initglobst(int tipst, int nporst,
               int day,int month,int year,int hour)
{
 double JD=JulianDate(day,month,year);
 if (hour<12) JD-=1;
 JDsea=JD;
 init_reduction(JD+(12-TZ)/24.);

 if (!init_pasp(tipst,nporst)) return 0;

 for(int ntk=1; ntk<=MAXTK; ntk++)
   initOMR(ntk);

 zktflag=opencat(zkt,zktpath);
 if (!zktflag) {
   printf("Не открыт файл каталога %s\n",zktpath);
 }
 return 1;
}
