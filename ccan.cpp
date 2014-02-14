#include "wx_pch.h"
#include "ccan.h"
#include "pci_7841.h"
#include "commdef.h"
#include "can4linux.h"

int set_bitrate(
	int can_fd, /* device descriptor */
	int baud    /* bit rate */
)
{
	Config_par_t  cfg;
	volatile Command_par_t cmd;

	cmd.cmd = CMD_STOP;
	ioctl(can_fd, CAN_IOCTL_COMMAND, &cmd);

	cfg.target = CONF_TIMING;
	cfg.val1   = baud;
	ioctl(can_fd, CAN_IOCTL_CONFIG, &cfg);

	cmd.cmd = CMD_START;
	ioctl(can_fd, CAN_IOCTL_COMMAND, &cmd);
	return 0;
}


CCan::CCan()
{
	m_hCan=-1;
	m_nType=0;
}

CCan::~CCan()
{
	Close();
}

bool CCan::Open(int nType, int nPort, int nSpeed)
{
	Close();
	m_nType=nType;
	switch(m_nType)
	{
	case 1:
		{
			char device[40];
			sprintf(device, "/dev/can%d", nPort-1);
			m_hCan=open(device,O_RDWR|O_NONBLOCK);
			if(m_hCan!=-1)
			{
				set_bitrate(m_hCan,nSpeed==0?125:nSpeed==1?250:500);
				return true;
			}
		}
		break;
	case 2:
		{
			m_hCan=CanOpenDriver(0,nPort-1);
			if(m_hCan!=-1)
			{
				PORT_STATUS ps;
				CanGetPortStatus(m_hCan,&ps);
				ps.port.baudrate=nSpeed;
				CanConfigPort(m_hCan,&ps.port);
				return true;
			}
		}
		break;
	}
	return false;
}

void CCan::Close()
{
	if(m_hCan!=-1)
	{
		switch(m_nType)
		{
		case 1:
			close(m_hCan);
			break;
		case 2:
			CanCloseDriver(m_hCan);
			break;
		}
		m_hCan=-1;
	}
}

bool CCan::RcvMsg(MY_CAN_MSG* msg)
{
	if(m_hCan==-1 || m_nType==0)
		return 0;
	switch(m_nType)
	{
	case 1:
		{
			canmsg_t can_msg;
			memset(&can_msg,0,sizeof(can_msg));
			if(read(m_hCan,&can_msg,1)>=1)
			{
				msg->id=can_msg.id;
				msg->flags=can_msg.flags;
				msg->length=can_msg.length;
				memcpy(msg->data,can_msg.data,8);
				return true;
			}
		}
		break;
	case 2:
		{
			CAN_PACKET can_msg;
			memset(&can_msg,0,sizeof(can_msg));
			if(CanRcvMsg(m_hCan, &can_msg)==0)
			{
				msg->id=can_msg.CAN_ID;
				msg->flags=can_msg.rtr;
				msg->length=can_msg.len;
				memcpy(msg->data,can_msg.data,8);
				CanClearOverrun(m_hCan);
				CanClearRxBuffer(m_hCan);
				return true;
			}
		}
		break;
	}
	return false;
}

bool CCan::SendMsg(MY_CAN_MSG* msg)
{
	if(m_hCan==-1 || m_nType==0)
		return 0;
	switch(m_nType)
	{
	case 1:
		{
			canmsg_t can_msg;
			memset(&can_msg,0,sizeof(can_msg));
			can_msg.id=msg->id;
			can_msg.flags=msg->flags;
			can_msg.length=msg->length;
			memcpy(can_msg.data,msg->data,8);
			return write(m_hCan,&can_msg,1)>0;
		}
		break;
	case 2:
		{
			CAN_PACKET can_msg;
			memset(&can_msg,0,sizeof(can_msg));
			can_msg.CAN_ID = msg->id;
			can_msg.rtr = msg->flags;
			can_msg.len = msg->length;
			memcpy(can_msg.data,msg->data,8);
			CanClearOverrun(m_hCan);
			CanClearTxBuffer(m_hCan);
			return CanSendMsg(m_hCan, &can_msg)==0;
		}
		break;
	}
	return false;
}

int CCan::GetErrorCode()
{
	if(m_hCan==-1 || m_nType==0)
		return 0;
	if(m_nType==1)
		return errno;
	else if(m_nType==2)
		return CanGetErrorCode(m_hCan);
	return 0;
}

bool CCan::IsOpen()
{
	return m_hCan!=-1;
}
