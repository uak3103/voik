int num_komplex=1; 
int inter_flag=0;    //интерфейс инициализирован
int CVS_SV,   //с кем установлена связь
    CVS_BZ;   //от кого пришла БЗ
int    svust_cvs=0; // признак: если = 1, то связь с ЦВС установлена
double tks_cvs=0;   // время получения последнего КС
int dtks_cvs=1;     // периодичность получения КС

char bz[200];       // текущая БЗ
int estbz_cvs=0;    // признак наличия БЗ

int nzas=0;         // координатные засечки по моделируемому КО
double Azas[2], hzas[2], tzas[2];
int kontr_flag=0, podtv_flag=0;
double kontr_time=+1e+10;

extern double DAVLENIE, TEMPERATURA;
int breakcode;

int kontr_sv=3;//0; //krikunov
int interNST(void)
{
  return (TIPST-55)+5*(NPORST-1);
}

void kon_sv(void)
{
  /*svust_cvs=0;*/ CVS_SV=0; CVS_BZ=0; kontr_time=+1e+10;
}


char* breakcodestr(int code)
{ char *s;
 switch(code) {
   case 1: s="Прервано оператором"; break;
   case 2: s="Сбой станции"; break;
   case 3: s="Перезапуск ЦВС"; break;
   case 4: s="Получена другая БЗ"; break;
   case 5: s="Завершение работы с ЦВС"; break;
   case 6: s="Разрыв связи с ЦВС"; break;
   case 7: s="Прервано по команде ЦВС"; break;
   default:s="???";
 }
 return s;
}

char *TSSOOBstr(int tip)
{ char *s;
 switch(tip) {
   case KONTR_TS:   s="КОНТР";   break;
   case USTSV_TS:   s="УСТСВ";   break;
   case PODTVSV_TS: s="ПОДТВСВ"; break;
   case BZ_TS:      s="БЗ";      break;
   case ZRAB_TS:    s="ЗРАБ";    break;
   case METEO_TS:   s="МЕТЕО";   break;
   case BZEND_TS:   s="БЗВЫП";   break;
   case RABZ_TS:    s="РАБЗ";    break;
   case	ESTKI_TS:   s="ЕСТЬКИ";  break;
   case	OTKAZST_TS: s="СБОЙСТ";  break;
   case	ESTNI_TS:   s="ЕСТЬНИ";  break;
   case	ESTPROZR_TS:s="ЕСТЬПРОЗР";break;
   case	ESTFON_TS:  s="ЕСТЬФОН"; break;
   default: s="???";
 }
 return s;
}

void output_msg(int code, ...)
{
  int *par = (&code)+1;
  char s[150]; int len; double t;
    int komu;

  OTSOOB(s) = interNST();
  TSSOOB(s) = code;
  switch (code) {
    case KONTR_TS:  len = sizeof(kontrvss_ts_type);
		    OSA_KS(s) = _cdf(0);
		    OSL_KS(s) = _cdf(1);
		    OSR_KS(s) = _cdf(2);
		    TEKT_KS(s)= time_kpst2kbp(rvtime);
		    ESTBZ_KS(s)= estbz_cvs;
            RABST_KS(s)  = rabst;
            RABVU_KS(s)  = rabvu;
            RABCSN_KS(s) = rabcsn;
            RABKSIO_KS(s)= rabksio;
            RABFMK_KS(s) = rabfmk;
            RABAPOI_KS(s)= rabapoi;
            RABASEV_KS(s)= rabasev;
            OBMVU_KS(s)=0;
            OBMCSN_KS(s)=0;
            OBMFMK_KS(s)=0;
            komu=CVS_SV;
		    break;
    case USTSV_TS:  len = sizeof(ustsv_ts_type); komu=CVS_SV;
                    VERSION_UST(s)=myversion; // 06/02/2004
                    break;
    case PODTVSV_TS:len = sizeof(podtvsv_ts_type); komu=CVS_SV;
                    VERSION_PUST(s)=myversion; // 06/02/2004
                    break;
    case RABZ_TS  : len = sizeof(rabz_ts_type); komu=CVS_SV; break;
    case BZEND_TS : PZ_BZV(s) = par[0];
		    len = sizeof(bzend_ts_type); komu=CVS_BZ; break;
    case OTKAZST_TS:
         len = sizeof(otkazst_ts_type); komu=CVS_BZ;
         //пересылка текста сообщения об ошибке. 06/02/2004
         char *text=TEXT_OC(s);
         strcpy(text,sterrmsg1);
         if (strlen(sterrmsg2)!=0)
         { strcat(text,"   "); strcat(text,sterrmsg2); }
         if (strlen(sterrmsg3)!=0)
         { strcat(text,"   "); strcat(text,sterrmsg3); }
         //пересылка текущего времени и признаков
         //работоспособности. 19/01/2007
         TEKT_OC(s)= time_kpst2kbp(rvtime);
         RABST_OC(s)  = rabst;
         RABVU_OC(s)  = rabvu;
         RABCSN_OC(s) = rabcsn;
         RABKSIO_OC(s)= rabksio;
         RABFMK_OC(s) = rabfmk;
         RABAPOI_OC(s)= rabapoi;
         RABASEV_OC(s)= rabasev;
         OBMVU_OC(s)=0;
         OBMCSN_OC(s)=0;
         OBMFMK_OC(s)=0;
         break;
  }
  IPX_Send( komu, s, len );
  if (code!=KONTR_TS&&code!=PODTVSV_TS)
	 printf("%02d:%02d:%02.0f послано сбщ. %s\n",
		 rvhour,rvmin,rvsec,TSSOOBstr(code));
}

void input_msg( int N, char *s, int m )
{

  if (CVS!=1) return; //Работа не под управлением ЦВС

/*
  if (TSSOOB(s)!=KONTR_TS)
     printf("получено сбщ. %s\n",TSSOOBstr(TSSOOB(s)));
*/

  switch (TSSOOB(s)) {
/********************  KONTR  *********************************/
  case KONTR_TS:
      if (N == interNST()) break;
      
      if (!svust_cvs) {
        CVS_SV=N;
        svust_cvs=1;
        //podtv_flag=1;
        //output_msg(PODTVSV_TS);
      }
      dtks_cvs  = TKS_KS(s);
      if (nzas<2) {
	Azas[nzas] = A_KS(s);
	hzas[nzas] = H_KS(s);
	tzas[nzas] = rvtime;//T_KS(s);
	nzas++;
      } else {
	Azas[0]=Azas[1]; Azas[1]=A_KS(s);
	hzas[0]=hzas[1]; hzas[1]=H_KS(s);
	tzas[0]=tzas[1]; tzas[1]=rvtime;//T_KS(s);
      }
      //output_msg(KONTR_TS);
      kontr_flag=1;
      kontr_time=rvtime;
      break;

/********************  USTSV  *********************************/
  case USTSV_TS:
    if(svust_cvs) breakcode=3; /* Перезапуск ЦВС */
    CVS_SV=N;
    version_KP54=VERSION_UST(s); // 06/02/2004
    if (version_KP54<1 || version_KP54>100) version_KP54=0;
    jd_KP54=JD_UST(s);           // 06/02/2004
    svust_cvs=1;
    podtv_flag=1;
    kontr_time=+1e+10;
    break;

/********************  PODTSV  *********************************/
  case PODTVSV_TS:
    version_KP54=VERSION_PUST(s); // 06/02/2004
    if (version_KP54<1 || version_KP54>100) version_KP54=0;
    jd_KP54=JD_PUST(s);           // 06/02/2004
    CVS_SV=N;
    svust_cvs=1;
    kontr_time=+1e+10;
    break;

/***************************************************************/
  case BZ_TS:
    CVS_BZ=N;
    if (estbz_cvs) breakcode=4;
    memset(bz,0,200);
    //bz_sbor_ts_type &xx = *(bz_sbor_ts_type*)bz;
    //int sm=(char*)(&xx.tvfot)-(char*)(&xx);
    memcpy(bz,s,m);
    estbz_cvs=1;
    break;

/***************************************************************/
  case ZRAB_TS:
    breakcode=5;
    kon_sv();
    break;

/***************************************************************/
  case METEO_TS:
    if (ISDAVL_MTO(s)==YES_PAR) DAVLENIE=DAVL_MTO(s);
    if (ISTEMP_MTO(s)==YES_PAR) TEMPERATURA=TEMP_MTO(s);
    break;

/***************************************************************/
  case STOPBZ_TS:
    CVS_BZ=N;
    if (estbz_cvs) breakcode=7;
    break;

  }

}

void inter_takt(void)
{
  if (inter_flag) {
    if (!svust_cvs) CVS_SV=interNST();
    output_msg(KONTR_TS);
  }

  if (CVS!=1) return;

  if (podtv_flag) { output_msg(PODTVSV_TS); podtv_flag=0; }
//=======krikunov===============
//если в теч. времени kontr_sv не было контр.сообщ от ЦВС -
  //разрыв связи с ЦВС
if (kontr_sv!=0)
  if (svust_cvs && rvtime-kontr_time > kontr_sv &&
      rvtime-kontr_time < kontr_sv+5)
    { breakcode=6; kon_sv(); }
//=======krikunov===============



}

void GetKoordModel(double t, double *A, double *h )
{
  switch(nzas) {
    case 0: *A=0; *h=0; break;
    case 1: *A=Azas[0]; *h=hzas[0]; break;
    default:
      if (fabs(tzas[1]-tzas[0])<0.0001)  {
	*A=Azas[0]; *h=hzas[0];
      } else {
	*A = (Azas[1]-Azas[0])*(t-tzas[0])/(tzas[1]-tzas[0]) + Azas[0];
	*h = (hzas[1]-hzas[0])*(t-tzas[0])/(tzas[1]-tzas[0]) + hzas[0];
      }
  }
}

void waitbz_cvs(void)
{
   int key, s;
rept:
   breakcode=0;
   if (!svust_cvs)
     printf( "Ждем установления связи с ВС-К (выход-F10)\n" );
   else if (!estbz_cvs)
     printf( "Ждем БЗ от ВС-К (выход-F10)\n" );
   else
     return;
   s = svust_cvs;

   while(1) {
     delaytakt(1);

     check_KP54_date();

     if (breakcode!=0) {
       //printf("%s!\n",breakcodestr(breakcode));
       void printsterror(void);
       printsterror();
       if (breakcode==1) return; // нажата F10
       else if (breakcode==5||breakcode==6) goto rept; // потеря связи с ЦВС
       else breakcode=0;
     }
     if (s==0 && svust_cvs==1)
       { printf("Связь установлена. "); goto rept; }
     if (svust_cvs==1 && estbz_cvs==1) return;
   }

}

void stat_recv_kp54(char *buf, int len);
void stat_send_kp54(char *buf, int len);


int send_to_kp54(int komu, void *mas, int len)
{
  if (inter_flag) {
    IPX_Send(komu, mas, len);
    stat_send_kp54( (char*)mas, len);
    return 1;
  } else 
    return 0;
}

int initinter(void)
{
  if (!IPX_Init(interNST(),input_msg)) {
    inter_flag=0; 
    return 0;
  }
  inter_flag=1;
  return 1;
}


////////////////////////// статистика обменов с КП54 ////////////////////////

//счетчики

int recv_kp54_bytes=0, recv_kp54_packets=0,
    recv_kp54_BZ=0, recv_kp54_KONTR=0, recv_kp54_ZRAB=0, recv_kp54_USTSV=0,
    recv_kp54_PODTVSV=0, recv_kp54_METEO=0, recv_kp54_STOPBZ=0,
    recv_kp54_unknown=0;
    
int send_kp54_bytes=0, send_kp54_packets=0, 
    send_kp54_BZEND=0, send_kp54_KONTR=0, send_kp54_RABZ=0, send_kp54_ESTKI=0, 
    send_kp54_OTKAZST=0, send_kp54_USTSV=0, send_kp54_PODTVSV=0, send_kp54_ESTNI=0,
    send_kp54_ESTPROZR=0, send_kp54_ESTFON=0,
    send_kp54_unknown=0;
    

void stat_recv_kp54(char *buf, int len)
{
    recv_kp54_bytes += len;
    recv_kp54_packets++;
    switch( TSSOOB(buf) ) {
        case BZ_TS:         recv_kp54_BZ++;       break;
        case KONTR_TS:      recv_kp54_KONTR++;    break;
        case ZRAB_TS:       recv_kp54_ZRAB++;     break;
        case USTSV_TS:      recv_kp54_USTSV++;    break;
        case PODTVSV_TS:    recv_kp54_PODTVSV++;  break;
        case METEO_TS:      recv_kp54_METEO++;    break;
        case STOPBZ_TS:     recv_kp54_STOPBZ++;   break;
        default:            recv_kp54_unknown++;
    }
}

void stat_send_kp54(char *buf, int len)
{
    send_kp54_bytes += len;
    send_kp54_packets++;
    switch( TSSOOB(buf) ) {
        case BZEND_TS:      send_kp54_BZEND++;    break;
        case KONTR_TS:      send_kp54_KONTR++;    break;
        case RABZ_TS:       send_kp54_RABZ++;     break;
        case ESTKI_TS:      send_kp54_ESTKI++;    break;
        case OTKAZST_TS:    send_kp54_OTKAZST++;  break;
        case USTSV_TS:      send_kp54_USTSV++;    break;
        case PODTVSV_TS:    send_kp54_PODTVSV++;  break;
        case ESTNI_TS:      send_kp54_ESTNI++;    break;
        case ESTPROZR_TS:   send_kp54_ESTPROZR++; break;
        case ESTFON_TS:     send_kp54_ESTFON++;   break;
        default:            send_kp54_unknown++;
    }
}


void print_stat_kp54(void)
{
  printf("Принято:\n"  
         "Байт: %d\n"
         "Всего сообщений: %d\n" 
         "БЗ:%3d   контрольных:%3d   уст.св.:%3d   подтв.св.: %3d\n"
         "заверш.раб.:%3d   стопБЗ:%3d   метео:%3d\n"
         "неопознанных:%3d\n\n", 
         recv_kp54_bytes, 
         recv_kp54_packets,
         recv_kp54_BZ, recv_kp54_KONTR, recv_kp54_USTSV, recv_kp54_PODTVSV,
         recv_kp54_ZRAB, recv_kp54_STOPBZ, recv_kp54_METEO,
         recv_kp54_unknown);
         
  printf("Отправлено:\n"
         "Байт: %d\n"
         "Всего сообщений: %d\n" 
         "контр:%3d   уст.св.:%3d   подтв.св.: %3d   БЗвып:%3d\n"
         "естьКИ:%3d   естьНИ:%3d   отказст:%3d   раб.заверш.:%3d\n"
         "есть прозр.:%3d   есть фон:%3d\n"
         "неопознанных:%3d\n", 
         send_kp54_bytes, 
         send_kp54_packets,
         send_kp54_KONTR, send_kp54_USTSV, send_kp54_PODTVSV, send_kp54_BZEND,
         send_kp54_ESTKI, send_kp54_ESTNI, send_kp54_OTKAZST, send_kp54_RABZ,
         send_kp54_ESTPROZR, send_kp54_ESTFON,
         send_kp54_unknown);

}
