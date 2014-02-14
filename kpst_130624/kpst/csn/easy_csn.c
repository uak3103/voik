/* изменены коэффициенты редукции по осям: L,R для станции 58-1 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "sys/sys.h"
#include "rv/rv.h"
#include "vu/vu.h"
#include "../common/candev.h"
#include "../common/elements.h"
#include "globst.h"

/*
enum {  OS_A = 0,
	OS_L = 1,
	OS_R = 2,
     };
*/

#define CDvGRAD (360./524288)
#define GRADvCD (524288./360)

#define _cd(nos)  cd[nos]
#define MINCD(nos) mincd[nos]
#define MAXCD(nos) maxcd[nos]

int
  regimos[3],  //режим оси: 1 - переброс, 2 - отработка скорости, 0 - ожидание
  encoder[3];  //показание энкодера

double
  cd[3],       //последнее показание ЦД, град
  timecd[3],   //время показания ЦД
//  mincd[3],    //мин.ЦД (программное ограничение)
//  maxcd[3],    //макс.ЦД (программное ограничение)
//  maxskor[3],  //макс.скорость по оси, град/с
//  maxuskor[3], //макс.ускорение по оси, град/с^2
  skor[3],     //тек.скорость по оси, град/с
  targetcd[3], //точка переброса, град
  koeff_red[3];
  
int csn_verbose_mode = 0;  

int statuscsn;

char csnerrmsg[80];
extern double tperebrosa; // для таймаута при перебросе
extern double Tperebros(double A0, double L0, double R0,
                 double A1, double L1, double R1);

void initcsn(void)
{

  maxskor[OS_A] = 6.0;
  maxskor[OS_L] = 6.0;
  maxskor[OS_R] = 6.0;

  maxuskor[OS_A] = 3.5;
  maxuskor[OS_L] = 3.5;
  maxuskor[OS_R] = 3.5;

  mincd[OS_A] = -210.0;   maxcd[OS_A] = 210.0;
  mincd[OS_L] =   20.0;   maxcd[OS_L] = 160.0;
  mincd[OS_R] =    0.0;   maxcd[OS_R] = 180.0;

  cd[OS_A] = 0.;
  cd[OS_L] = 90.;
  cd[OS_R] = 90.;


//koeff_red[OS_A] = 721.323;
//koeff_red[OS_L] = 723.846;
//koeff_red[OS_R] = 724.034;

  double K1 = (1 + 20./88) * (137./38) * (104./26);//17.698563
  koeff_red[OS_A] = (1044./27) *  (78./74) * K1; // 721.33590
//  koeff_red[OS_A] = 824.0*(131092./131216.);
  koeff_red[OS_L] = (918./27) *  (83./69) * K1;  // 723.84557
//  koeff_red[OS_L] = 634.0*(65546./65528.);
  koeff_red[OS_R] = (450./27) * (108./44) * K1;  // 724.031833

//printf("Коэфф. ред. A=%f L=%f R=%f\n",
//   koeff_red[OS_A], koeff_red[OS_L], koeff_red[OS_R]);
//Коэфф. ред. A=721.336092 L=723.845642 R=724.032188

}


void err(int code, int par1, int par2)
{
  char s[80]; char *p=s;
  static char nameos[]="ALR";

  if (statuscsn<0) return;

  statuscsn=-1;

  switch (code) {
    case 6: sprintf(s,"Сбой датчика оси %c",nameos[par1]); break;
    case 7: sprintf(s,"Область ограничения по оси %c",nameos[par1]);
	    break;
    case 10: sprintf(s,"Сбой наведения по оси %c",nameos[par1]); break;
    case 11: sprintf(s,"Сбой ЧВ по оси %c",nameos[par1]); break;
    default: p="?";
  }

  strcpy(csnerrmsg,p);

  if (sterror==0) {
    rabst=rabcsn=2;
    sterror=1;
    strcpy(sterrmsg1,"Ошибка ЦСН");
    strcpy(sterrmsg2,p);
    extern int breakcode;
    breakcode=2;
  }
}


// начало тактового процесса ЦСН

void taktcsn(void)
{
 
  for(int os=OS_A; os<=OS_R; os++) {

     if (IMITOS[os]) {
         double dt = rvtime-timecd[os];
         double X, V, dV;

         switch(regimos[os]) {
           case 1: //переброс
             cd[os] += skor[os] * dt;

             X = targetcd[os] - cd[os];

             if (fabs(X) < 0.05 && fabs(skor[os]) < 0.5) {
                regimos[os] = 0; 
                skor[os] = 0;
                if (regimos[OS_A]==0 && regimos[OS_L]==0 && regimos[OS_R]==0) 
                   statuscsn = 0;
                break;
             }

             V = sqrt(2*maxuskor[os]*fabs(X));
             if (X<0) V = -V;

             dV = V - skor[os];
             if (dV > maxuskor[os]) dV = maxuskor[os];
               else if (dV < -maxuskor[os]) dV = -maxuskor[os];

             skor[os] += dV;
           break;

           case 2: //фикс. скор.
             cd[os] += skor[os] * dt;
           break;
         }

         timecd[os] = rvtime;
     }
    
     //if (cd[os] < mincd[os] || cd[os] > maxcd[os]) err(7,os,0); 

  }

  // конец тактового процесса ЦСН

}


void ask_encoder(int os)
{
  int abon;

  if (os==OS_A) abon=CAN_ABN_KEPA;
  if (os==OS_L) abon=CAN_ABN_KEPL;
  if (os==OS_R) abon=CAN_ABN_KEPR;

  can_req2(fcan_kep, abon, CAN_CMD_GETP, m2b('P',' '));
}

void perebroscsn(double A, double L, double R){
     double dA, dL, dR;
     
  // Признак контрольного вывода
     int prPrnScr=0;
  // Эмпирический нормирующий множитель
     double kA=0.8;
     double kL=0.8;     
     double kR=0.8;
  // Текущее значение максимально допустимой скорости по оси ОПУ
     double VAmax, VLmax, VRmax;
     VAmax=maxskor[OS_A]; //[grad/sec] - по умолчанию (паспорт)
     VLmax=maxskor[OS_L]; //[grad/sec] - по умолчанию (паспорт)
     VRmax=maxskor[OS_R]; //[grad/sec] - по умолчанию (паспорт)
  // ---------------------------     
  // Пороговое значение ошибки контура регулирования (по положению)  
  // Определяется, в частности, по результатам прошивки КЭП и КДПО  
     double myEpsCsn=0.005; //[grad]
  // ---------------------------     
  // Пороговое значение для дуги при перебросе оси в заданную точку    
  // Если меньше, то меняется значение максимальной скорости
  // Уменьшается или увеличивается по заданному правилу (алгоритму)
     double ddA=10.0; //[grad] - исходя из Vmax=6.0[град/сек] на 1/3 интервала
     double ddL=10.0; //[grad] - исходя из Vmax=6.0[град/сек] на 1/3 интервала - исходя из Vmax=6.0[град/сек] на 1/3 интервала          
     double ddR=10.0; //[grad] - исходя из Vmax=6.0[град/сек] на 1/3 интервала          
  // ---------------------------     
  // Значение текущей максимальной скорости (уменьшенное) для более короткой дуги 
  // Вычисляется для текущей операции
  // double ddVA=maxskor[OS_A]; //[grad/sec] - по умолчанию (на всякий случай)
  // double ddVL=maxskor[OS_L]; //[grad/sec] - по умолчанию (на всякий случай)          
  // double ddVR=maxskor[OS_R]; //[grad/sec] - по умолчанию (на всякий случай)           
  // ---------------------------      
     int prA,prL,prR;     
  // ---------------------------           
  // Текущее значение интервалов переброски по осям ОПУ
     double DAt,DLt,DRt;
  // ---------------------------           
  // int i;
  // double porog[3]={0.0, 0.0, 0.0};
  // Контрольный вывод данных
     if (prPrnScr) printf("\n");
  // printf(" myEpsCsn=%+09.6f [grad]; \n",myEpsCsn);
  // printf(" mincd[0]=%+07.2f [grad]; mincd[1]=%+07.2f [grad]; mincd[2]=%+07.2f [grad];\n",mincd[0],mincd[1],mincd[2]);
  // printf(" maxcd[0]=%+07.2f [grad]; maxcd[1]=%+07.2f [grad]; maxcd[2]=%+07.2f [grad];\n",maxcd[0],maxcd[1],maxcd[2]);   
  // printf(" maxuskor[0]=%+07.4f; maxuskor[1]=%+07.4f; maxuskor[2]=%+07.4f;\n",maxuskor[0],maxuskor[1],maxuskor[2]);        
     
  // printf(" porog[0]=%+07.2f [grad]; porog[1]=%+07.2f [grad]; porog[2]=%+07.2f [grad];\n",porog[0],porog[1],porog[2]);   
  // for(i=0; i<3; i++){
  //    if (porog[i]<fabs(mincd[i])) porog[i]=fabs(mincd[i]);
  //    if (porog[i]<fabs(maxcd[i])) porog[i]=fabs(maxcd[i]);
  // };//i
  // printf("\n");
  // printf("---------------------------------------------------------------\n");     
  // printf(" OS_A=%d; OS_L=%d; OS_R=%d\n",OS_A,OS_L,OS_R);
  // printf(" porog[0]=%+07.2f [grad]; porog[1]=%+07.2f [grad]; porog[2]=%+07.2f [grad];\n",porog[0],porog[1],porog[2]);
  // printf("---------------------------------------------------------------\n");          

     if ((A>=mincd[OS_A])&&(A<=maxcd[OS_A])) prA=1;else prA=0;
     if ((L>=mincd[OS_L])&&(L<=maxcd[OS_L])) prL=1;else prL=0; 
     if ((R>=mincd[OS_R])&&(R<=maxcd[OS_R])) prR=1;else prR=0;     
     
     targetcd[OS_A] = A;
     targetcd[OS_L] = L;
     targetcd[OS_R] = R;

     statuscsn = 0;
     
  // Контрольный вывод данных     
  // --------------------------------------------------------------------------
     if (prPrnScr){
        printf("\n");
        printf(" Внимание! Установлены следующие режимы управления осями ОПУ:\n");                                    
        printf(" ----------------------------------------------------------");  
     };//if    
     if (prA){
        regimos[OS_A] = 1;
        skor[OS_A]    = 0;
        if (prPrnScr)
           printf("\n ......... # по оси A => <переброска> %010.6f",fabs(A-cd[OS_A]));                                    
     }else{
        regimos[OS_A] = 0;
        if (prPrnScr)
           printf("\n ......... # по оси A => <нет>");                            
     };//if
  // --------------------------------------------------------------------------    
     if (prL){
        regimos[OS_L] = 1; 
        skor[OS_L]    = 0;
        if (prPrnScr)
           printf("\n ......... # по оси L => <переброска> %010.6f",fabs(L-cd[OS_L]));                                            
     }else{
        regimos[OS_L] = 0;   
        if (prPrnScr)
           printf("\n ......... # по оси L => <нет>");                            
     };//if        
  // --------------------------------------------------------------------------   
     if (prR){
        regimos[OS_R] = 1; 
        skor[OS_R]    = 0;
        if (prPrnScr)
           printf("\n ......... # по оси R => <переброска> %010.6f",fabs(R-cd[OS_R]));                                    
     }else{
        regimos[OS_R] = 0;   
        if (prPrnScr)
           printf("\n ......... # по оси R => <нет>");                            
     };//if
  // =========================================================================
     if (!IMITOS[OS_A]&&prA){
        can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUTP,m2b('M',' '),1);
        usleep(1000);
        DAt=fabs(A-cd[OS_A]);
     // Уточнение максимально допустимой скорости
        if (prA&&prPrnScr) printf("\n ......... # VAmax=%+09.6f; ",VAmax);                                    
        if (DAt<=ddA){
           VAmax=maxuskor[OS_A]*DAt/3.0;
           if (VAmax>maxskor[OS_A]) VAmax=maxskor[OS_A];
           VAmax=kA*VAmax;
        };//if
        if (prA&&prPrnScr) printf("~VAmax=%+09.6f",VAmax);                                                 
        can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUTP,m2b('W',' '), VAmax * 9600);        
    //  can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUTP,m2b('W',' '), maxskor[OS_A] * 9600);
        usleep(200);
        dA = A - cd[OS_A];
        can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUSK,m2b('P',' '),   dA * 40 * koeff_red[OS_A]);
        if (fabs(dA)<myEpsCsn) regimos[OS_A]=0;
     };//if
  // --------------------------------------------------------------------------
     if (!IMITOS[OS_L]&&prL){
        can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUTP,m2b('M',' '),1);
        usleep(1000);
        DLt=fabs(L-cd[OS_L]);
     // Уточнение максимально допустимой скорости
        if (prL&&prPrnScr) printf("\n ......... # VLmax=%+09.6f; ",VLmax);                                    
        if (DLt<=ddL){
           VLmax=maxuskor[OS_L]*DLt/3.0;
           if (VLmax>maxskor[OS_L]) VLmax=maxskor[OS_L];
           VLmax=kL*VLmax;            
        };//if
        if (prL&&prPrnScr) printf("~VLmax=%+09.6f",VLmax);                                                                   
        can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUTP,m2b('W',' '), VLmax * 9600);
     // can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUTP,m2b('W',' '), maxskor[OS_L] * 9600);
        usleep(200);
        dL = L - cd[OS_L];
        can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUSK,m2b('P',' '),   dL * 40 * koeff_red[OS_L]);
        if (fabs(dL)<myEpsCsn) regimos[OS_L]=0;
     };//if
  // --------------------------------------------------------------------------
     if (!IMITOS[OS_R]&&prR){
        can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUTP,m2b('M',' '),1);
        usleep(1000);
        DRt=fabs(R-cd[OS_R]);
     // Уточнение максимально допустимой скорости
        if (prR&&prPrnScr) printf("\n ......... # VRmax=%+09.6f; ",VRmax);                                    
        if (DRt<=ddR){
           VRmax=maxuskor[OS_R]*DRt/3.0;
           if (VRmax>maxskor[OS_R]) VRmax=maxskor[OS_R];
           VRmax=kR*VRmax;            
        };//if
        
        if (prR&&prPrnScr) printf("~VRmax=%+09.6f",VRmax);                                                                   
        can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUTP,m2b('W',' '), VRmax * 9600);
     // can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUTP,m2b('W',' '), maxskor[OS_R] * 9600);
        usleep(200);
        dR = R - cd[OS_R];
        can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUSK,m2b('P',' '),   dR * 40 * koeff_red[OS_R]);
        if (fabs(dR)<myEpsCsn) regimos[OS_R]=0;
     };//if
  // --------------------------------------------------------------------------  
      if (prPrnScr){
         printf("\n");
         printf(" ----------------------------------------------------------\n");       
      };//if
      tperebrosa =Tperebros(cd[0],cd[1],cd[2],A,L,R);
      if ((regimos[OS_A]==1)||(regimos[OS_L]==1)||(regimos[OS_R]==1)) statuscsn=1;
};//perebroscsn()

/*
void perebroscsn(double A, double L, double R)
{
  double dA, dL, dR;

if(A != 999 && (A < MINCD(OS_A) || A > MAXCD(OS_A)))  { err(7,OS_A,0); return; }//krikunov 06.0.6.13
 if(L != 999 && (L < MINCD(OS_L) || L > MAXCD(OS_L)))  { err(7,OS_L,0); return; }
 if(R != 999 && (R < MINCD(OS_R) || R > MAXCD(OS_R)))  { err(7,OS_R,0); return; }

  targetcd[OS_A] = A;
  targetcd[OS_L] = L;
  targetcd[OS_R] = R;

  statuscsn = 0;
  if (A != 999.0) regimos[OS_A] = 1; else regimos[OS_A] = 0;
  if (L != 999.0) regimos[OS_L] = 1; else regimos[OS_L] = 0;
  if (R != 999.0) regimos[OS_R] = 1; else regimos[OS_R] = 0;

  if (A != 999.0) skor[OS_A] = 0;
  if (L != 999.0) skor[OS_L] = 0;
  if (R != 999.0) skor[OS_R] = 0;

  if (!IMITOS[OS_A] && A != 999.0) {
    //printf("check A=%f\n",A);
    can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUTP,m2b('M',' '),1);
    usleep(1000);
    can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUTP,m2b('W',' '), maxskor[OS_A] * 9600);
    usleep(200);
    dA = A - cd[OS_A];
    can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUSK,m2b('P',' '),   dA * 40 * koeff_red[OS_A]);
    if (fabs(dA)<0.001) regimos[OS_A]=0;
  }

  if (!IMITOS[OS_L] && L != 999.0) {
    //printf("check L=%f\n",L);
    can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUTP,m2b('M',' '),1);
    usleep(1000);
    can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUTP,m2b('W',' '), maxskor[OS_L] * 9600);
    usleep(200);
    dL = L - cd[OS_L];
    can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUSK,m2b('P',' '),   dL * 40 * koeff_red[OS_L]);
    if (fabs(dL)<0.001) regimos[OS_L]=0;
  }

  if (!IMITOS[OS_R] && R != 999.0) {
    //printf("check R=%f\n",R);
    can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUTP,m2b('M',' '),1);
    usleep(1000);
    can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUTP,m2b('W',' '), maxskor[OS_R] * 9600);
    usleep(200);
    dR = R - cd[OS_R];
    can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUSK,m2b('P',' '),   dR * 40 * koeff_red[OS_R]);
    if (fabs(dR)<0.001) regimos[OS_R]=0;
  }
     tperebrosa =Tperebros(cd[0],cd[1],cd[2],A,L,R);
  if (regimos[OS_A]==1 || regimos[OS_L]==1 || regimos[OS_R]==1)
    statuscsn=1;
}
*/

void stopcsn(void)
{
  regimos[OS_A] = regimos[OS_L] = regimos[OS_R] = 0;
  skor[OS_A] = skor[OS_L] = skor[OS_R] = 0;
  statuscsn = 0;

  if (!IMITOS[OS_A])
  {
   can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUSK,m2b('W',' '), 0);
   usleep(200);
  }
  if (!IMITOS[OS_L])
  {
   can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUSK,m2b('W',' '), 0);
   usleep(200);
  }
  if (!IMITOS[OS_R])
  {
   can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUSK,m2b('W',' '), 0);
   usleep(200);
  }

 can_req0(fcan_kep,CAN_ABN_KEPA,CAN_CMD_STOP);usleep(200); //krikunov
 can_req0(fcan_kep,CAN_ABN_KEPL,CAN_CMD_STOP);usleep(200); //krikunov
 can_req0(fcan_kep,CAN_ABN_KEPR,CAN_CMD_STOP);usleep(200); //krikunov

 can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUTP,m2b('M',' '),0);usleep(200);//krikunov
 can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUTP,m2b('M',' '),0);usleep(200);//krikunov
 can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUTP,m2b('M',' '),0);usleep(200);//krikunov

 delaytakt(10);          //1 cek

}

// Процедура инициализации режима <по скорости>
void skorcsn(double va,double vl,double vr){

  // Признак контрольного вывода
     int prPrnScr=0;
  // Признаки инициализации управления и принадлежности значения 
  // к допустимому интервалу (паспорт ОЭС)
     int prA=0;          
     int prL=0;          
     int prR=0;
  // Точность оценки нулевого состояния
     double myEpsCsnV = 0.00000001;// [град.сек]
     
  // Контроль параметра относительно интервала допустимых значений     
     if (fabs(va)<=maxskor[OS_A]) prA=1;
     if (fabs(vl)<=maxskor[OS_L]) prL=1; 
     if (fabs(vr)<=maxskor[OS_R]) prR=1;

  // Контрольный вывод данных
     if (prPrnScr){
        printf("\n");
        printf(" myEpsCsnV  =%+09.6f\n",myEpsCsnV);
        printf(" maxskor[0] =%+07.2f; maxskor[1] =%+07.2f; maxskor[2] =%+07.2f;\n",maxskor[0],maxskor[1],maxskor[2]);   
        printf(" maxuskor[0]=%+07.4f; maxuskor[1]=%+07.4f; maxuskor[2]=%+07.4f;\n",maxuskor[0],maxuskor[1],maxuskor[2]);        
     };//if        
 
  // Уточнение нулевого значения (когда малое значение есть "скорость")           
  // значение <0.0> считаем отдельной обособленной командой типа <stop>
     if (fabs(va)<myEpsCsnV){
        prA=0;
     // Далее аналог процедуры <stopcsn>        
        regimos[OS_A]= 0;
        skor[OS_A]   = 0;
     // statuscsn    = 0;
        if (!IMITOS[OS_A]){
           can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUSK,m2b('W',' '), 0);
           usleep(200);
        };//if
        can_req0(fcan_kep,CAN_ABN_KEPA,CAN_CMD_STOP);usleep(200);
        can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUTP,m2b('M',' '),0);usleep(200);
     // delaytakt(10);//1 cek        
     };//if

     if (fabs(vl)<myEpsCsnV){
        prL=0; 
     // Далее аналог процедуры <stopcsn>                
        regimos[OS_L]= 0;
        skor[OS_L]   = 0;
     // statuscsn    = 0;
        if (!IMITOS[OS_L]){
           can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUSK,m2b('W',' '), 0);
           usleep(200);           
        };//if
        can_req0(fcan_kep,CAN_ABN_KEPL,CAN_CMD_STOP);usleep(200);
        can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUTP,m2b('M',' '),0);usleep(200);
     // delaytakt(10);//1 cek                
     };//if

     if (fabs(vr)<myEpsCsnV){
        prR=0;     
     // Далее аналог процедуры <stopcsn>                
        regimos[OS_R]= 0;
        skor[OS_R]   = 0;
     // statuscsn    = 0;
        if (!IMITOS[OS_R]){
           can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUSK,m2b('W',' '), 0);
           usleep(200);
        };//if
        can_req0(fcan_kep,CAN_ABN_KEPR,CAN_CMD_STOP); //krikunov - ранее
        usleep(200);
        can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUTP,m2b('M',' '),0);usleep(200);
     // delaytakt(10);//1 cek                
     };//if
     
  // Статус на всякий случай !? (уточнить)
     if ( (regimos[OS_A]==0)&&(regimos[OS_L]==0)&&(regimos[OS_R]==0)&&
          (skor[OS_A]   ==0)&&(skor[OS_L]   ==0)&&(skor[OS_R]   ==0) ) 
        statuscsn=0;     
        
     if (prA) { skor[OS_A]=va; regimos[OS_A]=2; }
     if (prL) { skor[OS_L]=vl; regimos[OS_L]=2; }
     if (prR) { skor[OS_R]=vr; regimos[OS_R]=2; }

     statuscsn = 1;

     if (!IMITOS[OS_A] && prA)
        {can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUTP,m2b('M',' '),1); usleep(1000);}
     // --------------------------------------------------------------------------           
     if (!IMITOS[OS_L] && prL)
        {can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUTP,m2b('M',' '),1); usleep(1000);}
     // --------------------------------------------------------------------------           
     if (!IMITOS[OS_R] && prR)
        {can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUTP,m2b('M',' '),1); usleep(1000);}
     // --------------------------------------------------------------------------          
     
  // if (!IMITOS[OS_A] && va!=999) can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUSK,m2b('W',' '), va * 40./3 * koeff_red[OS_A]);
  // if (!IMITOS[OS_L] && vl!=999) can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUSK,m2b('W',' '), vl * 40./3 * koeff_red[OS_L]);
  // if (!IMITOS[OS_A] && vr!=999) can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUSK,m2b('W',' '), vr * 40./3 * koeff_red[OS_R]);

  // --------------------------------------------------------------------------                   
     long spda = (long) round(va *3600/0.3618);
  // if(spda>=8) // 04.04.2013 tsarev
     spda+=8;
     if (!IMITOS[OS_A] && prA) 
        can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUSK,m2b('W',' '), spda);
  // --------------------------------------------------------------------------                   
     long spdl = (long) round(vl *3600/0.3618);
  // if(spdl>=8) // 04.04.2013 tsarev
     spdl+=8;
     if (!IMITOS[OS_L] && prL) 
        can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUSK,m2b('W',' '), spdl);
  // --------------------------------------------------------------------------                   
     long spdr = (long) round(vr *3600/0.3618);
  // if(spdr>=8) // 04.04.2013 tsarev
     spdr+=8;
     if (!IMITOS[OS_R] && prR) 
        can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUSK,m2b('W',' '), spdr);

};//skorcsn()

/*
void skorcsn(double va, double vl, double vr)
{
  if (va!=999) { skor[OS_A] = va; regimos[OS_A] = 2; }
  if (vl!=999) { skor[OS_L] = vl; regimos[OS_L] = 2; }
  if (vr!=999) { skor[OS_R] = vr; regimos[OS_R] = 2; }

  statuscsn = 1;

  if (!IMITOS[OS_A] && va!=999)
   {can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUTP,m2b('M',' '),1); usleep(1000);}
  if (!IMITOS[OS_L] && vl!=999)
   {can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUTP,m2b('M',' '),1); usleep(1000);}
  if (!IMITOS[OS_R] && vr!=999)
   {can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUTP,m2b('M',' '),1); usleep(1000);}
       
//  if (!IMITOS[OS_A] && va!=999) can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUSK,m2b('W',' '), va * 40./3 * koeff_red[OS_A]);
//  if (!IMITOS[OS_L] && vl!=999) can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUSK,m2b('W',' '), vl * 40./3 * koeff_red[OS_L]);
//  if (!IMITOS[OS_A] && vr!=999) can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUSK,m2b('W',' '), vr * 40./3 * koeff_red[OS_R]);

  long spda = (long) round(va *3600/0.3618);
  if(spda>=8) spda+=8;
  if (!IMITOS[OS_A] && va!=999) 
  can_req6(fcan_kep,CAN_ABN_KEPA,CAN_CMD_PUSK,m2b('W',' '), spda);
  long spdl = (long) round(vl *3600/0.3618);
  if(spdl>=8) spdl+=8;
  if (!IMITOS[OS_L] && vl!=999) 
  can_req6(fcan_kep,CAN_ABN_KEPL,CAN_CMD_PUSK,m2b('W',' '), spdl);
  long spdr = (long) round(vr *3600/0.3618);
  if(spdr>=8) spdr+=8;
  if (!IMITOS[OS_A] && vr!=999) 
  can_req6(fcan_kep,CAN_ABN_KEPR,CAN_CMD_PUSK,m2b('W',' '), spdr);

}
*/

void hndl_kep(CAN_MSG *msg)
{ 
  int abon = CAN_ADR(msg->idn);
  int os;
  int val;

  if (abon==CAN_ABN_KEPA) os=OS_A;
  if (abon==CAN_ABN_KEPL) os=OS_L;
  if (abon==CAN_ABN_KEPR) os=OS_R;

  switch (CAN_CMD(msg->idn))
  {
   case CAN_CMD_GETP:
    switch (MSG_T(msg))
    {
     case m2b('P',' '): //encoder
       encoder[os] = MSG_V(msg);
       break;
    }
    break;

   case CAN_CMD_STAT:
    switch (MSG_T(msg))
    {
     case m2b('M','0'):  // mufta
       //printf("%s:mufta\n",can_abn_name(abon));
       break;
     case m2b('P','O'):  // duga OK
       regimos[os]=0;
       if (regimos[OS_A]==0 && regimos[OS_L]==0 && regimos[OS_R]==0) statuscsn = 0;
       //printf("%s:arc\n",can_abn_name(abon));
       break;
     case m2b('U','D'):  // High power
       //printf("%s:300V\n",can_abn_name(abon));
       break;
     case m2b('O','A'):  // Limiter first level aside "minus"
       //printf("%s:ogr1-A\n",can_abn_name(abon));
       break;
     case m2b('O','B'):  // Limiter first level aside "plus"
       //printf("%s:ogr1+B\n",can_abn_name(abon));
       break;
     case m2b('U','A'):  // Limiter second level aside "minus"
       //printf("%s:ogr2-A\n",can_abn_name(abon));
       break;
     case m2b('U','B'):  // Limiter second level aside "plus"
       //printf("%s:ogr2+B\n",can_abn_name(abon));
       break;
    }
     case CAN_CMD_PUSK://krikunov
    switch (MSG_T(msg))
    {case m2b('V','0'):  // ESCAPE TIMEOUT
      // printf("\nESCAPE: %s  %08X\n",can_abn_name(abon),MSG_T(msg));
       regimos[os]=0;
       if (regimos[OS_A]==0 && regimos[OS_L]==0 && regimos[OS_R]==0) statuscsn = 0;
       break;
     }

    break;
  }
}


void hndl_kdpo(CAN_MSG *msg)
{ 
  int os, val; 

  switch (CAN_CMD(msg->idn))
  {
    case CAN_CMD_GETP:

    os = MSG_T(msg);
    if (!IMITOS[os] && os<=2) {
      cd[os] = MSG_V(msg) * CDvGRAD;
      timecd[os] = rvtime;
    }
    break;

    case CAN_CMD_STAT:
    switch (MSG_T(msg))
    {
      case m2b('S','I'):
      val=MSG_V(msg);
      //printf("1Hz syncro %s\n",(val>0)?"on":"off");
      break;
    }
    break;
  }
}


double t_start, t_end;
double cd_start[3], cd_shift[3], cd_end[3];
int enc_start[3], enc_shift[3], enc_end[3];

int RESCSN(void);


void docum_perebroscsn(double A, double L, double R)
{
   double target[3];
   int os;

   printf("A=%f L=%f R=%f\n",A,L,R);
      
   if (!csn_verbose_mode) {
     perebroscsn(A, L, R);
     RESCSN();
//     stopcsn();     //19.06.13
     return;
   }

   target[0] = A;
   target[1] = L;
   target[2] = R;
   
   printf("перед перебросом\n");
   
   for(os=OS_A; os<=OS_R; os++) {
     encoder[os]=0;
     ask_encoder(os);
     //usleep(1000);
   }
   usleep(1000);
 
   for(os=OS_A; os<=OS_R; os++) {
     enc_start[os] = encoder[os];
     cd_start[os] = cd[os];
     if (target[os] != 999.0) {
       cd_shift[os] = target[os] - cd_start[os];
       enc_shift[os] = (int) (cd_shift[os] * 40 * koeff_red[os]);
     } else {
       cd_shift[os] = 0;
       enc_shift[os] = 0;
     }
     if (target[os] != 999.0) {
       printf("ось %c: ЦД %8.4f сдвиг %8.4f  энкодер %d сдвиг %d\n",
          "ALR"[os], cd_start[os], cd_shift[os], enc_start[os], enc_shift[os]);
     }
   }
   
   t_start = rvtime;
  tperebrosa =Tperebros(cd[0],cd[1],cd[2],A,L,R);        
   perebroscsn(A, L, R);

   RESCSN();
//   stopcsn();     //19.06.13
   
   t_end = rvtime;
   
   for(os=OS_A; os<=OS_R; os++) {
     cd_end[os] = cd[os];
   }

   for(os=OS_A; os<=OS_R; os++) {
     encoder[os]=0;
     ask_encoder(os);
   }
   usleep(1000);
   for(os=OS_A; os<=OS_R; os++) {
     enc_end[os] = encoder[os];
   }
   
 //  printf("время переброса %5.3f\n", t_end - t_start);
  printf("Расчетное время переброса: %5.3f реальное:%5.3f\n",tperebrosa,t_end - t_start);
   for(os=OS_A; os<=OS_R; os++) {
     if (target[os] != 999.0) {
       /* printf("ось %c: ЦД %8.4f сдвиг %6.4f ошибка %6.4f  энкодер %d сдвиг %d ошибка %d\n",
            "ALR"[os], 
            cd_end[os], 
            cd_end[os] - cd_start[os], 
            cd_end[os] - cd_start[os] - cd_shift[os], 
            enc_end[os], 
            enc_end[os] - enc_start[os], 
            enc_end[os] - enc_start[os] - enc_shift[os]);*/
printf("                     После переброса :\n");
printf("Ось %c: ЦД %8.4f° сдвиг %7.4f° ошибка по ЦД:%6.1f\"  энкодер %d сдвиг %d ошибка по энкодеру%7.1f\" \n",
            "ALR"[os], 
            cd_end[os], 
            cd_end[os] - cd_start[os], 
            (cd_end[os] - cd_start[os] - cd_shift[os])*3600, 
            enc_end[os], enc_end[os] - enc_start[os], 
            (enc_end[os] - enc_start[os] - enc_shift[os])* 1296000/(14400*koeff_red[os]) );
       }
   }
   
}


void docum_skorcsn(double va, double vl, double vr)
{
   int os;
   double v[3];

   if (!csn_verbose_mode) {
     skorcsn(va, vl, vr);
     return;
   }

   v[OS_A] = va;   
   v[OS_L] = vl;   
   v[OS_R] = vr;   
   
   for(os=OS_A; os<=OS_R; os++) {
     encoder[os]=0;
     ask_encoder(os);
     //usleep(1000);
   }
   usleep(1000);
 
   for(os=OS_A; os<=OS_R; os++) {
     enc_start[os] = encoder[os];
     cd_start[os] = cd[os];

     if (v[os]!=999) {
       printf("ось %c: ЦД %8.4f энкодер %d \n",
          "ALR"[os], cd_start[os], enc_start[os]);
     }
   }
   
   t_start = rvtime;
      
   skorcsn(va, vl, vr);

   RESCSN();
   
   t_end = rvtime;
   
   for(os=OS_A; os<=OS_R; os++) {
     cd_end[os] = cd[os];
   }

   for(os=OS_A; os<=OS_R; os++) {
     encoder[os]=0;
     ask_encoder(os);
     //usleep(1000);
   }
   usleep(1000);
   for(os=OS_A; os<=OS_R; os++) {
     enc_end[os] = encoder[os];
   }
   
   printf("время движения %5.3f\n", t_end - t_start);
   for(os=OS_A; os<=OS_R; os++) {
     if (v[os]!=999) {
        printf("ось %c: ЦД %8.4f сдвиг %6.4f скор. =%6.4f  энкодер %d сдвиг %d скор. =%6.4f\n",
            "ALR"[os], 
            cd_end[os], 
            cd_end[os] - cd_start[os], 
            (cd_end[os] - cd_start[os]) / (t_end - t_start), 
            enc_end[os], 
            enc_end[os] - enc_start[os], 
            (enc_end[os] - enc_start[os]) / (t_end - t_start) / 40 / koeff_red[os] );
       }
   }
   
}
