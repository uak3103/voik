struct KOINF {
 double t,ntk,x,y,e,vx,vy,A,L,R,
        blesk, Azam,Ham, Az,H,VAz,VH,alfa,delta,valfa,vdelta;
 int metod, svf, etk;
};

struct ZONA {
 int NTK;
 double A,
        LN, LK, DL,
        RN, RK, DR,
        R;
 int NL, NR,
     iL, iR,
     ksio;
 double tobn,
        percent;
};

struct UPRST {
  double chv_a, chv_l, chv_r;
  double VR;
  float savedcdm[3*200];
  KOINF ko[10]; int kolko;
  int ksio;
  double Rsoglas;
  ZONA zona, saved_zona;
  int zona_flag, saved_zona_flag;
};

#define savedcd(upr,takt,os) upr.savedcdm[3*(takt%200)+os]
//#define BL051
extern UPRST uprst;
extern int RABTK[];
extern int NUMRABTK;

extern int chv_bezA; //ЧВ без оси А

extern char bz[];       // текущая БЗ
extern int CVS;         /* признак работы с ЦВС */
extern int CVS_BZ;      /* адр.машины, от которой получена БЗ */

extern int breakcode;

void ispoln_bz(char *bz);
void print_bz(char *bz);

int catzvOMR(int NTK);
int MetodOMR(int NTK);

void DrawPicture(int NTK);

int RESVU(void);
int RESCSN(void);
int VU_OTKR(int *maskaTK);
int VU_ZAKR(int *maskaTK);
int IZMERAPOI(int *maskaTK);
int NavedShort(int NTK, double Az, double H, double XT, double YT,
       	                double *A1, double *L1, double *R1);
void skorchvcsn (double A, double L, double R,
		   double* VA, double* VL, double* VR, int bezA=0);
int PUSKCHV(double A, double L, double R);
void START_KONTROL_CHV(void);
void OTKAZ_CHV(void);
void IZMERKO(int *maskaTK);
int OBRKO(int *maskaTK, int omr);

void clearsterror(void);
void printsterror(void);


int koproc(long nko, double shift_t, double timefot, int silentmode);
int select_ko(int nspis);
void ident_nko_proc(long Nko);

void CTK_info(int ntk);
void CTK_checktime(int ntk);
