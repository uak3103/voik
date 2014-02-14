#ifndef _GLOBST_H
#define _GLOBST_H

//#define KOI8     1
#define UTF8     2
//#define CHARSET  KOI8
#define CHARSET  UTF8



extern int tipst, nporst;
extern int MAXTK;


#define TIPST  tipst
#define NPORST nporst

#define TZ  5 // ������� ����� ������� � ��������� �������� � �����
              // (����� �������� �����)
#define SHIROTA ((38+16.0/60+45.46/3600)/180*M_PI)
#define DOLGOTA ((4+36.0/60+53.772/3600)/12*M_PI)
#define VYSOTA  2226.45
        // ������ ���������:  38�16'45.46" ���.���.
        // ������� ���������: 4h 36m 53.772s = 69�13'26.58" ����.����.
        // ������ ��������� ��� ������� ����: 2226.45 �

extern double
  DAVLENIE   ,  //���.�������� � �� ��.��.
  TEMPERATURA;  //����������� ������� � ����.�

extern char kpstpath[];//���� � ����
extern char zktpath[]; //���� � ����� ��.��������

extern double JDsea; /* ��������� ���� �� �����.�������,
			�������������� ������ (�����) */

  
extern double pasport[5][25];
              
#define pasF(NTK)       pasport[NTK][0]
#define pasZDA(NTK)     pasport[NTK][1]
#define pasZDL(NTK)     pasport[NTK][2]
#define pasZDR(NTK)     pasport[NTK][3]
#define pasohZDA(NTK)   pasport[NTK][4]
#define pasohZDL(NTK)   pasport[NTK][5]
#define pasohZDR(NTK)   pasport[NTK][6]
#define paskolvos(NTK)  pasport[NTK][7]
#define pasKRL(NTK)     pasport[NTK][8]
#define pasKLA(NTK)     pasport[NTK][9]
#define pasAotC(NTK)    pasport[NTK][10]
#define pasAotZ(NTK)    pasport[NTK][11]
#define pasdefX(NTK)    pasport[NTK][12]
#define pasdefY(NTK)    pasport[NTK][13]
#define pasMASH(NTK)    pasport[NTK][14]
#define pasrazv(NTK)    pasport[NTK][15]
#define pasflipX(NTK)   pasport[NTK][16]
#define pasflipY(NTK)   pasport[NTK][17]
#define pastipctk(NTK)  pasport[NTK][18]
#define pasSX(NTK)      pasport[NTK][19]
#define pasSY(NTK)      pasport[NTK][20]
#define pasNX(NTK)      pasport[NTK][21]
#define pasNY(NTK)      pasport[NTK][22]
#define pasbin(NTK)     pasport[NTK][23]

int init_pasp (int tipst, int nporst);

#define TKsizeXmm(NTK) (pasSX(NTK)*pasNX(NTK))
#define TKsizeYmm(NTK) (pasSY(NTK)*pasNY(NTK))

#define PIX2MM(ntk) (pasSX(ntk)*pasbin(ntk))
#define MM2PIX(ntk) (1/pasSX(ntk)/pasbin(ntk))

void coord_pix2mm(int ntk, double Xpix, double Ypix, double *Xmm, double *Ymm);
void coord_mm2pix(int ntk, double Xmm, double Ymm, double *Xpix, double *Ypix);


extern int 
    ncan_vu,   //����� ����� CAN ��� ���
    ncan_kdpo, //����� ����� CAN ��� ����
    ncan_kep,  //����� ����� CAN ��� ���-A, ���-L, ���-R 
    ncan_ctk;  //����� ����� CAN ��� ���-1 ... ���-4

enum {           // ���� �����
  CTK_BIK  = 0,  // ���
  CTK_SHPK = 1,  // ���
  CTK_LIK  = 2,  // ���
  CTK_BPK  = 3   // ���
};


enum { OS_A=0, OS_L=1, OS_R=2 }; //��� A,L,R

extern double
  mincd[3],    //���.�� (����������� �����������)
  maxcd[3],    //����.�� (����������� �����������)
  maxskor[3],  //����.�������� �� ���, ����/�
  maxuskor[3]; //����.��������� �� ���, ����/�^2

#define MINCD(nos)    mincd[nos]
#define MAXCD(nos)    maxcd[nos]
#define MAXSKOR(nos)  maxskor[nos]
#define MAXUSKOR(nos) maxuskor[nos]

extern char APOI_ADDR[20];  // IP-����� ���������������
extern int APOI_PORT;       // ���� ���������������


#define GR (M_PI/180)
#define RG (180/M_PI)
#define HR (M_PI/12)
#define RH (12/M_PI)

#define time_kpst2apoi(t)  ((t)>43200? t-43200 : t+43200)
#define time_apoi2kpst(t)  ((t)>43200? t-43200 : t+43200)
#define time_kpst2kbp(t)   ((t)>43200? t-43200 : t+43200)
#define time_kbp2kpst(t)   ((t)>43200? t-43200 : t+43200)

double max(double x, double y);
double min(double x, double y);

#define _VECT3
typedef struct
      {
       double x;
       double y;
       double z;
       } VECT3;

typedef struct
      {
       double m11, m12, m13,
	      m21, m22, m23,
	      m31, m32, m33;
      } MATR33;

// ���������� ����� ����������� �������
double dlvect(const VECT3 &v);

// ���������� �������
void Norm(VECT3 &v);

// ��������� ������� �� �����
VECT3 operator* (const VECT3 &v, double k);

// ������� ������� �� �����
VECT3 operator/ (const VECT3 &v, double k);

// ��������� ������������ ���� ��������
double operator* (const VECT3 &v1, const VECT3 &v2);

// ����� ���� ��������
VECT3 operator+ (const VECT3 &v1, const VECT3 &v2);
VECT3 operator+= (VECT3 &v1, const VECT3 &v2);

// �������� ���� ��������
VECT3 operator- (const VECT3 &v1, const VECT3 &v2);
VECT3 operator-= (VECT3 &v1, const VECT3 &v2);

// ���������������� ������� 3*3
MATR33 operator~ (const MATR33 &m);

// ������������ ������� 3*3
double det33(const MATR33 &m);

// ��������� ������� 3*3
MATR33 ObrMatr(const MATR33 &m);

// ��������� ������ 3*3
MATR33 operator* (const MATR33 &m1, const MATR33 &m2);

// ��������� ������� 3*3 �� ������� 3*1
VECT3 operator* (const MATR33 &m, const VECT3 &v);

// ��������� ������������ ��������
VECT3 operator^ (const VECT3 v1, const VECT3 v2);

// ���������� ��������� ���������� ������� (x,y) : ���
double arg(double x, double y );

// �������������� ����������� ��������� � ��������� � �������.
// u - ������� (���, 0...2*��)
// v - ������  (���, -��/2...��/2)
// ������� ������������:     (u,v)     (x,y,z)
//                           (0,0)     (1,0,0)
//                           (0,��/2)  (0,1,0)
//                        (��/2,*)     (0,0,1)
void SferDek ( double u, double v, double *x, double *y, double *z);
void DekSfer(double x, double y, double z, double *u, double *v);

// �������������� ������.��������� � ��������� ������ � �������
VECT3 SferVect(double u, double v);
void VectSfer(VECT3 V, double *u, double *v);

// �������� ����� ���������������� ��������� ���������.
// ��������� � ���������� 'd' �������� � ���������� �����������,
// � ���������� 'v' - � ���������, ��� ��������� - � �����������.
// ��������� ��������:
//   A       - ������ (���), ������������� �� ��� � ������� ������
//   H       - ���� ����� (���)
//   alfa    - ������ ����������� (���)
//   delta   - ��������� (���)
//   t       - ������� ���� (���)
//   x,y,z - ��������� ����������
//   shirota - ������ ����� (���)
//   mzv     - ������� �������� ����� (���)

// �� �������������� � ������ ��������������:
void GorEkv1d (double& x, double &y, double &z, double shirota);
void GorEkv1v(VECT3& v, double shirota);
void GorEkv1(double A, double H, double shirota,
             double *t, double *delta);

// �� ������ �������������� � ��������������:
void Ekv1Gord (double& x, double &y, double &z, double shirota);
void Ekv1Gorv(VECT3& v, double shirota);
void Ekv1Gor(double t, double delta, double shirota,
             double *A, double *H);

// �� ������ �������������� �� ������ �������������� � �������
// (��� �������������� ���������):
void Ekv1Ekv2d (double& x, double &y, double &z, double mzv);
void Ekv1Ekv2v(VECT3& v, double mzv);
void Ekv1Ekv2(double t, double mzv, double *alfa);
#define Ekv2Ekv1d Ekv1Ekv2d
#define Ekv2Ekv1v Ekv1Ekv2v
#define Ekv2Ekv1  Ekv1Ekv2

// �� �������������� �� ������ ��������������:
void GorEkv2d (double& x, double &y, double &z,
               double shirota, double mzv);
void GorEkv2v (VECT3& v, double shirota, double mzv);
void GorEkv2(double A, double H, double shirota, double mzv,
             double *alfa, double *delta);

// �� ������ �������������� � ��������������:
void Ekv2Gord (double& x, double &y, double &z,
	       double shirota, double mzv);
void Ekv2Gorv (VECT3& v, double shirota, double mzv);
void Ekv2Gor(double alfa, double delta, double shirota, double mzv,
	     double *A, double *H);

double JulianDate(double D, int M, int Y);
double GZV(double JD, double time);


double MZVsea(double time);
void AHtd(double A, double H, double *t, double *delta);
void tdAH(double t, double delta, double *A, double *H);
void DPTdelta_tdelta(double dpt, double deltageo,
		       double *t, double *deltatopo);
double t_DPT(double t);
double atsea(double alfa, double time);
#define tasea atsea
void AHadsea(double A, double H, double time,
             double *alfa, double *delta);
void adAHsea(double alfa, double delta, double time,
             double *A, double *H);
void AHad(double jd, double utc, double A, double H,
	     double *alfa, double *delta);
void adAH(double jd, double utc, double alfa, double delta,
             double *A, double *H);
void KatVidsea(double time, double *alfa, double *delta);
void VidKatsea(double time, double *alfa, double *delta);
void TekJ2000sea (double time, double *alfa, double *delta);
void J2000Teksea(double time, double *alfa, double *delta);
void J1950J2000(double *alfa, double *delta);
void PlusRefraction(double *H);
void MinusRefraction(double *H);
double DMYtoJD(double D, int M, int Y);
int JDtoDMY(double JD, int *D, int *M, int *Y);
#define MJD_JD(mjd) (mjd+2436203.5)
#define JD_MJD(mjd) (mjd-2436203.5)
double MZV(double JD, double time);
void gnskkoord(double fi, double h, double mzv,
	  double *x, double *y, double *z);
void cksn(double JD, double t, double *x, double *y, double *z);

void skorchvcsn_A (double A, double L, double R,
		   double* VA, double* VL, double* VR);
void skorchvcsn_bezA (double A, double L, double R,
		   double* VA, double* VL, double* VR);


/* ��������� ����������� ������                                       */

VECT3 absmetd(int NTK,
	      double A, double L, double R,
	      double XT, double YT);


void absmet(int NTK,
	      double A, double L, double R,
	      double XT, double YT,
	      double *Az,double *H);
/* ������� �� ������� ��������� ��� � �������������� ����������������     */
/* ���������� ������� ���������                                           */
/* ������� ���������� : A, L, R - ������� ���� ��� ( ���� )               */
/*                      XT, YT - ���������� ����������� ������ ( �� )     */
/* �������� ���������� : Az, H - ���������� ������ � ��������������       */
/*                                 ���������. ������� ���������           */
/**************************************************************************/

struct OTKPOS { VECT3 x,y,z; };

void getOTKpos(int NTK, double A, double L, double R,
	       OTKPOS &p);
void GorTV(OTKPOS &p, double Az, double H, double *X, double *Y);
void AHXY(int NTK, double A, double L, double R,
                   double Az, double H, double *X, double *Y);



enum { ZENIT, POLUS, RAVND };
void NAVED (int NTK,
	    VECT3 VO, VECT3 VX,
	    double XT, double YT,
	    double *A, double *L, double *R);
void Naved (int NTK, double Az, double H,
	    int tip, double razvorot, double XT, double YT,
	    double *A, double *L, double *R);
void naved_polus(int NTK, double jd, double t,
       double *A, double *L, double *R);

// ��������� ��������� ����� ��������� ��������
struct CATHDR {
  short type;         // ��������������� ���� ����� - ������ ���� ����� 2
  char  name[8];      // ��� ��������
  short entrysz;
  float halfa, hdelta;// ������ ��������� �� �����, ������ (�)
  short nalfa, ndelta;// ����� ��������� �� �����, ������
  float deltalimit;   // ����������� �� ������ � ����� ���������
  float maglimit;     // ����������� �� �������� ��������
  long numzv;         // ����� ����� �����
  char reserved2[28];
};
  
// ��������� ��������� ��������:
struct CAT {
  FILE* f;    // ��������� �� �������� ���� ��������
  CATHDR hdr;  // ����� ��������� ����� � ����������� ������
  long* maddr; // ����� ��������� ������� � ����������� ������
  char *selected;
  unsigned nseg;
  long addr;
};  
  
// ������������� ���������� �� ������
struct CATSTAR {
   long N;            // ����� ������ �� ��������
   double alfa,delta; // �����, ������ J2000.0 (���)
   float valfa,vdelta;// ����������� �������� �� �����, ������ (���/���)
   float mag;         // ��.���.
   char sp[2];        // ������.�����
   char flags;        // ��������;
   float BV;          // B-V .
};

extern CAT zkt;
extern int zktflag;

int opencat(CAT& c, char* fname);
void closecat(CAT& c);
void startreadseg(CAT& c,
		  double alfamin,  double alfamax,
		  double deltamin, double deltamax);
void startreadcircle(CAT& c, double alfa, double delta, double radius);
int readstar(CAT& c, CATSTAR& s);
int findstar(CAT& c, long Nkat, CATSTAR &s);

struct REDOMR {
  double ax,ay,axx,axy,ayx,ayy;
};

struct REDZVVEL {
  double a0, a1;
  int Nzv, Nbrak;
  double sko;
};

struct CATZV { long N; double x,y,v; };
struct IZMZV { 
   double x,y,e;
   int brak_koord, brak_zvvel;
};

struct OMRDATA {
 double A,L,R;
 double time;
 OTKPOS otkpos;
 int maxcatkzv;
 int catkzv;
 CATZV *catzv;
 int maxizmkzv;
 int izmkzv;
 IZMZV *izmzv;
 int omrkzv;
 int *catss;
 int *izmss;
 double VXzv,VYzv;
 REDOMR red;
 REDZVVEL rzv;
};

    

void clear_redomr(REDOMR &r);
void redomr(REDOMR &red, double x0, double y0, double *x, double *y);
void inverse_redomr(REDOMR &red, double x0, double y0, double *x, double *y);
extern OMRDATA omrdata[3];
double dist(REDOMR &red, IZMZV &izmzv, CATZV &catzv);
int Kletka(OMRDATA &o, double *X, double *Y);
int StarsInKletka (OMRDATA &o, double X, double Y,
                   double *Xcentr, double *Ycentr);
int StarsInCircle(OMRDATA &o);
void get_reduction_OMR(OMRDATA &o, double xko, double yko);
void get_reduction_zvvel(OMRDATA &o);
int Otogdestv(OMRDATA &o);
void get_catzv(int ntk, double xmin, double xmax, double ymin, double ymax, double magmax);
int printOMR(int NTK);
int print2OMR(int NTK);
void print3OMR(int NTK); 
int korrxyOMR(int NTK, double *x, double *y);
int inverse_korrxyOMR(int NTK, double *x, double *y);
double get_zvvel(REDZVVEL &red, double e);
void initOMR(int NTK);
     
             
char otk_letter(int tipst, int ntk);
double gmsgrad(double g, double m, double s);
#define hmshour gmsgrad
void gradgms(double x, int* g, int* m, double *s);
#define hourhms gradgms
void printgms(double x, int posg, int poss);
void printhms(double x, int poss);
double duga(double A1, double H1, double A2, double H2);
  
int gauss (int n,  double* a, double* b, double* x );  
void approx2(int n, double *T, double *X, 
            double *a0, double *a1, double *a2);  
                        
int initglobst(int tipst, int nporst,
               int day,int month,int year,int hour);
#define BL051
#endif // _GLOBST_H   
