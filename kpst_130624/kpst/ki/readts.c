//////////////////////////////////////////////////////////////////////
//                      Чтение и анализ ТС11503                     //
//////////////////////////////////////////////////////////////////////

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

char tsline[4][128];
#define ERRORVALUE -999999
int tserrflag=0;
int tsprinterr=1;

long tserror(char *p)
{
  if (tsprinterr) {
    if (!tserrflag)
      printf("Ошибка в %s",tsline[0]);
    int nlin,npos;
    for(nlin=0;nlin<4;nlin++) {
      npos=p-tsline[nlin];
      if (npos>=0 && npos<128) break;
    }
    printf("%s",tsline[nlin]);
    for(int i=0;i<npos;i++) printf(" ");
    printf("!\n");
  }
  tserrflag=1;
  return ERRORVALUE;
}

void checkspace(char *s)
{ if (s[0]!=' ') tserror(s); }

int asterisks(char *s, int npos)
{
  if (s[0]!='*') return 0;
  for(int i=1;i<npos;i++) if (s[i]!='*') return 0;
  return 1;
}

long read_int(char *s, int npos)
{
  long val=0;
  int i;
  if (asterisks(s,npos)) return 0;
  for(i=0;i<npos;i++) {
    unsigned char c=s[i];
    if (c<'0'||c>'9') return tserror(&s[i]);
    val=10*val+(c-'0');
  }
  return val;
}

long read_signedint(char *s, int npos)
{
  if (asterisks(s,npos)) return 0;
  int sign;
  if (s[0]=='+') sign=1;
  else if (s[0]=='-') sign=-1;
  else return tserror(s);
  return sign*read_int(&s[1],npos-1);
}

double read_fixp(char *s, int n1, int n2)
{
  double val=0, dr;
  int i;
  if (asterisks(s,n1)) return 0;
  if ((val=read_int(s,n1-n2-1))==ERRORVALUE) return val;
  if (s[n1-n2-1]!='.') return tserror(&s[n1-n2-1]);
  if ((dr=read_int(&s[n1-n2],n2))==ERRORVALUE) return dr;
  for(i=0;i<n2;i++) dr/=10;
  return val+dr;
}

double read_signedfixp(char *s, int n1, int n2)
{
  if (asterisks(s,n1)) return 0;
  int sign;
  if (s[0]=='+') sign=1;
  else if (s[0]=='-') sign=-1;
  else return tserror(s);
  return sign*read_fixp(&s[1],n1-1,n2);
}

double read_data(char *s, int npos)
{
  int ny=npos-4; long day,month, year;
  double jd;
  if (asterisks(s,npos)) return 0;
  if ((day=read_int(s,2))==ERRORVALUE) goto err;
  if (day<1||day>31) { tserror(s); goto err; }
  s+=2;
  if ((month=read_int(s,2))==ERRORVALUE) goto err;
  if (month<1||month>12) { tserror(s); goto err; }
  s+=2;
  if ((year=read_int(s,ny))==ERRORVALUE) goto err;
  if (ny==2)
    { if (year>50) year+=1900; else year+=2000; }
  //if (year<1999||year>2010) { tserror(s); goto err; }
  jd=DMYtoJD(day,month,year);
  return jd;
err:
  return ERRORVALUE;
}

double read_vr(char *s, int n1, int n2)
{
  long h,m; double sec;
  if (asterisks(s,n1)) return 0;
  if ((h=read_int(s,2))==ERRORVALUE) goto err;
  if (h>24) { tserror(s); goto err; }
  s+=2;
  if ((m=read_int(s,2))==ERRORVALUE) goto err;
  if (m>59) { tserror(s); goto err; }
  s+=2;
  if ((sec=read_fixp(s,n1-4,n2))==ERRORVALUE) goto err;
  if (sec>60) { tserror(s); goto err; }
  return h*3600.+m*60.+sec;
err:
  return ERRORVALUE;
}

int readTC11503(FILE *f, TC11503 &t)
{
  char *s;
  int i;
  tserrflag=0;
repeat:
  if (fgets(tsline[0],128,f)==NULL) return -1;
  // if (memcmp(tsline[0],"ТС1150",6)!=0) goto repeat;
  if (memcmp(&tsline[0][2],"1150",4)!=0) goto repeat;
  for(i=0;i<3;i++)
    if (fgets(tsline[i+1],128,f)==NULL) return -1;
  s=tsline[0];
  t.Nts=read_int(&s[8],3);
  s=tsline[1];
  t.Nsr    =read_int(s,3); s+=3; checkspace(s); s++;
  t.Nko    =read_int(s,5); s+=5; checkspace(s); s++;
  if (memcmp(s,"HEИЗBECT",8)==0) t.NMko=0;
    else t.NMko   =read_int(s,8);
                           s+=8; checkspace(s); s++;
  t.Pstacko=read_int(s,1); s+=1; checkspace(s); s++;
  t.Ppr    =read_int(s,1); s+=1; checkspace(s); s++;
  t.Pnki   =read_int(s,1); s+=1; checkspace(s); s++;
  t.Ptipnki=read_int(s,1); s+=1; checkspace(s); s++;
  t.Ptipki =read_int(s,1); s+=1; checkspace(s); s++;
  t.Nprior =read_signedint(s,5); s+=5; checkspace(s); s++;
  t.epoha  =read_int(s,4); s+=4; checkspace(s); s++;
  t.Da  =read_fixp(s,4,1); s+=4; checkspace(s); s++;
     //if (memcmp(s,"0 0.",4)==0||memcmp(s,"0 1.",4)==0)
     //   memcpy(s,"00.0",4);
  t.Dv  =read_fixp(s,4,1); s+=4; checkspace(s); s++;
  t.smz =read_signedfixp(s,5,1); s+=5; checkspace(s); s++;
  t.dmz =read_fixp(s,3,1); s+=3; checkspace(s); s++;
  t.Pugz   =read_int(s,1);
  s=tsline[2];
  t.datanach=read_data(s,6);   s+=6; checkspace(s); s++;
  t.vrnach  =read_vr  (s,10,3);s+=10;checkspace(s); s++;
  t.datakon =read_data(s,6);   s+=6; checkspace(s); s++;
  t.vrkon   =read_vr  (s,10,3);s+=10;checkspace(s); s++;
    //if (s[0]==' ') s[0]='0';
  t.data    =read_data(s,6);   s+=6; checkspace(s); s++;
    //for(i=0;i<5;i++) if (s[i]==' ') s[i]='0'; else break;
  t.vr      =read_vr  (s,10,3);s+=10;
  if (t.Pstacko==1) { checkspace(s); s++; t.dpt=read_fixp(s,7,3); }
  s=tsline[3];
     //if (memcmp(s,"0 ",2)==0) memcpy(s,"00",2);
  t.Tom =read_fixp(s,10,4);s+=10;checkspace(s); s++;
     //if (memcmp(s,"0 ",2)==0) memcpy(s,"00",2);
  t.a   =read_fixp(s,10,3);s+=10;checkspace(s); s++;
  t.e   =read_fixp(s,9,7); s+=9; checkspace(s); s++;
  t.i   =read_fixp(s,8,4); s+=8; checkspace(s); s++;
  t.Om  =read_fixp(s,8,4); s+=8; checkspace(s); s++;
  t.om  =read_fixp(s,8,4); s+=8; checkspace(s); s++;
  t.U   =read_fixp(s,8,4); s+=8; checkspace(s); s++;
  t.DTom=read_signedfixp(s,6,2); s+=6; checkspace(s); s++;
  t.M   =read_fixp(s,7,3);
  if (tserrflag) return 0; else return 1;
}

void TC_to_kep(TC11503 &ts, double *kep)
{
  kep[0]=ts.data;
  kep[1]=ts.vr;
  kep[2]=ts.a;
  kep[3]=ts.e;
  kep[4]=ts.i*(M_PI/180);
  kep[5]=ts.Om*(M_PI/180);
  kep[6]=ts.om*(M_PI/180);
  kep[7]=ts.U*(M_PI/180);
}

void cantopen(char *fname)
{
  printf("Ошибка при открытии файла %s\n",fname);
}


#include <dirent.h>


char tsdirname[256];
DIR *tsdir = NULL;
FILE *tsfile = NULL;

int startreadTC11503(char *file_or_dir)
{
  struct stat buf;
  int res;

  res = stat(file_or_dir, &buf);
  if (res!=0) return 0;

  if (S_ISREG(buf.st_mode)) {

    tsdir = NULL;
    tsfile =  fopen(file_or_dir,"rt");
    if (tsfile==NULL) return 0;

  } else if (S_ISDIR(buf.st_mode)) {

    tsfile = NULL;
    strcpy(tsdirname, file_or_dir);
    tsdir = opendir (tsdirname);
    if (tsdir==NULL) return 0;
    
  } else return 0;

  return 1;
}


int readTC11503(TC11503 &ts)
{
start:
  if (tsdir!=NULL && tsfile==NULL) {

    while(1) {
      struct dirent *de;
      de = readdir(tsdir);
      if (de==NULL) {
        closedir(tsdir);
        return -1;
      }
      if (de->d_type == DT_REG) {
        char fname[256];
        strcpy(fname, tsdirname);
        strcat(fname,"/");
        strcat(fname, de->d_name);
        tsfile = fopen(fname,"rt");
        if (tsfile!=NULL) break;
      }
    }

  }

  int res;
  res=readTC11503(tsfile,ts);
  if (res>=0) return res;
  if (res==-1) {
       fclose(tsfile);
       tsfile = NULL;
       if (tsdir==NULL) 
         return -1;
       else goto start;
  }
}


void endreadTC11503(void)
{
}
