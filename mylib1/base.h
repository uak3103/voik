#ifndef BASE_H
#define BASE_H

#include <sys/types.h>
#include <fcntl.h>     /* open */
#include <stdlib.h>    /* atexit */
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <list>

using namespace std;

#define Q_TMP 512*2		//4096!!!
#define KKK   256
#define OBJ_MAX  10000
#define OBRAB_MAX 10
#define min(a,b) ((a<b)?(a):(b))
#define max(a,b) ((a>b)?(a):(b))

/*
struct TEX_INFO
{
        u_int32_t 	nID;
        u_char 		btCamNum;
        u_char		btCamType;
        u_char		pbtMAC[6];
        u_int32_t	nVideoOffset;
        u_int32_t 	nCadrNum;
        u_int16_t	nReserv;
        u_char		btExpYear;
        u_char		btExpMonth;
        u_char 		btExpDay;
        u_char		btExpHour;
        u_char		btExpMinutes;
        u_char		btExpSeconds;
        u_int32_t	nExpMicroseconds;
        u_int32_t	nExposition;
        u_int16_t	wOffsetX;
        u_int16_t	wOffsetY;
        u_int16_t	wSizeX;
        u_int16_t	wSizeY;
        u_char		btBinX;
        u_char		btBinY;
        u_int16_t	wConvolutionsCount;
        u_int16_t	wMinBright;
        u_int16_t	wMaxBright;

        union
        {
                struct
                {
                        u_char	btReserv1	:5;
                        u_char	btStabBlackLevel	:1;
                        u_char	btExcludeDarkCurrent	:1;
                        u_char	btCorrSens	:1;
                }ProcessType;
                u_char btProcessType;
        };

        union
        {
                struct
                {
                        u_char	btAutoLevel	:1;
                        u_char	btReserv2	:6;
                        u_char	btFindConvolution	:1;
                }ThreshType;
                u_char	btThreshType;
        };

        u_int16_t	wThresh;
        int16_t		nTempCurrent;
        int16_t		nTempSet;
        u_int16_t	wAnalogAmpl;
        u_int16_t	wEmAmpl;
        u_int16_t	wReadFreq;
        u_int16_t	wBlackLevel;
        u_char		btMatrixOutputNum;
        u_char 		btSpeedTransfer;
        u_char		btStepIntegration;
        u_char		pbtReserv1[57];

        u_int32_t       iDatchA;
        u_int32_t       iDatchL;
        u_int32_t       iDatchR;
        u_char		pbtReserv2[100];
};

struct CONVOLUTION
{
        u_int32_t	nID;           // "CONV"
        u_int16_t	wNum;
        u_int16_t	wReserv;
        u_int32_t 	nX;
        u_int32_t	nY;
        u_int32_t	nV;
        u_int32_t	nS;
        u_int32_t	nS_XX;
        u_int32_t	nS_XY;
        u_int32_t	nS_YY;
        u_char		pbtReserv[28];
};
*/
typedef struct {
  double T;
  float X;
  float Y;
  float DX;
  float DY;
  float S;
  float V;
  long  KO;
} nObj;

typedef struct {
    double SA;
    double SxA;
    double SyA;
    double SxxA; // xx moment
    double SxyA; // xy moment
    double SyyA; // yy moment
    int C;    // count
    int A;    // amplituda
    int Xleft;
    int Xright;
    } TmpType;

typedef struct {
  float X;  // x point
  float Y;  // y point
  long  V;  // volume
  long  S;  // square
  float XX; // xx moment
  float XY; // xy moment
  float YY; // yy moment
} tObj;

typedef struct {
  float X;   // x  point
  float Y;   // y  point
  float VX;  // vx point
  float VY;  // vy point
  float DX;  // dx point
  float DY;  // dy point
  float V;   // volume
  float S;   // square
  float SKO; // SKO
  long  ID;  // unique
  double T0; // first time
  double T;  // time
  short W;   // weight
  short N;   // hit count
  short KO;  // ko
  short reserv;
} sObj;

typedef struct {
    int Xmin;
    int Ymin;
    int Xmax;
    int Ymax;
    int S;
    int V;
    float X;
    float Y;
} bObj;

typedef struct {
    int Nkadr;
    float VXzv;
    float VYzv;
    float VXob;
    float VYob;
    float Xob;
    float Yob;
    int userv[6];
} puskprm;

class Base
{
private:
    list<nObj> lst;
    FILE *ftr;
    char nftr[40];
    int    s_ndx[256][KKK];
    int    s_kol[256];
    int    q_ob;
    int    q_sw;
    int    SETKA_X;
    int    SETKA_Y;
    int    obrab;
    int    ind_bigobj;
    bObj   m_bigobj[128];
    double m_tob[OBRAB_MAX];
    puskprm OBRAB;
    tObj   *m_swrt;
    sObj   *m_objs;
    tObj   m_objsn[OBJ_MAX][OBRAB_MAX];
    float  VX_aver;
    float  VY_aver;

    float  sko_V;
    int    is_OBRAB;
    int    LVL_OBR[4];
    int    n_kadr;

    int PLOSH;
    int DISTANT1;
    int DISTANT2;
    int FIND1;
    int correct_by_speed;
    float MAXNEVYAZ;
    float MAXNEVYAZBIK; // 1.0;
    int DISTPHM;
    int NEARPHM;
    int KOLZV_SHPK;
    int KOLZV_BIK;
    int MAXKO;

    TmpType Tmp1[Q_TMP];
    TmpType Tmp2[Q_TMP];
    int    u_id;
    int    LVL_MAX;
    int W_KADR;
    int H_KADR;
    TEX_INFO *tex_inf;
    char *m_kadr;

    int process_convolutions(int n);
    void fill_conv(int obj);
    double fun(int num,int obj,sObj *mas);
    int is_ko(int num,int obj,sObj *mas);
    int is_zv(int num,int obj,sObj *mas);
    void send_objs(int fff,int num);
    int NumKadrs();
    bool ReadKadr(int n);
    void trace(const char *fmt,...);
    int process_fon (int num,int X0, int DX,int Y0, int DY,u_short *inp,int &aver);
    void init_lines(int num);
    void sort_swrt(int num,tObj *obj);
    int process_swrt2(int num);
    void pre_obrab(int num);
    void obrab_bigobj(int num, int x0, int y0);
    int end_obrab(int num,int nnn);
    int find_match2(int num,double _x,double _y,double _v,tObj *obj,double maxdist,double *dd);
    int process_lines(int num,int X0 ,int DX,int Y0 ,int DY,u_short *inp,int por[4],int aver[4],tObj *out);

public:
    Base();
    ~Base();


    void fun_obrb(int n);
};

#endif // BASE_H
