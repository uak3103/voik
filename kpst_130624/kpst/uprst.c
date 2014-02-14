int identif_flag=1;
int test_ko_flag=0;
extern struct timespec delay;//krikunov
double tperebrosa=0; // для таймаута при перебросе
extern double Tperebros(double A0, double L0, double R0,
                 double A1, double L1, double R1);
void clearsterror(void)
{
  breakcode=0; sterror=0;
  strcpy(sterrmsg1,"");
  strcpy(sterrmsg2,"");
  strcpy(sterrmsg3,"");
}

void printsterror(void)
{
  if (sterror) {
    if (sterrmsg1[0]!='\0') printf("%s\n",sterrmsg1);
    if (sterrmsg2[0]!='\0') printf("%s\n",sterrmsg2);
    if (sterrmsg3[0]!='\0') printf("%s\n",sterrmsg3);
  } else if (breakcode)
    printf("%s\n",breakcodestr(breakcode));
}

int RESVU(void)
{
 int st;
 do {
   st=statusvu();
   if (st<0||breakcode||sterror) return 0;
   delaytakt(1);
 } while (st==1);
 return 1;
}

int RESCSN ( void )
{
  int st;
 long begtakt = nomertakta;

  while (1) {
    st=statuscsn;
//  printf("такт %d t=%11.5f A=%8.4f L=%8.4f  R=%8.4f\n", nomertakta, rvtime, cd[0], cd[1], cd[2]);

    if (st==0||st==2)  return 1;
    if (st<0) return 0;
    if (breakcode||sterror) return 0;
    if(nomertakta -begtakt >tperebrosa*20+50)//300)//krikunov
    {printf("\n escape timeout! tperebrosa:%7.1f\n",tperebrosa); return 1;}
    delaytakt(1);
  }

}

 
int VU_ZAKR(int *maskaTK)
{
  if (maskaTK[1])
    can_req6(fcan_vu, CAN_ABN_KVU, CAN_CMD_PUTP, _ZATVOR1, 2);
  if (maskaTK[2])
    can_req6(fcan_vu, CAN_ABN_KVU, CAN_CMD_PUTP, _ZATVOR2, 2);
  return 1;
}

int VU_OTKR(int *maskaTK)
{
  if (maskaTK[1])
    can_req6(fcan_vu, CAN_ABN_KVU, CAN_CMD_PUTP, _ZATVOR1, 1);
  if (maskaTK[2])
    can_req6(fcan_vu, CAN_ABN_KVU, CAN_CMD_PUTP, _ZATVOR2, 1);
  return 1;
}


int delay_sec(double dt)
{ 
  int nt = int(dt*10);
  for(int i=0; i<nt; i++) {
    waittakt(1+ nomertakta);
    if (breakcode || sterror) return 0;
  }
  return 1;
} 


void taktst(void)
{
  UPRST &u=uprst;
  double A=_cdf(OS_A), L=_cdf(OS_L), R=_cdf(OS_R);

  savedcd(u,nomertakta,OS_A) = A;
  savedcd(u,nomertakta,OS_L) = L;
  savedcd(u,nomertakta,OS_R) = R;

}


void dump_izmer(int ntk)
{
  int i;
  to_stdout=0;
  printf("============================================================================\n"); 
  printf("ST %2d-%d  TK %d\n", TIPST, NPORST, ntk); 
  printf("%4d-%02d-%02d %02d:%02d:%07.4f\n", 
    rvyear, rvmonth, rvday, rvhour, rvmin, rvsec);

  printf("PASP ");
  for(i=0; i<25; i++) printf("%f ",pasport[ntk][i]);
  printf("\n");

  printf("A=%8.4f L=%8.4f R=%8.4f VA=%6.4f VL=%6.4f VR=%6.4f\n",
     cd[OS_A], cd[OS_L], cd[OS_R], skor[OS_A], skor[OS_L], skor[OS_R]);

  printf("Nobj=%d Nzv=%d Nko=%d\n",
     KolobjAPOI[ntk], KZVAPOI[ntk], KKOAPOI[ntk]);

  for(i=0; i<KolobjAPOI[ntk]; i++) {
    nObj *obj = &ObjAPOI[ntk][i];
    printf("N=%3d T=%10.4f X=%8.3f Y=%8.3f VX=%7.3f VY=%7.3f S=%7.2f V=%8.2f KO=%2d\n",
       i, obj->T, obj->X, obj->Y, obj->DX, obj->DY, obj->S, obj->V, obj->KO); 
  }
  printf("============================================================================\n"); 
  to_stdout=1;
}



double vxapr[5], vyapr[5]; //апр.скорости КО в ТК (дискр/такт)


void getvxvy(double A, double L, double R,
             double Az, double H, double VAz, double VH)
{
  double x0,y0,x1,y1;
  for(int ntk=1;ntk<=MAXTK;ntk++) {
    AHXY(ntk,A,L,R,Az,H,&x0,&y0);
    AHXY(ntk,A,L,R,Az+VAz,H+VH,&x1,&y1);
    vxapr[ntk] = (x1-x0) * MM2PIX(ntk);
    vyapr[ntk] = (y1-y0) * MM2PIX(ntk);
  }
}


void get_VXVY_zv(int ntk, 
       double A, double L, double R, double VA, double VL, double VR,
       double *vxzv, double *vyzv)
{
  double t, Az,H, alfa,delta, vxmm, vymm;
  t=0;
  absmet(ntk, A,L,R, 0,0, &Az,&H);
  AHadsea(Az,H, t, &alfa, &delta);
  adAHsea(alfa, delta, t+1, &Az, &H);
  AHXY(ntk, A+VA, L+VL, R+VR, Az, H, &vxmm, &vymm);
  *vxzv = vxmm * MM2PIX(ntk);
  *vyzv = vymm * MM2PIX(ntk);
}


void IMITZVAPOI(int ntk, 
       double t,
       double A, double L, double R, double VA, double VL, double VR)
{
  OMRDATA &o = omrdata[ntk];
  double vxzv,vyzv;
  int kolobj=0, i;
  double sx = TKsizeXmm(ntk)/2, sy = TKsizeYmm(ntk)/2, magn=17;
  o.time = t;
  o.A = A;
  o.L = L;
  o.R = R;
  getOTKpos(ntk, o.A, o.L, o.R, o.otkpos);
  
  get_catzv(ntk, -sx, sx, -sy, sy, magn);
  get_VXVY_zv(ntk,A,L,R,VA,VL,VR,&vxzv,&vyzv);

  for(i=0; i<o.catkzv; i++) {
    CATZV &zv=o.catzv[i];
    nObj &obj = ObjAPOI[ntk][kolobj];
    if (fabs(zv.x) < sx && fabs(zv.y) < sy) {
       obj.T = time_kpst2apoi(t);
       double x,y;
       coord_mm2pix(ntk, zv.x, zv.y, &x, &y);
       obj.X = x; 
       obj.Y = y;
       obj.DX = vxzv;
       obj.DY = vyzv;
       obj.V = pow(10, (20-zv.v)/2.5);
       obj.S = pow(obj.V, 0.66);
       obj.KO = -1;
       kolobj++;
       if (kolobj == MAXKOLOBJ) break;
    }
  }
  KolobjAPOI[ntk] = kolobj;
  KZVAPOI[ntk] = kolobj;
}

void IMITKOAPOI(int ntk, double t)
{
  int kolobj = KolobjAPOI[ntk];
  nObj &obj = ObjAPOI[ntk][kolobj];
  obj.T = time_kpst2apoi(t);
  double x,y;
  coord_mm2pix(ntk, 0, 0, &x, &y);
  obj.X = x; 
  obj.Y = y;
  obj.DX = 0;
  obj.DY = 0;
  obj.V = 10000;
  obj.S = 100;
  obj.KO = 1;
  kolobj++;
  KolobjAPOI[ntk] = kolobj;
  KKOAPOI[ntk] = 1;
}


/* процедура измерения АПОИ */
int IZMERAPOI(int *maskaTK)
{
 int res;
 int ntk;
 int ntakt=0;

 int timeout = 200; //tmp! таймаут 20 сек


 for(ntk=1; ntk<=MAXTK; ntk++) {
   if (!maskaTK[ntk]) continue;
   KolobjAPOI[ntk] = 0; 
   KZVAPOI[ntk] = 0;
   KKOAPOI[ntk] = 0;
 }

 if (IMITAPOI) goto skip;

 APOI_connect(); 

 for(ntk=1; ntk<=MAXTK; ntk++) {
   double vx, vy;
   if (!maskaTK[ntk]) continue;

   get_VXVY_zv(ntk, cd[0], cd[1], cd[2], skor[0], skor[1], skor[2], &vx, &vy);
   res=APOI_pusk(ntk, NkadrAPOI[ntk], vx, vy);
   to_stdout=0;
   printf("NTK=%d Пуск АПОИ Nkadr=%d vxzv=%f vyzv=%f\n", ntk, NkadrAPOI[ntk], vx, vy);
   to_stdout=1;
   if (res==0) return 0;
 }

 while(1)
 { 
    int stat;
   
    stat=0;
    for(ntk=1; ntk<=MAXTK; ntk++) {
      if (!maskaTK[ntk]) continue;
      if (STATUSAPOI[ntk]==1) stat=1;
    }

    if (stat==0) break;
    if (sterror || breakcode) return 0;

    ntakt++;
    if (ntakt > timeout) {
       sterror=1;
       strcpy(sterrmsg1, "timeout from IZMERAPOI!");
       return 0;
    }

    delaytakt(1);
 }

skip:

 if (!IMITCTK) 
    for(ntk=1; ntk<=MAXTK; ntk++) {
      if (!maskaTK[ntk]) continue;
      CTK_get_param(ntk);
    }

 if (IMITAPOI)
    for(ntk=1; ntk<=MAXTK; ntk++) {
      double t;
      if (!maskaTK[ntk]) continue;
      t = rvtime;
      IMITZVAPOI(ntk, t, cd[0], cd[1], cd[2], skor[0], skor[1], skor[2]);
      IMITKOAPOI(ntk, t);
    }

 if (dumpizmer) {
    for(ntk=1; ntk<=MAXTK; ntk++) {
      if (!maskaTK[ntk]) continue;
      dump_izmer(ntk);
    }
 }

 return 1;
}



void get_izmzv(int ntk)
{
  OMRDATA &o=omrdata[ntk];
  IZMZV *izmzv=o.izmzv;
  int i,j,k,kzv=0;
  double T, Tsr=0, VXsr=0, VYsr=0;
  int maxkzv = o.maxizmkzv;

  for(i=0; i<KolobjAPOI[ntk]; i++) {
    nObj &obj = ObjAPOI[ntk][i];
    if (obj.KO != -1) continue;
    T = time_apoi2kpst(obj.T);
    Tsr += T;
    VXsr += obj.DX;
    VYsr += obj.DY;
    kzv++;
  }

  if (kzv==0) { o.izmkzv=0; return; }
 
  Tsr /= kzv;
  VXsr /= kzv;
  VYsr /= kzv;
  //printf("VXsr=%f VYsr=%f\n",VXsr,VYsr);

  o.time = Tsr;
  o.VXzv = VXsr;
  o.VYzv = VYsr;

  kzv = 0;
  for(i=0; i<KolobjAPOI[ntk]; i++) {
    nObj &obj = ObjAPOI[ntk][i];
    if (obj.KO != -1) continue;
    for(j=0;j<kzv;j++) if (obj.V>izmzv[j].e) break;
    if (kzv<maxkzv) kzv++;
    if (j<kzv) {
      for(k=kzv-1;k>j;k--) izmzv[k]=izmzv[k-1];
      T = time_apoi2kpst(obj.T);
      double x,y;
      coord_pix2mm(ntk, obj.X +VXsr*(Tsr-T), obj.Y +VYsr*(Tsr-T), &x, &y);
      izmzv[j].x = x;
      izmzv[j].y = y;
      izmzv[j].e = obj.V;
    }
  }

  o.izmkzv=kzv;
}


int oshibkiOMR[3];

int MetodOMR(int ntk)
{
  OMRDATA &o=omrdata[ntk];

  double sx,sy,limitmag;
  extern double RS;

  RS = 0.5*GR*pasF(ntk);
  sx = TKsizeXmm(ntk)/2 + RS;
  sy = TKsizeYmm(ntk)/2 + RS;
#ifdef  BL051
  limitmag = 17;//21.03.  krikunov
#else
  limitmag = 15;
#endif
  o.catkzv=o.izmkzv=o.omrkzv=0;

  get_izmzv(ntk);
  if (o.izmkzv==0) return 0;

  o.A= cd[OS_A] + (o.time-timecd[OS_A]) * skor[OS_A];
  o.L= cd[OS_L] + (o.time-timecd[OS_L]) * skor[OS_L];
  o.R= cd[OS_R] + (o.time-timecd[OS_R]) * skor[OS_R];
  getOTKpos(ntk,o.A,o.L,o.R,o.otkpos);

  get_catzv(ntk,-sx,sx,-sy,sy,limitmag);
  if (o.catkzv==0) return 0;

  clear_redomr(o.red);
  Otogdestv(o);

//  if (kzvapoi>=30 && o.catkzv>=5 && o.omrkzv<=2) oshibkiOMR[NTK]++;
//  else oshibkiOMR[NTK]=0;

  return o.omrkzv;
}



void MODELKO_CVS(int *maskaTK)
{
 UPRST &u=uprst;
 int ntk,nko=0;
 double t,A,L,R,Az,H,Az1,H1,x,y,x1,y1,a,d,a1,d1;
 for (ntk=1;ntk<=MAXTK;ntk++)
   if (RABTK[ntk]) {
        KOINF &ko=u.ko[nko];
        ko.ntk=ntk;
	t = rvtime;
//  printf("t=%f\n",t);
	GetKoordModel(t,&Az,&H);
	GetKoordModel(t+1,&Az1,&H1);
//  void GetKoordModel_1(double ,double *, double *);
// 	GetKoordModel_1(t,&Az,&H);
// 	GetKoordModel_1(t+1,&Az1,&H1);
        A=cd[OS_A]; L=cd[OS_L]; R=cd[OS_R];
//      printf("A=%f L=%f R=%f\n",A,L,R);
        OTKPOS pos;
        getOTKpos(ntk,A,L,R,pos);
        GorTV(pos,Az,H,&x,&y);
        A+=skor[OS_A]; L+=skor[OS_L]; R+=skor[OS_R];
        getOTKpos(ntk,A,L,R,pos);
        GorTV(pos,Az1,H1,&x1,&y1);
        printf("x=%f y=%f\n",x,y);
        if (fabs(x) < TKsizeXmm(ntk) && fabs(y) < TKsizeYmm(ntk)) {
          ko.t=t;
          ko.A=A; ko.L=L; ko.R=R; ko.x=x; ko.y=y; ko.e=1;
          ko.vx=x1-x; ko.vy=y1-y;
          ko.Az=Az; ko.H=H; ko.VAz=Az1-Az; ko.VH=H1-H;
          ko.metod=0;
//        alfadelta2000_ko(ko);
          AHadsea(ko.Az,        ko.H,       ko.t,   &a,  &d);
          AHadsea(ko.Az+ko.VAz, ko.H+ko.VH, ko.t+1, &a1, &d1);
          TekJ2000sea (0,&a,&d);
          TekJ2000sea (0,&a1,&d1);
          ko.alfa=a;      ko.delta=d;
          ko.valfa=a1-a;  ko.vdelta=d1-d;
          if(ko.valfa> 2*M_PI) ko.valfa-=2*M_PI;
          if(ko.valfa<-2*M_PI) ko.valfa+=2*M_PI;
          ko.blesk=15;
          nko++;
        }
   }
 u.kolko=nko;
}

/*
void getparamko_am(int ntk, nObj &obj, KOINF &ko)
{
  double
    tko, A,L,R,
    xko,yko, Az,H, a,d, a2000, d2000,
    xko1,yko1, Az1,H1, a1,d1;

  tko = time_apoi2kpst(obj.T);
  A = ?
  L = ?
  R = ?
  xko = obj.X; 
  yko = obj.Y;
  xko1 = xko + obj.DX;
  yko1 = yko + obj.DY;
  
  coord_pix2mm(ntk, xko, yko, &xko, &yko); 
  coord_pix2mm(ntk, xko1, yko1, &xko1, &yko1); 

  absmet(ntk, A,L,R, xko,yko, &Az,&H);
  MinusRefraction(&H);
  AHadsea(Az,H, tko, &a, &d);

  absmet(ntk,A,L,R,xko1,yko1,&Az1,&H1);
  MinusRefraction(&H1);
  AHadsea(Az, H, tko+1, &a1, &d1);

  VAz = Az1-Az;
  VH = H1-H;
  if (VAz> 2*M_PI) VAz-=2*M_PI;
  if (VAz<-2*M_PI) VAz+=2*M_PI;
  va = a1-a;
  vd = d1-d;  
  if (va> 2*M_PI) va-=2*M_PI;
  if (va<-2*M_PI) va+=2*M_PI;

  a2000=a; d2000=d; TekJ2000sea (0,&a2000,&d2000);

  ko.metod = 0;  
  ko.Az    = Az;
  ko.H     = H;
  ko.VAz   = VAz;
  ko.VH    = VH;
  ko.alfa  = a2000;      
  ko.delta = d2000;
  ko.valfa = va;  
  ko.vdelta= vd;
}
*/

void getparamko_omr(int ntk, nObj &obj, KOINF &ko)
{
  OMRDATA &o = omrdata[ntk];
  double
    tko, tzv, A,L,R,
    xko,yko, Az,H, a,d, a2000,d2000,
    xko1,yko1, Az1,H1, a1,d1, VAz, VH, va, vd,
    vxzv,vyzv;

  tko = time_apoi2kpst(obj.T);
  tzv = o.time;
  A = o.A;
  L = o.L;
  R = o.R;
  vxzv = o.VXzv;
  vyzv = o.VYzv;
  xko = obj.X-(tko-tzv)*vxzv; 
  yko = obj.Y-(tko-tzv)*vyzv;
  xko1 = xko + obj.DX - vxzv;
  yko1 = yko + obj.DY - vyzv;
  
//printf("vxzv=%f vyzv=%f\n",vxzv,vyzv);
//printf("xko=%f yko=%f\n",xko,yko);
//printf("xko1=%f yko1=%f\n",xko1,yko1);

  coord_pix2mm(ntk, xko, yko, &xko, &yko); 
  coord_pix2mm(ntk, xko1, yko1, &xko1, &yko1); 

//printf("xko=%f yko=%f\n",xko,yko);
//printf("xko1=%f yko1=%f\n",xko1,yko1);

  get_reduction_OMR(o, xko, yko);

  korrxyOMR(ntk,&xko,&yko);
  absmet(ntk, A,L,R, xko,yko, &Az,&H);
  MinusRefraction(&H);
  AHadsea(Az,H, tzv, &a, &d);
  adAHsea(a,d, tko, &Az,&H);

  korrxyOMR(ntk,&xko1,&yko1);
  absmet(ntk,A,L,R,xko1,yko1,&Az1,&H1);
  MinusRefraction(&H1);
  AHadsea(Az1, H1, tzv, &a1, &d1);
  adAHsea(a1,d1, tko+1, &Az1,&H1);

//printf("xko=%f yko=%f\n",xko,yko);
//printf("Az=%f H=%f\n",Az*RG,H*RG);
//printf("a=%f d=%f\n",a*RH,d*RG);
//printf("xko1=%f yko1=%f\n",xko1,yko1);
//printf("Az1=%f H1=%f\n",Az1*RG,H1*RG);
//printf("a1=%f d1=%f\n",a1*RH,d1*RG);

  VAz = Az1-Az;
  VH = H1-H;
  if (VAz> 2*M_PI) VAz-=2*M_PI;
  if (VAz<-2*M_PI) VAz+=2*M_PI;
  va = a1-a;
  vd = d1-d;  
  if (va> 2*M_PI) va-=2*M_PI;
  if (va<-2*M_PI) va+=2*M_PI;

  a2000=a; d2000=d; TekJ2000sea (0,&a2000,&d2000);

  ko.metod = 1;  
  ko.t     = tko;
  ko.ntk   = ntk;
  ko.blesk = get_zvvel(o.rzv, obj.V);
  ko.Az    = Az;
  ko.H     = H;
  ko.VAz   = VAz;
  ko.VH    = VH;
  ko.alfa  = a2000;      
  ko.delta = d2000;
  ko.valfa = va;  
  ko.vdelta= vd;
}


void IZMERKO(int *maskaTK)
{
  UPRST &u=uprst;
  int ntk,nko=0,i;
  for (ntk=1;ntk<=MAXTK;ntk++)
    if (maskaTK[ntk]) {
      OMRDATA &o = omrdata[ntk];

      for (i=0; i<KolobjAPOI[ntk]; i++) {
        nObj &obj = ObjAPOI[ntk][i];
        KOINF &ko=u.ko[nko];
        double dt;

        if (obj.KO != 1) continue;
        
        if (o.omrkzv>=5) { 
           getparamko_omr(ntk, obj, ko);
           nko++;
        }

        if (nko==10) { u.kolko=nko; return; }
      }
    }

  u.kolko=nko;
}


void Send_ESTKI(KOINF &ko)
{
  est_ki_ts_type* s;
  s = (est_ki_ts_type*)malloc( sizeof(est_ki_ts_type) );
  TSSOOB(s) = ESTKI_TS;
  OTSOOB(s) = interNST();
  A_OBN (s) = ko.Az*RG;
  H_OBN (s) = ko.H*RG;
  T_OBN (s) = time_kpst2kbp(ko.t);
  MAXPOL_OBN(s) = 1./60.;
  MAXTIME_OBN(s) = 1;
  BLESK_OBN(s) = ko.blesk;
  VA_OBN (s) = ko.VAz*RG;
  VH_OBN (s) = ko.VH*RG;
  TK_OBN (s) = ko.ntk;
  MET_OBN(s) = (ko.metod? OMR_MET:AM_MET);
  ALFA_OBN(s)   = ko.alfa*RG;
  DELTA_OBN(s)  = ko.delta*RG;
  VALFA_OBN(s)  = ko.valfa*RG;
  VDELTA_OBN(s) = ko.vdelta*RG;

  int ntk=ko.ntk;
  OMRDATA &o=omrdata[ntk];
  KZVIZM_OBN(s) = KZVAPOI[ntk];
  KZVKAT_OBN(s) = o.catkzv;
  KZVOMR_OBN(s) = o.omrkzv;

  if (IMIT) {
    MET_OBN(s) = OMR_MET;
    KZVKAT_OBN(s) = 3;
    KZVOMR_OBN(s) = 3;
  }


  send_to_kp54(CVS_BZ,s,sizeof(est_ki_ts_type));
  free(s);
//printf("послано сбщ. ЕСТЬКИ A=%7.3f H=%6.3f T=%8.2f "
//       "MAXPOL=%5.3f MAXTIME=%5.3f BLESK=%4.1f\n"
//       "VA=%7.5f VH=%7.5f TK=%d MET=%s\n",
//A_OBN(s),H_OBN(s),T_OBN(s),
//MAXPOL_OBN(s),MAXTIME_OBN(s),BLESK_OBN(s),
//VA_OBN(s),VH_OBN(s),TK_OBN(s),(MET_OBN(s)==OMR_MET?"omr":"am"));
}


void PRINTKOhdr2(void)
{
 printf(
 " N  ОТК  Мет      T           A       H      VA      VH    блеск(зв.в)"
//**   s   sss  **:**:**.**  ***.**  ***.**  **.**** **.****  **.*
 "\n");
}

void PRINTKO2(int nko)
{
 UPRST &u=uprst;
 KOINF &ko=u.ko[nko];
 int ntk=ko.ntk;
 double t=ko.t, Az=ko.Az, H=ko.H, VAz=ko.VAz, VH=ko.VH;
 int h,m; double s;
 if (t>43200.) t-=43200.; else t+=43200.;
 h=t/3600.; m=(t-h*3600.)/60; s=t-h*3600.-m*60.;
 printf(
  "%2d   %c   %3s  %02d:%02d:%05.2f  %6.2f  %6.2f  %7.4f %7.4f  %4.1f\n",
  nko+1,otk_letter(TIPST,ntk),(ko.metod?"ОМР":"АМ"),
  h,m,s,Az*RG,H*RG,VAz*RG,VH*RG,ko.blesk
       );
}


int OBRKO(int *maskaTK, int omr)
{
 UPRST &u=uprst;
 int ntk,n,kol,i,paused=0;
 int kolkotk[5];

 if (IMITAPOI&&CVS==1) MODELKO_CVS(maskaTK);
 else {

   for(ntk=1; ntk<=MAXTK; ntk++) 
     kolkotk[ntk]=0;

   for(n=0; n<u.kolko; n++) {
      int ntk=u.ko[n].ntk;
      kolkotk[ntk]++;
   }

   for(ntk=1;ntk<=MAXTK;ntk++) {
     if (!maskaTK[ntk]) continue;
     if (kolkotk[ntk])
       printf("В ОТК-%c обнаружен%s %d КО\n",
	 otk_letter(TIPST,ntk),(kolkotk[ntk]>1?"о":""),kolkotk[ntk]);
   }
   if (omr&&u.kolko>0)
     for(ntk=1;ntk<=MAXTK;ntk++)
       //if (kolkotk[ntk]) {
       if (KKOAPOI[ntk]>0) {
         print2OMR(ntk);
         if (oshibkiOMR[ntk]>=3) {
           printf("\n*** Отказ распознавания звезд в ОТК-%c ***\n\n",
                  otk_letter(TIPST,ntk));
         }
       }
 }

 kol=u.kolko;

 if (kol>0) {

   extern int vdol_flag;
   if (vdol_flag) {
     void PRINTKO_vdol(void);
     PRINTKO_vdol();
   } else {
     PRINTKOhdr2();
     for(n=0;n<kol;n++) PRINTKO2(n);
   }

   if (CVS==1)
     for(n=0;n<kol;n++) {
       Send_ESTKI(u.ko[n]);
     }

   void identif_and_print(int nko);
   if (identif_flag)
     for(n=0;n<kol;n++) identif_and_print(n);

 }

 return 1;
}



void skorchvcsn (double A, double L, double R,
		   double* VA, double* VL, double* VR,
			   int bezA)
{
  if (chv_bezA==0||bezA==0) goto sA;
  skorchvcsn_bezA(A,L,R,VA,VL,VR);
  if (fabs(*VL)<40/3600.) return;
sA:
  skorchvcsn_A(A,L,R,VA,VL,VR);
}

int PUSKCHV(double A, double L, double R)
{
 UPRST &u=uprst;
 double va,vl,vr;

 perebroscsn(A,L,R);
 tperebrosa =Tperebros(cd[0],cd[1],cd[2],A,L,R);
 if (!RESCSN()) return 0;

//// tsarev 16.06.2013
 if (A!=999)
 {
  can_req0(fcan_kep,CAN_ABN_KEPA,CAN_CMD_STOP);
  usleep(200);
  can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUTP,m2b('M',' '),0);
  usleep(200);
 }
 if (L!=999)
 {
  can_req0(fcan_kep,CAN_ABN_KEPL,CAN_CMD_STOP);
  usleep(200);
  can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUTP,m2b('M',' '),0);
  usleep(200);
 }
 if (R!=999)
 {
  can_req0(fcan_kep,CAN_ABN_KEPR,CAN_CMD_STOP);
  usleep(200);
  can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUTP,m2b('M',' '),0);
  usleep(200);
 }
//NkadrAPOI==1 при проверке ПСИ на скорость обзора - убираем задержки 28.08.13
 if (((A!=999) || (L!=999) || (R!=999)) && (NkadrAPOI[1]>1))
 {
   delaytakt(10);
 }
//// tsarev 16.06.2013

 if (A==999) A=_cdf(OS_A);
 if (L==999) L=_cdf(OS_L);
 if (R==999) R=_cdf(OS_R);
 skorchvcsn(A,L,R,&va,&vl,&vr);

 skorcsn(va,vl,vr);

 u.chv_a=va; u.chv_l=vl; u.chv_r=vr;
 return 1;
}


int sravni_ko(KOINF &ko1, KOINF &ko2)
{
  double dt, d, v, dv, maxosh, maxoshv;
  if (ko1.ntk!=ko2.ntk) return 0;
  maxosh=0.02;
  v=max(fabs(ko1.vx),fabs(ko1.vy));
  maxoshv=max(0.2,0.2*v);
  dt=ko2.t-ko1.t;
  d=max( fabs(ko2.x-(ko1.x+ko1.vx*dt)), fabs(ko2.y-(ko1.y+ko1.vy*dt)) );
  if (d>maxosh+maxoshv*fabs(dt)) return 0;
  dv=max(fabs(ko2.vx-ko1.vx), fabs(ko2.vy-ko1.vy));
  if (dv>1.5*maxoshv) return 0;
  return 1;
}


double Tperebros(double A0, double L0, double R0,
                 double A1, double L1, double R1)
{
  double dA=fabs(A1-A0), dL=fabs(L1-L0), dR=fabs(R1-R0),
	 d;
  double V= maxskor[OS_A],//10,
          a=3.5, t_razgon=V/a, d_razgon=V*V/(2*a), t;
if(A1==999) dA=0;if(L1==999) dL=0;if(R1==999) dR=0;//28.05.2013
  d=dA;
  if (d<dL) d=dL;
  if (d<dR) d=dR;
  if (d<2*d_razgon) {
    t=2*sqrt(d/a);
  } else {
    t=2*t_razgon+(d-2*d_razgon)/V;
  }
  return t+1.0;
}

int NavedShort(int NTK,
	       double Az, double H, double XT, double YT,
	       double *A1, double *L1, double *R1)
{
 double A0=cd[0], L0=cd[1], R0=cd[2],
        A,L,R,ZX,da,dl,dr,d,dmin=360,ZXmin,step=1,
        minL=MINCD(OS_L),
        maxL=MAXCD(OS_L),
        minR=MINCD(OS_R),
        maxR=MAXCD(OS_R);
 if (H<5*GR) return 0;
 for(ZX=0; ZX<360; ZX+=step) {
   Naved(NTK,Az,H,ZENIT,ZX,XT,YT,&A,&L,&R);
   if (L<minL+0.5 || L>maxL-0.5 || R<minR+0.5 || R>maxR-0.5) continue;
   da=fabs(A-A0); dl=fabs(L-L0); dr=fabs(R-R0);
   d=da; if (dl>d) d=dl; if(dr>d) d=dr;
   if (d<dmin) { dmin=d; ZXmin=ZX; }
 }
 if (dmin==360) return 0;
 Naved(NTK,Az,H,ZENIT,ZXmin,XT,YT,A1,L1,R1);
 return 1;
}


int NavedShort_spec(int NTK,
	       double Az, double H, double VAz, double VH,
	       double *A1, double *L1, double *R1)
{
 double A0=_cdf(0), L0=_cdf(1), R0=_cdf(2),
        A,L,R,ZX,da,dl,dr,d,dmin=360,ZXmin,step=1,
        minL=MINCD(OS_L),
        maxL=MAXCD(OS_L),
        minR=MINCD(OS_R),
        maxR=MAXCD(OS_R),
        x0,y0,x1,y1,VL;
 if (H<5*GR) return 0;
 for(ZX=0; ZX<360; ZX+=step) {
   Naved(NTK,Az,H,ZENIT,ZX,0,0,&A,&L,&R);
   if (L<minL||L>maxL||R<minR||R>maxR) continue;

   AHXY(NTK,A,L,R,Az,H,&x0,&y0);
   AHXY(NTK,A,L,R,Az+VAz,H+VH,&x1,&y1);
   VL=(x1-x0)/pasF(NTK)/sin(R*GR);
   if (fabs(VL)>100./3600*GR) continue;

   da=fabs(A-A0); dl=fabs(L-L0); dr=fabs(R-R0);
   d=da; if (dl>d) d=dl; if(dr>d) d=dr;
   if (d<dmin) { dmin=d; ZXmin=ZX; }
 }
 if (dmin==360) return 0;
 Naved(NTK,Az,H,ZENIT,ZXmin,0,0,A1,L1,R1);
 return 1;
}



int NavedKO(int NTK,
            int fot_flag,
            double T, double Az, double H, double VAz, double VH,
            double *A, double *L, double *R)
{
  double t,A0,L0,R0,A1,L1,R1;
  double dt=5;
  A0=_cdf(0); L0=_cdf(1); R0=_cdf(2);
  t=rvtime+dt;
  for(int i=0;i<3;i++) {
    if (fot_flag) {
     if (!NavedShort_spec(NTK,Az+VAz*(t-T),H+VH*(t-T),VAz,VH,
                          &A1,&L1,&R1))
       return 0;
    } else {
     if (!NavedShort(NTK,Az+VAz*(t-T),H+VH*(t-T),0,0,
                          &A1,&L1,&R1))
       return 0;
    }
    t=rvtime+dt+Tperebros(A0,L0,R0,A1,L1,R1);
  }
  *A=A1;
  *L=L1;
  *R=R1;
  return 1;
}


int SCANDISKR(ZONA &z)
{
 UPRST &u=uprst;
 double A,L,R;
 double before = 0, posle = 0;
 int ntk;
 double R1;
 int i1=1,i2=1;
 A=z.A;
 L=z.LN+z.DL*z.iL;
#ifdef  BL051
 before=getmks(); //POISKV
#endif
 for( ;z.iL<z.NL;z.iL++) {
   printf("дуга %d\n",z.iL+1);
#ifdef BL051  //
   if (NkadrAPOI[1]==1)
     i2=i1%2;   //i1-i1/2*2;
#endif
   R=z.RN+z.DR*z.iR;
   R1=z.RK-z.DR*z.iR;
   if (i2==1)
   {
     printf("\n L duga: perebros to %7.3f %7.3f %7.3f\n",A,L,R);
     PUSKCHV(z.A,L,R);
   }
   else
   {
     printf("\n L duga: perebros to %7.3f %7.3f %7.3f\n",A,L,R1);
     PUSKCHV(z.A,L,R1);
   }
printf("После переустановки дуги : A=%8.4f  L=%8.4f  R=%8.4f\n", cd[0], cd[1], cd[2]);
   for( ;z.iR<z.NR;z.iR++) {
//   printf("A=%8.4f  L=%8.4f  R=%8.4f\n", cd[0], cd[1], cd[2]);
     printf("\n     кадр %d...\n",z.iR+1);
     VU_OTKR(RABTK);

            if (NkadrAPOI[1]>1)     //28.08,13
            delaytakt(25);//2.5 sek//delay_sec(2); //  !
   
     if (!IZMERAPOI(RABTK)) continue;// return 0; BL051
//printf("\n After IZMERAPOI");
     for(ntk=1; ntk<=MAXTK; ntk++) {
       if (!RABTK[ntk]) continue;
       if (KKOAPOI[ntk]>0)
         MetodOMR(ntk);
     }
//printf("\n After ntk");
     IZMERKO(RABTK);
//printf("\n After IZMERKO");
     if (u.kolko>0) {
       if (!OBRKO(RABTK,1)) continue;// BL051return 0;
     }
//printf("\n After OBRKO");
     rabOK();
  
//printf("\n After rabOK");
     if (z.iR<z.NR-1) {
       R+=z.DR;
       R1-=z.DR;
//before=getmks();
   //    perebroscsn(A, L, R);
#ifdef BL051  //06.06.13
   if (i2==1)
     docum_perebroscsn(999,999,R);
   else
     docum_perebroscsn(999,999,R1);
#else
 perebroscsn(999, 999,R);
#endif

   if (i2==1)
     printf("\n R kadr: perebros to %7.3f %7.3f %7.3f",A,L,R);
   else
     printf("\n R kadr: perebros to %7.3f %7.3f %7.3f",A,L,R1);
       if (!RESCSN()) continue;//return 0; BL051
//posle = getmks();
//printf("\n Time Perebrosa: %8.2f mlsec\n", (posle-before)*1000.);
//        printf("\n After RESCSN\n");
     }
     PUSKCHV(999,999,999); // 16.06.2013
     z.percent+=100./(z.NL*z.NR);
   }
   printf("\n");
   L+=z.DL; z.iR=0;
   i1++;
 }
#ifdef  BL051
 posle = getmks();
 printf("\n Размеры Зоны поиска:  %7.2f[grad] x %7.2f[grad] =%8.1f[grad^2] t:%7.1f sec\n",fabs(z.RK-z.RN),fabs(z.LK-z.LN),fabs(z.RK-z.RN)*fabs(z.LK-z.LN),(posle-before));
//fprintf("\n Размеры Зоны поиска:  %7.2f[grad] x %7.2f[grad] t:%7.1 secf\n",fabs(z.RK-z.RN),fabs(z.LK-z.LN),(posle-before));
 printf("\n Поисковые возможности: %10.3f [kv.grad]/час\n",fabs(round(z.RK-z.RN))*fabs(round(z.LK-z.LN)) /(posle-before)*3600.);
#endif
 stopcsn();
 return RESCSN();

}


int BARIER56(ZONA &z)
{
 UPRST &u=uprst;
 double A,L,R;
 int ntk;
 A=z.A;
 L=z.LN;
 R=z.RN;
 PUSKCHV(z.A,L,R);
 VU_OTKR(RABTK);
// if (!RESVU()) return 0;

 printf("выполнен переброс\n");
 delay_sec(5); //пауза 2 сек
 printf("начато наблюдение на барьере\n");
 
 for(;;) {
   if (!IZMERAPOI(RABTK)) return 0;

   for(ntk=1; ntk<=MAXTK; ntk++) {
     if (!RABTK[ntk]) continue;
     if (KKOAPOI[ntk]>0)
       MetodOMR(ntk);
   }

   IZMERKO(RABTK);
   if (u.kolko>0) {
     if (!OBRKO(RABTK,1)) return 0;
   }
   rabOK();
   z.percent=100.;
 }

 stopcsn();
 return RESCSN();
}


int OBNTCHK(double A, double L, double R)
{
 UPRST &u=uprst;
 int ntk;
//printf("\n BEFORE PUSKCHV");
//delay_sec(3);

//#avm# 08/07/2013
printf("Наводимся в точку A=%f L=%f R=%f\n", A, L, R);
 if (!PUSKCHV(A,L,R)) return 0;
 printf("Навелись в точку A=%f L=%f R=%f\n", cd[0], cd[1], cd[2]);
 ntk=1;
 double Az, H;
 absmet(ntk, cd[0], cd[1], cd[2], 0, 0, &Az, &H);
 printf("Коорд.центра канала %d: Az=%f H=%f\n", ntk, Az*RG, H*RG);
//#avm# 08/07/2013

 VU_OTKR(RABTK);
 //if (!RESVU()) return 0;
//printf("\n BEFORE DELAY");
 delay_sec(2); //пауза 2 сек
//printf("\n AFTER DELAY");
 if (!IZMERAPOI(RABTK)) return 0;

 for(ntk=1; ntk<=MAXTK; ntk++) {
   if (!RABTK[ntk]) continue;
   if (KKOAPOI[ntk]>0)
     MetodOMR(ntk);
 }

 IZMERKO(RABTK);
 if (u.kolko>0 || (IMITAPOI && CVS==1))
     if (!OBRKO(RABTK,1)) return 0;

 stopcsn();
 rabOK();
 return 1;
}

