//#define RS     30.0
//#define H     0.14
//#define RAD    H

double RS;         //размер сетки, (=макс.сдвиг между изм. и кат. по х,у)
double HH=0.1;     //шаг сетки, мм
double RADIUS=0.1; //радиус круга для отождествления, мм

/*
struct REDOMR {
  float ax,ay,axx,axy,ayx,ayy;
};
*/

void clear_redomr(REDOMR &r)
{ r.ax=r.ay=0; r.axx=r.ayy=1; r.axy=r.ayx=0; }

void redomr(REDOMR &red, double x0, double y0, double *x, double *y)
{ *x = red.ax + red.axx*x0 + red.axy*y0;
  *y = red.ay + red.ayx*x0 + red.ayy*y0;
}

void inverse_redomr(REDOMR &red, double x0, double y0, double *x, double *y)
{ 
  double ax=red.ax, ay=red.ay, axx=red.axx, axy=red.axy, ayx=red.ayx, ayy=red.ayy,
         D = axx*ayy - axy*ayx;
  *x = ( ayy*(x0-ax) - axy*(y0-ay)) / D;
  *y = (-ayx*(x0-ax) + axx*(y0-ay)) / D;
}


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
*/

OMRDATA omrdata[3];

double dist(REDOMR &red, IZMZV &izmzv, CATZV &catzv)
{ double x,y,dx,dy;
  redomr(red,izmzv.x,izmzv.y,&x,&y);
  dx=x-catzv.x;	dy=y-catzv.y;
  return dx*dx+dy*dy;
}

int Kletka(OMRDATA &o, double *X, double *Y)
{
 int izmkzv=o.izmkzv, catkzv=o.catkzv;
 CATZV *catzv=o.catzv; IZMZV *izmzv=o.izmzv;
 int *catss=o.catss, *izmss=o.izmss;
 REDOMR &red=o.red;
 int i,j,nx,ny,nn,sz;
 int nxmax,nymax,max; double x,y,dx,dy,r,xmax,ymax,rmax;
 int *mas;

 sz=2*RS/HH; if (sz*HH<2*RS) sz++;
 mas = (int*)malloc(sz*sz*sizeof(int));
 for (i=0;i<sz*sz;i++) mas[i]=0;

 for (i=0;i<izmkzv;i++) {
   redomr(red,izmzv[i].x,izmzv[i].y,&x,&y);
   for (j=0;j<catkzv;j++) {
     dx=x-catzv[j].x;
     dy=y-catzv[j].y;
     #define INT(x) (int)(x+100)-100   /* целая часть x для x>-100  */
     nx=INT((dx+RS)/HH);
     ny=INT((dy+RS)/HH);
     if (nx>=0&&nx<sz && ny>=0&&ny<sz) {
       mas[nx*sz+ny]++;
       if (nx>0) mas[(nx-1)*sz+ny]++;
       if (ny>0) mas[nx*sz+(ny-1)]++;
       if (nx>0&&ny>0) mas[(nx-1)*sz+(ny-1)]++;
     }
   }
 }

 max=0;
 for(nx=0;nx<sz;nx++)
   for(ny=0;ny<sz;ny++) {
     int nn=mas[nx*sz+ny];
     if (nn>=max) {
       x=-RS+HH*(nx+1);
       y=-RS+HH*(ny+1);
       r=x*x+y*y;
       if (nn>max||r<rmax) {
         nxmax=nx; nymax=ny; max=nn; xmax=x; ymax=y; rmax=r;
       }
     }
   }

 free(mas);
 if (max==0) return 0;
 *X=xmax; *Y=ymax;
 return 1;
}

int StarsInKletka (OMRDATA &o, double X, double Y,
                   double *Xcentr, double *Ycentr)
{
 int izmkzv=o.izmkzv, catkzv=o.catkzv;
 CATZV *catzv=o.catzv; IZMZV *izmzv=o.izmzv;
 int *catss=o.catss, *izmss=o.izmss;
 REDOMR &red=o.red;
 double x,y,dx,dy,cx,cy; int i,j,kol;

 kol=0; cx=0; cy=0;
 for (i=0;i<izmkzv;i++) {
   redomr(red,izmzv[i].x,izmzv[i].y,&x,&y);
   for (j=0;j<catkzv;j++) {
     dx=x-catzv[j].x;
     dy=y-catzv[j].y;
     if (fabs(dx-X)<HH && fabs(dy-Y)<HH) {
       cx+=dx; cy+=dy; kol++;
     }
   }
 }

 *Xcentr=cx/kol; *Ycentr=cy/kol;
 return kol;
}

int StarsInCircle(OMRDATA &o)
{
 int izmkzv=o.izmkzv, catkzv=o.catkzv;
 CATZV *catzv=o.catzv; IZMZV *izmzv=o.izmzv;
 int *catss=o.catss, *izmss=o.izmss;
 REDOMR &red=o.red;
 int i,j,n,m,kol;
 double d1,d2,d3;

 for (i=0;i<catkzv;i++) catss[i]=-1;
 for (i=0;i<izmkzv;i++) izmss[i]=-1;

 kol = 0;
 for (i=0;i<izmkzv;i++) {

   for (j=0;j<catkzv;j++) {

     d1=dist(red,izmzv[i],catzv[j]);
     if (d1<RADIUS*RADIUS) {
       n=catss[j];
       if (n!=-1) d2=dist(red,izmzv[n],catzv[j]); else d2=1000;
       m=izmss[i];
       if (m!=-1) d3=dist(red,izmzv[i],catzv[m]); else d3=1000;
       if (d1<d2&&d1<d3) {
         if (n!=-1) { izmss[n]=-1; kol--; }
         if (m!=-1) { catss[m]=-1; kol--; }
         izmss[i]=j; catss[j]=i; kol++;
       }
     }
   }
 }
 o.omrkzv=kol;
 return kol;
}

void get_reduction_OMR(OMRDATA &o, double xko, double yko)
{
 int izmkzv=o.izmkzv, catkzv=o.catkzv;
 CATZV *catzv=o.catzv; IZMZV *izmzv=o.izmzv;
 int *catss=o.catss, *izmss=o.izmss;
 REDOMR &red=o.red;
 MATR33 M, MOBR,E;
 VECT3 U, V;
 double x1, y1, x2, y2, r, s, w;
 int i,j;
 VECT3 A0, A1;

 M.m11 = 0; M.m12 = 0; M.m13 = 0;
 M.m21 = 0; M.m22 = 0; M.m23 = 0;
 M.m31 = 0; M.m32 = 0; M.m33 = 0;
 U.x   = 0; U.y   = 0; U.z   = 0;
 V.x   = 0; V.y   = 0; V.z   = 0;

 //printf("kzv %d\n",xko,yko);
 //printf("ko x=%f y=%f\n",xko,yko);
 for (i=0;i<izmkzv;i++) {
   j=izmss[i];
   if (j!=-1) {
     x1=izmzv[i].x; y1=izmzv[i].y;
     //printf("izmer x=%f y=%f\n",x1,y1);
/*
     r=sqrt((x1-xko)*(x1-xko)+(y1-yko)*(y1-yko));
     s=r/5*0.014;
     if (s<0.014) s=0.014;
     w=1/(s*s);
*/
	 w=1;
     M.m11 += w;
     M.m12 += w * x1;
     M.m13 += w * y1;
     M.m22 += w * x1 * x1;
     M.m23 += w * x1 * y1;
     M.m33 += w * y1 * y1;
     x2=catzv[j].x; y2=catzv[j].y;
     //printf("cat   x=%f y=%f\n",x2,y2);
     U.x   += w * x2;
     U.y   += w * x1 * x2;
     U.z   += w * y1 * x2;
     V.x   += w * y2;
     V.y   += w * x1 * y2;
     V.z   += w * y1 * y2;
   }
 }
 M.m21 = M.m12;
 M.m31 = M.m13;
 M.m32 = M.m23;

// s=0.01;
// w=1/(s*s);
// M.m22+=w; M.m33+=w; U.y+=w; V.z+=w;

 MOBR = ObrMatr(M);

 A0 = MOBR * U;
 A1 = MOBR * V;

 red.ax=A0.x; red.axx=A0.y; red.axy=A0.z;
 red.ay=A1.x; red.ayx=A1.y; red.ayy=A1.z;
 //printf("red %f %f %f %f %f %f\n",
 //       red.ax, red.ay, red.axx, red.axy, red.ayx, red.ayy);
}



double get_zvvel(REDZVVEL &rzv, double e)
{
  double zvvel;
  zvvel = rzv.a0 + rzv.a1 * (-2.5*log10(e));
  return zvvel;
}

void get_reduction_zvvel(OMRDATA &o)
{
  int i,j,N,Nbrak,imaxdm;
  double S,dm,sko,maxdm;

  for (i=0; i<o.izmkzv; i++) {
    o.izmzv[i].brak_zvvel = 0;
  }
  Nbrak = 0;

rept:
  S=0; N=0;

  for (i=0; i<o.izmkzv; i++) {
    if (o.izmss[i]!=-1 && ! o.izmzv[i].brak_zvvel) {
      j = o.izmss[i];
      S += o.catzv[j].v + 2.5*log10(o.izmzv[i].e);
      N++;
    }
  }

  o.rzv.a0 = S/N;
  o.rzv.a1 = 1;

  S=0; N=0; maxdm=0;

  for (i=0; i<o.izmkzv; i++) {
    if (o.izmss[i]!=-1 && ! o.izmzv[i].brak_zvvel) {
      j = o.izmss[i];
      dm = get_zvvel(o.rzv, o.izmzv[i].e) - o.catzv[j].v;
      S += dm*dm;
      N++;
      if (fabs(dm) > maxdm) {
        maxdm = fabs(dm);
        imaxdm = i;
      }
    }
  }
  sko = sqrt(S/N);

  if (maxdm > 3*sko) {
     o.izmzv[imaxdm].brak_zvvel = 1; 
     Nbrak++;
     goto rept;
  }  

  o.rzv.Nzv = N;
  o.rzv.Nbrak = Nbrak;
  o.rzv.sko = sko;

}


int Otogdestv(OMRDATA &o)
{
 double x,y, Centrx, Centry;
 int pz;
 clear_redomr(o.red);
 if (!Kletka(o,&x,&y)) return 0;
 StarsInKletka(o,x,y, &Centrx, &Centry);
 o.red.ax-=Centrx;
 o.red.ay-=Centry;

 for(int i=0;i<3;i++) {
   REDOMR &r=o.red; int i,j,k; double dx,dy;
   pz=StarsInCircle(o);
   if (pz<3) break;
   get_reduction_OMR(o,0,0);
   to_stdout=0;
   printf("omrkzv=%d\n",o.omrkzv);
   printf(" ax=%f ay=%f axx=%f axy=%f ayx=%f ayy=%f\n",
            o.red.ax, o.red.ay, o.red.axx, o.red.axy, o.red.ayx, o.red.ayy);
   to_stdout=1;
 }

 get_reduction_zvvel(o);

 return pz;
}


//#undef H


void get_catzv(int ntk, double xmin, double xmax, double ymin, double ymax, double magmax)
{
 OMRDATA &o=omrdata[ntk];
 double time=o.time, A,H, alfa,delta, 
        rx,ry,rad, a,d,v,x,y;
 double year=(JDsea-2451545.0)/365.25;
 int catkzv=0;
 CATSTAR star;
 CATZV *catzv=o.catzv;
 extern int test_ko_flag;

 if (!zktflag) { o.catzv=0; return; }
 VectSfer(o.otkpos.z,&A,&H);
 MinusRefraction(&H);
 AHadsea(A,H,time,&alfa,&delta);
 VidKatsea(time,&alfa,&delta);

 rx = max(fabs(xmin),fabs(xmax));
 ry = max(fabs(ymin),fabs(ymax));
 rad = sqrt(rx*rx + ry*ry) / pasF(ntk);
 startreadcircle(zkt,alfa,delta,rad);

 while(1) {
   if (test_ko_flag&&catkzv==0) {
     a=alfa+0.1*GR; d=delta+0.1*GR; v=8;
     printf("Тестовый объект: a=%f d=%f\n",a,d);
   } else {
     if (!readstar(zkt,star)) break;
     a=star.alfa+star.valfa*year; d=star.delta+star.vdelta*year;
     v=star.mag;
   }

   if (v > magmax) continue;
   if (duga(alfa,delta,a,d)>rad) continue;

   KatVidsea(time,&a,&d);
   adAHsea(a,d,time,&A,&H);
   PlusRefraction(&H);
   GorTV(o.otkpos,A,H,&x,&y);
   if (x>xmin && x<xmax && y>ymin && y<ymax) {
     catzv[catkzv].N=star.N;
     catzv[catkzv].x=x;
     catzv[catkzv].y=y;
     catzv[catkzv].v=v;
     catkzv++;
     if (catkzv==o.maxcatkzv) break;
   }
 }
 o.catkzv=catkzv;
 to_stdout=0;
 printf("NTK=%d catkzv=%d\n",ntk,catkzv);
 to_stdout=1;
// for(int i=0;i<catkzv;i++)
//   printf("x=%f y=%f v=%f\n",catzv[i].x,catzv[i].y,catzv[i].v);
}

int printOMR(int NTK)
{
  OMRDATA &o=omrdata[NTK];
  printf("Результаты ОМР в ТК %d:\n",NTK);
  printf("каталожных звезд %d, из них отождествлено %d;\n",
         o.catkzv, o.omrkzv);
  if (o.omrkzv>0) {
    double dx=o.red.ax, dy=o.red.ay, d=sqrt(dx*dx+dy*dy);
    printf("сдвиг по x: %5.2fмм, по y: %5.2fмм, общий: %5.2fмм = %5.2f\"\n",
           dx, dy, d, d/pasF(NTK)/M_PI*180*3600 );
  }
  return 1;
}

int print2OMR(int NTK)
{
  OMRDATA &o=omrdata[NTK];
  extern int KZVAPOI[3];

  printf("ОМР в OТК-%c: ",otk_letter(TIPST,NTK));
  printf("Nзв измер %d кат %d распозн %d  ",KZVAPOI[NTK], o.catkzv, o.omrkzv);
  if (o.omrkzv>0) {
    double dx=o.red.ax, dy=o.red.ay, d=sqrt(dx*dx+dy*dy);
    printf("сдвиг %2.0f\"",d/pasF(NTK)/M_PI*180*3600 );
  }
  printf("\n");
  if (o.omrkzv<=5) 
    printf("Звезды не распознаны\n");
    
  return 1;
}

int korrxyOMR(int NTK, double *x, double *y)
{
  OMRDATA &o=omrdata[NTK];
  if (o.omrkzv==0) return 0;
  redomr(o.red,*x,*y,x,y);
  return 1;
}

int inverse_korrxyOMR(int NTK, double *x, double *y)
{
  OMRDATA &o=omrdata[NTK];
  if (o.omrkzv==0) return 0;
  inverse_redomr(o.red,*x,*y,x,y);
  return 1;
}



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



void analiz_omr(int ntk)
{
  OMRDATA &o=omrdata[ntk];
  int izmkzv=o.izmkzv, catkzv=o.catkzv;
  CATZV *catzv=o.catzv; IZMZV *izmzv=o.izmzv;
  int *catss=o.catss, *izmss=o.izmss;
  REDOMR &red=o.red;
  int i,j,n=0;
  double xred,yred, dx,dy,r, maxdx=0, maxdy=0, maxr=0, Sx=0, Sy=0, Sr=0;
  double mcat, mizm, dm, maxdm=0, Sm=0;
  double KK=0;

  for (i=0;i<izmkzv;i++) {
	j = izmss[i];
	if (j!=-1) {
	  double m=catzv[j].v, V=izmzv[i].e;
      KK += m + 2.5*log10(V);
	  n++;
	}
  }
  KK/=n;

  n=0;
  for (i=0;i<izmkzv;i++) {
    redomr(red,izmzv[i].x,izmzv[i].y,&xred,&yred);
    mizm = KK - 2.5*log10(izmzv[i].e);
	j = izmss[i];
	if (j!=-1) {
	  mcat = catzv[j].v;
	  dm = mizm - mcat;
      dx=xred-catzv[j].x;
      dy=yred-catzv[j].y;
	  r = sqrt(dx*dx+dy*dy);
	  if (fabs(dx)>maxdx) maxdx = fabs(dx);
	  if (fabs(dy)>maxdy) maxdy = fabs(dy);
	  if (r>maxr) maxr = r;
	  if (fabs(dm)>maxdm) maxdm = fabs(dm);
	  Sx += dx*dx;
	  Sy += dy*dy;
	  Sr += r*r;
	  Sm += dm*dm;
	  n++;
	  printf("X=%7.2f Y=%7.2f V=%9.1f mizm=%5.2f mcat=%5.2f dm=%5.2f dx=%6.3f dy=%6.3f\n",
         izmzv[i].x, izmzv[i].y, izmzv[i].e, mizm, mcat, dm, dx, dy);
	} else {
	  printf("X=%7.2f Y=%7.2f V=%9.1f mizm=%5.2f\n",
         izmzv[i].x, izmzv[i].y, izmzv[i].e, mizm);
	}
 }
 Sx = sqrt(Sx/n);
 Sy = sqrt(Sy/n);
 Sr = sqrt(Sr/n);
 Sm = sqrt(Sm/n);

 printf("------------------------------------------------------------------------\n");
 printf("Результаты ОМР NTK=%d\n",ntk);
 printf("Измерено звезд %d  распознано звезд %d\n", o.izmkzv, o.omrkzv);
 printf("СКО невязок по X    %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
	 Sx, Sx/pasSX(ntk)/pasbin(ntk), Sx/pasF(ntk)*RG*3600);
 printf("СКО невязок по Y    %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
	 Sy, Sy/pasSY(ntk)/pasbin(ntk), Sy/pasF(ntk)*RG*3600);
 printf("СКО невязок общ.    %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
	 Sr, Sr/pasSX(ntk)/pasbin(ntk), Sr/pasF(ntk)*RG*3600);
 printf("макс.невязка по X   %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
	 maxdx, maxdx/pasSX(ntk)/pasbin(ntk), maxdx/pasF(ntk)*RG*3600);
 printf("макс.невязка по Y   %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
	 maxdy, maxdy/pasSX(ntk)/pasbin(ntk), maxdy/pasF(ntk)*RG*3600);
 printf("макс.невязка общ.   %5.3f мм = %5.3f пикс. = %5.3f угл.сек.\n",
	 maxr, maxr/pasSX(ntk)/pasbin(ntk), maxr/pasF(ntk)*RG*3600);
 printf("СКО dm  %5.3f\n", Sm);
 printf("max dm  %5.3f\n", maxdm);
 printf("------------------------------------------------------------------------\n");
}
