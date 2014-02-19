//#include <QtCore/QCoreApplication>
#include "base.h"

int   q_www =0;

int main(int argc, char *argv[])
{
    Base base;
    char s[100];
    strcpy(s,"VX=-0.290799;VY=-1.404870;");

    base.fun_obrb(0,s);

    return(0);
}
