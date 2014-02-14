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

#define TZ  5 // разница между местным и всемирным временем в часах
              // (номер часового пояса)
#define SHIROTA ((38+16.0/60+45.46/3600)/180*M_PI)
#define DOLGOTA ((4+36.0/60+53.772/3600)/12*M_PI)
#define VYSOTA  2226.45
        // широта комплекса:  38°16'45.46" сев.шир.
        // долгота комплекса: 4h 36m 53.772s = 69°13'26.58" вост.долг.
        // высота комплекса над уровнем моря: 2226.45 м

extern double
  DAVLENIE   ,  //атм.давление в мм рт.ст.
  TEMPERATURA;  //температура воздуха в град.С°

extern char kpstpath[];//путь к КПСТ
extern char zktpath[]; //путь к файлу зв.каталога

extern double JDsea; /* юлианская дата на гринв.полдень,
			предшествующий сеансу (целая) */

  
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
    ncan_vu,   //номер линии CAN для КВУ
    ncan_kdpo, //номер линии CAN для КДПО
    ncan_kep,  //номер линии CAN для КЭП-A, КЭП-L, КЭП-R 
    ncan_ctk;  //номер линии CAN для ЦТК-1 ... ЦТК-4

enum {           // типы камер
  CTK_BIK  = 0,  // БИК
  CTK_SHPK = 1,  // ШПК
  CTK_LIK  = 2,  // ЛИК
  CTK_BPK  = 3   // БПК
};


enum { OS_A=0, OS_L=1, OS_R=2 }; //оси A,L,R

extern double
  mincd[3],    //мин.ЦД (программное ограничение)
  maxcd[3],    //макс.ЦД (программное ограничение)
  maxskor[3],  //макс.скорость по оси, град/с
  maxuskor[3]; //макс.ускорение по оси, град/с^2

#define MINCD(nos)    mincd[nos]
#define MAXCD(nos)    maxcd[nos]
#define MAXSKOR(nos)  maxskor[nos]
#define MAXUSKOR(nos) maxuskor[nos]

extern char APOI_ADDR[20];  // IP-адрес спецвычислителя
extern int APOI_PORT;       // порт спецвычислителя


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

// вычисление длины трехмерного вектора
double dlvect(const VECT3 &v);

// нормировка вектора
void Norm(VECT3 &v);

// умножение вектора на число
VECT3 operator* (const VECT3 &v, double k);

// деление вектора на число
VECT3 operator/ (const VECT3 &v, double k);

// скалярное произвеление двух векторов
double operator* (const VECT3 &v1, const VECT3 &v2);

// сумма двух векторов
VECT3 operator+ (const VECT3 &v1, const VECT3 &v2);
VECT3 operator+= (VECT3 &v1, const VECT3 &v2);

// разность двух векторов
VECT3 operator- (const VECT3 &v1, const VECT3 &v2);
VECT3 operator-= (VECT3 &v1, const VECT3 &v2);

// транспонирование матрицы 3*3
MATR33 operator~ (const MATR33 &m);

// определитель матрицы 3*3
double det33(const MATR33 &m);

// обращение матрицы 3*3
MATR33 ObrMatr(const MATR33 &m);

// умножение матриц 3*3
MATR33 operator* (const MATR33 &m1, const MATR33 &m2);

// умножение матрицы 3*3 на столбец 3*1
VECT3 operator* (const MATR33 &m, const VECT3 &v);

// векторное произведение векторов
VECT3 operator^ (const VECT3 v1, const VECT3 v2);

// вычисление аргумента двумерного вектора (x,y) : рад
double arg(double x, double y );

// преобразование сферических координат в декартовы и обратно.
// u - долгота (рад, 0...2*пи)
// v - широта  (рад, -пи/2...пи/2)
// таблица соответствия:     (u,v)     (x,y,z)
//                           (0,0)     (1,0,0)
//                           (0,пи/2)  (0,1,0)
//                        (пи/2,*)     (0,0,1)
void SferDek ( double u, double v, double *x, double *y, double *z);
void DekSfer(double x, double y, double z, double *u, double *v);

// преобразование сферич.координат в единичный вектор и обратно
VECT3 SferVect(double u, double v);
void VectSfer(VECT3 V, double *u, double *v);

// Переходы между астрономическими системами координат.
// Процедуры с окончанием 'd' работают в декартовых координатах,
// с окончанием 'v' - с векторами, без окончания - в сферических.
// Параметры процедур:
//   A       - азимут (рад), отсчитывается от юга в сторону запада
//   H       - угол места (рад)
//   alfa    - прямое восхождение (рад)
//   delta   - склонение (рад)
//   t       - часовой угол (рад)
//   x,y,z - декартовы координаты
//   shirota - широта места (рад)
//   mzv     - местное звездное время (рад)

// из горизонтальной в первую экваториальную:
void GorEkv1d (double& x, double &y, double &z, double shirota);
void GorEkv1v(VECT3& v, double shirota);
void GorEkv1(double A, double H, double shirota,
             double *t, double *delta);

// из первой экваториальной в горизонтальную:
void Ekv1Gord (double& x, double &y, double &z, double shirota);
void Ekv1Gorv(VECT3& v, double shirota);
void Ekv1Gor(double t, double delta, double shirota,
             double *A, double *H);

// из первой экваториальной во вторую экваториальную и обратно
// (оба преобразования совпадают):
void Ekv1Ekv2d (double& x, double &y, double &z, double mzv);
void Ekv1Ekv2v(VECT3& v, double mzv);
void Ekv1Ekv2(double t, double mzv, double *alfa);
#define Ekv2Ekv1d Ekv1Ekv2d
#define Ekv2Ekv1v Ekv1Ekv2v
#define Ekv2Ekv1  Ekv1Ekv2

// из горизонтальной во вторую экваториальную:
void GorEkv2d (double& x, double &y, double &z,
               double shirota, double mzv);
void GorEkv2v (VECT3& v, double shirota, double mzv);
void GorEkv2(double A, double H, double shirota, double mzv,
             double *alfa, double *delta);

// из второй экваториальной в горизонтальную:
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


/* процедуры абсолютного метода                                       */

VECT3 absmetd(int NTK,
	      double A, double L, double R,
	      double XT, double YT);


void absmet(int NTK,
	      double A, double L, double R,
	      double XT, double YT,
	      double *Az,double *H);
/* переход от системы координат птт в горизонтальную топоцентрическую     */
/* декартовую систему координат                                           */
/* входная информация : A, L, R - датчики осей ОПУ ( град )               */
/*                      XT, YT - координаты изображения звезды ( мм )     */
/* выходная информация : Az, H - координаты звезды в горизонтальной       */
/*                                 топоцентр. системе координат           */
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

// Структура заголовка файла звездного каталога
struct CATHDR {
  short type;         // опознавательный знак файла - должен быть равен 2
  char  name[8];      // имя каталога
  short entrysz;
  float halfa, hdelta;// размер сегментов по альфа, дельта (°)
  short nalfa, ndelta;// число сегментов по альфа, дельта
  float deltalimit;   // ограничение по дельта в южной полусфере
  float maglimit;     // ограничение по звездной величине
  long numzv;         // общее число звезд
  char reserved2[28];
};
  
// Заголовок открытого каталога:
struct CAT {
  FILE* f;    // указатель на открытый файл каталога
  CATHDR hdr;  // копия заголовка файла в оперативной памяти
  long* maddr; // копия адресного массива в оперативной памяти
  char *selected;
  unsigned nseg;
  long addr;
};  
  
// Распакованная информация по звезде
struct CATSTAR {
   long N;            // номер звезды по каталогу
   double alfa,delta; // альфа, дельта J2000.0 (рад)
   float valfa,vdelta;// собственное движение по альфа, дельта (рад/год)
   float mag;         // зв.вел.
   char sp[2];        // спектр.класс
   char flags;        // признаки;
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
