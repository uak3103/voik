#ifndef _CSN_H
#define _CSN_H

//  enum { OS_A=0, OS_L=1, OS_R=2 };

extern int statuscsn;

extern char csnerrmsg[80];

extern double 
  encoder[3], 
  cd[3], 
  timecd[3], 
  skor[3], 
  mincd[3], 
  maxcd[3], 
  maxskor[3], 
  maxuskor[3];

#define _cd(nos)  cd[nos]
#define _cdf(nos) cd[nos]
#define MINCD(nos) mincd[nos]
#define MAXCD(nos) maxcd[nos]

void initcsn(void);
void perebroscsn(double A, double L, double R);
void stopcsn(void);
void skorcsn(double va, double vl, double vr);
void ask_encoder(int os);

void docum_perebroscsn(double A, double L, double R);
void docum_skorcsn(double va, double vl, double vr);

#endif // _CSN_H
