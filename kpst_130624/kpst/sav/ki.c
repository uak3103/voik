#include <stdio.h>
#include <stdlib.h>
//#include <dos.h>
#include <math.h>
//#include <mem.h>
#include <malloc.h>
#include <string.h>

#include "ki/ki.h"
#include "globst.h"
#include "rv/rv.h"
#include "st.h"
#include "ki/prognozt.c"
#include "ki/orlova.c"
#include "ki/moon&sun.c"
#include "ki/readts.c"

/*
//Старая версия (кеплеровская)
void mykeplergnsk(double *kep, double jd, double t, double *gnsk)
{
 kep[7]-=kep[6]; //из арг.шир. в ист.аном.
 keplergnsk(&kep[1],(jd-kep[0])*86400.+t,&gnsk[1]);
 kep[7]+=kep[6]; //из ист.аном. в арг.шир.
 gnsk[0]=jd;
 gnsk[1]=t;
}
*/

// Новая версия (с использованием прогноза Т)
// 23.04.03
void mykeplergnsk(double *kep, double jd, double t, double *gnsk)
{
	double dt=(jd-kep[0])*86400.+(t-kep[1]),
			a=kep[2],
			e=kep[3],
			i=kep[4],
			Om=kep[5],
			om=kep[6],
			u=kep[7],
			X,Y,Z,VX,VY,VZ;
	prognoz_T(a,e,i,Om,om,u,0,dt,&X,&Y,&Z,&VX,&VY,&VZ);
	gnsk[0]=jd;
	gnsk[1]=t;
	gnsk[2]=X;
	gnsk[3]=Y;
	gnsk[4]=Z;
	gnsk[5]=VX;
	gnsk[6]=VY;
	gnsk[7]=VZ;
}

int mygnskkepler(double *gnsk, double *kep)
{
  double fikt1; int nzp;
  kep[0]=gnsk[0];
  kep[1]=gnsk[1];
  elorko(gnsk[2],gnsk[3],gnsk[4],gnsk[5],gnsk[6],gnsk[7],
         0,
         &kep[2],&kep[3],&kep[5],&kep[6],&kep[4],&kep[7],
         &fikt1, &nzp
        );
  if (nzp==0) return 1; else return 0;
}

void mytarget(double *kep, double jd, double t,
              double *alfa, double *delta)
{
  double gnsk[8];
  double x,y,z;
  mykeplergnsk(kep,jd,t,gnsk);
  cksn(jd,t,&x,&y,&z);
  DekSfer(gnsk[2]-x,gnsk[3]-y,gnsk[4]-z,alfa,delta);
}


double jdsoln,asoln,dsoln,Asoln,Hsoln;
VECT3 vsoln;
double jdluna,aluna,dluna,Aluna,Hluna;

void getsoln(double jd, double t)
{
  double jds=jd+t/86400.;
  if (fabs(jds-jdsoln)>0.01) {
    adsoln(jd,t,&asoln,&dsoln);
    vsoln=SferVect(asoln,dsoln);
    jdsoln=jds;
  }
  adAH(jd,t,asoln,dsoln,&Asoln,&Hsoln);
}

void getluna(double jd, double t)
{
  double jdl=jd+t/86400.;
  if (fabs(jdl-jdluna)>0.01) {
    adluna(jd,t,&aluna,&dluna);
    jdluna=jdl;
  }
  adAH(jd,t,aluna,dluna,&Aluna,&Hluna);
}

int vteni(double *gnsk)
{
  double s; VECT3 ko,v;
  getsoln(gnsk[0],gnsk[1]);
  ko.x=gnsk[2]; ko.y=gnsk[3]; ko.z=gnsk[4];
  s=ko*vsoln;
  if (s>0) return 0;
  else {
    v=ko-vsoln*s;
    if (dlvect(v)<6378.) return 1; else return 0;
  }
}



double  DtIPM(double jd)
{int dd, mm, gg;
 JDtoDMY(jd, &dd, &mm, &gg);
 return gg*10000.+mm*100.+dd;
}

double  VrIPM(double t)
{double cc, hh, ss;
 cc = floor(t/3600.); t -= cc*3600.;
 hh = floor(t/60.);
 ss = t-hh*60.;
 return cc*10000.+hh*100.+ss;
}

extern "C" void	prognoz_IPM( double dt, double tm,
		             double dto, double tmo,
		             double xx[], double yy[] );

void prognozIPMgnsk(double *gnsk0,
                    double jd, double t, double *gnsk)
{
 double gnsk1[8];
 double jd0=gnsk0[0], t0=gnsk0[1];
 int i;
rep: if (t>=86400.) { t-=86400.; jd++; goto rep; }
     if (t<0)       { t+=86400.; jd--; goto rep; }
 gnsk[0]=jd;
 gnsk[1]=t;
 t0+=3.*3600.;  if(t0>24.*3600.) {t0-=24.*3600.; jd0++;}
 t+=3.*3600.;   if( t>24.*3600.)  {t-=24.*3600.; jd++;}


 memcpy(gnsk1,gnsk0,8*sizeof(double));
 for(i=2;i<5;i++) gnsk1[i]/=1000;
#if (IPM==1)
  prognoz_IPM(DtIPM(jd), VrIPM(t), DtIPM(jd0), VrIPM(t0),
              &gnsk1[2], &gnsk[2]);
#else
#endif
 for(i=2;i<5;i++) gnsk[i]*=1000;
}


void prognozIPMkep(double *kep0,
                   double jd, double t, double *kep)
{
  double gnsk[8];
  double jd0=kep0[0], t0=kep0[1];
  mykeplergnsk(kep0,jd0,t0,gnsk);
rep: if (t>=86400.) { t-=86400.; jd++; goto rep; }
     if (t<0)       { t+=86400.; jd--; goto rep; }
  prognozIPMgnsk(gnsk,jd,t,gnsk);
  mygnskkepler(gnsk,kep);
}


int nevyaz(double *gnsk, int N, double *izm,
           double *nev, double *skoalfa, double *skodelta)
{
  double kep[8];
  double jd,t,a,d,a1,d1,da,dd,sa=0,sd=0;
  int i;
  if (!mygnskkepler(gnsk,kep)) return 0;
  for(i=0;i<N;i++) {
    jd=izm[5*i];
    t =izm[5*i+1];
    a =izm[5*i+2];
    d =izm[5*i+3];

    mytarget(kep,jd,t,&a1,&d1);

    da=a-a1;
    dd=d-d1;
    if      (da> M_PI) da-=2*M_PI;
    else if (da<-M_PI) da+=2*M_PI;
    da*=cos(d);
    if (nev!=NULL) {nev[2*i]=da; nev[2*i+1]=dd;}
    sa+=da*da;
    sd+=dd*dd;
  }
  if (skoalfa!=NULL)  *skoalfa=sqrt(sa/N);
  if (skodelta!=NULL) *skodelta=sqrt(sd/N);
  return 1;


}

double getblesk(double privblesk, double daln, double faz, double H)
{
  double w=1/sin(H)-1,
         Satm=1+w-w*(0.0018167-w*(0.002875-0.0008083*w)),
         Patm=0.87,
         Kfaz=(sin(faz)+(M_PI-faz)*cos(faz))/M_PI,
         Kdaln=1000000/(daln*daln);
  return privblesk-2.5*(log10(Kfaz*Kdaln)+Satm*log10(Patm));
}

/* Описано в ki.h
struct USLVIDKO { //условия видимости КО на момент t
  long nko;
  double jd, t;
  double a, d, va, vd;
  double Kad[4]; double osh_vdol, osh_pop;
  double A, H, VA, VH;
  int ten;
  double skor, vys, daln, faz, blesk,
         rluna, Hsoln;
};
*/

void get_uslvidKO(double *gnsk, double *KOV, double jd, double t, USLVIDKO &u)
{
  double g[8], a,d,a1,d1,A,H,A1,H1,Asoln,Hsoln,
         x,y,z;
  u.jd=jd; u.t=t;
  if (gnsk[0]!=jd || gnsk[1]!=t) {
    double kep[8]; mygnskkepler(gnsk,kep); mykeplergnsk(kep,jd,t,g);
  } else
    memcpy(g,gnsk,sizeof(g));
  u.vys=sqrt(g[2]*g[2]+g[3]*g[3]+g[4]*g[4])-6378.;
  cksn(jd,t,&x,&y,&z);
  x=g[2]-x; y=g[3]-y; z=g[4]-z;
  DekSfer(x,y,z,&a,&d);
  adAH(jd,t,a,d,&A,&H);
  u.a=a; u.d=d; u.A=A; u.H=H;
  u.daln=sqrt(x*x+y*y+z*z);
  cksn(jd,t+1,&x,&y,&z);
  x=g[2]+g[5]-x; y=g[3]+g[6]-y; z=g[4]+g[7]-z;
  DekSfer(x,y,z,&a1,&d1);
  adAH(jd,t+1,a1,d1,&A1,&H1);
  u.va=a1-a; u.vd=d1-d;
  u.VA=A1-A; u.VH=H1-H;
  u.skor=duga(a,d,a1,d1);
  memset(u.Kad,0,sizeof(u.Kad)); u.osh_vdol=0; u.osh_pop=0;
  getsoln(jd,t);
  getluna(jd,t);
  u.ten=vteni(g);
  u.faz=M_PI-duga(asoln,dsoln,a,d);
  adAH(jd,t,asoln,dsoln,&Asoln,&Hsoln);
  u.Hsoln=Hsoln;
  if (Hluna<0||H<0) u.rluna=100;
    else u.rluna=duga(aluna,dluna,a,d);
  if (u.ten||H<0) u.blesk=100;
    else u.blesk=getblesk(5,u.daln,u.faz,H);
}

/*
struct IZMER {
  char tipst,nporst,ntk,met,avoik,res1,res2,res3;
  double jd,t, A,H,VA,VH, alfa,delta, alfa2000, delta2000, valfa,vdelta,
	 sko,vsko, blesk;
  long Nko, NMko, N54j6;
};
*/

void name_for_izmer(char *s)
{
  int day, month, year;
  JDtoDMY(JDsea,&day,&month,&year);
  sprintf(s,"tmpdan/ki/%02d_%02d_%02d.izm",day,month,year%100);
}

int save_izmer(IZMER &izm)
{
  char name[100];
  FILE *f;
  double jd,t; int day, month, year, hour, min; double sec;
  int nst;
  name_for_izmer(name);
  create_dirs(name);
  if ((f=myfopen(name,"at"))==NULL) return 0;
  jd=izm.jd; t=izm.t+TZ*3600.;
  if (t>86400.) { jd++; t-=86400.; }
  JDtoDMY(jd,&day,&month,&year);
  hourhms(t/3600.,&hour,&min,&sec);
  nst=izm.tipst-55+5*(izm.nporst-1);

//A=67.290176 h=14.950181 date=14/10/02 t=69466.987011 maxpol=0.016667 maxtime=1.000000
//blesk=11.455044 vA=-0.012021 vh=-0.012926 st=9 tk=1 met=1
//alfa=242.537686 delta=-7.635010 valf=0.002647 vdel=-0.017312 kak=1 ch=9001
//number=9001 NMko=2017001 N54j6=0

  fprintf(f,"A=%f h=%f date=%02d/%02d/%02d t=%f maxpol=%f maxtime=%f\n"
	    "blesk=%f vA=%f vh=%f st=%d tk=%d met=%d\n"
	    "alfa=%f delta=%f valf=%f vdel=%f kak=%d ch=%ld\n"
	    "number=%ld NMko=%ld N54j6=%d\n\n",
	    izm.A*RG, izm.H*RG, day, month, year%100, t, izm.sko*RG, 0.0,
	    izm.blesk, izm.VA*RG, izm.VH*RG, nst, izm.ntk, izm.met,
	    izm.alfa2000*RG, izm.delta2000*RG, izm.valfa*RG, izm.vdelta*RG,
	    0, 0L,
	    izm.Nko, izm.NMko, izm.N54j6);
  fclose(f);
  return 1;
}

void clear_izmer(void)
{
  char name[20]; FILE *f;
  name_for_izmer(name);
  if ((f=fopen(name,"w"))==NULL) return;
  fclose(f);
}

void sravni_izmer(double *kep, IZMER &izm, NEVYAZ &nev)
{
  VECT3 V,Vskor,Vvdol,Vpop,
       Vizm,Vizmskor,
       v1;
  double alfa0,delta0,
         alfa1,delta1;
  Vizm=SferVect(izm.alfa,izm.delta);
  v1=  SferVect(izm.alfa+izm.valfa, izm.delta+izm.vdelta);
  Vizmskor = v1-Vizm;
  mytarget(kep,izm.jd,izm.t,  &alfa0,&delta0); V =SferVect(alfa0,delta0);
  mytarget(kep,izm.jd,izm.t+1,&alfa1,&delta1); v1=SferVect(alfa1,delta1);
  Vskor=v1-V;
  Vvdol=Vskor-V*(V*Vskor); Norm(Vvdol);
  Vpop=V^Vvdol;
  nev.vdol=(Vizm-V)*Vvdol;
  nev.pop =fabs((Vizm-V)*Vpop);
  nev.t_vdol=nev.vdol/dlvect(Vskor);
  nev.dv =dlvect(Vizmskor-Vskor);
  nev.dv_otn =nev.dv/dlvect(Vskor);
}

int identif(IZMER &izm, int maxkolko, long *mnko, NEVYAZ *mnev)
{
  char *ts_fname = "tmpdan/tc";
  TC11503 ts;
  double kep[8];
  int res;
  int kolko=0;
  if (!startreadTC11503(ts_fname)) return -1;
  while(1) {
    NEVYAZ &nev = mnev[kolko];
    res=readTC11503(ts);
    if (res==-1) goto end;
    //if (ts.Nko==10782) continue;
    TC_to_kep(ts,kep);
    sravni_izmer(kep,izm,nev);
    double vdol=ts.Da, pop=ts.Dv;
    if (vdol<0.2) vdol=0.2;
    if (pop <0.2) pop =0.2;
    if ( (fabs(nev.t_vdol)<60 || fabs(nev.vdol)<vdol*GR) &&
          nev.pop<pop*GR && nev.dv<3*izm.vsko ) {
      izm.Nko=ts.Nko;
      izm.NMko=ts.NMko;
      mnko[kolko]=ts.Nko;
      kolko++;
      if (kolko==maxkolko) goto end;
    }
  }
end:  endreadTC11503();
  return kolko;
}

