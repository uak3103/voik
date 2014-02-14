void NAVED (int NTK,
        VECT3 VO, VECT3 VX,
        double XT, double YT,
        double *A, double *L, double *R)
{
 VECT3 VY, VR, VL, VA, VO0;
 double Az, H;
 double k = paskolvos(NTK)/180*M_PI + XT/pasF(NTK);
 Norm(VO);
 Norm(VX);
 VY=VO^VX;
 VA.x=0; VA.y=0; VA.z=1;
 VR  = VX*cos(k) + VO*sin(k);
 VO0 = VX*(-sin(k)) + VO*cos(k);
 VL = VR^VA;
 VL = VL/sqrt(VL*VL);
 VectSfer(VL, &Az, &H);
 *A = Az*(180/M_PI) +pasZDA(NTK)-pasohZDA(NTK);
 if (*A>180) *A-=360;
 VectSfer(VR, &Az, &H);
 *L = 90 + H*(180/M_PI) +pasZDL(NTK)-90-pasohZDL(NTK);
 *R = (acos(VL*VO0)-YT/pasF(NTK)) * (180/M_PI)
      +pasZDR(NTK)-90-pasohZDR(NTK);
 while(1) {
  VECT3 Vosh=absmetd(NTK,*A,*L,*R,XT,YT)-VO;
  double dx=Vosh*VX,dy=Vosh*VY,
         dR=-dy*(180/M_PI), dL=dx/sin(*R*(M_PI/180))*(180/M_PI);
  *R+=dR; *L+=dL;
  if (fabs(dx)<1E-10&&fabs(dy)<1E-10) break;
 }
}

// enum { ZENIT, POLUS, RAVND };

void Naved (int NTK, double Az, double H,
	    int tip, double razvorot, double XT, double YT,
	    double *A, double *L, double *R)
{
 VECT3 VO=SferVect(Az,H), P, VX, VY;
 P.x=0; P.y=0; P.z=1;
 if (tip==RAVND) { P.x=1; P.y=0; P.z=0; }
 if (tip==POLUS||tip==RAVND) Ekv1Gorv(P,SHIROTA);
 VX = P-VO*(VO*P);
 VX = VX/sqrt(VX*VX);
 VY = VO^VX;
 VX = VX*cos(razvorot*(M_PI/180)) + VY*sin(razvorot*(M_PI/180));
 NAVED (NTK,VO,VX,XT,YT,A,L,R);
// double a,h,da,dh;
// absmet(NTK,*A,*L,*R,XT,YT,&a,&h);
// da=Az-a; dh=h-H;
}

//Спец. наведение на площадку вблизи полюса мира
//с определенной ориентацией
void naved_polus(int NTK, double jd, double t,
       double *A, double *L, double *R)
{

  double mzv,Az,H;
  VECT3
    vo={-0.0010531073, 0.0093818809, 0.9999201607},
    vx={ 0.8551580722,-0.5183343163, 0.0058487738};
//  extern MATR33 NP,NPT;
//  extern VECT3 Vaber;
//  void init_reduction(double JD);
//  void current_jd_t(double *jd, double *t);

  //current_jd_t(&jd,&t);
  init_reduction(jd);
  mzv=MZV(jd,t);
  vo=NP*vo+Vaber; Ekv2Gorv(vo,SHIROTA,mzv);
  VectSfer(vo,&Az,&H);
  PlusRefraction(&H);
  vo=SferVect(Az,H);
  vx=NP*vx; Ekv2Gorv(vx,SHIROTA,mzv);
  NAVED (NTK,vo,vx,0,0,A,L,R);
}
