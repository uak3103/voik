extern int naved_zvezda(int ntk, double alfa2000, double delta2000);
void fprintgms(FILE *f, double x)
{
 int sign=1,g,m; double s;
 x*=3600; if (x<0) {sign=-1;x=-x;}
 g=(int)(x/3600); x-=g*3600.;
 m=(int)(x/60); s=x-m*60;
 if (g==0&&sign==-1) fprintf(f,"%4s","-0");
 else fprintf(f,"%4d",g*sign);
 fprintf(f,"%c%02d'%02d\"",0xF8,m,(int)s);
}

void printforpasp(int NTK, int n, FILE *f)
{
  OMRDATA &o=omrdata[NTK];
  double x=0,y=0,Az,H;
  fprintf(f,"//Шаг %d: распознано %d звезд из %d\r\n",n,
              o.omrkzv, // кол-во распознанных звезд
              o.catkzv);// общее кол-во каталожных звезд
  if (o.omrkzv==0) return;
  korrxyOMR(NTK,&x,&y);
  absmet(NTK,o.A,o.L,o.R,x,y,&Az,&H);
  fprintf(f,"A = ");    fprintgms(f,o.A);
  fprintf(f,"  L = ");  fprintgms(f,o.L);
  fprintf(f,"  R = ");  fprintgms(f,o.R);
  fprintf(f,"\r\nAZ = "); fprintgms(f,Az*RG);
  fprintf(f,"  H = ");  fprintgms(f,H*RG);
  fprintf(f,"\r\n");
}

void testpasp(int n1, int n2)
{
 double acd[25] = { 0 , 90 , 0 , 90 , 0 ,-90, 0 , 90 , 0   , -90 ,  0 ,
		   -90,  0 , 90,  0 ,-90, 0 ,180,  0 , 180 , 0   , 180,
		    0 ,180 , 0 };
 double lcd[25] = { 90, 55 , 90, 125, 55, 90, 55, 125, 90  , 125 , 55 ,
		    90, 30 , 55, 90 , 55,125, 45, 90 , 45  , 150 , 135,
		   125, 135,125};
 double rcd[25] = { 90, 55 , 30, 125, 55,125, 90, 55 , 125 , 90  , 125,
		    55, 90 ,125, 55 , 90, 55, 45, 150, 135 , 90  , 45 ,
		    90, 135, 125};
 double alf[25]={0},dlf[25]={0}; double alfa=0,delta = 0;
 FILE *f[5];
 int n,i;int res=0;
 int ntk=1;// BIK==2 ! ! !
 // ntk==1 for SHPK ! ! !
 if (n1<1) n1=1;
 if (n2>25) n2=25;
 /*
 alf[0] =  hmshour(20 ,44, 21.1); dlf[0]=gmsgrad(29 ,29, 51.31 );alf[0]*=M_PI/12; dlf[0]*=M_PI/180
 alf[1] =  hmshour(20 ,55, 35.4); dlf[1]=gmsgrad(39 ,58, 1.94 );alf[1]*=M_PI/12; dlf[1]*=M_PI/180
 alf[2] =  hmshour(21 ,3, 10.41); dlf[2]=gmsgrad(33 ,2, 14.69 );alf[2]*=M_PI/12; dlf[2]*=M_PI/180
 alf[3] =  hmshour(20 ,56, 24.44);  dlf[3]=gmsgrad(31 ,38, 12.49 );alf[3]*=M_PI/12; dlf[3]*=M_PI/180
 alf[4] =  hmshour(20 ,46, 37.48);dlf[4]=gmsgrad(30 ,26, 52.68 );alf[4]*=M_PI/12; dlf[4]*=M_PI/180
 alf[5] =  hmshour(20 ,43, 45.32);dlf[5]=gmsgrad(31 ,18, 42.89 );alf[5]*=M_PI/12; dlf[5]*=M_PI/180
 alf[6] =  hmshour(20 ,52, 52.32);dlf[6]=gmsgrad(14 ,20, 54.2 );alf[6]*=M_PI/12; dlf[6]*=M_PI/180
 alf[7] =  hmshour(20 ,53, 9.08);dlf[7]=gmsgrad(27 ,43, 38.99 );alf[7]*=M_PI/12; dlf[7]*=M_PI/180
 alf[8] =  hmshour(22 ,8, 53.31);dlf[8]=gmsgrad(47 ,4, 46.12 );alf[8]*=M_PI/12; dlf[8]*=M_PI/180
 alf[9] =  hmshour(22 ,9, 31.76);dlf[9]=gmsgrad(47 ,0, 32.74 );alf[9]*=M_PI/12; dlf[9]*=M_PI/180
 alf[10] =  hmshour(18 ,27, 17.97);dlf[10]=gmsgrad(60 ,54, 34.13 );alf[10]*=M_PI/12; dlf[10]*=M_PI/180
 alf[11] =  hmshour(18 ,10, 30.59);dlf[11]=gmsgrad(32 ,52, 33.48 );alf[11]*=M_PI/12; dlf[11]*=M_PI/180
 alf[12] =  hmshour(18 ,10, 31.78);dlf[12]=gmsgrad(32 ,51, 37.4 );alf[12]*=M_PI/12; dlf[12]*=M_PI/180
 alf[13] =  hmshour(18 ,11, 51.58);dlf[13]=gmsgrad(43 ,7, 33.34 );alf[13]*=M_PI/12; dlf[13]*=M_PI/180
 alf[14] =  hmshour(22 ,11, 48.94);dlf[14]=gmsgrad(47 ,11, 20.21 );alf[14]*=M_PI/12; dlf[14]*=M_PI/180
 alf[15] =  hmshour(20 ,58, 30.4);dlf[15]=gmsgrad(27 ,37, 34.12 );alf[15]*=M_PI/12; dlf[15]*=M_PI/180
 alf[16] =  hmshour(18 ,11, 55.17);dlf[16]=gmsgrad(43 ,8, 48.35 );alf[16]*=M_PI/12; dlf[16]*=M_PI/180
 alf[17] =  hmshour(20 ,44, 21.1); dlf[17]=gmsgrad(29 ,29, 51.31 );alf[17]*=M_PI/12; dlf[17]*=M_PI/180
 alf[18] =  hmshour(20 ,55, 35.4); dlf[18]=gmsgrad(39 ,58, 1.94 );alf[18]*=M_PI/12; dlf[18]*=M_PI/180
 alf[19] =  hmshour(21 ,3, 10.41); dlf[19]=gmsgrad(33 ,2, 14.69 );alf[19]*=M_PI/12; dlf[19]*=M_PI/180
 alf[20] =  hmshour(18 ,11, 58.65);  dlf[20]=gmsgrad(43 ,9, 4.53 );alf[20]*=M_PI/12; dlf[20]*=M_PI/180
 alf[21] =  hmshour(18 ,12, 8.42);dlf[21]=gmsgrad(43 ,7, 22.6 );alf[21]*=M_PI/12; dlf[21]*=M_PI/180
 alf[22] =  hmshour(20 ,43, 45.32);dlf[22]=gmsgrad(31 ,18, 42.89 );alf[22]*=M_PI/12; dlf[22*=M_PI/180
 alf[23] =  hmshour(18 ,8, 8.42);dlf[23]=gmsgrad(43 ,7, 22.6 );alf[23]*=M_PI/12; dlf[23]*=M_PI/180
 alf[24] =  hmshour(20 ,53, 30.94);dlf[24]=gmsgrad(30 ,19, 49.0 );alf[24]*=M_PI/12; dlf[24]*=M_PI/180*/

/*
 alf[ 0] =    4.76686;  dlf[ 0]=   0.51392;
 alf[ 1] =    4.84412;  dlf[ 1]=   0.78233;
 alf[ 2] =    5.11550;  dlf[ 2]=   0.21402;
 alf[ 3] =    5.15939;  dlf[ 3]=   0.30679;
 alf[ 4] =    5.17516;  dlf[ 4]=   0.20884;
 alf[ 5] =    5.18350;  dlf[ 5]=   0.37843;
 alf[ 6] =    5.25227;  dlf[ 6]=   0.38138;
 alf[ 7] =    5.28416;  dlf[ 7]=   0.31089;
 alf[ 8] =    5.29543;  dlf[ 8]=   0.43524;
 alf[ 9] =    5.30308;  dlf[ 9]=   0.43393;
 alf[10] =    5.34791;  dlf[10]=   0.41095;
 alf[11] =    5.36786;  dlf[11]=   0.47016;
 alf[12] =    5.40054;  dlf[12]=   0.22253;
 alf[13] =    5.42129;  dlf[13]=   0.57649;
 alf[14] =    5.48287;  dlf[14]=   0.62125;
 alf[15] =    5.50021;  dlf[15]=   0.67271;
 alf[16] =    5.51289;  dlf[16]=   0.61582;
 alf[17] =    5.59134;  dlf[17]=   0.74989;
 alf[18] =    5.59179;  dlf[18]=   0.67264;
 alf[19] =    5.59284;  dlf[19]=   0.60328;
 alf[20] =    5.59968;  dlf[20]=   0.55128;
 alf[21] =    5.60536;  dlf[21]=   0.62086;
 alf[22] =    5.61134;  dlf[22]=   0.66708;
 alf[23] =    5.64935;  dlf[23]=   0.67268;
 alf[24] =    5.73760;  dlf[24]=   0.72719;*/





 //Для прямого наведения альфа-дельта
 alf[ 0] =    4.92118;  dlf[ 0]=   0.75128; //  154.78500   84.68907
 alf[ 1] =    5.10891;  dlf[ 1]=   0.20471; //  343.52753   62.58278
 alf[ 2] =    5.12811;  dlf[ 2]=   1.04956; //  191.41113   67.45224
 alf[ 3] =    5.12829;  dlf[ 3]=   0.33985; //  335.50010   69.70953
 alf[ 4] =    5.14194;  dlf[ 4]=   0.15258; //  341.29042   59.23477
 alf[ 5] =    5.14636;  dlf[ 5]=   0.32866; //  333.56970   68.76154
 alf[ 6] =    5.14664;  dlf[ 6]=   0.28836; //  335.67884   66.59558
 alf[ 7] =    5.16454;  dlf[ 7]=   0.38551; //  326.99766   71.32168
 alf[ 8] =    5.18691;  dlf[ 8]=   0.15687; //  336.41898   58.74028
 alf[ 9] =    5.19771;  dlf[ 9]=   0.25071; //  331.30220   63.55963
 alf[10] =    5.22206;  dlf[10]=   0.34595; //  322.43527   67.84439
 alf[11] =    5.29453;  dlf[11]=   0.36964; //  312.13376   66.74799
 alf[12] =    5.35208;  dlf[12]=   0.29277; //  312.67874   61.35751
 alf[13] =    5.37491;  dlf[13]=   0.41762; //  299.73773   65.81440
 alf[14] =    5.38310;  dlf[14]=   0.56929; //  279.48518   70.19768
 alf[15] =    5.39005;  dlf[15]=   0.41651; //  298.58702   65.17757
 alf[16] =    5.41301;  dlf[16]=   0.35436; //  302.54959   61.85231
 alf[17] =    5.42454;  dlf[17]=   0.48874; //  287.92864   66.22674
 alf[18] =    5.42570;  dlf[18]=   0.65423; //  264.18582   69.67624
 alf[19] =    5.56737;  dlf[19]=   0.67480; //  258.44983   63.58517
 alf[20] =    5.59642;  dlf[20]=   0.75326; //  248.23764   62.79524
 alf[21] =    5.61747;  dlf[21]=   0.63350; //  262.34316   60.84673
 alf[22] =    5.63856;  dlf[22]=   0.73225; //  250.53870   60.94167
 alf[23] =    5.65529;  dlf[23]=   0.69051; //  255.13626   59.89354
 alf[24] =    5.70480;  dlf[24]=   0.80893; //  241.55833   58.44631

/*
 // для naved_zvezda
 alf[ 0] =    4.91917;  dlf[ 0]=   0.75097; //  153.89296   84.66543
 alf[ 1] =    5.10601;  dlf[ 1]=   0.20402; //  343.89070   62.58101
 alf[ 2] =    5.12701;  dlf[ 2]=   1.04908; //  191.35507   67.48766
 alf[ 3] =    5.12557;  dlf[ 3]=   0.33918; //  335.93656   69.72392
 alf[ 4] =    5.13899;  dlf[ 4]=   0.15184; //  341.62820   59.23580
 alf[ 5] =    5.14362;  dlf[ 5]=   0.32797; //  333.99127   68.77897
 alf[ 6] =    5.14385;  dlf[ 6]=   0.28766; //  336.07645   66.60874
 alf[ 7] =    5.16188;  dlf[ 7]=   0.38481; //  327.44733   71.35094
 alf[ 8] =    5.18397;  dlf[ 8]=   0.15607; //  336.75154   58.74958
 alf[ 9] =    5.19488;  dlf[ 9]=   0.24994; //  331.66661   63.57932
 alf[10] =    5.21935;  dlf[10]=   0.34517; //  322.82976   67.88006
 alf[11] =    5.29184;  dlf[11]=   0.36879; //  312.49547   66.79906
 alf[12] =    5.34931;  dlf[12]=   0.29183; //  313.00030   61.40598
 alf[13] =    5.37228;  dlf[13]=   0.41670; //  300.05900   65.88161
 alf[14] =    5.38066;  dlf[14]=   0.56841; //  279.76067   70.28551
 alf[15] =    5.38742;  dlf[15]=   0.41557; //  298.90057   65.24594
 alf[16] =    5.41030;  dlf[16]=   0.35337; //  302.85301   61.91471
 alf[17] =    5.42199;  dlf[17]=   0.48779; //  288.21485   66.30657
 alf[18] =    5.42338;  dlf[18]=   0.65332; //  264.38745   69.77081
 alf[19] =    5.56503;  dlf[19]=   0.67377; //  258.62217   63.67844
 alf[20] =    5.59418;  dlf[20]=   0.75223; //  248.37448   62.88709
 alf[21] =    5.61504;  dlf[21]=   0.63241; //  262.52735   60.93856
 alf[22] =    5.63626;  dlf[22]=   0.73117; //  250.68510   61.03335
 alf[23] =    5.65292;  dlf[23]=   0.68941; //  255.29763   59.98540
 alf[24] =    5.70257;  dlf[24]=   0.80782; //  241.68102   58.53405
*/

for(i=1;i<=MAXTK;i++)
 if (RABTK[i]) {
     char s[20], *mode;
     sprintf(s,"tmpdan/pasp%2d%d%d",TIPST,NPORST,i);
     if (n1==1&&n2==25) mode="wt"; else mode="at";
     if ((f[i]=myfopen(s,mode))==NULL) return;
   }

 for(n=n1;n<=n2;n++) {
   double A=acd[n-1], L=lcd[n-1], R=rcd[n-1];
   double Az,H;
   printf("шаг %d\n",n);
   delay_sec(3);     //10
   if(ntk==2&&TIPST==59){   alfa=alf[n-1]; delta = dlf[n-1];
       printf("\n          alfa:%7.3f  delta:%7.3f",alfa,delta);
    //   J2000Teksea(rvtime, &alfa, &delta);
      /*adAHsea(alfa,delta,rvtime,&Az,&H);
       printf("\n Az=%6.2f H=%6.2f\n", Az*RG, H*RG);
       if (H<0) { printf("\n     Под горизонтом\n"); continue;}//goto step1; }
       if (H<5*GR) { printf("\n       Низко над горизонтом\n");continue;}// goto step1; }*/
         res = naved_zvezda(ntk, alfa, delta);
       if (res==0) continue;//return 0;
       printf("\n Выполнено наведение  на звезду шаг %d  Запуск ОМР\n",n); }
   else
         if (!PUSKCHV(A,L,R)) break;


   delay_sec(3);     //пауза 2 сек
   
    /*UAK 27.08.13*/
//   if (!IZMERAPOI(RABTK)) break;
   int res;
   res=IZMERAPOI(RABTK);
   if (!res) break;      //!IZMERAPOI
   for(i=1;i<=MAXTK;i++)
   {
     if (RABTK[i]) 
     {
        if ((TIPST==58)&&(KZVAPOI[i]<=100))
        {
          printf("\n Плохое измерение АПОИ %d звезд в %d камере. Повторим измерение АПОИ.\n",KZVAPOI[i],i);
          delay_sec(5);
          res=IZMERAPOI(RABTK);
          break;
        }
     }
   }
   if (!res) break;      //!IZMERAPOI

   for(i=1;i<=MAXTK;i++)
     if (RABTK[i]) {
        MetodOMR(i);
        printOMR(i);
        printforpasp(i,n,f[i]);
     }
 }

 for(i=1;i<=MAXTK;i++)
   if (RABTK[i])
     fclose(f[i]);
}



//---------------------------------------------------------------------------//
//--------------- наблюдение площадки возле северного полюса мира------------//
//---------------------------------------------------------------------------//

struct SPIS_ZV {
  int num;
  double a,d, mR,mB,mV;
  double X,Y,E;
};


int read_spis_zv(char *fname, SPIS_ZV *mzv)
{
  FILE *f;
  char str[80];
  int ah,am; double as; int dg,dm; double ds; double R,B; int num;
  int res, i=0;

  if ((f=fopen(fname,"rt"))==NULL) {
    printf("Cannot open %s\n", fname); return 0;
  }

  while(fgets(str,80,f)!=NULL) {
    res=sscanf(str,"%d%d%lf%d%d%lf%lf%lf%d",
               &ah,&am,&as,&dg,&dm,&ds,&R,&B,&num);
    if (res<7) continue;

    if (R==0 || B==0) continue;
    mzv[i].a  = (ah + am/60. + as/3600) * HR;
    mzv[i].d  = (dg + dm/60. + ds/3600) * GR;
    mzv[i].mR = R;
    mzv[i].mB = B;
    mzv[i].mV = 0.663*R + 0.337*B + 0.027;
    mzv[i].num = num;
    i++;
  }
  fclose(f);
  return i;
}

void upor_spis_zv(int kzv, SPIS_ZV *z)
{
  int i,j;
  SPIS_ZV tmp;
  for(i=1;i<kzv;i++)
    for(j=i-1;j>=0;j--)
      if (z[j].mV>z[j+1].mV) { tmp=z[j]; z[j]=z[j+1]; z[j+1]=tmp; }
}


int nabl_polus(int ntk)
{
  OMRDATA &o = omrdata[ntk];
  SPIS_ZV *mzv;
  int kzv, rkzv, i, j;
  double Vmax;
  double t, Az, H, A,L,R;
  VECT3 VO,VX;
  int maskaTK[5];
  double 
    alfa_center  = ( 6 + 58./60 + 25./3600) * HR,
    delta_center = (89 + 30./60 + 44./3600) * GR,
    alfa_right   = ( 4 + 44./60 + 25./3600) * HR,
    delta_right  = (89 + 18./60 + 10./3600) * GR;

//#avm# 07/07/2013
if(TIPST==59 && ntk==1)  //SHPK
{
     can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('E','T'), 2000);
     usleep(200);
     can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('B','X'), 1);
     usleep(200);
     can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('B','Y'), 1);
     usleep(200);
     can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('T','R'), 0);
     usleep(200);
     can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('K','A'),3846 ); //analog amplifier
     usleep(2000000);
}
else if(TIPST==59 && ntk==2)  //BIK
{
    can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('E','T'), 400);
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('B','X'), 1);
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('B','Y'), 1);
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('T','R'), 2);
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('K','A'),3846 ); //analog amplifier 15 db
    usleep(200);
    can_req2(fcan_ctk,CAN_ABN_CTK(ntk),CAN_CMD_EXTRA,0x8011); // EM on
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(ntk), CAN_CMD_PUTP,  m2b('E','M'),10240 ); //EM amplifier 40db

    usleep(100000);
}
//#avm# 07/07/2013

  mzv = new SPIS_ZV[400];
  kzv = read_spis_zv("dan/np.txt", mzv);
  if (kzv==0) return 1;

  //printf("kzv=%d\n",kzv);

  upor_spis_zv(kzv, mzv);
 
  t=rvtime;
  KatVidsea(0, &alfa_center, &delta_center);
  adAHsea(alfa_center, delta_center, t, &Az, &H);
  PlusRefraction(&H);
  VO = SferVect(Az,H);
  KatVidsea(0, &alfa_right, &delta_right);
  adAHsea(alfa_right, delta_right, t, &Az, &H);
  PlusRefraction(&H);
  VX = SferVect(Az,H);
  VX -= VO;

  NAVED(ntk, VO,VX, 0,0, &A,&L,&R);

//  int h,m; double s;
//  hourhms(t/3600+12-TZ, &h,&m,&s);
//  printf("%02d:%02d:%05.2f\n",h,m,s);
// printf("t=%f A=%f L=%f R=%f\n",t,A,L,R);
 printf("\n PRONIC:ntk=%d A=%f L=%f R=%f\n",ntk,A,L,R);

  perebroscsn(A,L,R);
  if (!RESCSN()) return 0;  

  memset(maskaTK, 0, sizeof(maskaTK));
  maskaTK[ntk]=1;
  delay_sec(2);//10.06.13
  if (!IZMERAPOI(maskaTK)) return 0;

  if (KZVAPOI[ntk] == 0) {
    printf("Звезды не обнаружены\n"); 
    return 1;
  }

  MetodOMR(ntk);

  if (o.omrkzv < 3) {
// /( /<5
     printf("Отождествлено звезд: %d  Отказ ОМР\n", o.omrkzv);
     return 1;
  }

  for(i=0; i<kzv; i++) {
    double a,d,Az,H,x,y;
    a = mzv[i].a;
    d = mzv[i].d;
    KatVidsea(0, &a, &d);
    adAHsea(a,d, o.time, &Az,&H);
    PlusRefraction(&H);
    GorTV(o.otkpos, Az,H, &x,&y);
	inverse_korrxyOMR(ntk,&x,&y);
    mzv[i].X = x;
    mzv[i].Y = y;
  }


  printf("\nИзмерение звезд в полярной области\n\n");

  printf(" Nзв     X      Y      mR    mB    mV    обн      V\n");
//        ****  ****.* ****.*  **.** **.** **.**   sss  ******* 

  rkzv=0;
  Vmax=0;
  for(i=0; i<kzv; i++) {
    double dx, dy, V, xpix, ypix;
    int obn;

    if (fabs(mzv[i].X) < TKsizeXmm(ntk)/2 && 
		fabs(mzv[i].Y) < TKsizeYmm(ntk)/2) {
      obn=0;
      for(j=0; j<o.izmkzv; j++) {
        IZMZV *zv=&o.izmzv[j];
		dx = zv->x - mzv[i].X;
		dy = zv->y - mzv[i].Y;
		V = zv->e;
        if (fabs(dx)<0.1 && fabs(dy)<0.1) { 
			obn=1; break; 
		}
      }

	  coord_mm2pix(ntk, mzv[i].X, mzv[i].Y, &xpix, &ypix);
      printf("%4d  %6.1f %6.1f  %5.2f %5.2f %5.2f   %3s",
            mzv[i].num, xpix, ypix, 
            mzv[i].mR, mzv[i].mB, mzv[i].mV, 
            (obn==1? "да":"нет") );

      if (obn) printf("  %7.0f", V);
      if (obn) printf("  [dx=%4.1f dy=%4.1f]", dx*MM2PIX(ntk), dy*MM2PIX(ntk));
      
	  printf("\n");

      
#ifdef BL051
      if (obn && mzv[i].mB>Vmax) Vmax=mzv[i].mB;// krikunov
#else 
     if (obn && mzv[i].mV>Vmax) Vmax=mzv[i].mV;
#endif
      if (obn) rkzv++;
    }
  }
//#ifdef BL051
 printf("\n        Среди распознанных звезд максимальная  зв.величина:%7.2f",Vmax);
//#endif
  printf("\nИзмерено и распознано звезд: %d\n",rkzv);

  delete mzv;

  return 1;
}
