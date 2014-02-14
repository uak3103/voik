void print_bz(char *bz);
void ispoln_bz(int NTK, char *bz);


int estglobkep;
double globkep[8];


void current_jd_t(double *jd, double *t)
{
  *jd=JDsea;
  *t=rvtime+(12-TZ)*3600.;
}

void tek_solnce_luna(void)
{
 double jd,t;
 current_jd_t(&jd,&t);
 getsoln(jd,t);
 getluna(jd,t);
}

double readdate(long input)
{
  int d,m,y;
  d=input/10000;
  m=(input%10000)/100;
  y=input%100;
  if (y<80) y+=1900; else y+=2000;
  if (d<1||d>31||m<1||m>12) return -1;
  return DMYtoJD(d,m,y);
}
double readtime(double input)
{
  int h,m; double s;
  h=input/10000;
  m=(input-h*10000.)/100;
  s=(input-h*10000.-m*100.);
  if (h<0||h>23||m<0||m>59||s<0||s>=60) return -1;
  return h*3600.+m*60.+s;
}
/*
long writedate(double jd)
{
  int d,m,y;
  JDtoDMY(jd,&d,&m,&y);
  return d*10000L+m*100+(y%100);
}
*/
double writetime(double t)
{
  int h,m; double s;
  h=t/3600.;
  m=(t-h*3600.)/60.;
  s=t-h*3600.-m*60.;
  return h*10000.+m*100+s;
}


void print_uslvidKO(USLVIDKO &u)
{
  int i;
  if (u.H<0) { printf("Под горизонтом.\n"); return; }
  if (u.H<5*GR) { printf("Низко над горизонтом.: H=%4.2f\n",u.H*RG); return; }
  if (u.ten) { printf("В тени.\n"); return; }
  printf("A=%6.2f H=%6.2f VA=%7.4f VH=%7.4f\n",u.A*RG,u.H*RG,u.VA*RG,u.VH*RG);
  printf("Скор. %7.4f °/c  Высота %5.0f км  Дальн. %5.0f км  Фаз.угол %3.0f°\n",
	  u.skor*RG, u.vys, u.daln, u.faz*RG);
  printf("Расчетный блеск %4.1f\n",u.blesk);
  if (u.rluna*RG<40) printf("Расст.до Луны %3.0f°\n",u.rluna*RG);
}

void printkoname(long Nmezd)
{
  FILE *f=fopen("dan/konames.txt","rt");
  char str[80];
  if (f==NULL) return;
  while(fgets(str,80,f)!=NULL) {
    char *s;
    long num; char *s_nvn, *name, *country;
    s=strchr(str,'\t'); if (s==NULL) goto badstr;
    *s='\0'; s_nvn=&s[1];
    s=strchr(s_nvn,'\t'); if (s==NULL) goto badstr;
    *s='\0'; name=&s[1];
    s=strchr(name,'\t');
    if (s==NULL) {
      country=NULL;
      s=strchr(name,'\n'); if (s!=NULL) *s='\0';
    } else {
      *s='\0'; country=&s[1];
      s=strchr(country,'\t'); if (s!=NULL) *s='\0';
      s=strchr(country,'\n'); if (s!=NULL) *s='\0';
    }
    if (strlen(name)>29) goto badstr;
    if (country!=NULL && strlen(country)>14) goto badstr;
    num=atol(str); if (num==0) goto badstr;
    if (num==Nmezd) {
      printf("%s",name);
      if (country!=NULL) printf(" (%s)",country);
      break;
    }
    badstr: ;
  }
  fclose(f);
}

int startreadTC11503(char *fname);
int readTC11503(TC11503 &ts);
void endreadTC11503(void);
void TC_to_kep(TC11503 &ts, double *kep);



char *ts_fname="tmpdan/tc";

int getspisok(int nspis, long *mas)
{
  char fname[80];
  sprintf(fname,"tmpdan/spisok%d.txt",nspis);
  FILE *f=fopen(fname,"rt");
  char str[80];
  int n=0;
  long NMko;
  if (f==NULL) return 0;
  while(fgets(str,80,f)!=NULL) {
    if (sscanf(str,"%ld",&NMko)==1) {
      mas[n++]=NMko;
    }
  }
  fclose(f);
  return n;
}

int select_ko(int nspis)
{
  TC11503 ts;
  double kep[8], gnsk[8];
  double jd,t;
  USLVIDKO u;
  int n=-1,nn=0,ten,i,j,res;
  long *spisok; int spis_len;
  current_jd_t(&jd,&t);
  startreadTC11503(ts_fname);
  if (nspis>0) {
    spisok = (long*)malloc(100*sizeof(long));
    spis_len = getspisok(nspis,spisok);
    if (spis_len==0) { printf("Нет списка %d\n",nspis); return 0; }
  }
  while (1) {
    res=readTC11503(ts);
    if (res==-1) break;
    if (res==0) continue;
    if (nspis!=0) {
      for(i=0;i<spis_len;i++)
        if (ts.NMko==spisok[i]) break;
      if (i==spis_len) continue;
    }
    TC_to_kep(ts,kep);
//  if (kep[2]<25000||kep[2]>26000||kep[3]>0.05) continue;
    mykeplergnsk(kep,jd,t,gnsk);
    get_uslvidKO(gnsk,NULL,jd,t,u);
    if (u.H<20*GR || u.ten || u.rluna<30*GR ) continue;
    if (nspis==0 && u.skor*RG<0.02 ) continue;
    nn++;
    printf("%05ld  %08ld  h=%5.0fкм  v=%5.3f°/c  H=%4.1f°  ",
           ts.Nko,ts.NMko,u.vys,u.skor*RG,u.H*RG);
    printkoname(ts.NMko);
    printf("\n");
    //printuslvidKO(kep,jd,t);
    //printf("\n");
//  if ((nn%5)==0) {
//    char s[10]; s[0]=0; gets(s);
//    if (s[0]!=0) return -1;
//  }
    n=i;
  }
  endreadTC11503();
  printf("Возможно наблюдение %d КО\n",nn);
  if (nspis!=0) free(spisok);
  return n;
}

void datetime_for_print(double rvtime, long *date, double *time)
{
  double jd=DMYtoJD(rvday,rvmonth,rvyear),
         t =(rvtime>43200? rvtime-43200:rvtime+43200);
  int d,m,y,h; double s;
  t-=TZ*3600.;
  if (t<0) { jd--; t+=86400; }
  JDtoDMY(jd,&d,&m,&y);
  *date=d*10000L+m*100+y%100;
  h=t/3600.; m=(t-h*3600.)/60; s=t-h*3600.-m*60.;
  *time=10000.*h+100.*m+s;
}


int getcatko(long Nko, long *NMko, double *kep)
{
  TC11503 ts;
  double jd,t;
  int res;
  if (!startreadTC11503(ts_fname))
    { printf("Не могу открыть %s\n",ts_fname); return 0; }
  while(1) {
    res=readTC11503(ts);
    if (res==-1) goto no;
    if (res==1&&ts.Nko==Nko) goto yes;
  }
no:  endreadTC11503(); return 0;
yes:
  TC_to_kep(ts,kep);
  current_jd_t(&jd,&t);
#if (IPM==1)
  prognozIPMkep(kep,jd,t,kep);
#endif
  memcpy(globkep,kep,8*sizeof(double));
  endreadTC11503();
  *NMko=ts.NMko;
  return 1;
}

int koproc(long nko, double shift_t, double timefot, int silentmode)
{
  /*KODATA dd;*/ char bz[256];
  long NMko; double kep[8], gnsk[8];
  double jd,t;
  USLVIDKO u;
  int djd,res,i,sss;
  char s[10];
  if (!getcatko(nko,&NMko,kep)) {
    printf("Нет КО %ld\n",nko); return 0;
  }
  printf("Есть КО %ld  Nмежд %08ld  ",nko,NMko);
  printkoname(NMko);
  printf("\n");
  current_jd_t(&jd,&t);
  if ((djd=jd-kep[0])>=3 && !silentmode) {
    printf("Параметры устарели на %d дней. Продолжать?(y/n)",djd);
    gets(s); if (s[0]!='y'&&s[0]!='Y') return 0;
  }
  mykeplergnsk(kep,jd,t+shift_t,gnsk);
  gnsk[1]=t;
  get_uslvidKO(gnsk,NULL,jd,t,u);
  print_uslvidKO(u);

  if (u.H<5*GR||u.ten==1) return 0;

  if (!silentmode) {
    printf("навести?(y/n)"); gets(s);
    if (s[0]!='y'&&s[0]!='Y') return 0;
  }

  sss=identif_flag;
  identif_flag=1;
  TYPEKV(bz)=SBORKI_KV;
  T_KI(bz)=t+TZ*3600.;
  A_KI(bz)=u.A*RG; H_KI(bz)=u.H*RG; VA_KI(bz)=u.VA*RG; VH_KI(bz)=u.VH*RG;
  NI_KI(bz)=NO_NI;
  if (timefot>0) {
   NI_KI(bz)=REALIZ_NI;
   TIMEFOT_NI(bz)=timefot;
  }
  NKO_KI(bz)=nko;
  kep[0]=JD_MJD(kep[0]);
  TVFOT_KI(bz)=1;
  BLESK_KI(bz)=11;
  memcpy(KEP_KI(bz),kep,8*sizeof(double));
  print_bz(bz);
  ispoln_bz(bz);
  identif_flag=sss;
  return 1;
}


void KOINF_to_IZMER(KOINF &ko, IZMER &izm)
{
  double jd,t,A,H,VA,VH,a,d,va,vd;
  jd = JDsea;
  t  = ko.t + (12-TZ)*3600.;
  if (t>86400.) { t-=86400.; jd++; }
  memset(&izm,0,sizeof(izm));
  izm.tipst  = TIPST;
  izm.nporst = NPORST;
  izm.ntk    = ko.ntk;
  izm.met    = ko.metod;
  izm.avoik  = 0;
  izm.res1 = izm.res2 = izm.res3 = 0;
  A=ko.Az; H=ko.H; VA=ko.VAz; VH=ko.VH;
  izm.A=A; 
  izm.H=H;
  izm.VA=VA;
  izm.VH=VH;
  AHad(jd,t,A,H,&a,&d);
  AHad(jd,t+1,A+VA,H+VH,&va,&vd);
  va-=a; vd-=d;
  izm.jd=jd;
  izm.t =t;
  izm.alfa  =a;
  izm.delta =d;
  izm.valfa =va;
  izm.vdelta=vd;
  TekJ2000sea(0,&a,&d);
  izm.alfa2000  =a;
  izm.delta2000 =d;
  izm.blesk     = ko.blesk;
  double v,k;
  va*=cos(d); v=sqrt(va*va+vd*vd);
  switch (izm.tipst) {
    case 58: k = 2; break;
    case 59: k = (izm.ntk==1? 1 : 6); break;
    case 60: k = (izm.ntk==1? 1 : 4); break;
  }
  izm.sko = (izm.met? k*5/3600.*GR : 1/60.*GR);
  izm.vsko = k*5./3600*GR;
  if (izm.vsko < 0.2*v) izm.vsko = 0.2*v;
}

void identif_and_print(int nko)
{
  KOINF &ko = uprst.ko[nko];
  IZMER izm;
  long Nko[10];
  NEVYAZ nev[10];
  int kol, i;
  double t;
  KOINF_to_IZMER(ko,izm);
  kol=identif(izm,10,Nko,nev);
  save_izmer(izm);
  if (kol<0) return;
  printf("Идентификация измер. N%d: ",nko+1);
  if (kol==0)  printf("неизвестный КО!\n");
  else {
    if (kol>1) printf("спорное измерение!");
    printf("\nN ЦККП       ош.вдоль       ош.поп.    ош.скор.\n");
           //"******  **.***° = **.** c   **.***°  **.*"/с = **%
    for(int i=0;i<kol;i++)
      printf("%6ld %6.3f° =%6.2f с %8.3f° %5.1f\"/с =%3.0f%\n",
             Nko[i], nev[i].vdol*RG, nev[i].t_vdol, nev[i].pop*RG,
                     nev[i].dv*RG*3600, nev[i].dv_otn*100);
  }
}
