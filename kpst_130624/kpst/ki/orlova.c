#define kgravpz 398600.44
#define kvvrzrad 7.2921151467e-5


/* расчет угла по его синусу и косинусу */
double ratan2 (double sinx,double cosx)
{
double s,arctgx;
if (cosx==0) cosx=1.e-10;
s=atan(sinx/cosx);
arctgx=cosx>0. ? (s>0. ? s : 2*M_PI+s) : M_PI+s;
return arctgx;
}


/*  расчет эксцентрической аномалии  по средней m и эксцентриситету ехс */
/*  eh-начальное приближение экс.аномалии(eh=m)*/
double eanom(double m,double exc,double eh)
{int i=0;
 double e1,a,ee1,e;
 e=eh;
 do
 {if(i++>100) break;
  e1=e;
  a=1.-exc*cos(e1);
  e=e1-(e1-exc*sin(e1)-m)/a;
  ee1=e-e1>0 ? e-e1:e1-e;
 }
 while(ee1>1.e-15);
 return e;
}

/*расчет средней аномалии m по эксцентрической e и эксцентриситету exc */
/*процедуру добавил Титенко "для полноты" 26/10/99 */
void sranom(double e, double exc, double *m)
{
  *m=e-exc*sin(e);
}

/* расчет эксцентрической аномалии e по истинной v и эксцентриситету f*/
void ekansm(double v,double f,double *e,int *nzp)
{
double q;
int ivc;
*nzp=0;
if (f<=1.)
{
q=sqrt(1.-f*f)*sin(v);
*e=ratan2(q,f+cos(v));
if (v<0.)
*e=*e-(2*M_PI);
ivc=v/(2*M_PI);
*e=*e+ivc*(2*M_PI);
}
else
*nzp=1;
return;
}


/*  расчет истинной аномалии v по эксцентрической e и эксцентриситету ехс */
void isankm(double e,double exc,double *v,int *nzp)
{
double q;
int iec;
*nzp=0;
if(exc<1.)
{
 q=sqrt(1.-exc*exc)*sin(e);
 *v=ratan2(q,cos(e)-exc);
 if(e<0) *v=*v-(2*M_PI);
 iec=e/(2*M_PI);
 *v=*v+(2*M_PI)*iec;
}
else
*nzp=1;
return;
}

/* ПЕРЕВОД ПАРАМЕТРОВ В ГНСК В КЕПЛЕРОВСКИЕ ЭЛЕМЕНТЫ*/
void elorko(double x,double y,double z,double dx,double dy,double dz,
double t,double *a,double *exc,double *dolv,double *omega,double *ai,
double *u,double *tp,int *nzp)
/* ВХОД:x,y,z,dx,dy,dz-параметры  в гнск(км,км/сек) на момент t(сек)*/
/* ВЫХОД:a-б.полуось(км); exc-эксцентриситет(б/р);dolv-долгота восх.узла*/
/*       omega-аргумент перигея(рад),ai-наклонение(рад),*/
/*       u-аргумент широты(рад),*/
/*       tp-время прохождения перицентра(сек),nzp-признак завершения*/
{
double ak,r,vkv,skr,v1,pivo,ux,uy,uz,p,pk,vx,vy,vz,sinr,cosr,v,e,
sine,cosl,sinl,al,sinv,cosv;
ak=sqrt(kgravpz);
*nzp=0;
r=sqrt(x*x+y*y+z*z);
vkv=(dx*dx+dy*dy+dz*dz);
*a=1./(2./r-vkv/kgravpz);
if (*a<=0) {*nzp=1; return;} //контроль гиперболич.орбиты (Титенко)
skr=x*dx+y*dy+z*dz;
v1=skr/r;
pivo=(1.-r/(*a))*(1.-r/(*a))+skr*skr/(*a*kgravpz);
if (pivo<=0||pivo>=1) {*nzp=1; return;} //контроль гиперболич.орбиты (Титенко)
*exc=sqrt(pivo);
uy=y/r;
ux=x/r;
uz=z/r;
p=*a*(1.-*exc**exc);
pk=sqrt(p)*ak;
vx=(r*dx-x*v1)/pk;
vy=(r*dy-y*v1)/pk;
vz=(r*dz-z*v1)/pk;
*u=ratan2(uz,vz);
sinr=sqrt(uz*uz+vz*vz);
cosr=sqrt((ux+vy)*(ux+vy)+(uy-vx)*(uy-vx))-1.;
*ai=ratan2(sinr,cosr);
cosl=(ux+vy)/(1.+cosr);
sinl=(uy-vx)/(1.+cosr);
al=ratan2(sinl,cosl);
*dolv=al-*u;
if(*dolv<0)
   *dolv=*dolv+(2*M_PI);
sinv=v1/ak*sqrt(p); // удалил деление на эксцентриситет
cosv=p/r-1.;           // в этих двух строках (Титенко)
v=ratan2(sinv,cosv);
*omega=*u-v;
if(*omega<0) *omega=*omega+(2*M_PI);
v=v-(2*M_PI);
ekansm(v,*exc,&e,nzp);
*tp=t-(e-*exc*sin(e))/ak**a*sqrt(*a);
return;
}

 /* РАСЧЕТ ПО КЕПЛЕРОВСКИМ ЭЛЕМЕНТАМ ГЕОЦЕНТР. ПАР-РОВ НА ЗАДАННЫЙ МОМЕНТ */
void keplergnsk(double el[],double tcek,double gnsk[])
/* вход:-el[7]-кеплеровские эл-ты:
	      el[0]-время на момент задания эл-тов;
	      el[1]-б полуось[km];
	      el[2]-эксцентриситет;
	      el[3] -наклонение;
	      el[4]-долгота восх. узла;
	     el[5]-аргумент перигея[рад];
	    el[6]-истинная аномалия на момент el[0];
	  tcek -время на момент прогноза [cek].
выход:-gnsk[7]-параметры x,y,z,dx,dy,dz-gnsk[1-6];gnsk[0]=tcek;[km,km/cek]. */
{
double ak,e0,m0,v,am,e,u,p,su,cu,sdol,cdol,si,ci,sk,r,rab,dv,dr;
int nzp;
ak=sqrt(kgravpz);
ekansm(el[6],el[2],&e0,&nzp);
m0=e0-el[2]*sin(e0);
am=m0+ak/(el[1]*sqrt(el[1]))*(tcek-el[0]);
e=eanom(am,el[2],am);
isankm(e,el[2],&v,&nzp);
u=v+el[5];
p=el[1]*(1.-el[2]*el[2]);
su=sin(u);
cu=cos(u);
sdol=sin(el[4]);
cdol=cos(el[4]);
si=sin(el[3]);
ci=cos(el[3]);
sk=1.+el[2]*cos(v);
r=p/sk;
gnsk[1]=r*(cu*cdol-su*sdol*ci);
gnsk[2]=r*(cu*sdol+su*cdol*ci);
gnsk[3]=r*su*si;
rab=sqrt(p);
dv=ak*rab/(r*r);
dr=ak*el[2]*sin(v)/rab;
gnsk[4]=dr*gnsk[1]/r-r*dv*(su*cdol+cu*sdol*ci);
gnsk[5]=dr*gnsk[2]/r-r*dv*(su*sdol-cu*cdol*ci);
gnsk[6]=dr*gnsk[3]/r+r*dv*cu*si;
gnsk[0]=tcek;
return;
}
