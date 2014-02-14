#ifndef _SYS_H
#define _SYS_H

#define BYTE  unsigned char
#define BOOL  unsigned char
#define WORD  unsigned int
#define DWORD unsigned long
#define FALSE 0
#define TRUE 1

  void echo(char *format,...);
#ifndef INSIDESYS
  void myprintf(char *format, ...);
  #define printf myprintf
  char *mygets(char *str);
  #define gets mygets
  void *mymalloc(size_t sz);
  #define malloc mymalloc
  void myfree(void*);
  #define free myfree
#endif

void initsys(int intr, FILE *textdump);

void set_line_mode();
void set_key_mode();

extern int to_stdout;
extern int to_file;

FILE *myfopen(char *name, char *mode);
void freaderror(char *fname);
void fwriteerror(char *fname);

void errorproc(char *);
void myexit(void);

#endif // _SYS_H