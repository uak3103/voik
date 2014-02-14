//-----------------------
#define dTimeVar 1.0
#define Niteration 500

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/ioctl.h> 
#include <time.h> 

#include "sys/sys.h"

#include "rv/rv.h"
//#include "vu/vu.h"
//#include "apoi/apoi.h"
//#include "csn/csn.h"
//#include "inter.h"
//#include "globst.h"

#include "st.h"
//#include "ki/ki.h"
#include "csn/csn.h"
#include "../common/elements.h"

#include <ctype.h>
//#include <conio.h>

#include "mycons.h" 

//----------------------------------------------------------------------------------
extern int IMIT;   
// ������ ��������� (CAN ��� ��)
   struct stLRcan mmCanKW[10][3]; // �������/��������
   struct prLRcan prCanKW[10][3]; // ��������� ��������� ��� ��� !?     
//----------------------------------------------------------------------------------

#define nnSwEnd 10      // ����������� ������� �������� �� ����� ����������
#define nSwEnd   2      // �������� ����������� ��
#define EpsKW    0.2    // ���������� �� ��������� ��

//#define EpsOS  0.01;  // ���������� �� ��������� �� - [���.����]
#define kw_A0    0.0    // �������� ������ ������� ��� ������������ ��� - [���.����]
#define kw_L0   90.0    // �������� ������ ������� ��� ��������     ��� - [���.����]
#define kw_R0   90.0    // �������� ������ ������� ��� �����������  ��� - [���.����]

#define AminKW  -220.0  // ����������� �� ��� <A> �����  [���.����]
#define AmaxKW   220.0  // ����������� �� ��� <A> ������ [���.����]

#define LminKW     1.0  // ����������� �� ��� <L> �����  [���.����]
#define LmaxKW   170.0  // ����������� �� ��� <L> ������ [���.����]

#define RminKW    1.0   // ����������� �� ��� <R> �����  [���.����]
#define RmaxKW   170.0  // ����������� �� ��� <R> ������ [���.����]
//----------------------------------------------------------------------------------


// ������� ������ ���� � ��������� �� ��� ���� ���� 
// (������������� ��������� ������� ��� ������� mmCanKW)       
   void SendKdpoKW(double  dtt){
//    if (!IMIT){
             printf(" ������..? ����������� ��������� � �������� � ��������� �� (����)\n");
             printf("           �����...\n");                                          
          // printf(" ------------------------------------------------------------------------\n");             

          // ������������ ���
          // ======================================================                 
             printf(" ========================================================================\n");                                    
          // ��������� �� 1-�� ������ (������)
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR1AA);                  
             usleep(dtt*1000000);                                            
             printf(" OK! ====> ������������ ��� - ������ 1-�� ������\n");
          // -------------------      
          // ��������� �� 1-�� ������ (�����)       
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR1BA);               
             usleep(dtt*1000000);                                            
             printf(" OK! ====> ������������ ��� - �����  1-�� ������\n");
             
          // printf(" -----------------------------------------------\n");                       
          // ======================================================          
          // ��������� �� 2-�� ������ (������)
          // can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR2AA);     
          // usleep(dtt*1000000);                                            
          // printf(" OK! ====> ������������ ��� - ������ 2-�� ������\n");      
          // -------------------      
          // ��������� �� 2-�� ������ (�����)       
          // can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR2BA);               
          // usleep(dtt*1000000);                                        
          // printf(" OK! ====> ������������ ��� - �����  2-�� ������\n");    
                       
             printf(" -----------------------------------------------\n");             
          // ======================================================          
          // ��������� �� 3-�� ������ (������)
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR3AA);     
             usleep(dtt*1000000);                                            
             printf(" OK! ====> ������������ ��� - ������ 3-�� ������\n");                   
          // -------------------      
          // ��������� �� 3-�� ������ (�����)       
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR3BA);               
             usleep(dtt*1000000);                                            
             printf(" OK! ====> ������������ ��� - �����  3-�� ������\n");                 
          // ======================================================                              
          
          // �������� ���
          // ======================================================                 
             printf(" ========================================================================\n");                                    
          // ��������� �� 1-�� ������ (������)
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR1AL);     
             usleep(dtt*1000000);                                            
             printf(" OK! ====> ��������     ��� - ������ 1-�� ������\n");                                
          // -------------------      
          // ��������� �� 1-�� ������ (�����)       
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR1BL);               
             usleep(dtt*1000000);                                            
             printf(" OK! ====> ��������     ��� - �����  1-�� ������\n");                 
             
          // printf(" -----------------------------------------------\n");                       
          // ======================================================          
          // ��������� �� 2-�� ������ (������)
          // can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR2AL);     
          // usleep(dtt*1000000);                                            
          // printf(" OK! ====> ��������     ��� - ������ 2-�� ������\n");                                          
          // -------------------      
          // ��������� �� 2-�� ������ (�����)        
          // can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR2BL);               
          // usleep(dtt*1000000);                                            
          // printf(" OK! ====> ��������     ��� - �����  2-�� ������\n");                           
          
             printf(" -----------------------------------------------\n");                                    
          // ======================================================          
          // ��������� �� 3-�� ������ (������)
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR3AL);     
             usleep(dtt*1000000);                                            
             printf(" OK! ====> ��������     ��� - ������ 3-�� ������\n");                                             
          // -------------------      
          // ��������� �� 3-�� ������ (�����)      
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR3AL);               
             usleep(dtt*1000000);                                            
             printf(" OK! ====> ��������     ��� - �����  3-�� ������\n");                              
          // ======================================================                              
          
          // ����������� ���
             printf(" ========================================================================\n");                                    
          // ======================================================                 
          // ��������� �� 1-�� ������ (������)
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR1AR);     
             usleep(dtt*1000000);                                            
             printf(" OK! ====> �����������  ��� - ������ 1-�� ������\n");                                                          
          // -------------------      
          // ��������� �� 1-�� ������ (�����)       
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR1BR);               
             usleep(dtt*1000000);                                            
             printf(" OK! ====> �����������  ��� - �����  1-�� ������\n");                                                                                    
             
          // printf(" -----------------------------------------------\n");                                              
          // ======================================================          
          // ��������� �� 2-�� ������ (������)
          // can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR2AR);     
          // usleep(dtt*1000000);                                            
          // printf(" OK! ====> �����������  ��� - ������ 2-�� ������\n");                                                                    
          // -------------------      
          // ��������� �� 2-�� ������ (�����)        
          // can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR2BR);               
          // usleep(dtt*1000000);                                            
          // printf(" OK! ====> �����������  ��� - �����  2-�� ������\n");                                                                    
          
             printf(" -----------------------------------------------\n");             
          // ======================================================          
          // ��������� �� 3-�� ������ (������)
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR3AR);     
             usleep(dtt*1000000);                                            
             printf(" OK! ====> �����������  ��� - ������ 3-�� ������\n");                                                                                 
          // -------------------      
          // ��������� �� 3-�� ������ (�����)      
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR3BR);               
             usleep(dtt*1000000);                                            
             printf(" OK! ====> �����������  ��� - �����  3-�� ������\n");                                                                                 
          // ======================================================                              
//        };//if
          printf(" ========================================================================\n");          
       // printf(" ------------------------------------------------------------------------\n");                       
   };// ������� ������ ���� � ��������� �� ��� ���� ����           

// ����� �������� �������� ��������� ��� ���� ���� ���
   void OutDelWork(int nInpKW,struct stDir mDataKW[][3],struct stLRc mConfKW[][3]){
      int ikw,jkw;
      double UgMin,UgMax,DelWork;       
      printf(" ����� �������� ��� �������� ��������� ���� ��� j\n");       
      printf(" ========================================================================\n");                                     
      for (jkw=0; jkw<3; jkw++){       
          switch (jkw){
                case 0:printf(" # ������������ ��� <A>: \n");break;
                case 1:printf(" # ��������     ��� <L>: \n");break;
                case 2:printf(" # �����������  ��� <R>: \n");break;
          };//switch                                        
          for (ikw=0; ikw<nInpKW; ikw++){

              if ((mConfKW[ikw][jkw].prL)&&(mConfKW[ikw][jkw].prR)){
                 UgMin=mDataKW[ikw][jkw].stDirL.val;
                 UgMax=mDataKW[ikw][jkw].stDirR.val;              
                 DelWork=UgMax-UgMin;
                 printf("  * ��� %d-������ ��: del=%+011.6f; min=%+011.6f; max=%+011.6f\n",ikw,DelWork,UgMin,UgMax);          
              };//if                 
              
          };//ikw
          if (jkw<2) printf(" ------------------------------------------------------------------------\n");                       
      };//jkw             
      printf(" ========================================================================\n");                              
   };//OutDelWork() - ����� �������� �������� ��������� ��� ���� ���� ���

// ������������ ����� �� - <DataBF.txt>    
   void FileDataBF(int nInpKW,struct stDir mDataBuf[][3]){
      char sss[256];
      int ikw,jkw;
      FILE *f_DataBF;
      printf(" ------------------------------------------------------------------------\n");              
      printf(" ������������ ����� �� - <DataBF.txt> ? (y->��;n->���)\n");
      printf(" => "); gets(sss); 
      if ((sss[0]=='y')||(sss[0]=='Y')){
      // ������� ����                
         f_DataBF=fopen("DataBF.txt","w");               

        for (ikw=0; ikw<nInpKW; ikw++){
           for (jkw=0; jkw<3; jkw++){              
            // ������ � ����
            // -------------------------------------
               fprintf(f_DataBF,"%20.10f",mDataBuf[ikw][jkw].stDirL.val);   
               fprintf(f_DataBF,"%c",' ');   
            // ----------------------------------- 
               fprintf(f_DataBF,"%20.10f",mDataBuf[ikw][jkw].stDirL.cko);   
               fprintf(f_DataBF,"%c",' ');   
            // ----------------------------------- 
               fprintf(f_DataBF,"%20.10f",mDataBuf[ikw][jkw].stDirR.val);   
               fprintf(f_DataBF,"%c",' ');   
            // ----------------------------------- 
               fprintf(f_DataBF,"%20.10f",mDataBuf[ikw][jkw].stDirR.cko);   
               fprintf(f_DataBF,"%c",' ');   
            // ----------------------------------- 
           };//jkw
           fprintf(f_DataBF,"%c",'\n');                 
         };//ikw                    
         fclose(f_DataBF);                                          
         printf(" ��������� ������������ ����� �� - <DataBF.txt>\n");
      };//if (sss)
   };//FileDataBF() ������������ ����� �� - <DataBF.txt>
            

// ��������� (�������� ���������) ��������� CAN ��� ��
   void StatCanKW(int nInpKW,struct stDir mDataKW[][3],struct stLRcan mmCanKW[][3]){       
       int ikw,ikk,jkk;
       double arc_OS;
       for (ikw=0; ikw<nInpKW; ikw++){
          for (ikk=0; ikk<3; ikk++){
             for (jkk=0; jkk<2; jkk++){                
                if (jkk==0) arc_OS=mDataKW[ikw][ikk].stDirL.val;
                else        arc_OS=mDataKW[ikw][ikk].stDirR.val;  
             // printf(" nKW= %d; nOs= %d; nLR= %d;\n",ikw,ikk,jkk);
             // printf(" arc_OS= %f; cd[]= %f;\n",arc_OS,cd[ikk]);                                                       
                if ( (((jkk==0)&&(cd[ikk]<=arc_OS))||((jkk==1)&&(cd[ikk]>=arc_OS))) 
                   ) mmCanKW[ikw][ikk].mLR[jkk]=1;
                else mmCanKW[ikw][ikk].mLR[jkk]=0;                      
             };//jkk
          };//ikk
       // printf(" # NEXT (������� <����>) ==> "); gets(sss);                       
       };//ikw                           
   };//StatCanKW() - ��������� (�������� ���������) ��������� CAN ��� ��
   
// �������������� � ����������� ������� ��������� ������� ��������� �� CAN
   void ScrCanKW(bool pr,int nInpKW,struct stLRcan mmCanKW[][3],struct prLRcan prCanKW[][3]){
      int ikw,jkw;
      if (pr){
         for (ikw=0; ikw<nInpKW; ikw++){
     
            for (jkw=0; jkw<3; jkw++){
               mmCanKW[ikw][jkw].mLR[0]=-1;
               mmCanKW[ikw][jkw].mLR[1]=-1;
               
               prCanKW[ikw][jkw].mLR[0]=-1;
               prCanKW[ikw][jkw].mLR[1]=-1;
               
            };//jkw
         };//ikw
      };//if
              printf(" ------------------------------------------------------------------------\n");
      if (pr) printf(" ��������: ��������� �������� �� ��������� ('0')\n");
                                         
              printf(" ��������: �������� ����������� ���� CAN (��������� ��)\n");
             
      printf(" ------------------------------------------------------------------------\n");
        
      printf("           :---------------------------------: \n");
                                        
      printf("           :   :    A    :    L    :    R    : \n");
      
      printf("           : N :---------:---------:---------: \n");
                     
      printf("           :   : -  : +  : -  : +  : -  : +  : \n");
                           
      ikw=0;
           
      while (ikw<nInpKW){     
         printf("           :---------------------------------:\n");                                
         printf("           : %d : %+d   %+d : %+d   %+d : %+d   %+d :\n",
         ikw+1,
         mmCanKW[ikw][0].mLR[0],mmCanKW[ikw][0].mLR[1],
         mmCanKW[ikw][1].mLR[0],mmCanKW[ikw][1].mLR[1],
         mmCanKW[ikw][2].mLR[0],mmCanKW[ikw][2].mLR[1]);
         ikw=ikw+1;
      };//ikw
      printf("           :---------------------------------:\n");
                                
      printf(" ------------------------------------------------------------------------\n");
                
      if (pr) printf(" OK! => ��������� ��������� �������� �� ���������\n"); 
      if (pr) printf(" ------------------------------------------------------------------------\n");
   };// �������������� � ����������� ������� ��������� ������� ��������� �� CAN

// ����������� ���������-������ ������ ��� �� �� �����
   void ScrDataKW(int nInpKW, struct stDir mDataBuf[][3]){
        int ikw;    
        char sss[256];
//        printf(" ------------------------------------------------------------------------\n");              
//        printf(" ����������� ���������-������ ������ ��� �� �� ����� ? (y->��;n->���)\n");
//        printf(" => "); gets(sss); 
//        if ((sss[0]=='y')||(sss[0]=='Y')){
           for (ikw=0; ikw<nInpKW; ikw++){
             printf(" ======:========================================================:\n");  
             printf(" N��=%02d:",ikw+1);             
             printf("  ����� (-)   : cko         : ������ (+)  : cko         :\n");
             printf(" ------:--------------------------------------------------------:\n");                  
             printf("  A    :  %+011.6f : %+011.6f : %+011.6f : %+011.6f :\n",
             mDataBuf[ikw][0].stDirL.val,
             mDataBuf[ikw][0].stDirL.cko,
             mDataBuf[ikw][0].stDirR.val,  
             mDataBuf[ikw][0].stDirR.cko );
             
             printf("  L    :  %+011.6f : %+011.6f : %+011.6f : %+011.6f :\n",
             mDataBuf[ikw][1].stDirL.val,
             mDataBuf[ikw][1].stDirL.cko,
             mDataBuf[ikw][1].stDirR.val,  
             mDataBuf[ikw][1].stDirR.cko );
             
             
             printf("  R    :  %+011.6f : %+011.6f : %+011.6f : %+011.6f :\n",
             mDataBuf[ikw][2].stDirL.val,
             mDataBuf[ikw][2].stDirL.cko,
             mDataBuf[ikw][2].stDirR.val,  
             mDataBuf[ikw][2].stDirR.cko );
             
           };//ikw
           printf(" ======:========================================================:\n");  
        
//        };//if     
   };// ����������� ���������-������ ������ ��� �� �� �����

// ��������� ��������
   void my_delay(int delta){
     long start,end,ltime;
     int n;
   //char P_txt[255];
     start=time(&ltime);
     end  = start+delta;
     while (time(&ltime)<end){ 
       n=rand();
     };
   }//my_delay

// ���������� (���)
   double my_random(){
      double rnd;  do {rnd=rand()*0.0001;} while(rnd>1);
      return rnd;
   }   
   double my_random1(double x0,double CKOx){
      double x,rnd;  
 
      do {rnd=rand()*0.0001;} while(rnd>1);
      x = x0 + CKOx*(2*rnd-1);
      
      return x;
   }
   double my_gauss(double m0, double sigma){
      double a,b,r,sq;
      do{a=2*my_random()-1;b=2*my_random()-1;r=a*a+b*b;} while(r>=1);
      sq=sqrt(-2*log(r)/r);
      return m0+sigma*a*sq;
   }//my_gauss
   double my_gauss1(double m0, double sigma, int Nsigma){
      double a,b,r,sq,del;
      do{
         do{a=2*my_random()-1;b=2*my_random()-1;r=a*a+b*b;} while(r>=1);
         sq=sqrt(-2*log(r)/r);
         del=sigma*a*sq;
         if (Nsigma==0){break;};
      } while(del>(Nsigma*sigma));
      return m0+del;
   }//my_gauss1
   double my_gauss2(double m0, double sigma1, double sigma2, int Nsigma){
       double a,b,r,sq,gauss,del;
       do{
          do{a=2*my_random()-1;b=2*my_random()-1;r=a*a+b*b;} while(r>=1);
          sq=sqrt(-2*log(r)/r);
          del=sigma1*a*sq;
          if (Nsigma==0){break;};
       } while(del>(Nsigma*sigma1));
       gauss=del;
       do{
          do{a=2*my_random()-1;b=2*my_random()-1;r=a*a+b*b;} while(r>=1);
          sq=sqrt(-2*log(r)/r);
          del=sigma2*a*sq;
          if (Nsigma==0){break;};
       } while(del>(Nsigma*sigma2));

       gauss=m0+gauss+del;
       return gauss;
   }//my_gauss2

// �������� !!!     
// ��������� ��������� ��� �� ���� ���� (A,L,R) - ��������� ��� ����� ���������
   void IniValOs(int nInpKW,bool *prCP,struct stDir mDataKW[][3],double cd[]){
        double ValOs0,ValOsMax,ValOsMin,DelOs;        
        double ugA_0,ugL_0,ugR_0;
        
        if (!*prCP)  {
           printf(" ------------------------------------------------------------------------\n");             
           printf(" ��������! ��������� ������� �� ��������� ���� ���: {A,L,R} => [���.����]\n");     
           printf("           ����� ��� - <�� ���������>\n");                

        // ��������� �������� ��������� ��� ��� �� �� 3-�� ������ + 3*���           
        // --------------------------------------------------------------
        // ������������ ���
           ValOsMax = mDataKW[nInpKW-1][0].stDirR.val + 3.0*mDataKW[nInpKW-1][0].stDirR.cko;
           ValOsMin = mDataKW[nInpKW-1][0].stDirL.val - 3.0*mDataKW[nInpKW-1][0].stDirL.cko;
           DelOs    = (ValOsMax - ValOsMin)/2.0;
           ValOs0   = ValOsMin  + DelOs;
           ugA_0    = my_random1(ValOs0,DelOs/2.0);                       
           printf(" ���[A]==> Amax=%+007.2f; Amin=%+007.2f; A0=%+007.2f; ^A=%+007.2f;\n", 
                              ValOsMax,ValOsMin,ValOs0,ugA_0);           
        // �������� ���
           ValOsMax = mDataKW[nInpKW-1][1].stDirR.val + 3.0*mDataKW[nInpKW-1][1].stDirR.cko;
           ValOsMin = mDataKW[nInpKW-1][1].stDirL.val - 3.0*mDataKW[nInpKW-1][1].stDirL.cko;
           DelOs    = (ValOsMax - ValOsMin)/2.0;
           ValOs0   = ValOsMin  + DelOs;
           ugL_0    = my_random1(ValOs0,DelOs/2.0);                      
           printf(" ���[L]==> Lmax=%+007.2f; Lmin=%+007.2f; L0=%+007.2f; ^L=%+007.2f;\n", 
                              ValOsMax,ValOsMin,ValOs0,ugL_0);           
        // ����������� ���
           ValOsMax = mDataKW[nInpKW-1][2].stDirR.val + 3.0*mDataKW[nInpKW-1][2].stDirR.cko;
           ValOsMin = mDataKW[nInpKW-1][2].stDirL.val - 3.0*mDataKW[nInpKW-1][2].stDirL.cko;
           DelOs    = (ValOsMax - ValOsMin)/2.0;
           ValOs0   = ValOsMin  + DelOs;
           ugR_0    = my_random1(ValOs0,DelOs/2.0);           
           printf(" ���[R]==> Rmax=%+007.2f; Rmin=%+007.2f; R0=%+007.2f; ^R=%+007.2f;\n", 
                              ValOsMax,ValOsMin,ValOs0,ugR_0);           
        // ���������� ��������� ��� ���           
           printf("           �����... \n");                 
        
           perebroscsn(ugA_0,ugL_0,ugR_0);
        // perebroscsn(-211.0,6.0,178.0);
           RESCSN();
        // printf(" ------------------------------------------------------------------------\n");     
           
           printf(" OK! ====> A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);
        // printf(" ------------------------------------------------------------------------\n");     
        // �������� �������� - ��� ������� �������     
           *prCP=true;
        };//if (prCP)
        
   }//IniValOs


// �������� ������� (��������) ������������ ��
   void PrMsg(char *msg){
      char s[256]=" ��������! ";
      strcat(s,msg);
      printf(" ------------------------------------------------------------------------\n");             
//    printf(" ��������! ��� ���������� ������ ����������� �������� ��\n");
      printf("%s\n",s);
      printf("           ������� � ������ �������� ����� ������...\n");        
      printf(" ------------------------------------------------------------------------\n");
   };//void YesKw()

// �������� ������� (��������) ������������ ��
   void YesKw(){
      printf(" ------------------------------------------------------------------------\n");             
      printf(" ��������! ��� ���������� ������ ����������� �������� ��\n");                                      
      printf("           ��� ����������� ������� ����� ������� ...");        
      printf(" ------------------------------------------------------------------------\n");
   };//void YesKw()
                        
// �������� ������� ����� ���������� "nTek"
   void NotIn(){
      printf(" ------------------------------------------------------------------------\n");     
      printf(" ��������! ������ ����� ������, ������������ ������ �������\n");                                      
      printf("           ��� ����������� ������� ����� ������� ...");        
      
      printf(" ------------------------------------------------------------------------\n");          
   };//PrintErr()

// ���������� � ������ ���������� ������� ( + ���������)
   void SpUchVar(char code_ALR,double EpsUch,double dt,double ug0){
     int i,j;
     char sss[1];
     double v6_ALR[6];
     double DelTek=999.0; // ��... ������ ����� ������� ��������!
     
// printf(" EpsUch=%f; dt=%f; ug0=%f; cd[1]=%f;\n",EpsUch,dt,ug0,cd[1]);     
// printf(" => "); gets(sss);   
  // ��������� ���������� ����������
     for (j=0; j<6; j++) v6_ALR[j]=999.0;       
  // ��������� � �������� ��������������� ������������ ������ ���������� �������
  // �������� �������  
  // printf("\n");
     printf(" ------------------------------------------------------------------------\n");            
     printf(" ��������! ��������� ��� � ������ ��...\n");                     
  // �������� ���� ��� ���            
     switch (code_ALR){     
        case 'A':        
          // ���������� � ������ ���������� ������� (��)
             v6_ALR[0]=ug0;                     
             perebroscsn(v6_ALR[0],v6_ALR[1],v6_ALR[2]);
             RESCSN();
             printf("           A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);                   
             printf(" ------------------------------------------------------------------------\n");                               
             v6_ALR[0]=999.0;
          // �������� ���������������  
             DelTek=(cd[0]-ug0); 
             
          // ������� �� ������ ��������� ��������� ������ ��   
             if (fabs(DelTek)>EpsUch){
                printf(" ��������! ��������������� �� ��������� ������������ ������ ��\n");            
                printf("           ��������� ��������� ���������? (y->��;n->���)\n");             
                printf(" => "); gets(sss); 
                
                if ((sss[0]=='y')||(sss[0]=='Y')){         
                   printf("           �����...\n");
                // ��������� ���������� � ������ "���������� �� ��������"                      
                   v6_ALR[3]=-0.1*DelTek/fabs(DelTek)*EpsUch;//�������� <�������> �� ��������� � ���������������
                   skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);                   
                   i=0;
                   while (fabs(cd[0]-ug0)>EpsUch){
                      i=i+1;                      
//                      skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);
                      usleep(dt*1000000);//[���]
                   // printf("         # i=%d; A=%+10.6f;\n",i,cd[0]);                                                                                 
                   };//while                   
                // printf(" ------------------------------------------------------------------------\n");                                                        
                // ��������� ��� ���
                   skorcsn(0.0,0.0,0.0);
                   printf(" OK! ====> ��������� ������ ��������� ������ ��\n");                      
                }else{
                   printf(" ??? ====> ������ ��������� ������ �� �� �����������...!?\n");                                      
                }                  
             } else printf(" OK! ====> ��������� ��������� ��� ��� � ������ ��\n");
                    printf("           A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);                                      
             break;
                   
        case 'L':         
          // ���������� � ������ ���������� ������� (��)
             v6_ALR[1]=ug0;                     
             perebroscsn(v6_ALR[0],v6_ALR[1],v6_ALR[2]);
             RESCSN();
             printf("           A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);                   
             printf(" ------------------------------------------------------------------------\n");                               
             v6_ALR[1]=999.0;
          // �������� ���������������  
             DelTek=(cd[1]-ug0); 
             
          // ������� �� ������ ��������� ��������� ������ ��   
             if (fabs(DelTek)>EpsUch){
                printf(" ��������! ��������������� �� ��������� ������������ ������ ��\n");            
                printf("           ��������� ��������� ���������? (y->��;n->���)\n");             
                printf(" => "); gets(sss); 
                if ((sss[0]=='y')||(sss[0]=='Y')){                      
                   printf("           �����...\n");
                // ��������� ���������� � ������ "���������� �� ��������"                      
                   v6_ALR[4]=-0.1*DelTek/fabs(DelTek)*EpsUch;//�������� <�������> �� ��������� � ���������������
                   skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);                   
                   i=0;
                   while (fabs(cd[1]-ug0)>EpsUch){
                      i=i+1;                      
//                      skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);
                      usleep(dt*1000000);//[���]
                    printf("         # i=%d; V= %+12.8f; L=%+12.8f;\n",i,v6_ALR[4],cd[1]);                      
                   };//while
                // printf(" ------------------------------------------------------------------------\n");                                                        
                // ��������� ��� ���
                   skorcsn(0.0,0.0,0.0);                
                   printf(" OK! ====> ��������� ������ ��������� ������ ��\n");                      
                }else{
                   printf(" ??? ====> ������ ��������� ������ �� �� �����������...!?\n");                                      
                }                                                  
             } else printf(" OK! ====> ��������� ��������� ��� ��� � ������ ��\n");
                    printf("           A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);                                          
             break;
        case 'R':                    
          // ���������� � ������ ���������� ������� (��)
             v6_ALR[2]=ug0;                     
             perebroscsn(v6_ALR[0],v6_ALR[1],v6_ALR[2]);
             RESCSN();
             printf("           A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);                   
             printf(" ------------------------------------------------------------------------\n");                               
             v6_ALR[2]=999.0;
          // �������� ���������������  
             DelTek=(cd[2]-ug0); 
          // ������� �� ������ ��������� ��������� ������ ��   
             if (fabs(DelTek)>EpsUch){
                printf(" ��������! ��������������� �� ��������� ������������ ������ ��\n");            
                printf("           ��������� ��������� ���������? (y->��;n->���)\n");             
                printf(" => "); gets(sss); 
                if ((sss[0]=='y')||(sss[0]=='Y')){                      
                   printf("           �����...\n");
                // ��������� ���������� � ������ "���������� �� ��������"                      
                   v6_ALR[5]=-0.1*DelTek/fabs(DelTek)*EpsUch;//�������� <�������> �� ��������� � ���������������
                   skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);                   
                   i=0;
                   while (fabs(cd[2]-ug0)>EpsUch){
                      i=i+1;                      
//                      skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);
                      usleep(dt*1000000);//[���]
                   // printf("         # i=%d; R=%+10.6f;\n",i,cd[2]);                      
                   };//while
                // printf(" ------------------------------------------------------------------------\n");                                                        
                // ��������� ��� ���
                   skorcsn(0.0,0.0,0.0);                                
                   printf(" OK! ====> ��������� ������ ��������� ������ ��\n");                      
                }else{
                   printf(" ??? ====> ������ ��������� ������ �� �� �����������...!?\n");                                      
                }                                  
             } else printf(" OK! ====> ��������� ��������� ��� ��� � ������ ��\n");
                    printf("           A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);
             break;                 
     }//switch         
     printf(" ------------------------------------------------------------------------\n");                                                     
        
};//SpUchVar()

// ���������� ���� ������������ ��������� ��� (��� �������� ����� �� ��)      
// ������� ������ ����������� � ���������� ������ �� ������ CAN
   void EventKW(bool  *pr_L0,
                bool  *pr_R0,
                int   *jL,
                int   *jR,
                double arc_OS,
                bool   pStatKW[][2],
                double pDataKW[][4],
                int Nkw){  
     bool pr_L = false;
     bool pr_R = false;      
     int jpr_L = -1; // ��� ������������ ��
     int jpr_R = -1; // ��� ������������ ��     
     int jkw;               
     printf("\n");
     printf("����������� ��������� ��� ������������ ������ �� (");         
  // printf("���.��� = %f",xx);printf(")\n");         
     printf("���.��� = %f",arc_OS);printf(")\n");         
     printf("-----------------------------------------------------------------------\n");         
     for (jkw=0; jkw<Nkw; jkw++){               
     // ��������������� �����
        printf(" # ��� �� %d",jkw+1); printf("-�� ������:\n");                                 
     // --------------------------------------------------------            
        printf("   ������ ������  �� = ");            
        if (pStatKW[jkw][0])  printf("<���������� ��� >");
        else                  printf("<������� ��� ���>");               
        if (arc_OS<=pDataKW[jkw][0]){
           printf(" ��������� ��: <<������������>>\n");        
           if (pStatKW[jkw][0]&&(!pr_L)){pr_L=true;jpr_L=jkw;};
        }
        else printf(" ��������� ��: <<��� ������� >>\n");
     // --------------------------------------------------------            
        printf("   ������ ������� �� = ");            
        if (pStatKW[jkw][1])  printf("<���������� ��� >");
        else                  printf("<������� ��� ���>");
        if (arc_OS>=pDataKW[jkw][2]){
           printf(" ��������� ��: <<������������>>\n");        
           if (pStatKW[jkw][1]&&(!pr_R)){pr_R=true;jpr_R=jkw;};
        }
        else printf(" ��������� ��: <<��� ������� >>\n");
     }//jkw      
     *pr_L0 = pr_L;
     *jL    = jpr_L;    
     *pr_R0 = pr_R;      
 
     *jR    = jpr_R;     
   }//EventKW()     

// ������������� � ������ ������ �� - ConfKW.txt 
   void ReadConf(char *myFile,bool *my_ERR, struct stLRc mConfKW[][3],int *N){
     int  nkw1,nkw2,nkw3,nkw4,nkw5,nkw6;
     int jkw;
     FILE *f_ConfKW;
     f_ConfKW=fopen(myFile,"r");
     if (!f_ConfKW){
        printf(" --------------------------------------------------------------------------\n");  
        printf(" ��������! ����������� ���� �� (������������ ��         ) => <ConfKW.txt>\n");
        printf(" --------------------------------------------------------------------------\n");  
        
        *my_ERR=true;
     } else {
        printf(" --------------------------------------------------------------------------\n");  
        printf(" OK! => ���������   ���� �� (������������ ��         ) => <ConfKW.txt>\n");
        printf("        * ������ �����... ����������� ��������������� ������ ��������...\n");
        printf(" --------------------------------------------------------------------------\n");
     // ������ ����� � ������������ ���������       
        printf(" :========:=======:=======:=======:=======:=======:=======:\n");                  
        printf(" : N��    : az_L  : az_R  : pol_L : pol_R : orb_L : orb_R :\n");
        printf(" :========:=======:=======:=======:=======:=======:=======:\n");     
        jkw=0;
        while (fscanf(f_ConfKW,"%d %d %d %d %d %d\n",&nkw1,&nkw2,&nkw3,&nkw4,&nkw5,&nkw6)!=EOF){
         // �������� ������� ������ (������ - 0;1)        
            if (!(((nkw1==0)||(nkw1==1))&&((nkw2==0)||(nkw2==1))&&
                 ((nkw3==0)||(nkw3==1))&&((nkw4==0)||(nkw4==1))&&
                 ((nkw5==0)||(nkw5==1))&&((nkw6==0)||(nkw6==1)) )){
                printf("\n");printf(" ����������! ������ ������ ������ ������...???\n");
        
                printf("\n");
                *my_ERR=true; break;
            }//if        
         // ������� ���������� �� (����� ����� � <1>)
         // if ( (nkw1==1)||(nkw2==1)||(nkw3==1)||
         //    (nkw4==1)||(nkw5==1)||(nkw6==1) ){
         // // �������     
         //    nConf=nConf+1;
         // }//if
            
            
            mConfKW[jkw][0].prL=(nkw1==1); mConfKW[jkw][0].prR=(nkw2==1);     
            mConfKW[jkw][1].prL=(nkw3==1); mConfKW[jkw][1].prR=(nkw4==1);     
            mConfKW[jkw][2].prL=(nkw5==1); mConfKW[jkw][2].prR=(nkw6==1);     
            
            if (jkw>0) printf(" :--------:-------:-------:-------:-------:-------:-------:\n");                    
            printf(" : N��=%02d",jkw+1);printf(" :");
            if (mConfKW[jkw][0].prL) printf("   +   :"); else printf("   -   :");
            if (mConfKW[jkw][0].prR) printf("   +   :"); else printf("   -   :");
            if (mConfKW[jkw][1].prL) printf("   +   :"); else printf("   -   :");
            if (mConfKW[jkw][1].prR) printf("   +   :"); else printf("   -   :");
            if (mConfKW[jkw][2].prL) printf("   +   :"); else printf("   -   :");
            if (mConfKW[jkw][2].prR) printf("   +   :"); else printf("   -   :");
            printf("\n");                        
            jkw=jkw+1;
        }//while;
        *N=jkw;
        if (!*my_ERR) printf(" :========:=======:=======:=======:=======:=======:=======:\n");
        fclose(f_ConfKW);
     
     }//if (!f_ConfKW);
   }//ReadConf()     

// ������������� � ������ ������ �� - StatKW.txt       
   void ReadStat(char *myFile,bool *my_ERR, 
                 struct stLR  mStatKW[][3],
                 struct stLRc mConfKW[][3],
                 int *N){
     int  nkw1,nkw2,nkw3,nkw4,nkw5,nkw6; //,nkw7,nkw8,nkw9,nkw10,nkw11,nkw12;
     int jkw;
     FILE *f_StatKW;
     f_StatKW=fopen(myFile,"r");
     
     if (!f_StatKW){
        printf(" --------------------------------------------------------------------------\n");       
        printf(" ��������! ����������� ���� �� (�������������� ������ ��) => <StatKW.txt>\n");
        printf(" --------------------------------------------------------------------------\n");          
        *my_ERR=true;
     } else {
        printf(" --------------------------------------------------------------------------\n");  
     
        printf(" OK! => ���������   ���� �� (�������������� ������ ��) => <StatKW.txt>\n");
        printf("        * ������ �����... ����������� ��������������� ������ ��������...\n");        
        printf(" --------------------------------------------------------------------------\n");  
        
     // ������ ����� � ������������ ���������       
        printf(" :========:==========:==========:==========:==========:==========:==========:\n");                  
        printf(" : N��    : az_L     : az_R     : pol_L    : pol_R    : orb_L    : orb_R    :\n");
        printf(" :========:==========:==========:==========:==========:==========:==========:\n");     
        jkw=0;
        while (fscanf(f_StatKW,"%d %d %d %d %d %d\n",&nkw1,&nkw2,&nkw3,&nkw4,&nkw5,&nkw6)!=EOF){
         // �������� ������� ������ (������ - 0;1)        
            if (!(((nkw1==0)||(nkw1==1))&&((nkw2==0)||(nkw2==1))&&
                  ((nkw3==0)||(nkw3==1))&&((nkw4==0)||(nkw4==1))&&
                  ((nkw5==0)||(nkw5==1))&&((nkw6==0)||(nkw6==1)) )){
                printf("\n");printf(" ����������! ������ ������ ������ ������...???\n");
        
                printf("\n");
                *my_ERR=true; break;
            }//if        
            mStatKW[jkw][0].prL=(nkw1==1); mStatKW[jkw][0].prR=(nkw2==1);     
            mStatKW[jkw][1].prL=(nkw3==1); mStatKW[jkw][1].prR=(nkw4==1);     
            mStatKW[jkw][2].prL=(nkw5==1); mStatKW[jkw][2].prR=(nkw6==1);     
            
            if (jkw>0) printf(" :--------:----------:----------:----------:----------:----------:----------:\n");
            printf(" : N��=%02d",jkw+1);printf(" :");                        
         // �������� ������� � ������ ������������
            if (mConfKW[jkw][0].prL){
               if (mStatKW[jkw][0].prL) printf(" ���� �/� :"); else printf(" �������? :");
            } else printf(" ��� �/�  :");            
            if (mConfKW[jkw][0].prR){
               if (mStatKW[jkw][0].prR) printf(" ���� �/� :"); else printf(" �������? :");
            } else printf(" ��� �/�  :");                        
            if (mConfKW[jkw][1].prL){
               if (mStatKW[jkw][1].prL) printf(" ���� �/� :"); else printf(" �������? :");
            } else printf(" ��� �/�  :");                        
            if (mConfKW[jkw][1].prR){
               if (mStatKW[jkw][1].prR) printf(" ���� �/� :"); else printf(" �������? :");
            } else printf(" ��� �/�  :");                        
            if (mConfKW[jkw][2].prL){
               if (mStatKW[jkw][2].prL) printf(" ���� �/� :"); else printf(" �������? :");
            } else printf(" ��� �/�  :");            
            if (mConfKW[jkw][2].prR){
               if (mStatKW[jkw][2].prR) printf(" ���� �/� :"); else printf(" �������? :");
            } else printf(" ��� �/�  :");                                    

            printf("\n");                                
            jkw=jkw+1;
        }//while;
        *N=jkw;
        if (!*my_ERR) printf(" :========:==========:==========:==========:==========:==========:==========:\n");        
        fclose(f_StatKW);
     }//if (!f_StatKW)
   }//ReadStat(     


// ������������� � ������ ������ �� - DataKW.txt
   void ReadData(char *myFile,bool *my_ERR,double mFixALR[][2], 
                 struct stDir mDataKW[][3],int *N)
        { 
   
     float pkw1,pkw2,pkw3,pkw4,pkw5,pkw6,pkw7,pkw8,pkw9,pkw10,pkw11,pkw12;
     int jkw;
     FILE *f_DataKW;
     f_DataKW=fopen(myFile,"r");     
     
     if (!f_DataKW){
        printf(" --------------------------------------------------------------------------\n");  
     
        printf(" ��������! ����������� ���� �� (������� ��������� ��    ) => <DataKW.txt>\n");
        printf(" --------------------------------------------------------------------------\n");  
        
        *my_ERR=true;
      } else {
        printf(" --------------------------------------------------------------------------\n");  
      
        printf(" OK! => ���������   ���� �� (������� ��������� ��    ) => <DataKW.txt>\n");
        printf("        * ������ �����... ����������� ��������������� ������ ��������...\n");        
        printf(" --------------------------------------------------------------------------\n");  
        
     // ������ ����� � ������������ ���������       
        printf(" ======:====================================================:\n");     
        jkw=0;        
        while (fscanf(f_DataKW,"%f %f %f %f %f %f %f %f %f %f %f %f\n",
                      &pkw1,&pkw2,&pkw3,&pkw4,&pkw5,&pkw6,
                      &pkw7,&pkw8,&pkw9,&pkw10,&pkw11,&pkw12)!=EOF){

         // �������� ������� ������ (������ - 0;1)        
            if (!(
                  (pkw1>= mFixALR[0][0])&&(pkw1<= mFixALR[0][1])&&
                  (pkw3>= mFixALR[0][0])&&(pkw3<= mFixALR[0][1])&&
                  (pkw5>= mFixALR[1][0])&&(pkw5<= mFixALR[1][1])&&
                  (pkw7>= mFixALR[1][0])&&(pkw7<= mFixALR[1][1])&&
                  (pkw9>= mFixALR[2][0])&&(pkw9<= mFixALR[2][1])&&
                  (pkw11>=mFixALR[2][0])&&(pkw11<=mFixALR[2][1])
               )){
               printf("\n");printf(" ����������! ����� ������ �� ������� �������� ���������...???\n");
        
               printf("\n");
               *my_ERR=true; 
               break;
            }//if                              
             mDataKW[jkw][0].stDirL.val=pkw1;  mDataKW[jkw][0].stDirL.cko=pkw2;             
             mDataKW[jkw][0].stDirR.val=pkw3;  mDataKW[jkw][0].stDirR.cko=pkw4;                      
             
             mDataKW[jkw][1].stDirL.val=pkw5;  mDataKW[jkw][1].stDirL.cko=pkw6;             
             mDataKW[jkw][1].stDirR.val=pkw7;  mDataKW[jkw][1].stDirR.cko=pkw8;                      

             mDataKW[jkw][2].stDirL.val=pkw9;  mDataKW[jkw][2].stDirL.cko=pkw10;             
             mDataKW[jkw][2].stDirR.val=pkw11; mDataKW[jkw][2].stDirR.cko=pkw12;

             printf(" N��=%02d:",jkw+1);             
             printf("  ����� (-)  : cko        : ������ (+) : cko        :\n");
             printf(" ------:----------------------------------------------------:\n");                  
             printf("  A    : %010.6f : %010.6f : %010.6f : %010.6f :\n",pkw1,pkw2,pkw3,  pkw4 );
             printf("  L    :  %010.6f : %010.6f : %010.6f : %010.6f :\n",pkw5,pkw6,pkw7,  pkw8 );              
             printf("  R    :  %010.6f : %010.6f : %010.6f : %010.6f :\n",pkw9,pkw10,pkw11,pkw12);             
             printf(" ======:====================================================:\n");              
             jkw=jkw+1;
                      
        }//while        
        *N=jkw;
        fclose(f_DataKW);        
     }//if (!f_DataKW)
     }//if (!my_ERR)

// ������������ �������-������ � �������� ������ ��� ������ �� (��������� �� ��)
// 2-� �������
void GetBufKWos(char typ, 
              bool         pConfKW[][2],               
              struct stLRc mConfKW[][3],              
              bool         pStatKW[][2],
              struct stLR  mStatKW[][3], 
              int nKW,
              double       pDataKW[][4], 
              struct stDir mDataKW[][3],
              bool* pr_La, bool* pr_Ra, bool* pr_DLa, bool* pr_DRa){
   int jkw;
   double swL,swR;
   bool   pr_L,pr_R;
   
   double swDL,swDR;                                   
   bool   pr_DL,pr_DR;  
   if (typ=='A'){ 
      swL  = 0;
      swR  = 0;
   }else{
      swL  = 90;
      swR  = 90;   
   }
   
   pr_L = true;
   pr_R = true;

   swDL  = 999;
   swDR  = 999;                                
   pr_DL = true;
   pr_DR = true;

   for (jkw=0;jkw<nKW;jkw++){                
      switch (typ){
         case 'A':
             pDataKW[jkw][0]=mDataKW[jkw][0].stDirL.val;                   
             pDataKW[jkw][1]=mDataKW[jkw][0].stDirL.cko;                   
             pDataKW[jkw][2]=mDataKW[jkw][0].stDirR.val;
             pDataKW[jkw][3]=mDataKW[jkw][0].stDirR.cko;                                                     
          // ������������ �������� �������-������ �������������� ��������� ��
             pStatKW[jkw][0]=mStatKW[jkw][0].prL;
             pStatKW[jkw][1]=mStatKW[jkw][0].prR;             
          // ������������ �������� �������-������ ������������ ��
             pConfKW[jkw][0]=mConfKW[jkw][0].prL;
             pConfKW[jkw][1]=mConfKW[jkw][0].prR;             
             break;
         case 'L':
             pDataKW[jkw][0]=mDataKW[jkw][1].stDirL.val;                   
             pDataKW[jkw][1]=mDataKW[jkw][1].stDirL.cko;                   
             pDataKW[jkw][2]=mDataKW[jkw][1].stDirR.val;
             pDataKW[jkw][3]=mDataKW[jkw][1].stDirR.cko;                                                     
          // ������������ �������� �������-������ �������������� ��������� ��
             pStatKW[jkw][0]=mStatKW[jkw][1].prL;
             pStatKW[jkw][1]=mStatKW[jkw][1].prR;             
          // ������������ �������� �������-������ ������������ ��
             pConfKW[jkw][0]=mConfKW[jkw][1].prL;
             pConfKW[jkw][1]=mConfKW[jkw][1].prR;             
             
             break;
         case  'R':
             pDataKW[jkw][0]=mDataKW[jkw][2].stDirL.val;                   
             pDataKW[jkw][1]=mDataKW[jkw][2].stDirL.cko;                   
             pDataKW[jkw][2]=mDataKW[jkw][2].stDirR.val;
             pDataKW[jkw][3]=mDataKW[jkw][2].stDirR.cko;                                                     
          // ������������ �������� �������-������ �������������� ��������� ��
             pStatKW[jkw][0]=mStatKW[jkw][2].prL;
             pStatKW[jkw][1]=mStatKW[jkw][2].prR;             
          // ������������ �������� �������-������ ������������ ��
             pConfKW[jkw][0]=mConfKW[jkw][2].prL;
             pConfKW[jkw][1]=mConfKW[jkw][2].prR;                          
             break;
      }//switch (typ)

   // �������� �� ���������� (����� - ������ ��������)
      if (pr_DL){
         pr_DL = fabs(swDL-pDataKW[jkw][0]) > EpsKW;
         swDL  = pDataKW[jkw][0];
      }                                                                  
      if (pr_DR){
         pr_DR = fabs(swDR-pDataKW[jkw][2]) > EpsKW;
         swDR  = pDataKW[jkw][2];
      }                                                                  
   // �������� �� �������� ���������� (����� - ����������� ������)                                      
      if (pr_L){      
         pr_L = pDataKW[jkw][0]<swL;
         swL  = pDataKW[jkw][0];
      }                                                          
      if (pr_R){
         pr_R = pDataKW[jkw][2] > swR;
         swR  = pDataKW[jkw][2];
      }                                                          
      
   };//jkw

   *pr_La  = pr_L;
   *pr_Ra  = pr_R;
   *pr_DLa = pr_DL;
   *pr_DRa = pr_DR;
                   
  }//GetBufKW()


// ��������� ������ ������ ������ ��� ��
   void Print_KW(double pDataKW[][4],int nKW){
      int jkw;                                      
      for (jkw=0;jkw<nKW;jkw++){                  
         printf("   - ������ ��� ");printf("%d",jkw+1); printf("-�� �� [���.����]:");                   
         printf(" ��[����� ]=%10.6f", pDataKW[jkw][0]);
         printf(" CKO=%12.8f\n",      pDataKW[jkw][1]);                
         printf("                                   ");                
         printf(" ��[������]= %10.6f",pDataKW[jkw][2]);
         printf(" CKO=%12.8f\n",      pDataKW[jkw][3]);                
     }//jkw                
     
   }//PrintKW


// ��������� ������� ������ ��� ������ ������ ��     
   void Print_Er(bool pr_L,bool pr_R,bool pr_DL,bool pr_DR){
     // ��������� ������ ��� ������ ��     
              printf("   --------------------------------------------------------------\n");              
        if (!pr_DL){   
              printf("   ��������! ������   �� ��� ������  ����� �� (�������� ���������)\n");
        }else printf("   OK! ====> �������� �� ��� ������  ����� �� (�������� ���������)\n");     
        if (!pr_L){    
              printf("   ��������! ������   �� ��� ������  ����� �� (������� ����������)\n");
        }else printf("   OK! ====> �������� �� ��� ������  ����� �� (������� ����������)\n");        
     // ��������� ������ ��� ������� ��                
        if (!pr_DR){   
              printf("   ��������! ������   �� ��� ������� ����� �� (�������� ���������)\n");
        }else printf("   OK! ====> �������� �� ��� ������� ����� �� (�������� ���������)\n");         
        if (!pr_R){    
              printf("   ��������! ������   �� ��� ������� ����� �� (������� ����������)\n");
        }else printf("   OK! ====> �������� �� ��� ������� ����� �� (������� ����������)\n");        
              printf("   --------------------------------------------------------------\n");              
    } //Print_Er           
    
// ��������� ����� ���������� ������ ������ �� ��������� ��� �� (��������� ��������)
// 2-� �������
   void InpDataKW( struct stDir mDataKW[][3]){

      // ������������� ������ ��� ������������ ��� - A
      //================================================================      
        mDataKW[0][0].stDirL.val=-208.400;   // [���.����] // �������!
        mDataKW[0][0].stDirL.cko=   0.134;   // [���.����] // �������!    
      //----------------------------------------------------------------        
        mDataKW[0][0].stDirR.val=+208.400;   // [���.����] // �������!
        mDataKW[0][0].stDirR.cko=   0.112;   // [���.����] // �������!    
      //================================================================
        mDataKW[1][0].stDirL.val=-209.400;   // [���.����] // �������!
        mDataKW[1][0].stDirL.cko=   0.163;   // [���.����] // �������!    
      //----------------------------------------------------------------        
        mDataKW[1][0].stDirR.val=+209.400;   // [���.����] // �������!
        mDataKW[1][0].stDirR.cko=   0.512;   // [���.����] // �������!    
      //================================================================        
        mDataKW[2][0].stDirL.val=-210.400;   // [���.����] // �������!
        mDataKW[2][0].stDirL.cko=   0.316;   // [���.����] // �������!    
      //----------------------------------------------------------------        
        mDataKW[2][0].stDirR.val=+210.900;   // [���.����] // �������!
        mDataKW[2][0].stDirR.cko=   0.042;   // [���.����] // �������!    
      //================================================================

      // ������������� ������ ��� �������� ��� - L
      //================================================================      
        mDataKW[0][1].stDirL.val=  20.400;   // [���.����] // �������!
        mDataKW[0][1].stDirL.cko=   0.134;   // [���.����] // �������!    
      //----------------------------------------------------------------        
        mDataKW[0][1].stDirR.val= 159.900;   // [���.����] // �������!
        mDataKW[0][1].stDirR.cko=   0.112;   // [���.����] // �������!    
      //================================================================
        mDataKW[1][1].stDirL.val=  19.400;   // [���.����] // �������!
        mDataKW[1][1].stDirL.cko=   0.163;   // [���.����] // �������!    
      //----------------------------------------------------------------        
        mDataKW[1][1].stDirR.val= 160.900;   // [���.����] // �������!
        mDataKW[1][1].stDirR.cko=   0.512;   // [���.����] // �������!    
      //================================================================        
        mDataKW[2][1].stDirL.val=  18.400;   // [���.����] // �������!
        mDataKW[2][1].stDirL.cko=   0.316;   // [���.����] // �������!    
      //----------------------------------------------------------------        
        mDataKW[2][1].stDirR.val= 161.900;   // [���.����] // �������!
        mDataKW[2][1].stDirR.cko=   0.042;   // [���.����] // �������!    
      //================================================================

      // ������������� ������ ��� ����������� ��� - R
      //================================================================      
        mDataKW[0][2].stDirL.val=  12.600;   // [���.����] // �������!
        mDataKW[0][2].stDirL.cko=   0.134;   // [���.����] // �������!    
      //---------------------------------------------------------------        
        mDataKW[0][2].stDirR.val= 169.900;   // [���.����] // �������!
        mDataKW[0][2].stDirR.cko=   0.112;   // [���.����] // �������!    
      //================================================================
        mDataKW[1][2].stDirL.val=  11.600;   // [���.����] // �������!
        mDataKW[1][2].stDirL.cko=   0.163;   // [���.����] // �������!    
      //----------------------------------------------------------------        
        mDataKW[1][2].stDirR.val= 170.900;   // [���.����] // �������!
        mDataKW[1][2].stDirR.cko=   0.512;   // [���.����] // �������!    
      //================================================================        
        mDataKW[2][2].stDirL.val=  10.600;   // [���.����] // �������!
        mDataKW[2][2].stDirL.cko=   0.316;   // [���.����] // �������!    
      //----------------------------------------------------------------        
        mDataKW[2][2].stDirR.val= 171.900;   // [���.����] // �������!
        mDataKW[2][2].stDirR.cko=   0.042;   // [���.����] // �������!    
      //=================================================================
   }//InpDatKWos()  

// ������ �������� �������������� ��������� ��
// 2-� �������
   void InpStatKW( struct stLR mStatKW[][3]){   
     // --------------------------------------------------   
        mStatKW[0][0].prL=false; mStatKW[0][0].prR=false;     
        mStatKW[1][0].prL=false; mStatKW[1][0].prR=true;
        mStatKW[2][0].prL=true;  mStatKW[2][0].prR=true;
     // --------------------------------------------------      
        mStatKW[0][1].prL=false; mStatKW[0][1].prR=false;     
        mStatKW[1][1].prL=false; mStatKW[1][1].prR=true;
        mStatKW[2][1].prL=true;  mStatKW[2][1].prR=true;
     // --------------------------------------------------   
        mStatKW[0][2].prL=false; mStatKW[0][2].prR=false;     
        mStatKW[1][2].prL=false; mStatKW[1][2].prR=true;
        mStatKW[2][2].prL=true;  mStatKW[2][2].prR=true;
     // --------------------------------------------------      
   }//InpDatKWpr()


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 
// ������ ������������� ������ ���������� ��������
   int ModKW(){
    // =======================================================================
    // ������ �������� �������������� ����������
    // =======================================================================     
    // ������� ������� �������   
       bool prCP=false;      
    // ���������� ������� ������� ������   
    // bool my_ERR=false;   
    // ���������� �����
       int ikw,jkw;   
    // ����� ��� ���
       int NumOs=-1;     
    // ���������� ������������ �����������   
       int Napp;
       int ikk,jkk;
       double Vtek,Atek,Utek,t0,tn,tk,ttek;
       double mBufKW[5000][4];// ������-�����   
    // ���������� ������ ������ ���������� ���� ���
       int  nReg=-1;
       char sReg[256];      
    // ������ ������������ ������   
    // !!! - ������� 1�� ��� ����������� ����������� ������ <�� ��������>
    // ������� 10�� - ������������ ����� <�� ��������>
       double dtt=0.1;
    // ��������������� � �����
       double DelVal;
       double EpsOS=0.1;
    // double EpsKW=0.2; // ���������� �� ��������� ��
        
    // �������� ��������� ��� ����� ��������� - ��������� ��� �� ���� ���� (A,L,R)
       double ugA_0,ugL_0,ugR_0;
       ugA_0 =-111.0;
       ugL_0 = 45.0; 
       ugR_0 = 125.0;    
    // ���������� �����
       char sss[256];
    // ����������-�������� (������ ������� ��)   
       int jpr_L=0;
       int jpr_R=0;   
        
    // ���������� ���������� �� ��������� (������ ��)
       double EpsUch=0.1;   
    // ���������� ���������� (������ ������)   
       int nConf = 0;   
       int nStat = 0;   
       int nData = 0;   
       int nInpKW= 0; // ����� ����������� ����� (������� ��) � ������ ��
    // ������� �������� ��������������� �� ���� ������������ ���������������� ����
       bool prDel_Os=false;           
    // ����� ������ ��
       FILE *f_ConfKW,*f_StatKW,*f_DataKW,*f_DataBF,*f_KW;
    // ��������� ������
       int    nkw1,nkw2,nkw3,nkw4,nkw5,nkw6;
       float  pkw1,pkw2,pkw3,pkw4,pkw5,pkw6,pkw7,pkw8,pkw9,pkw10,pkw11,pkw12;
    // ������ ��������-�����������
       double nFixALR[3][2];
       nFixALR[0][0]=AminKW; nFixALR[0][1]=AmaxKW;
       nFixALR[1][0]=LminKW; nFixALR[1][1]=LmaxKW;                
       nFixALR[2][0]=RminKW; nFixALR[2][1]=RmaxKW;     

    // ������� ������������ �� (�������� � ����������� �� + �����)
    // -----------------------------------------------------------
    // ���������� ���������
    // mConfKW[���-�� ��][��� ��� ���]
       struct stLRc mConfKW[nnSwEnd][3];   
    // ----------------------------------   
    // ������� ��� ��������� ��� ���   
    // pConfKW[���-�� ��][0-�����(=>-),1-������(=>+)]
       bool pConfKW[nnSwEnd][2];          

    // ������� �������������� ��������� �� (��������� ��� ��� ���)
    // -----------------------------------------------------------
    // ���������� ���������
    // mStatKW[���-�� ��][��� ��� ���]
       struct stLR mStatKW[nnSwEnd][3]; 
    // ----------------------------------   
    // ������� ��� ��������� ��� ���   
    // pStatKW[���-�� ��][0-�����(=>-),1-������(=>+)]  
       bool pStatKW[nnSwEnd][2];          
    // ----------------------------------   
    
    // ������� ������ �� �� (������ + ���)
    // -----------------------------------------------------------
    // ���������� ���������
    // mDataKW[���-�� ��][��� ��� ���]
       struct stDir mDataKW[nnSwEnd][3];      
    // ----------------------------------          
    // ����� ������ � ��������
       struct stDir mDataBuf[nnSwEnd][3];
    // ----------------------------------          
    // �������� ���������� ������
       struct prDir prDataBuf[nnSwEnd][3];       
    // ----------------------------------   
    // ������� ��� ��������� ��� ���   
    // pDataKW[���-�� ��][0,1-�����; 2,3-������]
       double pDataKW[nnSwEnd][4];
    // ----------------------------------   

    // ��������� ��������� ��������� ��� ��� ���
       double ValOs0,ValOsMax,ValOsMin,DelOs;     
   
    // ����������-�������� ��������� ��
       bool pr_L0,pr_R0;     
       bool pr_L ,pr_R; 
       bool pr_DL,pr_DR;
   
    // ���������� ���� ��� ���   
       char code_ALR;
       char sCod_ALR[256];
    // ��� ������ �������   
       const time_t my_timer=time(NULL); 
       char *my_str;        
    // ������� �������� ��������� ������� ��� ��� - ��������������� ����������
       double arc_OS;   
    // ����������� ������ ��� ���������� ����� ���    
       double v6_ALR[6];
    // ������� �������� ��������������� �� ���� ������������ ���������������� ����
       prDel_Os=false;  
    // ������� ���������� ������� ��
       char sTek[256];   
       int  nTek=-1;      
        
    // ����������� ���������� �������� �������� � ���������
    // mAVmaxKW[��������,���������][�����,������]     
       double mAVmaxKW[2][2];      
       double VmaxOS,AmaxOS;      
    // -----------------------------------------------------------------------         
       mAVmaxKW[0][0] = -6.0; //[���.����/���  ] - ��� ������  ��
       mAVmaxKW[0][1] =  6.0; //[���.����/���^2] - ��� ������� ��                  
    // -----------------------------------------------------------------------               
    //   mAVmaxKW[1][0] = -3.0; //[���.����/���  ] - ��� ������  ��
    //   mAVmaxKW[1][1] =  3.0; //[���.����/���^2] - ��� ������� ��  
       mAVmaxKW[1][0] = -0.6; //[���.����/���  ] - ��� ������  ��
       mAVmaxKW[1][1] =  0.6; //[���.����/���^2] - ��� ������� ��                         
    // -----------------------------------------------------------------------               

    // ��������� ��� �������� ����������� �������
    // ---------------------------------------------------------------   
    // �������� ��������� ������� (��������� � ��������� �������� ��)      
    // (������,��������,�����)
       double mPLineKW[3];      
    // ---------------------------------      
    // ��������� ��������� (������� �� <0> �� ������ ��������� �������)      
    // y=A0*[1-exp(-k*x)];
    // mExpKW[���������,����������];
       double mExpKW[2];
    // ---------------------------------
    // ��������� ��������� ������� (��������� � ��������� �������� ��)
    // y=V*(t-t0)+y0;
    // mLineKW[�����,����,��������]
       double mLineKW[3];
    // ---------------------------------
    // ��������� �������� - ���������� �������
    // y =1/2*a*x^2+b*x;
    // Vy=a*x+b; - ���������� ��������
    // Ay=a;     - ���������
    // mPParKW[����� ������/����������,���� �� ���������];
       double mPParKW[2];
    // ---------------------------------     
    // �������� ������������ ��                  
       double arc_GSS=0;// ������ ��������� �� + ����������� ���
       bool   prExeKW=false;           
    // ---------------------------------     
     
    // ���������� ������� ������� ������   
       bool my_ERR=false;   

    // ������� ��� ������� ������ ���������   
       bool prTrue=false;   
        
    // =======================================================================
    // ����� �������� �������������� ����������
    // =======================================================================
       
    // ������������� ������ ������ ��� ����    
       printf(" ------------------------------------------------------------------------\n");             
       if (IMIT){
          printf(" ��������! ������������� ������ � ������ <��������>\n");
          printf("           # ����������� �������������� ������� ��������� ��\n");
          printf("           # ����� ������������ �� �������������: [������ ��]+[���]\n");               
          printf("           # ���������� ����� ��� - �������� ������ <���>\n");                              
       }else 
          printf(" ��������! ������������� ������ � ������ <��������>\n");
       printf(" ------------------------------------------------------------------------\n");        
     
    // ������ ��� ��������� (��������) !!!        
    // printf(" # NEXT (������� <����>) ==> "); gets(sss); 
       printf(" => "); gets(sss);        
   
    // ������������� � ������ ������ �� - ConfKW.txt 
       nConf=0;
       ReadConf("ConfKW.txt",&my_ERR,mConfKW,&nConf); // &nConf - ���������� ��
    // ��������������� ����� - ���-�� �� �� ����� ������������
       printf(" N��(nConf)= %d\n",nConf);   

    // ������������� � ������ ������ �� - StatKW.txt 
       if (!my_ERR){
       // ������ ��� ��������� (��������) !!!      
       // printf(" # NEXT (������� <����>) ==> "); gets(sss);      
          printf("=> "); gets(sss);      
          nStat=0;
          ReadStat("StatKW.txt",&my_ERR,mStatKW,mConfKW,&nStat); // &nStat - ���������� ��
       // ��������������� ����� - ���-�� �� �� ����� ������������
          printf(" N��(nStat)= %d\n",nStat);      
       // �������� �� ����������� ���������� ��
          printf(" ------------------------------------------------------------------------\n");        
       if (nStat!=nConf){
          printf(" ��������! ������������ <N��> � ������ <ConfKW> � <StatKW>\n");
          my_ERR=true; 
       }else 
          printf(" OK! => ���������� �����   � ������ <ConfKW> � <StatKW>\n");        
          printf(" ------------------------------------------------------------------------\n");           
       }//if (!my_ERR)   
       
    // ������������� � ������ ������ �� - DataKW.txt
       if (!my_ERR){ 
       // ������ ��� ��������� (��������) !!!
       // printf(" # NEXT (������� <����>) ==> "); gets(sss);
          printf("=> "); gets(sss);      
          printf("\n");  
          nData=0;
          ReadData("DataKW.txt",&my_ERR,nFixALR,mDataKW,&nData); // &nData - ���������� ��
       // ��������������� ����� - ���-�� �� �� ����� �� ���������
          printf(" N��(nData)= %d\n",nData);              
       // �������� �� ����������� ���������� ��
          printf(" ------------------------------------------------------------------------\n");             
          if (nData!=nStat){
             printf(" ��������! ������������ <N��> � ������ <DataKW> � <StatKW>\n");
             my_ERR=true; 
          }else{
             printf(" OK! => ���������� ����� � ������ <DataKW> � <StatKW>\n"); 
          // ����� ������������ ��� ��������              
             nInpKW=nData;
          }//if                      
          printf(" ------------------------------------------------------------------------\n");                
       }//if (!my_ERR)          

    // ��������� ������ ������ � ��������� ����������
       for (ikw=0; ikw<nInpKW; ikw++){
          for (jkw=0; jkw<3; jkw++){
          // ����� ����� ������
             mDataBuf[ikw][jkw].stDirL.val = 0.0;
             mDataBuf[ikw][jkw].stDirL.cko = 0.0;
             mDataBuf[ikw][jkw].stDirR.val = 0.0;
             mDataBuf[ikw][jkw].stDirR.cko = 0.0;
          // �������� ����������             
             prDataBuf[ikw][jkw].mLR[0]=0;
             prDataBuf[ikw][jkw].mLR[1]=0;             
          };//jkw
       };//ikw                    

    // ========================= ���������� ������ ===========================
       if (my_ERR) {
          PrMsg("������ ������� ������ - ������ � ��������� ��");        
          my_ERR=false;
          return 1;// ��������� ����� �� ������� ���������!              
       }//if
    // =======================================================================        
    
    // ������ ��� ��������� (��������) !!!    
       printf("=> "); gets(sss);      

    // ======================================================================    
    // ����� ������ � ������� �� �� �� !!!
    // ======================================================================
    
    // ����� �������� ��� �������� ��������� ���� ���� ���
       OutDelWork(nInpKW,mDataKW,mConfKW);
    // ������ ��� ��������� (��������) !!!    
       printf("=> "); gets(sss);      
    // -----------------------------------------------------------------------  

    // �������������� � ����������� ������� ��������� ������� ��������� �� CAN     
    // ��������� ����������� ��������� ���������� � "����������� ���������"
    // ��� ������� ���� � ����������� ����� ��������� ��� ???
    // ���� �������, ��� ��� ����� ������ � ������� <mmCanKW> ���������...!?
    
    // �������� !!!     
          
    // ---------     
    // ��������� ��������� � ��������� �� ��������� (��������� ��� ������� �������)
    // <true > - ��������� � �����������
    // <false> - ������ �����������
//     if ((IMIT)&&(!prCP)) 
       if (!prCP)        
          ScrCanKW(true,nInpKW,mmCanKW,prCanKW);     
    // �� ������ <prCP> �� ��������� !
    // -----------------------------------------------------------------------    
    // ������ ��� ��������� (��������) !!!
    // printf(" # NEXT (������� <����>) ==> "); gets(sss); 
       printf("=> "); gets(sss);      
    // -----------------------------------------------------------------------  

     
    // ��������� ����������� �������� ��� ������� ���������� (999.0)
       for (jkw=0; jkw<6; jkw++) v6_ALR[jkw]=999.0;

    // ������ ��� �������� � ��� ������
       if (!prCP){
       // ��������� ��������� ��� �� ���� ���� (A,L,R) - ��������� ��� ����� ���������
          
       // ������ ��� ��������!
          if (IMIT) 
             IniValOs(nInpKW,&prCP,mDataKW,cd);
          // �� ������ <prCP> ��������� !              
          else{
             printf(" ------------------------------------------------------------------------\n");     
        
             printf(" CD! => A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);
           
          };//if
       // ��������� (�������� ���������) ��������� CAN ��� �� �� ������ ��
          StatCanKW(nInpKW,mDataKW,mmCanKW);
       // ����������� ���������
          ScrCanKW(false,nInpKW,mmCanKW,prCanKW);                   
       // �� ������ ������!   
          prCP=true;           
       // -----------------------------------------------------------------------    
       // ������ ��� ��������� (��������) !!!           
          printf("=> "); gets(sss);      
       // -----------------------------------------------------------------------             
       };//if - ��������� ��������� ���..                

    // ������� ������ ���� � ��������� �� ��� ���� ���� 
    // (������������� ��������� ������� ��� ������� mmCanKW)       
       if (!IMIT){       
//             SendKdpoKW(dtt);
             printf("=> "); gets(sss);      
          // printf("\n");               
             printf(" ------------------------------------------------------------------------\n");     
             printf(" ��������? �������� �������� �� ��������� ������ � ��������� �� (����)\n");
             printf("           �����...\n");                                          
          // usleep(5.0*1000000);                                                         
             my_delay(3.0);

       // ����������� ���������
          ScrCanKW(false,nInpKW,mmCanKW,prCanKW);                   
       // -----------------------------------------------------------------------    
       // ������ ��� ��������� (��������) !!!           
          printf("=> "); gets(sss);      
          
/*             
          // ������� ��������� ��������� �� CAN
          // ���� - ���� ����� �� ���� �����������!
             prTrue=0;   
             for (ikw=0; ikw<nInpKW; ikw++){
                for (ikk=0; ikk<3; ikk++){
                   for (jkk=0; jkk<2; jkk++){                
                   // if (!prTrue) prTrue=prCanKW[ikw][ikk].mLR[jkk];                     
                    if ((!prTrue)&&(prCanKW[ikw][ikk].mLR[jkk]==1)){  
                       prTrue=1;
                       break;
                    };//if
                   };//jkk
                };//ikk                       
             };//ikw                           
             if (!prTrue){
                printf(" ��������! �� ������������� �����, ��������� � ��������� �� �� ��������\n");          
                printf("           ����� �� ������������ ������ � ���� ������������ ��\n");                       
                printf(" ------------------------------------------------------------------------\n");                          
                return 6;
             }else
                printf(" OK! ====> �������� �� ����� ������ ������ �� ���� � ��������� ��\n");
             printf(" ------------------------------------------------------------------------\n");             
*/             
       };//(!IMIT)
/*
   for (ikw=0; ikw<100; ikw++){
    printf(" OK! ====> A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);
       skorcsn(0.1,999.0,999.0);
       usleep(0.1*1000000);
    printf(" OK! ====> A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);       
   }
   printf("=> "); gets(sss);             
   return 0;
*/   
    // ������ ����������� ���� ������ - ����� ������ ������ ���������� �������� 
       while (true){        
       
         // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
         // ������������� ������ ���������� �������� ��� � ����� ��� ���
         // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     
            printf(" ������������� ������ ���������� �������� ���:\n");
            printf("  # �������� �������� ��������� ������������ ��� => A:\n");
            printf("  # �������� �������� ��������� ��������     ��� => L:\n");
            printf("  # �������� �������� ��������� �����������  ��� => R:\n");        
         // ������ � ������� �������� ��� ��� => a(A),l(L) ��� r(R)    
            printf("  ? ���� (����� ���) => "); 
            gets(sCod_ALR);//���������� ���� - ���� ������!
         // �������������� ������-�������� (���������� � �������� ��������)         
            code_ALR=toupper(sCod_ALR[0]);// �������� ������ ������ ������!            
            if ( (!(strlen(sCod_ALR)==1))||(!((code_ALR=='A')||(code_ALR=='L')||(code_ALR=='R')))  ){
               PrMsg("������ ����� ������ - ������������ ������ �������.");
               break;// ������ ����� �� ����������� ���� ������!              
            };//if          
            
            printf("\n");        
         // ���� � ����� ��� ���������
            my_str=ctime(&my_timer);
                    
            printf(" ���� � ����� ��� ��������� = %s",my_str);
            
            printf(" ��������� � �� ������ ��������� �������� ������������:\n");
            printf("\n");                
         // ��������� ��������� ����������-��������� �������� ������ ������
            pr_L  = true; pr_R  = true;
            pr_DL = true; pr_DR = true;
     
         // �������� ���� ��� ��� ���        
            switch (code_ALR){
                case 'A':
                    NumOs=0;
                    printf(" * ������� ������������ ��� (A)\n");
                    printf("   ������� �������� �������� ��������� ��� = %10.6f\n",cd[0]);           
                    arc_OS=cd[0];
                 // �������� ������ ��� ������ �� (��������� �� ��)                              
                 // 2-� �������
                    GetBufKWos('A',pConfKW,mConfKW,pStatKW,mStatKW,nInpKW,pDataKW,mDataKW,&pr_L,&pr_R,&pr_DL,&pr_DR);                                
                    break;                
                case 'L':
                    NumOs=1;
                    printf(" * ������� �������� ��� (L)\n");
                    printf("   ������� �������� �������� ��������� ��� = %10.6f\n", cd[1]);
                    arc_OS=cd[1];
                 // �������� ������ ��� ������ �� (��������� �� ��)                              
                 // 2-� �������
                    GetBufKWos('L',pConfKW,mConfKW,pStatKW,mStatKW,nInpKW,pDataKW,mDataKW,&pr_L,&pr_R,&pr_DL,&pr_DR);
                    break;                
                case 'R':
                    NumOs=2;
                    printf(" * ������� ����������� ��� (R)\n");
                    printf("   ������� �������� �������� ��������� ��� = %10.6f\n", cd[2]);                            
                    arc_OS=cd[2];
                 // �������� ������ ��� ������ �� (��������� �� ��)                              
                 // 2-� �������
                    GetBufKWos('R',pConfKW,mConfKW,pStatKW,mStatKW,nInpKW,pDataKW,mDataKW,&pr_L,&pr_R,&pr_DL,&pr_DR);
                    break;            
                
                default:
                    PrMsg("������ ����� ������ - ������������ ������ �������.");
                    break;// ������ ����� �� ����������� ���� ������!                                
            }//switch (code_ALR)

         // ������ ��� ��������� (��������) !!!
         // printf(" # NEXT (������� <����>) ==> "); gets(sss);

         // ��������� ������� ������ ��� ������ ������ ��     
            Print_Er(pr_L,pr_R,pr_DL,pr_DR);     
         // ����� ������ �� �� (������ �������� ������ - OK!)
//pr_L=false;
            if (pr_L&&pr_R&&pr_DL&&pr_DR) Print_KW(pDataKW,nInpKW);
            else{ printf("   ...������ ������� ��...\n");
                  return 2;// ��������� ����� �� ������� ���������!              
            }//if
            
         // ������ ��� ��������� (��������) !!!
//            printf(" # NEXT (������� <����>) ==> "); gets(sss);
            printf("=> "); gets(sss);      
          
         // ����� �������� � ��������� ���� ���
         // -----------------------------------
         // ������������ �������
            pr_L0=false;
            pr_R0=false;
         // ���������� ���� ������������ ��������� ��� (��� �������� ����� �� ��)      
         // ������� ������ ����������� � ���������� ������ �� ������ CAN
            pr_L=false;jpr_L=-1;
            pr_R=false;jpr_R=-1;      
            printf("\n");
            EventKW(&pr_L,&pr_R,&jpr_L,&jpr_R,arc_OS,pStatKW,pDataKW,nInpKW);         
         // ��������� ��� ������������ �������������� �������    
//pr_L =true;                     
            pr_L0=pr_L;
            pr_R0=pr_R;
            printf(" -----------------------------------------------------------------------\n");                  

         // ������������ ������        
            if ((pr_L)||(pr_R)){
               
               printf(" ������ ��� �� ������ ��  => [��.������ ��������] - ");         
               if (pr_L){ printf(" ����� ");printf("%d",jpr_L+1);printf("-�� ������\n");}
               if (pr_R){ printf(" ������ ");printf("%d",jpr_R+1);printf("-�� ������\n");}
//               printf(" ...������ ������� ��...\n");
                              
//               break;// ������ ����� �� ����������� ���� ������!               
                 
            }//if
            
            else printf(" ������ ��� �� ������ ��  => [���������� ��� ���������]\n");
            
         // printf(" -----------------------------------------------------------------------\n");                           
//pStatKW[0][0]=1;
//mmCanKW[0][NumOs].mLR[0]=1;
         // ���������� ���� ������������ ��������� ��� (��� �������� ����� �� ��)      
            
         // ����������� ������������� ����� ��������� ������� �� �� �� !?          
         // �� ������ CAN (���������� !!! - ������� ������ � ������� ��)        
            pr_L=false;jpr_L=-1;
            pr_R=false;jpr_R=-1;      
                                    
            for (ikw=0; ikw<nInpKW; ikw++){       
               if (!pr_L)
                  {jpr_L=ikw;pr_L=mmCanKW[ikw][NumOs].mLR[0]&&pStatKW[ikw][0];};//pr_L
               if (!pr_R)
                  {jpr_R=ikw;pr_R=mmCanKW[ikw][NumOs].mLR[1]&&pStatKW[ikw][1];};//pr_R            
            };//ikw            
         // printf(" -----------------------------------------------------------------------\n");                  
         
//pr_L =true;         
         // ������������ ������        
            if ((pr_L)||(pr_R)){
               printf(" ������ ��� �� ������ CAN => [��.������ ��������] - ");         
               if (pr_L){ printf(" ����� ");printf("%d",jpr_L+1);printf("-�� ������\n");}
               if (pr_R){ printf(" ������ ");printf("%d",jpr_R+1);printf("-�� ������\n");}
//               printf(" ...������ ������� ��...\n");
               
//               break;// ������ ����� �� ����������� ���� ������!               
            }//if
            else printf(" ������ ��� �� ������ CAN => [���������� ��� ���������]\n");
                        
            printf(" -----------------------------------------------------------------------\n");                           
            
         // �������� ���������
            if (!( ((pr_L )&&(pr_L0 )||(!pr_L)&&(!pr_L0))&&
                   ((pr_R )&&(pr_R0 )||(!pr_R)&&(!pr_R0)) )){
//             printf(" ��������! �������� ������������ ������� �� ������������� � CAN � ��\n");
                
//             printf("           ������ ������� ��...\n");
//             break;// ������ ����� �� ����������� ���� ������!               
               return 3;               
            };//if
         // ������ � ����� ������� ����������    
            if ( ((pr_L )&&(pr_L0 ))||((pr_R )&&(pr_R0)) ){
               return 4;
            };//if
       
         // ��������� ��� ��� ��������� ��� � ��������� �� ���������
            printf(" ��� >>>>> ��������� ��� ��� � ��������� �� ��������� - ");                        
            for (ikw=0; ikw<6; ikw++) v6_ALR[ikw]=999.0;// ��������
            switch (code_ALR){
                case 'A': NumOs=0;v6_ALR[0]=kw_A0; printf("%+006.1f",v6_ALR[0]); break;
                case 'L': NumOs=1;v6_ALR[1]=kw_L0; printf("%+006.1f",v6_ALR[1]); break;
                case 'R': NumOs=2;v6_ALR[2]=kw_R0; printf("%+006.1f",v6_ALR[2]); break;                 
            }//switch                               
         // ���������� ��������� ���
            printf(" [���.����]\n");     
            printf("           ����� ��� - <�� ���������>\n");        
            printf("           �����... \n");      
            perebroscsn(v6_ALR[0],v6_ALR[1],v6_ALR[2]);
            RESCSN();
            printf(" OK! ====> A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);
            printf(" ------------------------------------------------------------------------\n");

         // ����� ����� ����� ����� ��������� !!!
            myREP:;     
         // �������� ���������� - ��� ���������� �������������     
            for (ikw=0; ikw<6; ikw++) v6_ALR[ikw]=999.0;       
         // ������� �������� ��������������� �� ���� ������������ ���������������� <0>
            prDel_Os = false;  
         // EpsOS    = 0.01;  
         // �������� �������        
            switch (code_ALR){
                case 'A':             
                    NumOs=0;
                    DelVal=cd[0]-kw_A0;
                    prDel_Os=fabs(DelVal)<=EpsOS;
                    v6_ALR[3]=-0.1*DelVal/fabs(DelVal)*EpsOS;
                    break;
                case 'L':         
                    NumOs=1;
                    DelVal=cd[1]-kw_L0;
                    prDel_Os=fabs(DelVal)<=EpsOS;
                    v6_ALR[4]=-0.1*DelVal/fabs(DelVal)*EpsOS;
                    break;
                case 'R':  
                    NumOs=2;                  
                    DelVal=cd[2]-kw_R0;
                    prDel_Os=fabs(DelVal)<=EpsOS;
                    v6_ALR[5]=-0.1*DelVal/fabs(DelVal)*EpsOS;
                    break;                 
            }//switch         
//prDel_Os=1;
         // ������ ���������������
            if (prDel_Os) {
               printf(" ------------------------------------------------------------------------\n");      
               printf(" OK! ====> ��������� ������ ���������� ���� ��� � ���������������� <0>\n");
               printf("           A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);       
               printf(" ------------------------------------------------------------------------\n");               
               printf("\n");
            }else{
               printf(" ------------------------------------------------------------------------\n");     
               printf(" ��������! ������������ ��������������� ������������ ���������������� <0>\n");
               printf("           ��������� ��������� ���������? (y->��;n->���)\n");             
               printf(" => "); gets(sss); 
               if ((sss[0]=='y')||(sss[0]=='Y')){         
                  printf("           �����...\n");      
                  skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);                  
                  ikw=0;            
                  while (fabs(DelVal)>EpsOS){            
                      ikw=ikw+1;
//                      skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);
                      usleep(dtt*1000000);                           
                      switch (code_ALR){
                          case 'A': DelVal=cd[0]-kw_A0; break;
                          case 'L': DelVal=cd[1]-kw_L0; break;
                          case 'R': DelVal=cd[2]-kw_R0; break;                 
                      }//switch                                    
                      printf("         # ikw=%d; A=%12.8f; L=%12.8f; R=%12.8f;\n",ikw,cd[0],cd[1],cd[2]);                        
                  };//while              
                // ��������� ��� ���
                   skorcsn(0.0,0.0,0.0);                                  
                  goto myREP;
               } else printf(" ??? ====> ��������� ��������� �� �����������\n");                          
               printf("\n");
            }//if (prDel_Os) - ������ ���������������
            
//mmCanKW[2][NumOs].mLR[1]=1;
         // ������������� (�������� ������ ��������� CAN)        
         // �������� ���� �� ������� ������� �� !?
            pr_L=false; pr_R=false;
            for (ikw=0; ikw<nInpKW; ikw++){
               pr_L=mmCanKW[ikw][NumOs].mLR[0];
               pr_R=mmCanKW[ikw][NumOs].mLR[1];               
            };//ikw
            if ((pr_L)||(pr_R)){            
               return 5;
            };//if
         
         // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
         // ������ � ������� ����� ������ (���� ��) �������� ��
         // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&   
            nTek=-1;
            printf(" ������� ������� ����� ���� (������) ��� ��:\n");
            printf(" ---------------------------------------------\n");                        
            printf("  1 - ������ ���� (�������) ��\n");            
            printf("  2 - ������ ���� (�������) ��\n");                        
            printf("  3 - � �.�. �� max-����������� �������� Nconf\n");                        
            printf(" ---------------------------------------------\n");            
            printf("  ? ���� (����� ����) => "); gets(sTek);       
            sscanf(sTek,"%d",&nTek);     
         // ====================================================================
         // �������� ������� ����� ���������� "nTek"
            my_ERR=false; // nInpKW - ���������� ������� �� � ������ ��
            for (ikw=1; ikw<=nInpKW; ikw++) if (nTek==ikw){ my_ERR=true;break; };          
            if (!my_ERR){
               PrMsg("������ ����� ������ - ������������ ������ �������.");
               break;// ������ ����� �� ����������� ���� ������!         
            }else{     
               nTek=nTek-1; // �������� � ������� �������              
            // �������� ����������� ������� �� ��� ���������� ������ (�����,������)
               if (!(pConfKW[nTek][0]||pConfKW[nTek][1])){
                  PrMsg("��� ���������� ������ ����������� �������� ��.");         
                  break;// ������ ����� �� ����������� ���� ������!
               } else my_ERR=false;// ���������� ���.�������� ��� ����.�������������
            }//if          
         // ====================================================================   
         
         // #########################################################################               
         // ���� �� ������ � ������� �� - ��������� �������� ��
            for (jkw=0; jkw<2; jkw++){
         // #########################################################################         
             // �������� ���� ��� ��� - ����������� ������� �������� ���������������� <0>
                switch (code_ALR){
                    case 'A': arc_OS=kw_A0; break;
                    case 'L': arc_OS=kw_L0; break;
                    case 'R': arc_OS=kw_R0; break;                
                }//switch              
             // �������� ����������� ������� ���������� ��
                if (!pConfKW[nTek][jkw]){
                   if (jkw==0) PrMsg("��� ������  �� ����������� �������� (���������� �������).");
                   else        PrMsg("��� ������� �� ����������� �������� (���������� �������).");
//                   printf(" # NEXT (������� <����>) ==> "); gets(sss);
                   printf("=> "); gets(sss);      
                   continue;// ������ ����� � ������ �����!   
                };//if         
             // ����������� ���������� �������� �������� � ���������          
                VmaxOS=mAVmaxKW[0][jkw];
                AmaxOS=mAVmaxKW[1][jkw];         
                
             // �������� ��������� ������� (��������� � ��������� �������� ��)               
             // ----------------------------------------------------------------------         
             // arc_OS - �������� (�������� � �������� ��������) �� ��������������� <0>
                if (jkw==0){
                   mPLineKW[0] = pDataKW[nTek][0]+3.0*pDataKW[nTek][1] - arc_OS;
                   mPLineKW[1] = pDataKW[nTek][0]                      - arc_OS;
                   mPLineKW[2] = pDataKW[nTek][0]-3.0*pDataKW[nTek][1] - arc_OS;
                } else {
                   mPLineKW[0] = pDataKW[nTek][2]-3.0*pDataKW[nTek][3] - arc_OS;
                   mPLineKW[1] = pDataKW[nTek][2]                      - arc_OS;
                   mPLineKW[2] = pDataKW[nTek][2]+3.0*pDataKW[nTek][3] - arc_OS;
                }//if               
                printf("\n");
                printf(" ========================================================================\n");
             // ������ ��������� (�����) ��� ��������� �������
                printf(" # ��������� ��: Ln= %+10.6f; Lkw= %+10.6f; Lk= %+10.6f;\n",mPLineKW[0],mPLineKW[1],mPLineKW[2]);
         
             // ��������� ��������� (������� �� <0> �� ������ ��������� �������)      
             // mExpKW[��������� �������,����������]
             // ----------------------------------------------------------------------      
                mExpKW[0]=mPLineKW[1];     
                mExpKW[1]=VmaxOS/mPLineKW[1];
                printf(" # ��������� ��: A== %+10.6f; k=== %+10.6f\n",mExpKW[0],mExpKW[1]);

             // ��������� ��������� ������� (��������� � ��������� �������� ��)
             // mLineKW[�����,����,��������][�����,������]
             // ----------------------------------------------------------------------      
                mLineKW[0] = -log(1-mPLineKW[0]/mPLineKW[1])/mExpKW[1];
                mLineKW[1] = mPLineKW[0];
                mLineKW[2] = mExpKW[0]*mExpKW[1]*exp(-mExpKW[1]*mLineKW[0]);
                printf(" # ��������� ��: Tn= %+010.6f; Ln== %+10.6f; V== %+10.6f\n",mLineKW[0],mLineKW[1],mLineKW[2]);

             // ��������� �������� - ���������� �������
             // ----------------------------------------------------------------------         
                mPParKW[0] = -VmaxOS/AmaxOS;
                mPParKW[1] =  AmaxOS*mPParKW[0]*mPParKW[0]/2+VmaxOS*mPParKW[0];
                printf(" # ��������� ��: t0=  %+10.6f; P0==  %+10.6f\n",mPParKW[0],mPParKW[1]);
                printf(" ========================================================================\n");    
           
             // ���������� ��������� ��� ������� � "0" ������������ ������ ���������� �������
             // ----------------------------------------------------------------------               
                printf(" ���������� ����������� � <0> ������������ ������ ��:\n");
                printf(" ---------------------------------\n");
                printf(" * P(t)=Amax*t^2+P0;\n");
                printf(" * E(t)=A*{1-exp[-k*(t+t0)]};\n");
                printf(" * L(t)=V*(t+t0-Tn)+Ln;\n"); 
                printf(" ---------------------------------\n");
                
             // ���������� � ������ ���������� ������� ( + ���������)
             // EpsUch=0.01; // [���.����]                            
             // SpUchVar(code_ALR,EpsUch,dtt,mPParKW[1]);
            
             // ������ ��� ��������� (��������) !!!           
             // printf(" # NEXT (������� <����>) ==> "); gets(sss);

             // �������� ���������� ������� (� ����� ������������ �������� �������)
                t0=mPParKW[0];            
             // ������ ��������� ������� ������������ ������ �� tn=|t0|+Tn
                tn=fabs(t0)+mLineKW[0];   
             // ����� ��������� ������� ������������ ������ ��,�.�. tk=tn+(Lk-Ln)/V                                    
                tk=tn+fabs((mPLineKW[2]-mPLineKW[0])/mLineKW[2]); 
             // ���-�� ������ ���������� (f=10�� - �� ��������� ��� ���� CAN)
                Napp=abs((tk+1.0)/dtt);  
         
             // ������� �����         
             // printf(" Napp= %d; t0=%f; tn=%f; tk=%f; dtt=%f\n",Napp,t0,tn,tk,dtt);            
             // ������ ��� ��������� (��������) !!!           
                printf("=> "); gets(sss);      

             // ���������� � ������ ���������� ������� ( + ���������)
//              EpsUch=0.0001; // [���.����]                            
                SpUchVar(code_ALR,EpsUch,dtt,arc_OS+mPParKW[1]);
         
             // ���� �� �������� ������������ ������ �� (������� ������-�����)
                printf(" ------------------------------------------------------------------------\n");      
                printf(" ��������! ����������� ������ ������������ ������� � �����\n");
                printf("           �����...\n");                                     
                for (ikw=0; ikw<=Napp; ikw++){                        
                   ttek=ikw*dtt;               
                // ��������� �������
                   if (ttek<=fabs(t0)){
                      Utek=arc_OS+1.0/2.0*AmaxOS*ttek*ttek+mPParKW[1];
                      Vtek=AmaxOS*ttek;
                      Atek=AmaxOS;
                   // printf(" ikw= %d; ttek= %f; Utek= %f;\n",ikw,ttek,Utek);
                   }//if               
                // ������� ����������
                   if ((ttek>fabs(t0))&&(ttek<=tn)){
                      Utek=arc_OS+mPLineKW[1]*(1-exp(-mExpKW[1]*(ttek+t0)));
                      Vtek= mPLineKW[1]*mExpKW[1]*exp(-mExpKW[1]*(ttek+t0));
                      Atek=-mPLineKW[1]*mExpKW[1]*mExpKW[1]*exp(-mExpKW[1]*(ttek+t0));
                   // printf(" ikw= %d; ttek= %f; Utek= %f;\n",ikw,ttek,Utek);
                   }//if
                // �������� �������
                   if (ttek>tn){
                      Utek=arc_OS+mLineKW[2]*(ttek+t0-mLineKW[0])+mPLineKW[0];
                      Vtek=mLineKW[2];
                      Atek=0;
                   // printf(" ikw= %d; ttek= %f; Utek= %f;\n",ikw,ttek,Utek);
                   }//if               
                // ������ � �����
                   mBufKW[ikw][0]=ttek; // ������� �����
                   mBufKW[ikw][1]=Utek; // ���������
                   mBufKW[ikw][2]=Vtek; // ���������� ��������  
                   mBufKW[ikw][3]=Atek; // ���������
                }//ikw - ���� �� �������� ������������ ������ �� (������� ������-�����)         
                printf(" OK! ====> ��������� ������ ������������ �������.\n");                  
                printf(" ------------------------------------------------------------------------\n");
         
             // ������� (������������) ���� ������
             // FILE *f_KW;
             // ������������ ��� ����� ��� �����������     
                char my_Str[80]="kw_";  my_Str[3]=code_ALR;       
                my_Str[4]='_';          my_Str[5]=sTek[0];                        
                if (jkw==0) my_Str[6]='L'; else my_Str[6]='R';            
                strcat(my_Str,".txt");            
             // ������� ����                
                f_KW=fopen(my_Str,"w");                           
             // ������� ����� ���������� ���� ���
                nReg=-1;      
                printf(" ������������� ������ ���������� ���� ���:\n");
                printf(" ------------------------------------------------\n");                
                printf("  0 - �� �������� (������� �������� �����������)\n");
                printf("  1 - �� �������� (��������� ����������� �� ����)\n");
                printf("  2 - �� ��������� �� ��������� ���\n");        
                printf(" ------------------------------------------------\n");                                
                printf("  ? ���� (����� ������ => "); 
                gets(sReg);       
                sscanf(sReg,"%d",&nReg);              
             // ====================================================================
             // �������� ������� ����� ���������� "nReg"
                my_ERR=false;
                for (ikw=0; ikw<=2; ikw++) if (nReg==ikw){ my_ERR=true;break; };          
                if (!my_ERR){
                   PrMsg("������ ����� ������, ������������ ������ �������");                   
                   break;// ������ ����� �� ����������� ���� ������!                   
                // goto mEND;
                };//if
             // ====================================================================                    
             // ��������� ������������ ������� �� ��������� (����������� ��������)      
                for (ikk=0; ikk<6; ikk++) v6_ALR[ikk]=999.0;
           
             // ���������� ��� �������� ������������ �� (�������������� ���������)
                if (IMIT){
                   arc_GSS = my_random1(pDataKW[nTek][jkw+jkw],pDataKW[nTek][1+jkw+jkw]);
                   printf("    arc_KW= %f; CKO= %f; arc_GSS= %f;\n",
                            pDataKW[nTek][jkw+jkw],pDataKW[nTek][1+jkw+jkw],arc_GSS);    
/*                                                                                  
                // ������ ��� ������� !!!
                // ------------------------------------------------------------
                   Napp=150;                
                   if (jkw==0) arc_GSS=-45.0; else arc_GSS=45.0;                
                   if (!(code_ALR=='A')) arc_GSS=arc_GSS+90.0;
                   printf("    arc_KW= %f; CKO= %f; arc_GSS= %f;\n",
                         pDataKW[nTek][jkw+jkw],pDataKW[nTek][1+jkw+jkw],arc_GSS);    
                // ------------------------------------------------------------                         
*/                                       
                   printf("=> "); gets(sss);      
                };//(IMIT)                         

             // !!!
             // ������ ����� ������ �������-������ (������� �� 1 ��.������)
             // ===============================================================
                printf(" ��������! ����������� ������ ������������ ����� ������.\n");
                prExeKW=false;
                for (ikw=0; ikw<Napp; ikw++){                   
                
                // !!!                
                // ������ ������ ������
                // ------------------------------------------------------------                
                   ttek=mBufKW[ikw][0]; // ������� ������ �������
                   Utek=mBufKW[ikw][1]; // ������� ���������                          
                   Atek=mBufKW[ikw][3]; // ������� ���������

                // !!!                                                                  
                // ������ ��� ������ ����� - ������ ���������� �������
                // ------------------------------------------------------------                                                                                                
                   if (ikw==0){
                      printf(" ��������! ���������� ���� ��� <");
                      if (nReg==2) printf("�� ���������>\n");               
                      else         printf("�� ��������>\n");                              
                      printf("           �����...\n");                         
                      printf("=> "); gets(sss);                         
                   };//if
                   
                // !!!                                   
                // �������� �������� ���������� (������� ����������)        
                // ------------------------------------------------------------                
                   if (ttek<=fabs(t0)) printf(" ��=> ");             // ��������� �������                
                   if ((ttek>fabs(t0))&&(ttek<=tn)) printf(" ��=> ");// ������� ����������                
                   if (ttek>tn) printf(" ��=> ");                    // �������� �������

                // !!!                                   
                // �������� ���� ��� ��� (���������)                
                // ------------------------------------------------------------                
                   switch (code_ALR){
                       case 'A': arc_OS=cd[0]; break;
                       case 'L': arc_OS=cd[1]; break;
                       case 'R': arc_OS=cd[2]; break;     	            
                   }//switch

                // !!!                                   
                // ������ � ���� (Vtek - ����������� � �����) 
                // ------------------------------------------------------------                
                   fprintf(f_KW,"%20.10f",ikw*1.0);   
                   fprintf(f_KW,"%c",' ');   
                // ----------------------------------- 
                   fprintf(f_KW,"%20.12f",ttek);   
                   fprintf(f_KW,"%c",' ');   
                // ----------------------------------- 
                   fprintf(f_KW,"%20.12f",Utek);   
                   fprintf(f_KW,"%c",' ');   
                // -----------------------------------  
                   fprintf(f_KW,"%20.12f",arc_OS);   
                   fprintf(f_KW,"%c",' ');   
                // ----------------------------------- 
                   fprintf(f_KW,"%20.12f",arc_OS-Utek);   
                   fprintf(f_KW,"%c",' ');   
                // -----------------------------------                
                   fprintf(f_KW,"%20.12f",Vtek);   
                   fprintf(f_KW,"%c",' ');   
                // -----------------------------------                
                   fprintf(f_KW,"%20.12f",Atek);   
                   fprintf(f_KW,"%c",'\n');   
                // -----------------------------------                

                // !!!                                   
                // ������������ ��������������� �������� ������������
                // ------------------------------------------------------------                                                
                // �������� �������� �� ����������� ...!?
                // ��� �������� ��� ������ ���� ������������� ��������� �� CAN �� ��� (��)
                   if ( (IMIT)&&(((jkw==0)&&(arc_OS<arc_GSS))||((jkw==1)&&(arc_OS>arc_GSS))) 
                      ) mmCanKW[nTek][NumOs].mLR[jkw]=1;

                // !!!                                   
                // ��������� ����� - �������� ������� � ������������ �� (CAN)
                // ------------------------------------------------------------                                                                
                   if (mmCanKW[nTek][NumOs].mLR[jkw]==1){
                      prExeKW=true;
                   // ��������� ���      
                      skorcsn(0.0, 0.0, 0.0);
                      for (ikk=0; ikk<6; ikk++) v6_ALR[ikk]=999.0; // ��������
                      printf(" ------------------------------------------------------------------------\n");                      
                      printf(" ��������! ���������� - ������� ��\n");                 
                      printf(" ��� Stop! A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);                                           
                      printf(" ����. --> Vtek=%10.6f;\n",Vtek);                                                            
                      Vtek=-0.1*Vtek;
                      printf(" ����. <-- Vtek=%10.6f;\n",Vtek);                                                            
                      printf(" ......... ����� �� �������� ����� (�������� �������)\n");                  
                      switch (code_ALR){
                          case 'A':v6_ALR[0]=mBufKW[ikw+1][1]; v6_ALR[3]=Vtek; break;
                          case 'L':v6_ALR[1]=mBufKW[ikw+1][1]; v6_ALR[4]=Vtek; break;
                          case 'R':v6_ALR[2]=mBufKW[ikw+1][1]; v6_ALR[5]=Vtek; break;                 
                      }//switch
                      break;
                   };//if    

                // !!!                                   
                // �������� ������ (������������ ���������� ��������)
                // ------------------------------------------------------------                                                                
                   Vtek=mBufKW[ikw][2]; // �� ������ ������ - ��� ���
                   if (!(nReg==0)) Vtek=(mBufKW[ikw+1][1]-arc_OS)/(mBufKW[ikw+1][0]-mBufKW[ikw][0]);

                // !!!                                   
                // ����������� �����                        
                // ------------------------------------------------------------                                
                   printf("CAN= %d; ikw= %d; arc_OS=%10.6f; Utek=%10.6f; Del=%10.6f;\n",
                          mmCanKW[nTek][NumOs].mLR[jkw],ikw,arc_OS,Utek,arc_OS-Utek);          
                          
                // !!!                                                      
                // �������� ���� ��� ��� (������������ ������������ ���.�������)
                // ------------------------------------------------------------                                                                                
                   switch (code_ALR){
                       case 'A':v6_ALR[0]=mBufKW[ikw+1][1]; v6_ALR[3]=Vtek; break;
                       case 'L':v6_ALR[1]=mBufKW[ikw+1][1]; v6_ALR[4]=Vtek; break;
                       case 'R':v6_ALR[2]=mBufKW[ikw+1][1]; v6_ALR[5]=Vtek; break;                 
                   }//switch

                // !!!                                                                  
                // ���������� ���������� ���� ���
                // ------------------------------------------------------------                                                                                                                
//!!!    
if (mmCanKW[nTek-1][NumOs].mLR[jkw]==1){
   v6_ALR[NumOs+3]=0.00001;
   skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);
   usleep(dtt*1000000);
}else{
                if (mmCanKW[nTek][NumOs].mLR[jkw]==0){
//                 if ((nReg==2)&&(ttek<tn)){
                   if (nReg==2){                   
                      perebroscsn(v6_ALR[0],v6_ALR[1],v6_ALR[2]);
                      RESCSN();
                   }else{
                   // ���������� �� �������� (��������� ��� ��������� ����.)               
//                    if (nReg==2)
                      skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);
                      usleep(dtt*1000000);            
                   };//if
                   }
}                   
             // !!!
             // ����� ����� ������ �������-������ (������� �� 1 ��.������)
             // ===============================================================                   
                }//ikw - ���� ������ �������-������ (������� �� 1 ��.������)

             // ������� ����
                fclose(f_KW);                               
                printf(" ------------------------------------------------------------------------\n");           
                printf(" OK! ====> ��������� ������ ������������ ����� ������.\n");                  
                printf(" OK! ====> ��������� ������������ �������� ��������� ��� �������� ��\n");           
             // ���� ������� �� �� �������� (���������� ��� �������� ���!)
                if (!prExeKW){
                   printf(" ��������! ������� �� � ������� ��������� �� ���������...\n");
                   printf(" ------------------------------------------------------------------------\n");                   
                };//(!prExeKW)
                
             // !!!           
             // ������� ������������ ��
             // ------------------------------------------------------------                                                                             
                if (prExeKW){                      
                   printf(" ------------------------------------------------------------------------\n");     
                   printf(" ��������! ������ �� ��������� �������� ��\n");
                   printf(" ��.�����  [CAN= %d] A=%10.6f; L=%10.6f; R=%10.6f;\n",mmCanKW[nTek][NumOs].mLR[jkw],cd[0],cd[1],cd[2]);                          
                   printf(" ------------------------------------------------------------------------\n");                   
                // ScrCanKW(false,nInpKW,mmCanKW);                              
                // printf("CAN= %d\n"+,mmCanKW[nTek][NumOs].mLR[jkw]);                          
                   printf(" => ��������� ��������� ���������? (y->��;n->���)\n");             
                   printf(" => "); gets(sss);               
                   if ((sss[0]=='y')||(sss[0]=='Y')){         
                      printf("           �����...\n");      
                      skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);                      
                      ikw=0;        
                   // �� ������ ������ ����������� �� ���-�� �������� (50-�������)
                      while (ikw<=Niteration){            
                          ikw=ikw+1;
//                          skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);
                          usleep(dtt*1000000);                                            
                       // �������� ���� ��� ��� (���������)                                 
                          switch (code_ALR){
                              case 'A': arc_OS=cd[0]; break;
                              case 'L': arc_OS=cd[1]; break;
                              case 'R': arc_OS=cd[2]; break;
                          }//switch                                                            
                          printf("         # ikw=%d; A=%10.6f; L=%10.6f; R=%10.6f;\n",ikw,cd[0],cd[1],cd[2]);                                     
                          
                       // !!!                                   
                       // ������������ ��������������� �������� ������������
                       // ------------------------------------------------------------                                                                          
                       // �������� �������� �� ����������� (�����) ...!?
                          if ( (IMIT)&&(((jkw==0)&&(arc_OS>arc_GSS))||((jkw==1)&&(arc_OS<arc_GSS))) 
                             ) mmCanKW[nTek][NumOs].mLR[jkw]=0;

                       // !!!                                   
                       // ��������� ����� - �������� ������� � ������������ �� (CAN)
                       // ------------------------------------------------------------                                                                
                          if (mmCanKW[nTek][NumOs].mLR[jkw]==0){
                             skorcsn(0.0,0.0,0.0);                 
                             printf(" ------------------------------------------------------------------------\n");                        
                             printf(" ��������! ������ �� ��������� �������� �� + ���������\n");
                             printf(" ��.�����  [CAN= %d] A=%10.6f; L=%10.6f; R=%10.6f;\n",mmCanKW[nTek][NumOs].mLR[jkw],cd[0],cd[1],cd[2]);                                        
                             printf(" ------------------------------------------------------------------------\n");                                     
                          // ScrCanKW(false,nInpKW,mmCanKW);                                                           
                          // printf("CAN= %d\n",mmCanKW[nTek][NumOs].mLR[jkw]);
                             break;
                          };//if                            
                      };//while  
                   // ��������� ��� ���
                      skorcsn(0.0,0.0,0.0);                
                   // ���������� ���-�� �������� ��� ����������                         
                      if (ikw==Niteration) return 7;                   
                   }else{
                   // printf(" ------------------------------------------------------------------------\n");                                      
                      printf(" => ��������� �� �����������\n");                                                   
                      printf(" ------------------------------------------------------------------------\n");                        
                   };//if        
                
                // !!!           
                // ������ ������ � ���������-�����?
                // ------------------------------------------------------------                                                                                               
                   printf(" => �������� ������ � ���������-�����? (y->��;n->���)");
                   printf(" => "); gets(sss); 
                   if ((sss[0]=='y')||(sss[0]=='Y')){         
                   // ����������!!!               
                   // �������� ���� ��� ���
                      ikw=-1;
                      switch (code_ALR){
                          case 'A': ikw=0; arc_OS=cd[0]; break;
                          case 'L': ikw=1; arc_OS=cd[1]; break;
                          case 'R': ikw=2; arc_OS=cd[2]; break;     	            
                      }//switch  
                   // ������� ���������� ������
                      prDataBuf[nTek][ikw].mLR[jkw]=1;
                   // ������������ ������ ������               
                      if (jkw==0){
                         mDataBuf[nTek][ikw].stDirL.val=arc_OS;
                      // ���� ����� ������ ��������     
                         mDataBuf[nTek][ikw].stDirL.cko=mDataKW[nTek][ikw].stDirL.cko;
                      }else{
                         mDataBuf[nTek][ikw].stDirR.val=arc_OS;
                      // ���� ����� ������ ��������     
                         mDataBuf[nTek][ikw].stDirR.cko=mDataKW[nTek][ikw].stDirL.cko;                 
                      };//if               
                          
                      printf(" ------------------------------------------------------------------------\n");                        
                      printf(" ��������! ������ ������ � ���������-����� ���������\n");
                      printf(" ------------------------------------------------------------------------\n");
                   };//if  
                   prExeKW = false;// ��� ���������� � ����������� �����!              
                   
                };//if(prExeKW) - �������� ������������ ��

         // ######################################################################           
            };//jkw - ���� �� ������ � ������� ��
         // ######################################################################                     
     
         // ������� �� ������ ����� ������ (����� ������) � ���������� ����� �� <jkw>
            if (!my_ERR) continue;
 mEND:;

         // ����������� ���������-������ ������ ��� �� �� �����
            printf(" ------------------------------------------------------------------------\n");              
            printf(" ����������� ������� ���������-������ ������ ��� �� �� ����� ? (y->��;n->���)\n");
            printf(" => "); gets(sss); 
            if ((sss[0]=='y')||(sss[0]=='Y')) 
               ScrDataKW(nInpKW,mDataBuf);            
               
         // ��������� ���������� (����������) ��������� � ������
            printf(" ------------------------------------------------------------------------\n");              
            printf(" ��������� ���������� (����������) ���������-������ ? (y->��;n->���)\n");
            printf(" => "); gets(sss); 
            if ((sss[0]=='y')||(sss[0]=='Y')){                   
               for (ikw=0; ikw<nInpKW; ikw++){
                  for (jkw=0; jkw<3; jkw++){
                     if (!prDataBuf[ikw][jkw].mLR[0]){
                        mDataBuf[ikw][jkw].stDirL.val = mDataKW[ikw][jkw].stDirL.val;
                        mDataBuf[ikw][jkw].stDirL.cko = mDataKW[ikw][jkw].stDirL.cko;
                     };//if     
                     if (!prDataBuf[ikw][jkw].mLR[1]){                  
                        mDataBuf[ikw][jkw].stDirR.val = mDataKW[ikw][jkw].stDirR.val;
                        mDataBuf[ikw][jkw].stDirR.cko = mDataKW[ikw][jkw].stDirR.cko;
                     };//if                  
                  };//jkw
               };//ikw                    
               printf(" OK! ====> ��������� ������ ���������� ���������-������\n");
            };//if - ��������� ���������� (����������) ��������� � ������
            
         // �����������  ����������� ���������-������ ������ ��� �� �� �����
            printf(" ------------------------------------------------------------------------\n");              
            printf(" ����������� ���������-������ �������� ������ ��� �� �� ����� ? (y->��;n->���)\n");
            printf(" => "); gets(sss); 
            if ((sss[0]=='y')||(sss[0]=='Y'))          
               ScrDataKW(nInpKW,mDataBuf);
                     
         // ������������ ����� �� - <DataBF.txt>    
            FileDataBF(nInpKW,mDataBuf);

         // ����� �������� ��� �������� ��������� ���� ���� ���
            printf(" ------------------------------------------------------------------------\n");              
            printf(" ����� ��������� ��� ���� ��� �� ����� ? (y->��;n->���)\n");
            printf(" => "); gets(sss); 
            if ((sss[0]=='y')||(sss[0]=='Y'))                   
               OutDelWork(nInpKW,mDataBuf,mConfKW);

         // ��������� ��� ��� ��������� ��� � ��������� �� ���������
            printf(" ��� >>>>> ��������� ��� ��� � ��������� �� ��������� - ");                        
            for (ikw=0; ikw<6; ikw++) v6_ALR[ikw]=999.0;// ��������
            switch (code_ALR){
                case 'A': NumOs=0;v6_ALR[0]=kw_A0; printf("%+006.1f",v6_ALR[0]); break;
                case 'L': NumOs=1;v6_ALR[1]=kw_L0; printf("%+006.1f",v6_ALR[1]); break;
                case 'R': NumOs=2;v6_ALR[2]=kw_R0; printf("%+006.1f",v6_ALR[2]); break;                 
            }//switch                               
         // ���������� ��������� ���
            printf(" [���.����]\n");     
            printf("           ����� ��� - <�� ���������>\n");        
            printf("           �����... \n");      
            perebroscsn(v6_ALR[0],v6_ALR[1],v6_ALR[2]);
            RESCSN();
            printf(" OK! ====> A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);
            printf(" ------------------------------------------------------------------------\n");
        
         // printf(" ------------------------------------------------------------------------\n");              
            printf(" ����� �� ����� ������ ���������� ��������? (y->��;n->���)\n");
            printf(" => "); gets(sss); 
            if ((sss[0]=='y')||(sss[0]=='Y')) break; else continue;                
        
       };//������ ����������� ���� ������ - ����� ������ ������ ���������� �������� 
    // �������� ����� �� ���������
      return 0;
   }//void ModKW() - ������ ������������� ������ ���������� ��������
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 

// ������������ ��� �������
   void InterXI2(int pkw0,int pkw00, 
                 float pkw1,float pkw01,float pkw2,float pkw02,
                 float pkw3,float pkw03,float pkw4,float pkw04,double mDataXI2[][4]){
      int iz,kz;
      for (iz=1; iz<=(pkw0-pkw00); iz++){
         kz=pkw00+iz;
         mDataXI2[kz][0]=pkw01+iz*(pkw1-pkw01)/(pkw0-pkw00);  
         mDataXI2[kz][1]=pkw02+iz*(pkw2-pkw02)/(pkw0-pkw00);               
         mDataXI2[kz][2]=pkw03+iz*(pkw3-pkw03)/(pkw0-pkw00);  
         mDataXI2[kz][3]=pkw04+iz*(pkw4-pkw04)/(pkw0-pkw00);  
//       printf(" kz=%d; k1=%f; k2=%f; k3=%f; k4=%f;\n",kz,
//                mDataXI2[kz][0],mDataXI2[kz][1],
//                mDataXI2[kz][2],mDataXI2[kz][3]);
      };//iz           
   };//InterXI2()              

// ������������� � ������ ������ �� - DataXI2.txt
   int ReadXI2(char *myFile, double mDataXI2[][4],int *N){ 
     char sss[256];
     int   pkw0,pkw00;
     float pkw1,pkw2,pkw3,pkw4;
     float pkw01,pkw02,pkw03,pkw04;
     int iz,jz,kz;
     FILE *f_Data;
     f_Data=fopen(myFile,"r");     
     
     if (!f_Data){
     // printf(" ------------------------------------------------------------------------\n");       
        printf(" ��������! ����������� ���� �� (������� ������������� XI^2) <DataXI2.txt>\n");
        printf(" ------------------------------------------------------------------------\n");          
        return 1;
      };//(!f_Data)
   // printf(" ------------------------------------------------------------------------\n");        
      printf(" OK! ====> ��������� ���� �� (������� ������������� XI^2) => <DataXI2.txt>\n");
      printf(" ......... ������ ����� � ������������ ��������� ������� ������\n");        
   // printf(" ------------------------------------------------------------------------\n");  
        
   // ������ ����� � ������������ ���������       
   // -----------------------------------------------------------     
      kz=0;
      while (kz<20){
         mDataXI2[kz][0]=0;
         mDataXI2[kz][1]=0;
         mDataXI2[kz][2]=0;
         mDataXI2[kz][3]=0;
//       printf(" kz=%d; k1=%f; k2=%f; k3=%f; k4=%f;\n",
//                kz,
//                mDataXI2[kz][0],mDataXI2[kz][1],
//                mDataXI2[kz][2],mDataXI2[kz][3]);                             
        kz=kz+1;                                                   
      };//while        
   // -----------------------------------------------------------        
      iz=0;
      while (fscanf(f_Data,"%d %f %f %f %f\n",&pkw0,&pkw1,&pkw2,&pkw3,&pkw4)!=EOF){
        if (pkw0<=55){
           mDataXI2[kz][0]=pkw1;  
           mDataXI2[kz][1]=pkw2;               
           mDataXI2[kz][2]=pkw3;  
           mDataXI2[kz][3]=pkw4;  
           if (pkw0>=500) printf(" kz=%d; k1=%f; k2=%f; k3=%f; k4=%f;\n",
                                   kz,
                                   mDataXI2[kz][0],mDataXI2[kz][1],
                                   mDataXI2[kz][2],mDataXI2[kz][3]);              
             kz=kz+1;                        
        };//if (kz<=55)
     // -----------------------------------------------------------                            
        if (pkw0==60) InterXI2(pkw0,pkw00,pkw1,pkw01,pkw2,pkw02,
                               pkw3,pkw03,pkw4,pkw04,mDataXI2);            
        if (pkw0==70) InterXI2(pkw0,pkw00,pkw1,pkw01,pkw2,pkw02,
                               pkw3,pkw03,pkw4,pkw04,mDataXI2);            
        if (pkw0==80) InterXI2(pkw0,pkw00,pkw1,pkw01,pkw2,pkw02,
                               pkw3,pkw03,pkw4,pkw04,mDataXI2);            
        if (pkw0==90) InterXI2(pkw0,pkw00,pkw1,pkw01,pkw2,pkw02,
                               pkw3,pkw03,pkw4,pkw04,mDataXI2);            
        if (pkw0==100) InterXI2(pkw0,pkw00,pkw1,pkw01,pkw2,pkw02,
                               pkw3,pkw03,pkw4,pkw04,mDataXI2);            
        if (pkw0==120) InterXI2(pkw0,pkw00,pkw1,pkw01,pkw2,pkw02,
                               pkw3,pkw03,pkw4,pkw04,mDataXI2);            
        if (pkw0==150) InterXI2(pkw0,pkw00,pkw1,pkw01,pkw2,pkw02,
                               pkw3,pkw03,pkw4,pkw04,mDataXI2);            
        if (pkw0==175) InterXI2(pkw0,pkw00,pkw1,pkw01,pkw2,pkw02,
                               pkw3,pkw03,pkw4,pkw04,mDataXI2);            
        if (pkw0==200) InterXI2(pkw0,pkw00,pkw1,pkw01,pkw2,pkw02,
                               pkw3,pkw03,pkw4,pkw04,mDataXI2);            
        if (pkw0==250) InterXI2(pkw0,pkw00,pkw1,pkw01,pkw2,pkw02,
                               pkw3,pkw03,pkw4,pkw04,mDataXI2);            
        if (pkw0==300) InterXI2(pkw0,pkw00,pkw1,pkw01,pkw2,pkw02,
                               pkw3,pkw03,pkw4,pkw04,mDataXI2);            
        if (pkw0==350) InterXI2(pkw0,pkw00,pkw1,pkw01,pkw2,pkw02,
                               pkw3,pkw03,pkw4,pkw04,mDataXI2);            
        if (pkw0==400) InterXI2(pkw0,pkw00,pkw1,pkw01,pkw2,pkw02,
                               pkw3,pkw03,pkw4,pkw04,mDataXI2);            
        if (pkw0==450) InterXI2(pkw0,pkw00,pkw1,pkw01,pkw2,pkw02,
                               pkw3,pkw03,pkw4,pkw04,mDataXI2);            
        if (pkw0==500) InterXI2(pkw0,pkw00,pkw1,pkw01,pkw2,pkw02,
                               pkw3,pkw03,pkw4,pkw04,mDataXI2);            
     // -----------------------------------------------------------                                                                 
     // ����� ������
        iz=iz+1;        
        pkw00=pkw0;
        pkw01=pkw1;
        pkw02=pkw2;
        pkw03=pkw3;
        pkw04=pkw4;           
      };//while                
      fclose(f_Data); 
   // -----------------------------------------------------------        
   // ��������� ���� ������ � ������ ���������������� ��������
      printf(" ......... ������������ ������� ������������� <XI2> � ������ ������������\n");             
      f_Data=fopen("dan/mDataXI2.txt","w");     
      kz=0;
      for (jz=1; jz<=26; jz++){
        for (iz=1; iz<=20; iz++){          
            // ������ � ����            
            // -------------------------------------
               fprintf(f_Data,"%d",kz);   
               fprintf(f_Data,"%c",' ');   
            // ----------------------------------- 
               fprintf(f_Data,"%8.6f",mDataXI2[kz][0]);   
               fprintf(f_Data,"%c",' ');   
            // ----------------------------------- 
               fprintf(f_Data,"%8.6f",mDataXI2[kz][1]);   
               fprintf(f_Data,"%c",' ');   
            // ----------------------------------- 
               fprintf(f_Data,"%8.6f",mDataXI2[kz][2]);   
               fprintf(f_Data,"%c",' ');   
            // ----------------------------------- 
               fprintf(f_Data,"%8.6f",mDataXI2[kz][3]);   
               fprintf(f_Data,"%c",'\n');   
            // -----------------------------------                       
/*            
               printf(" kz=%d; k1=%f; k2=%f; k3=%f; k4=%f;\n",              
                        kz,
                        mDataXI2[kz][0],mDataXI2[kz][1],
                        mDataXI2[kz][2],mDataXI2[kz][3]);                               
*/                        
               if (kz==500) break;
               kz=kz+1;                  
        };//iz           
//      printf(" => "); gets(sss); 
      };//jz                   
      fclose(f_Data);            
   // -----------------------------------------------------------             
   // printf(" ------------------------------------------------------------------------\n");      
      printf(" OK! ====> ��������� ������ ������������ ������� ������������� <XI2>\n");
      printf(" ------------------------------------------------------------------------\n");              
   // printf("\n");
      return 0;
   };//ReadXI2()

// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ 
// ������ ������������� ������ ���������� ��������
// ������������ ����� ������ � �������������� XI2
   int ModXI2(){
    // =======================================================================
    // ������ �������� �������������� ����������
    // =======================================================================     
    // ������� ������� �������   
       bool prCP=false;      
    // ���������� ������� ������� ������   
       bool my_ERR=false;   
    // ���������� �����
       int iz,jz,kz,N;   
    // ������ ������������� <k>   
       double mDataXI2[501][4];
       
    // �������������,������ � ������������ ������ �� - DataXI2.txt
       printf(" ------------------------------------------------------------------------\n");      
       printf(" ��������! ����� � ������ ������ �� - <DataXI2.txt>\n");
    // printf(" ------------------------------------------------------------------------\n");                   
    // my_ERR=false;
       if (ReadXI2("DataXI2.txt",mDataXI2,&N))
       // printf(" ------------------------------------------------------------------------\n");      
       // printf(" OK! ====> ��������� ������ ������������ ������� ������������� <XI2>\n");
       // printf(" ------------------------------------------------------------------------\n");              
       // printf("\n");
       return 1;

       return 0;
   }//void ModXI2() - ������������ ����� ������ � �������������� XI2
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\   


// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ 
// �������� ��������� ���������� ���.�����
   void ImSTAR(int Nmax,double ZVmin,     double ZVmax,
                        double UG1min,    double UG1max,    
                        double UG2min,    double UG2max,                       
                        double CkoZVkat,  double CkoZVvar,   
                        double CkoUG1kat, double CkoUG1var,
                        double CkoUG2kat, double CkoUG2var, int prPrint){
//                        struct stZV mDataStar[],
//                        int    prPrint){
    // struct stZV mDataStar[501];                        
    // ������ ���������� �����   
       struct stZV mDataStar[501];    
    // ���� ������ Nz<20, �� ������ ����������� �� ��������� Nz=20
       if (Nmax<=20) Nmax=21;
    // ���������� �����   
       char sss[256];           
    // ����� ����� ������
       FILE *f_Data;   
    // ������������ ���������
       double Pi,Pi2,gr,mr,sr;
       Pi = 3.1415926;
       Pi2= 2.0*Pi;
       gr = Pi/180.0;
       mr = gr/60.0;
       sr = mr/60.0;
    // ���������� �����
       int iz,jz,kz,Nz,js;   
    // ��������� ������ ����� ������   
       int   pkw0;
       float pkw1,pkw2,pkw3,pkw4,pkw5,pkw6;           
    // ���������� ������������ ������� ����� ��� ��.��������  
       int Nkat;
    // ��� ��.��������   
       double ZVkat;// �������
       double ZVvar;// ���������
    // ��� ���� - �������
       double UgKat1,UgKat2;// �������   - ������ ����������� � ���������
       double UgVar1,UgVar2;// ��������� - ������ ����������� � ���������    
       double ZV0,dZV;          
       dZV=(ZVmax-ZVmin)/2.0;
       ZV0=ZVmin+dZV;
       double dUG;       
              
    // ������������ ������� ������� (����������) �����
    // printf("\n");                
    // ���������� ���������� �����
       printf(" ------------------------------------------------------------------------\n");      
       printf(" ��������! ��������� ���. ���. ����� � ���� ��������������� (Nmax=%d)\n",Nmax);
       Nkat=0;       
       while (Nkat<20){ 
          Nkat=rand()%Nmax;
       // printf(" ========> %d\n",Nkat);          
       };//while
       printf(" OK! ====> ��������� ������ ������������ ���. ���. ����� � ���� ���-��\n");
       printf("           Nkat= %d\n",Nkat);       
    // printf(" ------------------------------------------------------------------------\n");              
       
    // ��������� ��� ���������
       if (prPrint){ printf(" => "); gets(sss);}

    // ��������� ��.�������� ���.������          
    // printf(" ------------------------------------------------------------------------\n");      
       printf(" ��������! ��������� ���������� ��� ����� � ���� ���������������\n");
       printf("           �����...\n");       
       if (prPrint) printf("           ------------------------------------------------------------\n");                     
       for (jz=1; jz<=Nkat; jz++){       
       // ���������� �������� ������ 
          ZVkat=my_random1(ZV0,dZV);          
          mDataStar[jz-1].mZV[0][0]=ZVkat;       
       // ���������� �������� 1-�� � 2-�� ����� 
          dUG=(UG1max-UG1min)/2.0;
          UgKat1=my_random1(UG1min+dUG,dUG);          
          mDataStar[jz-1].mZV[0][1]=UgKat1;                     
       // ----------------------------------
          dUG=(UG2max-UG2min)/2.0;          
          UgKat2=my_random1(UG2min+dUG,dUG);      
          mDataStar[jz-1].mZV[0][2]=UgKat2;                         
       // ���������� �������� ������       
          ZVvar=my_random1(ZVkat,CkoZVvar);
          mDataStar[jz-1].mZV[1][0]=ZVvar;       
       // ���������� �������� 1-�� � 2-�� ����� 
          UgVar1=my_random1(UgKat1,CkoUG1var);        
          mDataStar[jz-1].mZV[1][1]=UgVar1;                       
       // ----------------------------------          
          UgVar2=my_random1(UgKat2,CkoUG2var);      
          mDataStar[jz-1].mZV[1][2]=UgVar2;               
          if (prPrint){
             printf("        j=%03d; ���: ��.= %+010.6f;  Ug1= %+011.6f;  Ug2= %+011.6f;\n",jz, mDataStar[jz-1].mZV[0][0], mDataStar[jz-1].mZV[0][1]/gr, mDataStar[jz-1].mZV[0][2]/gr);
              printf("               ���: ��.= %+010.6f;  Ug1= %+011.6f;  Ug2= %+011.6f;\n",    mDataStar[jz-1].mZV[1][0], mDataStar[jz-1].mZV[1][1]/gr, mDataStar[jz-1].mZV[1][2]/gr);          
             printf("             ------------------------------------------------------------\n");                 
          };//if          
       };//jz               
       printf(" OK! ====> ��������� ������ ������������ ���������� ����� � ���� ���-��  \n");
    // printf(" ------------------------------------------------------------------------\n");                         
    // ��������� ��� ���������
    // if (prPrint){ printf(" => "); gets(sss);}

    // ������ � ����                   
       printf(" ��������! ������������ ����� ������ � ����������� �����\n");
       printf("           �����...\n");                  
       f_Data=fopen("DataStar.txt","w");           
       for (jz=0; jz<Nkat; jz++){
      // -------------------------------------
         fprintf(f_Data,"%d",jz);   
         fprintf(f_Data,"%c",' ');   
      // ===================================                             
         fprintf(f_Data,"%12.8f",mDataStar[jz].mZV[0][0]);   
         fprintf(f_Data,"%c",' ');   
      // -----------------------------------                       
         fprintf(f_Data,"%12.8f",mDataStar[jz].mZV[0][1]);   
         fprintf(f_Data,"%c",' ');   
      // -----------------------------------                       
         fprintf(f_Data,"%12.8f",mDataStar[jz].mZV[0][2]);   
         fprintf(f_Data,"%c",' ');   
      // ===================================                    
         fprintf(f_Data,"%12.8f",mDataStar[jz].mZV[1][0]);   
         fprintf(f_Data,"%c",' ');   
      // -----------------------------------                       
         fprintf(f_Data,"%12.8f",mDataStar[jz].mZV[1][1]);   
         fprintf(f_Data,"%c",' ');   
      // -----------------------------------                       
         fprintf(f_Data,"%12.8f",mDataStar[jz].mZV[1][2]);   
         fprintf(f_Data,"%c",'\n');         
      // -------------------------------------         
       };//jz                   
       fclose(f_Data);            
       printf(" OK! ====> ��������� ������ ������������ ����� ������\n");
       printf("           ���� ������ � ��������!\n");       
       printf(" ------------------------------------------------------------------------\n");
       printf(" ��������� ������ ������������ ���������� ��.��������\n");                           
       printf(" ------------------------------------------------------------------------\n");       
   };// �������� ��������� ���������� ���.�����

// ������ ������������� ������ ���������� ��������

// �������� ��������� ���������� ���.�����
   int ModKAT(){
    // =======================================================================
    // ������ �������� �������������� ����������
    // =======================================================================     
       char sss[256];           
    // ������������ ���������
       double Pi,Pi2,gr,mr,sr;
       Pi =3.1415926;
       Pi2=2.0*Pi;
       gr =Pi/180.0;
       mr =gr/60.0;
       sr =mr/60.0;
    // ���������� ������ ������ ���������� (����� ������ �� �����)
       int  nScr=-1;
       char sScr[256];                    
    // ���������� ������ ������ ����������
       int  nReg=-1;
       char sReg[256];             
    // ������� ������� �������   
       bool prCP =false;      
    // ������� ����������� ��������� 
       bool prXI2=false;
    // �������� ��������� ������ ��� � ���������� ��������� (���� "<", �� "true")
       bool mPrZV =false;   
       bool mPrUG1=false;          
       bool mPrUG2=false;                 
    // ���������� ������� ������� ������   
       bool my_ERR=false;   
    // ���������� �����
       int iz,jz,kz,Nz,js;   
    // ��������� ������ ����� ������   
       int   pkw0;
       float pkw1,pkw2,pkw3,pkw4,pkw5,pkw6;       
    // ������ ������������� <k> ��� XI^2
       double mDataXI2[501][4];       
    // ������ ���������� �����   
       struct stZV  mDataStar[501];// ����������� ������� ������ (���.�������)
       struct stBUF mBufStar[501]; // ����������� � ������ ������ ����.���������  
    // ������-������ �������� ����
       double mMatStar[3]; // M[Xj-X��j];
    // ������-������ ������������ ����
       double mDispStar[3];// CKO���*CKO���=M{(Xj-X��j-M[Xj-X��j])*(Xj-X��j-M[Xj-X��j])};
    // ������-������ ��� ����
       double mCkoStar[3]; // CKO^���*CKO^���=CKO���*CKO���-CKO���*CKO���;
    // ��������������� ��������� ��� �������� ����������   
       double DeltaX,DeltaX1,DeltaX2,DeltaX3,DeltaX4;       
    // ���������� ������������ ������� ����� ��� ��.�������� (��������) 
       int Nmax=500;// �� �� ����� 20 ����!!!           
       int Nkat;   // ����������� �������� ��� ��������
       int Nend;   // ������� �������� (�� ������ ���������� ����.���)
    // ��� ���� - �������,�������
       double UgKat1,UgKat2;// �������
       double UgVar1,UgVar2;// ���������
    // ��� ��.�������� - �������,�������
       double ZVkat,ZVvar;       
    // �������� ��.�������
       double ZVmin= 7.0;//��.���   
       double ZVmax=16.0;//��.���          
       double ZV0,dZV,dUG1,dUG2;          
              dZV=(ZVmax-ZVmin)/2.0;// ���������� �������� ���������
              ZV0=ZVmin+dZV;        // ������� �������� ��� �������� ���������
    // �������� ���.�������
       double UG1min=   0.0*gr;// [���] // [���.����]  
       double UG1max= 360.0*gr;// [���] // [���.����]
    // ---------------------
       double UG2min=   0.0*gr;// [���] // [���.����]   
       double UG2max=  90.0*gr;// [���] // [���.����]                        
    // ---------------------       
       double dUG;
       
    // �������� ��������� ��    
       double CkoZVpor= 0.5;//��.���    
    // �������� �������� �� ��.�������� (�����,���������� ��������)       
       double CkoZVkat= 0.1;//��.���
    // ��������� �������� ��������� ��.�������� ���.������ � ������� ���
       double CkoZVvar= 1.0;//��.���
    
    // �������� ��������� �� �����
       double CkoUGpor;
       CkoUGpor=1.0*sr;           // [���] - (������ � ������� ��������)    
    // �������� �������� �� ����� (min,max)       
       double CkoUG1kat=0.06*sr;  // [���] - (������ � ������� ��������)
       double CkoUG2kat=0.06*sr;  // [���] - (������ � ������� ��������)       
    // ��������� �������� ��������� ���� ���.������ � ������� ���
       double CkoUG1var=0.5*sr;   // [���] - (������ � ������� ��������)       
       double CkoUG2var=0.5*sr;   // [���] - (������ � ������� ��������)       
    // ���������� <min> � <max> ��� �������� ������ (�������� ��������)       
       double minZV= 999.0;
       double maxZV=-999.0;       
    // --------------------   
       double minZVend= 999.0;
       double maxZVend=-999.0;       

    // ������� ����� ���������� �������� �������
       printf("\n");
       while (true){    
          nScr=-1;      
          printf(" ������� ����������� ������� ���������� ��������� �� �����:\n");
          printf(" ------------------------------------------------\n");                
          printf("  * 0 - ��� �����������.\n");
          printf("  * 1 - � ������������..\n");
          printf(" ------------------------------------------------\n");                                
          printf("  ? ���� (����� ������ => "); 
          gets(sScr);       
          sscanf(sScr,"%d",&nScr);              
       // ====================================================================
       // �������� ������� ����� ���������� "nReg"
          my_ERR=false;
          for (iz=0; iz<=1; iz++) if (nScr==iz){ my_ERR=true;break; };          
          if (!my_ERR){
             PrMsg("������ ����� ������, ������������ ������ �������");                   
             continue;
          };//if
          break;
       };//while       

    // ������������ ������� ������� (����������) �����
    // ===============================================
    // �������� ��������� ���������� ���.�����    
       ImSTAR(Nmax,ZVmin,ZVmax, UG1min,UG1max, UG2min, UG2max,                       
              CkoZVkat,CkoZVvar, CkoUG1kat,CkoUG1var, CkoUG2kat,CkoUG2var,
              nScr);                                  
    // ===============================================       
       return 0;
   };// int ModKAT(){   

// �������� ��������� ���������� ���.�����
   int ModSTAR(){
    // =======================================================================
    // ������ �������� �������������� ����������
    // =======================================================================     
       char sss[256];           
    // ������������ ���������
       double Pi,Pi2,gr,mr,sr;
       Pi =3.1415926;
       Pi2=2.0*Pi;
       gr =Pi/180.0;
       mr =gr/60.0;
       sr =mr/60.0;
    // ���������� ������ ������ ���������� (����� ������ �� �����)
       int  nScr=-1;
       char sScr[256];                    
    // ���������� ������ ������ ����������
       int  nReg=-1;
       char sReg[256];             
    // ������� ������� �������   
       bool prCP =false;      
    // ������� ����������� ��������� 
       bool prXI2=false;
    // �������� ��������� ������ ��� � ���������� ��������� (���� "<", �� "true")
       bool mPrZV =false;   
       bool mPrUG1=false;          
       bool mPrUG2=false;                 
    // ���������� ������� ������� ������   
       bool my_ERR=false;   
    // ���������� �����
       int iz,jz,kz,Nz,js;   
    // ��������� ������ ����� ������   
       int   pkw0;
       float pkw1,pkw2,pkw3,pkw4,pkw5,pkw6;       
    // ������ ������������� <k> ��� XI^2
       double mDataXI2[501][4];       
    // ������ ���������� �����   
       struct stZV  mDataStar[501];// ����������� ������� ������ (���.�������)
       struct stBUF mBufStar[501]; // ����������� � ������ ������ ����.���������  
    // ������-������ �������� ����
       double mMatStar[3]; // M[Xj-X��j];
    // ������-������ ������������ ����
       double mDispStar[3];// CKO���*CKO���=M{(Xj-X��j-M[Xj-X��j])*(Xj-X��j-M[Xj-X��j])};
    // ������-������ ��� ����
       double mCkoStar[3]; // CKO^���*CKO^���=CKO���*CKO���-CKO���*CKO���;
    // ��������������� ��������� ��� �������� ����������   
       double DeltaX,DeltaX1,DeltaX2,DeltaX3,DeltaX4;       
    // ���������� ������������ ������� ����� ��� ��.�������� (��������) 
       int Nmax=50;// �� �� ����� 20 ����!!!           
       int Nkat;   // ����������� �������� ��� ��������
       int Nend;   // ������� �������� (�� ������ ���������� ����.���)
    // ��� ���� - �������,�������
       double UgKat1,UgKat2;// �������   (������ ����������� � ���������)
       double UgVar1,UgVar2;// ��������� (������ ����������� � ���������)
    // ��� ��.�������� - �������,�������
       double ZVkat,ZVvar;       
    // �������� ��.�������
       double ZVmin= 7.0;//��.���   
       double ZVmax=16.0;//��.���          
       double ZV0,dZV,dUG1,dUG2;          
              dZV=(ZVmax-ZVmin)/2.0;// ���������� �������� ���������
              ZV0=ZVmin+dZV;        // ������� �������� ��� �������� ���������
    // �������� ���.�������
       double UG1min=   0.0*gr;// [���] // [���.����] ������ �����������
       double UG1max= 360.0*gr;// [���] // [���.����] ������ �����������
    // ---------------------
       double UG2min= -90.0*gr;// [���] // [���.����] ���������
       double UG2max=  90.0*gr;// [���] // [���.����] ���������                       
    // ---------------------       
       double dUG;
       
    // �������� ��������� ��    
       double CkoZVpor= 0.5;//��.���    
    // �������� �������� �� ��.�������� (�����,���������� ��������)       
       double CkoZVkat= 0.1;//��.���
    // ��������� �������� ��������� ��.�������� ���.������ � ������� ���
       double CkoZVvar= 0.5;//��.���
    
    // �������� ��������� �� �����
       double CkoUGpor;
       CkoUGpor=1.0*sr;         // [���] - (������ � ������� ��������)    
    // �������� �������� �� ����� (min,max)       
       double CkoUG1kat=0.06*sr;  // [���] - (������ � ������� ��������)
       double CkoUG2kat=0.06*sr;  // [���] - (������ � ������� ��������)       
    // ��������� �������� ��������� ���� ���.������ � ������� ���
       double CkoUG1var=100.0*sr;// [���] - (������ � ������� ��������)       
       double CkoUG2var=100.0*sr;// [���] - (������ � ������� ��������)       
    // ���������� <min> � <max> ��� �������� ������ (�������� ��������)       
       double minZV= 999.0;
       double maxZV=-999.0;       
    // --------------------   
       double minZVend= 999.0;
       double maxZVend=-999.0;       
       
    // printf("\n");       
    // printf("           ������(��.��� )= %12.8f\n",CkoZVpor);                                           
    // printf("           ������(���.���)= %12.8f\n",CkoUGpor);                                           
    // ������� ����� ���������� �������� �������
       printf("\n");
       while (true){    
          nScr=-1;      
          printf(" ������� ����������� ������� ���������� ��������� �� �����:\n");
          printf(" ------------------------------------------------\n");                
          printf("  * 0 - ��� �����������.\n");
          printf("  * 1 - � ������������..\n");
          printf("  * 2 - ������ ���������\n");          
          printf(" ------------------------------------------------\n");                                
          printf("  ? ���� (����� ������ => "); 
          gets(sScr);       
          sscanf(sScr,"%d",&nScr);              
       // ====================================================================
       // �������� ������� ����� ���������� "nReg"
          my_ERR=false;
          for (iz=0; iz<=2; iz++) if (nScr==iz){ my_ERR=true;break; };          
          if (!my_ERR){
             PrMsg("������ ����� ������, ������������ ������ �������");                   
             continue;
          };//if
          break;
       };//while       
       
    // ������ ������� ������������� XI2
    // ===============================================
       FILE *f_Data;
       f_Data=fopen("dan/mDataXI2.txt","r");          
       printf(" ------------------------------------------------------------------------\n");              
       if (!f_Data){
          printf(" ��������! ����������� ���� �� (������� ������������� XI^2) <dan/mDataXI2.txt>\n");
          printf(" ------------------------------------------------------------------------\n");          
          return 1;
       };//if
       printf(" OK! ====> ��������� ���� �� (������� ������������� XI^2) => <dan/mDataXI2.txt>\n");
       printf(" ......... ������ ����� � ������������ ��������� ������� ������\n");        
    // ��������� ��� ���������
    // printf(" => "); gets(sss);        
       iz=0;
       while (fscanf(f_Data,"%d %f %f %f %f\n",&pkw0,&pkw1,&pkw2,&pkw3,&pkw4)!=EOF){
          mDataXI2[iz][0]=pkw1;  
          mDataXI2[iz][1]=pkw2;               
          mDataXI2[iz][2]=pkw3;  
          mDataXI2[iz][3]=pkw4;                    
          if (nScr==1){
             printf(" iz=%d; k1=%f; k2=%f; k3=%f; k4=%f;\n",iz,
                      mDataXI2[iz][0],mDataXI2[iz][1],mDataXI2[iz][2],mDataXI2[iz][3]);
          };//if
          iz=iz+1;       
       };//while
       iz=iz-1;
       fclose(f_Data); 
    // �������� ������ ������
       if (iz==pkw0){
          printf(" OK! ====> ��������� ������ ����� � ������������ ������\n");
          printf(" ------------------------------------------------------------------------\n");         
          printf(" ��������� ������ ������������ ������� ������������� <XI2>\n");                    
       } else {
          printf(" ��������! ������ ������ ����� � ������������ ��������� ������� ������\n");       
          printf(" ------------------------------------------------------------------------\n");                
          return 1;
       };//if (iz==pkw0)       

    // ������������ ������� ������� (����������) �����
    // ===============================================
    // �������� ��������� ���������� ���.�����    
    // ImSTAR(Nmax,ZVmin,ZVmax, UG1min,UG1max, UG2min, UG2max,                       
    //        CkoZVkat,CkoZVvar, CkoUG1kat,CkoUG1var, CkoUG2kat,CkoUG2var,
    //        1);                                  
    // ===============================================    
    
    // ������ ����� � ��������� �������� ���������
       f_Data=fopen("DataStar.txt","r");          
       printf(" ------------------------------------------------------------------------\n");              
       if (!f_Data){
          printf(" ��������! ����������� ���� �� (���� � ��������� ���������) <DataStar.txt>\n");
          printf(" ------------------------------------------------------------------------\n");          
          return 1;
       };//if
       printf(" OK! ====> ��������� ���� �� (���� � ��������� ���������) <DataStar.txt>\n");
       printf(" ......... ������ ����� � ������������ ��������� ������� ������\n");        
    // ��������� ��� ���������       
    // printf(" => "); gets(sss);               
    // ���������� <min> � <max> ��� �������� ������ (�������� ��������)
       minZV   = 999.0; maxZV   =-999.0;              
       iz=0;
       while (fscanf(f_Data,"%d %f %f %f %f %f %f\n",&pkw0,&pkw1,&pkw2,&pkw3,&pkw4,&pkw5,&pkw6)!=EOF){       
          mDataStar[iz].mZV[0][0]=pkw1;
          mDataStar[iz].mZV[0][1]=pkw2;          
          mDataStar[iz].mZV[0][2]=pkw3;          
       // -----------------------------
          mDataStar[iz].mZV[1][0]=pkw4;
          mDataStar[iz].mZV[1][1]=pkw5;          
          mDataStar[iz].mZV[1][2]=pkw6;                              
       // -----------------------------          
       // ����� <min> � <max> ��� ������
          if (minZV>pkw4) minZV=pkw4;
          if (maxZV<pkw4) maxZV=pkw4;
       // -----------------------------       
          if (nScr==1){          
             printf("        j=%03d; ���: ��.= %+010.6f;  Ug1= %+011.6f;  Ug2= %+011.6f;\n",iz, mDataStar[iz].mZV[0][0], mDataStar[iz].mZV[0][1]/gr, mDataStar[iz].mZV[0][2]/gr);
              printf("               ���: ��.= %+010.6f;  Ug1= %+011.6f;  Ug2= %+011.6f;\n",    mDataStar[iz].mZV[1][0], mDataStar[iz].mZV[1][1]/gr, mDataStar[iz].mZV[1][2]/gr);          
             printf("             ------------------------------------------------------------\n");                 
         };//if          
         iz=iz+1;                
       // ����������� �� ���������� �����-������ (Nstr<=501)     
         if (iz==500) break;
       };//while
       fclose(f_Data);        
    // ������� ����� �� ��������� �� ������ ��������� ����������
       Nkat=iz;
       Nend=iz;
       printf(" >>>>>>>>> Nend=%d - �� ������ ����������!\n",Nend);                  

    // ������� ����� ���������� �������� �������
    // ====================================================================    
       printf("\n");
       while (true){    
          nReg=-1;      
          printf(" ������������� ������ ���������� �������� ������:\n");
          printf(" ------------------------------------------------\n");                
          printf("  * 0 - ������ �������� ��.��������.....\n");
          printf("  * 1 - ������ �������� ������� ��������\n");
          printf(" ------------------------------------------------\n");                                
          printf("  ? ���� (����� ������ => "); 
          gets(sReg);       
          sscanf(sReg,"%d",&nReg);              
       // �������� ������� ����� ���������� "nReg"
          my_ERR=false;
          for (iz=0; iz<=1; iz++) if (nReg==iz){ my_ERR=true;break; };          
          if (!my_ERR){
          // PrMsg("������ ����� ������, ������������ ������ �������");                   
             printf(" ------------------------------------------------\n");                          
             printf(" ��������! ������ ����� ������, ������������ ������ �������\n");                   
             printf(" ------------------------------------------------\n");                             
             continue;
          };//if
          break;
       };//while       

    // ��������� �������� ������� ������ �� ������������ �������
    // ====================================================================                            
       js=0;   // ������� ������ (��������) ���������� ���������� ���������
mmAnom:  
       js=js+1;// ������� ������ (��������) ���������� ���������� ���������       
       printf(" ------------------------------------------------------------------------\n");      
       printf(" ��������! ��������� �������� ������� ������ �� ������������ �������\n");
       printf(" >>>>>>>>> [i++] = %d\n",js);       
    // printf(" ------------------------------------------------------------------------\n");                               
    // ��������� ���������� - ��� �������� ����
       for (jz=0; jz<3; jz++) mMatStar[jz]=0.0;        

    // ��������� ����������    
       minZVend= 999.0; maxZVend=-999.0;                         
    // ������� �������� ����       
       for (jz=0; jz<Nend; jz++){ 
       // -------------------------------                 
          dZV = mDataStar[jz].mZV[0][0] - mDataStar[jz].mZV[1][0];       
          mMatStar[0] = mMatStar[0]+dZV;
       // -------------------------------     
       // !!! �������������, ������ � �.�.- [*cos(?)]            
          dZV = mDataStar[jz].mZV[0][1] - mDataStar[jz].mZV[1][1];
          mMatStar[1] = mMatStar[1]+dZV*cos(mDataStar[jz].mZV[1][2]);
       // -------------------------------                    
          dZV = mDataStar[jz].mZV[0][2] - mDataStar[jz].mZV[1][2];
          mMatStar[2] = mMatStar[2]+dZV;                    
       // -------------------------------          
       // ����� <min> � <max> ��� ������
          if (minZVend>mDataStar[jz].mZV[1][0]) minZVend=mDataStar[jz].mZV[1][0];
          if (maxZVend<mDataStar[jz].mZV[1][0]) maxZVend=mDataStar[jz].mZV[1][0];          
       };//jz       
    // ���������� ���������� �������
       mMatStar[0]= mMatStar[0]/Nend;
       mMatStar[1]= mMatStar[1]/Nend;
       mMatStar[2]= mMatStar[2]/Nend;
if (!(nScr==2)){                                                  
       printf(" ------------------------------------------------------------------------\n");      
       printf(" ��������! ��������� ������ ������������ �������� ����\n");
       printf("           M[]:m=%+08.4f[�/�]; ug1=%+09.4f[���.���]; Ug2=%+09.4f[���.���];\n",mMatStar[0],mMatStar[1]/sr,mMatStar[2]/sr);                        
    // printf(" ------------------------------------------------------------------------\n");               
};//if              
    // ��������� ���������� - ��� ������������ ����    
       for (jz=0; jz<3; jz++) mDispStar[jz]=0.0;       
    // ������� ������������ ����
       for (jz=0; jz<Nend; jz++){        
          dZV = mDataStar[jz].mZV[0][0] - mDataStar[jz].mZV[1][0] - mMatStar[0];
          mDispStar[0] = mDispStar[0]+dZV*dZV;
       // -------------------------------                    
///!!! �������������, ������ � �.�.- [*cos(...)] =>???                 
          dZV = (mDataStar[jz].mZV[0][1] - mDataStar[jz].mZV[1][1])*cos(mDataStar[jz].mZV[1][2]) - mMatStar[1];
//        dZV = (mDataStar[jz].mZV[0][1] - mDataStar[jz].mZV[1][1])*1.0 - mMatStar[1];
          mDispStar[1] = mDispStar[1]+dZV*dZV;       
       // -------------------------------                    
          dZV = mDataStar[jz].mZV[0][2] - mDataStar[jz].mZV[1][2] - mMatStar[2];
          mDispStar[2] = mDispStar[2]+dZV*dZV;          
       };//jz                      
    // ���������� ���������
       mDispStar[0] = mDispStar[0]/(Nend-1); 
       mDispStar[1] = mDispStar[1]/(Nend-1); 
       mDispStar[2] = mDispStar[2]/(Nend-1);         
if (!(nScr==2)){                                           
    // printf(" ------------------------------------------------------------------------\n");      
       printf(" ��������! ��������� ������ ������������ ������������ ����\n");
       printf("           CKO:m=%+08.4f[�/�]; ug1=%+09.4f[���.���]; Ug2=%+09.4f[���.���];\n",sqrt(mDispStar[0]),sqrt(mDispStar[1])/sr,sqrt(mDispStar[2])/sr);              
       printf("           DIS:m=%+08.4f;      ug1=%+09.4f;          Ug2=%+09.4f          \n",mDispStar[0],mDispStar[1]/sr/sr,mDispStar[2]/sr/sr);                               
       printf(" ------------------------------------------------------------------------\n");               
};//if       
    // ������ �������� ��������� ��� (��.��� � ������� ���������)       
    // ------------------------------------------------------    
       mCkoStar[0] = mDispStar[0] - CkoZVkat*CkoZVkat;
//     dZV = cos(mDataStar[jz].mZV[0][2])*cos(mDataStar[jz].mZV[0][2])
       mCkoStar[1] = mDispStar[1] - CkoUG1kat*CkoUG1kat;//*dZV;            
       mCkoStar[2] = mDispStar[2] - CkoUG2kat*CkoUG2kat;                    
    // �������������� <0>    
       if (mCkoStar[0]<0) {mCkoStar[0]=mDispStar[0];mPrZV =true;};
       if (mCkoStar[1]<0) {mCkoStar[1]=mDispStar[1];mPrUG1=true;};
       if (mCkoStar[2]<0) {mCkoStar[2]=mDispStar[2];mPrUG2=true;};     
if (!(nScr==2)){                                    
    // printf(" ------------------------------------------------------------------------\n");      
       printf(" OK! ====> ��������� ������ ������ �������� ��������� ���������\n");
       printf("           CKO:m=%+08.4f[�/�]; ug1=%+09.4f[���.���]; Ug2=%+09.4f[���.���];\n",sqrt(mCkoStar[0]),sqrt(mCkoStar[1])/sr,sqrt(mCkoStar[2])/sr);                        
       printf("           DIS:m=%+08.4f;      ug1=%+09.4f;          Ug2=%+09.4f          \n",mCkoStar[0],mCkoStar[1]/sr/sr,mCkoStar[2]/sr/sr);                               
    // printf(" ------------------------------------------------------------------------\n");                   
};//if    
    // �������� ����������
       switch (nReg){
             case 0:                    
                if (mPrZV) goto mmEND;        
                break;
             case 1:                    
                if (mPrUG1&&mPrUG2) goto mmEND;
                break;                
       };//switch       
    // ��������� ��� ���������
       if (!(nScr==2)){
          printf(" => "); gets(sss);
       };//if
    // �������� ������ - ���������� ��������� ��.��������
       printf(" ------------------------------------------------------------------------\n");      
       printf(" ����������� ������ ������ ���������� ���������...\n");
       printf(" ------------------------------------------------------------------------\n");             
       kz=0;//����� ���������� ��������� 
       iz=0;//����� ���������� ��������� (������ ����������������� �������)       
       for (jz=0; jz<Nend; jz++){               
          switch (nReg){
             case 0:                    
                dZV   = mDataStar[jz].mZV[0][0] - mDataStar[jz].mZV[1][0] - mMatStar[0];
                DeltaX= (Nend-1)*mDataXI2[Nend][1]*mCkoStar[0]/Nend;                    
                prXI2 = (dZV*dZV)>=DeltaX;
                break;
             case 1:                 
                dUG1   = (mDataStar[jz].mZV[0][1] - mDataStar[jz].mZV[1][1])*cos(mDataStar[jz].mZV[1][2]) - mMatStar[1];
//              dUG1   = (mDataStar[jz].mZV[0][1] - mDataStar[jz].mZV[1][1])*1.0 - mMatStar[1];
                DeltaX1= (Nend-1)*mDataXI2[Nend][1]*mCkoStar[1]/Nend;                    
             // -----------------------------------------------------    
                dUG2   = mDataStar[jz].mZV[0][2] - mDataStar[jz].mZV[1][2] - mMatStar[2];
                DeltaX2= (Nend-1)*mDataXI2[Nend][1]*mCkoStar[2]/Nend;                    
             // -----------------------------------------------------                 
                prXI2  = ( (!mPrUG1)&&((dUG1*dUG1)>=DeltaX1) )||
                         ( (!mPrUG2)&&((dUG2*dUG2)>=DeltaX2) );                             
                break;
          };//switch          
          
          if (prXI2){          
             kz=kz+1;// ������� ���������� ���������
          // ��������� ��������� ��� ���������� ��������    
             if (nScr==1){                    
                switch (nReg){
                   case 0:                    
                      printf("           jz= %03d; k2=%f; dZV^2= %08.4f; DeltaX= %08.4f;",jz+1,mDataXI2[Nend][1],dZV*dZV,DeltaX);                
                      break;
                   case 1:    
                      printf("           jz= %03d; dUG1^2= %08.4f; DeltaX1= %08.4f\n",jz+1,dUG1*dUG1/sr/sr,DeltaX1/sr/sr);
                       printf("                    dUG2^2= %08.4f; DeltaX2= %08.4f",       dUG2*dUG2/sr/sr,DeltaX2/sr/sr);
                      break;                   
                };//switch                          
                printf(" >>> [��.���]\n");
             };//if                       
          // printf(" => "); gets(sss);                               
          }else{                    
             iz=iz+1;
             mBufStar[iz-1].mZV[0][0]=mDataStar[jz].mZV[0][0];
             mBufStar[iz-1].mZV[0][1]=mDataStar[jz].mZV[0][1];             
             mBufStar[iz-1].mZV[0][2]=mDataStar[jz].mZV[0][2];             
          // -------------------------------------------------
             mBufStar[iz-1].mZV[1][0]=mDataStar[jz].mZV[1][0];
             mBufStar[iz-1].mZV[1][1]=mDataStar[jz].mZV[1][1];             
             mBufStar[iz-1].mZV[1][2]=mDataStar[jz].mZV[1][2];                                       
          };//if
       };//jz               
       
    // ��������� � ������ ���������� ���������
       if (nScr==1) printf("           --------------------------------------------------------------\n");                          
       printf("           Nkat= %d; Nend= %d; Nanom= %d; Nanom(tek)= %d\n",Nkat,iz,Nkat-iz,kz);                           
       Nend=iz;//Nend-kz;   
    // ��������� ��� ���������
       if (!(nScr==2)){
          printf(" => "); gets(sss);                  
       };//if      
    // �������� �� ������������ �������� �������              
       if (Nend<20){
          printf(" ------------------------------------------------------------------------\n");                                           
          printf(" ��������! ����������� �������� ������� < 20...\n");
          printf(" ------------------------------------------------------------------------\n");          
          return 2;       
       };//if
                                
       if (kz>0){
          printf(" ��������! ���������� [%d] ���������� ��������, ����������� ������-�����\n",kz);          
          printf("           Nend(iz)= %d\n",Nend);                    
          printf(" ��������! ����������� ��������� �������� ������� ������\n");                              
          printf("           �����...\n");                                        
       // ����� �������          
          for (jz=0; jz<Nend; jz++){           
             mDataStar[jz].mZV[0][0]=mBufStar[jz].mZV[0][0];
             mDataStar[jz].mZV[0][1]=mBufStar[jz].mZV[0][1];             
             mDataStar[jz].mZV[0][2]=mBufStar[jz].mZV[0][2];             
          // -----------------------------------------------
             mDataStar[jz].mZV[1][0]=mBufStar[jz].mZV[1][0];
             mDataStar[jz].mZV[1][1]=mBufStar[jz].mZV[1][1];             
             mDataStar[jz].mZV[1][2]=mBufStar[jz].mZV[1][2];
          // -----------------------------------------------          
            if (nScr==1){                    
               printf(" �����:jz=%03d;���: �/�=%+010.6f; Ug1=%+015.2f; Ug2=%+015.2f;\n",jz+1, mDataStar[jz].mZV[0][0], mDataStar[jz].mZV[0][1]/sr, mDataStar[jz].mZV[0][2]/sr);
                printf("              ���: �/�=%+010.6f; Ug1=%+015.2f; Ug2=%+015.2f;\n",      mDataStar[jz].mZV[1][0], mDataStar[jz].mZV[1][1]/sr, mDataStar[jz].mZV[1][2]/sr);
                printf("              -----------------------------------------------------------\n");                              
            };//if
             
          };//jz           
          printf(" OK! ====> ��������� ������ �������� ������ (���.��.������)\n");          
          printf(" ------------------------------------------------------------------------\n");                      
       // ��������� ��� ���������
          if (!(nScr==2)){       
             printf(" => "); gets(sss);                  
          };//if
          goto mmAnom;
       };//else{
mmEND:
          printf(" ------------------------------------------------------------------------\n");                                    
          printf(" ��������! ��������� ���������� ���������� �������� ���������\n");
          printf("           �� �����  ����������, ������� ����������� ��.������ Nend=%d\n",Nend);                    
          printf("           --------------------------------------------------------------\n");                                              
if (!(nScr==2)){                             
          switch (nReg){
                case 0:                    
                   printf(" OK! ====> ������� ���.������ ������ �������� ���.��.��������\n");          
                   printf("           CKO��:�/�=%10.6f [��.���]\n",sqrt(mCkoStar[0]));
                   printf("           DIS��:�/�=%10.6f\n",mCkoStar[0]);
                   break;                                   
                case 1:    
                   printf(" OK! ====> ������� ���.������ ������ �������� ���.���.���������\n");          
                   printf("           CKO��:ug1=%10.4f [���.���]; ug2= %10.4f[���.���];\n",sqrt(mCkoStar[1])/sr,sqrt(mCkoStar[2])/sr);                        
                   printf("           DIS��:ug1=%10.4f;           ug2= %10.4f;\n",mCkoStar[1]/sr/sr,mCkoStar[2]/sr/sr);                                                                         
                   break;                   
          };//switch                    
};//if          
          printf("           ����� ������� Nkat=%d, �� ��� ����������� N= %d\n",Nkat,Nend);                                  
          printf(" ------------------------------------------------------------------------\n");                                       
          
          printf(" ����� -> <����> "); gets(sss);                            
          printf(" ------------------------------------------------------------------------\n");                                       
       // �������� ������� ��������������� ������������ ������ (� P���>=0.9)
if (!(nScr==2)){              
          switch (nReg){
                case 0:                
                   DeltaX= mDataXI2[Nend][0]*sqrt(mCkoStar[0]/(Nend-1));
                   if (fabs(mMatStar[0])<DeltaX){
                      printf(" OK! ====> ��������������� ������������ � ���.��.���. �����������\n");                      
                   }else{
                      printf(" ��������! ������� ��������������� ������������ � ���.��.���.\n");                                         
                   };
                      printf("           |���.����.��.���.|= %12.6f [��.���]\n",DeltaX);                                           
                   break;                                   
                case 1:    
                   DeltaX1= mDataXI2[Nend][0]*sqrt(mCkoStar[1]/(Nend-1));
                   DeltaX2= mDataXI2[Nend][0]*sqrt(mCkoStar[2]/(Nend-1));                   
                   if ((fabs(mMatStar[1])<DeltaX1)&&
                       (fabs(mMatStar[2])<DeltaX2)
                      ){
                      printf(" OK! ====> ��������������� ������������ � ���.���. �����������\n");                      
                   }else{
                      printf(" ��������! ������� ��������������� ������������ � ���.���.\n");                                         
                   };
                      printf("           ����� ������� Nkat=%d, �� ��� ����������� N= %d\n",Nkat,Nend);                                                            
                      printf("           |���.����.UG1|= %12.6f [���.���]\n",DeltaX1/sr);                                           
                      printf("           |���.����.UG2|= %12.6f [���.���]\n",DeltaX2/sr);                                                                                 
                   break;                   
          };//switch
          printf(" ------------------------------------------------------------------------\n");                             
          printf(" ����� -> <����> "); gets(sss);                                      
};//if
       // ������� � ������ ��.���.���. ��� ��������� ���������(� P���=0.95)       
if (!(nScr==2)) printf(" ------------------------------------------------------------------------\n");                 
          switch (nReg){
                case 0:
                   dZV=CkoZVkat*CkoZVkat;
                   DeltaX1= mCkoStar[0]/mDataXI2[Nend][3]-dZV;//������  �������
                   DeltaX2= mCkoStar[0]/mDataXI2[Nend][2]-dZV;//������� �������                   
                   if (DeltaX1<0) DeltaX1=mCkoStar[0]/mDataXI2[Nend][3];
                   if (DeltaX2<0) DeltaX2=mCkoStar[0]/mDataXI2[Nend][2];
                // -------------------------------------------                      
if (!(nScr==2)){                
                   printf(" OK! ====> ������� �������������� ��������� ��� ������ ��.��������\n");                      
                   printf("           |������. ��.���.��.���.|= %12.8f [�/�];\n",sqrt(DeltaX1));                                           
                   printf("           |�������.��.���.��.���.|= %12.8f [�/�];\n",sqrt(DeltaX2));
                   printf(" ------------------------------------------------------------------------\n");                                                                             
};//if                   
                   printf("\n");
                // �������� ������� � ������������ ������� ������������� �����������                      
                   printf(" ========================================================================\n");                                                 
/*                   
                   if (sqrt(DeltaX2)<=CkoZVpor){
                      printf(" ��������: [���.��.���.������] ������������� ���������� ��� \n");                      
                   }else{
                      printf(" ��������! ��������: [���.��.���.������] �� ������������� ���������� ��� \n");                   
                   };//if
                   printf("           ������(��.���)= %11.6f [���.���];\n",CkoZVpor);                                           
                   printf("           ����� ������� Nkat=%d, �� ��� ����������� N= %d\n",Nkat,Nend);                                                        
*/
                   DeltaX=sqrt(DeltaX2);
                   printf(" ��������! ����� ���������� ������� Nkat=%d, �� ��� ����������� N= %d\n",Nkat,Nend);                   
                   printf("           � �������� ������ �������� ��.���-��� ������ �������:\n");                       
                   printf("           ���=== %012.8f [��.���];\n",DeltaX);                                          
                   printf("           ----------------------------\n");
                   printf("           �������� �������: minZV= %07.4f [�/�]; maxZV= %07.4f [�/�];\n",minZV,maxZV);
                   printf("           �������� �������: minZV= %07.4f [�/�]; maxZV= %07.4f [�/�];\n",minZVend,maxZVend);                                      
                   printf("           --------------------------------------------------------------\n");                                                
                   printf("           �����, ���������� �������� ��������� � ��������� ���������!\n");                            
//                 printf(" ========================================================================\n");                                                                    
                   break;                                   
                case 1:    
                   dZV=CkoUG1kat*CkoUG1kat;
                   DeltaX1= mCkoStar[1]/mDataXI2[Nend][3]-dZV;//������  ������� UG1
                   DeltaX2= mCkoStar[1]/mDataXI2[Nend][2]-dZV;//������� ������� UG1
                   if (DeltaX1<0) DeltaX1=mCkoStar[1]/mDataXI2[Nend][3];
                   if (DeltaX2<0) DeltaX2=mCkoStar[1]/mDataXI2[Nend][2];
if (!(nScr==2)){                                   
                   printf(" OK! ====> ������� �������������� ��������� ��� ������ UG1\n");                      
                   printf("           |������. ��.���.���.���|= %12.4f [���.���];\n",sqrt(DeltaX1)/sr);                                           
                   printf("           |�������.��.���.���.���|= %12.4f [���.���];\n",sqrt(DeltaX2)/sr);                   
                // -------------------------------------------   
                   printf("           --------------------------------------------------------------\n");                                                       
};//if                   
                   dZV=CkoUG2kat*CkoUG2kat;
                   DeltaX3= mCkoStar[2]/mDataXI2[Nend][3]-dZV;//������  ������� UG2
                   DeltaX4= mCkoStar[2]/mDataXI2[Nend][2]-dZV;//������� ������� UG2
                   if (DeltaX3<0) DeltaX3=mCkoStar[2]/mDataXI2[Nend][3];
                   if (DeltaX4<0) DeltaX4=mCkoStar[2]/mDataXI2[Nend][2];                   
if (!(nScr==2)){                   
                   printf(" OK! ====> ������� �������������� ��������� ��� ������ UG2\n");
                   printf("           |������. ��.���.���.���|= %12.4f [���.���];\n",sqrt(DeltaX3)/sr);                                           
                   printf("           |�������.��.���.���.���|= %12.4f [���.���];\n",sqrt(DeltaX4)/sr);                                      
                   printf(" ------------------------------------------------------------------------\n");                             
};//                   
                   printf("\n");
                // -------------------------------------------                      
                // �������� ������� � ������������ ������� ������������� �����������                      
                   printf(" ========================================================================\n");                                                                 
/*                   
                   if ((sqrt(DeltaX2)<=CkoUGpor)&&
                       (sqrt(DeltaX4)<=CkoUGpor)){
                      printf(" ��������: [���.��.���.����] ������������� ���������� ��� \n");
                   }else{
                      printf(" ��������! ��������: [���.��.���.����] �� ������������� ���������� ��� \n");                   
                   };//if                
*/                   
//                 printf("           ������(���.���)= %12.4f [���.���];\n",CkoUGpor/sr);                                           

                   DeltaX=sqrt(DeltaX2);
                   if (DeltaX<sqrt(DeltaX4)) DeltaX=sqrt(DeltaX4);
                   printf(" ��������! ����� ���������� ������� Nkat=%d, �� ��� ����������� N= %d\n",Nkat,Nend);                   
                   printf("           � �������� ������ �������� ��.��������� ���� �������:\n");                       
                   printf("           ���=== %012.8f [���.���];\n",DeltaX/sr);                                          
                   printf("           -----------------------------\n");                                                                   
                   printf("           �������� �������: minZV= %07.4f [�/�]; maxZV= %07.4f [�/�];\n",minZV,maxZV);
                   printf("           �������� �������: minZV= %07.4f [�/�]; maxZV= %07.4f [�/�];\n",minZVend,maxZVend);                   
                   printf("           --------------------------------------------------------------\n");                                                
                   printf("           �����, ���������� �������� ��������� � ��������� ���������!\n");      
//                 printf(" ========================================================================\n");                                                 
                   break;                   
          };//switch
//        printf(" ------------------------------------------------------------------------\n");                                       
          printf(" ========================================================================\n");                                                 

       // ��������� ��� ���������
          printf(" ����� -> <����> "); gets(sss);                                   
          printf(" ========================================================================\n");                                                           
       // ������ � ����                   
          printf(" ������� ���� ������ ? y-<���������� ���������>,n-<����� ������� �������>\n");
          printf(" => "); gets(sss);                  
          printf(" ========================================================================\n");                                                                     
          printf(" ��������! ������������ �������� ��������� �������� ");
          if ((sss[0]=='y')||(sss[0]=='Y')){
            f_Data=fopen("DataStar.txt","w");
            printf("<DataStar.txt>\n");                              
          }else{
            if (nReg==0){ 
               f_Data=fopen("ZvStar.txt","w");           
               printf("<ZvStar.txt>\n");                                             
            };//if   
            if (nReg==1){
               f_Data=fopen("UgStar.txt","w");                       
               printf("<UgStar.txt>\n");                                                            
            };//if   
          };//if
          printf("           �����...\n");                            
          for (jz=0; jz<Nend; jz++){
          // -------------------------------------
             fprintf(f_Data,"%d",jz);   
             fprintf(f_Data,"%c",' ');   
          // ===================================                             
             fprintf(f_Data,"%12.8f",mDataStar[jz].mZV[0][0]);   
             fprintf(f_Data,"%c",' ');   
          // -----------------------------------                       
             fprintf(f_Data,"%12.8f",mDataStar[jz].mZV[0][1]);   
             fprintf(f_Data,"%c",' ');   
          // -----------------------------------                       
             fprintf(f_Data,"%12.8f",mDataStar[jz].mZV[0][2]);   
             fprintf(f_Data,"%c",' ');   
          // ===================================                    
             fprintf(f_Data,"%12.8f",mDataStar[jz].mZV[1][0]);   
             fprintf(f_Data,"%c",' ');   
          // -----------------------------------                       
             fprintf(f_Data,"%12.8f",mDataStar[jz].mZV[1][1]);   
             fprintf(f_Data,"%c",' ');   
          // -----------------------------------                       
             fprintf(f_Data,"%12.8f",mDataStar[jz].mZV[1][2]);   
             fprintf(f_Data,"%c",'\n');         
          // -------------------------------------         
          };//jz                   
          fclose(f_Data);            
          printf(" OK! ====> ��������� ������ ������������ �������� ��������� ��������\n");
          printf(" ========================================================================\n");                                                                     
//        printf(" ------------------------------------------------------------------------\n");          
//       };//if (kz>0)
       
    // ��������� ��� ���������
    // printf(" => "); gets(sss);        
              
       return 0;
   }//void ModSTAR() - �������� ��������� ���������� ���.�����
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\   

