#ifndef _KI_H
#define _KI_H

//В процедурах расчета орбит используются:
//  jd  - юлианская дата;
//
//  t   - время в сек. от гринв.полночи;
//
//  kep - кеплеровские параметры движения.
//        Массив из 8 элементов: jd, t, a, e, i, ДВУ, арг.перигея, u;
//        все углы в рад;
//
//  gnsk - параметры движения в ГНСК.
//        Массив из 8 элементов: jd, t, X, Y, Z, VX, VY, VZ;
//        X,Y,Z в км; VX,VY,VZ в км/c;
//
//  izm - массив измерений;
//        по 5 эл-нтов на измерение: jd, t, альфа, дельта, СКО;
//        альфа, дельта, СКО в рад.

//перевод кеплеровских парам. в ГНСК на дату jd и время t
void mykeplergnsk(double *kep, double jd, double t, double *gnsk);

//перевод парам. ГНСК в кеплеровские
int mygnskkepler(double *gnsk, double *kep);
//результат: 1-нормальное завершение, 0-орбита не эллиптическая

//расчет видимых координат КО (альфа,дельта) на дату jd и время t
void mytarget(double *kep, double jd, double t,
              double *alfa, double *delta);


extern double jdsoln,asoln,dsoln,Asoln,Hsoln;
void getsoln(double jd, double t);
int vteni(double *gnsk);

extern double jdluna,aluna,dluna,Aluna,Hluna;
void getluna(double jd, double t);


void prognozIPMgnsk(double *gnsk0, double jd, double t, double *gnsk);

void prognozIPMkep(double *kep0, double jd, double t, double *kep);

#ifndef __TC11503__
#define __TC11503__
struct TC11503 {
  int Nts;
  int Nsr;
  long Nko, NMko;
  char Pstacko, Ppr, Pnki, Ptipnki, Ptipki;
  int Nprior, epoha;
  double Da, Dv, smz, dmz;
  char Pugz;
  double datanach, vrnach, datakon, vrkon, data, vr, dpt;
  double Tom, a, e, i, Om, om, U, DTom, M;
};
#endif


#ifndef __TIPNOMERA__
#define __TIPNOMERA__
  enum TIPNOMERA { NPOR, NVNUTR, NCKKP, NMEZD };
#endif

int startreadTC11503(char *fname);
int readTC11503(TC11503 &ts);
void endreadTC11503(void);
void TC_to_kep(TC11503 &ts, double *kep);
int getkatKO(int tipnum, long num, TC11503 &ts);

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

void get_uslvidKO(double *gnsk, double *KOV, double jd, double t, USLVIDKO &u);

struct IZMER {
  char tipst,nporst,ntk,met,avoik,res1,res2,res3;
  double jd,t, A,H,VA,VH, alfa,delta, alfa2000, delta2000, valfa,vdelta,
	 sko,vsko, blesk;
  long Nko, NMko, N54j6;
};

struct NEVYAZ {
  double vdol, pop, t_vdol; double dv, dv_otn;
};

int save_izmer(IZMER &izm);
int identif(IZMER &izm, int maxkolko, long *mnko, NEVYAZ *mnev);

#endif // _KI_H
