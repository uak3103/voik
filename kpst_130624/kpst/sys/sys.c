#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>
#include <stdarg.h>

#include <../common/common.h>

#define INSIDESYS
#include "sys/sys.h"
#include "globst.h"


void errorproc(char *msg)
{
  puts("Произошла программная ошибка.");
  exit(0);
}

struct termios 
    linemode_trms, //стандартный режим клавиатуры - для ввода строк
    keymode_trms;  //нестандартный режим клавиатуры - 
                   //для немедленной реакции на нажатия клавиш
int term_mode=0;

void init_trms(void)
{
  tcgetattr(0,&linemode_trms);

  keymode_trms=linemode_trms;

  keymode_trms.c_lflag&=~(ICANON|ECHO);
  keymode_trms.c_cc[VTIME]=0;
  keymode_trms.c_cc[VMIN]=0;

  atexit(set_line_mode);
}

void set_line_mode(void)
{
  if (term_mode!=0) {
    tcsetattr(0,TCSANOW,&linemode_trms);
    term_mode=0;
  }
}

void set_key_mode(void)
{
  if (term_mode!=1) {
    tcsetattr(0,TCSANOW,&keymode_trms);
    term_mode=1;
  }
}

FILE *fd;
extern int sock_conn;

void initsys(int intr, FILE *textdump)
{
  fd=textdump;
  init_trms();
}

/*
void echo(char *format,...)
{
 char bufi[800];
 char bufo[800];
 char *param = (char*)((&format)+1);
  if (vsprintf(bufi,format,param)>800) errorproc(0);
  strcpy(bufo,bufi);
  if (sock_conn>=0) send(sock_conn,bufo,strlen(bufo)+1,0);
}
*/

//режимы работы процедуры myprintf
int to_stdout = 1;  //выводить на консоль
int to_file = 1;    //печатать в файл

void myprintf(char *format, ...)
{
 iconv_t cd;
 size_t l1,l2;
 char bufi[800];
 char bufo[800];
 char *ptri;
 char *ptro;
 //char *param = (char*)((&format)+1);
 va_list param;
 va_start(param,format);

  if (vsprintf(bufi,format,param)>800) errorproc(0);
#if CHARSET==UTF8
  cd=iconv_open("utf8","koi8-r");
  memset(bufo,0,sizeof(bufo));
  l1=strlen(bufi);
  l2=sizeof(bufo);
  ptri=bufi;
  ptro=bufo;
  if (iconv(cd,&ptri,&l1,&ptro,&l2)<0) perror("iconv");
  iconv_close(cd);
#else
  strcpy(bufo,bufi);
#endif // UTF8
  if (to_stdout) 
    fputs(bufo,stdout);
  if (to_file && fd!=NULL) 
    fputs(bufo,fd);
}

char* mygets(char *str)
{
 int k;
 set_line_mode();
 fgets(str,80,stdin);
 k=strlen(str);
 if (k>0 && k<80 && str[k-1]=='\n') str[k-1]=0;
  if (fd!=NULL) {
    fputs(str,fd);
    fputs("\n",fd);
  }
 return str;
}

void *mymalloc(size_t sz)
{ void *p;
  p=malloc(sz);
  if (p==NULL) {
     puts("Недостаточно памяти!");
     puts("Hажмите любую клавишу");
     exit(0);
  }
  return p;
}

void *myfree(void *p)
{
  free(p);
}

FILE *myfopen(char *name, char *mode)
{
  FILE *f;
  f=fopen(name,mode);
  if (f==NULL) printf("Не открыт файл %s\n",name);
  return f;
}

void freaderror(char *fname)
{
  printf("Ошибка при чтении файла: %s\n",fname);
}

void fwriteerror(char *fname)
{
  printf("Ошибка при записи в файл: %s\n",fname);
}

