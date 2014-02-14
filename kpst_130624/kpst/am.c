enum {Xos,Yos,Zos };   /* оси вращения */
double Xor,Yor,Zor;    /* прямоугольная сис. кооринат*/

void POVOROT(double FI,int os)
 { double C,S,R;
   C=cos(FI);
   S=sin(FI);
   switch(os) { case Xos: R=Yor;
                         Yor=R*C+Zor*S;
                         Zor=Zor*C-R*S;
                         break ;
               case Yos: R=Xor;
                         Xor=R*C-Zor*S;
                         Zor=Zor*C+R*S;
                         break ;
               case Zos: R=Xor;
                         Xor=R*C+Yor*S;
                         Yor=Yor*C-R*S;
			 break ;
              }
 }

VECT3 absmetd(int NTK,
	      double A, double L, double R,
	      double XT, double YT)
{
 VECT3 v;
 double  pom1,pom2;

 A=( A-pasZDA(NTK)+pasohZDA(NTK))*(M_PI/180);
 L=(-L+pasZDL(NTK)+pasohZDL(NTK))*(M_PI/180);
 R=(-R+pasZDR(NTK)+pasohZDR(NTK))*(M_PI/180);

  if (pasflipX(NTK)!=0) XT = -XT;
  if (pasflipY(NTK)!=0) YT = -YT;

  Xor=XT/pasMASH(NTK)/pasF(NTK);
  Yor=YT/pasMASH(NTK)/pasF(NTK);
  Zor=1.0;
  POVOROT(-pasdefX(NTK)*(M_PI/180)*sin(L),Yos);
  POVOROT(-pasdefY(NTK)*(M_PI/180)*cos(L)*sin(R),Xos);

  POVOROT(-pasrazv(NTK)*(M_PI/180),Zos);
  POVOROT(-paskolvos(NTK)*(M_PI/180),Yos);
  POVOROT(-R,Xos);
  POVOROT(-pasKRL(NTK)*(M_PI/180),Zos);
  POVOROT(-L,Yos);
  POVOROT(-pasKLA(NTK)*(M_PI/180),Xos);
  POVOROT(-A,Zos);
  POVOROT(-pasAotC(NTK)*(M_PI/180),Yos);
  POVOROT(-pasAotZ(NTK)*(M_PI/180),Xos);
  POVOROT(-M_PI_2,Zos);
  v.x=Xor; v.y=Yor; v.z=Zor;
  return v;
}

void absmet(int NTK,
	    double A, double L, double R,
	    double XT, double YT,
	    double *Az,double *H)
{
  VECT3 v=absmetd(NTK,A,L,R,XT,YT);
  VectSfer(v,Az,H);
}

// struct OTKPOS { VECT3 x,y,z; };

void getOTKpos(int NTK, double A, double L, double R,
	       OTKPOS &p)
{ VECT3 v,vx,vy; double d=pasF(NTK);
  v= absmetd(NTK,A,L,R,0,0);
  vx=absmetd(NTK,A,L,R,d,0);
  vy=absmetd(NTK,A,L,R,0,d);
  p.z=v;
  vx=vx-v*(v*vx); p.x=vx/(vx*vx)*d;
  vy=vy-v*(v*vy); p.y=vy/(vy*vy)*d;
}

void GorTV(OTKPOS &p, double Az, double H, double *X, double *Y)
{ VECT3 v=SferVect(Az,H);
  double q=v*p.z;
  *X=(v*p.x)/q;
  *Y=(v*p.y)/q;
}

void AHXY(int NTK, double A, double L, double R,
                            double Az, double H, double *X, double *Y)
{ OTKPOS p;
  getOTKpos(NTK,A,L,R,p);
  GorTV(p,Az,H,X,Y);
}
