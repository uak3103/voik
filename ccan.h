#ifndef CCAN_H
#define CCAN_H

typedef struct
{
    u_int32_t id;       //  CAN id
    u_char flags;           //  RTR bit
    u_char length;           //  Data length
    u_char data[8];       //  Data
} MY_CAN_MSG;

class CCan
{
	public:
		CCan();
		virtual ~CCan();
		bool Open(int nType, int nPort, int nSpeed);
		void Close();
		bool RcvMsg(MY_CAN_MSG* msg);
		bool SendMsg(MY_CAN_MSG* msg);
		int GetErrorCode();
		bool IsOpen();
	protected:
		int m_hCan;
		int m_nType;
	private:
};

#endif // CCAN_H
