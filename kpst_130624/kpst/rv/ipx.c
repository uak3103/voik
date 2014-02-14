#include <sys/socket.h>
#include <netinet/in.h>
#include <netipx/ipx.h>

int ipx_init_flag=0;

extern int num_komplex;

#define LLL 512

sockaddr_ipx my_adr={AF_IPX,0,0,{0x00,0x00,0x00,0x00,0x00,0x00}};
sockaddr_ipx   Cadr={AF_IPX,0,0,{0x00,0x00,0x00,0x00,0x00,0x00}};

sockaddr_ipx maddr[16];

int  sock;
int  my_port; 
char m_recv[LLL];

typedef void (*cback_t)(int,char*,int);

int     MyNST=-1;
cback_t MyREAC=NULL;

/**********************************************************************/
void IPX_Done(void)
{ 
}

void* thr_recv_ipx(void *param)
{
  socklen_t lll;
  int len,N,i;
  
  for (;;)
  {
    lll=sizeof(sockaddr);
    len=recvfrom(sock,m_recv,LLL,0,(sockaddr*)&Cadr,&lll);
    if (len<=0) break;

    N=-1;    
    for(i=0;i<16;i++) 
      if (maddr[i].sipx_port==Cadr.sipx_port) { N=i; break; }

    if (N!=-1 && MyREAC!=NULL) 
      MyREAC(N,m_recv,len);
      
  }
  return 0;
}

/**********************************************************************/
int IPX_Init(int NST,cback_t proc)
{ 
  int res,port,TTT,i;
  pthread_t id_recv;

 if (ipx_init_flag==1) return 1;
 
  port=0x5400+256*(num_komplex-1)+NST;
  
  my_adr.sipx_port=htons(port);
  
  for(i=0;i<16;i++) {

    if (i==0) 
      port=0x2706;
    else 
      port=0x5400+256*(num_komplex-1)+i;

    memcpy(&maddr[i],&my_adr,sizeof(my_adr));    
    maddr[i].sipx_port = htons(port);
    memset(maddr[i].sipx_node,0xFF,6);
  }
  
  sock=socket(AF_IPX,SOCK_DGRAM,0);
  if (sock<0) { perror("socket"); return 0; }
  
  res=bind(sock,(sockaddr*)&my_adr,sizeof(my_adr));
  if (res<0) { 
    perror("bind"); 
    printf("Не установлен протокол IPX! Обмен с КП54 невозможен!\n");
    return 0;
  }

  TTT=true;
  res=setsockopt(sock,SOL_SOCKET,SO_BROADCAST,(void*)&TTT,sizeof(int));
  if (res<0) { perror("setsockopt"); return 0; }

  pthread_create(&id_recv,0,&thr_recv_ipx,&sock);

  MyNST=NST;MyREAC=proc;
  
  atexit(IPX_Done);
  ipx_init_flag=1;

  return 1;
}

/**********************************************************************/
int IPX_Send(int NST,void *mas,int len)
{ // SEND PACKET
  int res;
  res=sendto(sock,mas,len,0,(sockaddr*)&maddr[NST],sizeof(sockaddr_ipx));
  return res;
}
/**********************************************************************/

