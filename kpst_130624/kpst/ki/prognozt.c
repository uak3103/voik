#define dvapi  6.28318530717958647692
#define pi     3.14159265358979323846
#define sqrtmu 631.348113
#define C20    -1.082635e-3
#define RZ     6378.137

void shortperiodics(
    double *a, double *e, double *i, double *Om, double *om, double *u,
    int ADD);

/****************************************************************************/
/*  Прогноз с учетом 2-й зональной гармоники и атмосферы.                   */
/*                                                                          */
/*  Входные параметры.                                                      */
/*  a,e,i,Om,om,u - оскулирующие параметры: бол.п-ось(км), эксцентриситет,  */
/*  наклонение(рад), долг.восх.узла(рад), арг.перигея(рад), арг.широты(рад) */
/*  dT - падение периода за виток (сек)                                     */
/*  t - время от момента привязки параметров (сек)                          */
/*                                                                          */
/*  Выходные параметры.                                                     */
/*  X,Y,Z,VX,VY,VZ  - координаты и скорости в ГНСК на время t (км,км/сек)   */
/****************************************************************************/
void prognoz_T(
        double a, double e, double i, double Om, double om, double u,
        double dT, double t,
        double *X, double *Y, double *Z, double *VX, double *VY, double *VZ)
{
    double v,E,dE,M0,M,dM,T,t0,tN,Hper,p,n,K,r,vr,vt,
           sin_i,cos_i,sin_Om,cos_Om,sin_u,cos_u,
           Mx,My,Mz, Ux,Uy,Uz, Vx,Vy,Vz;
    int N,k=0;

    shortperiodics(&a,&e,&i,&Om,&om,&u,0);

    v=u-om;
    E=2*atan(sqrt((1-e)/(1+e))*tan(v/2));
    M0=E-e*sin(E);
    if (M0<0) M0+=dvapi;
    p=a*(1-e*e);
    Hper=a*(1-e);
    n=sqrtmu/(a*sqrt(a));
    cos_i = cos(i);
    if (dT!=0) {
        T=dvapi/n;
        t0=-M0/dvapi*T;
        N=floor((t-t0)/T);
        tN=t0+T*N-dT*N*(N-1)/2;
        T-=N*dT;
        while(t-tN>T) { N++; tN+=T; T-=dT; if (++k==100) break;}
        M = (t-tN)/T*dvapi;
        dM = M-M0+N*dvapi;
        a=pow(sqrtmu*T/dvapi,0.66666666666666667);
        e=1-Hper/a;
        if (e<0) e=0;
    } else {
        dM=n*t;
        M=M0+dM;
    }
    K = 1.5*C20*RZ*RZ/(p*p);
    Om += K*cos_i*dM;
    om -= 0.5*K*(5*cos_i*cos_i-1)*dM;
    M=fmod(M,dvapi);
    E=M;
    k=0;
cycle:
    dE=(E-e*sin(E)-M)/(1-e*cos(E));
    if (dE>1) dE=1; else if (dE<-1) dE=-1;
    E-=dE;
    if (fabs(dE)>1e-10 && ++k<100) goto cycle;
    v=2*atan(sqrt((1+e)/(1-e))*tan(E/2));
    u=om+v;
    shortperiodics(&a,&e,&i,&Om,&om,&u,1);
    v=u-om;
    E=2*atan(sqrt((1-e)/(1+e))*tan(v/2));
    r=a*(1-e*cos(E));
    vr=sqrtmu*sqrt(a)/r*e*sin(E);
    vt=sqrtmu*sqrt(a*(1-e*e))/r;
    sin_i=sin(i);    cos_i=cos(i);
    sin_Om=sin(Om);  cos_Om=cos(Om);
    sin_u=sin(u);    cos_u=cos(u);
    Mx=-sin_Om*cos_i;
    My= cos_Om*cos_i;
    Mz= sin_i;
    Ux=Mx*sin_u+cos_Om*cos_u;
    Uy=My*sin_u+sin_Om*cos_u;
    Uz=Mz*sin_u;
    Vx=Mx*cos_u-cos_Om*sin_u;
    Vy=My*cos_u-sin_Om*sin_u;
    Vz=Mz*cos_u;
    *X=r*Ux;
    *Y=r*Uy;
    *Z=r*Uz;
    *VX=vr*Ux+vt*Vx;
    *VY=vr*Uy+vt*Vy;
    *VZ=vr*Uz+vt*Vz;
}

/****************************************************************************/
/*  Процедура учета короткопериодических возмущений.                        */
/*  a,e,i,Om,om,u  -  кеплеровы параметры,                                  */
/*  ADD=1  -  прибавить короткопериодические возмущения,                    */
/*  ADD=0  -  вычесть короткопериодические возмущения.                      */
/****************************************************************************/
void shortperiodics(
    double *a, double *e, double *i, double *Om, double *om, double *u,
    int ADD)
{
double l,h,p,v,r,K,
       l2, h2, lh,
       sin_i, cos_i, sin_i2, sin_2i,
       sin_u, sin_2u, sin_3u, sin_4u, sin_5u,
       cos_u, cos_2u, cos_3u, cos_4u, cos_5u,
       da, di, dOm, dl, dh, du;

    l=(*e)*cos(*om);
    h=(*e)*sin(*om);
    lh=l*h;
    l2=l*l;
    h2=h*h;

    p=(*a)*(1-(*e)*(*e));
    v=*u-*om;
    r=p/(1+(*e)*cos(v));
    K=1.5*C20*RZ*RZ/(p*p);

    sin_i=sin(*i);
    cos_i=cos(*i);
    sin_i2=sin_i*sin_i;
    sin_2i=2*sin_i*cos_i;

    sin_u=sin(*u);
    cos_u=cos(*u);
    cos_2u=2*cos_u*cos_u-1;
    sin_2u=2*cos_u*sin_u;
    cos_3u=cos_2u*cos_u-sin_2u*sin_u;
    sin_3u=cos_2u*sin_u+sin_2u*cos_u;
    cos_4u=2*cos_2u*cos_2u-1;
    sin_4u=2*cos_2u*sin_2u;
    cos_5u=cos_4u*cos_u-sin_4u*sin_u;
    sin_5u=cos_4u*sin_u+sin_4u*cos_u;

    da=-C20*RZ*RZ*(*a)*(*a)/(r*r*r)*(1-3*sin_i2*sin_u*sin_u);

    di=-0.25*K*sin_2i*(cos_2u+l*cos_u-h*sin_u+(l*cos_3u+h*sin_3u)/3);
    dOm=K*cos_i*(-0.5*sin_2u+0.5*l*sin_u-1.5*h*cos_u-(l*sin_3u-h*cos_3u)/6);
    dl=-K*(
        (1+0.25*l2+2.25*h2)*cos_u-lh*sin_u+0.5*l*cos_2u+h*sin_2u+
        (l2-3*h2)*cos_3u/12+lh*sin_3u/3+
        sin_i2*(-(1.25-0.375*l2+3.125*h2)*cos_u+0.25*lh*sin_u+
                0.5*l*cos_2u-2*h*sin_2u+(28+11*l2+25*h2)/48*cos_3u-
                7*lh*sin_3u/24+0.375*l*cos_4u+0.375*h*sin_4u+
                0.0625*(l2-h2)*cos_5u+0.125*lh*sin_5u) );
    dh=-K*(
        (1-1.25*l2+0.25*h2)*sin_u-2*lh*cos_u-0.5*h*cos_2u-
        (l2+h2)*sin_3u/12+
        sin_i2*(-(1.75+1.125*(l2+h2))*sin_u+3.25*lh*cos_u+
                2*h*cos_2u+0.5*l*sin_2u+5*lh*cos_3u/24+
                (28+13*l2+23*h2)*sin_3u/48-0.375*h*cos_4u+0.375*l*sin_4u
                -0.125*lh*cos_5u+0.0625*(l2-h2)*sin_5u) );
    du=K/12*(7*cos_i*cos_i-1)*sin_2u;

    if (ADD) {
	*a+=da;
	l+=dl;
        h+=dh;
	*i+=di;
	*Om+=dOm;
	*u+=du;
    } else {
	*a-=da;
	l-=dl;
        h-=dh;
	*i-=di;
	*Om-=dOm;
	*u-=du;
    }

    *e=sqrt(l*l+h*h);
    *om=atan(h/l); if (l<0) *om+=pi;
}

#undef dvapi
#undef pi
#undef sqrtmu
#undef C20   
#undef RZ
