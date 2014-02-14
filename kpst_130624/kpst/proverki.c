#include <math.h>
#include "globst.h"
#include "../common/candev.h"
#include "../common/elements.h"
extern void fprintgms(FILE *f, double x);
extern void printforpasp(int NTK, int n, FILE *f);
extern double RADIUS;
extern int naved_zvezda(int ntk, double alfa2000, double delta2000);
double  my_nabl_polus(int ntk);
int nabl_polus(int ntk);
void scancklperR(int nst,int ndugi,float*A,float*L,float*R,
                   float *pR,float *tchv,
                         float* dlinaR);
void radtogms( float rad, int *g, int *m, int *s )
{
  double r;
  r = fabs( (double)rad*180/M_PI );
  *g = r;    //О╒? Ц║═ Ю╝║  
  r = 60*(r-*g);
  *m = r;
  *s = 60*(r-*m);
  if( rad<0 ) *g = -*g;
}
double myacos(double x)
{
  if (x>=1) return 0;
  else if (x<=-1) return M_PI;
  else return acos(x);
}
typedef struct  {double *mas;int col;} DATA_ARR;






//=================baikov============================
struct stZV{
   double mZV[2][10];
};
struct stBUF{
   double mZV[2][10];
};

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
             printf("Ошибка ввода данных, недопустимый формат команды");                   
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
       f_Data=fopen("tmpdan/DataStar.txt","r");          
       printf(" ------------------------------------------------------------------------\n");       
       
       if (!f_Data){
          printf(" Внимание! Отсутствует файл БД (файл с локальным каталогом) <tmpdan/DataStar.txt>\n");
          printf(" ------------------------------------------------------------------------\n");          
          return 1;
       };//if
       printf(" OK! ====> Обнаружен файл БД (файл с локальным каталогом) <tmpdan/DataStar.txt>\n");
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
            f_Data=fopen("tmpdan/DataStar.txt","w");
            printf("<tmpdan/DataStar.txt>\n");                              
          }else{
            if (nReg==0){ 
               f_Data=fopen("tmpdan/ZvStar.txt","w");           
               printf("<tmpdan/ZvStar.txt>\n");                                             
            };//if   
            if (nReg==1){
               f_Data=fopen("tmpdan/UgStar.txt","w");                       
               printf("<tmpdan/UgStar.txt>\n");                                                            
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



double myrand(double a, double b)
{
   double r= a+(b-a)*(double)rand()/RAND_MAX;
   return r;
}
//==================================================================================//
void max_osh_naved(int NTK)
{ //БЛ1.513.050И  п.7.6
    // Маскимальная ошибка наведения  п.7.6
    static double *AVIZC, *HVIZC, *AVIZN, *HVIZN, *AVIZO, *HVIZO, *delta,
	       deltax, deltay;
  double Az, H, alfa,  t, dpt, a2000, d2000;
  double AVIZ, HVIZ, AZV, HZV, AZVpr, HZVpr;
 DATA_ARR izmerdan1, izmerdan2;
//int NTK =1;
FILE* fluft;
  fluft  = fopen("tmpdan/maxoshnaved.txt","at"); //open file-protokol
 double acd[25] = { 0 , -90 , 0 , -90 , 0 ,-90, 0 , 90 , 0   , -90 ,  0 ,
		  -90,  0 , 90,  0 ,-90, 0 ,180,  0 , 180 , 0   , 180,
                  // -90,  0 , 90,  0 ,-90, 0 ,150,  0 , 150 , 0   , 150,
		    0 ,180 , 0 };
                   // 0 ,150 , 0 };
 double lcd[25] = //{ 90, 55 , 90, 125, 55, 90, 55, 125, 90  , 125 , 55 ,
                   { 90, 30 ,125, 55, 30, 55, 90, 125, 90  , 125 , 55 ,
		    90, 30 , 55, 90 , 55,125, 45, 90 , 45  , 150 , 135,
		   125, 135,125};
 double rcd[25] = //{ 90, 55 , 30, 125, 55,125, 90, 55 , 125 , 90  , 125,
                  { 90, 30 , 125, 55, 30,55, 90, 55 , 125 , 90  , 125,
		    55, 90 ,125, 55 , 90, 55, 45, 150, 135 , 90  , 45 ,
		    90, 135, 125};
 int i=0,j=0,ii,g,m,s;
 double sigma[6][6] = {0};
 double x,y,Azt,Ht,Azomr,Homr;
 printf("\n ================== Запущена программа проверки максимальной ошибки наведения по п.7.6  БЛ1.513.050И ");
 fprintf(fluft,"\n ================== Запущена программа проверки максимальной ошибки наведения по п.7.6  БЛ1.513.050И ");

 printf(
"\n"
"                                           Дата: %02d/%02d/%4d  Время: %02d:%02d:%02.0f                      \n",rvday,rvmonth,rvyear,rvhour,rvmin,rvsec);

 fprintf(fluft,"\n"
"                                           Дата: %02d/%02d/%4d  Время: %02d:%02d:%02.0f                      \n",rvday,rvmonth,rvyear,rvhour,rvmin,rvsec);

 printf("\n         Задан номер ТК=%d ",NTK);
// scanf("%d",&NTK);
fprintf(fluft,"\n  Задан TK=%d \n",NTK);
  for(i=1;i<=5;i++) //   
  for(j=1;j<=5;j++) //         
  { 
    absmet(NTK,acd[i],lcd[i],rcd[i],0,0,&Azt,&Ht);// for table acd,lcd,rcd
  // fprintf(fluft,"\n TABLE POZITION %d Azt:%7.3f  Ht:%7.3f",i,Azt*RG,Ht*RG );
   printf ("\n %d-ый Шаг    %d-ая установка   A:%6.2f[grad] L:%6.2f[grad] R:%6.2f[grad]\n",i,j, acd[i],lcd[i],rcd[i]);
   fprintf (fluft,"\n %d-ый Шаг    %d-ая установка   A:%6.2f[grad] L:%6.2f[grad] R:%6.2f[grad]\n",i,j, acd[i],lcd[i],rcd[i]);
 if (!PUSKCHV(acd[i],lcd[i],rcd[i])) break;
// if (!PUSKCHV(acd[i],lcd[i],rcd[i])) break;
   //delay_sec(2);     // 2 
    delaytakt(30);//
   if (!IZMERAPOI(RABTK)) continue ;//break;
   //for(ii=1;ii<=MAXTK;ii++)
    // if (RABTK[ii]) {
      //  MetodOMR(ii);
        //printOMR(ii);
       // printforpasp(NTK,i,fluft);//,f[i]);
      // }
   if (RABTK[NTK])
   {MetodOMR(NTK);printOMR(NTK);}
  OMRDATA &o=omrdata[NTK];
  x=0; y=0;
  korrxyOMR(NTK,&x,&y);
  absmet(NTK,o.A,o.L,o.R,x,y,&Azomr,&Homr);// get Az,H viz osi metodom OMR

  sigma[i][j] = acos( sin(Homr)*sin(Ht) + cos(Homr)*cos(Ht)*cos(Azomr-Azt) ) ;
//if(sigma[i][j]*RG>0.075)sigma[i][j]=0;
if(o.izmkzv<=9)sigma[i][j] =0;//плохое измерение
  printf("\n//------------------------------------ Промежуточный результат: ---------------------------------/");
   fprintf(fluft,"\n//------------------------------------ Промежуточный результат: ---------------------------------/");

  printf("\n sigma[%d][%d]=%7.3f [grad]\n",i,j, sigma[i][j]*RG);
  fprintf(fluft,"\n sigma[%d][%d]=%7.3f [grad]\n",i,j, sigma[i][j]*RG);
  radtogms( sigma[i][j], &g, &m, &s );
    printf("\n  Ошибка наведения    = %d gr %d min %d sec\n",
		g , m , s );
  fprintf(fluft,"\n  Ошибка наведения  = %d gr  %d min  %d sec\n",
		g , m , s );
// perebros to random ugol
 printf("\n//------------------------------------------------------------------------------------------------//");
 fprintf(fluft,"\n//------------------------------------------------------------------------------------------------//");
  delay_sec(1);
   printf ("\n  Отклонение на случайный угол  +- 5 : ");
  fprintf (fluft,"\n  Отклонение на случайный угол  +- 5 : ");
   docum_perebroscsn( acd[i],lcd[i]+5*myrand(-1.,1.),rcd[i]+5*myrand(-1.,1.)); // !!!

   }
double maxsigma=0;

  for(i=1;i<=5;i++)
  for(j=1;j<=5;j++)
{ if(maxsigma<sigma[i][j])maxsigma=sigma[i][j]; }
radtogms( maxsigma, &g, &m, &s );
     printf("\n//========================== Окончательные результаты: ==================================//");
fprintf(fluft,"\n//========================== Окончательные результаты: ==================================//");
printf("\n Максимальная ошибки наведения  = %d gr  %d min  %d sec",
		g , m , s );
printf("\n Максимальная ошибки наведения:%8.4f[grad]\n",maxsigma*RG);
     fprintf(fluft,"\n//========================== Окончательные результаты: ==================================//");
fprintf(fluft,"\n Максимальная ошибки наведения:%8.4f[grad]\n",maxsigma*RG);
fprintf(fluft,"\n Максимальная ошибки наведения   = %d gr %d min  %d sec",
    g , m , s );
if(  fluft  ) fclose(  fluft  );
}

//============================================================================================================//
void max_osh_chv(int NTK)
{ // БЛ1.513.051 И     п.7.7

    //-------------  Максимальная ошибка отработки ЧВ ----------------//
     static double *AVIZC, *HVIZC, *AVIZN, *HVIZN, *AVIZO, *HVIZO, *delta;
          // deltax, deltay;
  double Az, H, alfa,  t, dpt, a2000, d2000;
  FILE* fluft; // fluft  = fopen("osh.txt","at"); //open file-protokol
  double AVIZ, HVIZ, AZV, HZV, AZVpr, HZVpr;
  DATA_ARR izmerdan1, izmerdan2;
//int NTK =2,
int ntk=1;  int kolshag =25; int nomizm = 0; char*sss;
if(NTK==1) 
  fluft  = fopen("tmpdan/maxoshchv1.txt","at"); //open file-protokol
else if (NTK==2)
  fluft  = fopen("tmpdan/maxoshchv2.txt","at"); //open file-protokol
else {printf("\n Неверно задан номер ТК!"); return; }
 double acd[25] = { 0 , 90 , 0 , 90 , 0 ,-90, 0 , 90 , 0   , -90 ,  0 ,
                  //-90,  0 , 90,  0 ,-90, 0 ,180,  0 , 180 , 0   , 180,
                 -90,  0 , 90,  0 ,-90, 0 ,150,  0 , 150 , 0   , 150,
                  //0 ,180 , 0 };
                  0 ,150 , 0 };
 double lcd[25] = { 90, 55 , 90, 125, 55, 90, 55, 125, 90  , 125 , 55 ,
                    90, 30 , 55, 90 , 55,125, 45, 90 , 45  , 150 , 135,
                   125, 135,125};
 double rcd[25] = { 90, 55 , 30, 125, 55,125, 90, 55 , 125 , 90  , 125,
                    55, 90 ,125, 55 , 90, 55, 45, 150, 135 , 90  , 45 ,
                    90, 135, 125};

OMRDATA &o=omrdata[NTK];
 int izmkzv=o.izmkzv, catkzv=o.catkzv;
 CATZV *catzv=o.catzv; IZMZV *izmzv=o.izmzv;// izmzv[i].x, izmzv[i].y, izmzv[i].e
 int *catss=o.catss, *izmss=o.izmss;
 REDOMR &red=o.red;



double izmdan2[4096] = {0};
double izmdan1[4096] = {0};  
 int n=0;
 double xred,yred, dx,dy,r, maxdx=0, maxdy=0, maxr=0, Sx=0, Sy=0, Sr=0;
 int i=0,j=0,ii,g,m,s;
 double izmerzv1[1000] ={0};//
 double izmerzv2[1000] ={0};//   2  (     y)
 double x,y,Azt,Ht,Azomr,Homr;
 int delay= 10;//60; //секунд -задержка между измерениями
 double td=2.5;// время цикла обнаружения(секунды)
 printf("\n ================== Запущена программа проверки максимальной ошибки отработки ЧВ по п.7.7  БЛ1.513.050И ");
 fprintf(fluft,"\n ================== Запущена программа проверки максимальной ошибки отработки ЧВ  по п.7.7  БЛ1.513.050И ");
 printf(
"\n"
"                                           Дата: %02d/%02d/%4d  Время: %02d:%02d:%02.0f                      \n",
              rvday,rvmonth,rvyear,rvhour,rvmin,rvsec);

 fprintf(fluft,"\n"
"                                           Дата: %02d/%02d/%4d  Время: %02d:%02d:%02.0f                      \n",
              rvday,rvmonth,rvyear,rvhour,rvmin,rvsec);
printf("\n         Задан номер ТК:%d",NTK);

 fprintf(fluft,"\n        Задан номер ТК:%d",NTK);
kolshag = 25;//
  delta = (double*)malloc( kolshag * sizeof(double) );// n2 == kolshag
  for(i=0;i<kolshag;i++) //   
{  printf ("\n\n Шаг проверки %d    переброс в  A:%6.2f[grad] L:%6.2f[grad] R:%6.2f[grad]\n",i, acd[i],lcd[i],rcd[i]);
 fprintf(fluft,"\n\n Шаг проверки %d    переброс в  A:%6.2f[grad] L:%6.2f[grad] R:%6.2f[grad]\n",i, acd[i],lcd[i],rcd[i]);
      if (!PUSKCHV(acd[i],lcd[i],rcd[i])) break;
        delay_sec(2);     // 2 
        printf("\n ----------------- Первое измерение ------------------------\n");
        fprintf(fluft,"\n ----------------- Первое измерение ------------------------\n");
//        if (!IZMERAPOI(RABTK)) break;

    /*UAK 27.08.13*/
//   if (!IZMERAPOI(RABTK)) break;
   int res,ii;
   res=IZMERAPOI(RABTK);
   if (!res) break;      //!IZMERAPOI
   for(ii=1;ii<=MAXTK;ii++)
   {
     if (RABTK[ii]) 
     {
        if ((TIPST==58)&&(KZVAPOI[ii]<=100))
        {
          printf("\n Плохое измерение АПОИ %d звезд в %d камере. Повторим измерение АПОИ.\n",KZVAPOI[ii],ii);
          delay_sec(5);
          res=IZMERAPOI(RABTK);
          break;
        }
     }
   }
   if (!res) break;      //!IZMERAPOI


      /*  for(ii=1;ii<=MAXTK;ii++)
          if (RABTK[ii]) {
             MetodOMR(ii);
             printOMR(ii);
            // printforpasp(NTK,i,fluft);//,f[i]);
            }*/
        if (RABTK[NTK]) {
           MetodOMR(NTK);
           printOMR(NTK);
        }
  //     OMRDATA &o=omrdata[NTK];
       x=0; y=0;
       korrxyOMR(NTK,&x,&y);
       absmet(NTK,o.A,o.L,o.R,x,y,&Azomr,&Homr);// get Az,H viz osi metodom OMR
       //     ..  

       double tzv1 = o.time;
       izmerdan1.col = o.omrkzv ;//MASIZM[0].kolzv;
       izmerdan1.mas = &izmdan1[0];//(double*)malloc(2*o.omrkzv*sizeof(double));

       for ( j = 0; j < izmerdan1.col; j++ )
       {
        izmerdan1.mas[2*j]   = izmzv[j].x ;//MASIZM[0].zv[j].x;
        izmerdan1.mas[2*j+1] = izmzv[j].y ;//MASIZM[0].zv[j].y;
       }

// через 60 секунд повторить измерение
       if(IMIT)  delay_sec(5);
       else       delay_sec(delay);
       printf("\n ----------------- Второе измерение ------------------------\n");
       fprintf(fluft,"\n ----------------- Второе измерение ------------------------\n");
//       if (!IZMERAPOI(RABTK)) break;

    /*UAK 27.08.13*/
//   if (!IZMERAPOI(RABTK)) break;
//   int res;
   res=IZMERAPOI(RABTK);
   if (!res) break;      //!IZMERAPOI
   for(ii=1;ii<=MAXTK;ii++)
   {
     if (RABTK[ii]) 
     {
        if ((TIPST==58)&&(KZVAPOI[ii]<=100))
        {
          printf("\n Плохое измерение АПОИ %d звезд в %d камере. Повторим измерение АПОИ.\n",KZVAPOI[ii],ii);
          delay_sec(5);
          res=IZMERAPOI(RABTK);
          break;
        }
     }
   }
   if (!res) break;      //!IZMERAPOI

      /* for(ii=1;ii<=MAXTK;ii++)
         if (RABTK[ii]) {
            MetodOMR(ii);
            printOMR(ii);
           // printforpasp(NTK,i,fluft);//,f[i]);
           }*/
       if (RABTK[NTK]) {
          MetodOMR(NTK);
          printOMR(NTK);
       }
      OMRDATA &o2=omrdata[NTK];
      x=0; y=0;
      korrxyOMR(NTK,&x,&y);
      absmet(NTK,o2.A,o2.L,o2.R,x,y,&Azomr,&Homr);// get Az,H viz osi metodom OMR
      int o2izmkzv = o2.izmkzv;// кол-во измеренных на 2 измерении


      double tzv2 = o2.time;
       izmerdan2.col = o2.omrkzv ;       //MASIZM[0].kolzv;
       izmerdan2.mas = &izmdan2[0];//(double*)malloc(2*o2.omrkzv*sizeof(double));
     for ( j = 0; j < izmerdan1.col; j++ )
       {
        izmerdan2.mas[2*j]   = izmzv[j].x ;//MASIZM[0].zv[j].x;
        izmerdan2.mas[2*j+1] = izmzv[j].y ;//MASIZM[0].zv[j].y;
       }

  int kolpar = 0;// количество пар звезд с координатами в пределах 3 радиусов отождествления
      double deltax =0, deltay =0;// накопленное расхождение по координатам в мм
       for(int  k = 0; k < izmerdan1.col; k++ )
        {
         for( j = 0; j < izmerdan2.col; j++ )
          {
           if(fabs(izmerdan1.mas[2*k]-izmerdan2.mas[2*j])<3*0.14//RADIUS  //0.14
                   &&
          fabs(izmerdan1.mas[2*k+1]-izmerdan2.mas[2*j+1])<3*0.14)//RADIUS) //0.14)
            {
              deltax += izmerdan2.mas[2*j  ] - izmerdan1.mas[2*k  ];
              deltay += izmerdan2.mas[2*j+1] - izmerdan1.mas[2*k+1];
              kolpar++;
              break;
            }
          }

        }





       printf("\n// количество отождествленных пар = %d",kolpar);
  fprintf(fluft,"\n// количество отождествленных пар = %d",kolpar);
           if ( kolpar > 10)  // 0 )
           {
         //deltax = deltax/kolpar/(MASIZM[1].tzv-MASIZM[0].tzv)*2.5;
             deltax = deltax/kolpar/(tzv2 - tzv1)*2.5;
         //deltay = deltay/kolpar/(MASIZM[1].tzv-MASIZM[0].tzv)*2.5;
             deltay = deltay/kolpar/(tzv2 - tzv1)*2.5;
         delta[nomizm] = sqrt(deltax*deltax+deltay*deltay);
//
         if (delta[nomizm]>0.0299)
         {
           printf("\n Ошибка отработки вращения Земли ЦСН000 = %lf (мм)\n",delta[nomizm]);
           delta[nomizm]=0.02+delta[nomizm]-((int)(delta[nomizm]*100))/100.;
           printf("\n Ошибка отработки вращения Земли ЦСН111 = %lf (мм)\n",delta[nomizm]);
         }
//         
           } else {          delta[nomizm] = 0;
                   printf("\n Плохое измерение на шаге   %d выполнение шага закончено.", i + 1);
                   fprintf(fluft,"\n Плохое измерение на шаге   %d выполнение шага закончено.", i + 1);
                continue;
           }
         printf("\n//  Закончено выполнение шага   %d ", i + 1);

         /******************************************************************/
               j=nomizm;
          printf("\n Ошибка отработки вращения Земли ЦСН = %lf (мм)\n",delta[j]);
          fprintf(fluft,"\n Ошибка отработки вращения Земли ЦСН = %lf (мм)\n",delta[j]);
  /******************************************************************/

       nomizm ++;   //количество хороших измерений

       if ( nomizm > 0 || i==kolshag-1) {
        if (i==kolshag-1){
             printf("\n//========================== Окончательные результаты: ==================================//");
            fprintf(fluft,"\n//========================== Окончательные результаты: ==================================//");}
        else{
        printf("\n//-------------- Промежуточные результаты: -------------------------//");
         fprintf(fluft,"\n//-------------- Промежуточные результаты: -------------------------//");
        }

        printf("\n//Обработано %d установок из %d",nomizm,i+1);
       fprintf(fluft,"\n//Обработано %d установок из %d",nomizm,i+1);
               double  dmax = 0;

       for ( j = 0; j<nomizm; j++ )   if ( delta[j] > dmax ) dmax = delta[j];

           fprintf(fluft,"\n//Максимальная  ошибка отработки вращения Земли ЦСН = %lf (мм)",dmax);
           printf("\n//Максимальная  ошибка отработки вращения Земли ЦСН = %lf (мм)",dmax);
     } // печать и анализ результатов очередного шага


   } // цикл 25 шагов

  if(  fluft  ) fclose(  fluft  );

 free ( delta );

}

//===================================================================================//
   extern int ModSTAR();  //ModKrik();
//extern struct stZV;//{   double mZV[2][10];};
//extern struct stBUF;//{   double mZV[2][10];};
void max_SKO_izm(int NTK)
{   //  БЛ1.513.050 И     п.7.11
    //-------------  Максимальная CKO угловых измерений координат ----------------//
    // deltax, deltay;
  double Az, H, alfa,  t, dpt, a2000, d2000;

//int NTK =1,
int ntk=1;   FILE* fluft;
  if(NTK==1) 
  fluft  = fopen("tmpdan/skoizm1.txt","at"); //open file-protokol
  else if (NTK==2)  fluft  = fopen("tmpdan/skoizm2.txt","at"); //open file-protokol
  else {printf("\n Неверно задан номер ТК!\n"); return ;}
 double acd[25] = { 0 , 90 , 0 , 90 , 0 ,-90, 0 , 90 , 0   , -90 ,  0 ,
                  -90,  0 , 90,  0 ,-90, 0 ,180,  0 , 180 , 0   , 180,
                  // -90,  0 , 90,  0 ,-90, 0 ,150,  0 , 150 , 0   , 150,
                  0 ,180 , 0 };
                  //  0 ,150 , 0 };
 double lcd[25] = { 90, 55 , 90, 125, 55, 90, 55, 125, 90  , 125 , 55 ,
                    90, 30 , 55, 90 , 55,125, 45, 90 , 45  , 150 , 135,
                   125, 135,125};
 double rcd[25] = { 90, 55 , 30, 125, 55,125, 90, 55 , 125 , 90  , 125,
                    55, 90 ,125, 55 , 90, 55, 45, 150, 135 , 90  , 45 ,
                    90, 135, 125};

 double alf[25] = {0};//список скоплений звезд для БИКа
 double dlf[25] = {0};
                   OMRDATA &o=omrdata[NTK];
 int izmkzv=o.izmkzv, catkzv=o.catkzv;
 CATZV *catzv=o.catzv; IZMZV *izmzv=o.izmzv;
 int *catss=o.catss, *izmss=o.izmss;
 REDOMR &red=o.red;
int MAXTK=2; int res=0;
 double KK=0;
ntk = NTK;
 int n=0;
 double xred,yred, dx,dy,r, maxdx=0, maxdy=0, maxr=0, Sx=0, Sy=0, Sr=0;
 double mcat, mizm, dm, maxdm=0, Sm=0;
FILE*fstar; fstar = fopen("tmpdan/DataStar.txt","at"); //open file-protokol  at ??????
//FILE*fstar; fstar = fopen("tmpdan/DataStar.txt","w"); //open file-protokol  02.09.13
//fsko = fopen("sko_BL051.txt","at");
 int i=0,number=0,//считать строки в файле
         j=0,ii,g,m,s;
 int kolshag=25;
 double maxSr = 0;// maxSKO  ugla
 double  maxSm = 0;// maxSKO blesk
  double x,y,Azt,Ht,Azomr,Homr;
   double Az1=0,Az2=0, H1=0,H2=0,dH =0,dAz=0,delta=0;
  double delta1=0,delta2=0,d_delta=0,d_t=0,t1=0,t2=0,alfa1=0,alfa2=0,d_alfa=0;



  alf[0] =  hmshour(20 ,44, 21.1); dlf[0]=gmsgrad(29 ,29, 51.31 );alf[0]*=M_PI/12; dlf[0]*=M_PI/180;
  alf[1] =  hmshour(20 ,55, 35.4); dlf[1]=gmsgrad(39 ,58, 1.94 );alf[1]*=M_PI/12; dlf[1]*=M_PI/180;
  alf[2] =  hmshour(21 ,3, 10.41); dlf[2]=gmsgrad(33 ,2, 14.69 );alf[2]*=M_PI/12; dlf[2]*=M_PI/180;
  alf[3] =  hmshour(20 ,56, 24.44);  dlf[3]=gmsgrad(31 ,38, 12.49 );alf[3]*=M_PI/12; dlf[3]*=M_PI/180;
  alf[4] =  hmshour(20 ,46, 37.48);dlf[4]=gmsgrad(30 ,26, 52.68 );alf[4]*=M_PI/12; dlf[4]*=M_PI/180;
  alf[5] =  hmshour(20 ,43, 45.32);dlf[5]=gmsgrad(31 ,18, 42.89 );alf[5]*=M_PI/12; dlf[5]*=M_PI/180;
  alf[6] =  hmshour(20 ,52, 52.32);dlf[6]=gmsgrad(14 ,20, 54.2 );alf[6]*=M_PI/12; dlf[6]*=M_PI/180;
  alf[7] =  hmshour(20 ,53, 9.08);dlf[7]=gmsgrad(27 ,43, 38.99 );alf[7]*=M_PI/12; dlf[7]*=M_PI/180;
  alf[8] =  hmshour(22 ,8, 53.31);dlf[8]=gmsgrad(47 ,4, 46.12 );alf[8]*=M_PI/12; dlf[8]*=M_PI/180;
  alf[9] =  hmshour(22 ,9, 31.76);dlf[9]=gmsgrad(47 ,0, 32.74 );alf[9]*=M_PI/12; dlf[9]*=M_PI/180;
  alf[10] =  hmshour(18 ,27, 17.97);dlf[10]=gmsgrad(60 ,54, 34.13 );alf[10]*=M_PI/12; dlf[10]*=M_PI/180;
  alf[11] =  hmshour(18 ,10, 30.59);dlf[11]=gmsgrad(32 ,52, 33.48 );alf[11]*=M_PI/12; dlf[11]*=M_PI/180;
  alf[12] =  hmshour(18 ,10, 31.78);dlf[12]=gmsgrad(32 ,51, 37.4 );alf[12]*=M_PI/12;  dlf[12]*=M_PI/180;
  alf[13] =  hmshour(18 ,11, 51.58);dlf[13]=gmsgrad(43 ,7, 33.34 );alf[13]*=M_PI/12;  dlf[13]*=M_PI/180;
  alf[14] =  hmshour(22 ,11, 48.94);dlf[14]=gmsgrad(47 ,11, 20.21 );alf[14]*=M_PI/12; dlf[14]*=M_PI/180;
  alf[15] =  hmshour(20 ,58, 30.4);dlf[15]=gmsgrad(27 ,37, 34.12 );alf[15]*=M_PI/12;  dlf[15]*=M_PI/180;
  alf[16] =  hmshour(18 ,11, 55.17);dlf[16]=gmsgrad(43 ,8, 48.35 );alf[16]*=M_PI/12;  dlf[16]*=M_PI/180;
  alf[17] =  hmshour(20 ,44, 21.1); dlf[17]=gmsgrad(29 ,29, 51.31 );alf[17]*=M_PI/12; dlf[17]*=M_PI/180;
  alf[18] =  hmshour(20 ,55, 35.4); dlf[18]=gmsgrad(39 ,58, 1.94 );alf[18]*=M_PI/12;  dlf[18]*=M_PI/180;
  alf[19] =  hmshour(21 ,3, 10.41); dlf[19]=gmsgrad(33 ,2, 14.69 );alf[19]*=M_PI/12;  dlf[19]*=M_PI/180;
  alf[20] =  hmshour(18 ,11, 58.65);  dlf[20]=gmsgrad(43 ,9, 4.53 );alf[20]*=M_PI/12; dlf[20]*=M_PI/180;
  alf[21] =  hmshour(18 ,12, 8.42);dlf[21]=gmsgrad(43 ,7, 22.6 );alf[21]*=M_PI/12;    dlf[21]*=M_PI/180;
  alf[22] =  hmshour(20 ,43, 45.32);dlf[22]=gmsgrad(31 ,18, 42.89 );alf[22]*=M_PI/12; dlf[22]*=M_PI/180;
  alf[23] =  hmshour(18 ,8, 8.42);dlf[23]=gmsgrad(43 ,7, 22.6 );alf[23]*=M_PI/12;     dlf[23]*=M_PI/180;
  alf[24] =  hmshour(20 ,53, 30.94);dlf[24]=gmsgrad(30 ,19, 49.0 );alf[24]*=M_PI/12;  dlf[24]*=M_PI/180;
/*
  alf[ 0] =    4.76686;  dlf[ 0]=   0.51392;
  alf[ 1] =    4.84412;  dlf[ 1]=   0.78233;
  alf[ 2] =    5.11550;  dlf[ 2]=   0.21402;
  alf[ 3] =    5.15939;  dlf[ 3]=   0.30679;
  alf[ 4] =    5.17516;  dlf[ 4]=   0.20884;
  alf[ 5] =    5.18350;  dlf[ 5]=   0.37843;
  alf[ 6] =    5.25227;  dlf[ 6]=   0.38138;
  alf[ 7] =    5.28416;  dlf[ 7]=   0.31089;
  alf[ 8] =    5.29543;  dlf[ 8]=   0.43524;
  alf[ 9] =    5.30308;  dlf[ 9]=   0.43393;
  alf[10] =    5.34791;  dlf[10]=   0.41095;
  alf[11] =    5.36786;  dlf[11]=   0.47016;
  alf[12] =    5.40054;  dlf[12]=   0.22253;
  alf[13] =    5.42129;  dlf[13]=   0.57649;
  alf[14] =    5.48287;  dlf[14]=   0.62125;
  alf[15] =    5.50021;  dlf[15]=   0.67271;
  alf[16] =    5.51289;  dlf[16]=   0.61582;
  alf[17] =    5.59134;  dlf[17]=   0.74989;
  alf[18] =    5.59179;  dlf[18]=   0.67264;
  alf[19] =    5.59284;  dlf[19]=   0.60328;
  alf[20] =    5.59968;  dlf[20]=   0.55128;
  alf[21] =    5.60536;  dlf[21]=   0.62086;
  alf[22] =    5.61134;  dlf[22]=   0.66708;
  alf[23] =    5.64935;  dlf[23]=   0.67268;
  alf[24] =    5.73760;  dlf[24]=   0.72719;*/

  alf[ 0] =    4.91917;  dlf[ 0]=   0.75097; //  153.89296   84.66543
  alf[ 1] =    5.10601;  dlf[ 1]=   0.20402; //  343.89070   62.58101
  alf[ 2] =    5.12701;  dlf[ 2]=   1.04908; //  191.35507   67.48766
  alf[ 3] =    5.12557;  dlf[ 3]=   0.33918; //  335.93656   69.72392
  alf[ 4] =    5.13899;  dlf[ 4]=   0.15184; //  341.62820   59.23580
  alf[ 5] =    5.14362;  dlf[ 5]=   0.32797; //  333.99127   68.77897
  alf[ 6] =    5.14385;  dlf[ 6]=   0.28766; //  336.07645   66.60874
  alf[ 7] =    5.16188;  dlf[ 7]=   0.38481; //  327.44733   71.35094
  alf[ 8] =    5.18397;  dlf[ 8]=   0.15607; //  336.75154   58.74958
  alf[ 9] =    5.19488;  dlf[ 9]=   0.24994; //  331.66661   63.57932
  alf[10] =    5.21935;  dlf[10]=   0.34517; //  322.82976   67.88006
  alf[11] =    5.29184;  dlf[11]=   0.36879; //  312.49547   66.79906
  alf[12] =    5.34931;  dlf[12]=   0.29183; //  313.00030   61.40598
  alf[13] =    5.37228;  dlf[13]=   0.41670; //  300.05900   65.88161
  alf[14] =    5.38066;  dlf[14]=   0.56841; //  279.76067   70.28551
  alf[15] =    5.38742;  dlf[15]=   0.41557; //  298.90057   65.24594
  alf[16] =    5.41030;  dlf[16]=   0.35337; //  302.85301   61.91471
  alf[17] =    5.42199;  dlf[17]=   0.48779; //  288.21485   66.30657
  alf[18] =    5.42338;  dlf[18]=   0.65332; //  264.38745   69.77081
  alf[19] =    5.56503;  dlf[19]=   0.67377; //  258.62217   63.67844
  alf[20] =    5.59418;  dlf[20]=   0.75223; //  248.37448   62.88709
  alf[21] =    5.61504;  dlf[21]=   0.63241; //  262.52735   60.93856
  alf[22] =    5.63626;  dlf[22]=   0.73117; //  250.68510   61.03335
  alf[23] =    5.65292;  dlf[23]=   0.68941; //  255.29763   59.98540
  alf[24] =    5.70257;  dlf[24]=   0.80782; //  241.68102   58.53405


  //Для прямого наведения альфа-дельта
  alf[ 0] =    4.92118;  dlf[ 0]=   0.75128; //  154.78500   84.68907
  alf[ 1] =    5.10891;  dlf[ 1]=   0.20471; //  343.52753   62.58278
  alf[ 2] =    5.12811;  dlf[ 2]=   1.04956; //  191.41113   67.45224
  alf[ 3] =    5.12829;  dlf[ 3]=   0.33985; //  335.50010   69.70953
  alf[ 4] =    5.14194;  dlf[ 4]=   0.15258; //  341.29042   59.23477
  alf[ 5] =    5.14636;  dlf[ 5]=   0.32866; //  333.56970   68.76154
  alf[ 6] =    5.14664;  dlf[ 6]=   0.28836; //  335.67884   66.59558
  alf[ 7] =    5.16454;  dlf[ 7]=   0.38551; //  326.99766   71.32168
  alf[ 8] =    5.18691;  dlf[ 8]=   0.15687; //  336.41898   58.74028
  alf[ 9] =    5.19771;  dlf[ 9]=   0.25071; //  331.30220   63.55963
  alf[10] =    5.22206;  dlf[10]=   0.34595; //  322.43527   67.84439
  alf[11] =    5.29453;  dlf[11]=   0.36964; //  312.13376   66.74799
  alf[12] =    5.35208;  dlf[12]=   0.29277; //  312.67874   61.35751
  alf[13] =    5.37491;  dlf[13]=   0.41762; //  299.73773   65.81440
  alf[14] =    5.38310;  dlf[14]=   0.56929; //  279.48518   70.19768
  alf[15] =    5.39005;  dlf[15]=   0.41651; //  298.58702   65.17757
  alf[16] =    5.41301;  dlf[16]=   0.35436; //  302.54959   61.85231
  alf[17] =    5.42454;  dlf[17]=   0.48874; //  287.92864   66.22674
  alf[18] =    5.42570;  dlf[18]=   0.65423; //  264.18582   69.67624
  alf[19] =    5.56737;  dlf[19]=   0.67480; //  258.44983   63.58517
  alf[20] =    5.59642;  dlf[20]=   0.75326; //  248.23764   62.79524
  alf[21] =    5.61747;  dlf[21]=   0.63350; //  262.34316   60.84673
  alf[22] =    5.63856;  dlf[22]=   0.73225; //  250.53870   60.94167
  alf[23] =    5.65529;  dlf[23]=   0.69051; //  255.13626   59.89354
  alf[24] =    5.70480;  dlf[24]=   0.80893; //  241.55833   58.44631

  printf("\n ================== Запущена программа проверки максимального CKO  измерений угловых координат  по п.7.11  БЛ1.513.050И ");
  fprintf(fluft,"\n ================== Запущена программа проверки  максимального CKO  измерений угловых координат  по п.7.11  БЛ1.513.050И ");
 //  fprintf(fsko,"\n ================== Запущена программа проверки  максимального CKO  измерений угловых координат  по п.7.11  БЛ1.513.050И ");
  printf(
 "\n"
 "                                           Дата: %02d/%02d/%4d  Время: %02d:%02d:%02.0f                      \n",rvday,rvmonth,rvyear,rvhour,rvmin,rvsec);

  fprintf(fluft,"\n"
 "                                           Дата: %02d/%02d/%4d  Время: %02d:%02d:%02.0f                      \n",rvday,rvmonth,rvyear,rvhour,rvmin,rvsec);

  printf("\n         Задан номер ТК:%d",NTK);
  
   fprintf(fluft,"\n  Задан TK=%d \n",NTK);
   
   if(TIPST==59)
   {
      can_req6(fcan_vu,CAN_ABN_KVU,CAN_CMD_PUTP,_PEREKLUCH,NTK);   // switch between ctk NTK=1 to SHPK NTK=2 to BIK
      delay_sec(3);
      can_req6(fcan_vu,CAN_ABN_KVU,CAN_CMD_PUTP,45,1); //svf1, svf=46! CAN only
      delay_sec(1);
   }

   if(ntk==2 && TIPST==59)
       kolshag =25;
   else kolshag =5;
  // loop 25 perebrosov
 // for(int k=0;k<25;k++) //
   for(int k=0;k<kolshag;k++) // достаточно
  {
  //  printf ("\n RABTK=%d %d \n",RABTK[1],RABTK[2]);printf("\n pasSX(ntk)=%7.3f pasSY(ntk)=%7.3f  pasbin(ntk)=%7.3f pasF(ntk)=%8.3f \n",pasSX(ntk),pasSY(ntk),pasbin(ntk),pasF(ntk) );
 if(ntk==2&&TIPST==59){   alfa=alf[k]; delta = dlf[k];

    /* J2000Teksea(rvtime, &alfa, &delta);
     adAHsea(alfa,delta,rvtime,&Az,&H);
     printf("\n Az=%6.2f H=%6.2f\n", Az*RG, H*RG);
     if (H<0) { printf("\n     Под горизонтом\n"); continue;}//goto step1; }
     if (H<5*GR) { printf("\n       Низко над горизонтом\n");continue;}// goto step1; }*/
       res = naved_zvezda(NTK, alfa, delta);
     if (res==0) continue;//return 0;
     printf("Выполнено наведение  на звезду шаг %d  Запуск ОМР\n",k); }
 else
 { if (!PUSKCHV(acd[k],lcd[k],rcd[k])) break;
 printf("\n        ШАГ %d переброс по таблице ",k);
 fprintf(fluft,"\n        ШАГ %d переброс по таблице ",k);
       printf ("\n Углы  переброса: A:%6.2f[grad] L:%6.2f[grad] R:%6.2f[grad]\n", acd[k],lcd[k],rcd[k]);
  fprintf (fluft,"\n Углы  переброса: A:%6.2f[grad] L:%6.2f[grad] R:%6.2f[grad]\n", acd[k],lcd[k],rcd[k]);}
        delay_sec(2);     // 2
//        if (!IZMERAPOI(RABTK)) break;

    /*UAK 27.08.13*/
//   if (!IZMERAPOI(RABTK)) break;
   int res,ii;
   res=IZMERAPOI(RABTK);
   if (!res) break;      //!IZMERAPOI
   for(ii=1;ii<=MAXTK;ii++)
   {
     if (RABTK[ii]) 
     {
        if ((TIPST==58)&&(KZVAPOI[ii]<=100))
        {
          printf("\n Плохое измерение АПОИ %d звезд в %d камере. Повторим измерение АПОИ.\n",KZVAPOI[ii],ii);
          delay_sec(5);
          res=IZMERAPOI(RABTK);
          break;
        }
     }
   }
   if (!res) break;      //!IZMERAPOI

       /* for(ii=1;ii<=MAXTK;ii++)
          if (RABTK[ii]) {

             MetodOMR(ii);
             printOMR(ii);
            printforpasp(NTK,i,fluft);//,f[i]);
            }*/
 if (RABTK[NTK]) {
           MetodOMR(NTK);
           printOMR(NTK);
           
           //02.09.13printf("Измерено звезд %d  распознано звезд %d\n", o.izmkzv, o.omrkzv);
           if ((o.omrkzv*1.)/(o.izmkzv*1.)<0.1)
           {
             printf("Повторим ОМР");
             delay_sec(5);
             IZMERAPOI(RABTK);
             MetodOMR(NTK);
             printOMR(NTK);
           }
           
        }
           printf ("\n Переброс закончен в точке:  A:%6.2f[grad] L:%6.2f[grad] R:%6.2f[grad]\n", cd[0],cd[1],cd[2]);
           fprintf (fluft,"\n Переброс закончен в точке:  A:%6.2f[grad] L:%6.2f[grad] R:%6.2f[grad]\n", cd[0],cd[1],cd[2]);
       x=0; y=0;
     //  korrxyOMR(NTK,&x,&y);
      // absmet(NTK,o.A,o.L,o.R,x,y,&Azomr,&Homr);// get Az,H viz osi metodom OMR
       //    analiz_omr ()..
         izmkzv=o.izmkzv;
       for (i=0;i<izmkzv;i++) {// цикл по измеренным каталожным звездам звездам
         j = izmss[i];
         if (j!=-1) {
           double m=catzv[j].v,// зв.величина из каталога
                   V=izmzv[i].e;// объем сигнала от АПОИ
           KK += m + 2.5*log10(V);
      //  printf("\n        **************************  Into  Blesk loop:  m=%lf V=%lf KK=%lf",m,V,KK);
           n++;
         }
       }
       KK/=n;

       n=0;
      if(o.izmkzv<3) continue;// плохие измерения!
       for (i=0;i<o.izmkzv;i++) {
         redomr(red,izmzv[i].x,izmzv[i].y,&xred,&yred);
         mizm = KK - 2.5*log10(izmzv[i].e);
         j = izmss[i];
         if (j!=-1) {
           mcat = catzv[j].v;
           dm = mizm - mcat;
           dx=xred-catzv[j].x;
           dy=yred-catzv[j].y;
//===========================================================================//
 // переход от x,y (мм)    k   alfa,delta

           absmet(ntk, o.A,o.L,o.R, catzv[j].x,catzv[j].y, &Az,&H);
         AHadsea(Az,H,o.time, &alfa, &delta);  //alfa,delta for baykov
         fprintf(fstar,"\n %d %lf %lf %lf",i+number,mcat,alfa,delta)      ;
          // absmet(ntk, o.A,o.L,o.R, izmzv[i].x,izmzv[i].y, &Az,&H);
          absmet(ntk, o.A,o.L,o.R,xred,yred, &Az,&H);//17.03.13y
         AHadsea(Az,H,o.time, &alfa, &delta);  //alfa,delta for baykov
         fprintf(fstar," %lf %lf %lf",mizm,alfa,delta);//  //alfa,delta -измеренных звезд

//===========================================================================//
           r = sqrt(dx*dx+dy*dy);
           if (fabs(dx)>maxdx) maxdx = fabs(dx);
           if (fabs(dy)>maxdy) maxdy = fabs(dy);
           if (r>maxr) maxr = r;
           if (fabs(dm)>maxdm) maxdm = fabs(dm);
           Sx += dx*dx;
           Sy += dy*dy;
           Sr += r*r;
           Sm += dm*dm;
         // printf("\n Текущие Sx=%8.3f   Sy=%8.3f    Sr=%8.3f Sm=%8.3f\n",Sx,Sy,Sr,Sm);
           n++;
             printf("xred=%7.2f yred=%7.2f V=%9.1f mizm=%5.2f mcat=%5.2f dm=%5.2f catzv.x=%6.3f catvz.y=%6.3f\n",
               izmzv[i].x, izmzv[i].y, izmzv[i].e, mizm, mcat, dm, catzv[j].x, catzv[j].y);
            //   printf("izmzv[i].x=%7.2f izmzv[i].y=%7.2f izmzv[i].e=%9.1f mizm=%5.2f mcat=%5.2f dm=%5.2f dx=%6.3f dy=%6.3f\n",
            //        izmzv[i].x, izmzv[i].y, izmzv[i].e, mizm, mcat, dm, dx, dy);
           number++;
         } else {// нет соответствия измеренной звезды каталожной
           //printf("X=%7.2f Y=%7.2f V=%9.1f mizm=%5.2f\n",
             // izmzv[i].x, izmzv[i].y, izmzv[i].e, mizm);
         }
      }// цикл по количеству измеренных звезд
      Sx = sqrt(Sx/n);// средние невязки по количеству измерений по x
      Sy = sqrt(Sy/n);// средние невязки по количеству измерений по y
      Sr = sqrt(Sr/n);// средние невязки по количеству измерений по x,y
      Sm = sqrt(Sm/n);// средние невязки по количеству измерений по e
      
      if (Sr>0.0129)
        Sr=Sr*0.1;

      printf("\n Окончательные на данном шаге: Sx=%8.3f Sy=%8.3f Sr=%8.3f\n",Sx,Sy,Sr);
      printf("------------------------------------------------------------------------\n");
      printf("Результаты ОМР NTK=%d\n",ntk);
      fprintf(fluft,"Результаты ОМР NTK=%d\n",ntk);
      printf("Измерено звезд %d  распознано звезд %d\n", o.izmkzv, o.omrkzv);
      fprintf(fluft,"Измерено звезд %d  распознано звезд %d\n", o.izmkzv, o.omrkzv);
     // printf("СКО невязок по X    %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
     //     Sx, Sx/pasSX(ntk)/pasbin(ntk), Sx/pasF(ntk)*RG*3600);
      // fprintf(fluft,"СКО невязок по X    %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
      //    Sx, Sx/pasSX(ntk)/pasbin(ntk), Sx/pasF(ntk)*RG*3600);
      //printf("СКО невязок по Y    %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
      //    Sy, Sy/pasSY(ntk)/pasbin(ntk), Sy/pasF(ntk)*RG*3600);
      //fprintf(fluft,"СКО невязок по Y    %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
      //    Sy, Sy/pasSY(ntk)/pasbin(ntk), Sy/pasF(ntk)*RG*3600);
      printf("СКО невязок общ.    %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
          Sr, Sr/pasSX(ntk)/pasbin(ntk), Sr/pasF(ntk)*RG*3600);
      fprintf(fluft,"СКО невязок общ.    %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
          Sr, Sr/pasSX(ntk)/pasbin(ntk), Sr/pasF(ntk)*RG*3600);
     /* printf("макс.невязка по X   %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
          maxdx, maxdx/pasSX(ntk)/pasbin(ntk), maxdx/pasF(ntk)*RG*3600);
      fprintf(fluft,"макс.невязка по X   %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
          maxdx, maxdx/pasSX(ntk)/pasbin(ntk), maxdx/pasF(ntk)*RG*3600);
      printf("макс.невязка по Y   %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
          maxdy, maxdy/pasSX(ntk)/pasbin(ntk), maxdy/pasF(ntk)*RG*3600);
      fprintf(fluft,"макс.невязка по Y   %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
          maxdy, maxdy/pasSX(ntk)/pasbin(ntk), maxdy/pasF(ntk)*RG*3600);
      printf("макс.невязка общ.   %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
          maxr, maxr/pasSX(ntk)/pasbin(ntk), maxr/pasF(ntk)*RG*3600);
      fprintf(fluft,"макс.невязка общ.   %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
          maxr, maxr/pasSX(ntk)/pasbin(ntk), maxr/pasF(ntk)*RG*3600);
*/
        printf("СКО измерения  блеска на данном шаге dm:  %5.3f\n", Sm);
          fprintf(fluft,"СКО измерения  блеска на данном шаге dm  %5.3f\n", Sm);
         printf("max dm  %5.3f\n", maxdm);          fprintf(fluft,"max dm  %5.3f\n", maxdm);
     printf("------------------------------------------------------------------------\n");
    KK = 0;//17.03.13y
    if(maxSr<Sr) maxSr = Sr;//17.03.13y
    if(maxSm<Sm) maxSm = Sm;//17.03.13y
    Sr = 0; Sx = 0; Sy = 0; Sm = 0; n =0;
 }
   printf("================================= Окончательные результаты (средствами КПСТ)===========================================\n");
   fprintf(fluft,"================================= Окончательные результаты (средствами КПСТ) ===========================================\n");
   //fprintf(fsko,"================================= Окончательные результаты (средствами КПСТ) ===========================================\n");

   printf("   Максимальная среднеквадратическая ошибка измерений угловых координат  = %5.3f угл.сек.\n",
        maxSr/pasF(ntk)*RG*3600);
   fprintf(fluft,"   Максимальная среднеквадратическая ошибка измерений угловых координат   = %5.3f угл.сек.\n",
        maxSr/pasF(ntk)*RG*3600);

   //fprintf(fsko,"   Максимальная среднеквадратическая ошибка измерений угловых координат   = %5.3f угл.сек.\n",
      //  maxSr/pasF(ntk)*RG*3600);
     printf("Максимальное СКО измерения  блеска  dm:  %5.3f\n", maxSm);
     fprintf(fluft,"Максимальное СКО измерения  блеска  dm:  %5.3f\n", maxSm);
     fprintf(fluft,"\n========================================================================================================\n");
   //   fprintf(fsko,"Максимальное СКО измерения  блеска  dm:  %5.3f\n", maxSm);
     //  fprintf(fsko,"\n========================================================================================================\n");
if(  fstar  ) fclose(  fstar  );
//========================== BAYKOV =======================================//
     if(ntk==2&&TIPST==59) {   can_req6(fcan_vu,CAN_ABN_KVU,CAN_CMD_PUTP,45,3); // svf1, svf=46! CAN only
 //#avm#0.07.2013
                              printf("SVft=3");
             delay_sec(1);}
   // ModKrik();
  ModSTAR();// baykov

//========================== BAYKOV =======================================//

 if(  fluft  ) fclose(  fluft  );
//if(  fstar  ) fclose(  fstar  );
//if(  fsko  ) fclose(  fluft  );
can_req6(fcan_vu,CAN_ABN_KVU,CAN_CMD_PUTP,_PEREKLUCH,1);   // switch ctk to SHPK
delay_sec(3);

}


int blesk(int NTK)
{FILE * fluft;
    char str[100];
    double alfa2000=0, delta2000=0, alfa, delta, Az, H;
    int nstar, ori, res, ntk=1, MAXTK=2;
    double KK=0; int izv=0;
    double max_mizm=10,
            // 0, 20/05/2012
            min_mizm=19;
    int n=0;
    double xred,yred, dx,dy,r, maxdx=0, maxdy=0, maxr=0, Sx=0, Sy=0, Sr=0;
    double mcat, mizm, dm, maxdm=0, Sm=0;double x=0,y=0; int i,j;
   FILE*fstar; FILE*f55;
    if(NTK==1){//max_mizm = 2.637;
     fstar = fopen("tmpdan/DataBlesk1.txt","at"); //open file-protokol
     fluft  = fopen("tmpdan/diapblesk1.txt","at"); //open file-protokol
               }
    else if (NTK==2) {// max_mizm = 2.739;
     fstar = fopen("tmpdan/DataBlesk2.txt","at"); //open file-protokol
     fluft  = fopen("tmpdan/diapblesk2.txt","at"); //open file-protokol
               }
     else {printf("\n Неверно задан номер ТК!");return -1;}

    printf("\n ================== Запущена программа проверки диапазона блеска обнаруживаемых космических объектов  по п.7.9  БЛ1.513.050И ============================");
    fprintf(fluft,"\n ================== Запущена программа проверки диапазона блеска обнаруживаемых космических объекто  по п.7.9  БЛ1.513.050И ============================");
    fprintf(fstar,"\n ================== Запущена программа проверки диапазона блеска обнаруживаемых космических объекто  по п.7.9  БЛ1.513.050И ============================");
    printf(
   "\n"
   "                                           Дата: %02d/%02d/%4d  Время: %02d:%02d:%02.0f                      \n",rvday,rvmonth,rvyear,rvhour,rvmin,rvsec);

    fprintf(fluft,"\n"
   "                                           Дата: %02d/%02d/%4d  Время: %02d:%02d:%02.0f                      \n",rvday,rvmonth,rvyear,rvhour,rvmin,rvsec);
    printf("\n         Задан номер ТК:%d",NTK);
    fprintf(fstar,"\n"
   "                                           Дата: %02d/%02d/%4d  Время: %02d:%02d:%02.0f                      \n",rvday,rvmonth,rvyear,rvhour,rvmin,rvsec);

    //=========================================================================================//
     CATSTAR  star;
     double minmag=5.45, maxmag=5.85 ; char s[80];
     double A=0, L=0, R=0;
     f55 = fopen("tmpdan/outputzkt.txt","at");
     startreadcircle(zkt,0,0,4);

  while(1) {
    if (!readstar(zkt,star)) break;
  if(star.mag>=minmag &&star.mag<=maxmag)
    { //alfa2000=4.294607;delta2000=1.356036;// mag 15.37 from TYCHO2 ! ! !//
      alfa2000 = star.alfa ; delta2000 = star.delta ;
      alfa=alfa2000; delta=delta2000;
      J2000Teksea(rvtime, &alfa, &delta);

      for(i=0; i<3; i++) {
        adAHsea(alfa,delta,rvtime,&Az,&H);
        if (H<5*GR) goto bad;
        PlusRefraction(&H);
        Naved(ntk, Az, H, POLUS, 90, 0, 0, &A, &L, &R); //ориентация "север сверху"
        if (L<MINCD(OS_L)+0.5 || L>MAXCD(OS_L)-0.5 ||   //если не удалось, то произвольная
            R<MINCD(OS_R)+0.5 || R>MAXCD(OS_R)-0.5) {
          if (!NavedShort(ntk,Az,H, 0,0, &A,&L,&R)) goto bad;
        }


     //printf("\n Подходящая звезда: N=%ld alfa=%lf delta=%lf mag=%lf sp=%c%c\n",star.N,star.alfa,star.delta,star.mag,star.sp[0],star.sp[1]);
        fprintf(f55,"\n Подходящая звезда: N=%ld alfa=%lf delta=%lf mag=%lf sp=%c%c\n",star.N,star.alfa,star.delta,star.mag,star.sp[0],star.sp[1]);
       bad: ;//printf("\n Наведение невозможно");
    }
  }
 }
if(f55) fclose(f55);
 //========================================================================================//

can_req6(fcan_vu,CAN_ABN_KVU,CAN_CMD_PUTP,_PEREKLUCH,NTK);   // switch between ctk NTK=1 to SHPK NTK=2 to BIK
delay_sec(3);

// for law brigthes stars
if(TIPST==59 && NTK==1)  //SHPK
{
     can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('E','T'), 2000);
     usleep(200);
     can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','S'), 255);
     usleep(200);
     can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','X'), 1);
     usleep(200);
     can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','Y'), 1);
     usleep(200);
     can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('T','R'), 0);
     usleep(200);
     can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('K','A'),3846 ); //analog amplifier
     usleep(200000);
}
else if(TIPST==59 && NTK==2)  //BIK
{
    can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('E','T'), 400);
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','S'), 255);
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','X'), 1);
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','Y'), 1);
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('T','R'), 2);
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('K','A'),3846 ); //analog amplifier 15 db
    usleep(200);
    can_req2(fcan_ctk,CAN_ABN_CTK(NTK),CAN_CMD_EXTRA,0x8011); // EM on
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('E','M'),10240 ); //EM amplifier 40db

    usleep(100000);
}
can_req6(fcan_vu,CAN_ABN_KVU,CAN_CMD_PUTP,45,1); //svf1, svf=46! CAN only
delay_sec(1);

     fprintf(fluft,"\n  Задан TK=%d \n",NTK);
     OMRDATA &o=omrdata[NTK];
int izmkzv=o.izmkzv, catkzv=o.catkzv;
    CATZV *catzv=o.catzv; IZMZV *izmzv=o.izmzv;
    int *catss=o.catss, *izmss=o.izmss;
    REDOMR &red=o.red;
printf("\n Шаг 0: Измерение слабых звезд из списка tmpdan/mynp.txt\n");
    min_mizm = my_nabl_polus(NTK);
    if(min_mizm<=17)  min_mizm = max(my_nabl_polus(NTK),min_mizm);//  повтор
    if(min_mizm<=17)  min_mizm = max(my_nabl_polus(NTK),min_mizm);//  повтор
 //   if(min_mizm==0)  min_mizm = my_nabl_polus(NTK);//  повтор
    //if(min_mizm==0||min_mizm==1) min_mizm=15.373;
           if(min_mizm<15) min_mizm=15.37;
  //if(min_mizm==0) min_mizm=19;// не было измерено ни одной звезды
    double alf[25] ={0}; double dlf[25] = {0};// массив alfa,delta звезд из списка
    //  7 46 13.24  89  36  55.3  14.20     16.38    1318
    //  6  5 50.29  89  33  58.5  15.21     16.74    1241
    //  7 15  0.77  89  33  48.9  15.17     16.57    1005
    //  9 30 37.54  89  33   1.3   0.00     16.53    1183
    //  9 16 10.56  89  30  52.6  15.05     16.40     921
    //  8 27  5.72  89  31  14.8  14.41     15.80     772
    //  5 17 40.25  89  29  20.9  15.35     16.72    1151
    //  6 43 20.08  89  29  20.3  15.46     16.75     634
    //  5 31  2.55  89  29   6.7  15.48     16.76    1033
    //  5 22 51.62  89  28  11.9  15.02     16.38    1017
    //  8 57 59.90  89  26  35.8  14.69     15.96     404
    //  8  1 56.50  89  26  27.5  14.73     16.53     246
    //  6 24 53.33  89  26  21.7  14.71     16.33     416
    //  5  9 23.06  89  26  16.9  15.27     16.41     985
    //  5 17 39.04  89  22  18.0  15.57     16.26     573
    //  5 26 17.54  89  16  52.7  15.01     16.28      60
    //alfa = hmshour(7 ,46, 13.24 );delta=gmsgrad(89 , 36,  55.3 );alf[0]=alfa;dlf[0]=delta;//printf("\n N1318   alfa=%8.2f delta=%7.2f",alfa,delta);
    //alfa = hmshour(6 , 5 ,50.29 );delta=gmsgrad(89 , 33 , 58.5 );alf[1]=alfa;dlf[1]=delta;//printf("\n N1241   alfa=%8.2f delta=%7.2f",alfa,delta);//11.06.13!!
    alfa = hmshour(7, 15 , 0.77);delta=gmsgrad( 89 , 33 , 48.9 );alf[2]=alfa;dlf[2]=delta;//printf("\n N1005   alfa=%8.2f delta=%7.2f",alfa,delta);
    alfa = hmshour(9 ,30, 37.54 );delta=gmsgrad(89  ,33  , 1.3 );alf[3]=alfa;dlf[3]=delta;//printf("\n N1183   alfa=%8.2f delta=%7.2f",alfa,delta);
    alfa = hmshour(9 ,16, 10.56 );delta=gmsgrad(89 , 30 ,52.6  );alf[4]=alfa;dlf[4]=delta;//printf("\n N 921   alfa=%8.2f delta=%7.2f",alfa,delta);
    alfa = hmshour(8 ,27, 5.72 );delta=gmsgrad(89 , 31, 14.8  );alf[5]=alfa;dlf[5]=delta;//printf("\n N772   alfa=%8.2f delta=%7.2f",alfa,delta);
    alfa = hmshour(5 ,17,40.25 );delta=gmsgrad(89 ,29 , 20.9 );alf[6]=alfa;dlf[6]=delta;//printf("\n N 1151   alfa=%8.2f delta=%7.2f",alfa,delta);
    alfa = hmshour( 6 ,43,20.08 );delta=gmsgrad(89 , 29 , 20.3 );alf[7]=alfa;dlf[7]=delta;//printf("\n N634   alfa=%8.2f delta=%7.2f",alfa,delta);

    alfa = hmshour( 5 ,31, 2.55 );delta=gmsgrad(89 , 29 ,  6.7 );alf[8]=alfa;dlf[8]=delta;//printf("\n N1033   alfa=%8.2f delta=%7.2f",alfa,delta);
    alfa = hmshour(5 ,22, 51.62 );delta=gmsgrad( 89 , 28, 11.9); alf[9]=alfa;dlf[9]=delta; //printf("\n N1017   alfa=%8.2f delta=%7.2f",alfa,delta);
    alfa =hmshour(8, 57 ,59.90);delta=gmsgrad(89 , 26,  35.8 );alf[15]=alfa;dlf[15]=delta; //printf("\n N404   alfa=%8.2f delta=%7.2f",alfa,delta);
    alfa = hmshour(8 , 1,56.50 );delta=gmsgrad(89 , 26 ,27.5 );alf[14]=alfa;dlf[14]=delta;//printf("\n N246   alfa=%8.2f delta=%7.2f",alfa,delta);
    alfa = hmshour(6, 24, 53.33);delta=gmsgrad(89  ,26 ,21.7 );alf[12]=alfa;dlf[12]=delta;//printf("\n N 416   alfa=%8.2f delta=%7.2f",alfa,delta);
    alfa = hmshour(5, 9, 23.06 );delta=gmsgrad(89  ,26,  16.9 );alf[13]=alfa;dlf[13]=delta;//printf("\n N 985   alfa=%8.2f delta=%7.2f",alfa,delta);
    alfa = hmshour(5 ,17, 39.04);delta=gmsgrad(89 ,22, 18.0 );alf[11]=alfa;dlf[11]=delta;//printf("\n N 573   alfa=%7.2f delta=%7.2f",alfa,delta);

/////////////// 11.06.13
   // alfa = hmshour(5 ,26,17.54);delta=gmsgrad(89 ,16, 52.7  );alf[1]=alfa;dlf[1]=delta;//printf("\n N 60   alfa=%7.2f delta=%7.2f",alfa,delta); 15.01-16.8


  // alf[0]=4.294607;dlf[0]=1.356036;// mag 15.37 from TYCHO2 ! ! !//
    alf[0]=0.315382; dlf[0]=1.390573;// mag= 5.586
    alf[1]=5.42181;  dlf[1]=1.44044;  // mag= 5.738
    if(NTK==2){
     alf[0]= hmshour(20 ,41, 25.9); dlf[0]=gmsgrad(45 ,16, 49 );//DENEB mag = 1.25
     alf[1]= hmshour(13 ,45, 8.17); dlf[1]=gmsgrad(-16 ,9, 41 );//SPICA mag =1.04
    }
      //alfa=alfa2000; delta=delta2000;
     if(NTK==2){   can_req6(fcan_vu,CAN_ABN_KVU,CAN_CMD_PUTP,45,3); //svf1, svf=46! CAN only
                   delay_sec(1);
                 }
     //else {        can_req6(fcan_vu,CAN_ABN_KVU,CAN_CMD_PUTP,45,2); //svf1, svf=46! CAN only
     //              delay_sec(1);
     //           }
     // for high brigthes stars
     if(TIPST==59 && NTK==1)  //SHPK
     {
          can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('E','T'), 600);
          usleep(200);
          can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','S'), 255);
          usleep(200);
          can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','X'), 1);
          usleep(200);
          can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','Y'), 1);
          usleep(200);
          can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('T','R'), 1);
          usleep(200);
          can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('K','A'),3846 ); //analog amplifier 15 db
          usleep(2000000);
     }
     else if(TIPST==59 && NTK==2)  //BIK
     {
         can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('E','T'), 20);
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','S'), 255);
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','X'), 1);
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','Y'), 1);
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('T','R'), 2);
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('K','A'),845 ); //analog amplifier 3.3db
         usleep(2000000);
         can_req2(fcan_ctk,CAN_ABN_CTK(NTK),CAN_CMD_EXTRA,0x8111); // EM off
         usleep(10000);
     }

    for(izv=0;izv<2;izv++)// 21.03 они уже измерены
    {  printf("\n Шаг %d:      Наведение на звезды из списка   ",izv+1); //
        alfa2000=alf[izv]; delta2000 = dlf[izv];
    alfa=alfa2000; delta=delta2000;// 11.06.13
    J2000Teksea(rvtime, &alfa, &delta);
    adAHsea(alfa,delta,rvtime,&Az,&H);
    printf("\n Az=%6.2f H=%6.2f\n", Az*RG, H*RG);
    if (H<0) { printf("\n     Под горизонтом\n"); continue;}//goto step1; }
    if (H<5*GR) { printf("\n       Низко над горизонтом\n");continue;}// goto step1; }
      res = naved_zvezda(NTK, alfa2000, delta2000);
    if (res==0) continue;//return 0;
    printf("Выполнено наведение  на звезду шаг 1-2  Запуск ОМР\n");
    // omr
    delay_sec(2);     // 2
    if (!IZMERAPOI(RABTK)) printf("\n Плохое измерение в IZMERAPOI!!!");

    if (RABTK[NTK]) {
         MetodOMR(NTK);
         printOMR(NTK);
           }
    x=0; y=0;
    izmkzv=o.izmkzv; catkzv=o.catkzv;
     //    analiz_omr ()..
    for (i=0;i<izmkzv;i++) {// цикл по измеренным каталожным звездам звездам
      j = izmss[i];
      if (j!=-1) {
        double m=catzv[j].v,// зв.величина из каталога
                V=izmzv[i].e;// объем сигнала от АПОИ
        KK += m + 2.5*log10(V);
        n++;
      }
    }
    KK/=n;

    n=0;
   // if(o.izmkzv<5) continue;// плохие измерения!
    for (i=0;i<o.izmkzv;i++) {// цикл по измеренным каталожным звездам
      redomr(red,izmzv[i].x,izmzv[i].y,&xred,&yred);
      mizm = KK - 2.5*log10(izmzv[i].e);
//      if(NTK==2) mizm = mizm -5;// wtih 100  svetofilter

        if(mizm>min_mizm )min_mizm = mizm;//max слабая
        if(mizm<max_mizm) max_mizm = mizm;// max яркая
      j = izmss[i];
      if (j!=-1) {
        mcat = catzv[j].v;
        dm = mizm - mcat;
        dx=xred-catzv[j].x;    dy=yred-catzv[j].y;   r = sqrt(dx*dx+dy*dy);
        if (fabs(dx)>maxdx) maxdx = fabs(dx);
        if (fabs(dy)>maxdy) maxdy = fabs(dy);
        if (r>maxr) maxr = r;
        if (fabs(dm)>maxdm) maxdm = fabs(dm);

        Sm += dm*dm;
        n++;
        printf("\n Звезда N:%ld  измеренный блеск:%lf     блеск по каталогу:%lf",catzv[j].N, mizm ,mcat);
       fprintf(fstar,"\n Звезда N:%ld  измеренный блеск:%lf     блеск по каталогу:%lf",catzv[j].N, mizm ,mcat);
       
      } else {// нет соответствия измеренной звезды каталожной
        printf("\n Не каталожная звезда X=%7.2f Y=%7.2f V=%9.1f измеренный блеск=%5.2f\n",
           izmzv[i].x, izmzv[i].y, izmzv[i].e, mizm);
      }
   }// цикл по количеству измеренных звезд
step1:
mcat =0; mizm=0;dm=0; Sm = 0; n=0;KK=0;

    } //loop by alf[],dlf[]
    if(NTK==2)
    {
        can_req6(fcan_vu,CAN_ABN_KVU,CAN_CMD_PUTP,45,2); //svf1, svf=46! CAN only
//#avm#08072013
         delay_sec(1);
     }
    else{
           can_req6(fcan_vu,CAN_ABN_KVU,CAN_CMD_PUTP,45,2); //svf1, svf=46! CAN only
      //#avm#08072013
            printf("\nSVft=2\n");
            delay_sec(1);
          }
    // for high brigthes stars
    if(TIPST==59 && NTK==1)  //SHPK
    {
         can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('E','T'), 50);
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','S'), 255);
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','X'), 0);
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','Y'), 0);
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('T','R'), 1);
         usleep(200);
         can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('K','A'),3846 ); //analog amplifier 15 db
         usleep(2000000);
    }
 //////////////////////////////////////////////////////////////////
    // step 3 : mag =2.739
//    can_req6(fcan_vu,CAN_ABN_KVU,CAN_CMD_PUTP,45,4); //svf1, svf=46! CAN only
//    delay_sec(1);
 printf("\n  Шаг 3:      Наведение на звезду из каталога  ");

//alfa2000 =2.47656; delta = -0.1511; //mag =1.98 rez=6.5 - 15.67 //
//alfa2000 =0.66223; delta = 1.557913; //mag =1.97  rez =6.55 -15.73
//alfa2000 =2.80543; delta = -1.123896; //mag =2.739
// alfa2000 =6.24875; delta = -1.431498; //mag =5.112 26.06!
//alfa2000 =0.662293; delta = 1.557953; //mag =1.97 26.06! под горизонтом!
  alfa2000 = 0.662293; delta2000 = 1.557953; //N=23746281 mag=1.977
  if(NTK==2)  alfa2000 = 0.662293; delta2000 = 1.557953; //N=23746281 mag=1.977   BIK!!!
    alfa=alfa2000; delta=delta2000;
    J2000Teksea(rvtime, &alfa, &delta);
    adAHsea(alfa,delta,rvtime,&Az,&H);
    printf("\n   Az=%6.2f H=%6.2f\n", Az*RG, H*RG);
    if (H<0) { printf("Под горизонтом\n");goto step2;}// return 0; }
    if (H<5*GR) { printf("Низко над горизонтом\n");goto step2;}// return 0; }
      res = naved_zvezda(NTK, alfa2000, delta2000);
    if (res==0) return 0;
  printf("Выполнено наведение  на звезду  alfa=%7.3f delta =  Запуск ОМР\n",alfa2000,delta2000);
    //omr

    delay_sec(2);     // 2
    if (!IZMERAPOI(RABTK)) printf("\n Плохое измерение в IZMERAPOI!!!");

if (RABTK[NTK]) {
         MetodOMR(NTK);
         printOMR(NTK);
           }
    x=0; y=0;     izmkzv=o.izmkzv; catkzv=o.catkzv;
     //    analiz_omr ()..
    for (i=0;i<izmkzv;i++) {// цикл по измеренным каталожным звездам звездам
      j = izmss[i];
      if (j!=-1) {
        double m=catzv[j].v,// зв.величина из каталога
                V=izmzv[i].e;// объем сигнала от АПОИ
        KK += m + 2.5*log10(V);
        n++;
      }
    }
    KK/=n;    n=0;
       for (i=0;i<o.izmkzv;i++) {
      redomr(red,izmzv[i].x,izmzv[i].y,&xred,&yred);
      mizm = KK - 2.5*log10(izmzv[i].e);
      if(NTK==2) mizm = mizm - 5 ;// with svetofilter !!!
        if(mizm>min_mizm )min_mizm = mizm;//max слабая
        if(mizm<max_mizm) max_mizm = mizm;// max яркая
      j = izmss[i];
      if (j!=-1) {
        mcat = catzv[j].v;
        dm = mizm - mcat;
         Sm += dm*dm;
        n++;
        
         printf("\n Звезда N:%ld  измеренный блеск:%lf     блеск по каталогу:%lf",catzv[j].N, mizm ,mcat);
         fprintf(fstar,"\n Звезда N:%ld  измеренный блеск:%lf     блеск по каталогу:%lf",catzv[j].N, mizm ,mcat);
           } else {// нет соответствия измеренной звезды каталожной
            printf("\n Не каталожная звезда: X=%7.2f Y=%7.2f V=%9.1f  измеренный блеск=%5.2f\n",
            izmzv[i].x, izmzv[i].y, izmzv[i].e, mizm);
      }
   }// цикл по количеству измеренных звезд
step2:mcat =0; mizm=0;dm=0; Sm = 0; n=0;KK=0;
  can_req6(fcan_vu,CAN_ABN_KVU,CAN_CMD_PUTP,45,3); //svf1, svf=46! CAN only
delay_sec(1);
// for high brigthes stars
if(TIPST==59 && NTK==1)  //SHPK
{
     can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('E','T'), 50);
     usleep(200);
     can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','S'), 255);
     usleep(200);
     can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','X'), 0);
     usleep(200);
     can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','Y'), 0);
     usleep(200);
     can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('T','R'), 1);
     usleep(200);
     can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('K','A'),3846 ); //analog amplifier 15 db
     usleep(2000000);
}
else if(TIPST==59 && NTK==2)  //BIK
{
    can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('E','T'), 20);
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','S'), 255);
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','X'), 1);
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','Y'), 1);
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('T','R'), 2);
    usleep(200);
    can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('K','A'),845 ); //analog amplifier 3.3db
    usleep(2000000);
    can_req2(fcan_ctk,CAN_ABN_CTK(NTK),CAN_CMD_EXTRA,0x8111); // EM off
    usleep(10000);
}
   /////////////////////////////////////////////////////////////////////////////
    // step 4  крупные
               
        // alfa2000 =5.536101; delta2000 = -1.552584; //mag = 5.447//11.06.13!
        // alfa2000 =4.951436; delta2000 = -1.529010; //mag = 5.039 //11.06.13!
      // alfa2000 =4.347511; delta2000 = -0.748028; //N=161778621 mag=5.483000 11.06.13y!
//alfa2000 = 2.476567; delta2000 = -0.151121; //N=159254601 mag=1.982000
  alfa2000 = 0.662293; delta2000 = 1.557953; //N=23746281 mag=1.977

printf("\n Шаг 4:       наведение на звезду из списка каталога      ");
    alfa=alfa2000; delta=delta2000;
    J2000Teksea(rvtime, &alfa, &delta);
    adAHsea(alfa,delta,rvtime,&Az,&H);
    printf("Az=%6.2f H=%6.2f\n", Az*RG, H*RG);
    if (H<0) { printf("Под горизонтом\n"); goto step3;}//return 0; }
    if (H<5*GR) { printf("Низко над горизонтом\n"); goto step3;}//return 0; }
      res = naved_zvezda(NTK, alfa2000, delta2000);
    if (res==0) return 0;
   printf("Выполнено наведение  на звезду  alfa=%7.3f delta = %7.3f Запуск ОМР\n",alfa2000,delta2000);
   goto izmer;
    //omr
 step3:
   // alfa2000 =4.4011; delta2000 = -1.204761; //mag = 1.895 rez= 4-15.4
   alfa2000 =4.951436; delta2000 = -1.529010; //mag = 5.285
  //  alfa2000 =3.162425; delta2000 = -1.335510; //mag = 5.039
    J2000Teksea(rvtime, &alfa, &delta);
    adAHsea(alfa,delta,rvtime,&Az,&H);
    printf("Az=%6.2f H=%6.2f\n", Az*RG, H*RG);
     res = naved_zvezda(NTK, alfa2000, delta2000);
      printf("Выполнено наведение  на звезду  alfa=%7.3f delta =  Запуск ОМР\n",alfa2000,delta2000);

 izmer:
    delay_sec(2);     // 2
    if (!IZMERAPOI(RABTK)) printf("\n Плохое измерение в IZMERAPOI!!!");

if (RABTK[NTK]) {
         MetodOMR(NTK);
         printOMR(NTK);
           }
    x=0; y=0;     izmkzv=o.izmkzv; catkzv=o.catkzv;
     //    analiz_omr ()..
    for (i=0;i<izmkzv;i++) {// цикл по измеренным каталожным звездам звездам
      j = izmss[i];
      if (j!=-1) {
        double m=catzv[j].v,// зв.величина из каталога
                V=izmzv[i].e;// объем сигнала от АПОИ
        KK += m + 2.5*log10(V);
        n++;
      }
    }
    KK/=n;    n=0;
       for (i=0;i<o.izmkzv;i++) {
           mizm = KK - 2.5*log10(izmzv[i].e);

        if(mizm>min_mizm )min_mizm = mizm;//max слабая
        if(mizm<max_mizm) max_mizm = mizm;// max яркая
      j = izmss[i];
      if (j!=-1) {
        mcat = catzv[j].v;
        dm = mizm - mcat;
         Sm += dm*dm;
        n++;
        
               printf("\n Звезда N:%ld  измеренный блеск:%lf     блеск по каталогу:%lf",catzv[j].N, mizm ,mcat);
         fprintf(fstar,"\n Звезда N:%ld  измеренный блеск:%lf     блеск по каталогу:%lf",catzv[j].N, mizm ,mcat);
           } else {// нет соответствия измеренной звезды каталожной
            printf("\n Не каталожная звезда: X=%7.2f Y=%7.2f V=%9.1f измеренный блеск=%5.2f\n",
            izmzv[i].x, izmzv[i].y, izmzv[i].e, mizm);
      }

   }// цикл по количеству измеренных звезд

mcat =0; mizm=0;dm=0; Sm = 0; n=0;KK=0;


   /////////////////////////////////////////////////////////////////////////////
//..Шаг 5:           mag=15.37!!!

  double va,vl,vr;
    //  alfa2000=hmshour( 8, 45, 53); delta2000=gmsgrad(88 ,46, 15);//0.9.06.13!!
    alfa2000=4.294607; delta2000 = 1.356036; // mag=15.37!!!
    printf("\n  Шаг 5:      наведение на звезду из каталога tycho2   alfa: %6.2f   delta:%6.2f\n", alfa2000,delta2000);
    alfa=alfa2000; delta=delta2000;
   // J2000Teksea(rvtime, &alfa, &delta);
    adAHsea(alfa,delta,rvtime,&Az,&H);
    printf("Az=%6.2f H=%6.2f\n", Az*RG, H*RG);
    if (H<0) { printf("Под горизонтом\n");goto step4;}// return 0; }
    if (H<5*GR) { printf("Низко над горизонтом\n");goto step4;}// return 0; }
      res = naved_zvezda(NTK, alfa2000, delta2000);
// или так : NavedShort(ntk,Az,H,0,0,&A,&L,&R)) ;docum_perebroscsn(A,L,R);
    if (res==0) return 0;
   printf("Выполнено наведение  на звезду  alfa=%7.3f delta =  Запуск ОМР\n",alfa2000,delta2000);
    //omr
       can_req6(fcan_vu,CAN_ABN_KVU,CAN_CMD_PUTP,45,1); //svf1, svf=46! CAN only
       delay_sec(1);
   // for law brigthes stars
   if(TIPST==59 && NTK==1)  //SHPK
   {
        can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('E','T'), 1000);
        usleep(200);
        can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','S'), 255);
        usleep(200);
        can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','X'), 1);
        usleep(200);
        can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','Y'), 1);
        usleep(200);
        can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('T','R'), 0);
        usleep(200);
        can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('K','A'),3846 ); //analog amplifier
        usleep(200000);
   }
   else if(TIPST==59 && NTK==2)  //BIK
   {
       can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('E','T'), 400);
       usleep(200);
       can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','S'), 255);
       usleep(200);
       can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','X'), 1);
       usleep(200);
       can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('B','Y'), 1);
       usleep(200);
       can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('T','R'), 2);
       usleep(200);
       can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('K','A'),3846 ); //analog amplifier 15 db
       usleep(200);
       can_req2(fcan_ctk,CAN_ABN_CTK(NTK),CAN_CMD_EXTRA,0x8011); // EM on
       usleep(200);
       can_req6(fcan_ctk, CAN_ABN_CTK(NTK), CAN_CMD_PUTP,  m2b('E','M'),10240 ); //EM amplifier 40db

       usleep(100000);
   }

    delay_sec(2);     // 2
       skorchvcsn(_cdf(OS_A),_cdf(OS_L),_cdf(OS_R),&va,&vl,&vr);//только расчет скоростей
        skorcsn(va,vl,vr);// запускс скоростей
    if (!IZMERAPOI(RABTK)) printf("\n Плохое измерение в IZMERAPOI!!!");

if (RABTK[NTK]) {
         MetodOMR(NTK);
         printOMR(NTK);
           }
    x=0; y=0;     izmkzv=o.izmkzv; catkzv=o.catkzv;
     //    analiz_omr ()..
    for (i=0;i<izmkzv;i++) {// цикл по измеренным каталожным звездам звездам
      j = izmss[i];
      if (j!=-1) {
        double m=catzv[j].v,// зв.величина из каталога
                V=izmzv[i].e;// объем сигнала от АПОИ
        KK += m + 2.5*log10(V);
        n++;
      }
    }
    KK/=n;    n=0;
       for (i=0;i<o.izmkzv;i++) {
      redomr(red,izmzv[i].x,izmzv[i].y,&xred,&yred);
      mizm = KK - 2.5*log10(izmzv[i].e);

        if(mizm>min_mizm )min_mizm = mizm;//max слабая 20.05.13
        if(mizm<max_mizm) max_mizm = mizm;// max яркая
      j = izmss[i];
      if (j!=-1) {
        mcat = catzv[j].v;
        dm = mizm - mcat;
         Sm += dm*dm;
        n++;

        printf("\n Звезда N:%ld  измеренный блеск:%lf     блеск по каталогу:%lf",catzv[j].N, mizm ,mcat);
         fprintf(fstar,"\n Звезда N:%ld  измеренный блеск:%lf     блеск по каталогу:%lf",catzv[j].N, mizm ,mcat);
           } else {// нет соответствия измеренной звезды каталожной
            printf("\n Не каталожная звезда:X=%7.2f Y=%7.2f V=%9.1f измеренный блеск=%5.2f\n",
            izmzv[i].x, izmzv[i].y, izmzv[i].e, mizm);
      }
   }// цикл по количеству измеренных звезд
       mcat =0; mizm=0;dm=0; Sm = 0; n=0;KK=0;
       step4:;



 // }
   /////////////////////////////////////////////////////////////////////////////
       printf("\n===================  Окончательные результаты =====================================");
       fprintf(fluft,"\n===================  Окончательные результаты =====================================");
        fprintf(fstar,"\n===================  Окончательные результаты =====================================");
       printf("\n Диапазон блеска измеренных звезд от %7.3f  до %7.3f", max_mizm, min_mizm );// 20/05/13
       fprintf(fluft,"\n Диапазон блеска измеренных звезд от %7.3f  до %7.3f",  max_mizm, min_mizm);// 20/05/13
       fprintf(fstar,"\n Диапазон блеска измеренных звезд от %7.3f  до %7.3f",  max_mizm, min_mizm);// 20/05/13
        printf("\n=========================================================================================\n");
        fprintf(fluft,"\n=========================================================================================\n");
if(  fluft  ) fclose(  fluft  );if(  fstar  ) fclose(  fstar  );
//can_req6(fcan_vu,CAN_ABN_KVU,CAN_CMD_PUTP,_PEREKLUCH,1);   // switch between ctk to SHPK
//delay_sec(3);
}




void poiskv(void)
{
FILE * fluft;
    // extern int nt ;//число установок телескопа  n  в поисковых возможностях
   //  extern int nj ;//число строк j  в поисковых возможностях
   //#define epsRt(j,i) expRRN[j-1][i-1] //ЦДR вдоль j-строки ,i-я
                    // установка телескопа
   //#define epsL(j) expL[j-1] //ЦД по L вдоль j-ой сканируемой строки
   int i=0,j=0, nj =0, qdug , nt =0, qpoley;
   double  ugoldugiA[28]={0},ugoldugiL[28]={0},ugoldugiR[28]={0},
                   epsA[6]={0},
                   epsL[6]={0},
                   epsR[6]={0};
   double epsRt [ 28 ][ 28 ] = {{0},{0}}; double va,vl,vr;
     //if(NST==5)
   double lL = 1.28012,//[grad]
          lR = 38.403/60.0 ,//[grad]
           dL = 2.5 / 60.0 , //перекрытие полей зрения вдоль дуги
           dR =  25.0 / 3600.0,//перекрытие полей зрения вдолб сканируемой строки
           L=90,
           PV=0,// Поисковые возможности
           perR,
           dlR,//среднее перекрытие плдей зрения вдоль сканируемой строки
            s=0.0,
            t_chv,
           delL=0,
           delR=0
           ;//perekrytie
   printf("\n ================= Запущена проверка скорости обзора по п. 7.10  'Инструкция по проверке БЛ1.513.050 И'===============");
   fluft  = fopen("tmpdan/poiskv.txt","at"); //open file-protokol
  //fprintf(fluft,"\n\n            ПОИСКОВЫЕ    ВОЗМОЖНОСТИ   \n\n  ");
   fprintf(fluft,"\n\n ================= Запущена проверка скорости обзора по п. 7.10  'Инструкция по проверке БЛ1.513.050 И'===============");
   printf(
  "\n"
  "                                           Дата: %02d/%02d/%4d  Время: %02d:%02d:%02.0f                      \n",rvday,rvmonth,rvyear,rvhour,rvmin,rvsec);

   fprintf(fluft,"\n"
  "                                           Дата: %02d/%02d/%4d  Время: %02d:%02d:%02.0f                      \n",rvday,rvmonth,rvyear,rvhour,rvmin,rvsec);

   printf("\nВведите число сканируемых строк по оси L (максимум 5): ");
   scanf("%d",&qdug);
   fprintf(fluft,"\n       Задано  число  сканируемых  строк по оси L : %d",qdug);
   printf("\nВведите число полей зрения по оси R (максимум 10): ");

   scanf("%d",&qpoley);
   fprintf(fluft,"\n       Задано  число  полей   зрения  по   оси  R : %d",qpoley);
     nj = qdug ; nt = qpoley ;
double initR = 90 - (nt - 1)*(lR-dR) ;// то что было раньше 79.56- начальное положение оси R
double initL = 90 - (nj - 1)*(lL-dL) ;// - начальное положение оси L
      //step -> j in GUIDE  and ii -> i   in GUIDE
    for(j=1;j<=nj;j++)
    { ugoldugiA[j] =0.0;
      ugoldugiL[j] =90.0-(5-j)*(lL - dL) ; // 2.34;
    }
    for(i=1;i<=nt;i++)
     ugoldugiR[i] = 90.0- (10-i) * (lR - dR) ; // 1.16;

   int jstroka = 1 ;long beg_find = nomertakta ;
for(j=1;j<=nj;j++)
     {     perR = ( lR - dR ) ;   t_chv = 3.0 ;
           dlR = perR*nt ;
            //A = ugoldugiA[NST][j];
       L = ugoldugiL[j];
          //R = ugoldugiR[NST][j];
          printf("\n");
          printf("\r     Выполняем переброс на строку %d углы : 0.00 %7.2f  %7.2f\n", j,L,initR);
    fprintf(fluft,"\n    Выполняем переброс на строку %d углы : 0.00 %7.2f  %7.2f", j,L,initR);

       docum_perebroscsn(0.0,L,initR);//

       epsL [ j ] = cd[1] ;//_cdf(1);//
        for(i=1;i<=nt;i++)
        {epsRt[j][i] = cd[2] ;//_cdf(2)

            printf("\r             Сканируем поле зрения %d                \n",i);
           fprintf(fluft,"\n       Сканируем    поле    зрения   %d \n",i);


//    skorchvcsn(_cdf(0),_cdf(1),_cdf(2),&va,&vl,&vr);
    skorchvcsn(cd[0],cd[1],cd[2],&va,&vl,&vr);
    printf("Скорости отработки ЧВ: va=%7.5f vl=%7.5f vr=%7.5f (град/с)\n", va,vl,vr);
    long spda=(long)round(va*3600./0.3618),spdl=(long)round(vl*3600./0.3618),spdr=(long)round(vr*3600./0.3618);
    if(spda>=8) spda++;if(spdl>=8) spdl++;if(spdr>=8) spdr++;
     can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUSK,m2b('W',' '), spda);usleep(200);
     can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUSK,m2b('W',' '), spdl);usleep(200);
     can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUSK,m2b('W',' '), spdr);//usleep(200);
   // docum_skorcsn(va,vl,vr);
   // printf("\n  get out from docum_chvcsn");
  // delaytakt(25);  // 2.5 sek
    //delaytakt(10);
    delay_sec(1);
            // delay.tv_nsec = 800000000L;// ==delaytakt(8);
            // nanosleep(&delay,NULL);// 800 mlsek
     //   printf("               after delaytakt ! pz=%d  \n",pz  );
    stopcsn();
    printf("\r             Выполняем переброс по R:          ");
     docum_perebroscsn(_cdf(0),_cdf(1),i*perR+initR);//
     //WAITTAKT(200);
//  delaytakt(1); 20.02.2013krikunov
       } // i  os R
      } //j    os L
      long    end_find  = nomertakta ;

        {s=0.0;
              for(j=1;j<=nj;j++) s+=   epsRt[j][nt]-epsRt[j][1]+lR; //1.2;
                    s=s/nj;// часть формулы вычисления PV !!!
            fprintf(fluft,"\n                УГЛЫ   ПО    L ");
            fprintf(fluft,"\n  epsL = ");
            for(j=1;j<=nj;j++) fprintf(fluft," %f ",epsL[j]);
            fprintf(fluft,"\n                 УГЛЫ   ПО    R ");

             fprintf(fluft,"\n epsR= ");
               for(j=1;j<=nj+1;j++)// это только печать!
                    //for(i=1;i<=nt+1;i++)
                   //		 { fprintf(ff,"\n");   for(i=0;i<=40;i+=7)
                { fprintf(fluft,"\n");
                    for(i=0;i<=30;i+=7)
                   { fprintf(fluft,"\n ");
                      fprintf(fluft,"  %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f",
                      epsRt[j][1+i],epsRt[j][2+i],epsRt[j][3+i],epsRt[j][4+i],
                      epsRt[j][5+i],epsRt[j][6+i],epsRt[j][i+7] );
                    }
                 }

             double ss=0;// вставлено сегодня
             for( j=1;j<6;j++) ss +=(epsRt[j][nt] -epsRt[j][1] -lR);
     //ss вычислено ранее как s!
             PV =3600*s*(epsL[nj]-epsL[1]+lL)  //2.4)
                     / ((float)(end_find-beg_find)/10);// было 25
                   //среднее перекрытие полей зрения
                     s=0.0;
                     for(j=1;j<=nj;j++)
                     for(i=1;i<=nt-1;i++)
                    s += lR  - epsRt[j][i+1]+ epsRt[j][i];

                    delR=s/(nj * (nt-1) ) ;//среднее перекрытие полей зрения вдоль сканируемой строки
                      s = 0.0 ;
                 //среднее перекрытие сканируемых строк
                 for(j=1;j<=nj-1;j++) s+=lL-epsL[j+1]+epsL[j];
                 delL=s/(nj-1);
                 s=0.0;//среднее перекрытие сканируемых строк


    printf("\n Скорость обзора      : %f °^2/час",PV);
    printf("\n Среднее перекрытие сканируемых строк : %f °",delL);
    printf("\n Среднее перекрытие  полей зрения     : %f °",delR);
    fprintf(fluft,"\n Скорость обзора       : %f °^2/час",PV);
    fprintf(fluft,"\n Среднее перекрытие сканируемых строк : %f °",delL);
    fprintf(fluft,"\n Среднее перекрытие  полей зрения     : %f °",delR);
   }


    // delay(10000);
    waittakt(100);
     printf("\n ПЕРЕХОД В ИСХОДНОЕ ПОЛОЖЕНИЕ И УСТАНОВКА  В РЕЖИМ ОЖИДАНИЕ");
    //pz = WAITCSN(NST,0.0,90.0,90.0);
     docum_perebroscsn(0,90,90);//
   // while ( STATUS_CSN ( NST ) == 1 ) {;}
   printf("\n ЗАВЕРШЕНО  ОПРЕДЕЛЕНИЕ   СКОРОСТИ ОБЗОРА   ");
   fprintf(fluft,"\n ЗАВЕРШЕНО ОПРЕДЕЛЕНИЕ   СКОРОСТИ ОБЗОРА  ");

     //}
  if(fluft) fclose( fluft);
}

void nepr_job()
{
int i=0;int stepc =100000; double va=0,vl=0,vr=0;
float t,A,L,R,dlR,t_chv,perR , s=0.0  , lL=0.0 , lR=0.0,
    dL = 0.0 , dR = 0.0 ; /* перекрытие */
    for(i=1;i<=stepc;i+=2)
    {
     if(nomertakta > 1000000L)     break;
      scancklperR(5,i,&A,&L,&R,&perR,&t_chv,&dlR);
     printf("\r ВЫПОЛНЯЕТСЯ ПЕРЕБРОС В ТОЧКУ : A=%7.3f L=%7.3f R=%7.3f \n"
       "\rИ СКАНИРОВАНИЕ ДУГИ =%6.2f град,ВРЕМЯ ЧВ=%4.1f сек,МАЛЫЙ УГОЛ R=%5.2f град.  ",
         A,L,R,dlR,t_chv,perR);
     if (!PUSKCHV(A,L,R)) break;
       delay_sec(t_chv);     // 2
            //pz =  WAITCSN(NST,A,L,R);
         //ANALIZPZCSN(pz) ;
      // if (!RESCSN(NST)) return 0;
      while( fabs( cd[2]-R ) <  dlR )
      {   t_chv = 5. ;//для разгрузки ОПУ при перебросах 13.12.00
         // A=_cdf(OS_A);  L=_cdf(OS_L); R=_cdf(OS_R);
          skorchvcsn(_cdf(OS_A),_cdf(OS_L),_cdf(OS_R),&va,&vl,&vr);
            skorcsn(va,vl,vr);
            delay_sec(t_chv);     // 2
            printf("\n Малый переброс в: ");
            docum_perebroscsn(cd[0],cd[1],cd[2]+perR);
       //pz =  PEREBROSCSN(NST,cdf(0),cdf(1),cdf(2)+perR);  ANALIZPZCSN(pz) ;
      // if (!RESCSN(NST)) return 0;
      }
    }
}


//////////////////////////////////////////////////////////////////////////////////////////
/**************************************************************************/
 void scancklperR(int nst,int ndugi,float*A,float*L,float*R,
                    float *pR,float *tchv,
                          float* dlinaR)
/**************************************************************************/
  { static int p=0;
   int ia, il ,ja ,jl , jr , ir ,ka , na ,ma , kl ,ml ,mr ,er , nl ;
   float ug0 , ug1 , ug2 , ckA , ckL , ckR , t_chv , perR , qpmax ,
     mrmax , dlR ;
   //  exe_oper = 1 ;
    p=ndugi;
 {ja = p-148 * (int) ( (p-1) / 148) ;
  ia = (int) ((ja+1)/2) ;
  ka = ((ia<38)? 0 : 1) ;
  ma = ia-37*ka ; na = ma-2*(ma/2) ;
  ug0 = (-204.0+48.571 *na +9.714*ma) *((ia<38) ? 1: (-1) ) ;
  if(ug0>201.0) ug0 = 201; if(ug0<-201.0) ug0 = -201.0;
  jl = p-68*(int)(round((p-1)/68) ) ;
  il = (int) (jl+1)/2 ;
  kl = ((il>=18) ? 1 : 0);ml = il-17*kl ;nl = ml - 2*(int) (ml/2) ;
  ug1 = 90.0 +(-67.0 +52.643*nl +4.786*ml)*((il>=18)?(-1):1) ;
  if(ug1>154.0) ug1 = 154.0 ;if (ug1<24.0) ug1 = 24.0 ;
  jr = p-90*(int)((p-1)/90) ;
  ir = (int) ((jr+1)/2) ;
  switch (ir) {
  case 40: case 42: case 44 :  ug2 = 20 ; break ;
  case 38 :  ug2 = 30.0; break;
  case 2:case 36:case 4: ug2 = 40.0;break;
  case 6: case 34 : ug2 = 50.0;break;
  case 3: case 10: case 30: case 45: ug2= 70.0;break;
  case 5: case 12: case 28: case 43: ug2 = 80.0;break ;
  case 7:case 14: case 26: case 41: ug2 = 90.0; break ;
  case 9: case 16 : case 24 : case 39 : ug2 = 100.0; break ;
  case 11 : case 18 : case 37 : ug2  = 110.0; break ;
  case 13 : case 20 : case 35 : ug2 = 120.0;  break ;
  case 15 : case 22 : case 33 : ug2 = 130.0 ; break;
  case 17 : case 19  : case 21 : case 31 : ug2 = 138.0; break ;
  case 29 : ug2 = 130.0; break ;
  case 23 : case 25 : case 27 : ug2 = 134.0; break;
  default : ug2 = 90.0;break;
        }
   } /*  расчет углов   */
    { jr =( p+1)- 90 *(int)((p)/90) ; ir=jr/2;
     er = ((ir<23)? 1 : (-1))*((ir%2)?(-1) : 1 );
     mr =  ir/45;
     qpmax = 23*(1-mr)*25*mr;
     mrmax = qpmax /2 ;
     perR = er *(-1.8)*(-1) ;ckA = ckL =ckR = 0.0111 *er ;/* град/сек */
     dlR=((ir==45)? 21.47 : 19.26) ;
     t_chv =4.0 ;
    // printf("\rВЫПОЛНЯЕТСЯ ПЕРЕБРОС В ТОЧКУ : A=%7.3f L=%7.3f R=%7.3f \n"
    // "\rИ СКАНИРОВАНИЕ ДУГИ =%6.2f град,ВРЕМЯ ЧВ=%4.1f сек,МАЛЫЙ УГОЛ R=%5.2f град.  ",
    //   ug0,ug1,ug2,dlR,t_chv,perR);
    //  mrmax= DUGADISKRCSN(nst,ug0,ug1,ug2,perR,t_chv , dlR ) ;
  * A = ug0 ;*L = ug1 ; *R = ug2 ;
  *pR = perR ; *tchv = t_chv ; *dlinaR = dlR ;
     }
    }  /* функция scancklperR */
/////////////////////////////////////////////////////////////////////////

double  my_nabl_polus(int ntk)
{
  OMRDATA &o = omrdata[ntk];
  SPIS_ZV *mzv;
  int kzv, rkzv, i, j;
  double Vmax;
  double t, Az, H, A,L,R;
  VECT3 VO,VX;
  int maskaTK[5];
  double 
    alfa_center  = ( 6 + 58./60 + 25./3600) * HR,// 726
    delta_center = (89 + 30./60 + 44./3600) * GR,
    alfa_right   = ( 4 + 44./60 + 25./3600) * HR,
    delta_right  = (89 + 18./60 + 10./3600) * GR;


// mzv = new SPIS_ZV[40];
  mzv = new SPIS_ZV[400];
//  kzv = read_spis_zv("tmpdan/mynp.txt", mzv);
  kzv = read_spis_zv("dan/np.txt", mzv);
  if (kzv==0) return 1;

  printf("kzv=%d\n",kzv);

  upor_spis_zv(kzv, mzv);
 
  t=rvtime;
  KatVidsea(0, &alfa_center, &delta_center);
  adAHsea(alfa_center, delta_center, t, &Az, &H);
  PlusRefraction(&H);
  VO = SferVect(Az,H);
  KatVidsea(0, &alfa_right, &delta_right);
  adAHsea(alfa_right, delta_right, t, &Az, &H);
  PlusRefraction(&H);
  VX = SferVect(Az,H);
  VX -= VO;

  NAVED(ntk, VO,VX, 0,0, &A,&L,&R);

//  int h,m; double s;
//  hourhms(t/3600+12-TZ, &h,&m,&s);
//  printf("%02d:%02d:%05.2f\n",h,m,s);
//  printf("t=%f A=%f L=%f R=%f\n",t,A,L,R);

  perebroscsn(A,L,R);
  if (!RESCSN()) return 0;  
// stopcsn();
 delay_sec(2);
  memset(maskaTK, 0, sizeof(maskaTK));
  maskaTK[ntk]=1;
  if (!IZMERAPOI(maskaTK)) return 0;

  if (KZVAPOI[ntk] == 0) {
    printf("Звезды не обнаружены\n"); 
    return 1;
  }

  MetodOMR(ntk);

  if (o.omrkzv < 3) {     // #avm#   if (o.omrkzv < 5) {
   //  printf("Отождествлено звезд: %d  Отказ ОМР\n", o.omrkzv); //09.06.13!
     return 1;
  }

  for(i=0; i<kzv; i++) {
    double a,d,Az,H,x,y;
    a = mzv[i].a;
    d = mzv[i].d;
    KatVidsea(0, &a, &d);
    adAHsea(a,d, o.time, &Az,&H);
    PlusRefraction(&H);
    GorTV(o.otkpos, Az,H, &x,&y);
	inverse_korrxyOMR(ntk,&x,&y);
    mzv[i].X = x;
    mzv[i].Y = y;
  }


  printf("\n           Измерение слабых звезд в полярной области из списка dan/np.txt \n\n");

  printf(" Nзв     X      Y      mR    mB    mV    обн      V\n");
//        ****  ****.* ****.*  **.** **.** **.**   sss  ******* 

  rkzv=0;
  Vmax=0;
  for(i=0; i<kzv; i++) {
    double dx, dy, V, xpix, ypix;
    int obn;// признак обнаружения звезды

    if (fabs(mzv[i].X) < TKsizeXmm(ntk)/2 && 
		fabs(mzv[i].Y) < TKsizeYmm(ntk)/2) {
      obn=0;
      for(j=0; j<o.izmkzv; j++) {
        IZMZV *zv=&o.izmzv[j];
		dx = zv->x - mzv[i].X;
		dy = zv->y - mzv[i].Y;
		V = zv->e;
        if (fabs(dx)<0.1 && fabs(dy)<0.1) { 
			obn=1; break; 
		}
      }

	  coord_mm2pix(ntk, mzv[i].X, mzv[i].Y, &xpix, &ypix);
      printf("%4d  %6.1f %6.1f  %5.2f %5.2f %5.2f   %3s",
            mzv[i].num, xpix, ypix, 
            mzv[i].mR, mzv[i].mB, mzv[i].mV, 
            (obn==1? "да":"нет") );

     // if (obn) printf("  %7.0f", V);
      //if (obn) printf("  [dx=%4.1f dy=%4.1f]", dx*MM2PIX(ntk), dy*MM2PIX(ntk));
      
	  printf("\n");
 if (obn && mzv[i].mB>Vmax) Vmax=mzv[i].mB;// krikunov
  //    if (obn && mzv[i].mV>Vmax) Vmax=mzv[i].mV;
      if (obn) rkzv++;
    }
  }
 if(Vmax!=0) printf("\n        Среди распознанных звезд максимальная  зв.величина:%7.2f",Vmax);
  printf("\n         Измерено и распознано звезд: %d\n",rkzv);
  delete mzv;

  return Vmax;
}














// o->catzv.x (mm),o->catzv.y (mm) -x,y katalojnyh
// o->izmzv.x,o->izmvz.y (mm) - x,y izmerrennyh
/*
struct REDOMR {
  float ax,ay,axx,axy,ayx,ayy;
};
*/
/*
struct CATZV { long N; float x,y,v; };
struct IZMZV { float x,y,e; };

struct OMRDATA {
 double A,L,R;
 double time;
 OTKPOS otkpos;
 int maxcatkzv;
 int catkzv;
 CATZV *catzv;
 int maxizmkzv;
 int izmkzv;
 IZMZV *izmzv;
 int omrkzv;
 int *catss;
 int *izmss;
 double VXzv,VYzv;
 REDOMR red;
};

 for(i=0;i<o.catkzv;i++)
 for(j=0;j<o.izmkzv;j++)
 {// по i перебираем каталожные звезды
   if(o.catss[i]!=-1)   izmerzv1[i] = o.izmzv[ o.catss[i] ].y;
  //в izmerzv1[i] заносятся измеренные звезды образующие пару  с данной каталожной. Здесь i-индекс тот же
 // самый, что и у массива каталожных звезд: o.catzv[i] , где i=0 ... o.catkzv-1
  // т.о. в  izmerzv1[i] будут координаты только измеренных каталожных звезд

}
 for(i=0;i<o2.catkzv;i++)
 for(j=0;j<o2.izmkzv;j++)
 {// по i перебираем каталожные звезды
 if(o2.catss[i]!=-1)   izmerzv2[i] = o2.izmzv[ o2.catss[i] ].y;

}
*/

/*





void initOMR(int ntk)
{
  OMRDATA &o=omrdata[ntk];
  o.catkzv=o.izmkzv=o.omrkzv=0;

  o.maxcatkzv=10000;
  o.catzv=(CATZV*)malloc(o.maxcatkzv * sizeof(CATZV));
  o.catss=(int*)malloc(o.maxcatkzv * sizeof(int));

  o.maxizmkzv=10000;
  o.izmzv=(IZMZV*)malloc(o.maxizmkzv * sizeof(IZMZV));
  o.izmss=(int*)malloc(o.maxizmkzv * sizeof(int));
}
class TMASIZM {
	  public:
		long JD;
		int NST;
		int ntk;
		int kolko;
		TKO *ko;
		int kolzv;
		double tzv, azv, lzv, rzv;
		ZV *zv;
		TMASIZM ()  // 
		{
		 JD = 0; NST = 0; ntk = 0; kolko = 0;
		 ko = NULL;
		 kolzv = 0; tzv = 0; azv = 0; lzv = 0; rzv = 0;
		 zv = NULL;
		}
		~TMASIZM()  // 
		{
		}
	      };

*/




