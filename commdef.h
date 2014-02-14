#ifndef         _COMMDEF_H_
#define         _COMMDEF_H_

#include <linux/ioctl.h>

/****************************************************************************/
/*      Constant Definitions                                                */
/****************************************************************************/
#define         PCI_7841        0x7841
#define         PCL_CS0         0x00
#define         PCL_CS1         0x80

/****************************************************************************/
/*      Typedef  Definitions                                                */
/****************************************************************************/
#ifndef basic_type
#define basic_type
typedef unsigned char   U8;
typedef short           I16;
typedef unsigned short  U16;
typedef long            I32;
typedef unsigned long   U32;
typedef float           F32;
typedef double          F64;

//typedef enum { FALSE, TRUE } BOOLEAN;
#define FIRSTBYTE(VALUE)  (VALUE & 0x00FF)
#define SECONDBYTE(VALUE) ((VALUE >> 8) & 0x00FF)
#define THIRDBYTE(VALUE) ((VALUE >> 8) & 0x00FF)
#define FOURTHBYTE(VALUE) ((VALUE >> 8) & 0x00FF)
#endif // basic_type

#define MAGIC_NUM 'S'

#define IOCTL_7841_SIGNAL1_SET _IO(MAGIC_NUM, 5)
#define IOCTL_7841_SIGNAL2_SET _IO(MAGIC_NUM, 6)
//#define IOCTL_7841_SIGNAL3_SET _IO(MAGIC_NUM, 7)
//#define IOCTL_7841_SIGNAL4_SET _IO(MAGIC_NUM, 8)
//#define IOCTL_7841_SIGNAL5_SET _IO(MAGIC_NUM, 9)
//#define IOCTL_7841_SIGNAL6_SET _IO(MAGIC_NUM, 10)
//#define IOCTL_7841_SIGNAL7_SET _IO(MAGIC_NUM, 11)
//#define IOCTL_7841_SIGNAL8_SET _IO(MAGIC_NUM, 12)
//#define IOCTL_7841_SIGNAL9_SET _IO(MAGIC_NUM, 13)
//#define IOCTL_7841_SIGNAL10_SET _IO(MAGIC_NUM, 14)
//#define IOCTL_7841_SIGNAL11_SET _IO(MAGIC_NUM, 15)
//#define IOCTL_7841_SIGNAL12_SET _IO(MAGIC_NUM, 16)
//#define IOCTL_7841_SIGNAL13_SET _IO(MAGIC_NUM, 17)
//#define IOCTL_7841_SIGNAL14_SET _IO(MAGIC_NUM, 18)
#define IOCTL_7841_CLOSE_PORT _IOW(MAGIC_NUM, 19, REG_RW)
#define IOCTL_7841_CLOSE_CARD _IOW(MAGIC_NUM, 20, REG_RW)
#define IOCTL_7841_SET_PORT_MODE _IOW(MAGIC_NUM, 21, RETINFO)
#define IOCTL_7841_REG_READ _IOR(MAGIC_NUM, 22, REG_RW)
#define IOCTL_7841_REG_WRITE _IOW(MAGIC_NUM, 23, REG_RW)
#define IOCTL_7841_SET_BUFFER_DATA _IOW(MAGIC_NUM, 24, CAN_MSG)
#define IOCTL_7841_GET_BUFFER_DATA _IOR(MAGIC_NUM, 25, CAN_MSG)
#define IOCTL_7841_GET_UNRD_COUNT _IOR(MAGIC_NUM, 26, REC_CN)
#define IOCTL_7841_CLEAR_RX_BUFFER _IOW(MAGIC_NUM, 27, int)     // 2008/06/03
#define IOCTL_7841_CLEAR_TX_BUFFER _IOW(MAGIC_NUM, 28, int)     // 2008/06/03

#define MAX_PCI_CARDS 	4
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

#endif








