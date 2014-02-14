#ifndef PCI_7841_H
#define PCI_7841_H

#define	MAX_PCI_CARDS	4
#define MAXSUPPORT 		4

#define   SIGEVENT1		63
#define   SIGEVENT2		62
//#define   SIGEVENT3	61
//#define   SIGEVENT4	60
//#define   SIGEVENT5	59
//#define   SIGEVENT6	58
//#define   SIGEVENT7	57
//#define   SIGEVENT8	56
//#define   SIGEVENT9	55
//#define   SIGEVENT10	54
//#define   SIGEVENT11	53
//#define   SIGEVENT12	52
//#define   SIGEVENT13	51
//#define   SIGEVENT14	50

#define ERR_NoError                   0
#define ERR_InvalidCardNumber        -1
#define ERR_OpenFileFailed           -2
#define ERR_OpenEventFailed          -3
#define ERR_BoardNoInit              -4
#define ERR_AllocateMemory           -5
#define ERR_OwnerSet                 -6
#define ERR_SignalHandle             -7
#define ERR_SignalNotify             -8
#define ERR_InputAxisError					 -9
#define ERR_SetVelError              -10
#define ERR_ClrPlsError              -11
#define ERR_RunMode                  -12
#define ERR_MoveError                -13

#ifndef BYTE
typedef unsigned char BYTE;
#endif  //  BYTE

#ifndef WORD
typedef unsigned short WORD;
#endif  //  WORD

#ifndef DWORD
typedef unsigned long DWORD;
#endif  //  DWORD

//  Define for can port struct
typedef struct
{
    int mode;       //  0   for 11-bit;     1 for 29-bit
    DWORD accCode, accMask;
    int baudrate;	//	0 : 125KBps, 1 : 250KBps, 2 : 500KBps, 3 : 1MBps,
					//	4 : Self-Defined
	BYTE brp, tseg1, tseg2;	//	Used only if baudrate = 4
	BYTE sjw, sam;          //  Used only if baudrate = 4

}PORT_STRUCT;

//  Define can packet struct
typedef struct 
{
    DWORD CAN_ID;       //  CAN id
    BYTE rtr;           //  RTR bit
    BYTE len;           //  Data length
    BYTE data[8];       //  Data
    DWORD time;         //  Occur time (non use)
    BYTE reserved;      //  future use
}CAN_PACKET;

//  Define CAN status register
typedef struct
{
    unsigned char RxBuffer      : 1;
    unsigned char DataOverrun   : 1;
    unsigned char TxBuffer      : 1;
    unsigned char TxEnd         : 1;
    unsigned char RxStatus      : 1;
    unsigned char TxStatus      : 1;
    unsigned char ErrorStatus   : 1;
    unsigned char BusStatus     : 1;
    //unsigned short reserved      : 8;
} PORTREG_BIT;

typedef union 
{
    PORTREG_BIT bit;
    unsigned char reg;
}PORT_REG;

typedef struct 
{
    PORT_STRUCT port;
    PORT_REG status;
}PORT_STATUS;

#define DISABLE            0x00
#define ENABLE             0x01

#define PCI_INIT_OK        0x00
#define PCI_INIT_ERROR     -1
#define PCI_INIT_NotFound  -2

#define EEPROM_WR_OK       0x00
#define EEPROM_WR_ERR      -3


WORD  GetDriverVersion(void);
int  CanOpenDriver(int card, int port);
int  CanCloseDriver(int handle);
int  CanConfigPort(int handle, PORT_STRUCT *PortStruct);
int  CanGetPortStatus(int handle, PORT_STATUS *PortStruct);
int  CanDetectBaudrate(int handle, int miliSecs);
int  CanGetReceiveEvent(int handle,void (*event_handler)(int));
int  CanStopReceiveEvent(int handle);
int  CanGetRcvCnt(int handle);

int  CanGetReceiveErrorWarningEvent(int handle,void (*event_handler)(int));
int  CanStopReceiveErrorWarningEvent(int handle);
int  CanGetReceiveDataOverrunEvent(int handle,void (*event_handler)(int));
int  CanStopReceiveDataOverrunEvent(int handle);
int  CanGetReceiveWakeUpEvent(int handle,void (*event_handler)(int));
int  CanStopReceiveWakeUpEvent(int handle);
int  CanGetReceiveErrorPassiveEvent(int handle,void (*event_handler)(int));
int  CanStopReceiveErrorPassiveEvent(int handle);
int  CanGetReceiveArbitrationLostEvent(int handle,void (*event_handler)(int));
int  CanStopReceiveArbitrationLostEvent(int handle);
int  CanGetReceiveBusErrorEvent(int handle,void (*event_handler)(int));
int  CanStopReceiveBusErroreEvent(int handle);


BYTE  _7841_Read(int handle, int offset);
void  _7841_Write(int handle, int offset, BYTE data);
//  use for cPCI-7841
BYTE   CanGetLedStatus(int handle, int index);
void  CanSetLedStatus(int handle, int index, int mode);
//  CAN layer functions
void  CanEnableReceive(int handle);
void  CanDisableReceive(int handle);
int  CanSendMsg(int handle, CAN_PACKET *packet);
int  CanRcvMsg(int handle, CAN_PACKET *packet);

//  CAN layer status control
int  CanGetPortStatus(int handle, PORT_STATUS *PortStatus);
void  CanClearOverrun(int handle);
void  CanClearRxBuffer(int handle);
void  CanClearTxBuffer(int handle);
BYTE  CanGetArbitrationLostBit(int handle);
BYTE  CanGetErrorCode(int handle);
BYTE  CanGetErrorWarningLimit(int handle);
void  CanSetErrorWarningLimit(int handle, BYTE limit);
BYTE  CanGetRxErrorCount(int handle);
BYTE  CanGetTxErrorCount(int handle);
void  CanSetTxErrorCount(int handle, BYTE value);
BYTE  CanGetIntStatusReg(int handle);

#endif

