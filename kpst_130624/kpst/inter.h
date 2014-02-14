#ifndef _INTER_H
#define _INTER_H

/*      ���� ����������� ���������      */

/*      ���� ���������         */
#define BZ_TS      0     /* �� ������� 		  */
#define BZEND_TS   1     /* �� ��������� 	  */
#define KONTR_TS   2     /* ����������� ��������� */
#define ZRAB_TS    3     /* ��������� ������      */
#define RABZ_TS    4     /* ������ ��������� 	  */
#define ESTKI_TS   5     /* ���� ��       	  */
#define OTKAZST_TS 6     /* ����� �������    	  */
#define USTSV_TS   7	 /* ���������� ����� 	  */
#define PODTVSV_TS 8 	 /* ����������� �����	  */
#define ESTNI_TS   9 	 /* ���� ��		  */
#define ESTPROZR_TS  10	 /* ���-� �����-�� ������������	*/
#define METEO_TS  11	 /* ������������	  */
#define ESTFON_TS  12	 /* ���-� ��������� ����  */
#define STOPBZ_TS  20	 /* ���������� ���������� �� */

/*      ��� �������		*/
#define POISK_KV   0    /* ����� �� � �� 	*/
#define PRPOISK_KV 1	/* ������������ ������ � ��	*/
#define SBORKI_KV  2	/* ���� ����	*/
#define USLVID_KV  3	/* ��������� ������� ���������	*/
#define POISKVDOL_KV   4    /* ����� �� � �� 	*/

/*      �������� ���� ������� ���������             */
#define	NEPODV_SK	0	/* 1-� ���������� - ���		*/
#define PODV_SK		1	/* 1-� ���������� - alfa	*/

/*      �������� �������� ���� � ��             */
#define	NOFON_LZ	0	/* ��� �� ����� */
#define YESFON_LZ	1	/* ��� �����	*/

/* 	�������� �������� ����� �� (NI_KI)	*/
#define NO_NI	0	/* �� ���� ��		*/
#define	EDIN_NI	1	/* ����� ��������� �����	*/
#define	REALIZ_NI	2	/* ����� ���������� 	*/

/* 	�������� �������� ���� � "���� ����"	*/
#define NOFON_KI	0	/* ��� �� ����� */
#define YESFON_KI	1	/* ��� �����    */

/*      �������� ���������� � "�� ���������"    */
#define BZVIP_PZ   0          /* ������� ���������  */
#define KONET_PZ   1          /* �� �� ���������    */
#define SRIVAS_PZ  2          /* ���� ��            */

/*	������ ��������� ��������� � "���� ��"	    */
#define AM_MET	 0
#define OMR_MET	 1

/*	������� ���������� ������ �  "���� =��"	    */
#define NOLAST_PAK	 0
#define LAST_PAK	 1

/*      ������� ��������� ���� � "��������� ������������"    */
#define NO_FON   0          /* �� �������� ���  */
#define YES_FON  1          /* �������� ���     */

/*      ������� ��������� ������������ � "��������� ������������"    */
#define NO_PROZR    0          /* �� �������� ������������  */
#define YES_PROZR   1          /* �������� ������������     */

/*      ������� ������� ���� � ����    */
#define NO_LUNA  0        /* ���� ��� ����  */
#define YES_LUNA 1        /* ���� � ����    */

/*      ������� ������� ����������    */
#define NO_PAR  0        /* �������� �� �����  */
#define YES_PAR 1        /* �������� �����     */

typedef struct {
       char ot;
       char ts;
       char typekv;
       char stac;
       double A;		// ���. ���
       double h;		// ���. delta
       double delAA;	// ����. ���
       double delh;		// ����. delta
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
       double tbeg;		// ��������� �������� ����� (���)
       double tend;		// �������� �������� ����� (���)
       double smpopbeg;	// �������� ������� ���������(����)
       char	  TK;		// ����� ������
       double kper;		// ����������� ����������
       double razv;     // �������� ���� ������ (����)
	   char		AVOIK;
       double   jd;
       double   tp;
       double   ap;
       double   e;
       double   i;
       double   OM;
       double   om;
       double   U;
	   double	BK;		// �������������� �����������
	   double   dT;		// ������� ������� �� �����
	   char  pyrin;		// "������� ������"
       double smpopend;	// �������� ������� ��������(����)
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
       char   st;       //� ���� �� �����������
       long   nlz, nko; //� ���� �� �����������
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
       char nst; //krikunov  �� ������� ������
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



/*      ������������ ��������              */
#define OTSOOB(a)	((bz_poisk_ts_type*)(a))->ot
#define TSSOOB(a) 	((bz_poisk_ts_type*)(a))->ts

/* ��������� ��������� "���������� �����"             */
#define VERSION_UST(a) 	((ustsv_ts_type*)(a))->version
#define JD_UST(a) 	    ((ustsv_ts_type*)(a))->jd

/* ��������� ��������� "����������� �����"            */
#define VERSION_PUST(a)  ((podtvsv_ts_type*)(a))->version
#define JD_PUST(a)       ((podtvsv_ts_type*)(a))->jd

/*      ������������ �������� ��� ��       */
#define TYPEKV(a) 	((bz_poisk_ts_type*)(a))->typekv

/* ��������� ������ ���c�� � ����������� ���c��	*/
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

/* ��������� ������ ���c�� ����� ����������	*/
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

/* ��������� ������ ����� ����            */
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

/* ��������� ������ ��������� ������� ���������            */
#define FON_IMT(a)		((bz_uslvid_ts_type*)(a))->fon
#define PROZR_IMT(a)	((bz_uslvid_ts_type*)(a))->prozr

/* ��������� ��������� "�� ���������"            */
#define PZ_BZV(a)      ((bzend_ts_type*)(a))->pz

/* ��������� �� �� ��� */
#define TKS_KS(a) 	((kontrcvs_ts_type*)(a))->tks
#define A_KS(a) 	((kontrcvs_ts_type*)(a))->A
#define H_KS(a) 	((kontrcvs_ts_type*)(a))->h
#define T_KS(a)		((kontrcvs_ts_type*)(a))->t

/* ��������� �� �� ��_� */
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

/* ��������� ��������� "���� ��"            */
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

/* ��������� ��������� "���� ��"            */
#define TIME_FOT(a)		((est_ni_ts_type*)(a))->time
#define NIZM_FOT(a)		((est_ni_ts_type*)(a))->nizm
#define STEP_FOT(a)		((est_ni_ts_type*)(a))->step
#define OSHBLESK_FOT(a)	((est_ni_ts_type*)(a))->oshblesk
#define LASTPAK_FOT(a)	((est_ni_ts_type*)(a))->lastpak
#define MASIZM_FOT(a)	((est_ni_ts_type*)(a))->masizm
#define METOD_FOT(a)	((est_ni_ts_type*)(a))->metod //krikunov

/* ��������� ��������� "��������� ��������� ������������ "    */
#define PROZR_RIP(a)      ((estprozr_ts_type*)(a))->prozr

/* ��������� ��������� "������������"            */
#define ISTEMP_MTO(a)	((meteo_ts_type*)(a))->istemp
#define TEMP_MTO(a)		((meteo_ts_type*)(a))->temp
#define ISDAVL_MTO(a)	((meteo_ts_type*)(a))->isdavl
#define DAVL_MTO(a)		((meteo_ts_type*)(a))->davl
#define ISVLAJN_MTO(a)	((meteo_ts_type*)(a))->isvlajn
#define VLAJN_MTO(a)	((meteo_ts_type*)(a))->vlajn
#define ISPROZR_MTO(a)	((meteo_ts_type*)(a))->isprozr
#define PROZR_MTO(a)	((meteo_ts_type*)(a))->prozr
#define LUNA_MTO(a)		((meteo_ts_type*)(a))->luna

/* ��������� ��������� "��������� ��������� ���� "            */
#define FON_RIF(a)      ((estfon_ts_type*)(a))->fon

/* ��������� ��������� "����� �������"                        */
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
