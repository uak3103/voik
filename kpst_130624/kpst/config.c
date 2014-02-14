int read_config_line(char *line, char *key, int *param, char *value);


int readCONFIG(void)
{
//  char *fname="stan.ini";
  char fname[80];
  char ss[80];
  FILE *f;
  char str[200];
  int numstr=0;
  char key[200], value[200];
  int param;
  double valuef;
  int valuei;
  int resf, resi, res, ntk1, ntk2, n;


  for(n=1; n<=4; n++) 
    RABTK[n]=1;

  strcpy(fname,"stan.ini");
  strcpy(ss,kpstpath);
  strcat(ss,fname);
  strcpy(fname,ss);
//  printf("%s\n",fname);
  if ((f=myfopen(fname,"rt"))==NULL) return 0;

  while (fgets(str,200,f)!=NULL) { // прочитали строку файла

    numstr++;

    res=read_config_line(str, key, &param, value);
    if (res==0) continue;
    if (res==-1) goto err;

    if (param==-1) { 
      ntk1=1; ntk2=4; 
    } else if (param>=1 && param<=4) {
      ntk1=ntk2=param;
    } else
      goto err;

    resf = sscanf(value, "%lf", &valuef);
    resi = sscanf(value, "%d", &valuei);

    if (strcasecmp(key,"st")==0) {
      if (resi==0) goto err;
      n = valuei;
      if (n<561||n>602||(n%10)>2) goto err;
      TIPST=n/10;
      NPORST=n%10;
      switch(TIPST) {
        case 56: MAXTK=4; break;
        case 57: MAXTK=4; break;
        case 58: MAXTK=2; break;
        case 59: MAXTK=2; break;
        case 60: MAXTK=1; break;
      }
      //printf("TIPST = %d  NPORST = %d\n",TIPST, NPORST);
 
    } else if (strcasecmp(key,"RABTK")==0) {
      if (resi==0) goto err;
      if (param==-1) goto err;
      if (valuei<0 || valuei>1) goto err;
      RABTK[param] = valuei;
 
    } else if (strcasecmp(key,"imit")==0) {
      if (resi==0) goto err;
      if (valuei<0 || valuei>1) goto err;
      IMIT = IMITVU = IMITOS[OS_A] = IMITOS[OS_L] = IMITOS[OS_R] =
      IMITAPOI = IMITCTK = valuei;
 
    } else if (strcasecmp(key,"imitvu")==0) {
      if (resi==0) goto err;
      if (valuei<0 || valuei>1) goto err;
      IMITVU = valuei;
 
    } else if (strcasecmp(key,"imitcsn")==0) {
      if (resi==0) goto err;
      if (valuei<0 || valuei>1) goto err;
      IMITOS[OS_A] = IMITOS[OS_L] = IMITOS[OS_R] = valuei;
 
    } else if (strcasecmp(key,"imitosa")==0) {
      if (resi==0) goto err;
      if (valuei<0 || valuei>1) goto err;
      IMITOS[OS_A] = valuei;
 
    } else if (strcasecmp(key,"imitosl")==0) {
      if (resi==0) goto err;
      if (valuei<0 || valuei>1) goto err;
      IMITOS[OS_L] = valuei;
 
    } else if (strcasecmp(key,"imitosr")==0) {
      if (resi==0) goto err;
      if (valuei<0 || valuei>1) goto err;
      IMITOS[OS_R] = valuei;
 
    } else if (strcasecmp(key,"imitapoi")==0) {
      if (resi==0) goto err;
      if (valuei<0 || valuei>1) goto err;
      IMITAPOI = valuei;
 
    } else if (strcasecmp(key,"imitctk")==0) {
      if (resi==0) goto err;
      if (valuei<0 || valuei>1) goto err;
      IMITCTK = valuei;
 
    } else if (strcasecmp(key,"kompl")==0) {
      if (resi==0) goto err;
      num_komplex = valuei;
 
    } else if (strcasecmp(key,"zkt")==0) {
      strcpy(zktpath, value);
 
    } else if (strcasecmp(key,"dumpizmer")==0) {
      if (resi==0) goto err;
      if (valuei<0 || valuei>1) goto err;
      //dumpizmer = resi;
       dumpizmer = valuei;//29.05.2013 rkikunov
    } else if (strcasecmp(key,"Nkadr")==0) {
      if (resi==0) goto err;
      for(n=ntk1;n<=ntk2;n++)
         NkadrAPOI[n] = valuei;
      printf("\n Число кадров ЦТК:%d\n",  valuei);
    } else if (strcasecmp(key,"TexpCTK")==0) {
      if (resf==0) goto err;
      for(n=ntk1;n<=ntk2;n++) {
         TexpCTK[n] = valuef;
         TexpCTK_set[n] = 1;

      }
      printf("\n Экспозиция: %f",valuef);
    } else if (strcasecmp(key,"binCTK")==0) {
      if (resi==0) goto err;
      if (valuei!=1 && valuei!=2 && valuei!=4 && valuei!=8) goto err;
      for(n=ntk1;n<=ntk2;n++) {
         binCTK[n] = valuei;
         binCTK_set[n] = 1;

      }
            printf("\n Коэфициент бинирования:%d",binCTK[1]);
    } else if (strcasecmp(key,"rateCTK")==0) {
      int v;
      if (strcasecmp(value,"low")==0)         {v=CTK_RATE_LOW;printf("\n rateCTK: LOW");}
      else if (strcasecmp(value,"medium")==0){ v=CTK_RATE_MEDIUM;printf("\n rateCTK: MEDIUM");}
      else if (strcasecmp(value,"high")==0)   {v=CTK_RATE_HIGH;printf("\n rateCTK: HIGH");}
      else goto err;
      for(n=ntk1;n<=ntk2;n++) {
         rateCTK[n] = v;
         rateCTK_set[n] = 1;
      }
 
    } else goto err;

  }

  if (TIPST==0) {
    printf("Не указан тип станции в файле %s\n", fname);
    fclose(f);
    return 0;
  } 

  NUMRABTK=0;
  for(n=1; n<=MAXTK; n++) 
    NUMRABTK+=RABTK[n];

  fclose(f);
  return 1;

err:
  printf("Ошибка в строке %d файла %s\n", numstr, fname);
  printf("%s\n", str);
  fclose(f);
  return 0;
}
