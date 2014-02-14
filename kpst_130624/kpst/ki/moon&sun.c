	      void eklluna(double jd0, long tb, double &dolg, double &shir)

      {         /* Расчет эклиптических координат Луны.

	      Вход: jd0 - юлианская дата на Гринв.полночь (полуцелая)
		     тв - время (сек.) от Гринв. полночи
             Выход: dolg- долгота Луны (рад.).
                    shir- широта Луны.

                */
                double k1,k2,t,l,f,d,lr1,lr,jd1,x,y;
                double const  p=2.*M_PI,  r=M_PI/180.,   r1=M_PI/(180.*60.),
                             r2=M_PI/(180.*60.*60.);
		  jd1=jd0-2415020.;
		  // tb=tb-(1+tchaspojasmest)*3600.+dolgotak*43200./M_PI; //??
		    t=(jd1+(tb/*+dtluna*/)/(3600.*24.))/36525.;

		  l   =270.*r+26.*r1+2.99*r2+1732564379.31*r2*t-4.08*r2*t*t;
		  f   =11.*r+15.*r1+3.2*r2+1739527290.54*r2*t-11.56*r2*t*t;
		  d   =350.*r+44.*r1+14.95*r2+1602961611.18*r2*t-5.17*r2*t*t;
		  lr  =296.*r+6.*r1+16.59*r2+1717915856.79*r2*t+33.09*r2*t*t;
		  lr1 =358.*r+28.*r1+33.*r2+129596579.1*r2*t-0.54*r2*t*t;

		  dolg=6.289*sin(lr)-1.274*sin(lr-2.*d)+
		       0.658*sin(2.*d)+0.214*sin(2.*lr)-0.186*sin(lr1)-
		       0.114*sin(2.*f)-0.059*sin(2.*lr-2.*d)-
		       0.057*sin(lr1+lr-2.*d)+0.053*sin(lr+2.*d)-
		       0.046*sin(lr1-2.*d)+0.041*sin(lr-lr1)-0.035*sin(d)-
		       0.03*sin(lr+lr1);
		  dolg=dolg*r+l;
		  dolg=dolg/p;
		     y=modf(dolg,&x);
		  dolg=y*p;

		  shir=5.128*sin(f)+0.281*sin(lr+f)-
		       0.278*sin(f-lr)-0.173*sin(f-2.*d)+
		       0.055*sin(f+2.*d-lr)-0.046*sin(lr+f-2.*d)+
		       0.033*sin(f+2.*d);
		  shir=shir*r;
      }

 /*********************************************************************
 *			eklsoln.cpp				      *
 *								      *
 *  В этом файле описана процедура eklkoordsolnca		      *
 *        Рассчет эклиптической долготы Солнца.			      *
 *        Вход:  jd2- юлианская   дата (дни, длинная с 2450... ).     *
 *               tv- время (сек.).                                    *
 *        Выход: долгота Солнца (рад.).                               *
 *								      *
 *								      *
 *  (c) Copyright 1997, Подольский А.  НТЦ ОАО "Красногорский завод"  *
 **********************************************************************/
	   double  eklkoordsolnca( double jd2, long tv)


     {        double  const  k1=180.0*60.0*60.0;
	      double  l1,l2,t,dolg,jd1;
			   double x,y;
//	 t= tv-(tchaspojasmest+1)*3600.+dolgotak*86400./(M_PI*2); //!!!!!
		 t = tv;
		 t= t/86400.0;
	       jd1= jd2-2415020.;
		 t= (jd1+t)/36525.0;
		l1= M_PI+99.696678*M_PI/180.0+129602768.13*M_PI*t/k1+
		    M_PI*1.089*t*t/k1;
		l2= 358.*M_PI/180.0+28*M_PI/(180.*60.)+33.*M_PI/k1+
		    129596579.1*M_PI*t/k1-0.54*M_PI*t*t/k1;
	      dolg= l1+1.9171*M_PI*sin(l2)/180.+
		    0.02*M_PI*sin(2.*l2)/180.+0.0003*M_PI*sin(3.*l2)/180.;
	      dolg= dolg/(2.*M_PI);
		 y= modf(dolg,&x);
			 return (y*2.*M_PI);
      }

double jd0;

       void eklvekv(double l, double b, double &alf, double &del)
   {
     /*  Перевод эклиптических координат в экваториальные.
	 Вход:  l   - эклиптическая долгота. (рад.)
		b   - эклиптическая широта. (рад.)
	 Выход: alf - прямое восхождениею (рад.)
		del - склонение (рад.)
	    Используемые глобалы: jd.
      */
	  double y,x;
	  double ce,se,cb,sb,sl,sih,koc,cal,sal,e,jd1,t1;
	  double  const  t =0.003*M_PI/180.0;  // точность сравнения с особыми
                                            // точками.
            jd1=jd0-2415020.0;
             t1=jd1/36525.0;
              e=23.452294-0.0130125*t1-0.00000164*t1*t1;   // e - в градусах
              e=e*M_PI/180.0;                           // е >> в радианы.
                l=l/(2.*M_PI);
		y=modf(l,&x);
                l=y*2.*M_PI;
	{ if ((fabs(M_PI_2-l)<t) && (fabs(M_PI_2-e-b)<t))
	     { del=M_PI_2; alf=0.0; }
	  else { if ( fabs(-M_PI_2-l)<t && fabs(-M_PI_2+e-b)<t)
			  {del=-M_PI_2; alf=0.0; }
		 else {
			 sih=(cb=cos(b))*(sl=sin(l))*
			     (se=sin(e))+(sb=sin(b))*(ce=cos(e));
			 del=(fabs(sih)>=1.0)?(M_PI_2*((sih>0.0)?
			     (1.0):(-1.0))):(asin(sih));
			 koc=sqrt(1.0-sih*sih);
			 cal=cb*cos(l)/koc;
			 sal=(cb*sl*ce-sb*se)/koc;
			 sih=(fabs(cal)>=1.0)?(M_PI_2-M_PI_2*((cal>0.0)?
			     (1.0):(-1.0))):(acos(cal));
			 alf=(sal>=0.0)?(sih):(2.*M_PI-sih);
		       }
	       }
	}
    }

void adluna(double JD, double t, double *a, double *d)
{
  double dol,shir;
  eklluna(JD,t,dol,shir);
  eklvekv(dol,shir,*a,*d);
}

void adsoln(double JD, double t, double *a, double *d)
{
  double dol;
  dol=eklkoordsolnca(JD,t);
  eklvekv(dol,0,*a,*d);
}
