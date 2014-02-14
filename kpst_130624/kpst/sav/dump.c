char ftextdump_name[80];
FILE *ftextdump;

void standard_dump_names(int day, int month, int year, int hour)
{
   double jd;
   char date[9];
   char *p;
   jd=DMYtoJD(day,month,year);
   if (hour<12) jd--;
   JDtoDMY(jd,&day,&month,&year);
   sprintf(date,"%02d_%02d_%02d",day,month,year%100);
   p = ftextdump_name;
   strcpy(p,"dump/"); strcat(p,date); strcat(p,".txt");
}


void create_dirs(char *path)
{
  char *p=path;
  while((p=strchr(p,'/'))!=NULL) {
    *p='\0';
    mkdir(path, 0777);
    *p='/'; p++;
  }
}

int open_text_dump(void)
{
  create_dirs(ftextdump_name);
  if ((ftextdump=myfopen(ftextdump_name,"a+t"))==NULL) return 0;
    else return 1;
}

int flush_text_dump(void)
{
  if (ftextdump!=NULL) fclose(ftextdump);
  return open_text_dump();
}
