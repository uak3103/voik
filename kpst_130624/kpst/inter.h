#ifndef _INTER_H
#define _INTER_H

/*      Коды отправителя сообщений      */

/*      Типы сообщений         */
#define BZ_TS      0     /* БЗ станции 		  */
#define BZEND_TS   1     /* БЗ выполнена 	  */
#define KONTR_TS   2     /* Контрольное сообщение */
#define ZRAB_TS    3     /* Закончить работу      */
#define RABZ_TS    4     /* Работа завершена 	  */
#define ESTKI_TS   5     /* Есть КИ       	  */
#define OTKAZST_TS 6     /* Отказ станции    	  */
#define USTSV_TS   7	 /* Установить связь 	  */
#define PODTVSV_TS 8 	 /* Подтвердить связь	  */
#define ESTNI_TS   9 	 /* Есть НИ		  */
#define ESTPROZR_TS  10	 /* Рез-т измер-ия прозрачности	*/
#define METEO_TS  11	 /* Метеоусловия	  */
#define ESTFON_TS  12	 /* Рез-т измерения фона  */
#define STOPBZ_TS  20	 /* Прекватить выполнение БЗ */

/*      Тип квантов		*/
#define POISK_KV   0    /* Поиск КО в ЛЗ 	*/
#define PRPOISK_KV 1	/* Продоолжение поиска в ЛЗ	*/
#define SBORKI_KV  2	/* Сбор КИНИ	*/
#define USLVID_KV  3	/* Измерение условий видимости	*/
#define POISKVDOL_KV   4    /* Поиск КО в ЛЗ 	*/

/*      Значения типа системы координат             */
#define	NEPODV_SK	0	/* 1-я координата - ДПТ		*/
#define PODV_SK		1	/* 1-я координата - alfa	*/

/*      Значения признака фона в ЛЗ             */
#define	NOFON_LZ	0	/* фон не задан */
#define YESFON_LZ	1	/* фон задан	*/

/* 	Значения признака сбора НИ (NI_KI)	*/
#define NO_NI	0	/* не надо НИ		*/
#define	EDIN_NI	1	/* нужен единичный замер	*/
#define	REALIZ_NI	2	/* нужен реализация 	*/

/* 	Значения признака фона в "Сбор КИНИ"	*/
#define NOFON_KI	0	/* фон не задан */
#define YESFON_KI	1	/* фон задан    */

/*      Признаки завершения в "БЗ выполнена"    */
#define BZVIP_PZ   0          /* успешно выполнена  */
#define KONET_PZ   1          /* КО не обнаружен    */
#define SRIVAS_PZ  2          /* срыв АС            */

/*	Методы получения измерений в "Есть КИ"	    */
#define AM_MET	 0
#define OMR_MET	 1

/*	Признак последнего пакета в  "Есть =НИ"	    */
#define NOLAST_PAK	 0
#define LAST_PAK	 1

/*      Признак измерения фона в "Измерение метеоусловий"    */
#define NO_FON   0          /* не измерять фон  */
#define YES_FON  1          /* измерять фон     */

/*      Признак измерения прозрачности в "Измерение метеоусловий"    */
#define NO_PROZR    0          /* не измерять прозрачность  */
#define YES_PROZR   1          /* измерять прозрачность     */

/*      Признак наличия Луны в зоне    */
#define NO_LUNA  0        /* Луна вне зоны  */
#define YES_LUNA 1        /* Луна в зоне    */

/*      Признак наличия параметров    */
#define NO_PAR  0        /* параметр не задан  */
#define YES_PAR 1        /* параметр задан     */

typedef struct {
       char ot;
       char ts;
       char typekv;
       char stac;
       double A;		// мин. ДПТ
       double h;		// мин. delta
       double delAA;	// макс. ДПТ
       double delh;		// макс. delta
       double r;
       char   isfon;
       double fon;
       char avoik;
	   } __attribute__((__packed__)) bz_poisk_ts_type;

typedef struct {
       char ot;
       char ts;
       char typekv;
       long  NKO;
       double tbeg;		// начальное смещение вдоль (сек)
       double tend;		// конечное смещение вдоль (сек)
       double smpopbeg;	// смещение поперек начальное(град)
       char	  TK;		// номер канала
       double kper;		// коэффициент перекрытия
       double razv;     // разворот поля зрения (град)
	   char		AVOIK;
       double   jd;
       double   tp;
       double   ap;
       double   e;
       double   i;
       double   OM;
       double   om;
       double   U;
	   double	BK;		// баллистический коеффициент
	   double   dT;		// падение периода за виток
	   char  pyrin;		// "спираль Пырина"
       double smpopend;	// смещение поперек конечное(град)
	   } __attribute__((__packed__)) bz_poiskvd_ts_type;

typedef struct {
       char ot;
       char ts;
       char typekv;
       double A;
       double h;
       double t;
       double vA;
       double vh;
       char   ni;
       double blesk;
       double maxpol;
       double maxt;
       char   isfon;
       double fon;
       double tfot;
       long Nko;
       double kep[8];
       char avoik;
       char tvfot;
       char autosvf;
	   } __attribute__((__packed__)) bz_sbor_ts_type;

typedef struct {
       char ot;
       char ts;
       char typekv;
       char fon;
       char prozr;
	   } __attribute__((__packed__)) bz_uslvid_ts_type;

typedef struct {
       char ot;
       char ts;
       short pz;
	   } __attribute__((__packed__)) bzend_ts_type;

typedef struct {
       char ot;
       char ts;
       short tks;
       double t;
       double A;
       double h;
	   } __attribute__((__packed__)) kontrcvs_ts_type;

typedef struct {
       char ot;
       char ts;
       double osa;
       double osl;
       double osr;
       double t;
       char estbz;
       char rabst;
       char rabvu;
       char rabcsn;
       char rabksio;
       char rabfmk;
       char rabapoi;
       char rabasev;
       char obmvu;
       char obmcsn;
       char obmfmk;
	   } __attribute__((__packed__)) kontrvss_ts_type;

typedef struct {
       char ot;
       char ts;
	   } __attribute__((__packed__)) zrab_ts_type;

typedef struct {
       char ot;
       char ts;
	   } __attribute__((__packed__)) rabz_ts_type;

typedef struct {
       char ot;
       char ts;
       double A;
       double h;
       double t;
       double vA;
       double vh;
       double blesk;
       double maxpol;
       double maxt;
       char   tk;
       char   met;
       char   st;       //В КПСТ не заполняется
       long   nlz, nko; //В КПСТ не заполняется
       double alfa;
       double delta;
       double valfa;
       double vdelta;
       int kzvizm;
       int kzvkat;
       int kzvomr;
	   } __attribute__((__packed__)) est_ki_ts_type;

typedef struct {
       char ot;
       char ts;
       char type;
       char text[100];
       double t;
       char rabst;
       char rabvu;
       char rabcsn;
       char rabksio;
       char rabfmk;
       char rabapoi;
       char rabasev;
       char obmvu;
       char obmcsn;
       char obmfmk;
	   } __attribute__((__packed__)) otkazst_ts_type;

typedef struct {
       char ot;
       char ts;
       long version;
       char pw;
       long jd;
	   } __attribute__((__packed__)) ustsv_ts_type;

typedef struct {
       char ot;
       char ts;
       long version;
       char pw;
       long jd;
	   } __attribute__((__packed__)) podtvsv_ts_type;

typedef struct {
       char ot;
       char ts;
       double time;
       short  nizm;
       double step;
       double oshblesk;
       char lastpak;
       char nst; //krikunov  по просьбе Попова
       char metod; //krikunov
       int masizm[1];

	   } __attribute__((__packed__)) est_ni_ts_type;

typedef struct {
       char ot;
       char ts;
       double  prozr;
	   } __attribute__((__packed__)) estprozr_ts_type;

typedef struct {
       char ot;
       char ts;
       char	istemp;
       double   temp;
       char	isdavl;
       double   davl;
       char	isvlajn;
       double   vlajn;
       char	isprozr;
       double   prozr;
       char     luna;
	   } __attribute__((__packed__)) meteo_ts_type;

typedef struct {
       char ot;
       char ts;
       double  fon;
	   } __attribute__((__packed__)) estfon_ts_type;



/*      Обязательные элементы              */
#define OTSOOB(a)	((bz_poisk_ts_type*)(a))->ot
#define TSSOOB(a) 	((bz_poisk_ts_type*)(a))->ts

/* Структура сообщения "Установить связь"             */
#define VERSION_UST(a) 	((ustsv_ts_type*)(a))->version
#define JD_UST(a) 	    ((ustsv_ts_type*)(a))->jd

/* Структура сообщения "Подтвердить связь"            */
#define VERSION_PUST(a)  ((podtvsv_ts_type*)(a))->version
#define JD_PUST(a)       ((podtvsv_ts_type*)(a))->jd

/*      Обязательные элементы для БЗ       */
#define TYPEKV(a) 	((bz_poisk_ts_type*)(a))->typekv

/* Структура кванта поиcка и продолжения поиcка	*/
#define TYPESK_LZ(a)		((bz_poisk_ts_type*)(a))->stac
#define DPT_LZ(a)		((bz_poisk_ts_type*)(a))->A
#define ALFA_LZ(a)		((bz_poisk_ts_type*)(a))->A
#define DELTA_LZ(a)		((bz_poisk_ts_type*)(a))->h
#define DELDPT_LZ(a)	((bz_poisk_ts_type*)(a))->delAA
#define DELALFA_LZ(a)	((bz_poisk_ts_type*)(a))->delAA
#define DELDEL_LZ(a)	((bz_poisk_ts_type*)(a))->delh
#define R_LZ(a)			((bz_poisk_ts_type*)(a))->r
#define ISFON_LZ(a)		((bz_poisk_ts_type*)(a))->isfon
#define FON_LZ(a)		((bz_poisk_ts_type*)(a))->fon
#define AVOIK_LZ(a)		((bz_poisk_ts_type*)(a))->avoik

/* Структура кванта поиcка вдоль траектории	*/
#define NKO_LZV(a)		((bz_poiskvd_ts_type*)(a))->NKO
#define TBEG_LZV(a)		((bz_poiskvd_ts_type*)(a))->tbeg
#define TEND_LZV(a)		((bz_poiskvd_ts_type*)(a))->tend
#define SMPOPBEG_LZV(a)	((bz_poiskvd_ts_type*)(a))->smpopbeg
#define SMPOPEND_LZV(a)	((bz_poiskvd_ts_type*)(a))->smpopend
#define TK_LZV(a)		((bz_poiskvd_ts_type*)(a))->TK
#define KPER_LZV(a)		((bz_poiskvd_ts_type*)(a))->kper
#define RAZV_LZV(a)		((bz_poiskvd_ts_type*)(a))->razv
#define AVOIK_LZV(a)	((bz_poiskvd_ts_type*)(a))->AVOIK
#define JD_LZV(a)  ((bz_poiskvd_ts_type*)(a))->jd
#define TP_LZV(a)  ((bz_poiskvd_ts_type*)(a))->tp
#define AP_LZV(a)  ((bz_poiskvd_ts_type*)(a))->ap
#define E_LZV(a)   ((bz_poiskvd_ts_type*)(a))->e
#define I_LZV(a)   ((bz_poiskvd_ts_type*)(a))->i
#define OMB_LZV(a) ((bz_poiskvd_ts_type*)(a))->OM
#define OML_LZV(a) ((bz_poiskvd_ts_type*)(a))->om
#define U_LZV(a)   ((bz_poiskvd_ts_type*)(a))->U
#define BK_LZV(a)  ((bz_poiskvd_ts_type*)(a))->BK
#define DT_LZV(a)  ((bz_poiskvd_ts_type*)(a))->dT
#define EXKEP_LZV(a) &((bz_poiskvd_ts_type*)(a))->jd
#define PYRIN_LZV(a) ((bz_poiskvd_ts_type*)(a))->pyrin

/* Структура кванта сбора КИНИ            */
#define A_KI(a)			((bz_sbor_ts_type*)(a))->A
#define H_KI(a)			((bz_sbor_ts_type*)(a))->h
#define T_KI(a)			((bz_sbor_ts_type*)(a))->t
#define VA_KI(a)		((bz_sbor_ts_type*)(a))->vA
#define VH_KI(a)		((bz_sbor_ts_type*)(a))->vh
#define NI_KI(a)		((bz_sbor_ts_type*)(a))->ni
#define BLESK_KI(a)		((bz_sbor_ts_type*)(a))->blesk
#define MAXPOL_KI(a)	((bz_sbor_ts_type*)(a))->maxpol
#define MAXTIME_KI(a)	((bz_sbor_ts_type*)(a))->maxt
#define ISFON_KI(a)     ((bz_sbor_ts_type*)(a))->isfon
#define FON_KI(a)       ((bz_sbor_ts_type*)(a))->fon
#define TIMEFOT_NI(a)   ((bz_sbor_ts_type*)(a))->tfot
#define NKO_KI(a)	((bz_sbor_ts_type*)(a))->Nko
#define KEP_KI(a)	((bz_sbor_ts_type*)(a))->kep
#define AVOIK_KI(a)	((bz_sbor_ts_type*)(a))->avoik
#define TVFOT_KI(a)	((bz_sbor_ts_type*)(a))->tvfot
#define AUTOSVF_KI(a)	((bz_sbor_ts_type*)(a))->autosvf

/* Структура кванта измерения условий видимости            */
#define FON_IMT(a)		((bz_uslvid_ts_type*)(a))->fon
#define PROZR_IMT(a)	((bz_uslvid_ts_type*)(a))->prozr

/* Структура сообщения "БЗ выполнена"            */
#define PZ_BZV(a)      ((bzend_ts_type*)(a))->pz

/* Структура КС от ЦВС */
#define TKS_KS(a) 	((kontrcvs_ts_type*)(a))->tks
#define A_KS(a) 	((kontrcvs_ts_type*)(a))->A
#define H_KS(a) 	((kontrcvs_ts_type*)(a))->h
#define T_KS(a)		((kontrcvs_ts_type*)(a))->t

/* Структура КС от ВС_С */
#define OSA_KS(a) 	((kontrvss_ts_type*)(a))->osa
#define OSL_KS(a) 	((kontrvss_ts_type*)(a))->osl
#define OSR_KS(a) 	((kontrvss_ts_type*)(a))->osr
#define TEKT_KS(a) 	((kontrvss_ts_type*)(a))->t
#define ESTBZ_KS(a) 	((kontrvss_ts_type*)(a))->estbz
#define RABST_KS(a) 	((kontrvss_ts_type*)(a))->rabst
#define RABVU_KS(a) 	((kontrvss_ts_type*)(a))->rabvu
#define RABCSN_KS(a) 	((kontrvss_ts_type*)(a))->rabcsn
#define RABKSIO_KS(a)	((kontrvss_ts_type*)(a))->rabksio
#define RABFMK_KS(a) 	((kontrvss_ts_type*)(a))->rabfmk
#define RABAPOI_KS(a)	((kontrvss_ts_type*)(a))->rabapoi
#define RABASEV_KS(a)   ((kontrvss_ts_type*)(a))->rabasev
#define OBMVU_KS(a)     ((kontrvss_ts_type*)(a))->obmvu
#define OBMCSN_KS(a)    ((kontrvss_ts_type*)(a))->obmcsn
#define OBMFMK_KS(a)    ((kontrvss_ts_type*)(a))->obmfmk

/* Структура сообщения "Есть КИ"            */
#define A_OBN(a)		((est_ki_ts_type*)(a))->A
#define H_OBN(a)		((est_ki_ts_type*)(a))->h
#define T_OBN(a)		((est_ki_ts_type*)(a))->t
#define VA_OBN(a)		((est_ki_ts_type*)(a))->vA
#define VH_OBN(a)		((est_ki_ts_type*)(a))->vh
#define BLESK_OBN(a)	((est_ki_ts_type*)(a))->blesk
#define MAXPOL_OBN(a)	((est_ki_ts_type*)(a))->maxpol
#define MAXTIME_OBN(a)	((est_ki_ts_type*)(a))->maxt
#define TK_OBN(a)		((est_ki_ts_type*)(a))->tk
#define MET_OBN(a)		((est_ki_ts_type*)(a))->met
#define ALFA_OBN(a)		((est_ki_ts_type*)(a))->alfa
#define DELTA_OBN(a)		((est_ki_ts_type*)(a))->delta
#define VALFA_OBN(a)		((est_ki_ts_type*)(a))->valfa
#define VDELTA_OBN(a)		((est_ki_ts_type*)(a))->vdelta
#define KZVIZM_OBN(a)		((est_ki_ts_type*)(a))->kzvizm
#define KZVKAT_OBN(a)		((est_ki_ts_type*)(a))->kzvkat
#define KZVOMR_OBN(a)		((est_ki_ts_type*)(a))->kzvomr

/* Структура сообщения "Есть НИ"            */
#define TIME_FOT(a)		((est_ni_ts_type*)(a))->time
#define NIZM_FOT(a)		((est_ni_ts_type*)(a))->nizm
#define STEP_FOT(a)		((est_ni_ts_type*)(a))->step
#define OSHBLESK_FOT(a)	((est_ni_ts_type*)(a))->oshblesk
#define LASTPAK_FOT(a)	((est_ni_ts_type*)(a))->lastpak
#define MASIZM_FOT(a)	((est_ni_ts_type*)(a))->masizm
#define METOD_FOT(a)	((est_ni_ts_type*)(a))->metod //krikunov

/* Структура сообщения "Результат измерения прозрачности "    */
#define PROZR_RIP(a)      ((estprozr_ts_type*)(a))->prozr

/* Структура сообщения "Метеоусловия"            */
#define ISTEMP_MTO(a)	((meteo_ts_type*)(a))->istemp
#define TEMP_MTO(a)		((meteo_ts_type*)(a))->temp
#define ISDAVL_MTO(a)	((meteo_ts_type*)(a))->isdavl
#define DAVL_MTO(a)		((meteo_ts_type*)(a))->davl
#define ISVLAJN_MTO(a)	((meteo_ts_type*)(a))->isvlajn
#define VLAJN_MTO(a)	((meteo_ts_type*)(a))->vlajn
#define ISPROZR_MTO(a)	((meteo_ts_type*)(a))->isprozr
#define PROZR_MTO(a)	((meteo_ts_type*)(a))->prozr
#define LUNA_MTO(a)		((meteo_ts_type*)(a))->luna

/* Структура сообщения "Результат измерения фона "            */
#define FON_RIF(a)      ((estfon_ts_type*)(a))->fon

/* Структура сообщения "Отказ станции"                        */
#define TEXT_OC(a)      ((otkazst_ts_type*)(a))->text
#define TEKT_OC(a) 	    ((otkazst_ts_type*)(a))->t
#define RABST_OC(a) 	((otkazst_ts_type*)(a))->rabst
#define RABVU_OC(a) 	((otkazst_ts_type*)(a))->rabvu
#define RABCSN_OC(a) 	((otkazst_ts_type*)(a))->rabcsn
#define RABKSIO_OC(a)	((otkazst_ts_type*)(a))->rabksio
#define RABFMK_OC(a) 	((otkazst_ts_type*)(a))->rabfmk
#define RABAPOI_OC(a)	((otkazst_ts_type*)(a))->rabapoi
#define RABASEV_OC(a)   ((otkazst_ts_type*)(a))->rabasev
#define OBMVU_OC(a)     ((otkazst_ts_type*)(a))->obmvu
#define OBMCSN_OC(a)    ((otkazst_ts_type*)(a))->obmcsn
#define OBMFMK_OC(a)    ((otkazst_ts_type*)(a))->obmfmk

#endif // _INTER_H
