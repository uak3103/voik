#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "inter.h"
#include "st.h"
#include "globst.h"
#include "rv/rv.h"
#include "csn/csn.h"

void prognoz_T(
        double a, double e, double i, double Om, double om, double u,
        double dT, double t,
        double *X, double *Y, double *Z, double *VX, double *VY, double *VZ);
double Tperebros(double A0, double L0, double R0,
                 double A1, double L1, double R1);
void current_jd_t(double *jd, double *t);
int NavedKO(int NTK,
            int fot_flag,
            double T, double Az, double H, double VAz, double VH,
            double *A, double *L, double *R);
void getvxvy(double A, double L, double R,
             double Az, double H, double VAz, double VH);
int OBNTCHK(double A, double L, double R);
extern int maintk58;
int getcatko(long Nko, long *NMko, double *kep);

double glob_exkep[10];
int vdol_flag=0;

void exkeplergnsk(double *exkep, double jd, double t, double *gnsk)
{
	double dt=(jd-exkep[0])*86400.+(t-exkep[1]),
			a=exkep[2],
			e=exkep[3],
			i=exkep[4],
			Om=exkep[5],
			om=exkep[6],
			u=exkep[7],
            DT=exkep[9],
			X,Y,Z,VX,VY,VZ;
	prognoz_T(a,e,i,Om,om,u,DT,dt,&X,&Y,&Z,&VX,&VY,&VZ);
	gnsk[0]=jd;
	gnsk[1]=t;
	gnsk[2]=X;
	gnsk[3]=Y;
	gnsk[4]=Z;
	gnsk[5]=VX;
	gnsk[6]=VY;
	gnsk[7]=VZ;
}

void extarget(double *exkep, double jd, double t,
              double *alfa, double *delta)
{
  double gnsk[8];
  double x,y,z;
  exkeplergnsk(exkep,jd,t,gnsk);
  cksn(jd,t,&x,&y,&z);
  DekSfer(gnsk[2]-x,gnsk[3]-y,gnsk[4]-z,alfa,delta);
}

VECT3 Vtarget(double *exkep, double jd, double t, double tvdol)
{
  double sav=exkep[1];
  double gnsk[8];
  double x,y,z;
  VECT3 V;
  exkep[1]-=tvdol;
  exkeplergnsk(exkep,jd,t,gnsk);
  exkep[1]=sav;
  cksn(jd,t,&x,&y,&z);
  V.x=gnsk[2]-x; V.y=gnsk[3]-y; V.z=gnsk[4]-z;
  Norm(V);
  Ekv2Gorv(V,SHIROTA,MZV(jd,t));
  return V;
}

double ugol_vect(VECT3 &V1, VECT3 &V2)
{
  double a=(V1*V2)/sqrt((V1*V1)*(V2*V2));
  if (a>=1) return 0;
  else if (a<=-1) return M_PI;
  else return acos(a);
}

double tvdol_to_vdol(double *exkep, double jd, double t, double tvdol)
{
  VECT3 V,V1;
  double R;
  V= Vtarget(exkep,jd,t,0);
  V1=Vtarget(exkep,jd,t,tvdol);
  R=ugol_vect(V,V1);
  if (tvdol<0) R=-R;
  return R;
}

double vdol_to_tvdol(double *exkep, double jd, double t, double vdol)
{
  double DT=0,
         A=exkep[2],
         period=2*M_PI*A*sqrt(A)/631.348113,
         maxd=0.1*period,
         r,r1,dr,dt;
  VECT3 V0,V,V1;
  V0=Vtarget(exkep,jd,t,0);
  for(int i=0;i<100;i++) {
    V=Vtarget(exkep,jd,t,DT);
    r=ugol_vect(V0,V); if (DT<0) r=-r;
    if (fabs(r-vdol)<1e-5) break;
    V1=Vtarget(exkep,jd,t,DT+1);
    r1=ugol_vect(V0,V1); if (DT+1<0) r1=-r1;
    dr=r1-r;
    if (dr<=0) break;
    dt=(vdol-r)/dr;
    if (fabs(dt)>maxd) {
      if (dt>0) dt=maxd; else dt=-maxd;
    }
    DT+=dt;
  }
  return DT;
}

void get_vdol_pop(double *exkep,
                  double jd_izm, double t_izm, double A_izm, double H_izm,
                  double &tvdol, double &vdol, double &pop)
{
  double jd=jd_izm, t=t_izm;
  VECT3 Vizm, V0, V, Vvdol, Vpop;
  double dt;
  Vizm=SferVect(A_izm,H_izm);
  V=Vtarget(exkep,jd,t,0);
  V0=V;
  Vvdol=Vtarget(exkep,jd,t,1)-V;
  tvdol=0;
  for(int i=0;i<100;i++) {
	dt=((Vizm-V)*Vvdol)/(Vvdol*Vvdol);
	tvdol+=dt;
    V=Vtarget(exkep,jd,t,tvdol);
    Vvdol=Vtarget(exkep,jd,t,tvdol+1)-V;
	if (fabs(dt)<1e-5) break;
  }
  vdol=ugol_vect(V0,V);
  if (tvdol<0) vdol=-vdol;
  Vpop=V^Vvdol;
  pop=ugol_vect(Vizm,V);
  if ((Vizm-V)*Vpop<0) pop=-pop;
}

void rvtime_to_jd_t(double rvtime, double *p_jd, double *p_t)
{
   double jd,t;
   jd=JDsea;
   t=rvtime;
   if (t>43200.) { t-=43200.; jd++; } else t+=43200.;
   t-=TZ*3600.;
   if (t<0) { jd--; t+=86400.; }
   *p_jd=jd;
   *p_t=t;
}

void PRINTKO_vdol(void)
{
 UPRST &u=uprst;
 printf(
 "NКО  ОТК   вдоль(°) вдоль(с)  поперек(°)"
// **   s    +***.**   +****     ***.**
 "\n");
 for(int nko=0;nko<u.kolko;nko++) {
   KOINF &ko=u.ko[nko];
   int ntk=ko.ntk;
   double jd, t, tvdol, vdol, pop;
   rvtime_to_jd_t(ko.t, &jd, &t);
   get_vdol_pop(glob_exkep, jd, t, ko.Az, ko.H, tvdol, vdol, pop);
   printf(
    " %2d   %c    %+7.2f%+8.0f    %6.2f\n",
    nko+1,otk_letter(TIPST,ntk), vdol*RG, tvdol, pop*RG
         );
  }
}

/*
void sravni_izmer(PARAM &par, KATKO &katko, NEVYAZ &nev)
{
  VECT3 V,V0,Vskor,Vvdol,Vizm,Vizmskor,v1;
  double a,d, a1,d1, jd=katko.jd, t=katko.t, sdvig=0, tvdol;
  AHad(jd,t,katko.A*GR,katko.H*GR,&a,&d); Vizm=SferVect(a,d);
  AHad(jd,t+1,(katko.A+katko.VA)*GR,(katko.H+katko.VH)*GR,&a,&d);
  Vizmskor=SferVect(a,d)-Vizm;
  mytarget_sdvig(par,jd,t,0,&a,&d); V=SferVect(a,d); V0=V;
  mytarget_sdvig(par,jd,t,1,&a1,&d1); v1=SferVect(a1,d1)-V;
  do {
	tvdol=((Vizm-V)*v1)/(v1*v1);
	sdvig+=tvdol;
	mytarget_sdvig(par,jd,t,sdvig,&a,&d); V =SferVect(a,d);
	mytarget_sdvig(par,jd,t,sdvig+1,&a1,&d1); v1=SferVect(a1,d1)-V;
  }
	while (fabs(tvdol)>1e-5);
  nev.vdol=acos(V0*V);
  nev.pop =acos(Vizm*V);
  nev.t_vdol=sdvig;
  mytarget_sdvig(par,jd,t+1,sdvig,&a1,&d1); Vskor =SferVect(a1,d1)-V;
  nev.dv =dlvect(Vizmskor-Vskor);
  nev.dv_otn =nev.dv/dlvect(Vskor);
}
*/

void paramKO_vdol_pop(double *exkep, double jd, double t,
   double tvdol, double pop,
   VECT3 &Vko, VECT3 &Vskor, VECT3 &Vvdol)
{
   VECT3 V1,Vpop;
   Vko=Vtarget(exkep,jd,t,tvdol);
   V1=Vtarget(exkep,jd,t,tvdol+1);
   Vskor=V1-Vko;
   V1=Vtarget(exkep,jd,t,tvdol+1);
   Vvdol=V1-Vko; Vvdol-=Vko*(Vko*Vvdol); Norm(Vvdol);
   Vpop=Vko^Vvdol;
   Vko=Vko*cos(pop)+Vpop*sin(pop);
}

int NavedKO_orient(int NTK,
            VECT3 Vko, VECT3 Vskor, VECT3 Vvdol,
            double razvorot,
            double *A, double *L, double *R)
{
  double t,A0,L0,R0,A1,L1,R1, Az,H,
         minL=MINCD(OS_L),
         maxL=MAXCD(OS_L),
         minR=MINCD(OS_R),
         maxR=MAXCD(OS_R);
  VECT3 VO,V1,V2,VX;
  double dt=5;
  A0=_cdf(0); L0=_cdf(1); R0=_cdf(2);
  t=dt;
  for(int i=0;i<3;i++) {
    VO = Vko+Vskor*t;
    V1 = Vvdol-VO*(VO*Vvdol); Norm(V1);
    V2=VO^V1;
    VX=V1*sin(razvorot)+V2*cos(razvorot);
    NAVED(NTK, VO, VX, 0,0, &A1,&L1,&R1);
    if (L1<minL||L1>maxL||R1<minR||R1>maxR) {
       VectSfer(VO,&Az,&H);
       if (!NavedShort(NTK, Az,H, 0,0, &A1,&L1,&R1))
           return 0;
    }
    t=dt+Tperebros(A0,L0,R0,A1,L1,R1);
  }
  *A=A1;
  *L=L1;
  *R=R1;
  return 1;
}

double shift_DT(double *exkep, double DT, double shift)
{
  double sav=exkep[1], jd, t, DT1;
  current_jd_t(&jd,&t);
  exkep[1]-=DT;
  DT1=DT+vdol_to_tvdol(exkep,jd,t,shift);
  exkep[1]=sav;
  return DT1;
}

struct PYRIN {
  double *exkep;
  double step, DTmax, DTmin, DTplus, DTminus, DT;
};


void get_PYRIN_first(PYRIN &p)
{
  p.DT=0;
  p.DTplus =shift_DT(p.exkep, 0,  p.step/2);
  p.DTminus=shift_DT(p.exkep, 0, -p.step/2);
}

int get_PYRIN_next(PYRIN &p)
{
   int plus= (p.DT>=0);
   int plusfinish=(p.DTplus>p.DTmax);
   int minusfinish=(p.DTminus<p.DTmin);

   if (plusfinish && minusfinish) return 0;

   if (plusfinish) plus=0;
   else if (minusfinish) plus=1;
   else if (plus && p.DTplus>1.5*fabs(p.DTminus)) plus=0;
   else if (!plus && fabs(p.DTminus)>1.5*p.DTplus) plus=1;

   if (plus) {
      p.DT     = shift_DT(p.exkep, p.DTplus,  p.step/2);
      p.DTplus = shift_DT(p.exkep, p.DT,      p.step/2);
   } else {
      p.DT      = shift_DT(p.exkep, p.DTminus, -p.step/2);
      p.DTminus = shift_DT(p.exkep, p.DT,      -p.step/2);
   }
   return 1;
}

int POISK_vdol(double *exkep,
               double DTnach, double DTkon, double pop_nach, double pop_kon,
               int NTK_naved, double perekr, double razvorot,
               char pyrin_flag)
{
   double step, DT,DR, step_pop, pop,
          jd,t,Az,H,Az1,H1, VAz,VH,
           A,L,R;
   VECT3 Vko,Vskor,Vvdol;
   int NTK;
   PYRIN p;

   vdol_flag=1;

   //step = step_pop = 28./pasF(NTK_naved)*(1-perekr);

  // step = TKsizeXmm(NTK)/pasF(NTK_naved)*(1-perekr);//titenko 28.03
  // step_pop = TKsizeYmm(NTK)/pasF(NTK_naved)*(1-perekr);//titenko 28.03

   step = TKsizeXmm(NTK_naved)/pasF(NTK_naved)*(1-perekr);
   step_pop = TKsizeYmm(NTK_naved)/pasF(NTK_naved)*(1-perekr);
   pop = pop_nach;
   if (pop_kon<pop_nach) step_pop*=-1;
   
   if (DTkon<DTnach) step*=-1;

   if (DTnach*DTkon>0) pyrin_flag=0;

next_pop:

   if (pop_nach!=0 || pop_kon!=0)
       printf("\nСмещение поперек орбиты %+5.2f°\n",pop*RG);

   if (pyrin_flag) {
     p.exkep=exkep;
     p.step=fabs(step);
     p.DTmin=min(DTnach,DTkon);
     p.DTmax=max(DTnach,DTkon);
     get_PYRIN_first(p);
     DT=p.DT;
   } else {
     DT=DTnach;
   }

   while(1) {
      current_jd_t(&jd,&t);
      paramKO_vdol_pop(exkep, jd,t, DT,pop, Vko, Vskor, Vvdol);
      VectSfer(Vko,&Az,&H);
      VectSfer(Vko+Vskor,&Az1,&H1); VAz=Az1-Az; VH=H1-H;
      DR=tvdol_to_vdol(exkep,jd,t,DT);
      printf("\n"
"Отрабатывается смещение %+5.2f° (%+2.0fc)  Азимут=%5.2f Угол места=%5.2f\n",
          DR*RG, DT, Az*RG, H*RG);
      if (razvorot<0) {
        if (!NavedKO(NTK_naved,0,rvtime,Az,H,VAz,VH, &A,&L,&R))
           goto no_naved;
      } else {
        if (!NavedKO_orient(NTK_naved, Vko,Vskor,Vvdol,razvorot, &A,&L,&R))
           goto no_naved;
      }
      //printf("A=%6.2f L=%6.2f R=%6.2f\n",A,L,R);
      getvxvy(A,L,R,Az,H,VAz,VH);
      //printf("vxa_glob=%f vya_glob=%f\n",vxa_glob,vya_glob);
      if (!OBNTCHK(A,L,R)) { vdol_flag=0; return 0; }

      if (pyrin_flag) {
        if (!get_PYRIN_next(p)) break;
        DT=p.DT;
      } else {
        double r=shift_DT(exkep,DT,step/2);
        if (step>0 && r>DTkon || step<0 && r<DTkon) break;
        DT=shift_DT(exkep,DT,step);
      }
   }
   
   pop+=step_pop;
   if (step_pop>0 && pop<=pop_kon ||
       step_pop<0 && pop>=pop_kon)
       goto next_pop;
   
   vdol_flag=0;
   return 1;
no_naved:
   printf("Угол места меньше допустимого. Наведение невозможно.\n");
   vdol_flag=0;
   return 1;
}

void print_bz_VdolTr(char *bz)
{
  double jd,t, tbeg=TBEG_LZV(bz), tend=TEND_LZV(bz),
     rbeg, rend, exkep[10];
  char str[10];
  int tk=TK_LZV(bz);
  memcpy(exkep, EXKEP_LZV(bz), 10*sizeof(double));
  exkep[0] = MJD_JD(exkep[0]);
  current_jd_t(&jd,&t);
  rbeg=tvdol_to_vdol(exkep,jd,t,tbeg);
  rend=tvdol_to_vdol(exkep,jd,t,tend);

/*  if (tk==1) strcpy(str,"ОТК-У");
    else if (tk==2) strcpy(str,"ОТК-Ш");*/
    if (tk==1) strcpy(str,"ОТК-1");//29.05.2013 krikunov
    else if (tk==2) strcpy(str,"ОТК-2");
    // else itoa(tk,str,10); // commented or Linux
    else sprintf(str,"%d",tk);
  printf(
 "   NКО           Смещение вдоль             поперек      ТК\n"
 "          нач.    кон.      нач.  кон.     нач.  кон.\n"
 "         (град)  (град)    (сек) (сек)   (град) (град)\n");
//******   ***.**  ***.**    ****  ****    ***.** ***.**  ОТК-У
  printf("%6ld  %7.2f %7.2f   %5.0f %5.0f   %7.2f%7.2f  %s\n",
      NKO_LZV(bz), rbeg*RG, rend*RG, tbeg, tend, SMPOPBEG_LZV(bz), SMPOPEND_LZV(bz), str);
}

void ispoln_bz_VdolTr(char *bz)
{
  double exkep[10];
  int ntk;
  memcpy(exkep, EXKEP_LZV(bz), 10*sizeof(double));
  exkep[0] = MJD_JD(exkep[0]);
  memcpy(glob_exkep,exkep,10*sizeof(double));
  ntk = TK_LZV(bz);
  if (ntk<1 || ntk>MAXTK || !RABTK[ntk]) {
    for(ntk=1; ntk<=MAXTK; ntk++)
      if (RABTK[ntk]) break;
  }
  POISK_vdol(exkep,
             TBEG_LZV(bz), TEND_LZV(bz), SMPOPBEG_LZV(bz)*GR, SMPOPEND_LZV(bz)*GR,
             ntk, KPER_LZV(bz), RAZV_LZV(bz)*GR,
             (PYRIN_LZV(bz)==1) );
}

int vvod_bz_VdolTr(char *bz)
{
  char line[80], *p;
  long Nko=0,NMko;
  int prizn;
  double exkep[10],jd,t,d1=0,d2=0;

  memset(bz,0,200);
  memset(exkep,0,10*sizeof(double));
  TYPEKV(bz)=POISKVDOL_KV;
  TK_LZV(bz)=1;
  KPER_LZV(bz)=0.3;
  printf("Номер КО:");
  gets(line); sscanf(line,"%ld",&Nko);
  if (!getcatko(Nko, &NMko, exkep)) {
    printf("В каталоге нет КО N %ld\n",Nko);
    return 0;
  }
  NKO_LZV(bz)=Nko;
  printf("Задавать смещение в град - g, в сек - c (умолч=g)");
  gets(line);
  if (line[0]=='c'||line[0]=='C')
     { prizn=1; p="сек"; }
  else
     { prizn=0; p="град"; }
  printf("Начальное смещение (%s):", p);
  gets(line); sscanf(line,"%lf",&d1);
  printf("Конечное смещение (%s):", p);
  gets(line); sscanf(line,"%lf",&d2);
  if (prizn==0) {
    current_jd_t(&jd,&t);
    TBEG_LZV(bz) = vdol_to_tvdol(exkep,jd,t,d1*GR);
    TEND_LZV(bz) = vdol_to_tvdol(exkep,jd,t,d2*GR);
  } else {
    TBEG_LZV(bz)=d1;
    TEND_LZV(bz)=d2;
  }
  printf("Смещение поперек начальное(в град; умолч=0):");
  gets(line); sscanf(line,"%lf",&SMPOPBEG_LZV(bz));
  printf("Смещение поперек конечное (в град; умолч=0):");
  gets(line); sscanf(line,"%lf",&SMPOPEND_LZV(bz));
  printf("Номер ТК (умолч=1):");
  gets(line); sscanf(line,"%d",&TK_LZV(bz));
  printf("Перекрытие (умолч=0.3):");
  gets(line); sscanf(line,"%lf",&KPER_LZV(bz));
  printf("Разворот (град; умолч=0):");
  gets(line); sscanf(line,"%lf",&RAZV_LZV(bz));
  printf("Спираль Пырина? (y/n; умолч=n):");
  gets(line);
  if (line[0]=='y'||line[0]=='Y') PYRIN_LZV(bz)=1;
  exkep[0] = JD_MJD(exkep[0]);
  memcpy(EXKEP_LZV(bz),exkep,10*sizeof(double));
  return 1;
}

int sample_bz_VdolTr(char *bz)
{
  long Nko,NMko;
  int prizn;
  double exkep[10],jd,t,d1,d2;

  memset(bz,0,200);
  memset(exkep,0,10*sizeof(double));

  TYPEKV(bz)=POISKVDOL_KV;
  Nko=10505;
  prizn=0;
  d1=0.3;
  d2=-0.3
  ;
  SMPOPBEG_LZV(bz)=0;
  SMPOPEND_LZV(bz)=0;
  TK_LZV(bz)=1;
  KPER_LZV(bz)=0.3;
  RAZV_LZV(bz)=-1;
  PYRIN_LZV(bz)=1;

  if (!getcatko(Nko, &NMko, exkep)) {
    printf("В каталоге нет КО N %ld\n",Nko);
    return 0;
  }
  NKO_LZV(bz)=Nko;
  exkep[0] = JD_MJD(exkep[0]);
  memcpy(EXKEP_LZV(bz),exkep,10*sizeof(double));
  if (prizn==0) {
    double *exkep=EXKEP_LZV(bz);
    current_jd_t(&jd,&t);
    TBEG_LZV(bz) = vdol_to_tvdol(exkep,jd,t,d1*GR);
    TEND_LZV(bz) = vdol_to_tvdol(exkep,jd,t,d2*GR);
  } else {
    TBEG_LZV(bz)=d1;
    TEND_LZV(bz)=d2;
  }
  return 1;
}

void GetKoordModel_1(double rvt,double *Az, double *H)
{
   VECT3 V;
   double jd,t;
   rvtime_to_jd_t(rvt, &jd, &t);
   V=Vtarget(glob_exkep, jd, t, 0);
   VectSfer(V,Az,H);
}
