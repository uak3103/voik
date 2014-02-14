#ifndef _KI_H
#define _KI_H

//� ���������� ������� ����� ������������:
//  jd  - ��������� ����;
//
//  t   - ����� � ���. �� �����.�������;
//
//  kep - ������������ ��������� ��������.
//        ������ �� 8 ���������: jd, t, a, e, i, ���, ���.�������, u;
//        ��� ���� � ���;
//
//  gnsk - ��������� �������� � ����.
//        ������ �� 8 ���������: jd, t, X, Y, Z, VX, VY, VZ;
//        X,Y,Z � ��; VX,VY,VZ � ��/c;
//
//  izm - ������ ���������;
//        �� 5 ��-���� �� ���������: jd, t, �����, ������, ���;
//        �����, ������, ��� � ���.

//������� ������������ �����. � ���� �� ���� jd � ����� t
void mykeplergnsk(double *kep, double jd, double t, double *gnsk);

//������� �����. ���� � ������������
int mygnskkepler(double *gnsk, double *kep);
//���������: 1-���������� ����������, 0-������ �� �������������

//������ ������� ��������� �� (�����,������) �� ���� jd � ����� t
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

struct USLVIDKO { //������� ��������� �� �� ������ t
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
