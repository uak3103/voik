struct TEX_INFO
{
        u_int32_t 	nID;
        u_char 		btCamNum;
        u_char		btCamType;
        u_char		pbtMAC[6];
        u_int32_t	nVideoOffset;
        u_int32_t 	nCadrNum;
        u_int16_t	nReserv;
        u_char		btExpYear;
        u_char		btExpMonth;
        u_char 		btExpDay;
        u_char		btExpHour;
        u_char		btExpMinutes;
        u_char		btExpSeconds;
        u_int32_t	nExpMicroseconds;
        u_int32_t	nExposition;
        u_int16_t	wOffsetX;
        u_int16_t	wOffsetY;
        u_int16_t	wSizeX;
        u_int16_t	wSizeY;
        u_char		btBinX;
        u_char		btBinY;
        u_int16_t	wConvolutionsCount;
        u_int16_t	wMinBright;
        u_int16_t	wMaxBright;

        union
        {
                struct
                {
                        u_char	btReserv1	:5;
                        u_char	btStabBlackLevel	:1;
                        u_char	btExcludeDarkCurrent	:1;
                        u_char	btCorrSens	:1;
                }ProcessType;
                u_char btProcessType;
        };

        union
        {
                struct
                {
                        u_char	btAutoLevel	:1;
                        u_char	btReserv2	:6;
                        u_char	btFindConvolution	:1;
                }ThreshType;
                u_char	btThreshType;
        };

        u_int16_t	wThresh;
        int16_t		nTempCurrent;
        int16_t		nTempSet;
        u_int16_t	wAnalogAmpl;
        u_int16_t	wEmAmpl;
        u_int16_t	wReadFreq;
        u_int16_t	wBlackLevel;
        u_char		btMatrixOutputNum;
        u_char 		btSpeedTransfer;
        u_char		btStepIntegration;
        u_char		pbtReserv1[57];

        u_int32_t       iDatchA;
        u_int32_t       iDatchL;
        u_int32_t       iDatchR;
        u_char		pbtReserv2[100];
};

struct CONVOLUTION
{
        u_int32_t	nID;           // "CONV"
        u_int16_t	wNum;
        u_int16_t	wReserv;
        u_int32_t 	nX;
        u_int32_t	nY;
        u_int32_t	nV;
        u_int32_t	nS;
        u_int32_t	nS_XX;
        u_int32_t	nS_XY;
        u_int32_t	nS_YY;
        u_char		pbtReserv[28];
};

bool ReadKadr()
{
    int fff;
    char sFile[50]="/tmp/testVOIK.dat";
    char *m_temp,*m_kadr,*m11;
    int VideoOffset;
    TEX_INFO *tex_inf;
    char *s;
    int i,ntk,maska[5],kko=0,kzv=0;
    CONVOLUTION conv;
    CONVOLUTION *cnv=&conv;
    nObj oobj;
    nObj *pobj=&oobj;
    double A,T;
    UPRST &u=uprst;
    
    m_temp=(char*)malloc(1024);
    fff=open(sFile,O_RDONLY);
    if (fff<0)
    {
	printf("Error read file /tmp/testVOIK.dat\n");
	return false;
    }
    read(fff,m_temp,1024);
    tex_inf=(TEX_INFO*)(m_temp+16);
    VideoOffset=tex_inf->nVideoOffset;
//    printf("kadr%6d Video%6d\n",tex_inf->nCadrNum,tex_inf->nVideoOffset);
    if (VideoOffset!=1024)
    {
	m_kadr=(char*)malloc(VideoOffset);
	memcpy(m_kadr,m_temp,1024);
	free(m_temp);
        read(fff,m_kadr+1024,VideoOffset-1024);
    }
    else
	m_kadr=m_temp;
    
    tex_inf=(TEX_INFO*)(m_kadr+16);
    
    m11=(char*)malloc(tex_inf->wSizeX*tex_inf->wSizeY*sizeof(short));
    read(fff,m11,tex_inf->wSizeX*tex_inf->wSizeY*sizeof(short));
    close(fff);
    
    char *stanc;
    int stani=0;
    stanc=(char*)tex_inf->pbtReserv2;		//ŒœÕ≈“ ”‘¡Œ√……
    stani=atoi(stanc);
    if (stani<561||stani>602||(stani%10)>2)
    {
	if (!readCONFIG()) return false;
    }
    else
    {
	//printf("CTAH=%s (%d)\n",stanc,stani);
	for(i=1;i<=4;i++)
	    RABTK[i]=1;
	TIPST=stani/10;
	NPORST=stani%10;
        switch(TIPST) 
	{
	    case 56: MAXTK=4; break;
	    case 57: MAXTK=4; break;
	    case 58: MAXTK=2; break;
	    case 59: MAXTK=2; break;
	    case 60: MAXTK=1; break;
	}
	NUMRABTK=0;
	for(i=1;i<=MAXTK;i++)
	    NUMRABTK+=RABTK[i];
    }
    IMIT = IMITVU = IMITOS[OS_A] = IMITOS[OS_L] = IMITOS[OS_R] =
    IMITAPOI = IMITCTK = 0;
    num_komplex=2;			//ÓÔÌÂÚ ÎÔÌÏÂÎÛ·
    
    dumpizmer=1;
    //IMITAPOI=1;       //deb
    
    //initrv();   //date-time
    //printf("D=%2d.%2d.%4d JD=%f\n",rvday,rvmonth,rvyear,JDsea);
    
    struct stat fi;
    char *sobj;
    int  count_o=0;
    strcpy(sFile,"/tmp/testVOIK.obj");
    if (stat(sFile,&fi)==0)
    {
	sobj=(char*)malloc(fi.st_size);
	fff=open(sFile,O_RDONLY);
	if (fff<0)
	{
	    printf("Error read file /tmp/testVOIK.obj\n");
	    return false;
	}
	read(fff,sobj,fi.st_size);
	close(fff);
	count_o=fi.st_size/sizeof(nObj);
	//printf("count_o=%d\n",count_o);
    }
    
    rvday  =tex_inf->btExpDay;
    rvmonth=tex_inf->btExpMonth;
    rvyear =tex_inf->btExpYear+2000;
    rvhour =tex_inf->btExpHour;
    rvmin  =tex_inf->btExpMinutes;
    rvsec  =tex_inf->btExpSeconds;
    rvtime=hms_to_rvtime(rvhour,rvmin,rvsec);
    standard_dump_names(rvday,rvmonth,rvyear,rvhour);
//    printf("Date=%d.%d.%d hour=%d %s\n",rvday,rvmonth,rvyear,rvhour,ftextdump);
    if (!open_text_dump()) return 0;
    initsys(0,ftextdump);
    initcsn();
    if (!initglobst(TIPST,NPORST,rvday,rvmonth,rvyear,rvhour)) return false;
//    printf("ST=%d%d\n",TIPST,NPORST);
//    printf("D=%2d.%2d.%4d JD=%lf\n",rvday,rvmonth,rvyear,JDsea);
    
    initinter();
    
    T=tex_inf->btExpHour*3600+tex_inf->btExpMinutes*60+
      tex_inf->btExpSeconds+tex_inf->nExpMicroseconds/1000000.0+tex_inf->nExposition/2000000.0;
      
    if (tex_inf->wConvolutionsCount==0) return false;
    switch (tex_inf->btCamType)
    {
    case 0:s="‚ÈÎ";break;
    case 1:s="˚Î";break;
    case 2:s="ÏÈÎ";break;
    case 3:s="‚Î";break;
    }
    A=(tex_inf->iDatchA%524288)*360./524288.;
    if(A>215) A=A-360;
    cd[OS_A]=A;
    cd[OS_L]=(tex_inf->iDatchL%524288)*360./524288.;
    cd[OS_R]=(tex_inf->iDatchR%524288)*360./524288.;
    timecd[OS_A]=timecd[OS_L]=timecd[OS_R]=rvtime;
    skor[OS_A]=skor[OS_L]=skor[OS_R]=0;
    ntk=tex_inf->btCamNum;		//ŒœÕ≈“ À¡Õ≈“Ÿ
    for (i=1;i<=MAXTK;i++)
	maska[i]=0;
    maska[ntk]=1;
    NUMRABTK=0;
    for (i=1;i<=MAXTK;i++)
    {
	RABTK[i]=maska[i];
	NUMRABTK+=RABTK[i];
    }
    int bin=0;
    switch (tex_inf->btBinX)
    {
     case 0:bin=1;break;
     case 1:bin=2;break;
     case 2:bin=4;break;
     case 3:bin=8;break;
    }
    pasbin(ntk)=bin;
    ObjAPOI[ntk]=(nObj*)malloc(sizeof(nObj)*tex_inf->wConvolutionsCount);
    KOAPOI[ntk]= (KOtype*)malloc(sizeof(KOtype)*tex_inf->wConvolutionsCount);
    ZVAPOI[ntk]= (ZVtype*)malloc(sizeof(ZVtype)*tex_inf->wConvolutionsCount);
    
    printf("kadr%6d %s T=%d:%d:%d:%d A=%8.3f L=%8.3f r=%8.3f TK=%d Bin=%d T=%f RT=%f JD=%f\n",
	tex_inf->nCadrNum,s,
	tex_inf->btExpHour,tex_inf->btExpMinutes,tex_inf->btExpSeconds,tex_inf->nExpMicroseconds,
	cd[OS_A],cd[OS_L],cd[OS_R],ntk,bin,T,rvtime,JDsea);
    
    if (!IMITAPOI)
    {
        double x,y,vx,vy;
        get_VXVY_zv(ntk,cd[OS_A],cd[OS_L],cd[OS_R],skor[OS_A],skor[OS_L],skor[OS_L],&vx,&vy);
	for (i=0;i<tex_inf->wConvolutionsCount;i++)
        {
	    nObj *obj=&ObjAPOI[ntk][i];
	    if (count_o==0)
	    {
	    memcpy(cnv,m_kadr+256+i*sizeof(CONVOLUTION),sizeof(CONVOLUTION));
	    //coord_pix2mm(ntk,cnv->nX,cnv->nY,&x,&y);
	    x=cnv->nX;y=cnv->nY;
	    obj->X=x;
	    obj->Y=y;
	    obj->DX=cnv->wReserv==1?0:vx;
	    obj->DY=cnv->wReserv==1?0:vy;
	    obj->S=cnv->nS;
	    obj->V=cnv->nV;
	    obj->KO=cnv->wReserv==1?1:-1;
	    obj->T=T;
	    if (cnv->wReserv==+1)
	    {
		KOtype *ko=&KOAPOI[ntk][kko];
		if (kko>=MAXKKO) continue;
		coord_pix2mm(ntk,cnv->nX,cnv->nY,&x,&y);
		ko->x=x;
		ko->y=y;
		ko->vx=ko->vy=0;
		ko->e=cnv->nV;
		kko++;
	    }
	    else if (cnv->wReserv==65535)
	    {
		ZVtype *zv=&ZVAPOI[ntk][kzv];
		if (kzv>=MAXKZV) continue;
		coord_pix2mm(ntk,cnv->nX,cnv->nY,&x,&y);
		zv->x=x;
		zv->y=y;
		zv->e=cnv->nV;
		kzv++;
	    }
	    printf("wNum=%3d s=%6d v=%8d x=%4d y=%4d ko=%d\n",cnv->wNum,cnv->nS,cnv->nV,cnv->nX,cnv->nY,cnv->wReserv);
	    }
	    else
	    {
		memcpy(pobj,sobj+i*sizeof(nObj),sizeof(nObj));
		obj->X=pobj->X;
		obj->Y=pobj->Y;
		obj->DX=pobj->DX;  //pobj->KO==1?pobj->DX:vx;
		obj->DY=pobj->DY;  //pobj->KO==1?pobj->DY:vy;
		obj->S=pobj->S;
		obj->V=pobj->V;
		obj->KO=pobj->KO==1?1:-1;
		obj->T=pobj->T;
		if (pobj->KO==+1)
		{
		    KOtype *ko=&KOAPOI[ntk][kko];
		    if (kko>=MAXKKO) continue;
		    coord_pix2mm(ntk,obj->X,obj->Y,&x,&y);
		    ko->x=x;
		    ko->y=y;
		    ko->vx=ko->vy=0;
		    ko->e=cnv->nV;
		    kko++;
		    //?????
		    rvhour =obj->T/3600;
		    rvmin  =(obj->T-rvhour*3600)/60;
		    rvsec  =obj->T-rvhour*3600-rvmin*60;
		    rvtime=hms_to_rvtime(rvhour,rvmin,rvsec);
		}
		else //if (pobj->KO==-1)   ???
		{
		    ZVtype *zv=&ZVAPOI[ntk][kzv];
		    if (kzv>=MAXKZV) continue;
		    coord_pix2mm(ntk,obj->X,obj->Y,&x,&y);
		    zv->x=x;
		    zv->y=y;
		    zv->e=obj->V;
		    kzv++;
		}
		printf("i=%3d s=%6.2f v=%8.2f x=%f y=%f dx=%f dy=%f T=%8.2f ko=%d\n",i,obj->S,obj->V,obj->X,obj->Y,obj->DX,obj->DY,obj->T,obj->KO);
	    }
	}
	KolobjAPOI[ntk]=tex_inf->wConvolutionsCount;    //kko+kzv;
	KKOAPOI[ntk]=kko;
	KZVAPOI[ntk]=kzv;
    
	if (dumpizmer)
	{
	    dump_izmer(ntk);
	}
	printf("KKO=%d KZV=%d\n",kko,kzv);
    }
    else
    {
	IZMERAPOI(RABTK);
    }
    
 printf(
"\n"
"•†◊≈“”…— œ‘ %-8s††††††††††††††††††††††††††††††††††††††††††††††††®\n"
"°      *** ˙¡–’›≈Œ¡ ÀœÕ–Ã≈À”Œ¡— –“œ«“¡ÕÕ¡ ”‘¡Œ√…… %dˆ6-%d ***        °\n"
"°                 ‰¡‘¡: %02d/%02d/%4d  ˜“≈Õ—: %02d:%02d:%02.0f                 °\n",
 myversion_date,
 TIPST,NPORST,rvday,rvmonth,rvyear,rvhour,rvmin,rvsec);
 if (IMIT) printf(
"°                        ÚÂˆÈÌ ÈÌÈÙ·„ÈÈ!                            °\n");
 printf(
"´†††††††††††††††††††††††††††††††††††††††††††††††††††††††††††††††††††Æ\n");
    
    if (kko+kzv>0)     //(KKOAPOI[ntk]>0)
    {
	MetodOMR(ntk);
	printOMR(ntk);
    }
    IZMERKO(RABTK);
    if (u.kolko>0)
	OBRKO(RABTK,1);

    //⁄¡–…”ÿ “≈⁄’Ãÿ‘¡‘œÕ ÔÌÚ
    OMRDATA &o = omrdata[ntk];
    double x,y;
    int r,VideoOffsetr;
//    TEX_INFO texr1;
//    TEX_INFO *texr=&texr1;
    char *m_dubl;
    
    r=(256+(tex_inf->wConvolutionsCount+o.catkzv)*sizeof(CONVOLUTION))/1024+1;
    VideoOffsetr=1024*r;
    m_dubl=(char*)malloc(VideoOffsetr);
//    memcpy(texr,tex_inf,sizeof(TEX_INFO));
    tex_inf->nVideoOffset=VideoOffsetr;
    tex_inf->wConvolutionsCount=tex_inf->wConvolutionsCount+o.catkzv;
    memcpy(m_dubl,m_kadr,VideoOffset);
    for(i=0;i<o.catkzv;i++)
    {
	CATZV &zv=o.catzv[i];
	coord_mm2pix(ntk,zv.x,zv.y,&x,&y);
	cnv->wNum=i+1+tex_inf->wConvolutionsCount-o.catkzv;
	cnv->nS=10;        //???
	cnv->nV=pow(10,(20-zv.v)/2.5);
	cnv->nX=x;
	cnv->nY=y;
	if (o.catss[i]==-1)
	    cnv->nID='KTZV';
	else
	{
	    cnv->nID='KZ++';		//œ‘œ÷ƒ≈”‘◊Ã≈Œ¡
//	    printf("KatZv: x=%8.3f y=%8.3f\n",x,y);
	    coord_mm2pix(ntk,o.izmzv[o.catss[i]].x,o.izmzv[o.catss[i]].y,&x,&y);
//	    printf("IzmZv: x=%8.3f y=%8.3f\n",x,y);
	    cnv->nS_XX=x;
	    cnv->nS_YY=y;
	}
	memcpy(m_dubl+256+(cnv->wNum-1)*sizeof(CONVOLUTION),cnv,sizeof(CONVOLUTION));
    }
    
    mode_t mode=S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    strcpy(sFile,"/tmp/resKPST.dat");
    fff=open(sFile,O_WRONLY | O_CREAT | O_TRUNC,mode);   //fcntl.h
    if (fff<0)
    {
	printf("Error write file /tmp/resKPST.dat\n");
	return false;
    }
    write(fff,m_dubl,VideoOffsetr);
    write(fff,m11,tex_inf->wSizeX*tex_inf->wSizeY*sizeof(short));
    
    close(fff);

    free(ObjAPOI[ntk]);
    free(KOAPOI[ntk]);
    free(ZVAPOI[ntk]);
    free(m_kadr);
    free(m_dubl);
    free(m11);
    if (sobj!=NULL)
	free(sobj);
    return true;
}
