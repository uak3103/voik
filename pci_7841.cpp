#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <math.h>
#include <signal.h>
#include <string.h>
#include "pci_7841.h"
#include "commdef.h"
#include <time.h>

typedef struct
{
      	unsigned short    	initFlag;
        unsigned char   	Owner_Set;
        int    				DeviceID;
        struct sigaction sig_act1, sig_act2;
 //       struct sigaction sig_actErrorWarning1, sig_actDataOverrun1;
 //       struct sigaction sig_actWakeUp1, sig_actPassiveEvent1;
 //       struct sigaction sig_actArbitrationLost1, sig_actBusError1;
 //       struct sigaction sig_actErrorWarning2, sig_actDataOverrun2;
 //       struct sigaction sig_actWakeUp2, sig_actPassiveEvent2;
 //       struct sigaction sig_actArbitrationLost2, sig_actBusError2;
} Lib_PCI_Info;

typedef struct
{
  U16 ret_val;
  int offset;
  BYTE data;
} REG_RW;

typedef struct
{
  DWORD handle;
  DWORD mode;
} RETINFO;

typedef struct
{
  DWORD handle;
  U16 ret_val;
  CAN_PACKET msg;
} CAN_MSG;

typedef struct
{
  DWORD handle;
  U16 ret_val;
} REC_CN;

int initFlag=0, totalCards=0;
int LED_ADDR_START = 64;
WORD majorVersion = 0x00;
WORD minorVersion = 0x04;
Lib_PCI_Info pci_Info_7841[MAXSUPPORT];
int InstallRef[MAXSUPPORT*2] = { 0, 0, 0, 0, 0, 0, 0, 0};
PORT_STRUCT portStruct[MAXSUPPORT*2];
BYTE canIntStatus[MAXSUPPORT*2];
BYTE LEDStatus[MAXSUPPORT];


WORD  GetDriverVersion(void)
{
	return majorVersion<<8 | minorVersion;
}

int  _7841_InstallPCI(int card_number)
{
     int DevId;
    char buf[255];

    if (card_number>=MAXSUPPORT)return ERR_InvalidCardNumber;

    sprintf(buf,"/dev/PCI7841W%d",card_number);
    DevId = open( buf, O_RDWR );
    if (DevId == -1)
       return ERR_OpenFileFailed;

    memset( &(pci_Info_7841[card_number]), '\0', sizeof( Lib_PCI_Info ) );
    pci_Info_7841[card_number].DeviceID = DevId;
    pci_Info_7841[card_number].initFlag=0x7841;
    if( fcntl( DevId, F_SETOWN, getpid()) != -1 ){
    	pci_Info_7841[card_number].Owner_Set = 1;
        pci_Info_7841[card_number].sig_act1.sa_handler = SIG_IGN;
        pci_Info_7841[card_number].sig_act2.sa_handler = SIG_IGN;
/*
        pci_Info_7841[card_number].sig_actErrorWarning1.sa_handler = SIG_IGN;
        pci_Info_7841[card_number].sig_actDataOverrun1.sa_handler = SIG_IGN;
        pci_Info_7841[card_number].sig_actWakeUp1.sa_handler = SIG_IGN;
        pci_Info_7841[card_number].sig_actPassiveEvent1.sa_handler = SIG_IGN;
        pci_Info_7841[card_number].sig_actArbitrationLost1.sa_handler = SIG_IGN;
        pci_Info_7841[card_number].sig_actBusError1.sa_handler = SIG_IGN;
        pci_Info_7841[card_number].sig_actErrorWarning2.sa_handler = SIG_IGN;
        pci_Info_7841[card_number].sig_actDataOverrun2.sa_handler = SIG_IGN;
        pci_Info_7841[card_number].sig_actWakeUp2.sa_handler = SIG_IGN;
        pci_Info_7841[card_number].sig_actPassiveEvent2.sa_handler = SIG_IGN;
        pci_Info_7841[card_number].sig_actArbitrationLost2.sa_handler = SIG_IGN;
        pci_Info_7841[card_number].sig_actBusError2.sa_handler = SIG_IGN;
*/
        if( sigaction( SIGEVENT1, &(pci_Info_7841[0].sig_act1), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT2, &(pci_Info_7841[0].sig_act2), NULL ) ==-1 )
                return ERR_OpenEventFailed;
/*
        if( sigaction( SIGEVENT3, &(pci_Info_7841[0].sig_actErrorWarning1), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT4, &(pci_Info_7841[0].sig_actDataOverrun1), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT5, &(pci_Info_7841[0].sig_actWakeUp1), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT6, &(pci_Info_7841[0].sig_actPassiveEvent1), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT7, &(pci_Info_7841[0].sig_actArbitrationLost1), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT8, &(pci_Info_7841[0].sig_actBusError1), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT9, &(pci_Info_7841[0].sig_actErrorWarning2), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT10, &(pci_Info_7841[0].sig_actDataOverrun2), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT11, &(pci_Info_7841[0].sig_actWakeUp2), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT12, &(pci_Info_7841[0].sig_actPassiveEvent2), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT13, &(pci_Info_7841[0].sig_actArbitrationLost2), NULL ) ==-1 )
                return ERR_OpenEventFailed;
        if( sigaction( SIGEVENT14, &(pci_Info_7841[0].sig_actBusError2), NULL ) ==-1 )
                return ERR_OpenEventFailed;
*/
    }
    else
	pci_Info_7841[0].Owner_Set = 0;
    return(ERR_NoError);
}


int  CanGetReceiveEvent(int handle, void (*event_handler)(int))
{
    U16 enable_flag = TRUE;
    int ret_value=0;

    if(pci_Info_7841[handle/2].initFlag == FALSE)
	return ERR_BoardNoInit;
    if (InstallRef[handle] != 0 )
    {
	if( pci_Info_7841[handle/2].Owner_Set == 0)
	   return ERR_OwnerSet; // check the owner setting
	if( event_handler == NULL )
	   return (ERR_SignalHandle);
	else{
	   if ((handle%2) == 0){
		pci_Info_7841[handle/2].sig_act1.sa_handler = event_handler;
		pci_Info_7841[handle/2].sig_act1.sa_flags |= SA_RESTART;
		if( sigaction( SIGEVENT1, &(pci_Info_7841[handle/2].sig_act1), NULL ) == -1 )
			return(ERR_SignalNotify);
		ret_value = ioctl(pci_Info_7841[handle/2].DeviceID,IOCTL_7841_SIGNAL1_SET,&(enable_flag));
	   }
	   else {
		pci_Info_7841[handle/2].sig_act2.sa_handler = event_handler;
		pci_Info_7841[handle/2].sig_act2.sa_flags |= SA_RESTART;
		if( sigaction( SIGEVENT2, &(pci_Info_7841[handle/2].sig_act2), NULL ) == -1 )
			return(ERR_SignalNotify);
		ret_value = ioctl(pci_Info_7841[handle/2].DeviceID,IOCTL_7841_SIGNAL2_SET,&(enable_flag));
	   }
	   if( ret_value == 0 )
		return(ERR_NoError);
	   else
		return (ERR_AllocateMemory);
	}
    }
    else
      return ERR_BoardNoInit;
}

int  CanStopReceiveEvent(int handle)
{
    U16 enable_flag = FALSE;
    int ret_value=0;

    if(pci_Info_7841[handle/2].initFlag == FALSE)
	return ERR_BoardNoInit;
    if (InstallRef[handle] != 0 )
    {
	if ((handle%2) == 0) {
	   ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL1_SET, &enable_flag);
	   pci_Info_7841[handle/2].sig_act1.sa_handler = SIG_IGN;
	   if( sigaction( SIGEVENT1, &(pci_Info_7841[handle/2].sig_act1), NULL ) ==-1 )
		return(ERR_SignalNotify);
	}
	if ((handle%2) == 1){
	   ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL2_SET, &enable_flag);
	   pci_Info_7841[handle/2].sig_act2.sa_handler = SIG_IGN;
	   if( sigaction( SIGEVENT2, &(pci_Info_7841[handle/2].sig_act2), NULL ) ==-1 )
		return(ERR_SignalNotify);
	}
        if( ret_value == 0 )
	   return(ERR_NoError);
        else
	   return (ERR_AllocateMemory);
    }
    else
      return ERR_BoardNoInit;
}


int  CanOpenDriver(int card, int port)
{
	int index;
	PORT_STRUCT setPort;	// 2006/06/05, for init config

	if(pci_Info_7841[card].initFlag==FALSE) {
		if (_7841_InstallPCI(card) != ERR_NoError) {
			printf("Couldn't find PCI-7841 card %d\n", card);
            		return -1;
		}
		else
			totalCards++;
	}
        index = card*2+port;
        InstallRef[index] = 1;

	// 2006/06/05, init config------------------------------------------------
        setPort.mode     = 0;       //  0 : 11-bit ;  1 : 29-bit CAN network
        setPort.accCode  = 0;       //  Only ID = accCode can pass the filter
        setPort.accMask  = 0x7ff;   //  Don't care bit
        setPort.baudrate = 3;       //  0: 125kBps; 1:250kBps;  2:500kBps; 3:1MBps
        CanConfigPort(index, &setPort);
	// --------------------------------------------------------------------------

        return index;
}

int  CanCloseDriver(int handle)
{
	int anotherPort, count;
    	REG_RW temp_write_reg;

	if (InstallRef[handle] != 1)
		return -1;
	InstallRef[handle]=0;

     	temp_write_reg.offset = (handle%2)*128;
    	temp_write_reg.data = 0x01;
	ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_CLOSE_PORT, &temp_write_reg);
	anotherPort = 0;
	for(count=0;count<totalCards*2;count++)
	    if (InstallRef[count] == 1)
	        anotherPort = 1;
	if (anotherPort == 0)
	{
		temp_write_reg.offset = LED_ADDR_START;
		temp_write_reg.data = 0xFF;
		ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_CLOSE_CARD, &temp_write_reg);
	}
	return 0;
}

int  CanConfigPort(int handle, PORT_STRUCT *PortStruct)
{
    	BYTE temp;
	RETINFO RetInfo;

	//  Port has not installed
	if (InstallRef[handle] != 1)
		return -1;

    	if (PortStruct == NULL)
        	return -1;
	if ((PortStruct->mode != 0) && (PortStruct->mode != 1))
		return -1;
	//	Disable Interrupt
	temp = _7841_Read(handle, 4);
	_7841_Write(handle, 4, 0x00);
	//	Entering Reset mode
	_7841_Write(handle, 0, 0x01);
	//	setup PeliCAN, CBP=1, No TX1 output and No Clk out
	_7841_Write(handle, 31, 0xC8);

	//	setup acceptance code & acceptance mask
	//	Standard frame, single filter mode

	if (PortStruct->mode == 0)		//	11-bit ID
	{
		//	Disable checking data field
		_7841_Write(handle, 18, 0x00);
		_7841_Write(handle, 19, 0x00);
		_7841_Write(handle, 22, 0xff);
		_7841_Write(handle, 23, 0xff);

		_7841_Write(handle, 16, (BYTE)(PortStruct->accCode >> 3));
		_7841_Write(handle, 17, (BYTE)((PortStruct->accCode << 5) | 0x10));
		_7841_Write(handle, 20, (BYTE)(PortStruct->accMask >> 3));
		_7841_Write(handle, 21, (BYTE)((PortStruct->accMask << 5) | 0x10));
	}
	else							//	29-bit ID
	{
		//	Fill Acceptance Code Register
		_7841_Write(handle, 16, (BYTE)(PortStruct->accCode >> 21));
		_7841_Write(handle, 17, (BYTE)(PortStruct->accCode >> 13));
		_7841_Write(handle, 18, (BYTE)(PortStruct->accCode >> 5));
		_7841_Write(handle, 19, (BYTE)(PortStruct->accCode << 3));

		//	Fill Acceptance Mask Register
		_7841_Write(handle, 20, (BYTE)(PortStruct->accMask >> 21));
		_7841_Write(handle, 21, (BYTE)(PortStruct->accMask >> 13));
		_7841_Write(handle, 22, (BYTE)(PortStruct->accMask >> 5));
		_7841_Write(handle, 23, (BYTE)(PortStruct->accMask << 3) | 0x04);  // Or RTR
	}

	//	setup bus timing registers
	switch(PortStruct->baudrate)
	{
		//	125kBps
		case 0:	_7841_Write(handle, 6, 0x03);
				_7841_Write(handle, 7, 0x1C);
				break;
		//	250kBps
		case 1:	_7841_Write(handle, 6, 0x01);
				_7841_Write(handle, 7, 0x1C);
				break;
		//	500kBps
		case 2:	_7841_Write(handle, 6, 0x00);
				_7841_Write(handle, 7, 0x1C);
				break;
		//	1MkBps
		case 3:	_7841_Write(handle, 6, 0x00);
				_7841_Write(handle, 7, 0x14);
				break;
		//	Self Defined Baudrate
		case 4: _7841_Write(handle, 6, PortStruct->brp | PortStruct->sjw << 6);
//                		_7841_Write(handle, 7, PortStruct->sam << 8 | PortStruct->tseg2 << 4 | PortStruct->tseg1);
				// 2007/04/25 Bug fix!
                                _7841_Write(handle, 7, PortStruct->sam << 7 | PortStruct->tseg2 << 4 | PortStruct->tseg1);

				break;
		default :   return -1;
	}
	//	setup output control register;  normal mode; push-pull
	_7841_Write(handle, 8, 0xfa);
	//	return operation mode
	//	write port mode to VXD
	RetInfo.handle=handle;
	RetInfo.mode=PortStruct->mode;
    	ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SET_PORT_MODE, &RetInfo);

	//	enable receive and transmit interrupt
	_7841_Write(handle, 0, 0x08);
    	_7841_Write(handle, 4, temp | 0x03);

    //	copy configuration to Port Setting
	memcpy((void*)(&(portStruct[handle])), PortStruct, sizeof(PORT_STRUCT));
	return 0;
}

int  CanGetPortStatus(int handle, PORT_STATUS *PortStatus)
{
    memcpy((void*)&(PortStatus->port), (void*)(&(portStruct[handle])), sizeof(PORT_STRUCT));
    (PortStatus->status).reg = _7841_Read(handle, 2);
    return 0;
}

BYTE  _7841_Read(int handle, int offset)
{
    REG_RW temp_reg;

    if (InstallRef[handle] == 0)
	return 0;
    temp_reg.offset = offset+(128 * (handle%2));
    ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_REG_READ, &temp_reg);
    return (temp_reg.data);

}

void  _7841_Write(int handle, int offset, BYTE data)
{
    REG_RW temp_reg;

    if (InstallRef[handle] == 0)
	return;
    temp_reg.offset = offset+(128*(handle%2));
    temp_reg.data = data;
    ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_REG_WRITE, &temp_reg);
}


BYTE  CanGetLedStatus(int card_number, int index)
{
	return (LEDStatus[card_number]>>index) & 1;
}

void  CanSetLedStatus(int cardno, int index, int mode)
{
	BYTE tmp;
    REG_RW temp_write_reg;

    tmp = 0x01 << index;
    switch(mode)
    {
        case 0 : LEDStatus[cardno] &= ~tmp;
                 break;
        case 1 : LEDStatus[cardno] |= tmp;
                 break;
        case 2 : break;
    }


    temp_write_reg.offset = LED_ADDR_START;
    temp_write_reg.data = LEDStatus[cardno];
    ioctl(pci_Info_7841[cardno].DeviceID, IOCTL_7841_REG_WRITE, &temp_write_reg);
	return;
}

void  CanEnableReceive(int handle)
{
    _7841_Write(handle, 4, _7841_Read(handle, 4) | 0x01);
   	return;
}


void  CanDisableReceive(int handle)
{
    _7841_Write(handle, 4, _7841_Read(handle, 4) & 0xFE);
	return;
}

int  CanSendMsg(int handle, CAN_PACKET *msg)
{
        CAN_MSG canmsg;
        long   result, i;
        PORT_STATUS status;

        // 2008/05/21 - Canceled 1ms delay
        //              Retry 200 times until TX transmittion was done
        for( i = 0; i < 200; i++)
        {
                CanGetPortStatus(handle, &status);
                if(status.status.bit.TxStatus)
                        usleep(10);
                else
                        break;
        }
        if(i == 200)
        {
                return -1;
        }

//      usleep(1000);
        memcpy((CAN_PACKET*)(&(canmsg.msg)), msg, sizeof(CAN_PACKET));
        canmsg.handle = handle;
        ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SET_BUFFER_DATA, &canmsg);
        //yuan modify 06/18/01
        result = canmsg.ret_val;
        if (result < 0)
           return -1;
        return 0;
}

int  CanRcvMsg(int handle, CAN_PACKET *msg)
{
	CAN_MSG	canmsg;
	long   result;

	if( (msg == NULL) || (InstallRef[handle] != 1) )		return -1;
	canmsg.handle = handle;
    	ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_GET_BUFFER_DATA, &canmsg);
	//yuan modify 06/18/01
	result = canmsg.ret_val;
	if (result >= 0) {//yuan modify for data received successfully
        	memcpy(msg, (CAN_PACKET*)(&(canmsg.msg)), sizeof(CAN_PACKET));
		return 0;
    	}
    return -1;
}

int  CanGetRcvCnt(int handle)
{
	REC_CN	rec_cn;

	if(InstallRef[handle] != 1)
		return -1;
	rec_cn.handle = handle;
        ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_GET_UNRD_COUNT, &rec_cn);
        return rec_cn.ret_val;
}

void  CanClearOverrun(int handle)
{
    //	Clear Data Overrun
	_7841_Write(handle, 1, 0x08);
	//	Enable Data Overrun Interrupt and Receive Interrupt
	_7841_Write(handle, 4, _7841_Read(handle, 4) | 0x09);
}

void  CanClearRxBuffer(int handle)
{
        int i;
        if(InstallRef[handle] != 1)
                return ;

        // 2008/06/03 - Bug fixed. Clear hardware buffers in limited number
        for(i = 0; i < 64; i++)
        {
                if(_7841_Read(handle, 29) != 0)
                        _7841_Write(handle, 1, 0x04);
                else
                        break;
        }

        // 2008/06/03 - Clear driver buffers
        ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_CLEAR_RX_BUFFER, &handle);

        return;
}

void  CanClearTxBuffer(int handle)
{
        if(InstallRef[handle] != 1)
                return ;

        //  write SJA CMR to abort transmission
        _7841_Write(handle, 1, 0x02);

        // 2008/06/03 - Clear driver buffers
        ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_CLEAR_TX_BUFFER, &handle);

        return;
}

BYTE  CanGetArbitrationLostBit(int handle)
{
    return _7841_Read(handle, 11);
}

BYTE  CanGetErrorCode(int handle)
{
    return _7841_Read(handle, 12);
}

BYTE  CanGetErrorWarningLimit(int handle)
{
    return _7841_Read(handle, 13);
}

void  CanSetErrorWarningLimit(int handle, BYTE limit)
{
    BYTE v = _7841_Read(handle, 0);
    //  enter RESET mode
    _7841_Write(handle, 0, 0);
    _7841_Write(handle, 13, limit);
    _7841_Write(handle, 0, v);
}

BYTE  CanGetRxErrorCount(int handle)
{
    return _7841_Read(handle, 14);
}

BYTE  CanGetTxErrorCount(int handle)
{
    return _7841_Read(handle, 15);
}

void  CanSetTxErrorCount(int handle, BYTE value)
{
    _7841_Write(handle, 15, value);
}

BYTE  CanGetIntStatusReg(int handle)
{
    return canIntStatus[handle];
}

int CanDetectBaudrate(int handle, int miliSecs)
{
    PORT_STRUCT save, set;
    long count, mode = -1;
    time_t start,end;
    BYTE previousMode;

    //  Save previous Port setting
    memcpy((void*)&save, (void*)&portStruct[handle], sizeof(PORT_STRUCT));
    memcpy((void*)&set, (void*)&save, sizeof(PORT_STRUCT));
    //  Bit-rate detection algorithm
    //  save previoius mode and enter Listen only mode
    previousMode = _7841_Read(handle, 0);
    _7841_Write(handle, 0, 0x02);

    for(count=3;count>=0;count--)
    {
          set.baudrate = count;
          CanConfigPort(handle, &set);
          canIntStatus[handle] = _7841_Read(handle,0x02);
          canIntStatus[handle] &= 0xFE;
          _7841_Write(handle,0x02,canIntStatus[handle]);
          CanEnableReceive(handle);
          time(&start);
 	  do
	  {
	       canIntStatus[handle] = _7841_Read(handle,0x02);
	       //  Bus error occurs
	       if(canIntStatus[handle] & 0x80)
               {
                     printf("0x80");
                     break;
               }
               if(canIntStatus[handle] & 0x01)
               {
                     mode = count;
                     break;
               }
               time(&end);
          } while(difftime(end,start)<((double)miliSecs/1000.0));

	  if (mode != -1)
             break;
      }
      CanConfigPort(handle, &save);
      CanEnableReceive(handle);
      _7841_Write(handle, 0, previousMode);

      if (mode == -1)    return -1;
      return mode;
}

/*
int  CanGetReceiveErrorWarningEvent(int handle, void (*event_handler)(int))
{
    U16 enable_flag = TRUE;
    int ret_value=0;

   if(pci_Info_7841[handle/2].initFlag == FALSE)
	return ERR_BoardNoInit;
   if (InstallRef[handle] != 0 ) {
   	if( pci_Info_7841[handle/2].Owner_Set == 0)
		return ERR_OwnerSet; // check the owner setting
   	if( event_handler == NULL )
		return (ERR_SignalHandle);
 	else {
	   if ((handle%2) == 0) {
	      pci_Info_7841[handle/2].sig_actErrorWarning1.sa_handler = event_handler;
	      pci_Info_7841[handle/2].sig_actErrorWarning1.sa_flags |= SA_RESTART;
	      if( sigaction( SIGEVENT3, &(pci_Info_7841[handle/2].sig_actErrorWarning1), NULL ) == -1 )
		   return(ERR_SignalNotify);
	      ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL3_SET, &(enable_flag));
	   }
	   if ((handle%2) == 1){
	      pci_Info_7841[handle/2].sig_actErrorWarning2.sa_handler = event_handler;
	      pci_Info_7841[handle/2].sig_actErrorWarning2.sa_flags |= SA_RESTART;
	      if( sigaction( SIGEVENT9, &(pci_Info_7841[handle/2].sig_actErrorWarning2), NULL ) == -1 )
		   return(ERR_SignalNotify);
	      ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL9_SET, &(enable_flag));
	   }
	   if( ret_value == 0 )
	      return(ERR_NoError);
	   else
	      return (ERR_AllocateMemory);
	}
   }
   else
     return ERR_BoardNoInit;
}

int  CanStopReceiveErrorWarningEvent(int handle)
{
    U16 enable_flag = FALSE;
    int ret_value=0;

    if(pci_Info_7841[handle/2].initFlag == FALSE)
	return ERR_BoardNoInit;
    if (InstallRef[handle] != 0 )
    {
 	if ((handle%2) == 0) {
	   ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL3_SET, &enable_flag);
	   pci_Info_7841[handle/2].sig_actErrorWarning1.sa_handler = SIG_IGN;
	   if ( sigaction( SIGEVENT3, &(pci_Info_7841[handle/2].sig_actErrorWarning1), NULL ) ==-1 )
		return(ERR_SignalNotify);
	}
	if ((handle%2) == 1){
	   ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL9_SET, &enable_flag);
	   pci_Info_7841[handle/2].sig_actErrorWarning2.sa_handler = SIG_IGN;
	   if ( sigaction( SIGEVENT9, &(pci_Info_7841[handle/2].sig_actErrorWarning2), NULL ) ==-1 )
		return(ERR_SignalNotify);
	}
        if( ret_value == 0 )
	   return(ERR_NoError);
        else
	   return (ERR_AllocateMemory);
    }
    else
       return ERR_BoardNoInit;
}

int  CanGetReceiveDataOverrunEvent(int handle, void (*event_handler)(int))
{
    U16 enable_flag = TRUE;
    int ret_value=0;

    if(pci_Info_7841[handle/2].initFlag == FALSE)
	return ERR_BoardNoInit;
    if (InstallRef[handle] != 0 ) {
       if( pci_Info_7841[handle/2].Owner_Set == 0)
	   return ERR_OwnerSet; // check the owner setting
       if( event_handler == NULL )
	   return (ERR_SignalHandle);
       else {
	   if ((handle%2) == 0) {
	      pci_Info_7841[handle/2].sig_actDataOverrun1.sa_handler = event_handler;
	      pci_Info_7841[handle/2].sig_actDataOverrun1.sa_flags |= SA_RESTART;
	      if( sigaction( SIGEVENT4, &(pci_Info_7841[handle/2].sig_actDataOverrun1), NULL ) == -1 )
		   return(ERR_SignalNotify);
	      ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL4_SET, &(enable_flag));
	   }
	   if ((handle%2) == 1){
	      pci_Info_7841[handle/2].sig_actDataOverrun2.sa_handler = event_handler;
	      pci_Info_7841[handle/2].sig_actDataOverrun2.sa_flags |= SA_RESTART;
	      if( sigaction( SIGEVENT10, &(pci_Info_7841[handle/2].sig_actDataOverrun2), NULL ) == -1 )
		   return(ERR_SignalNotify);
	      ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL10_SET,&(enable_flag));
	   }
	   if( ret_value == 0 )
	      return(ERR_NoError);
	   else
	   return (ERR_AllocateMemory);
      }
   }
   else
       return ERR_BoardNoInit;
}

int  CanStopReceiveDataOverrunEvent(int handle)
{
    U16 enable_flag = FALSE;
    int ret_value=0;

    if(pci_Info_7841[handle/2].initFlag == FALSE)
	return ERR_BoardNoInit;
    if (InstallRef[handle] != 0 ) {
	if ((handle%2) == 0){
	   ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL4_SET, &enable_flag);
	   pci_Info_7841[handle/2].sig_actDataOverrun1.sa_handler = SIG_IGN;
	   if( sigaction( SIGEVENT4, &(pci_Info_7841[handle/2].sig_actDataOverrun1), NULL ) ==-1 )
		return(ERR_SignalNotify);
	}
	if ((handle%2) == 1){
	   ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL10_SET, &enable_flag);
	   pci_Info_7841[handle/2].sig_actDataOverrun2.sa_handler = SIG_IGN;
	   if( sigaction( SIGEVENT10, &(pci_Info_7841[handle/2].sig_actDataOverrun2), NULL ) ==-1 )
				return(ERR_SignalNotify);
	}
        if( ret_value == 0 )
	   return(ERR_NoError);
        else
	   return (ERR_AllocateMemory);
    }
   else
       return ERR_BoardNoInit;
}


int  CanGetReceiveWakeUpEvent(int handle, void (*event_handler)(int))
{
    U16 enable_flag = TRUE;
    int ret_value=0;

   if(pci_Info_7841[handle/2].initFlag == FALSE)
   	return ERR_BoardNoInit;
   if (InstallRef[handle] != 0 ) {
       if( pci_Info_7841[handle/2].Owner_Set == 0)
	    return ERR_OwnerSet; // check the owner setting
       if( event_handler == NULL )
	    return (ERR_SignalHandle);
       else {
	   if ((handle%2) == 0) {
		pci_Info_7841[handle/2].sig_actWakeUp1.sa_handler = event_handler;
		pci_Info_7841[handle/2].sig_actWakeUp1.sa_flags |= SA_RESTART;
		if( sigaction( SIGEVENT5, &(pci_Info_7841[handle/2].sig_actWakeUp1), NULL ) == -1 )
				return(ERR_SignalNotify);
		ret_value = ioctl(pci_Info_7841[handle/2].DeviceID,IOCTL_7841_SIGNAL5_SET,&(enable_flag));
	   }
	   if ((handle%2) == 1) {
		pci_Info_7841[handle/2].sig_actWakeUp2.sa_handler = event_handler;
		pci_Info_7841[handle/2].sig_actWakeUp2.sa_flags |= SA_RESTART;
		if( sigaction( SIGEVENT11, &(pci_Info_7841[handle/2].sig_actWakeUp2), NULL ) == -1 )
				return(ERR_SignalNotify);
	        ret_value = ioctl(pci_Info_7841[handle/2].DeviceID,IOCTL_7841_SIGNAL11_SET,&(enable_flag));
           }
	   if( ret_value == 0 )
	   	return(ERR_NoError);
	   else
		return (ERR_AllocateMemory);
      }
   }
   else
       return ERR_BoardNoInit;
}

int  CanStopReceiveWakeUpEvent(int handle)
{
    U16 enable_flag = FALSE;
    int ret_value=0;

    if(pci_Info_7841[handle/2].initFlag == FALSE)
	return ERR_BoardNoInit;
    if (InstallRef[handle] != 0 )
    {
	if ((handle%2) == 0)  {
	   ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL5_SET, &enable_flag);
	   pci_Info_7841[handle/2].sig_actWakeUp1.sa_handler = SIG_IGN;
	   if( sigaction( SIGEVENT5, &(pci_Info_7841[handle/2].sig_actWakeUp1), NULL ) ==-1 )
		return(ERR_SignalNotify);
	}
	if ((handle%2) == 1) {
	   ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL11_SET, &enable_flag);
	   pci_Info_7841[handle/2].sig_actWakeUp2.sa_handler = SIG_IGN;
	   if( sigaction( SIGEVENT11, &(pci_Info_7841[handle/2].sig_actWakeUp2), NULL ) ==-1 )
		return(ERR_SignalNotify);
       }
       if( ret_value == 0 )
	   return(ERR_NoError);
       else
	   return (ERR_AllocateMemory);
    }
    else
	return ERR_BoardNoInit;
}

int  CanGetReceiveErrorPassiveEvent(int handle, void (*event_handler)(int))
{
    U16 enable_flag = TRUE;
    int ret_value=0;

    if(pci_Info_7841[handle/2].initFlag == FALSE)
	return ERR_BoardNoInit;
    if (InstallRef[handle] != 0 ) {
        if( pci_Info_7841[handle/2].Owner_Set == 0)
	    return ERR_OwnerSet; // check the owner setting
        if( event_handler == NULL )
	    return (ERR_SignalHandle);
        else {
	    if ((handle%2) == 0) {
	        pci_Info_7841[handle/2].sig_actPassiveEvent1.sa_handler = event_handler;
	        pci_Info_7841[handle/2].sig_actPassiveEvent1.sa_flags |= SA_RESTART;
	        if( sigaction( SIGEVENT6, &(pci_Info_7841[handle/2].sig_actPassiveEvent1), NULL ) == -1 )
		    return(ERR_SignalNotify);
	        ret_value = ioctl(pci_Info_7841[handle/2].DeviceID,IOCTL_7841_SIGNAL6_SET,&(enable_flag));
	    }
	    if ((handle%2) == 1){
	        pci_Info_7841[handle/2].sig_actPassiveEvent2.sa_handler = event_handler;
	        pci_Info_7841[handle/2].sig_actPassiveEvent2.sa_flags |= SA_RESTART;
	        if( sigaction( SIGEVENT12, &(pci_Info_7841[handle/2].sig_actPassiveEvent2), NULL ) == -1 )
	            return(ERR_SignalNotify);
	       ret_value = ioctl(pci_Info_7841[handle/2].DeviceID,IOCTL_7841_SIGNAL12_SET,&(enable_flag));
	    }
	    if( ret_value == 0 )
	       return(ERR_NoError);
	    else
	       return (ERR_AllocateMemory);
        }
    }
    else
	return ERR_BoardNoInit;
}

int  CanStopReceiveErrorPassiveEvent(int handle)
{
    U16 enable_flag = FALSE;
    int ret_value=0;

    if(pci_Info_7841[handle/2].initFlag == FALSE)
	return ERR_BoardNoInit;
    if (InstallRef[handle] != 0 ) {
    	if ((handle%2) == 0) {
	    ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL6_SET, &enable_flag);
	    pci_Info_7841[handle/2].sig_actPassiveEvent1.sa_handler = SIG_IGN;
	    if( sigaction( SIGEVENT6, &(pci_Info_7841[handle/2].sig_actPassiveEvent1), NULL ) ==-1 )
		return(ERR_SignalNotify);
	}
	if ((handle%2) == 1) {
	    ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL12_SET, &enable_flag);
	    pci_Info_7841[handle/2].sig_actPassiveEvent2.sa_handler = SIG_IGN;
	    if( sigaction( SIGEVENT12, &(pci_Info_7841[handle/2].sig_actPassiveEvent2), NULL ) ==-1 )
		return(ERR_SignalNotify);
	}
        if( ret_value == 0 )
	   return(ERR_NoError);
        else
	   return (ERR_AllocateMemory);
    }
    else
	return ERR_BoardNoInit;
}


int  CanGetReceiveArbitrationLostEvent(int handle, void (*event_handler)(int))
{
    U16 enable_flag = TRUE;
    int ret_value=0;

    if(pci_Info_7841[handle/2].initFlag == FALSE)
	return ERR_BoardNoInit;
    if (InstallRef[handle] != 0 ) {
	if( pci_Info_7841[handle/2].Owner_Set == 0)
	    return ERR_OwnerSet; // check the owner setting
	if( event_handler == NULL )
	    return (ERR_SignalHandle);
	else{
	    if ((handle%2) == 0) {
	        pci_Info_7841[handle/2].sig_actArbitrationLost1.sa_handler = event_handler;
		pci_Info_7841[handle/2].sig_actArbitrationLost1.sa_flags |= SA_RESTART;
		if( sigaction( SIGEVENT7,&(pci_Info_7841[handle/2].sig_actArbitrationLost1), NULL ) == -1 )
		    return(ERR_SignalNotify);
		ret_value = ioctl(pci_Info_7841[handle/2].DeviceID,IOCTL_7841_SIGNAL7_SET,&(enable_flag));
	    }
	    if ((handle%2) == 1){
		pci_Info_7841[handle/2].sig_actArbitrationLost2.sa_handler = event_handler;
		pci_Info_7841[handle/2].sig_actArbitrationLost2.sa_flags |= SA_RESTART;
		if(sigaction( SIGEVENT13,&(pci_Info_7841[handle/2].sig_actArbitrationLost2),NULL ) == -1 )
		    return(ERR_SignalNotify);
		ret_value =ioctl(pci_Info_7841[handle/2].DeviceID,IOCTL_7841_SIGNAL13_SET,&(enable_flag));
	    }
	    if( ret_value == 0 )
	        return(ERR_NoError);
	    else
		return (ERR_AllocateMemory);
	}
   }
   else
      return ERR_BoardNoInit;
}

int  CanStopReceiveArbitrationLostEvent(int handle)
{
    U16 enable_flag = FALSE;
    int ret_value=0;

    if(pci_Info_7841[handle/2].initFlag == FALSE)
	return ERR_BoardNoInit;
    if (InstallRef[handle] != 0 ) {
	if ((handle%2) == 0) {
	    ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL7_SET, &enable_flag);
	    pci_Info_7841[handle/2].sig_actArbitrationLost1.sa_handler = SIG_IGN;
	    if( sigaction( SIGEVENT7, &(pci_Info_7841[handle/2].sig_actArbitrationLost1), NULL ) ==-1 )
	        return(ERR_SignalNotify);
        }
	if ((handle%2) == 1) {
	    ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL13_SET, &enable_flag);
	    pci_Info_7841[handle/2].sig_actArbitrationLost2.sa_handler = SIG_IGN;
	    if( sigaction( SIGEVENT13, &(pci_Info_7841[handle/2].sig_actArbitrationLost2), NULL ) ==-1 )
		return(ERR_SignalNotify);
	}
        if( ret_value == 0 )
	    return(ERR_NoError);
        else
	    return (ERR_AllocateMemory);
    }
    else
	return ERR_BoardNoInit;
}

int  CanGetReceiveBusErrorEvent(int handle, void (*event_handler)(int))
{
    U16 enable_flag = TRUE;
    int ret_value=0;

    if(pci_Info_7841[handle/2].initFlag == FALSE)
	return ERR_BoardNoInit;
    if (InstallRef[handle] != 0 ) {
	if( pci_Info_7841[handle/2].Owner_Set == 0)
	    return ERR_OwnerSet; // check the owner setting
	if( event_handler == NULL )
	    return (ERR_SignalHandle);
	else{
	    if ((handle%2) == 0) {
		pci_Info_7841[handle/2].sig_actBusError1.sa_handler = event_handler;
		pci_Info_7841[handle/2].sig_actBusError1.sa_flags |= SA_RESTART;
		if( sigaction( SIGEVENT8, &(pci_Info_7841[handle/2].sig_actBusError1), NULL ) == -1 )
			return(ERR_SignalNotify);
		ret_value = ioctl(pci_Info_7841[handle/2].DeviceID,IOCTL_7841_SIGNAL8_SET,&(enable_flag));
	    }
	    if ((handle%2) == 1) {
		pci_Info_7841[handle/2].sig_actBusError2.sa_handler = event_handler;
		pci_Info_7841[handle/2].sig_actBusError2.sa_flags |= SA_RESTART;
		if( sigaction( SIGEVENT14, &(pci_Info_7841[handle/2].sig_actBusError2), NULL ) == -1 )
			return(ERR_SignalNotify);
		ret_value =ioctl(pci_Info_7841[handle/2].DeviceID,IOCTL_7841_SIGNAL14_SET,&(enable_flag));
	    }
	    if( ret_value == 0 )
		return(ERR_NoError);
	    else
		return (ERR_AllocateMemory);
	}
   }
   else
	return ERR_BoardNoInit;
}

int  CanStopReceiveBusErrorEvent(int handle)
{
    U16 enable_flag = FALSE;
    int ret_value=0;

    if(pci_Info_7841[handle/2].initFlag == FALSE)
	return ERR_BoardNoInit;
    if (InstallRef[handle] != 0 ) {
	if ((handle%2) == 0) {
	    ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL8_SET, &enable_flag);
	    pci_Info_7841[handle/2].sig_actBusError1.sa_handler = SIG_IGN;
	    if( sigaction( SIGEVENT8, &(pci_Info_7841[handle/2].sig_actBusError1), NULL ) ==-1 )
		return(ERR_SignalNotify);
	}
	if ((handle%2) == 1) {
	    ret_value = ioctl(pci_Info_7841[handle/2].DeviceID, IOCTL_7841_SIGNAL13_SET, &enable_flag);
	    pci_Info_7841[handle/2].sig_actBusError2.sa_handler = SIG_IGN;
	    if( sigaction( SIGEVENT13, &(pci_Info_7841[handle/2].sig_actBusError2), NULL ) ==-1 )
		return(ERR_SignalNotify);
	}
        if( ret_value == 0 )
	    return(ERR_NoError);
        else
	    return (ERR_AllocateMemory);
    }
    else
        return ERR_BoardNoInit;
}
*/

