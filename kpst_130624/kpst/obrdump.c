struct DUMPIZM {
	int tipst, nporst, ntk;
	double jd, time;
	double pasp[25];
	double A,L,R,VA,VL,VR;
	int Nobj,Nzv,Nko;
	nObj *mobj;
};

int read_dumpizm(FILE *f, DUMPIZM &dump)
{
	char str[300];
	int n;
	int Y,M,D, h,m; double s;
	double p[25];
	while(1) {
		if (fgets(str,300,f)==NULL) return -1;
		if (memcmp(str,"ST ",3)==0) break;
	}

	n = sscanf(str,"ST %d-%d TK %d", &dump.tipst, &dump.nporst, &dump.ntk);
	if (n<3) return 0;

	if (fgets(str,300,f)==NULL) return 0;
	n = sscanf(str,"%d-%d-%d %d:%d:%lf", &Y,&M,&D, &h,&m,&s);
	if (n<6) return 0;
	dump.jd = JulianDate(D,M,Y);
	dump.time = h*3600+m*60+s;
	if (dump.time<43200) { 
	  dump.jd--; dump.time+=43200;
	} else { 
	  dump.time-=43200;
	}

	if (fgets(str,300,f)==NULL) return 0;
	n = sscanf(str,"PASP %lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf"
		             "%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",
        &p[ 0],&p[ 1],&p[ 2],&p[ 3],&p[ 4],&p[ 5],&p[ 6],&p[ 7],&p[ 8],&p[ 9],
	    &p[10],&p[11],&p[12],&p[13],&p[14],&p[15],&p[16],&p[17],&p[18],&p[19],
	    &p[20],&p[21],&p[22],&p[23],&p[24]);
	if (n<25) return 0;
	memcpy(dump.pasp, p, 25*sizeof(double));

	if (fgets(str,300,f)==NULL) return 0;
	n = sscanf(str,"A=%lf L=%lf R=%lf VA=%lf VL=%lf VR=%lf",
		&dump.A, &dump.L, &dump.R, &dump.VA, &dump.VL, &dump.VR);
	if (n<6) return 0;

	if (fgets(str,300,f)==NULL) return 0;
	n = sscanf(str,"Nobj=%d Nzv=%d Nko=%d",
		&dump.Nobj, &dump.Nzv, &dump.Nko);
	if (n<3) return 0;

	dump.mobj = new nObj[dump.Nobj];

	for(int i=0; i<dump.Nobj; i++) {
	  nObj o;
	  int nn;
	  if (fgets(str,300,f)==NULL) return 0;
  	  n = sscanf(str,"N=%d T=%lf X=%f Y=%f VX=%f VY=%f S=%f V=%f KO=%d",
		  &nn, &o.T, &o.X, &o.Y, &o.DX, &o.DY, &o.S, &o.V, &o.KO);
	  if (n<9) return 0;
	  dump.mobj[i]=o;
		
	}

	return 1;
}



int MetodOMR_fordump(int ntk)
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

//  o.A= cd[OS_A] + (o.time-timecd[OS_A]) * skor[OS_A];
//  o.L= cd[OS_L] + (o.time-timecd[OS_L]) * skor[OS_L];
//  o.R= cd[OS_R] + (o.time-timecd[OS_R]) * skor[OS_R];
  getOTKpos(ntk,o.A,o.L,o.R,o.otkpos);

  get_catzv(ntk,-sx,sx,-sy,sy,limitmag);
  if (o.catkzv==0) return 0;

  clear_redomr(o.red);
  Otogdestv(o);

//  if (kzvapoi>=30 && o.catkzv>=5 && o.omrkzv<=2) oshibkiOMR[NTK]++;
//  else oshibkiOMR[NTK]=0;

  return o.omrkzv;
}


int obrdump(char *filename)
{
  //UPRST &u=uprst;
  int ntk, D,M,Y;

  FILE *f;
  DUMPIZM di;

  f=fopen(filename,"rt");
  if (f==NULL) {
    printf("File %s not open\n",filename);
    return 0;
  }

  for(int n=0;;n++) {
    int res = read_dumpizm(f, di);
    if (res==-1) break;
    if (res==0) continue;

    printf("dumpizm number %d\n", n);
    printf("Nobj=%d Nzv=%d Nko=%d\n", di.Nobj, di.Nzv, di.Nko);

    ntk = di.ntk;
    OMRDATA &o = omrdata[ntk];

    JDtoDMY(di.jd, &D,&M,&Y);
    
    if (di.tipst != TIPST || di.nporst != NPORST) {
      printf("Не тот номер станции!\n");
      continue;
    }
    if (n==0) initglobst(TIPST, NPORST, D, M, Y, 24);

    //memcpy(pasport[ntk], di.pasp, 25*sizeof(double));
    pasport[ntk][23] = di.pasp[23]; //bin

    KolobjAPOI[ntk] = di.Nobj;
    memcpy(ObjAPOI[ntk],di.mobj, di.Nobj*sizeof(nObj));

    o.A= di.A;
    o.L= di.L;
    o.R= di.R;
    o.time = di.time;
    getOTKpos(ntk,o.A,o.L,o.R,o.otkpos);	

    MetodOMR_fordump(ntk);

    IZMERKO(RABTK);
    OBRKO(RABTK,1);
  }

  return 1;
}
