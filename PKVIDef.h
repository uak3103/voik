#ifndef PKVIDEF_H_INCLUDED
#define PKVIDEF_H_INCLUDED

#define WORD u_short
#define DWORD u_int
#define BYTE u_char
#define UINT u_int
#define BOOL bool
#define LONG long

#define MAKEWORD(a, b)  ((WORD) (((BYTE) (a)) | ((WORD) ((BYTE) (b))) << 8))
#define LOBYTE(x) (u_char)(x&0xff)
#define HIBYTE(x) (u_char)(x>>8)
#define LOWORD(x) (u_int16_t)(x&0xffff)
#define HIWORD(x) (u_int16_t)(x>>16)
typedef DWORD   COLORREF;
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#define BI_RGB	0

#undef max
#define max(x,y) ((x) > (y) ? (x) : (y))
#undef min
#define min(x,y) ((x) < (y) ? (x) : (y))

#pragma pack(push, 2)

typedef struct tagBITMAPINFOHEADER
{
	DWORD  biSize;
	LONG   biWidth;
	LONG   biHeight;
	WORD   biPlanes;
	WORD   biBitCount;
	DWORD  biCompression;
	DWORD  biSizeImage;
	LONG   biXPelsPerMeter;
	LONG   biYPelsPerMeter;
	DWORD  biClrUsed;
	DWORD  biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagBITMAPFILEHEADER
{
	WORD    bfType;
	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagRGBQUAD
{
	BYTE    rgbBlue;
	BYTE    rgbGreen;
	BYTE    rgbRed;
	BYTE    rgbReserved;
} RGBQUAD;

#pragma pack(pop)

#pragma pack(1)

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
	u_char		btReserv;
	u_char		btStepIntegration;
	u_char		btReserv1;
	u_int16_t	wSpeedTransfer;
	u_char		pbtReserv1[54];

    u_int32_t   DatchA;
    u_int32_t   DatchL;
    u_int32_t   DatchR;
/*  UAK
    u_int16_t	wDatchAGrad;
	u_char		btDatchAMinutes;
	u_char		btDatchASeconds;
	u_int16_t	wDatchLGrad;
	u_char		btDatchLMinutes;
	u_char		btDatchLSeconds;
	u_int16_t	wDatchRGrad;
	u_char		btDatchRMinutes;
	u_char		btDatchRSeconds;
*/
	u_char		pbtReserv2[100];
};

struct CONVOLUTION
{
	u_int32_t	nID;
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

struct INFO_DATA
{
	u_char		btCamNum;
	u_char		btType;
	u_char		pbtMAC[6];
	u_char		pbtReserv1[8];
	u_char		pbtIPCam[4];
	u_int16_t 	wReserv1;
	u_int16_t	wReserv2;
	u_char		pbtReserv2[4];
	u_int16_t	wPortHP;
	u_int16_t	wReserv3;
	u_char		pbtReserv3[4];
	u_int16_t	wPortLP;
	u_char		pbtReserv4[10];
	union
	{
		struct
		{
			u_char	btError	:4;
			u_char	btPower	:1;
			u_char	btCamOn	:1;
			u_char	btMatrixOutput	:1;
			u_char	btCooler	:1;
		}Ready;
		u_char	btReady;
	};
	u_char 		btReserv5;

	u_char		btYear;
	u_char		btMonth;
	u_char		btDay;
	u_char		btHour;
	u_char		btMinute;
	u_char		btSecond;
	u_int32_t	nCamWork;
	u_int32_t	nReserv6;
	u_int32_t	nExposition;
	u_int16_t	wOffsetX;
	u_int16_t	wOffsetY;
	u_int16_t	wSizeX;
	u_int16_t	wSizeY;
	u_char		btBinX;
	u_char		btBinY;
	union
	{
		struct
		{
			u_char	btReserv7	:5;
			u_char	btStabBlackLevel	:1;
			u_char	btExcludeDarkCurrent	:1;
			u_char	btCorrSens	:1;
		}CadrProcessing;
		u_char	btCadrProcessing;
	};
	union
	{
		struct
		{
			u_char	btAutoLevel	:1;
			u_char	btReserv8	:6;
			u_char	btFindConvolution	:1;
		}AmpLevel;
		u_char	btAmpLevel;
	};
	u_int16_t	nAmpLevel;
	int16_t		nTempCurrent;
	int16_t		nTempSet;
	u_int16_t	wAnalogAmpl;
	u_int16_t	wEmAmpl;
	u_int16_t	wReadFreq;
	u_int16_t	wBlackLevel;
	u_int16_t	wSpeedTransfer;
	u_char		btStepCount;
	u_char		pbtReserv7[31];

	u_int16_t	wDatchAGrad;
	u_char		btDatchAMinutes;
	u_char		btDatchASeconds;
	u_int16_t	wDatchLGrad;
	u_char		btDatchLMinutes;
	u_char		btDatchLSeconds;
	u_int16_t	wDatchRGrad;
	u_char		btDatchRMinutes;
	u_char		btDatchRSeconds;
	u_char		pbtReserv9[868];

};

struct COMMAND
{
	u_char			pbtID[8];
	u_int32_t		nSize;
	u_int32_t		nNumCommand;
	u_int16_t		nCodCommand;
};

struct CAN_INFO
{
	union
	{
		struct
		{
			u_char	btCool	:1;
			u_char	btTermostatReady	:1;
			u_char	btUpHot	:1;
			u_char	btTermostatStatus	:1;
			u_char 	btTermMatrixCrist	:1;
			u_char 	btTermMatrixRadiator	:1;
			u_char 	btTermMatrixPeltie	:1;
			u_char 	btTermRadiator	:1;
		}sTermostat;
		u_char	btTermostat;
	};
	union
	{
		struct
		{
			u_char	btErrorCode	:4;
			u_char	btPower	:1;
			u_char	btEthernetLink	:1;
			u_char	btErrorSyncro	:1;
			u_char	btKPDO	:1;
		}sErrors;
		u_char	btErrors;
	};
	union
	{
		struct
		{
			u_char	btSurvay	:1;
			u_char	btCamera	:1;
			u_char	btMatrixOutput	:1;
			u_char	btTestImage	:2;
			u_char	btReserv	:3;
		}sStatus;
		u_char	btStatus;
	};
	u_char btReserv1;
};
#pragma pack()


extern u_int32_t g_nCommandNum;

enum marker_type {cross=0, cross1=1, rect=2, rect1=3, circle=4, };
struct MARKER
{
	bool bVisible;
	marker_type nType;
	int nSize;
	wxPoint Coord;
};

struct CALC_INFO
{
    WORD wMin;
    WORD wMax;
    u_int64_t ullMid;
    u_int64_t ullDev;
    float fD;
};

struct CAMERA_INFO
{
	wxString sName;
	wxString sIP;
	u_int16_t nPort;
	wxString sGroupIP;
	u_int16_t nGroupPort;
};

WX_DEFINE_ARRAY(CAMERA_INFO*,CameraInfoArray);

int FindCameraInfo(CameraInfoArray& cameraInfoArr, wxString sCameraName, CAMERA_INFO* pCamInfo=NULL);
void RemoveAllCameraInfo(CameraInfoArray& cameraInfoArr);
void CopyCameraInfo(CameraInfoArray& src, CameraInfoArray& dest);

u_int32_t GetIP(wxString sIP);
wxString GetIP(u_int32_t nIP);
#endif // PKVIDEF_H_INCLUDED
