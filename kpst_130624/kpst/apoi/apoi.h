#ifndef _APOI_H
#define _APOI_H

enum { MAXKKO=10, MAXKZV=3000, MAXKOLOBJ=3000 };

typedef struct {
          int nvyd;
	  float x,y,e,vx,vy;
	  long takt;
	  float dt;
	}
	  KOtype;

typedef struct {
	  float x,y,e;
	}
	  ZVtype;


extern char APOI_ADDR[];
extern int APOI_PORT;
extern int APOI_connected;
extern int APOI_nbytes_read;
extern int APOI_nbytes_write;


extern int RABTKAPOI[5];

extern int NkadrAPOI[5];

extern int KolobjAPOI[5];
extern nObj *(ObjAPOI[5]);

extern int       KKOAPOI[5];
extern KOtype   *(KOAPOI[5]);

extern int       KZVAPOI[5];
extern long   TAKTZVAPOI[5];
extern ZVtype   *(ZVAPOI[5]);

extern int    STATUSAPOI[5];

int  INITAPOI(int NTK);

int APOI_pusk(int Nctk, int Nkadr, double, double);
void* APOI_thread(void*);
int APOI_connect(void);

#endif // _APOI_H
