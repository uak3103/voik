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
// Массив признаков (CAN для КВ)
   struct stLRcan mmCanKW[10][3]; // включен/отключен
   struct prLRcan prCanKW[10][3]; // приходили сообщения или нет !?     
//----------------------------------------------------------------------------------

#define nnSwEnd 10      // размерность массива структур по числу концевиков
#define nSwEnd   2      // реальная размерность КВ
#define EpsKW    0.2    // разрешение по положению КВ

//#define EpsOS  0.01;  // разрешение по положению КВ - [угл.град]
#define kw_A0    0.0    // привязка начала отсчета для азимутальной оси - [угл.град]
#define kw_L0   90.0    // привязка начала отсчета для полярной     оси - [угл.град]
#define kw_R0   90.0    // привязка начала отсчета для орбитальной  оси - [угл.град]

#define AminKW  -220.0  // ограничение по оси <A> слева  [угл.град]
#define AmaxKW   220.0  // ограничение по оси <A> справа [угл.град]

#define LminKW     1.0  // ограничение по оси <L> слева  [угл.град]
#define LmaxKW   170.0  // ограничение по оси <L> справа [угл.град]

#define RminKW    1.0   // ограничение по оси <R> слева  [угл.град]
#define RmaxKW   170.0  // ограничение по оси <R> справа [угл.град]
//----------------------------------------------------------------------------------


// Послать запрос КДПО о состоянии КВ для всех осей 
// (инициализация начальных условий для матрицы mmCanKW)       
   void SendKdpoKW(double  dtt){
//    if (!IMIT){
             printf(" Запрос..? Формируется сообщение с запросом о состоянии КВ (КДПО)\n");
             printf("           Ждите...\n");                                          
          // printf(" ------------------------------------------------------------------------\n");             

          // Азимутальная ось
          // ======================================================                 
             printf(" ========================================================================\n");                                    
          // Состояние КВ 1-го уровня (правый)
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR1AA);                  
             usleep(dtt*1000000);                                            
             printf(" OK! ====> азимутальная ось - правый 1-го уровня\n");
          // -------------------      
          // Состояние КВ 1-го уровня (левый)       
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR1BA);               
             usleep(dtt*1000000);                                            
             printf(" OK! ====> азимутальная ось - левый  1-го уровня\n");
             
          // printf(" -----------------------------------------------\n");                       
          // ======================================================          
          // Состояние КВ 2-го уровня (правый)
          // can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR2AA);     
          // usleep(dtt*1000000);                                            
          // printf(" OK! ====> азимутальная ось - правый 2-го уровня\n");      
          // -------------------      
          // Состояние КВ 2-го уровня (левый)       
          // can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR2BA);               
          // usleep(dtt*1000000);                                        
          // printf(" OK! ====> азимутальная ось - левый  2-го уровня\n");    
                       
             printf(" -----------------------------------------------\n");             
          // ======================================================          
          // Состояние КВ 3-го уровня (правый)
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR3AA);     
             usleep(dtt*1000000);                                            
             printf(" OK! ====> азимутальная ось - правый 3-го уровня\n");                   
          // -------------------      
          // Состояние КВ 3-го уровня (левый)       
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR3BA);               
             usleep(dtt*1000000);                                            
             printf(" OK! ====> азимутальная ось - левый  3-го уровня\n");                 
          // ======================================================                              
          
          // Полярная ось
          // ======================================================                 
             printf(" ========================================================================\n");                                    
          // Состояние КВ 1-го уровня (правый)
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR1AL);     
             usleep(dtt*1000000);                                            
             printf(" OK! ====> полярная     ось - правый 1-го уровня\n");                                
          // -------------------      
          // Состояние КВ 1-го уровня (левый)       
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR1BL);               
             usleep(dtt*1000000);                                            
             printf(" OK! ====> полярная     ось - левый  1-го уровня\n");                 
             
          // printf(" -----------------------------------------------\n");                       
          // ======================================================          
          // Состояние КВ 2-го уровня (правый)
          // can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR2AL);     
          // usleep(dtt*1000000);                                            
          // printf(" OK! ====> полярная     ось - правый 2-го уровня\n");                                          
          // -------------------      
          // Состояние КВ 2-го уровня (левый)        
          // can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR2BL);               
          // usleep(dtt*1000000);                                            
          // printf(" OK! ====> полярная     ось - левый  2-го уровня\n");                           
          
             printf(" -----------------------------------------------\n");                                    
          // ======================================================          
          // Состояние КВ 3-го уровня (правый)
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR3AL);     
             usleep(dtt*1000000);                                            
             printf(" OK! ====> полярная     ось - правый 3-го уровня\n");                                             
          // -------------------      
          // Состояние КВ 3-го уровня (левый)      
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR3AL);               
             usleep(dtt*1000000);                                            
             printf(" OK! ====> полярная     ось - левый  3-го уровня\n");                              
          // ======================================================                              
          
          // Орбитальная ось
             printf(" ========================================================================\n");                                    
          // ======================================================                 
          // Состояние КВ 1-го уровня (правый)
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR1AR);     
             usleep(dtt*1000000);                                            
             printf(" OK! ====> орбитальная  ось - правый 1-го уровня\n");                                                          
          // -------------------      
          // Состояние КВ 1-го уровня (левый)       
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR1BR);               
             usleep(dtt*1000000);                                            
             printf(" OK! ====> орбитальная  ось - левый  1-го уровня\n");                                                                                    
             
          // printf(" -----------------------------------------------\n");                                              
          // ======================================================          
          // Состояние КВ 2-го уровня (правый)
          // can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR2AR);     
          // usleep(dtt*1000000);                                            
          // printf(" OK! ====> орбитальная  ось - правый 2-го уровня\n");                                                                    
          // -------------------      
          // Состояние КВ 2-го уровня (левый)        
          // can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR2BR);               
          // usleep(dtt*1000000);                                            
          // printf(" OK! ====> орбитальная  ось - левый  2-го уровня\n");                                                                    
          
             printf(" -----------------------------------------------\n");             
          // ======================================================          
          // Состояние КВ 3-го уровня (правый)
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR3AR);     
             usleep(dtt*1000000);                                            
             printf(" OK! ====> орбитальная  ось - правый 3-го уровня\n");                                                                                 
          // -------------------      
          // Состояние КВ 3-го уровня (левый)      
             can_req2(fcan[0], CAN_ABN_KDPO, CAN_CMD_GETP, _VOGR3BR);               
             usleep(dtt*1000000);                                            
             printf(" OK! ====> орбитальная  ось - левый  3-го уровня\n");                                                                                 
          // ======================================================                              
//        };//if
          printf(" ========================================================================\n");          
       // printf(" ------------------------------------------------------------------------\n");                       
   };// Послать запрос КДПО о состоянии КВ для всех осей           

// Вывод значений рабочего диапазона для всех осей ОПУ
   void OutDelWork(int nInpKW,struct stDir mDataKW[][3],struct stLRc mConfKW[][3]){
      int ikw,jkw;
      double UgMin,UgMax,DelWork;       
      printf(" Вывод значений для рабочего диапазона осей ОПУ j\n");       
      printf(" ========================================================================\n");                                     
      for (jkw=0; jkw<3; jkw++){       
          switch (jkw){
                case 0:printf(" # азимутальная ось <A>: \n");break;
                case 1:printf(" # полярная     ось <L>: \n");break;
                case 2:printf(" # орбитальная  ось <R>: \n");break;
          };//switch                                        
          for (ikw=0; ikw<nInpKW; ikw++){

              if ((mConfKW[ikw][jkw].prL)&&(mConfKW[ikw][jkw].prR)){
                 UgMin=mDataKW[ikw][jkw].stDirL.val;
                 UgMax=mDataKW[ikw][jkw].stDirR.val;              
                 DelWork=UgMax-UgMin;
                 printf("  * для %d-уровня КВ: del=%+011.6f; min=%+011.6f; max=%+011.6f\n",ikw,DelWork,UgMin,UgMax);          
              };//if                 
              
          };//ikw
          if (jkw<2) printf(" ------------------------------------------------------------------------\n");                       
      };//jkw             
      printf(" ========================================================================\n");                              
   };//OutDelWork() - Вывод значений рабочего диапазона для всех осей ОПУ

// Формирование файла БД - <DataBF.txt>    
   void FileDataBF(int nInpKW,struct stDir mDataBuf[][3]){
      char sss[256];
      int ikw,jkw;
      FILE *f_DataBF;
      printf(" ------------------------------------------------------------------------\n");              
      printf(" Формирование файла БД - <DataBF.txt> ? (y->да;n->нет)\n");
      printf(" => "); gets(sss); 
      if ((sss[0]=='y')||(sss[0]=='Y')){
      // Открыть файл                
         f_DataBF=fopen("DataBF.txt","w");               

        for (ikw=0; ikw<nInpKW; ikw++){
           for (jkw=0; jkw<3; jkw++){              
            // Запись в файл
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
         printf(" Выполнено формирование файла БД - <DataBF.txt>\n");
      };//if (sss)
   };//FileDataBF() Формирование файла БД - <DataBF.txt>
            

// Состояние (признаки отработки) сообщений CAN для КВ
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
       // printf(" # NEXT (нажмите <Ввод>) ==> "); gets(sss);                       
       };//ikw                           
   };//StatCanKW() - Состояние (признаки отработки) сообщений CAN для КВ
   
// Форматирование и отображение массива признаков наличия сообщения от CAN
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
      if (pr) printf(" Операция: установка значений по умолчанию ('0')\n");
                                         
              printf(" Просмотр: признаки мониторинга сети CAN (состояние КВ)\n");
             
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
                
      if (pr) printf(" OK! => Выполнена установка значений по умолчанию\n"); 
      if (pr) printf(" ------------------------------------------------------------------------\n");
   };// Форматирование и отображение массива признаков наличия сообщения от CAN

// Отображение структуры-буфера данных для КВ на экран
   void ScrDataKW(int nInpKW, struct stDir mDataBuf[][3]){
        int ikw;    
        char sss[256];
//        printf(" ------------------------------------------------------------------------\n");              
//        printf(" Отображение структуры-буфера данных для КВ на экран ? (y->да;n->нет)\n");
//        printf(" => "); gets(sss); 
//        if ((sss[0]=='y')||(sss[0]=='Y')){
           for (ikw=0; ikw<nInpKW; ikw++){
             printf(" ======:========================================================:\n");  
             printf(" Nкв=%02d:",ikw+1);             
             printf("  Левый (-)   : cko         : Правый (+)  : cko         :\n");
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
   };// Отображение структуры-буфера данных для КВ на экран

// Генератор задержки
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

// Генераторы (ГСС)
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

// ИМИТАЦИЯ !!!     
// Начальная установка ОПУ по всем осям (A,L,R) - состояния ОПУ после включения
   void IniValOs(int nInpKW,bool *prCP,struct stDir mDataKW[][3],double cd[]){
        double ValOs0,ValOsMax,ValOsMin,DelOs;        
        double ugA_0,ugL_0,ugR_0;
        
        if (!*prCP)  {
           printf(" ------------------------------------------------------------------------\n");             
           printf(" Имитация! Начальные условия по положению осей ОПУ: {A,L,R} => [угл.град]\n");     
           printf("           режим КЭП - <по положению>\n");                

        // Случайное значение положения оси ОПУ по КВ 3-го уровня + 3*СКО           
        // --------------------------------------------------------------
        // Азимутальная ось
           ValOsMax = mDataKW[nInpKW-1][0].stDirR.val + 3.0*mDataKW[nInpKW-1][0].stDirR.cko;
           ValOsMin = mDataKW[nInpKW-1][0].stDirL.val - 3.0*mDataKW[nInpKW-1][0].stDirL.cko;
           DelOs    = (ValOsMax - ValOsMin)/2.0;
           ValOs0   = ValOsMin  + DelOs;
           ugA_0    = my_random1(ValOs0,DelOs/2.0);                       
           printf(" ГСС[A]==> Amax=%+007.2f; Amin=%+007.2f; A0=%+007.2f; ^A=%+007.2f;\n", 
                              ValOsMax,ValOsMin,ValOs0,ugA_0);           
        // Полярная ось
           ValOsMax = mDataKW[nInpKW-1][1].stDirR.val + 3.0*mDataKW[nInpKW-1][1].stDirR.cko;
           ValOsMin = mDataKW[nInpKW-1][1].stDirL.val - 3.0*mDataKW[nInpKW-1][1].stDirL.cko;
           DelOs    = (ValOsMax - ValOsMin)/2.0;
           ValOs0   = ValOsMin  + DelOs;
           ugL_0    = my_random1(ValOs0,DelOs/2.0);                      
           printf(" ГСС[L]==> Lmax=%+007.2f; Lmin=%+007.2f; L0=%+007.2f; ^L=%+007.2f;\n", 
                              ValOsMax,ValOsMin,ValOs0,ugL_0);           
        // Орбитальная ось
           ValOsMax = mDataKW[nInpKW-1][2].stDirR.val + 3.0*mDataKW[nInpKW-1][2].stDirR.cko;
           ValOsMin = mDataKW[nInpKW-1][2].stDirL.val - 3.0*mDataKW[nInpKW-1][2].stDirL.cko;
           DelOs    = (ValOsMax - ValOsMin)/2.0;
           ValOs0   = ValOsMin  + DelOs;
           ugR_0    = my_random1(ValOs0,DelOs/2.0);           
           printf(" ГСС[R]==> Rmax=%+007.2f; Rmin=%+007.2f; R0=%+007.2f; ^R=%+007.2f;\n", 
                              ValOsMax,ValOsMin,ValOs0,ugR_0);           
        // Собственно установка оси ОПУ           
           printf("           Ждите... \n");                 
        
           perebroscsn(ugA_0,ugL_0,ugR_0);
        // perebroscsn(-211.0,6.0,178.0);
           RESCSN();
        // printf(" ------------------------------------------------------------------------\n");     
           
           printf(" OK! ====> A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);
        // printf(" ------------------------------------------------------------------------\n");     
        // Рекурсия признака - для первого прохода     
           *prCP=true;
        };//if (prCP)
        
   }//IniValOs


// Контроль наличия (распайки) запрошенного КВ
   void PrMsg(char *msg){
      char s[256]=" Внимание! ";
      strcat(s,msg);
      printf(" ------------------------------------------------------------------------\n");             
//    printf(" Внимание! Для выбранного уровня отсутствует распайка КВ\n");
      printf("%s\n",s);
      printf("           Переход в начало текущего цикла опроса...\n");        
      printf(" ------------------------------------------------------------------------\n");
   };//void YesKw()

// Контроль наличия (распайки) запрошенного КВ
   void YesKw(){
      printf(" ------------------------------------------------------------------------\n");             
      printf(" Внимание! Для выбранного уровня отсутствует распайка КВ\n");                                      
      printf("           Для продолжения нажмите любую клавишу ...");        
      printf(" ------------------------------------------------------------------------\n");
   };//void YesKw()
                        
// Контроль формата ввода переменной "nTek"
   void NotIn(){
      printf(" ------------------------------------------------------------------------\n");     
      printf(" Внимание! Ошибка ввода данных, недопустимый формат команды\n");                                      
      printf("           Для продолжения нажмите любую клавишу ...");        
      
      printf(" ------------------------------------------------------------------------\n");          
   };//PrintErr()

// Переброска к началу разгонного участка ( + коррекция)
   void SpUchVar(char code_ALR,double EpsUch,double dt,double ug0){
     int i,j;
     char sss[1];
     double v6_ALR[6];
     double DelTek=999.0; // ну... просто очень большое значение!
     
// printf(" EpsUch=%f; dt=%f; ug0=%f; cd[1]=%f;\n",EpsUch,dt,ug0,cd[1]);     
// printf(" => "); gets(sss);   
  // Первичная блокировка управления
     for (j=0; j<6; j++) v6_ALR[j]=999.0;       
  // Пороговое и текущеее рассогласование относительно начала разгонного участка
  // Принятие решения  
  // printf("\n");
     printf(" ------------------------------------------------------------------------\n");            
     printf(" Имитация! Установка оси в начало РУ...\n");                     
  // Селектор типа оси ОПУ            
     switch (code_ALR){     
        case 'A':        
          // Переброска к началу разгонного участка (РУ)
             v6_ALR[0]=ug0;                     
             perebroscsn(v6_ALR[0],v6_ALR[1],v6_ALR[2]);
             RESCSN();
             printf("           A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);                   
             printf(" ------------------------------------------------------------------------\n");                               
             v6_ALR[0]=999.0;
          // Величина рассогласования  
             DelTek=(cd[0]-ug0); 
             
          // Условие на запуск процедуры коррекции начала РУ   
             if (fabs(DelTek)>EpsUch){
                printf(" Внимание! Рассогласование по положению относительно начала РУ\n");            
                printf("           Запустить процедуру коррекции? (y->да;n->нет)\n");             
                printf(" => "); gets(sss); 
                
                if ((sss[0]=='y')||(sss[0]=='Y')){         
                   printf("           Ждите...\n");
                // Коррекция проводится в режиме "управление по скорости"                      
                   v6_ALR[3]=-0.1*DelTek/fabs(DelTek)*EpsUch;//скорость <обратна> по отношению к рассогласованию
                   skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);                   
                   i=0;
                   while (fabs(cd[0]-ug0)>EpsUch){
                      i=i+1;                      
//                      skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);
                      usleep(dt*1000000);//[мкс]
                   // printf("         # i=%d; A=%+10.6f;\n",i,cd[0]);                                                                                 
                   };//while                   
                // printf(" ------------------------------------------------------------------------\n");                                                        
                // Остановка оси ОПУ
                   skorcsn(0.0,0.0,0.0);
                   printf(" OK! ====> Выполнена задача коррекции начала РУ\n");                      
                }else{
                   printf(" ??? ====> Задача коррекции начала РУ не выполнялась...!?\n");                                      
                }                  
             } else printf(" OK! ====> Выполнена установка оси ОПУ в начало РУ\n");
                    printf("           A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);                                      
             break;
                   
        case 'L':         
          // Переброска к началу разгонного участка (РУ)
             v6_ALR[1]=ug0;                     
             perebroscsn(v6_ALR[0],v6_ALR[1],v6_ALR[2]);
             RESCSN();
             printf("           A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);                   
             printf(" ------------------------------------------------------------------------\n");                               
             v6_ALR[1]=999.0;
          // Величина рассогласования  
             DelTek=(cd[1]-ug0); 
             
          // Условие на запуск процедуры коррекции начала РУ   
             if (fabs(DelTek)>EpsUch){
                printf(" Внимание! Рассогласование по положению относительно начала РУ\n");            
                printf("           Запустить процедуру коррекции? (y->да;n->нет)\n");             
                printf(" => "); gets(sss); 
                if ((sss[0]=='y')||(sss[0]=='Y')){                      
                   printf("           Ждите...\n");
                // Коррекция проводится в режиме "управление по скорости"                      
                   v6_ALR[4]=-0.1*DelTek/fabs(DelTek)*EpsUch;//скорость <обратна> по отношению к рассогласованию
                   skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);                   
                   i=0;
                   while (fabs(cd[1]-ug0)>EpsUch){
                      i=i+1;                      
//                      skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);
                      usleep(dt*1000000);//[мкс]
                    printf("         # i=%d; V= %+12.8f; L=%+12.8f;\n",i,v6_ALR[4],cd[1]);                      
                   };//while
                // printf(" ------------------------------------------------------------------------\n");                                                        
                // Остановка оси ОПУ
                   skorcsn(0.0,0.0,0.0);                
                   printf(" OK! ====> Выполнена задача коррекции начала РУ\n");                      
                }else{
                   printf(" ??? ====> Задача коррекции начала РУ не выполнялась...!?\n");                                      
                }                                                  
             } else printf(" OK! ====> Выполнена установка оси ОПУ в начало РУ\n");
                    printf("           A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);                                          
             break;
        case 'R':                    
          // Переброска к началу разгонного участка (РУ)
             v6_ALR[2]=ug0;                     
             perebroscsn(v6_ALR[0],v6_ALR[1],v6_ALR[2]);
             RESCSN();
             printf("           A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);                   
             printf(" ------------------------------------------------------------------------\n");                               
             v6_ALR[2]=999.0;
          // Величина рассогласования  
             DelTek=(cd[2]-ug0); 
          // Условие на запуск процедуры коррекции начала РУ   
             if (fabs(DelTek)>EpsUch){
                printf(" Внимание! Рассогласование по положению относительно начала РУ\n");            
                printf("           Запустить процедуру коррекции? (y->да;n->нет)\n");             
                printf(" => "); gets(sss); 
                if ((sss[0]=='y')||(sss[0]=='Y')){                      
                   printf("           Ждите...\n");
                // Коррекция проводится в режиме "управление по скорости"                      
                   v6_ALR[5]=-0.1*DelTek/fabs(DelTek)*EpsUch;//скорость <обратна> по отношению к рассогласованию
                   skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);                   
                   i=0;
                   while (fabs(cd[2]-ug0)>EpsUch){
                      i=i+1;                      
//                      skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);
                      usleep(dt*1000000);//[мкс]
                   // printf("         # i=%d; R=%+10.6f;\n",i,cd[2]);                      
                   };//while
                // printf(" ------------------------------------------------------------------------\n");                                                        
                // Остановка оси ОПУ
                   skorcsn(0.0,0.0,0.0);                                
                   printf(" OK! ====> Выполнена задача коррекции начала РУ\n");                      
                }else{
                   printf(" ??? ====> Задача коррекции начала РУ не выполнялась...!?\n");                                      
                }                                  
             } else printf(" OK! ====> Выполнена установка оси ОПУ в начало РУ\n");
                    printf("           A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);
             break;                 
     }//switch         
     printf(" ------------------------------------------------------------------------\n");                                                     
        
};//SpUchVar()

// Определяем факт критического состояния ОПУ (КЭП отключен одним из КВ)      
// Событие должно увязываться с протоколом обмена на уровне CAN
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
     int jpr_L = -1; // нет срабатывания КВ
     int jpr_R = -1; // нет срабатывания КВ     
     int jkw;               
     printf("\n");
     printf("Диагностика положения ОПУ относительно набора КВ (");         
  // printf("угл.пол = %f",xx);printf(")\n");         
     printf("угл.пол = %f",arc_OS);printf(")\n");         
     printf("-----------------------------------------------------------------------\n");         
     for (jkw=0; jkw<Nkw; jkw++){               
     // Технологический вывод
        printf(" # для КВ %d",jkw+1); printf("-го уровня:\n");                                 
     // --------------------------------------------------------            
        printf("   статус левого  КВ = ");            
        if (pStatKW[jkw][0])  printf("<отключение КЭП >");
        else                  printf("<команда для ПАК>");               
        if (arc_OS<=pDataKW[jkw][0]){
           printf(" состояние КВ: <<срабатывание>>\n");        
           if (pStatKW[jkw][0]&&(!pr_L)){pr_L=true;jpr_L=jkw;};
        }
        else printf(" состояние КВ: <<нет реакции >>\n");
     // --------------------------------------------------------            
        printf("   статус правого КВ = ");            
        if (pStatKW[jkw][1])  printf("<отключение КЭП >");
        else                  printf("<команда для ПАК>");
        if (arc_OS>=pDataKW[jkw][2]){
           printf(" состояние КВ: <<срабатывание>>\n");        
           if (pStatKW[jkw][1]&&(!pr_R)){pr_R=true;jpr_R=jkw;};
        }
        else printf(" состояние КВ: <<нет реакции >>\n");
     }//jkw      
     *pr_L0 = pr_L;
     *jL    = jpr_L;    
     *pr_R0 = pr_R;      
 
     *jR    = jpr_R;     
   }//EventKW()     

// Идентификация и чтение файлов БД - ConfKW.txt 
   void ReadConf(char *myFile,bool *my_ERR, struct stLRc mConfKW[][3],int *N){
     int  nkw1,nkw2,nkw3,nkw4,nkw5,nkw6;
     int jkw;
     FILE *f_ConfKW;
     f_ConfKW=fopen(myFile,"r");
     if (!f_ConfKW){
        printf(" --------------------------------------------------------------------------\n");  
        printf(" Внимание! Отсутствует файл БД (конфигурация КВ         ) => <ConfKW.txt>\n");
        printf(" --------------------------------------------------------------------------\n");  
        
        *my_ERR=true;
     } else {
        printf(" --------------------------------------------------------------------------\n");  
        printf(" OK! => Обнаружен   файл БД (конфигурация КВ         ) => <ConfKW.txt>\n");
        printf("        * чтение файла... формируется соответствующий массив структур...\n");
        printf(" --------------------------------------------------------------------------\n");
     // Чтение файла и формирование структуры       
        printf(" :========:=======:=======:=======:=======:=======:=======:\n");                  
        printf(" : Nкв    : az_L  : az_R  : pol_L : pol_R : orb_L : orb_R :\n");
        printf(" :========:=======:=======:=======:=======:=======:=======:\n");     
        jkw=0;
        while (fscanf(f_ConfKW,"%d %d %d %d %d %d\n",&nkw1,&nkw2,&nkw3,&nkw4,&nkw5,&nkw6)!=EOF){
         // Контроль входных данных (формат - 0;1)        
            if (!(((nkw1==0)||(nkw1==1))&&((nkw2==0)||(nkw2==1))&&
                 ((nkw3==0)||(nkw3==1))&&((nkw4==0)||(nkw4==1))&&
                 ((nkw5==0)||(nkw5==1))&&((nkw6==0)||(nkw6==1)) )){
                printf("\n");printf(" Прерывание! Ошибка чтения строки данных...???\n");
        
                printf("\n");
                *my_ERR=true; break;
            }//if        
         // Считаем количество КВ (число строк с <1>)
         // if ( (nkw1==1)||(nkw2==1)||(nkw3==1)||
         //    (nkw4==1)||(nkw5==1)||(nkw6==1) ){
         // // Счетчик     
         //    nConf=nConf+1;
         // }//if
            
            
            mConfKW[jkw][0].prL=(nkw1==1); mConfKW[jkw][0].prR=(nkw2==1);     
            mConfKW[jkw][1].prL=(nkw3==1); mConfKW[jkw][1].prR=(nkw4==1);     
            mConfKW[jkw][2].prL=(nkw5==1); mConfKW[jkw][2].prR=(nkw6==1);     
            
            if (jkw>0) printf(" :--------:-------:-------:-------:-------:-------:-------:\n");                    
            printf(" : Nкв=%02d",jkw+1);printf(" :");
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

// Идентификация и чтение файлов БД - StatKW.txt       
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
        printf(" Внимание! Отсутствует файл БД (функциональный статус КВ) => <StatKW.txt>\n");
        printf(" --------------------------------------------------------------------------\n");          
        *my_ERR=true;
     } else {
        printf(" --------------------------------------------------------------------------\n");  
     
        printf(" OK! => Обнаружен   файл БД (функциональный статус КВ) => <StatKW.txt>\n");
        printf("        * чтение файла... формируется соответствующий массив структур...\n");        
        printf(" --------------------------------------------------------------------------\n");  
        
     // Чтение файла и формирование структуры       
        printf(" :========:==========:==========:==========:==========:==========:==========:\n");                  
        printf(" : Nкв    : az_L     : az_R     : pol_L    : pol_R    : orb_L    : orb_R    :\n");
        printf(" :========:==========:==========:==========:==========:==========:==========:\n");     
        jkw=0;
        while (fscanf(f_StatKW,"%d %d %d %d %d %d\n",&nkw1,&nkw2,&nkw3,&nkw4,&nkw5,&nkw6)!=EOF){
         // Контроль входных данных (формат - 0;1)        
            if (!(((nkw1==0)||(nkw1==1))&&((nkw2==0)||(nkw2==1))&&
                  ((nkw3==0)||(nkw3==1))&&((nkw4==0)||(nkw4==1))&&
                  ((nkw5==0)||(nkw5==1))&&((nkw6==0)||(nkw6==1)) )){
                printf("\n");printf(" Прерывание! Ошибка чтения строки данных...???\n");
        
                printf("\n");
                *my_ERR=true; break;
            }//if        
            mStatKW[jkw][0].prL=(nkw1==1); mStatKW[jkw][0].prR=(nkw2==1);     
            mStatKW[jkw][1].prL=(nkw3==1); mStatKW[jkw][1].prR=(nkw4==1);     
            mStatKW[jkw][2].prL=(nkw5==1); mStatKW[jkw][2].prR=(nkw6==1);     
            
            if (jkw>0) printf(" :--------:----------:----------:----------:----------:----------:----------:\n");
            printf(" : Nкв=%02d",jkw+1);printf(" :");                        
         // Контроль статуса с учетом конфигурации
            if (mConfKW[jkw][0].prL){
               if (mStatKW[jkw][0].prL) printf(" стоп э/д :"); else printf(" событие? :");
            } else printf(" нет к/в  :");            
            if (mConfKW[jkw][0].prR){
               if (mStatKW[jkw][0].prR) printf(" стоп э/д :"); else printf(" событие? :");
            } else printf(" нет к/в  :");                        
            if (mConfKW[jkw][1].prL){
               if (mStatKW[jkw][1].prL) printf(" стоп э/д :"); else printf(" событие? :");
            } else printf(" нет к/в  :");                        
            if (mConfKW[jkw][1].prR){
               if (mStatKW[jkw][1].prR) printf(" стоп э/д :"); else printf(" событие? :");
            } else printf(" нет к/в  :");                        
            if (mConfKW[jkw][2].prL){
               if (mStatKW[jkw][2].prL) printf(" стоп э/д :"); else printf(" событие? :");
            } else printf(" нет к/в  :");            
            if (mConfKW[jkw][2].prR){
               if (mStatKW[jkw][2].prR) printf(" стоп э/д :"); else printf(" событие? :");
            } else printf(" нет к/в  :");                                    

            printf("\n");                                
            jkw=jkw+1;
        }//while;
        *N=jkw;
        if (!*my_ERR) printf(" :========:==========:==========:==========:==========:==========:==========:\n");        
        fclose(f_StatKW);
     }//if (!f_StatKW)
   }//ReadStat(     


// Идентификация и чтение файлов БД - DataKW.txt
   void ReadData(char *myFile,bool *my_ERR,double mFixALR[][2], 
                 struct stDir mDataKW[][3],int *N)
        { 
   
     float pkw1,pkw2,pkw3,pkw4,pkw5,pkw6,pkw7,pkw8,pkw9,pkw10,pkw11,pkw12;
     int jkw;
     FILE *f_DataKW;
     f_DataKW=fopen(myFile,"r");     
     
     if (!f_DataKW){
        printf(" --------------------------------------------------------------------------\n");  
     
        printf(" Внимание! Отсутствует файл БД (угловое положение КВ    ) => <DataKW.txt>\n");
        printf(" --------------------------------------------------------------------------\n");  
        
        *my_ERR=true;
      } else {
        printf(" --------------------------------------------------------------------------\n");  
      
        printf(" OK! => Обнаружен   файл БД (угловое положение КВ    ) => <DataKW.txt>\n");
        printf("        * чтение файла... формируется соответствующий массив структур...\n");        
        printf(" --------------------------------------------------------------------------\n");  
        
     // Чтение файла и формирование структуры       
        printf(" ======:====================================================:\n");     
        jkw=0;        
        while (fscanf(f_DataKW,"%f %f %f %f %f %f %f %f %f %f %f %f\n",
                      &pkw1,&pkw2,&pkw3,&pkw4,&pkw5,&pkw6,
                      &pkw7,&pkw8,&pkw9,&pkw10,&pkw11,&pkw12)!=EOF){

         // Контроль входных данных (формат - 0;1)        
            if (!(
                  (pkw1>= mFixALR[0][0])&&(pkw1<= mFixALR[0][1])&&
                  (pkw3>= mFixALR[0][0])&&(pkw3<= mFixALR[0][1])&&
                  (pkw5>= mFixALR[1][0])&&(pkw5<= mFixALR[1][1])&&
                  (pkw7>= mFixALR[1][0])&&(pkw7<= mFixALR[1][1])&&
                  (pkw9>= mFixALR[2][0])&&(pkw9<= mFixALR[2][1])&&
                  (pkw11>=mFixALR[2][0])&&(pkw11<=mFixALR[2][1])
               )){
               printf("\n");printf(" Прерывание! Выход данных за пределы рабочего диапазона...???\n");
        
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

             printf(" Nкв=%02d:",jkw+1);             
             printf("  Левый (-)  : cko        : Правый (+) : cko        :\n");
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

// Формирование массива-буфера и контроль данных для оценок КВ (состояние БД КВ)
// 2-й вариант
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
          // Формирование текущего массива-буфера функциональных признаков КВ
             pStatKW[jkw][0]=mStatKW[jkw][0].prL;
             pStatKW[jkw][1]=mStatKW[jkw][0].prR;             
          // Формирование текущего массива-буфера конфигурации КВ
             pConfKW[jkw][0]=mConfKW[jkw][0].prL;
             pConfKW[jkw][1]=mConfKW[jkw][0].prR;             
             break;
         case 'L':
             pDataKW[jkw][0]=mDataKW[jkw][1].stDirL.val;                   
             pDataKW[jkw][1]=mDataKW[jkw][1].stDirL.cko;                   
             pDataKW[jkw][2]=mDataKW[jkw][1].stDirR.val;
             pDataKW[jkw][3]=mDataKW[jkw][1].stDirR.cko;                                                     
          // Формирование текущего массива-буфера функциональных признаков КВ
             pStatKW[jkw][0]=mStatKW[jkw][1].prL;
             pStatKW[jkw][1]=mStatKW[jkw][1].prR;             
          // Формирование текущего массива-буфера конфигурации КВ
             pConfKW[jkw][0]=mConfKW[jkw][1].prL;
             pConfKW[jkw][1]=mConfKW[jkw][1].prR;             
             
             break;
         case  'R':
             pDataKW[jkw][0]=mDataKW[jkw][2].stDirL.val;                   
             pDataKW[jkw][1]=mDataKW[jkw][2].stDirL.cko;                   
             pDataKW[jkw][2]=mDataKW[jkw][2].stDirR.val;
             pDataKW[jkw][3]=mDataKW[jkw][2].stDirR.cko;                                                     
          // Формирование текущего массива-буфера функциональных признаков КВ
             pStatKW[jkw][0]=mStatKW[jkw][2].prL;
             pStatKW[jkw][1]=mStatKW[jkw][2].prR;             
          // Формирование текущего массива-буфера конфигурации КВ
             pConfKW[jkw][0]=mConfKW[jkw][2].prL;
             pConfKW[jkw][1]=mConfKW[jkw][2].prR;                          
             break;
      }//switch (typ)

   // Контроль за положением (класс - ошибка смещения)
      if (pr_DL){
         pr_DL = fabs(swDL-pDataKW[jkw][0]) > EpsKW;
         swDL  = pDataKW[jkw][0];
      }                                                                  
      if (pr_DR){
         pr_DR = fabs(swDR-pDataKW[jkw][2]) > EpsKW;
         swDR  = pDataKW[jkw][2];
      }                                                                  
   // Контроль за порядком следования (класс - критическая ошибка)                                      
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


// Процедура вывода набора данных для КВ
   void Print_KW(double pDataKW[][4],int nKW){
      int jkw;                                      
      for (jkw=0;jkw<nKW;jkw++){                  
         printf("   - оценки для ");printf("%d",jkw+1); printf("-го КВ [угл.град]:");                   
         printf(" КВ[левый ]=%10.6f", pDataKW[jkw][0]);
         printf(" CKO=%12.8f\n",      pDataKW[jkw][1]);                
         printf("                                   ");                
         printf(" КВ[правый]= %10.6f",pDataKW[jkw][2]);
         printf(" CKO=%12.8f\n",      pDataKW[jkw][3]);                
     }//jkw                
     
   }//PrintKW


// Индикация статуса задачи для набора данных КВ     
   void Print_Er(bool pr_L,bool pr_R,bool pr_DL,bool pr_DR){
     // Индикация ошибки для левого КВ     
              printf("   --------------------------------------------------------------\n");              
        if (!pr_DL){   
              printf("   Внимание! Ошибка   НД для левого  блока КВ (взаимное положение)\n");
        }else printf("   OK! ====> контроль НД для левого  блока КВ (взаимное положение)\n");     
        if (!pr_L){    
              printf("   Внимание! Ошибка   НД для левого  блока КВ (порядок следования)\n");
        }else printf("   OK! ====> контроль НД для левого  блока КВ (порядок следования)\n");        
     // Индикация ошибки для правого КВ                
        if (!pr_DR){   
              printf("   Внимание! Ошибка   НД для правого блока КВ (взаимное положение)\n");
        }else printf("   OK! ====> контроль НД для правого блока КВ (взаимное положение)\n");         
        if (!pr_R){    
              printf("   Внимание! Ошибка   НД для правого блока КВ (порядок следования)\n");
        }else printf("   OK! ====> контроль НД для правого блока КВ (порядок следования)\n");        
              printf("   --------------------------------------------------------------\n");              
    } //Print_Er           
    
// Процедура ввода начального набора данных по умолчанию для КВ (априорные значения)
// 2-й вариант
   void InpDataKW( struct stDir mDataKW[][3]){

      // Инициализация данных для азимутальной оси - A
      //================================================================      
        mDataKW[0][0].stDirL.val=-208.400;   // [угл.град] // затычка!
        mDataKW[0][0].stDirL.cko=   0.134;   // [угл.град] // затычка!    
      //----------------------------------------------------------------        
        mDataKW[0][0].stDirR.val=+208.400;   // [угл.град] // затычка!
        mDataKW[0][0].stDirR.cko=   0.112;   // [угл.град] // затычка!    
      //================================================================
        mDataKW[1][0].stDirL.val=-209.400;   // [угл.град] // затычка!
        mDataKW[1][0].stDirL.cko=   0.163;   // [угл.град] // затычка!    
      //----------------------------------------------------------------        
        mDataKW[1][0].stDirR.val=+209.400;   // [угл.град] // затычка!
        mDataKW[1][0].stDirR.cko=   0.512;   // [угл.град] // затычка!    
      //================================================================        
        mDataKW[2][0].stDirL.val=-210.400;   // [угл.град] // затычка!
        mDataKW[2][0].stDirL.cko=   0.316;   // [угл.град] // затычка!    
      //----------------------------------------------------------------        
        mDataKW[2][0].stDirR.val=+210.900;   // [угл.град] // затычка!
        mDataKW[2][0].stDirR.cko=   0.042;   // [угл.град] // затычка!    
      //================================================================

      // Инициализация данных для полярной оси - L
      //================================================================      
        mDataKW[0][1].stDirL.val=  20.400;   // [угл.град] // затычка!
        mDataKW[0][1].stDirL.cko=   0.134;   // [угл.град] // затычка!    
      //----------------------------------------------------------------        
        mDataKW[0][1].stDirR.val= 159.900;   // [угл.град] // затычка!
        mDataKW[0][1].stDirR.cko=   0.112;   // [угл.град] // затычка!    
      //================================================================
        mDataKW[1][1].stDirL.val=  19.400;   // [угл.град] // затычка!
        mDataKW[1][1].stDirL.cko=   0.163;   // [угл.град] // затычка!    
      //----------------------------------------------------------------        
        mDataKW[1][1].stDirR.val= 160.900;   // [угл.град] // затычка!
        mDataKW[1][1].stDirR.cko=   0.512;   // [угл.град] // затычка!    
      //================================================================        
        mDataKW[2][1].stDirL.val=  18.400;   // [угл.град] // затычка!
        mDataKW[2][1].stDirL.cko=   0.316;   // [угл.град] // затычка!    
      //----------------------------------------------------------------        
        mDataKW[2][1].stDirR.val= 161.900;   // [угл.град] // затычка!
        mDataKW[2][1].stDirR.cko=   0.042;   // [угл.град] // затычка!    
      //================================================================

      // Инициализация данных для орбитальной оси - R
      //================================================================      
        mDataKW[0][2].stDirL.val=  12.600;   // [угл.град] // затычка!
        mDataKW[0][2].stDirL.cko=   0.134;   // [угл.град] // затычка!    
      //---------------------------------------------------------------        
        mDataKW[0][2].stDirR.val= 169.900;   // [угл.град] // затычка!
        mDataKW[0][2].stDirR.cko=   0.112;   // [угл.град] // затычка!    
      //================================================================
        mDataKW[1][2].stDirL.val=  11.600;   // [угл.град] // затычка!
        mDataKW[1][2].stDirL.cko=   0.163;   // [угл.град] // затычка!    
      //----------------------------------------------------------------        
        mDataKW[1][2].stDirR.val= 170.900;   // [угл.град] // затычка!
        mDataKW[1][2].stDirR.cko=   0.512;   // [угл.град] // затычка!    
      //================================================================        
        mDataKW[2][2].stDirL.val=  10.600;   // [угл.град] // затычка!
        mDataKW[2][2].stDirL.cko=   0.316;   // [угл.град] // затычка!    
      //----------------------------------------------------------------        
        mDataKW[2][2].stDirR.val= 171.900;   // [угл.град] // затычка!
        mDataKW[2][2].stDirR.cko=   0.042;   // [угл.град] // затычка!    
      //=================================================================
   }//InpDatKWos()  

// Массив структур функциональных признаков КВ
// 2-й вариант
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
// Модуль инициализации режима автономных проверок
   int ModKW(){
    // =======================================================================
    // Начало описания дополнительных переменных
    // =======================================================================     
    // Признак первого прохода   
       bool prCP=false;      
    // Обобщенный признак наличия ошибки   
    // bool my_ERR=false;   
    // Переменные цикла
       int ikw,jkw;   
    // Номер оси ОПУ
       int NumOs=-1;     
    // Переменные управляющего функционала   
       int Napp;
       int ikk,jkk;
       double Vtek,Atek,Utek,t0,tn,tk,ttek;
       double mBufKW[5000][4];// массив-буфер   
    // Переменные выбора режима управления осью ОПУ
       int  nReg=-1;
       char sReg[256];      
    // Период формирования команд   
    // !!! - штатные 1Гц для обеспечения устойчивого режима <по скорости>
    // штатные 10Гц - неустойчивый режим <по скорости>
       double dtt=0.1;
    // Рассогласование и порог
       double DelVal;
       double EpsOS=0.1;
    // double EpsKW=0.2; // разрешение по положению КВ
        
    // Имитация состояния ОПУ после включения - установка ОПУ по всем осям (A,L,R)
       double ugA_0,ugL_0,ugR_0;
       ugA_0 =-111.0;
       ugL_0 = 45.0; 
       ugR_0 = 125.0;    
    // Переменные ввода
       char sss[256];
    // Переменные-признаки (номера уровней КВ)   
       int jpr_L=0;
       int jpr_R=0;   
        
    // Допустимое отклонение по положению (начало РУ)
       double EpsUch=0.1;   
    // Переменные количества (чтение данных)   
       int nConf = 0;   
       int nStat = 0;   
       int nData = 0;   
       int nInpKW= 0; // число совпадающих строк (уровней КВ) в файлах БД
    // Признак текущего рассогласования по осям относительно технологического нуля
       bool prDel_Os=false;           
    // Метки файлов БД
       FILE *f_ConfKW,*f_StatKW,*f_DataKW,*f_DataBF,*f_KW;
    // Параметры чтения
       int    nkw1,nkw2,nkw3,nkw4,nkw5,nkw6;
       float  pkw1,pkw2,pkw3,pkw4,pkw5,pkw6,pkw7,pkw8,pkw9,pkw10,pkw11,pkw12;
    // Массив констант-ограничений
       double nFixALR[3][2];
       nFixALR[0][0]=AminKW; nFixALR[0][1]=AmaxKW;
       nFixALR[1][0]=LminKW; nFixALR[1][1]=LmaxKW;                
       nFixALR[2][0]=RminKW; nFixALR[2][1]=RmaxKW;     

    // Матрица конфигурации КВ (распайка и подключение КВ + копир)
    // -----------------------------------------------------------
    // Обобщенная структура
    // mConfKW[кол-во КВ][тип оси ОПУ]
       struct stLRc mConfKW[nnSwEnd][3];   
    // ----------------------------------   
    // Текущая для выбранной оси ОПУ   
    // pConfKW[кол-во КВ][0-левый(=>-),1-правый(=>+)]
       bool pConfKW[nnSwEnd][2];          

    // Матрица функциональных признаков КВ (отключает КЭП или нет)
    // -----------------------------------------------------------
    // Обобщенная структура
    // mStatKW[кол-во КВ][тип оси ОПУ]
       struct stLR mStatKW[nnSwEnd][3]; 
    // ----------------------------------   
    // Текущая для выбранной оси ОПУ   
    // pStatKW[кол-во КВ][0-левый(=>-),1-правый(=>+)]  
       bool pStatKW[nnSwEnd][2];          
    // ----------------------------------   
    
    // Матрица данных по КВ (оценка + ско)
    // -----------------------------------------------------------
    // Обобщенная структура
    // mDataKW[кол-во КВ][тип оси ОПУ]
       struct stDir mDataKW[nnSwEnd][3];      
    // ----------------------------------          
    // Буфер данных с оценками
       struct stDir mDataBuf[nnSwEnd][3];
    // ----------------------------------          
    // Признаки обновления данных
       struct prDir prDataBuf[nnSwEnd][3];       
    // ----------------------------------   
    // Текущая для выбранной оси ОПУ   
    // pDataKW[кол-во КВ][0,1-левый; 2,3-правый]
       double pDataKW[nnSwEnd][4];
    // ----------------------------------   

    // Параметры начальной установки оси ОПУ ОЭС
       double ValOs0,ValOsMax,ValOsMin,DelOs;     
   
    // Переменные-признаки состояния КВ
       bool pr_L0,pr_R0;     
       bool pr_L ,pr_R; 
       bool pr_DL,pr_DR;
   
    // Переменная типа оси ОПУ   
       char code_ALR;
       char sCod_ALR[256];
    // Для вывода времени   
       const time_t my_timer=time(NULL); 
       char *my_str;        
    // Текущее значение положения текущей оси ОПУ - вспомогательная переменная
       double arc_OS;   
    // Шестимерный вектор для управления осями ОПУ    
       double v6_ALR[6];
    // Признак текущего рассогласования по осям относительно технологического нуля
       prDel_Os=false;  
    // Команда управление уровнем КВ
       char sTek[256];   
       int  nTek=-1;      
        
    // Максимально допустимые значения скорости и ускорения
    // mAVmaxKW[скорость,ускорение][левый,правый]     
       double mAVmaxKW[2][2];      
       double VmaxOS,AmaxOS;      
    // -----------------------------------------------------------------------         
       mAVmaxKW[0][0] = -6.0; //[угл.град/сек  ] - для левого  КВ
       mAVmaxKW[0][1] =  6.0; //[угл.град/сек^2] - для правого КВ                  
    // -----------------------------------------------------------------------               
    //   mAVmaxKW[1][0] = -3.0; //[угл.град/сек  ] - для левого  КВ
    //   mAVmaxKW[1][1] =  3.0; //[угл.град/сек^2] - для правого КВ  
       mAVmaxKW[1][0] = -0.6; //[угл.град/сек  ] - для левого  КВ
       mAVmaxKW[1][1] =  0.6; //[угл.град/сек^2] - для правого КВ                         
    // -----------------------------------------------------------------------               

    // Параметры для описания управляющей функции
    // ---------------------------------------------------------------   
    // Привязка линейного участка (привязано к положению текущего КВ)      
    // (начало,середина,конец)
       double mPLineKW[3];      
    // ---------------------------------      
    // Параметры экпоненты (участок от <0> до начала линейного участка)      
    // y=A0*[1-exp(-k*x)];
    // mExpKW[амплитуда,постоянная];
       double mExpKW[2];
    // ---------------------------------
    // Параметры линейного участка (привязано к положению текущего КВ)
    // y=V*(t-t0)+y0;
    // mLineKW[время,фаза,скорость]
       double mLineKW[3];
    // ---------------------------------
    // Параметры параболы - разгонного участка
    // y =1/2*a*x^2+b*x;
    // Vy=a*x+b; - мгновенная скорость
    // Ay=a;     - ускорение
    // mPParKW[время начала/опережения,фаза по положению];
       double mPParKW[2];
    // ---------------------------------     
    // Имитация срабатывания КВ                  
       double arc_GSS=0;// оценка положения КВ + равномерный шум
       bool   prExeKW=false;           
    // ---------------------------------     
     
    // Обобщенный признак наличия ошибки   
       bool my_ERR=false;   

    // Признак для текущих оценок состояния   
       bool prTrue=false;   
        
    // =======================================================================
    // Конец описания дополнительных переменных
    // =======================================================================
       
    // Идентификация режима работы ПАК КПСТ    
       printf(" ------------------------------------------------------------------------\n");             
       if (IMIT){
          printf(" Внимание! Инициализация модуля в режиме <имитатор>\n");
          printf("           # формируется альтернативный признак состояния КВ\n");
          printf("           # точка срабатывания КВ соответствует: [оценка БД]+[ГСС]\n");               
          printf("           # управление осями ОПУ - имитатор модуля <ЦСН>\n");                              
       }else 
          printf(" Внимание! Инициализация модуля в режиме <реальный>\n");
       printf(" ------------------------------------------------------------------------\n");        
     
    // Только для остановки (просмотр) !!!        
    // printf(" # NEXT (нажмите <Ввод>) ==> "); gets(sss); 
       printf(" => "); gets(sss);        
   
    // Идентификация и чтение файлов БД - ConfKW.txt 
       nConf=0;
       ReadConf("ConfKW.txt",&my_ERR,mConfKW,&nConf); // &nConf - количество КВ
    // Технологический вывод - кол-во КВ из файла конфигурации
       printf(" Nкв(nConf)= %d\n",nConf);   

    // Идентификация и чтение файлов БД - StatKW.txt 
       if (!my_ERR){
       // Только для остановки (просмотр) !!!      
       // printf(" # NEXT (нажмите <Ввод>) ==> "); gets(sss);      
          printf("=> "); gets(sss);      
          nStat=0;
          ReadStat("StatKW.txt",&my_ERR,mStatKW,mConfKW,&nStat); // &nStat - количество КВ
       // Технологический вывод - кол-во КВ из файла конфигурации
          printf(" Nкв(nStat)= %d\n",nStat);      
       // Контроль за совпадением количества КВ
          printf(" ------------------------------------------------------------------------\n");        
       if (nStat!=nConf){
          printf(" Внимание! Несовпадение <Nкв> в файлах <ConfKW> и <StatKW>\n");
          my_ERR=true; 
       }else 
          printf(" OK! => совпадение строк   в файлах <ConfKW> и <StatKW>\n");        
          printf(" ------------------------------------------------------------------------\n");           
       }//if (!my_ERR)   
       
    // Идентификация и чтение файлов БД - DataKW.txt
       if (!my_ERR){ 
       // Только для остановки (просмотр) !!!
       // printf(" # NEXT (нажмите <Ввод>) ==> "); gets(sss);
          printf("=> "); gets(sss);      
          printf("\n");  
          nData=0;
          ReadData("DataKW.txt",&my_ERR,nFixALR,mDataKW,&nData); // &nData - количество КВ
       // Технологический вывод - кол-во КВ из файла по положению
          printf(" Nкв(nData)= %d\n",nData);              
       // Контроль за совпадением количества КВ
          printf(" ------------------------------------------------------------------------\n");             
          if (nData!=nStat){
             printf(" Внимание! Несовпадение <Nкв> в файлах <DataKW> и <StatKW>\n");
             my_ERR=true; 
          }else{
             printf(" OK! => совпадение строк в файлах <DataKW> и <StatKW>\n"); 
          // Далее используется как параметр              
             nInpKW=nData;
          }//if                      
          printf(" ------------------------------------------------------------------------\n");                
       }//if (!my_ERR)          

    // Обнуление буфера данных и признаков обновления
       for (ikw=0; ikw<nInpKW; ikw++){
          for (jkw=0; jkw<3; jkw++){
          // Буфер буфер данных
             mDataBuf[ikw][jkw].stDirL.val = 0.0;
             mDataBuf[ikw][jkw].stDirL.cko = 0.0;
             mDataBuf[ikw][jkw].stDirR.val = 0.0;
             mDataBuf[ikw][jkw].stDirR.cko = 0.0;
          // Признаки обновления             
             prDataBuf[ikw][jkw].mLR[0]=0;
             prDataBuf[ikw][jkw].mLR[1]=0;             
          };//jkw
       };//ikw                    

    // ========================= контроллер ошибок ===========================
       if (my_ERR) {
          PrMsg("Отмена текущей задачи - ошибка в структуре БД");        
          my_ERR=false;
          return 1;// аварийный выход за пределы процедуры!              
       }//if
    // =======================================================================        
    
    // Только для остановки (просмотр) !!!    
       printf("=> "); gets(sss);      

    // ======================================================================    
    // Конец чтения и анализа БД по КВ !!!
    // ======================================================================
    
    // Вывод значений для рабочего диапазона всех осей ОПУ
       OutDelWork(nInpKW,mDataKW,mConfKW);
    // Только для остановки (просмотр) !!!    
       printf("=> "); gets(sss);      
    // -----------------------------------------------------------------------  

    // Форматирование и отображение массива признаков наличия сообщения от CAN     
    // Прояснить возможность появления конфликтов с "паралельным процессом"
    // Как обстоят дела с сообщениями после включения ОЭС ???
    // Пока считаем, что они сразу пришли и матрица <mmCanKW> заполнена...!?
    
    // ИМИТАЦИЯ !!!     
          
    // ---------     
    // Индикация состояния и установка по умолчанию (обнуление для первого прохода)
    // <true > - обнуление и отображение
    // <false> - только отображение
//     if ((IMIT)&&(!prCP)) 
       if (!prCP)        
          ScrCanKW(true,nInpKW,mmCanKW,prCanKW);     
    // На выходе <prCP> не изменился !
    // -----------------------------------------------------------------------    
    // Только для остановки (просмотр) !!!
    // printf(" # NEXT (нажмите <Ввод>) ==> "); gets(sss); 
       printf("=> "); gets(sss);      
    // -----------------------------------------------------------------------  

     
    // Установка нейтральных значений для вектора управления (999.0)
       for (jkw=0; jkw<6; jkw++) v6_ALR[jkw]=999.0;

    // Только для имитации и при старте
       if (!prCP){
       // Начальная установка ОПУ по всем осям (A,L,R) - состояния ОПУ после включения
          
       // Только для имитации!
          if (IMIT) 
             IniValOs(nInpKW,&prCP,mDataKW,cd);
          // На выходе <prCP> изменился !              
          else{
             printf(" ------------------------------------------------------------------------\n");     
        
             printf(" CD! => A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);
           
          };//if
       // Состояние (признаки отработки) сообщений CAN для КВ по данным БД
          StatCanKW(nInpKW,mDataKW,mmCanKW);
       // Отображение признаков
          ScrCanKW(false,nInpKW,mmCanKW,prCanKW);                   
       // На выходе всегда!   
          prCP=true;           
       // -----------------------------------------------------------------------    
       // Только для остановки (просмотр) !!!           
          printf("=> "); gets(sss);      
       // -----------------------------------------------------------------------             
       };//if - Начальная установка ОПУ..                

    // Послать запрос КДПО о состоянии КВ для всех осей 
    // (инициализация начальных условий для матрицы mmCanKW)       
       if (!IMIT){       
//             SendKdpoKW(dtt);
             printf("=> "); gets(sss);      
          // printf("\n");               
             printf(" ------------------------------------------------------------------------\n");     
             printf(" Ожидание? Задержка операций до получения ответа о состоянии КВ (КДПО)\n");
             printf("           Ждите...\n");                                          
          // usleep(5.0*1000000);                                                         
             my_delay(3.0);

       // Отображение признаков
          ScrCanKW(false,nInpKW,mmCanKW,prCanKW);                   
       // -----------------------------------------------------------------------    
       // Только для остановки (просмотр) !!!           
          printf("=> "); gets(sss);      
          
/*             
          // Признак получения сообщения от CAN
          // Пока - один любой из всех запрошенных!
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
                printf(" Внимание! За установленное время, сообщение о состоянии КВ не получено\n");          
                printf("           Выход из программного модуля в цикл формирования БЗ\n");                       
                printf(" ------------------------------------------------------------------------\n");                          
                return 6;
             }else
                printf(" OK! ====> Получено не менее одного ответа от КДПО о состоянии КВ\n");
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
    // Просто безусловный цикл опроса - после выбора режима автономных проверок 
       while (true){        
       
         // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
         // Инициализация режима автономных проверок ОПУ и выбор оси ОПУ
         // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@     
            printf(" Инициализация режима автономных проверок ОПУ:\n");
            printf("  # контроль рабочего диапазона азимутальной оси => A:\n");
            printf("  # контроль рабочего диапазона полярной     оси => L:\n");
            printf("  # контроль рабочего диапазона орбитальной  оси => R:\n");        
         // Ввести и считать мнемокод оси ОПУ => a(A),l(L) или r(R)    
            printf("  ? ввод (мнемо оси) => "); 
            gets(sCod_ALR);//корректный ввод - один символ!
         // Форматирование строки-признака (приведение к верхнему регистру)         
            code_ALR=toupper(sCod_ALR[0]);// выбираем только первый символ!            
            if ( (!(strlen(sCod_ALR)==1))||(!((code_ALR=='A')||(code_ALR=='L')||(code_ALR=='R')))  ){
               PrMsg("Ошибка ввода данных - недопустимый формат команды.");
               break;// всегда выход за безусловный цикл опроса!              
            };//if          
            
            printf("\n");        
         // Дата и время для протокола
            my_str=ctime(&my_timer);
                    
            printf(" Дата и время для протокола = %s",my_str);
            
            printf(" Обращение к БД оценок положения концевых выключателей:\n");
            printf("\n");                
         // Начальная установка параметров-признаков контроля набора данных
            pr_L  = true; pr_R  = true;
            pr_DL = true; pr_DR = true;
     
         // Селектор типа оси ОПУ ОЭС        
            switch (code_ALR){
                case 'A':
                    NumOs=0;
                    printf(" * Выбрана азимутальная ось (A)\n");
                    printf("   текущее значение углового положения оси = %10.6f\n",cd[0]);           
                    arc_OS=cd[0];
                 // Контроль данных для оценок КВ (состояние БД КВ)                              
                 // 2-й вариант
                    GetBufKWos('A',pConfKW,mConfKW,pStatKW,mStatKW,nInpKW,pDataKW,mDataKW,&pr_L,&pr_R,&pr_DL,&pr_DR);                                
                    break;                
                case 'L':
                    NumOs=1;
                    printf(" * Выбрана полярная ось (L)\n");
                    printf("   Текущее значение углового положения оси = %10.6f\n", cd[1]);
                    arc_OS=cd[1];
                 // Контроль данных для оценок КВ (состояние БД КВ)                              
                 // 2-й вариант
                    GetBufKWos('L',pConfKW,mConfKW,pStatKW,mStatKW,nInpKW,pDataKW,mDataKW,&pr_L,&pr_R,&pr_DL,&pr_DR);
                    break;                
                case 'R':
                    NumOs=2;
                    printf(" * Выбрана орбитальная ось (R)\n");
                    printf("   Текущее значение углового положения оси = %10.6f\n", cd[2]);                            
                    arc_OS=cd[2];
                 // Контроль данных для оценок КВ (состояние БД КВ)                              
                 // 2-й вариант
                    GetBufKWos('R',pConfKW,mConfKW,pStatKW,mStatKW,nInpKW,pDataKW,mDataKW,&pr_L,&pr_R,&pr_DL,&pr_DR);
                    break;            
                
                default:
                    PrMsg("Ошибка ввода данных - недопустимый формат команды.");
                    break;// всегда выход за безусловный цикл опроса!                                
            }//switch (code_ALR)

         // Только для остановки (просмотр) !!!
         // printf(" # NEXT (нажмите <Ввод>) ==> "); gets(sss);

         // Индикация статуса задачи для набора данных КВ     
            Print_Er(pr_L,pr_R,pr_DL,pr_DR);     
         // Вывод данных по КВ (задача контроля данных - OK!)
//pr_L=false;
            if (pr_L&&pr_R&&pr_DL&&pr_DR) Print_KW(pDataKW,nInpKW);
            else{ printf("   ...Отмена текущей БЗ...\n");
                  return 2;// аварийный выход за пределы процедуры!              
            }//if
            
         // Только для остановки (просмотр) !!!
//            printf(" # NEXT (нажмите <Ввод>) ==> "); gets(sss);
            printf("=> "); gets(sss);      
          
         // Далее работаем с выбранной осью ОПУ
         // -----------------------------------
         // Интегральный признак
            pr_L0=false;
            pr_R0=false;
         // Определяем факт критического состояния ОПУ (КЭП отключен одним из КВ)      
         // Событие должно увязываться с протоколом обмена на уровне CAN
            pr_L=false;jpr_L=-1;
            pr_R=false;jpr_R=-1;      
            printf("\n");
            EventKW(&pr_L,&pr_R,&jpr_L,&jpr_R,arc_OS,pStatKW,pDataKW,nInpKW);         
         // Запомнили для последующего сравнительного анализа    
//pr_L =true;                     
            pr_L0=pr_L;
            pr_R0=pr_R;
            printf(" -----------------------------------------------------------------------\n");                  

         // Интегральная оценка        
            if ((pr_L)||(pr_R)){
               
               printf(" Статус ОПУ по данным БД  => [эл.привод отключен] - ");         
               if (pr_L){ printf(" левый ");printf("%d",jpr_L+1);printf("-го уровня\n");}
               if (pr_R){ printf(" правый ");printf("%d",jpr_R+1);printf("-го уровня\n");}
//               printf(" ...Отмена текущей БЗ...\n");
                              
//               break;// всегда выход за безусловный цикл опроса!               
                 
            }//if
            
            else printf(" Статус ОПУ по данным БД  => [управление КЭП разрешено]\n");
            
         // printf(" -----------------------------------------------------------------------\n");                           
//pStatKW[0][0]=1;
//mmCanKW[0][NumOs].mLR[0]=1;
         // Определяем факт критического состояния ОПУ (КЭП отключен одним из КВ)      
            
         // Рассмотреть необходимость учета признаков статуса КВ из БД !?          
         // По данным CAN (доработать !!! - сделать запрос о статусе КВ)        
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
         // Интегральная оценка        
            if ((pr_L)||(pr_R)){
               printf(" Статус ОПУ по данным CAN => [эл.привод отключен] - ");         
               if (pr_L){ printf(" левый ");printf("%d",jpr_L+1);printf("-го уровня\n");}
               if (pr_R){ printf(" правый ");printf("%d",jpr_R+1);printf("-го уровня\n");}
//               printf(" ...Отмена текущей БЗ...\n");
               
//               break;// всегда выход за безусловный цикл опроса!               
            }//if
            else printf(" Статус ОПУ по данным CAN => [управление КЭП разрешено]\n");
                        
            printf(" -----------------------------------------------------------------------\n");                           
            
         // Арбитраж признаков
            if (!( ((pr_L )&&(pr_L0 )||(!pr_L)&&(!pr_L0))&&
                   ((pr_R )&&(pr_R0 )||(!pr_R)&&(!pr_R0)) )){
//             printf(" Внимание! Конфликт соответствия статуса КВ применительно к CAN и БД\n");
                
//             printf("           Отмена текущей БЗ...\n");
//             break;// всегда выход за безусловный цикл опроса!               
               return 3;               
            };//if
         // Именно в таком порядке следования    
            if ( ((pr_L )&&(pr_L0 ))||((pr_R )&&(pr_R0)) ){
               return 4;
            };//if
       
         // Установка ОПУ для выбранной оси в положение по умолчанию
            printf(" КЭП >>>>> Установка оси ОПУ в положение по умолчанию - ");                        
            for (ikw=0; ikw<6; ikw++) v6_ALR[ikw]=999.0;// нейтраль
            switch (code_ALR){
                case 'A': NumOs=0;v6_ALR[0]=kw_A0; printf("%+006.1f",v6_ALR[0]); break;
                case 'L': NumOs=1;v6_ALR[1]=kw_L0; printf("%+006.1f",v6_ALR[1]); break;
                case 'R': NumOs=2;v6_ALR[2]=kw_R0; printf("%+006.1f",v6_ALR[2]); break;                 
            }//switch                               
         // Собственно установка ОПУ
            printf(" [угл.град]\n");     
            printf("           режим КЭП - <по положению>\n");        
            printf("           Ждите... \n");      
            perebroscsn(v6_ALR[0],v6_ALR[1],v6_ALR[2]);
            RESCSN();
            printf(" OK! ====> A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);
            printf(" ------------------------------------------------------------------------\n");

         // Метка входа после цикла коррекции !!!
            myREP:;     
         // Рекурсия параметров - для повторного использования     
            for (ikw=0; ikw<6; ikw++) v6_ALR[ikw]=999.0;       
         // Признак текущего рассогласования по осям относительно технологического <0>
            prDel_Os = false;  
         // EpsOS    = 0.01;  
         // Принятие решения        
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
         // Оценка рассогласования
            if (prDel_Os) {
               printf(" ------------------------------------------------------------------------\n");      
               printf(" OK! ====> Выполнена задача приведения осей ОПУ к технологическому <0>\n");
               printf("           A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);       
               printf(" ------------------------------------------------------------------------\n");               
               printf("\n");
            }else{
               printf(" ------------------------------------------------------------------------\n");     
               printf(" Внимание! Недопустимое рассогласование относительно технологического <0>\n");
               printf("           Запустить процедуру коррекции? (y->да;n->нет)\n");             
               printf(" => "); gets(sss); 
               if ((sss[0]=='y')||(sss[0]=='Y')){         
                  printf("           Ждите...\n");      
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
                // Остановка оси ОПУ
                   skorcsn(0.0,0.0,0.0);                                  
                  goto myREP;
               } else printf(" ??? ====> процедура коррекции не выполнялась\n");                          
               printf("\n");
            }//if (prDel_Os) - Оценка рассогласования
            
//mmCanKW[2][NumOs].mLR[1]=1;
         // Дополнительно (контроль строки признаков CAN)        
         // Опросить КДПО на предмет статуса КВ !?
            pr_L=false; pr_R=false;
            for (ikw=0; ikw<nInpKW; ikw++){
               pr_L=mmCanKW[ikw][NumOs].mLR[0];
               pr_R=mmCanKW[ikw][NumOs].mLR[1];               
            };//ikw
            if ((pr_L)||(pr_R)){            
               return 5;
            };//if
         
         // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
         // Ввести и считать номер уровня (пары КВ) текущего КВ
         // &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&   
            nTek=-1;
            printf(" Выбрать текущий номер пары (уровня) для КВ:\n");
            printf(" ---------------------------------------------\n");                        
            printf("  1 - первая пара (уровень) КВ\n");            
            printf("  2 - вторая пара (уровень) КВ\n");                        
            printf("  3 - и т.д. до max-допустимого значения Nconf\n");                        
            printf(" ---------------------------------------------\n");            
            printf("  ? ввод (номер пары) => "); gets(sTek);       
            sscanf(sTek,"%d",&nTek);     
         // ====================================================================
         // Контроль формата ввода переменной "nTek"
            my_ERR=false; // nInpKW - количество уровней КВ в файлах БД
            for (ikw=1; ikw<=nInpKW; ikw++) if (nTek==ikw){ my_ERR=true;break; };          
            if (!my_ERR){
               PrMsg("Ошибка ввода данных - недопустимый формат команды.");
               break;// всегда выход за безусловный цикл опроса!         
            }else{     
               nTek=nTek-1; // привязка к индексу массива              
            // Контроль физического наличия КВ для выбранного уровня (левый,правый)
               if (!(pConfKW[nTek][0]||pConfKW[nTek][1])){
                  PrMsg("Для выбранного уровня отсутствует распайка КВ.");         
                  break;// всегда выход за безусловный цикл опроса!
               } else my_ERR=false;// возвращаем нач.значение для повт.использования
            }//if          
         // ====================================================================   
         
         // #########################################################################               
         // Цикл по левому и правому КВ - обработка текущего КВ
            for (jkw=0; jkw<2; jkw++){
         // #########################################################################         
             // Селектор типа оси ОПУ - формируется текущее значение технологического <0>
                switch (code_ALR){
                    case 'A': arc_OS=kw_A0; break;
                    case 'L': arc_OS=kw_L0; break;
                    case 'R': arc_OS=kw_R0; break;                
                }//switch              
             // Контроль физического наличия выбранного КВ
                if (!pConfKW[nTek][jkw]){
                   if (jkw==0) PrMsg("Для левого  КВ отсутствует распайка (физическое наличие).");
                   else        PrMsg("Для правого КВ отсутствует распайка (физическое наличие).");
//                   printf(" # NEXT (нажмите <Ввод>) ==> "); gets(sss);
                   printf("=> "); gets(sss);      
                   continue;// всегда выход в начало цикла!   
                };//if         
             // Максимально допустимые значения скорости и ускорения          
                VmaxOS=mAVmaxKW[0][jkw];
                AmaxOS=mAVmaxKW[1][jkw];         
                
             // Привязка линейного участка (привязано к положению текущего КВ)               
             // ----------------------------------------------------------------------         
             // arc_OS - поправка (привязка к нулевому значению) на технологический <0>
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
             // Печать координат (углов) для линейного участка
                printf(" # положение ЛУ: Ln= %+10.6f; Lkw= %+10.6f; Lk= %+10.6f;\n",mPLineKW[0],mPLineKW[1],mPLineKW[2]);
         
             // Параметры экпоненты (участок от <0> до начала линейного участка)      
             // mExpKW[амплитуда предела,постоянная]
             // ----------------------------------------------------------------------      
                mExpKW[0]=mPLineKW[1];     
                mExpKW[1]=VmaxOS/mPLineKW[1];
                printf(" # параметры ЭУ: A== %+10.6f; k=== %+10.6f\n",mExpKW[0],mExpKW[1]);

             // Параметры линейного участка (привязано к положению текущего КВ)
             // mLineKW[время,фаза,скорость][левый,правый]
             // ----------------------------------------------------------------------      
                mLineKW[0] = -log(1-mPLineKW[0]/mPLineKW[1])/mExpKW[1];
                mLineKW[1] = mPLineKW[0];
                mLineKW[2] = mExpKW[0]*mExpKW[1]*exp(-mExpKW[1]*mLineKW[0]);
                printf(" # параметры ЛУ: Tn= %+010.6f; Ln== %+10.6f; V== %+10.6f\n",mLineKW[0],mLineKW[1],mLineKW[2]);

             // Параметры параболы - разгонного участка
             // ----------------------------------------------------------------------         
                mPParKW[0] = -VmaxOS/AmaxOS;
                mPParKW[1] =  AmaxOS*mPParKW[0]*mPParKW[0]/2+VmaxOS*mPParKW[0];
                printf(" # параметры ПУ: t0=  %+10.6f; P0==  %+10.6f\n",mPParKW[0],mPParKW[1]);
                printf(" ========================================================================\n");    
           
             // Приведение выражений для функций к "0" относительно начала разгонного участка
             // ----------------------------------------------------------------------               
                printf(" Приведение функционала к <0> относительно начала РУ:\n");
                printf(" ---------------------------------\n");
                printf(" * P(t)=Amax*t^2+P0;\n");
                printf(" * E(t)=A*{1-exp[-k*(t+t0)]};\n");
                printf(" * L(t)=V*(t+t0-Tn)+Ln;\n"); 
                printf(" ---------------------------------\n");
                
             // Переброска к началу разгонного участка ( + коррекция)
             // EpsUch=0.01; // [угл.град]                            
             // SpUchVar(code_ALR,EpsUch,dtt,mPParKW[1]);
            
             // Только для остановки (просмотр) !!!           
             // printf(" # NEXT (нажмите <Ввод>) ==> "); gets(sss);

             // Задержка разгонного участка (в минус относительно рабочего участка)
                t0=mPParKW[0];            
             // Начало линейного участка относительно начала РУ tn=|t0|+Tn
                tn=fabs(t0)+mLineKW[0];   
             // Конец линейного участка относительно начала РУ,т.е. tk=tn+(Lk-Ln)/V                                    
                tk=tn+fabs((mPLineKW[2]-mPLineKW[0])/mLineKW[2]); 
             // Кол-во тактов управления (f=10Гц - по умолчанию для сети CAN)
                Napp=abs((tk+1.0)/dtt);  
         
             // Текущий вывод         
             // printf(" Napp= %d; t0=%f; tn=%f; tk=%f; dtt=%f\n",Napp,t0,tn,tk,dtt);            
             // Только для остановки (просмотр) !!!           
                printf("=> "); gets(sss);      

             // Переброска к началу разгонного участка ( + коррекция)
//              EpsUch=0.0001; // [угл.град]                            
                SpUchVar(code_ALR,EpsUch,dtt,arc_OS+mPParKW[1]);
         
             // Цикл по отсчетам относительно начала РУ (создать массив-буфер)
                printf(" ------------------------------------------------------------------------\n");      
                printf(" Внимание! Выполняется задача формирования сплайна в буфер\n");
                printf("           Ждите...\n");                                     
                for (ikw=0; ikw<=Napp; ikw++){                        
                   ttek=ikw*dtt;               
                // Разгонный участок
                   if (ttek<=fabs(t0)){
                      Utek=arc_OS+1.0/2.0*AmaxOS*ttek*ttek+mPParKW[1];
                      Vtek=AmaxOS*ttek;
                      Atek=AmaxOS;
                   // printf(" ikw= %d; ttek= %f; Utek= %f;\n",ikw,ttek,Utek);
                   }//if               
                // Участок экспоненты
                   if ((ttek>fabs(t0))&&(ttek<=tn)){
                      Utek=arc_OS+mPLineKW[1]*(1-exp(-mExpKW[1]*(ttek+t0)));
                      Vtek= mPLineKW[1]*mExpKW[1]*exp(-mExpKW[1]*(ttek+t0));
                      Atek=-mPLineKW[1]*mExpKW[1]*mExpKW[1]*exp(-mExpKW[1]*(ttek+t0));
                   // printf(" ikw= %d; ttek= %f; Utek= %f;\n",ikw,ttek,Utek);
                   }//if
                // Линейный участок
                   if (ttek>tn){
                      Utek=arc_OS+mLineKW[2]*(ttek+t0-mLineKW[0])+mPLineKW[0];
                      Vtek=mLineKW[2];
                      Atek=0;
                   // printf(" ikw= %d; ttek= %f; Utek= %f;\n",ikw,ttek,Utek);
                   }//if               
                // Запись в буфер
                   mBufKW[ikw][0]=ttek; // текущее время
                   mBufKW[ikw][1]=Utek; // положение
                   mBufKW[ikw][2]=Vtek; // мгновенная скорость  
                   mBufKW[ikw][3]=Atek; // ускорение
                }//ikw - цикл по отсчетам относительно начала РУ (создать массив-буфер)         
                printf(" OK! ====> Выполнена задача формирования сплайна.\n");                  
                printf(" ------------------------------------------------------------------------\n");
         
             // Создать (перезаписать) файл данных
             // FILE *f_KW;
             // Сформировать имя файла для отображения     
                char my_Str[80]="kw_";  my_Str[3]=code_ALR;       
                my_Str[4]='_';          my_Str[5]=sTek[0];                        
                if (jkw==0) my_Str[6]='L'; else my_Str[6]='R';            
                strcat(my_Str,".txt");            
             // Открыть файл                
                f_KW=fopen(my_Str,"w");                           
             // Выбрать режим управления осью ОПУ
                nReg=-1;      
                printf(" Инициализация режима управления осью ОПУ:\n");
                printf(" ------------------------------------------------\n");                
                printf("  0 - по скорости (текущее значение производной)\n");
                printf("  1 - по скорости (численная производная на шаге)\n");
                printf("  2 - по положению на следующий шаг\n");        
                printf(" ------------------------------------------------\n");                                
                printf("  ? ввод (мнемо режима => "); 
                gets(sReg);       
                sscanf(sReg,"%d",&nReg);              
             // ====================================================================
             // Контроль формата ввода переменной "nReg"
                my_ERR=false;
                for (ikw=0; ikw<=2; ikw++) if (nReg==ikw){ my_ERR=true;break; };          
                if (!my_ERR){
                   PrMsg("Ошибка ввода данных, недопустимый формат команды");                   
                   break;// всегда выход за безусловный цикл опроса!                   
                // goto mEND;
                };//if
             // ====================================================================                    
             // Установка шестимерного вектора по умолчанию (нейтральное значение)      
                for (ikk=0; ikk<6; ikk++) v6_ALR[ikk]=999.0;
           
             // Подготовка для имитации срабатывания КВ (альтернативные параметры)
                if (IMIT){
                   arc_GSS = my_random1(pDataKW[nTek][jkw+jkw],pDataKW[nTek][1+jkw+jkw]);
                   printf("    arc_KW= %f; CKO= %f; arc_GSS= %f;\n",
                            pDataKW[nTek][jkw+jkw],pDataKW[nTek][1+jkw+jkw],arc_GSS);    
/*                                                                                  
                // Только для отладки !!!
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
             // Начало цикла чтения массива-буфера (условие на 1 ед.меньше)
             // ===============================================================
                printf(" Внимание! Выполняется задача формирования файла данных.\n");
                prExeKW=false;
                for (ikw=0; ikw<Napp; ikw++){                   
                
                // !!!                
                // Чтение буфера данных
                // ------------------------------------------------------------                
                   ttek=mBufKW[ikw][0]; // текущий момент времени
                   Utek=mBufKW[ikw][1]; // текущее положение                          
                   Atek=mBufKW[ikw][3]; // текущее ускорение

                // !!!                                                                  
                // Только для начало цикла - начало разгонного участка
                // ------------------------------------------------------------                                                                                                
                   if (ikw==0){
                      printf(" Имитация! Управление осью ОПУ <");
                      if (nReg==2) printf("по положению>\n");               
                      else         printf("по скорости>\n");                              
                      printf("           Ждите...\n");                         
                      printf("=> "); gets(sss);                         
                   };//if
                   
                // !!!                                   
                // Селектор участков траектории (функция управления)        
                // ------------------------------------------------------------                
                   if (ttek<=fabs(t0)) printf(" РУ=> ");             // Разгонный участок                
                   if ((ttek>fabs(t0))&&(ttek<=tn)) printf(" ЭУ=> ");// Участок экспоненты                
                   if (ttek>tn) printf(" ЛУ=> ");                    // Линейный участок

                // !!!                                   
                // Селектор типа оси ОПУ (положение)                
                // ------------------------------------------------------------                
                   switch (code_ALR){
                       case 'A': arc_OS=cd[0]; break;
                       case 'L': arc_OS=cd[1]; break;
                       case 'R': arc_OS=cd[2]; break;     	            
                   }//switch

                // !!!                                   
                // Запись в файл (Vtek - производная в точке) 
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
                // Формирование альтернативного признака срабатывания
                // ------------------------------------------------------------                                                
                // Обратить внимание на неравенства ...!?
                // Для реальной ОЭС должно быть задействовано сообщение по CAN от КЭП (КВ)
                   if ( (IMIT)&&(((jkw==0)&&(arc_OS<arc_GSS))||((jkw==1)&&(arc_OS>arc_GSS))) 
                      ) mmCanKW[nTek][NumOs].mLR[jkw]=1;

                // !!!                                   
                // Пороговая схема - принятие решения о срабатывании КВ (CAN)
                // ------------------------------------------------------------                                                                
                   if (mmCanKW[nTek][NumOs].mLR[jkw]==1){
                      prExeKW=true;
                   // Остановка ОПУ      
                      skorcsn(0.0, 0.0, 0.0);
                      for (ikk=0; ikk<6; ikk++) v6_ALR[ikk]=999.0; // нейтраль
                      printf(" ------------------------------------------------------------------------\n");                      
                      printf(" Внимание! Прерывание - отсечка КВ\n");                 
                      printf(" КЭП Stop! A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);                                           
                      printf(" разв. --> Vtek=%10.6f;\n",Vtek);                                                            
                      Vtek=-0.1*Vtek;
                      printf(" разв. <-- Vtek=%10.6f;\n",Vtek);                                                            
                      printf(" ......... Выход из рабочего цикла (линейный участок)\n");                  
                      switch (code_ALR){
                          case 'A':v6_ALR[0]=mBufKW[ikw+1][1]; v6_ALR[3]=Vtek; break;
                          case 'L':v6_ALR[1]=mBufKW[ikw+1][1]; v6_ALR[4]=Vtek; break;
                          case 'R':v6_ALR[2]=mBufKW[ikw+1][1]; v6_ALR[5]=Vtek; break;                 
                      }//switch
                      break;
                   };//if    

                // !!!                                   
                // Селектор режима (формирование мгновенной скорости)
                // ------------------------------------------------------------                                                                
                   Vtek=mBufKW[ikw][2]; // на всякий случай - еще раз
                   if (!(nReg==0)) Vtek=(mBufKW[ikw+1][1]-arc_OS)/(mBufKW[ikw+1][0]-mBufKW[ikw][0]);

                // !!!                                   
                // Контрольный вывод                        
                // ------------------------------------------------------------                                
                   printf("CAN= %d; ikw= %d; arc_OS=%10.6f; Utek=%10.6f; Del=%10.6f;\n",
                          mmCanKW[nTek][NumOs].mLR[jkw],ikw,arc_OS,Utek,arc_OS-Utek);          
                          
                // !!!                                                      
                // Селектор типа оси ОПУ (формирование шестимерного упр.вектора)
                // ------------------------------------------------------------                                                                                
                   switch (code_ALR){
                       case 'A':v6_ALR[0]=mBufKW[ikw+1][1]; v6_ALR[3]=Vtek; break;
                       case 'L':v6_ALR[1]=mBufKW[ikw+1][1]; v6_ALR[4]=Vtek; break;
                       case 'R':v6_ALR[2]=mBufKW[ikw+1][1]; v6_ALR[5]=Vtek; break;                 
                   }//switch

                // !!!                                                                  
                // Собственно управление осью ОПУ
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
                   // Управление по скорости (аналитика или численное дифф.)               
//                    if (nReg==2)
                      skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);
                      usleep(dtt*1000000);            
                   };//if
                   }
}                   
             // !!!
             // Конец цикла чтения массива-буфера (условие на 1 ед.меньше)
             // ===============================================================                   
                }//ikw - Цикл чтения массива-буфера (условие на 1 ед.меньше)

             // Закрыть файл
                fclose(f_KW);                               
                printf(" ------------------------------------------------------------------------\n");           
                printf(" OK! ====> Выполнена задача формирования файла данных.\n");                  
                printf(" OK! ====> Закончено обслуживание рабочего диапазона для текущего КВ\n");           
             // Если текущий КВ не сработал (доработать для реальной ОЭС!)
                if (!prExeKW){
                   printf(" Внимание! Текущий КВ в рабочем диапазоне не обнаружен...\n");
                   printf(" ------------------------------------------------------------------------\n");                   
                };//(!prExeKW)
                
             // !!!           
             // Признак срабатывания КВ
             // ------------------------------------------------------------                                                                             
                if (prExeKW){                      
                   printf(" ------------------------------------------------------------------------\n");     
                   printf(" Внимание! Оценка по положению текущего КВ\n");
                   printf(" шк.грубо  [CAN= %d] A=%10.6f; L=%10.6f; R=%10.6f;\n",mmCanKW[nTek][NumOs].mLR[jkw],cd[0],cd[1],cd[2]);                          
                   printf(" ------------------------------------------------------------------------\n");                   
                // ScrCanKW(false,nInpKW,mmCanKW);                              
                // printf("CAN= %d\n"+,mmCanKW[nTek][NumOs].mLR[jkw]);                          
                   printf(" => Запустить процедуру коррекции? (y->да;n->нет)\n");             
                   printf(" => "); gets(sss);               
                   if ((sss[0]=='y')||(sss[0]=='Y')){         
                      printf("           Ждите...\n");      
                      skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);                      
                      ikw=0;        
                   // На всякий случай ограничение на кол-во итераций (50-кратное)
                      while (ikw<=Niteration){            
                          ikw=ikw+1;
//                          skorcsn(v6_ALR[3],v6_ALR[4],v6_ALR[5]);
                          usleep(dtt*1000000);                                            
                       // Селектор типа оси ОПУ (положение)                                 
                          switch (code_ALR){
                              case 'A': arc_OS=cd[0]; break;
                              case 'L': arc_OS=cd[1]; break;
                              case 'R': arc_OS=cd[2]; break;
                          }//switch                                                            
                          printf("         # ikw=%d; A=%10.6f; L=%10.6f; R=%10.6f;\n",ikw,cd[0],cd[1],cd[2]);                                     
                          
                       // !!!                                   
                       // Формирование альтернативного признака срабатывания
                       // ------------------------------------------------------------                                                                          
                       // Обратить внимание на неравенства (знаки) ...!?
                          if ( (IMIT)&&(((jkw==0)&&(arc_OS>arc_GSS))||((jkw==1)&&(arc_OS<arc_GSS))) 
                             ) mmCanKW[nTek][NumOs].mLR[jkw]=0;

                       // !!!                                   
                       // Пороговая схема - принятие решения о срабатывании КВ (CAN)
                       // ------------------------------------------------------------                                                                
                          if (mmCanKW[nTek][NumOs].mLR[jkw]==0){
                             skorcsn(0.0,0.0,0.0);                 
                             printf(" ------------------------------------------------------------------------\n");                        
                             printf(" Внимание! Оценка по положению текущего КВ + коррекция\n");
                             printf(" шк.точно  [CAN= %d] A=%10.6f; L=%10.6f; R=%10.6f;\n",mmCanKW[nTek][NumOs].mLR[jkw],cd[0],cd[1],cd[2]);                                        
                             printf(" ------------------------------------------------------------------------\n");                                     
                          // ScrCanKW(false,nInpKW,mmCanKW);                                                           
                          // printf("CAN= %d\n",mmCanKW[nTek][NumOs].mLR[jkw]);
                             break;
                          };//if                            
                      };//while  
                   // Остановка оси ОПУ
                      skorcsn(0.0,0.0,0.0);                
                   // Превышение кол-ва итераций при коррекцции                         
                      if (ikw==Niteration) return 7;                   
                   }else{
                   // printf(" ------------------------------------------------------------------------\n");                                      
                      printf(" => Коррекция не проводилась\n");                                                   
                      printf(" ------------------------------------------------------------------------\n");                        
                   };//if        
                
                // !!!           
                // Запись данные в структуру-буфер?
                // ------------------------------------------------------------                                                                                               
                   printf(" => Записать данные в структуру-буфер? (y->да;n->нет)");
                   printf(" => "); gets(sss); 
                   if ((sss[0]=='y')||(sss[0]=='Y')){         
                   // Доработать!!!               
                   // Селектор типа оси ОПУ
                      ikw=-1;
                      switch (code_ALR){
                          case 'A': ikw=0; arc_OS=cd[0]; break;
                          case 'L': ikw=1; arc_OS=cd[1]; break;
                          case 'R': ikw=2; arc_OS=cd[2]; break;     	            
                      }//switch  
                   // Признак обновления данных
                      prDataBuf[nTek][ikw].mLR[jkw]=1;
                   // Формирование буфера данных               
                      if (jkw==0){
                         mDataBuf[nTek][ikw].stDirL.val=arc_OS;
                      // Пока берем старое значение     
                         mDataBuf[nTek][ikw].stDirL.cko=mDataKW[nTek][ikw].stDirL.cko;
                      }else{
                         mDataBuf[nTek][ikw].stDirR.val=arc_OS;
                      // Пока берем старое значение     
                         mDataBuf[nTek][ikw].stDirR.cko=mDataKW[nTek][ikw].stDirL.cko;                 
                      };//if               
                          
                      printf(" ------------------------------------------------------------------------\n");                        
                      printf(" Внимание! Запись данных в структуру-буфер выполнена\n");
                      printf(" ------------------------------------------------------------------------\n");
                   };//if  
                   prExeKW = false;// для блокировки в последующем цикле!              
                   
                };//if(prExeKW) - Имитация срабатывания КВ

         // ######################################################################           
            };//jkw - цикл по левому и правому КВ
         // ######################################################################                     
     
         // Реакция на ошибку ввода данных (выбор режима) в предыдущем цикле по <jkw>
            if (!my_ERR) continue;
 mEND:;

         // Отображение структуры-буфера данных для КВ на экран
            printf(" ------------------------------------------------------------------------\n");              
            printf(" Отображение текущей структуры-буфера данных для КВ на экран ? (y->да;n->нет)\n");
            printf(" => "); gets(sss); 
            if ((sss[0]=='y')||(sss[0]=='Y')) 
               ScrDataKW(nInpKW,mDataBuf);            
               
         // Выполнить подготовку (обновление) структуры к выходу
            printf(" ------------------------------------------------------------------------\n");              
            printf(" Выполнить подготовку (обновление) структуры-буфера ? (y->да;n->нет)\n");
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
               printf(" OK! ====> Выполнена задача обновление структуры-буфера\n");
            };//if - Выполнить подготовку (обновление) структуры к выходу
            
         // Отображение  обновленной структуры-буфера данных для КВ на экран
            printf(" ------------------------------------------------------------------------\n");              
            printf(" Отображение структуры-буфера выходных данных для КВ на экран ? (y->да;n->нет)\n");
            printf(" => "); gets(sss); 
            if ((sss[0]=='y')||(sss[0]=='Y'))          
               ScrDataKW(nInpKW,mDataBuf);
                     
         // Формирование файла БД - <DataBF.txt>    
            FileDataBF(nInpKW,mDataBuf);

         // Вывод значений для рабочего диапазона всех осей ОПУ
            printf(" ------------------------------------------------------------------------\n");              
            printf(" Вывод диапазона для осей ОПУ на экран ? (y->да;n->нет)\n");
            printf(" => "); gets(sss); 
            if ((sss[0]=='y')||(sss[0]=='Y'))                   
               OutDelWork(nInpKW,mDataBuf,mConfKW);

         // Установка ОПУ для выбранной оси в положение по умолчанию
            printf(" КЭП >>>>> Установка оси ОПУ в положение по умолчанию - ");                        
            for (ikw=0; ikw<6; ikw++) v6_ALR[ikw]=999.0;// нейтраль
            switch (code_ALR){
                case 'A': NumOs=0;v6_ALR[0]=kw_A0; printf("%+006.1f",v6_ALR[0]); break;
                case 'L': NumOs=1;v6_ALR[1]=kw_L0; printf("%+006.1f",v6_ALR[1]); break;
                case 'R': NumOs=2;v6_ALR[2]=kw_R0; printf("%+006.1f",v6_ALR[2]); break;                 
            }//switch                               
         // Собственно установка ОПУ
            printf(" [угл.град]\n");     
            printf("           режим КЭП - <по положению>\n");        
            printf("           Ждите... \n");      
            perebroscsn(v6_ALR[0],v6_ALR[1],v6_ALR[2]);
            RESCSN();
            printf(" OK! ====> A=%10.6f; L=%10.6f; R=%10.6f;\n",cd[0],cd[1],cd[2]);
            printf(" ------------------------------------------------------------------------\n");
        
         // printf(" ------------------------------------------------------------------------\n");              
            printf(" Выход из цикла опроса автономных проверок? (y->да;n->нет)\n");
            printf(" => "); gets(sss); 
            if ((sss[0]=='y')||(sss[0]=='Y')) break; else continue;                
        
       };//Просто безусловный цикл опроса - после выбора режима автономных проверок 
    // Успешный выход из процедуры
      return 0;
   }//void ModKW() - Модуль инициализации режима автономных проверок
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 

// Интерполяция для таблицы
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

// Идентификация и чтение файлов БД - DataXI2.txt
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
        printf(" Внимание! Отсутствует файл БД (таблица коэффициентов XI^2) <DataXI2.txt>\n");
        printf(" ------------------------------------------------------------------------\n");          
        return 1;
      };//(!f_Data)
   // printf(" ------------------------------------------------------------------------\n");        
      printf(" OK! ====> Обнаружен файл БД (таблица коэффициентов XI^2) => <DataXI2.txt>\n");
      printf(" ......... Чтение файла и формирование буферного массива данных\n");        
   // printf(" ------------------------------------------------------------------------\n");  
        
   // Чтение файла и формирование структуры       
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
     // Буфер чтения
        iz=iz+1;        
        pkw00=pkw0;
        pkw01=pkw1;
        pkw02=pkw2;
        pkw03=pkw3;
        pkw04=pkw4;           
      };//while                
      fclose(f_Data); 
   // -----------------------------------------------------------        
   // Формируем файл данных с учетом интерполяционных значений
      printf(" ......... Формирование таблицы коэффициентов <XI2> с учетом интерполяции\n");             
      f_Data=fopen("dan/mDataXI2.txt","w");     
      kz=0;
      for (jz=1; jz<=26; jz++){
        for (iz=1; iz<=20; iz++){          
            // Запись в файл            
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
      printf(" OK! ====> Выполнена задача формирования таблицы коэффициентов <XI2>\n");
      printf(" ------------------------------------------------------------------------\n");              
   // printf("\n");
      return 0;
   };//ReadXI2()

// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ 
// Модуль инициализации режима автономных проверок
// Формирование файла данных с коэффициентами XI2
   int ModXI2(){
    // =======================================================================
    // Начало описания дополнительных переменных
    // =======================================================================     
    // Признак первого прохода   
       bool prCP=false;      
    // Обобщенный признак наличия ошибки   
       bool my_ERR=false;   
    // Переменные цикла
       int iz,jz,kz,N;   
    // Массив коэффициентов <k>   
       double mDataXI2[501][4];
       
    // Идентификация,чтение и формирование файлов БД - DataXI2.txt
       printf(" ------------------------------------------------------------------------\n");      
       printf(" Внимание! Поиск и чтение файлов БД - <DataXI2.txt>\n");
    // printf(" ------------------------------------------------------------------------\n");                   
    // my_ERR=false;
       if (ReadXI2("DataXI2.txt",mDataXI2,&N))
       // printf(" ------------------------------------------------------------------------\n");      
       // printf(" OK! ====> Выполнена задача формирования таблицы коэффициентов <XI2>\n");
       // printf(" ------------------------------------------------------------------------\n");              
       // printf("\n");
       return 1;

       return 0;
   }//void ModXI2() - Формирование файла данных с коэффициентами XI2
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\   


// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ 
// Имитатор измерений параметров кат.звезд
   void ImSTAR(int Nmax,double ZVmin,     double ZVmax,
                        double UG1min,    double UG1max,    
                        double UG2min,    double UG2max,                       
                        double CkoZVkat,  double CkoZVvar,   
                        double CkoUG1kat, double CkoUG1var,
                        double CkoUG2kat, double CkoUG2var, int prPrint){
//                        struct stZV mDataStar[],
//                        int    prPrint){
    // struct stZV mDataStar[501];                        
    // Массив параметров звезд   
       struct stZV mDataStar[501];    
    // Если задано Nz<20, то всегда принимается по умолчанию Nz=20
       if (Nmax<=20) Nmax=21;
    // Переменная ввода   
       char sss[256];           
    // Метка файла данных
       FILE *f_Data;   
    // Коэффициенты пересчета
       double Pi,Pi2,gr,mr,sr;
       Pi = 3.1415926;
       Pi2= 2.0*Pi;
       gr = Pi/180.0;
       mr = gr/60.0;
       sr = mr/60.0;
    // Переменные цикла
       int iz,jz,kz,Nz,js;   
    // Параметры чтения файла данных   
       int   pkw0;
       float pkw1,pkw2,pkw3,pkw4,pkw5,pkw6;           
    // Количество генерируемых опорных звезд для зв.каталога  
       int Nkat;
    // Все зв.величины   
       double ZVkat;// каталог
       double ZVvar;// измерения
    // Все углы - текущие
       double UgKat1,UgKat2;// каталог   - прямое восхождение и склонение
       double UgVar1,UgVar2;// измерения - прямое восхождение и склонение    
       double ZV0,dZV;          
       dZV=(ZVmax-ZVmin)/2.0;
       ZV0=ZVmin+dZV;
       double dUG;       
              
    // Формирование массива опорных (каталожных) звезд
    // printf("\n");                
    // Количество каталожных звезд
       printf(" ------------------------------------------------------------------------\n");      
       printf(" Имитация! Генератор кол. кат. звезд в зоне ответственности (Nmax=%d)\n",Nmax);
       Nkat=0;       
       while (Nkat<20){ 
          Nkat=rand()%Nmax;
       // printf(" ========> %d\n",Nkat);          
       };//while
       printf(" OK! ====> Выполнена задача формирования кол. кат. звезд в зоне отв-ти\n");
       printf("           Nkat= %d\n",Nkat);       
    // printf(" ------------------------------------------------------------------------\n");              
       
    // Остановка для просмотра
       if (prPrint){ printf(" => "); gets(sss);}

    // Формируем зв.величину кат.звезды          
    // printf(" ------------------------------------------------------------------------\n");      
       printf(" Имитация! Генератор параметров для звезд в зоне ответственности\n");
       printf("           ждите...\n");       
       if (prPrint) printf("           ------------------------------------------------------------\n");                     
       for (jz=1; jz<=Nkat; jz++){       
       // Каталожное значение блеска 
          ZVkat=my_random1(ZV0,dZV);          
          mDataStar[jz-1].mZV[0][0]=ZVkat;       
       // Каталожные значения 1-го и 2-го углов 
          dUG=(UG1max-UG1min)/2.0;
          UgKat1=my_random1(UG1min+dUG,dUG);          
          mDataStar[jz-1].mZV[0][1]=UgKat1;                     
       // ----------------------------------
          dUG=(UG2max-UG2min)/2.0;          
          UgKat2=my_random1(UG2min+dUG,dUG);      
          mDataStar[jz-1].mZV[0][2]=UgKat2;                         
       // Измеренное значение блеска       
          ZVvar=my_random1(ZVkat,CkoZVvar);
          mDataStar[jz-1].mZV[1][0]=ZVvar;       
       // Измеренные значения 1-го и 2-го углов 
          UgVar1=my_random1(UgKat1,CkoUG1var);        
          mDataStar[jz-1].mZV[1][1]=UgVar1;                       
       // ----------------------------------          
          UgVar2=my_random1(UgKat2,CkoUG2var);      
          mDataStar[jz-1].mZV[1][2]=UgVar2;               
          if (prPrint){
             printf("        j=%03d; кат: бл.= %+010.6f;  Ug1= %+011.6f;  Ug2= %+011.6f;\n",jz, mDataStar[jz-1].mZV[0][0], mDataStar[jz-1].mZV[0][1]/gr, mDataStar[jz-1].mZV[0][2]/gr);
              printf("               изм: бл.= %+010.6f;  Ug1= %+011.6f;  Ug2= %+011.6f;\n",    mDataStar[jz-1].mZV[1][0], mDataStar[jz-1].mZV[1][1]/gr, mDataStar[jz-1].mZV[1][2]/gr);          
             printf("             ------------------------------------------------------------\n");                 
          };//if          
       };//jz               
       printf(" OK! ====> Выполнена задача формирования параметров звезд в зоне отв-ти  \n");
    // printf(" ------------------------------------------------------------------------\n");                         
    // Остановка для просмотра
    // if (prPrint){ printf(" => "); gets(sss);}

    // Запись в файл                   
       printf(" ДатаФайл! Формирование файла данных с параметрами звезд\n");
       printf("           ждите...\n");                  
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
       printf(" OK! ====> Выполнена задача формирования файла данных\n");
       printf("           Углы заданы в радианах!\n");       
       printf(" ------------------------------------------------------------------------\n");
       printf(" Выполнена задача формирования локального зв.каталога\n");                           
       printf(" ------------------------------------------------------------------------\n");       
   };// Имитатор измерений параметров кат.звезд

// Модуль инициализации режима автономных проверок

// Имитатор измерений параметров кат.звезд
   int ModKAT(){
    // =======================================================================
    // Начало описания дополнительных переменных
    // =======================================================================     
       char sss[256];           
    // Коэффициенты пересчета
       double Pi,Pi2,gr,mr,sr;
       Pi =3.1415926;
       Pi2=2.0*Pi;
       gr =Pi/180.0;
       mr =gr/60.0;
       sr =mr/60.0;
    // Переменные выбора режима интерфейса (вывод данных на экран)
       int  nScr=-1;
       char sScr[256];                    
    // Переменные выбора режима фильтрации
       int  nReg=-1;
       char sReg[256];             
    // Признак первого прохода   
       bool prCP =false;      
    // Признак аномального измерения 
       bool prXI2=false;
    // Признаки сравнения оценки СКО с каталожным значением (если "<", то "true")
       bool mPrZV =false;   
       bool mPrUG1=false;          
       bool mPrUG2=false;                 
    // Обобщенный признак наличия ошибки   
       bool my_ERR=false;   
    // Переменные цикла
       int iz,jz,kz,Nz,js;   
    // Параметры чтения файла данных   
       int   pkw0;
       float pkw1,pkw2,pkw3,pkw4,pkw5,pkw6;       
    // Массив коэффициентов <k> для XI^2
       double mDataXI2[501][4];       
    // Массив параметров звезд   
       struct stZV  mDataStar[501];// изначальный входной массив (лок.каталог)
       struct stBUF mBufStar[501]; // модификация с учетом чистки аном.измерений  
    // Вектор-массив линейных форм
       double mMatStar[3]; // M[Xj-Xсрj];
    // Вектор-массив квадратичных форм
       double mDispStar[3];// CKOизм*CKOизм=M{(Xj-Xсрj-M[Xj-Xсрj])*(Xj-Xсрj-M[Xj-Xсрj])};
    // Вектор-массив СКО форм
       double mCkoStar[3]; // CKO^изм*CKO^изм=CKOизм*CKOизм-CKOкат*CKOкат;
    // Вспомогательные параметры для операции фильтрации   
       double DeltaX,DeltaX1,DeltaX2,DeltaX3,DeltaX4;       
    // Количество генерируемых опорных звезд для зв.каталога (имитатор) 
       int Nmax=500;// но не менее 20 штук!!!           
       int Nkat;   // изначальное значение для каталога
       int Nend;   // текущее значение (по циклам фильтрации аном.изм)
    // Все углы - каталог,текущие
       double UgKat1,UgKat2;// каталог
       double UgVar1,UgVar2;// измерения
    // Все зв.величины - каталог,текущие
       double ZVkat,ZVvar;       
    // Диапазон зв.величин
       double ZVmin= 7.0;//зв.вел   
       double ZVmax=16.0;//зв.вел          
       double ZV0,dZV,dUG1,dUG2;          
              dZV=(ZVmax-ZVmin)/2.0;// полуширина рабочего диапазона
              ZV0=ZVmin+dZV;        // среднее значение для рабочего диапазона
    // Диапазон угл.величин
       double UG1min=   0.0*gr;// [рад] // [угл.град]  
       double UG1max= 360.0*gr;// [рад] // [угл.град]
    // ---------------------
       double UG2min=   0.0*gr;// [рад] // [угл.град]   
       double UG2max=  90.0*gr;// [рад] // [угл.град]                        
    // ---------------------       
       double dUG;
       
    // Точность пороговая по    
       double CkoZVpor= 0.5;//зв.вел    
    // Точность каталога по зв.величине (порог,каталожное значения)       
       double CkoZVkat= 0.1;//зв.вел
    // Единичная точность измерений зв.величины кат.звезды с помощью ОЭС
       double CkoZVvar= 1.0;//зв.вел
    
    // Точность пороговая по углам
       double CkoUGpor;
       CkoUGpor=1.0*sr;           // [рад] - (задано в угловых секундах)    
    // Точность каталога по углам (min,max)       
       double CkoUG1kat=0.06*sr;  // [рад] - (задано в угловых секундах)
       double CkoUG2kat=0.06*sr;  // [рад] - (задано в угловых секундах)       
    // Единичная точность измерений угла кат.звезды с помощью ОЭС
       double CkoUG1var=0.5*sr;   // [рад] - (задано в угловых секундах)       
       double CkoUG2var=0.5*sr;   // [рад] - (задано в угловых секундах)       
    // Переменные <min> и <max> для значения блеска (звезвная величина)       
       double minZV= 999.0;
       double maxZV=-999.0;       
    // --------------------   
       double minZVend= 999.0;
       double maxZVend=-999.0;       

    // Выбрать режим фильтрации входными данными
       printf("\n");
       while (true){    
          nScr=-1;      
          printf(" Признак отображения текущей информации алгоритма на экран:\n");
          printf(" ------------------------------------------------\n");                
          printf("  * 0 - без детализации.\n");
          printf("  * 1 - с детализацией..\n");
          printf(" ------------------------------------------------\n");                                
          printf("  ? ввод (мнемо режима => "); 
          gets(sScr);       
          sscanf(sScr,"%d",&nScr);              
       // ====================================================================
       // Контроль формата ввода переменной "nReg"
          my_ERR=false;
          for (iz=0; iz<=1; iz++) if (nScr==iz){ my_ERR=true;break; };          
          if (!my_ERR){
             PrMsg("Ошибка ввода данных, недопустимый формат команды");                   
             continue;
          };//if
          break;
       };//while       

    // Формирование массива опорных (каталожных) звезд
    // ===============================================
    // Имитатор измерений параметров кат.звезд    
       ImSTAR(Nmax,ZVmin,ZVmax, UG1min,UG1max, UG2min, UG2max,                       
              CkoZVkat,CkoZVvar, CkoUG1kat,CkoUG1var, CkoUG2kat,CkoUG2var,
              nScr);                                  
    // ===============================================       
       return 0;
   };// int ModKAT(){   

// Имитатор измерений параметров кат.звезд
   int ModSTAR(){
    // =======================================================================
    // Начало описания дополнительных переменных
    // =======================================================================     
       char sss[256];           
    // Коэффициенты пересчета
       double Pi,Pi2,gr,mr,sr;
       Pi =3.1415926;
       Pi2=2.0*Pi;
       gr =Pi/180.0;
       mr =gr/60.0;
       sr =mr/60.0;
    // Переменные выбора режима интерфейса (вывод данных на экран)
       int  nScr=-1;
       char sScr[256];                    
    // Переменные выбора режима фильтрации
       int  nReg=-1;
       char sReg[256];             
    // Признак первого прохода   
       bool prCP =false;      
    // Признак аномального измерения 
       bool prXI2=false;
    // Признаки сравнения оценки СКО с каталожным значением (если "<", то "true")
       bool mPrZV =false;   
       bool mPrUG1=false;          
       bool mPrUG2=false;                 
    // Обобщенный признак наличия ошибки   
       bool my_ERR=false;   
    // Переменные цикла
       int iz,jz,kz,Nz,js;   
    // Параметры чтения файла данных   
       int   pkw0;
       float pkw1,pkw2,pkw3,pkw4,pkw5,pkw6;       
    // Массив коэффициентов <k> для XI^2
       double mDataXI2[501][4];       
    // Массив параметров звезд   
       struct stZV  mDataStar[501];// изначальный входной массив (лок.каталог)
       struct stBUF mBufStar[501]; // модификация с учетом чистки аном.измерений  
    // Вектор-массив линейных форм
       double mMatStar[3]; // M[Xj-Xсрj];
    // Вектор-массив квадратичных форм
       double mDispStar[3];// CKOизм*CKOизм=M{(Xj-Xсрj-M[Xj-Xсрj])*(Xj-Xсрj-M[Xj-Xсрj])};
    // Вектор-массив СКО форм
       double mCkoStar[3]; // CKO^изм*CKO^изм=CKOизм*CKOизм-CKOкат*CKOкат;
    // Вспомогательные параметры для операции фильтрации   
       double DeltaX,DeltaX1,DeltaX2,DeltaX3,DeltaX4;       
    // Количество генерируемых опорных звезд для зв.каталога (имитатор) 
       int Nmax=50;// но не менее 20 штук!!!           
       int Nkat;   // изначальное значение для каталога
       int Nend;   // текущее значение (по циклам фильтрации аном.изм)
    // Все углы - каталог,текущие
       double UgKat1,UgKat2;// каталог   (прямое восхождение и склонение)
       double UgVar1,UgVar2;// измерения (прямое восхождение и склонение)
    // Все зв.величины - каталог,текущие
       double ZVkat,ZVvar;       
    // Диапазон зв.величин
       double ZVmin= 7.0;//зв.вел   
       double ZVmax=16.0;//зв.вел          
       double ZV0,dZV,dUG1,dUG2;          
              dZV=(ZVmax-ZVmin)/2.0;// полуширина рабочего диапазона
              ZV0=ZVmin+dZV;        // среднее значение для рабочего диапазона
    // Диапазон угл.величин
       double UG1min=   0.0*gr;// [рад] // [угл.град] прямое восхождение
       double UG1max= 360.0*gr;// [рад] // [угл.град] прямое восхождение
    // ---------------------
       double UG2min= -90.0*gr;// [рад] // [угл.град] склонение
       double UG2max=  90.0*gr;// [рад] // [угл.град] склонение                       
    // ---------------------       
       double dUG;
       
    // Точность пороговая по    
       double CkoZVpor= 0.5;//зв.вел    
    // Точность каталога по зв.величине (порог,каталожное значения)       
       double CkoZVkat= 0.1;//зв.вел
    // Единичная точность измерений зв.величины кат.звезды с помощью ОЭС
       double CkoZVvar= 0.5;//зв.вел
    
    // Точность пороговая по углам
       double CkoUGpor;
       CkoUGpor=1.0*sr;         // [рад] - (задано в угловых секундах)    
    // Точность каталога по углам (min,max)       
       double CkoUG1kat=0.06*sr;  // [рад] - (задано в угловых секундах)
       double CkoUG2kat=0.06*sr;  // [рад] - (задано в угловых секундах)       
    // Единичная точность измерений угла кат.звезды с помощью ОЭС
       double CkoUG1var=100.0*sr;// [рад] - (задано в угловых секундах)       
       double CkoUG2var=100.0*sr;// [рад] - (задано в угловых секундах)       
    // Переменные <min> и <max> для значения блеска (звезвная величина)       
       double minZV= 999.0;
       double maxZV=-999.0;       
    // --------------------   
       double minZVend= 999.0;
       double maxZVend=-999.0;       
       
    // printf("\n");       
    // printf("           СКОттх(зв.вел )= %12.8f\n",CkoZVpor);                                           
    // printf("           СКОттх(угл.изм)= %12.8f\n",CkoUGpor);                                           
    // Выбрать режим фильтрации входными данными
       printf("\n");
       while (true){    
          nScr=-1;      
          printf(" Признак отображения текущей информации алгоритма на экран:\n");
          printf(" ------------------------------------------------\n");                
          printf("  * 0 - без детализации.\n");
          printf("  * 1 - с детализацией..\n");
          printf("  * 2 - только результат\n");          
          printf(" ------------------------------------------------\n");                                
          printf("  ? ввод (мнемо режима => "); 
          gets(sScr);       
          sscanf(sScr,"%d",&nScr);              
       // ====================================================================
       // Контроль формата ввода переменной "nReg"
          my_ERR=false;
          for (iz=0; iz<=2; iz++) if (nScr==iz){ my_ERR=true;break; };          
          if (!my_ERR){
             PrMsg("Ошибка ввода данных, недопустимый формат команды");                   
             continue;
          };//if
          break;
       };//while       
       
    // Чтение таблицы коэффициентов XI2
    // ===============================================
       FILE *f_Data;
       f_Data=fopen("dan/mDataXI2.txt","r");          
       printf(" ------------------------------------------------------------------------\n");              
       if (!f_Data){
          printf(" Внимание! Отсутствует файл БД (таблица коэффициентов XI^2) <dan/mDataXI2.txt>\n");
          printf(" ------------------------------------------------------------------------\n");          
          return 1;
       };//if
       printf(" OK! ====> Обнаружен файл БД (таблица коэффициентов XI^2) => <dan/mDataXI2.txt>\n");
       printf(" ......... Чтение файла и формирование буферного массива данных\n");        
    // Остановка для просмотра
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
    // Контроль чтения данных
       if (iz==pkw0){
          printf(" OK! ====> Выполнено чтение файла и формирование буфера\n");
          printf(" ------------------------------------------------------------------------\n");         
          printf(" Выполнена задача формирования таблицы коэффициентов <XI2>\n");                    
       } else {
          printf(" Внимание! Ошибка чтения файла и формирование буферного массива данных\n");       
          printf(" ------------------------------------------------------------------------\n");                
          return 1;
       };//if (iz==pkw0)       

    // Формирование массива опорных (каталожных) звезд
    // ===============================================
    // Имитатор измерений параметров кат.звезд    
    // ImSTAR(Nmax,ZVmin,ZVmax, UG1min,UG1max, UG2min, UG2max,                       
    //        CkoZVkat,CkoZVvar, CkoUG1kat,CkoUG1var, CkoUG2kat,CkoUG2var,
    //        1);                                  
    // ===============================================    
    
    // Чтение файла с локальным исходным каталогом
       f_Data=fopen("DataStar.txt","r");          
       printf(" ------------------------------------------------------------------------\n");              
       if (!f_Data){
          printf(" Внимание! Отсутствует файл БД (файл с локальным каталогом) <DataStar.txt>\n");
          printf(" ------------------------------------------------------------------------\n");          
          return 1;
       };//if
       printf(" OK! ====> Обнаружен файл БД (файл с локальным каталогом) <DataStar.txt>\n");
       printf(" ......... Чтение файла и формирование буферного массива данных\n");        
    // Остановка для просмотра       
    // printf(" => "); gets(sss);               
    // Переменные <min> и <max> для значения блеска (звезвная величина)
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
       // Поиск <min> и <max> для блеска
          if (minZV>pkw4) minZV=pkw4;
          if (maxZV<pkw4) maxZV=pkw4;
       // -----------------------------       
          if (nScr==1){          
             printf("        j=%03d; кат: бл.= %+010.6f;  Ug1= %+011.6f;  Ug2= %+011.6f;\n",iz, mDataStar[iz].mZV[0][0], mDataStar[iz].mZV[0][1]/gr, mDataStar[iz].mZV[0][2]/gr);
              printf("               изм: бл.= %+010.6f;  Ug1= %+011.6f;  Ug2= %+011.6f;\n",    mDataStar[iz].mZV[1][0], mDataStar[iz].mZV[1][1]/gr, mDataStar[iz].mZV[1][2]/gr);          
             printf("             ------------------------------------------------------------\n");                 
         };//if          
         iz=iz+1;                
       // Ограничение на количество строк-данных (Nstr<=501)     
         if (iz==500) break;
       };//while
       fclose(f_Data);        
    // Сколько звезд по умолчанию на начало процедуры фильтрации
       Nkat=iz;
       Nend=iz;
       printf(" >>>>>>>>> Nend=%d - на начало фильтрации!\n",Nend);                  

    // Выбрать режим фильтрации входными данными
    // ====================================================================    
       printf("\n");
       while (true){    
          nReg=-1;      
          printf(" Инициализация режима фильтрации исходных данных:\n");
          printf(" ------------------------------------------------\n");                
          printf("  * 0 - оценка точности зв.величины.....\n");
          printf("  * 1 - оценка точности угловых значений\n");
          printf(" ------------------------------------------------\n");                                
          printf("  ? ввод (мнемо режима => "); 
          gets(sReg);       
          sscanf(sReg,"%d",&nReg);              
       // Контроль формата ввода переменной "nReg"
          my_ERR=false;
          for (iz=0; iz<=1; iz++) if (nReg==iz){ my_ERR=true;break; };          
          if (!my_ERR){
          // PrMsg("Ошибка ввода данных, недопустимый формат команды");                   
             printf(" ------------------------------------------------\n");                          
             printf(" Внимание! Ошибка ввода данных, недопустимый формат команды\n");                   
             printf(" ------------------------------------------------\n");                             
             continue;
          };//if
          break;
       };//while       

    // Обработка рабочего массива данных по распознанным звездам
    // ====================================================================                            
       js=0;   // счетчик циклов (итераций) фильтрации аномальных измерений
mmAnom:  
       js=js+1;// счетчик циклов (итераций) фильтрации аномальных измерений       
       printf(" ------------------------------------------------------------------------\n");      
       printf(" Операция! Обработка рабочего массива данных по распознанным звездам\n");
       printf(" >>>>>>>>> [i++] = %d\n",js);       
    // printf(" ------------------------------------------------------------------------\n");                               
    // Обнуление сумматоров - для линейных форм
       for (jz=0; jz<3; jz++) mMatStar[jz]=0.0;        

    // Обнуление сумматоров    
       minZVend= 999.0; maxZVend=-999.0;                         
    // Подсчет линейных форм       
       for (jz=0; jz<Nend; jz++){ 
       // -------------------------------                 
          dZV = mDataStar[jz].mZV[0][0] - mDataStar[jz].mZV[1][0];       
          mMatStar[0] = mMatStar[0]+dZV;
       // -------------------------------     
       // !!! Принципиально, вопрос к А.Ф.- [*cos(?)]            
          dZV = mDataStar[jz].mZV[0][1] - mDataStar[jz].mZV[1][1];
          mMatStar[1] = mMatStar[1]+dZV*cos(mDataStar[jz].mZV[1][2]);
       // -------------------------------                    
          dZV = mDataStar[jz].mZV[0][2] - mDataStar[jz].mZV[1][2];
          mMatStar[2] = mMatStar[2]+dZV;                    
       // -------------------------------          
       // Поиск <min> и <max> для блеска
          if (minZVend>mDataStar[jz].mZV[1][0]) minZVend=mDataStar[jz].mZV[1][0];
          if (maxZVend<mDataStar[jz].mZV[1][0]) maxZVend=mDataStar[jz].mZV[1][0];          
       };//jz       
    // Собственно выборочное среднее
       mMatStar[0]= mMatStar[0]/Nend;
       mMatStar[1]= mMatStar[1]/Nend;
       mMatStar[2]= mMatStar[2]/Nend;
if (!(nScr==2)){                                                  
       printf(" ------------------------------------------------------------------------\n");      
       printf(" Оператор! Выполнена задача формирования линейных форм\n");
       printf("           M[]:m=%+08.4f[з/в]; ug1=%+09.4f[угл.сек]; Ug2=%+09.4f[угл.сек];\n",mMatStar[0],mMatStar[1]/sr,mMatStar[2]/sr);                        
    // printf(" ------------------------------------------------------------------------\n");               
};//if              
    // Обнуление сумматоров - для квадратичных форм    
       for (jz=0; jz<3; jz++) mDispStar[jz]=0.0;       
    // Подсчет квадратичных форм
       for (jz=0; jz<Nend; jz++){        
          dZV = mDataStar[jz].mZV[0][0] - mDataStar[jz].mZV[1][0] - mMatStar[0];
          mDispStar[0] = mDispStar[0]+dZV*dZV;
       // -------------------------------                    
///!!! Принципиально, вопрос к А.Ф.- [*cos(...)] =>???                 
          dZV = (mDataStar[jz].mZV[0][1] - mDataStar[jz].mZV[1][1])*cos(mDataStar[jz].mZV[1][2]) - mMatStar[1];
//        dZV = (mDataStar[jz].mZV[0][1] - mDataStar[jz].mZV[1][1])*1.0 - mMatStar[1];
          mDispStar[1] = mDispStar[1]+dZV*dZV;       
       // -------------------------------                    
          dZV = mDataStar[jz].mZV[0][2] - mDataStar[jz].mZV[1][2] - mMatStar[2];
          mDispStar[2] = mDispStar[2]+dZV*dZV;          
       };//jz                      
    // Собственно дисперсия
       mDispStar[0] = mDispStar[0]/(Nend-1); 
       mDispStar[1] = mDispStar[1]/(Nend-1); 
       mDispStar[2] = mDispStar[2]/(Nend-1);         
if (!(nScr==2)){                                           
    // printf(" ------------------------------------------------------------------------\n");      
       printf(" Оператор! Выполнена задача формирования квадратичных форм\n");
       printf("           CKO:m=%+08.4f[з/в]; ug1=%+09.4f[угл.сек]; Ug2=%+09.4f[угл.сек];\n",sqrt(mDispStar[0]),sqrt(mDispStar[1])/sr,sqrt(mDispStar[2])/sr);              
       printf("           DIS:m=%+08.4f;      ug1=%+09.4f;          Ug2=%+09.4f          \n",mDispStar[0],mDispStar[1]/sr/sr,mDispStar[2]/sr/sr);                               
       printf(" ------------------------------------------------------------------------\n");               
};//if       
    // Оценка точности измерений ОЭС (зв.вел и угловое положение)       
    // ------------------------------------------------------    
       mCkoStar[0] = mDispStar[0] - CkoZVkat*CkoZVkat;
//     dZV = cos(mDataStar[jz].mZV[0][2])*cos(mDataStar[jz].mZV[0][2])
       mCkoStar[1] = mDispStar[1] - CkoUG1kat*CkoUG1kat;//*dZV;            
       mCkoStar[2] = mDispStar[2] - CkoUG2kat*CkoUG2kat;                    
    // Принудительный <0>    
       if (mCkoStar[0]<0) {mCkoStar[0]=mDispStar[0];mPrZV =true;};
       if (mCkoStar[1]<0) {mCkoStar[1]=mDispStar[1];mPrUG1=true;};
       if (mCkoStar[2]<0) {mCkoStar[2]=mDispStar[2];mPrUG2=true;};     
if (!(nScr==2)){                                    
    // printf(" ------------------------------------------------------------------------\n");      
       printf(" OK! ====> Выполнена задача оценки точности единичных измерений\n");
       printf("           CKO:m=%+08.4f[з/в]; ug1=%+09.4f[угл.сек]; Ug2=%+09.4f[угл.сек];\n",sqrt(mCkoStar[0]),sqrt(mCkoStar[1])/sr,sqrt(mCkoStar[2])/sr);                        
       printf("           DIS:m=%+08.4f;      ug1=%+09.4f;          Ug2=%+09.4f          \n",mCkoStar[0],mCkoStar[1]/sr/sr,mCkoStar[2]/sr/sr);                               
    // printf(" ------------------------------------------------------------------------\n");                   
};//if    
    // Селектор прерывания
       switch (nReg){
             case 0:                    
                if (mPrZV) goto mmEND;        
                break;
             case 1:                    
                if (mPrUG1&&mPrUG2) goto mmEND;
                break;                
       };//switch       
    // Остановка для просмотра
       if (!(nScr==2)){
          printf(" => "); gets(sss);
       };//if
    // Контроль данных - аномальные измерения зв.величины
       printf(" ------------------------------------------------------------------------\n");      
       printf(" Выполняется задача поиска аномальных измерений...\n");
       printf(" ------------------------------------------------------------------------\n");             
       kz=0;//число аномальных измерений 
       iz=0;//число оставшихся измерений (индекс модифицированного массива)       
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
             kz=kz+1;// счетчик аномальных измерений
          // Коррекция признаков для аномальных значений    
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
                printf(" >>> [ан.изм]\n");
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
       
    // Коррекция с учетом аномальных измерений
       if (nScr==1) printf("           --------------------------------------------------------------\n");                          
       printf("           Nkat= %d; Nend= %d; Nanom= %d; Nanom(tek)= %d\n",Nkat,iz,Nkat-iz,kz);                           
       Nend=iz;//Nend-kz;   
    // Остановка для просмотра
       if (!(nScr==2)){
          printf(" => "); gets(sss);                  
       };//if      
    // Контроль за размерностью входного массива              
       if (Nend<20){
          printf(" ------------------------------------------------------------------------\n");                                           
          printf(" Внимание! Размерность входного массива < 20...\n");
          printf(" ------------------------------------------------------------------------\n");          
          return 2;       
       };//if
                                
       if (kz>0){
          printf(" Внимание! Обнаружены [%d] аномальных значений, сформирован массив-буфер\n",kz);          
          printf("           Nend(iz)= %d\n",Nend);                    
          printf(" Операция! Выполняется коррекция входного массива данных\n");                              
          printf("           ждите...\n");                                        
       // Обмен данными          
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
               printf(" Обмен:jz=%03d;кат: з/в=%+010.6f; Ug1=%+015.2f; Ug2=%+015.2f;\n",jz+1, mDataStar[jz].mZV[0][0], mDataStar[jz].mZV[0][1]/sr, mDataStar[jz].mZV[0][2]/sr);
                printf("              изм: з/в=%+010.6f; Ug1=%+015.2f; Ug2=%+015.2f;\n",      mDataStar[jz].mZV[1][0], mDataStar[jz].mZV[1][1]/sr, mDataStar[jz].mZV[1][2]/sr);
                printf("              -----------------------------------------------------------\n");                              
            };//if
             
          };//jz           
          printf(" OK! ====> Выполнена задача рекурсии данных (кор.вх.данных)\n");          
          printf(" ------------------------------------------------------------------------\n");                      
       // Остановка для просмотра
          if (!(nScr==2)){       
             printf(" => "); gets(sss);                  
          };//if
          goto mmAnom;
       };//else{
mmEND:
          printf(" ------------------------------------------------------------------------\n");                                    
          printf(" Внимание! Процедура фильтрации аномальных значения закончена\n");
          printf("           На конец  фильтрации, рабочая размерность вх.данных Nend=%d\n",Nend);                    
          printf("           --------------------------------------------------------------\n");                                              
if (!(nScr==2)){                             
          switch (nReg){
                case 0:                    
                   printf(" OK! ====> Успешно вып.задача оценки точности изм.зв.величины\n");          
                   printf("           CKOед:з/в=%10.6f [зв.вел]\n",sqrt(mCkoStar[0]));
                   printf("           DISед:з/в=%10.6f\n",mCkoStar[0]);
                   break;                                   
                case 1:    
                   printf(" OK! ====> Успешно вып.задача оценки точности изм.угл.положения\n");          
                   printf("           CKOед:ug1=%10.4f [угл.сек]; ug2= %10.4f[угл.сек];\n",sqrt(mCkoStar[1])/sr,sqrt(mCkoStar[2])/sr);                        
                   printf("           DISед:ug1=%10.4f;           ug2= %10.4f;\n",mCkoStar[1]/sr/sr,mCkoStar[2]/sr/sr);                                                                         
                   break;                   
          };//switch                    
};//if          
          printf("           Всего замеров Nkat=%d, из них достоверных N= %d\n",Nkat,Nend);                                  
          printf(" ------------------------------------------------------------------------\n");                                       
          
          printf(" далее -> <ввод> "); gets(sss);                            
          printf(" ------------------------------------------------------------------------\n");                                       
       // Проверка наличия систематической составляющей ошибки (с Pдов>=0.9)
if (!(nScr==2)){              
          switch (nReg){
                case 0:                
                   DeltaX= mDataXI2[Nend][0]*sqrt(mCkoStar[0]/(Nend-1));
                   if (fabs(mMatStar[0])<DeltaX){
                      printf(" OK! ====> Систематическая составляющая в зам.зв.вел. отсутствует\n");                      
                   }else{
                      printf(" Внимание! Наличие систематической составляющей в зам.зв.вел.\n");                                         
                   };
                      printf("           |сис.комп.зв.вел.|= %12.6f [зв.вел]\n",DeltaX);                                           
                   break;                                   
                case 1:    
                   DeltaX1= mDataXI2[Nend][0]*sqrt(mCkoStar[1]/(Nend-1));
                   DeltaX2= mDataXI2[Nend][0]*sqrt(mCkoStar[2]/(Nend-1));                   
                   if ((fabs(mMatStar[1])<DeltaX1)&&
                       (fabs(mMatStar[2])<DeltaX2)
                      ){
                      printf(" OK! ====> Систематическая составляющая в зам.угл. отсутствует\n");                      
                   }else{
                      printf(" Внимание! Наличие систематической составляющей в зам.угл.\n");                                         
                   };
                      printf("           Всего замеров Nkat=%d, из них достоверных N= %d\n",Nkat,Nend);                                                            
                      printf("           |сис.комп.UG1|= %12.6f [угл.сек]\n",DeltaX1/sr);                                           
                      printf("           |сис.комп.UG2|= %12.6f [угл.сек]\n",DeltaX2/sr);                                                                                 
                   break;                   
          };//switch
          printf(" ------------------------------------------------------------------------\n");                             
          printf(" далее -> <ввод> "); gets(sss);                                      
};//if
       // Верхняя и нижняя гр.дов.инт. для дисперсии измерений(с Pдов=0.95)       
if (!(nScr==2)) printf(" ------------------------------------------------------------------------\n");                 
          switch (nReg){
                case 0:
                   dZV=CkoZVkat*CkoZVkat;
                   DeltaX1= mCkoStar[0]/mDataXI2[Nend][3]-dZV;//нижняя  граница
                   DeltaX2= mCkoStar[0]/mDataXI2[Nend][2]-dZV;//верхняя граница                   
                   if (DeltaX1<0) DeltaX1=mCkoStar[0]/mDataXI2[Nend][3];
                   if (DeltaX2<0) DeltaX2=mCkoStar[0]/mDataXI2[Nend][2];
                // -------------------------------------------                      
if (!(nScr==2)){                
                   printf(" OK! ====> Границы доверительного интервала для оценок зв.величины\n");                      
                   printf("           |нижняя. гр.изм.зв.вел.|= %12.8f [з/в];\n",sqrt(DeltaX1));                                           
                   printf("           |верхняя.гр.изм.зв.вел.|= %12.8f [з/в];\n",sqrt(DeltaX2));
                   printf(" ------------------------------------------------------------------------\n");                                                                             
};//if                   
                   printf("\n");
                // Принятие решения о соответствии изделия предъявленным требованиям                      
                   printf(" ========================================================================\n");                                                 
/*                   
                   if (sqrt(DeltaX2)<=CkoZVpor){
                      printf(" Параметр: [точ.ед.зам.блеска] соответствует нормативам ТТХ \n");                      
                   }else{
                      printf(" Внимание! Параметр: [точ.ед.зам.блеска] не соответствует нормативам ТТХ \n");                   
                   };//if
                   printf("           СКОттх(зв.вел)= %11.6f [угл.сек];\n",CkoZVpor);                                           
                   printf("           Всего замеров Nkat=%d, из них достоверных N= %d\n",Nkat,Nend);                                                        
*/
                   DeltaX=sqrt(DeltaX2);
                   printf(" Внимание! Всего обработано замеров Nkat=%d, из них достоверных N= %d\n",Nkat,Nend);                   
                   printf("           В качестве оценки точности ед.изм-ния блеска принято:\n");                       
                   printf("           СКО=== %012.8f [зв.вел];\n",DeltaX);                                          
                   printf("           ----------------------------\n");
                   printf("           Исходный каталог: minZV= %07.4f [з/в]; maxZV= %07.4f [з/в];\n",minZV,maxZV);
                   printf("           Выходной каталог: minZV= %07.4f [з/в]; maxZV= %07.4f [з/в];\n",minZVend,maxZVend);                                      
                   printf("           --------------------------------------------------------------\n");                                                
                   printf("           Далее, необходимо провести сравнение с требуемым значением!\n");                            
//                 printf(" ========================================================================\n");                                                                    
                   break;                                   
                case 1:    
                   dZV=CkoUG1kat*CkoUG1kat;
                   DeltaX1= mCkoStar[1]/mDataXI2[Nend][3]-dZV;//нижняя  граница UG1
                   DeltaX2= mCkoStar[1]/mDataXI2[Nend][2]-dZV;//верхняя граница UG1
                   if (DeltaX1<0) DeltaX1=mCkoStar[1]/mDataXI2[Nend][3];
                   if (DeltaX2<0) DeltaX2=mCkoStar[1]/mDataXI2[Nend][2];
if (!(nScr==2)){                                   
                   printf(" OK! ====> Границы доверительного интервала для оценок UG1\n");                      
                   printf("           |нижняя. гр.изм.угл.пол|= %12.4f [угл.сек];\n",sqrt(DeltaX1)/sr);                                           
                   printf("           |верхняя.гр.изм.угл.пол|= %12.4f [угл.сек];\n",sqrt(DeltaX2)/sr);                   
                // -------------------------------------------   
                   printf("           --------------------------------------------------------------\n");                                                       
};//if                   
                   dZV=CkoUG2kat*CkoUG2kat;
                   DeltaX3= mCkoStar[2]/mDataXI2[Nend][3]-dZV;//нижняя  граница UG2
                   DeltaX4= mCkoStar[2]/mDataXI2[Nend][2]-dZV;//верхняя граница UG2
                   if (DeltaX3<0) DeltaX3=mCkoStar[2]/mDataXI2[Nend][3];
                   if (DeltaX4<0) DeltaX4=mCkoStar[2]/mDataXI2[Nend][2];                   
if (!(nScr==2)){                   
                   printf(" OK! ====> Границы доверительного интервала для оценок UG2\n");
                   printf("           |нижняя. гр.изм.угл.пол|= %12.4f [угл.сек];\n",sqrt(DeltaX3)/sr);                                           
                   printf("           |верхняя.гр.изм.угл.пол|= %12.4f [угл.сек];\n",sqrt(DeltaX4)/sr);                                      
                   printf(" ------------------------------------------------------------------------\n");                             
};//                   
                   printf("\n");
                // -------------------------------------------                      
                // Принятие решения о соответствии изделия предъявленным требованиям                      
                   printf(" ========================================================================\n");                                                                 
/*                   
                   if ((sqrt(DeltaX2)<=CkoUGpor)&&
                       (sqrt(DeltaX4)<=CkoUGpor)){
                      printf(" Параметр: [точ.ед.зам.угла] соответствует нормативам ТТХ \n");
                   }else{
                      printf(" Внимание! Параметр: [точ.ед.зам.угла] не соответствует нормативам ТТХ \n");                   
                   };//if                
*/                   
//                 printf("           СКОттх(угл.изм)= %12.4f [угл.сек];\n",CkoUGpor/sr);                                           

                   DeltaX=sqrt(DeltaX2);
                   if (DeltaX<sqrt(DeltaX4)) DeltaX=sqrt(DeltaX4);
                   printf(" Внимание! Всего обработано замеров Nkat=%d, из них достоверных N= %d\n",Nkat,Nend);                   
                   printf("           В качестве оценки точности ед.измерения угла принято:\n");                       
                   printf("           СКО=== %012.8f [угл.сек];\n",DeltaX/sr);                                          
                   printf("           -----------------------------\n");                                                                   
                   printf("           Исходный каталог: minZV= %07.4f [з/в]; maxZV= %07.4f [з/в];\n",minZV,maxZV);
                   printf("           Выходной каталог: minZV= %07.4f [з/в]; maxZV= %07.4f [з/в];\n",minZVend,maxZVend);                   
                   printf("           --------------------------------------------------------------\n");                                                
                   printf("           Далее, необходимо провести сравнение с требуемым значением!\n");      
//                 printf(" ========================================================================\n");                                                 
                   break;                   
          };//switch
//        printf(" ------------------------------------------------------------------------\n");                                       
          printf(" ========================================================================\n");                                                 

       // Остановка для просмотра
          printf(" далее -> <ввод> "); gets(sss);                                   
          printf(" ========================================================================\n");                                                           
       // Запись в файл                   
          printf(" Входной файл данных ? y-<перезапись исходного>,n-<новый целевой каталог>\n");
          printf(" => "); gets(sss);                  
          printf(" ========================================================================\n");                                                                     
          printf(" ДатаФайл! Формирование целевого звездного каталога ");
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
          printf("           ждите...\n");                            
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
          printf(" OK! ====> Выполнена задача формирование целевого звездного каталога\n");
          printf(" ========================================================================\n");                                                                     
//        printf(" ------------------------------------------------------------------------\n");          
//       };//if (kz>0)
       
    // Остановка для просмотра
    // printf(" => "); gets(sss);        
              
       return 0;
   }//void ModSTAR() - Имитатор измерений параметров кат.звезд
// \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\   

