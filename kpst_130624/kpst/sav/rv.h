#ifndef _RV_H
#define _RV_H

#include "sys/sys.h"
#include "fcntl.h"
#include "signal.h"
#include "sys/time.h"
#include "../common/candev.h"
#include "../common/common.h"
#include "globst.h"


extern long nomertakta;

extern int IMIT, IMITVU, IMITOS[3], IMITAPOI, IMITCTK;

extern int rvday, rvmonth, rvyear, rvhour, rvmin;
extern double rvsec;
extern double rvtime;

void get_realtime(int *day, int *month, int *year,
             int *hour, int *min, double *sec);

int initrv(void);
void waittakt(long n);
void delaytakt(int n);

extern int sterror;
extern char sterrmsg1[80];
extern char sterrmsg2[80];
extern char sterrmsg3[80];
extern int rabst, rabvu, rabcsn,
           rabksio, rabfmk, rabapoi, rabasev;


extern FILE *ftextdump;
void create_dirs(char *path);
//int flush_dump(void);
int flush_text_dump(void);
int change_dump(char *new_fdump_name);
//int dump(int Class, int subclass, int numblocks,
//           void *block, unsigned size, ...);

extern int fcan[2];
extern void* can_thread(void *arg);
extern double hms_to_rvtime(int h, int m, double s);
extern int ncan_vu, ncan_kdpo, ncan_kep, ncan_ctk;

#define fcan_vu   fcan[ncan_vu]
#define fcan_kdpo fcan[ncan_kdpo]
#define fcan_kep  fcan[ncan_kep]
#define fcan_ctk  fcan[ncan_ctk]

extern CAN_MSG CAN_last_msg[];
extern int CAN_msg_flag[];
extern int CAN_abn_info[16][8];

typedef void (*cback_t)(int,char*,int);
int IPX_Init(int NST,cback_t proc);
int IPX_Send(int NST,void *mas,int len);

#endif // _RV_H
