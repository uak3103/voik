double pasport[5][25];

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


void coord_pix2mm(int ntk, double Xpix, double Ypix, double *Xmm, double *Ymm)
{
  *Xmm = (Xpix*pasbin(ntk)-pasNX(ntk)/2) * pasSX(ntk);
  *Ymm = (Ypix*pasbin(ntk)-pasNY(ntk)/2) * pasSY(ntk);
}

void coord_mm2pix(int ntk, double Xmm, double Ymm, double *Xpix, double *Ypix)
{
  *Xpix = (Xmm/pasSX(ntk)+pasNX(ntk)/2) / pasbin(ntk);
  *Ypix = (Ymm/pasSY(ntk)+pasNY(ntk)/2) / pasbin(ntk);
}


int default_paramCTK(int ntk)
{
  switch ( (int)pastipctk(ntk) ) {
    case CTK_BIK:
      pasSX(ntk) = pasSY(ntk) = 0.013;
      pasNX(ntk) = 1024;
      pasNY(ntk) = 1024;
      break;

    case CTK_LIK:
      pasSX(ntk) = pasSY(ntk) = 0.013;
      pasNX(ntk) = 4096;
      pasNY(ntk) = 96;
      break;

    case CTK_BPK:
      pasSX(ntk) = pasSY(ntk) = 0.015;
      pasNX(ntk) = 2048;
      pasNY(ntk) = 1024;
      break;

    case CTK_SHPK:
      pasSX(ntk) = pasSY(ntk) = 0.015;
      pasNX(ntk) = 4096;
      pasNY(ntk) = 2048;
      break;
  }
}


void default_pasp (int tipst)
{
  int ntk;

  switch(tipst) {
    case 56: MAXTK=4; break;
    case 57: MAXTK=4; break;      
    case 58: MAXTK=2; break;    
    case 59: MAXTK=2; break;
    case 60: MAXTK=1; break;
  }

  for(ntk=1; ntk<=MAXTK; ntk++) {
    memset(pasport[ntk], 0, 25*sizeof(double));
    pasZDL(ntk) = pasZDR(ntk) = 90.;
    pasMASH(ntk) = 1.;
    pasflipX(ntk) = 1.;
    pasbin(ntk) = 1.;
  }

  switch(tipst) {
    case 56:
      for(ntk=1; ntk<=4; ntk++) {
        pasF(ntk) = 228;
        pasrazv(ntk) = -90.;
        pastipctk(ntk) = CTK_BPK;
      }
      pasZDR(3) = 90 - 11.25;
      pasZDR(1) = 90 -  3.75;
      pasZDR(4) = 90 +  3.75;
      pasZDR(2) = 90 + 11.25;
      break;

    case 57: 
      pasF(1)=pasF(3)=pasF(4)= 3000;
      pasF(2)= 750;
      pasrazv(1)=pasrazv(2)=pasrazv(3)=pasrazv(4)= 180.;
      pastipctk(1) = CTK_BIK;
      pastipctk(2) = CTK_BIK;
      pastipctk(3) = CTK_LIK;
      pastipctk(4) = CTK_LIK;
      break;

    case 58: 
      for(ntk=1; ntk<=4; ntk++) {
        pasF(ntk) = 1333.;
        pastipctk(ntk) = CTK_SHPK;
        pasrazv(ntk) = 90.;
      }
      paskolvos(1) =  35./60.;  
      paskolvos(2) = -35./60.;
      break;

    case 59: 
      pasF(1) = pasF(2) = 2750;  
      pastipctk(1) = CTK_SHPK;
      pastipctk(2) = CTK_BIK;
      break;

    case 60: 
      pasF(1) = 2750;
      pastipctk(1) = CTK_SHPK;
      break;
  }

  for(ntk=1; ntk<=MAXTK; ntk++) {
    default_paramCTK(ntk);
  }

  MINCD(OS_A) = -200;
  MAXCD(OS_A) =  200;

  MINCD(OS_L) =   20;
  MAXCD(OS_L) =  160;

  MINCD(OS_R) =   20;
  MAXCD(OS_R) =  160;

  MAXSKOR(OS_A) = 6.0;
  MAXSKOR(OS_L) = 6.0;
  MAXSKOR(OS_R) = 6.0;

}


void strlwr(char *s) 
{
  while(*s) { *s = tolower(*s); s++; }
}


int read_config_line(char *line, char *key, int *param, char *value)
{
  char *s, *s1;

  s=strstr(line,"//"); //обрубание комментариев после '//'
  if (s!=NULL) *s=0;

  s=strchr(line,'#'); //обрубание комментариев после '#'
  if (s!=NULL) *s=0;

  s=line;
  while(*s==' ' || *s=='\t') s++; //пропуск передних пробелов

  if (*s=='\n' || *s=='\r' || *s==0) return 0; //в строке ничего нет

  if (sscanf(s,"%[a-zA-Z]",key)!=1) return -1; //выделили key

  *param = -1;
  if ((s1=strchr(s,'('))!=NULL) {  // нашли знак '('
    s1++;
    if (sscanf(s1,"%d",param)!=1) return -1; //выделили param
  }

  if ((s1=strchr(s,'='))==NULL) return -1;   // нашли знак '='
  s1++;
  if (sscanf(s1,"%s",value)!=1) return -1; //выделили value

  return 1;
}


int read_pasp_file(char* fname)
{
  FILE *f;
  char str[200];
  int numstr=0;
  char key[200], value[200];
  int param;
  double valuef;
  int valuei;
  int resf, resi, res, ntk1, ntk2, n;

  if ((f=fopen(fname,"rt"))==NULL) return 0;

  while (fgets(str,200,f)!=NULL) {

    numstr++;

    res = read_config_line(str, key, &param, value);
    if (res==0) continue;
    if (res==-1) goto err;
    
    if (param==-1) { 
      ntk1=1; ntk2=MAXTK; 
    } else if (param>=1 && param<=MAXTK) {
      ntk1=ntk2=param;
    } else
      goto err;

    
    resf = sscanf(value, "%lf", &valuef);
    resi = sscanf(value, "%d", &valuei);

    if (strcasecmp(key,"MAXTK")==0) {
      if (resi==0) goto err;
      if (valuei<1 || valuei>4) goto err;
      MAXTK = valuei;
 
    } else if (strcasecmp(key,"F")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasF(n) = valuef;
 
    } else if (strcasecmp(key,"A")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasZDA(n) = valuef;

    } else if (strcasecmp(key,"L")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasZDL(n) = valuef;

    } else if (strcasecmp(key,"R")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasZDR(n) = valuef;

    } else if (strcasecmp(key,"oshA")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasohZDA(n) = valuef/3600;

    } else if (strcasecmp(key,"oshL")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasohZDL(n) = valuef/3600;

    } else if (strcasecmp(key,"oshR")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasohZDR(n) = valuef/3600;

    } else if (strcasecmp(key,"kolvos")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) paskolvos(n) = valuef/3600;

    } else if (strcasecmp(key,"KRL")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasKRL(n) = valuef/3600;

    } else if (strcasecmp(key,"KLA")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasKLA(n) = valuef/3600;

    } else if (strcasecmp(key,"AotC")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasAotC(n) = valuef/3600;

    } else if (strcasecmp(key,"AotZ")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasAotZ(n) = valuef/3600;

    } else if (strcasecmp(key,"defX")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasdefX(n) = valuef/3600;

    } else if (strcasecmp(key,"defY")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasdefY(n) = valuef/3600;

    } else if (strcasecmp(key,"MASH")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasMASH(n) = valuef;

    } else if (strcasecmp(key,"razv")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasrazv(n) = valuef; // /3600;

    } else if (strcasecmp(key,"flipX")==0) {
      if (resi==0) goto err;
      if (valuei!=0 && valuei!=1) goto err;
      for(n=ntk1;n<=ntk2;n++) pasflipX(n) = valuei;

    } else if (strcasecmp(key,"flipY")==0) {
      if (resi==0) goto err;
      if (valuei!=0 && valuei!=1) goto err;
      for(n=ntk1;n<=ntk2;n++) pasflipY(n) = valuei;

    } else if (strcasecmp(key,"tipctk")==0) {
        int tip;

	if (strcasecmp(value,"BIK")==0)       tip = CTK_BIK;
	else if (strcasecmp(value,"LIK")==0)  tip = CTK_LIK;
	else if (strcasecmp(value,"BPK")==0)  tip = CTK_BPK;
	else if (strcasecmp(value,"SHPK")==0) tip = CTK_SHPK;
        else goto err;

	for(n=ntk1;n<=ntk2;n++) {
          pastipctk(n) = tip;
          default_paramCTK(n);
        }

    } else if (strcasecmp(key,"SX")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasSX(n) = valuef;

    } else if (strcasecmp(key,"SY")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasSY(n) = valuef;

    } else if (strcasecmp(key,"NX")==0) {
      if (resi==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasNX(n) = valuei;

    } else if (strcasecmp(key,"NY")==0) {
      if (resi==0) goto err;
      for(n=ntk1;n<=ntk2;n++) pasNY(n) = valuei;

    } else if (strcasecmp(key,"minA")==0) {
      if (resf==0) goto err;
      MINCD(OS_A) = valuef;

    } else if (strcasecmp(key,"maxA")==0) {
      if (resf==0) goto err;
      MAXCD(OS_A) = valuef;

    } else if (strcasecmp(key,"minL")==0) {
      if (resf==0) goto err;
      MINCD(OS_L) = valuef;

    } else if (strcasecmp(key,"maxL")==0) {
      if (resf==0) goto err;
      MAXCD(OS_L) = valuef;

    } else if (strcasecmp(key,"minR")==0) {
      if (resf==0) goto err;
      MINCD(OS_R) = valuef;

    } else if (strcasecmp(key,"maxR")==0) {
      if (resf==0) goto err;
      MAXCD(OS_R) = valuef;

    } else if (strcasecmp(key,"maxskorA")==0) {
      if (resf==0) goto err;
      MAXSKOR(OS_A) = valuef;

    } else if (strcasecmp(key,"maxskorL")==0) {
      if (resf==0) goto err;
      MAXSKOR(OS_L) = valuef;

    } else if (strcasecmp(key,"maxskorR")==0) {
      if (resf==0) goto err;
      MAXSKOR(OS_R) = valuef;

    } else if (strcasecmp(key,"ncanvu")==0) {
      if (resi==0) goto err;
      if (valuei<0 || valuei>1) goto err;
      ncan_vu = valuei;

    } else if (strcasecmp(key,"ncankdpo")==0) {
      if (resi==0) goto err;
      if (valuei<0 || valuei>1) goto err;
      ncan_kdpo = valuei;

    } else if (strcasecmp(key,"ncankep")==0) {
      if (resi==0) goto err;
      if (valuei<0 || valuei>1) goto err;
      ncan_kep = valuei;

    } else if (strcasecmp(key,"ncanctk")==0) {
      if (resi==0) goto err;
      if (valuei<0 || valuei>1) goto err;
      ncan_ctk = valuei;

    } else if (strcasecmp(key,"apoiIP")==0) {
      if (strlen(value)>15) goto err;
      strcpy(APOI_ADDR,value);

    } else goto err;

  }

  fclose(f);
  return 1;

err:
  printf("Ошибка в строке %d файла %s\n", numstr, fname);
  printf("%s\n", str);
  fclose(f);
  return -1;
}



int init_pasp (int tipst, int nporst)
{
  char fname[20];
  int res;

  default_pasp(tipst);

  sprintf(fname, "dan/pasp%2d%d", tipst, nporst);
  res =read_pasp_file(fname);
      
  if (res==0) {
      printf("Не найден файл паспорта %s\n",fname);
      printf("Будет использоваться номинальный паспорт\n");
  }
  if (res==-1) {
    return 0;
  }

  if (TIPST==56 || TIPST==58) {
    int i,ntk; 
    for(i=0;i<25;i++) {
      double p = 0;
      for(ntk=1; ntk<=MAXTK; ntk++) p += pasport[ntk][i];
      p /= MAXTK;
      pasport[0][i] = p;
    }
  }

  return 1;
}

