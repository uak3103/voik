double max(double x, double y)
{ if (x>y) return x; else return y; }

double min(double x, double y)
{ if (x<y) return x; else return y; }

#ifndef _VECT3
#define _VECT3

typedef struct
      {
       double x;
       double y;
       double z;
       } VECT3;

typedef struct
      {
       double m11, m12, m13,
	      m21, m22, m23,
	      m31, m32, m33;
      } MATR33;
#endif


// вычисление длины трехмерного вектора
double dlvect(const VECT3 &v) { return sqrt(v.x*v.x+v.y*v.y+v.z*v.z); }

void Norm(VECT3 &v)
{
  double k=1/dlvect(v);
  v.x*=k; v.y*=k; v.z*=k;
}

// умножение вектора на число
VECT3 operator* (const VECT3 &v, double k)
{ VECT3 v1;
  v1.x=v.x*k; v1.y=v.y*k; v1.z=v.z*k;
  return v1;
}

// деление вектора на число
VECT3 operator/ (const VECT3 &v, double k)  { return v*(1/k); }

// скалярное произвеление двух векторов
double operator* (const VECT3 &v1, const VECT3 &v2)
{ return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z; }

// сумма двух векторов
VECT3 operator+ (const VECT3 &v1, const VECT3 &v2)
{ VECT3 v3;
  v3.x=v1.x+v2.x;v3.y=v1.y+v2.y;v3.z=v1.z+v2.z;
  return v3;
}

VECT3 operator+= (VECT3 &v1, const VECT3 &v2) { return (v1=v1+v2); }

// разность двух векторов
VECT3 operator- (const VECT3 &v1, const VECT3 &v2)
{ VECT3 v3;
  v3.x=v1.x-v2.x;v3.y=v1.y-v2.y;v3.z=v1.z-v2.z;
  return v3;
}

VECT3 operator-= (VECT3 &v1, const VECT3 &v2) { return (v1=v1-v2); }

// транспонирование матрицы 3*3
MATR33 operator~ (const MATR33 &m)
{ MATR33 mt;
  mt.m11 = m.m11; mt.m12 = m.m21; mt.m13 = m.m31;
  mt.m21 = m.m12; mt.m22 = m.m22; mt.m23 = m.m32;
  mt.m31 = m.m13; mt.m32 = m.m23; mt.m33 = m.m33;
  return mt;
}

// определитель матрицы 3*3
double det33(const MATR33 &m)
{ double p;
  p  = m.m11*(m.m22*m.m33-m.m23*m.m32);
  p -= m.m12*(m.m21*m.m33-m.m23*m.m31);
  p += m.m13*(m.m21*m.m32-m.m22*m.m31);
  return p;
}

// обращение матрицы 3*3
MATR33 ObrMatr(const MATR33 &m)
{ MATR33 mo; double det=det33(m);
  mo.m11= (m.m22*m.m33-m.m23*m.m32)/det;
  mo.m12=-(m.m21*m.m33-m.m23*m.m31)/det;
  mo.m13= (m.m21*m.m32-m.m22*m.m31)/det;
  mo.m21=-(m.m12*m.m33-m.m13*m.m32)/det;
  mo.m22= (m.m11*m.m33-m.m13*m.m31)/det;
  mo.m23=-(m.m11*m.m32-m.m12*m.m31)/det;
  mo.m31= (m.m12*m.m23-m.m13*m.m22)/det;
  mo.m32=-(m.m11*m.m23-m.m13*m.m21)/det;
  mo.m33= (m.m11*m.m22-m.m12*m.m21)/det;
 return mo;
}

// умножение матриц 3*3
MATR33 operator* (const MATR33 &m1, const MATR33 &m2)
{
 MATR33 m;

 m.m11 = m1.m11*m2.m11 + m1.m12*m2.m21 + m1.m13*m2.m31;
 m.m12 = m1.m11*m2.m12 + m1.m12*m2.m22 + m1.m13*m2.m32;
 m.m13 = m1.m11*m2.m13 + m1.m12*m2.m23 + m1.m13*m2.m33;

 m.m21 = m1.m21*m2.m11 + m1.m22*m2.m21 + m1.m23*m2.m31;
 m.m22 = m1.m21*m2.m12 + m1.m22*m2.m22 + m1.m23*m2.m32;
 m.m23 = m1.m21*m2.m13 + m1.m22*m2.m23 + m1.m23*m2.m33;

 m.m31 = m1.m31*m2.m11 + m1.m32*m2.m21 + m1.m33*m2.m31;
 m.m32 = m1.m31*m2.m12 + m1.m32*m2.m22 + m1.m33*m2.m32;
 m.m33 = m1.m31*m2.m13 + m1.m32*m2.m23 + m1.m33*m2.m33;

 return m;
}

// умножение матрицы 3*3 на столбец 3*1
VECT3 operator* (const MATR33 &m, const VECT3 &v)
{
 VECT3 vm;
 vm.x = m.m11*v.x+m.m12*v.y+m.m13*v.z;
 vm.y = m.m21*v.x+m.m22*v.y+m.m23*v.z;
 vm.z = m.m31*v.x+m.m32*v.y+m.m33*v.z;
 return vm;
}

// векторное произведение векторов
VECT3 operator^ (const VECT3 v1, const VECT3 v2)
{ VECT3 v;
  v.x = v1.y*v2.z-v1.z*v2.y;
  v.y = v1.z*v2.x-v1.x*v2.z;
  v.z = v1.x*v2.y-v1.y*v2.x;
  return v;
}

// вычисление аргумента двумерного вектора (x,y) : рад
double arg(double x, double y )
{
 double a;
 if ( fabs(x)>fabs(y) ) { if ( x>0 ) a = atan(y/x);
                          else       a = M_PI+atan(y/x);
                        }

 else                   { if ( y>0 ) a = M_PI_2-atan(x/y);
                          else       a = 3*M_PI_2-atan(x/y);
			}
 if ( a<0 ) return(a+2*M_PI);
 return(a);
}

// поворот вектора (x,y) на угол fi ( рад )
void Povorot ( double &x, double &y, double fi)
{ double sinfi=sin(fi), cosfi=cos(fi), x1, y1;
 x1 = x * cosfi + y * sinfi;
 y1 =-x * sinfi + y * cosfi;
 x=x1; y=y1;
}

// преобразование сферических координат в декартовы и обратно.
// u - долгота (рад, 0...2*пи)
// v - широта  (рад, -пи/2...пи/2)
// таблица соответствия:     (u,v)     (x,y,z)
//                           (0,0)     (1,0,0)
//                           (0,пи/2)  (0,1,0)
//                        (пи/2,*)     (0,0,1)

void SferDek ( double u, double v, double* x, double* y, double* z )
{ double sinu=sin(u), cosu=cos(u), sinv=sin(v), cosv=cos(v);
  *x=cosv*cosu;
  *y=cosv*sinu;
  *z=sinv;
}
void DekSfer(double x, double y, double z, double* u, double* v )
{ double r;
  r = sqrt(x*x+y*y);
  *v = arg(r,z);
  if (*v>M_PI_2) *v-=2*M_PI;
  if (r==0) *u=0; else *u=arg(x,y);
}


// преобразование сферич.координат в единичный вектор и обратно
VECT3 SferVect(double u, double v)
{ VECT3 V;
  SferDek(u,v,&V.x,&V.y,&V.z);
  return V;
}
void VectSfer(VECT3 V, double* u, double* v)
{ DekSfer(V.x,V.y,V.z,u,v); }


// Переходы между астрономическими системами координат.
// Процедуры с окончанием 'd' работают в декартовых координатах,
// с окончанием 'v' - с векторами, без окончания - в сферических.
// Параметры процедур:
//   A       - азимут (рад), отсчитывается от юга в сторону запада
//   H       - угол места (рад)
//   alfa    - прямое восхождение (рад)
//   delta   - склонение (рад)
//   t       - часовой угол (рад)
//   x,y,z - декартовы координаты
//   shirota - широта места (рад)
//   mzv     - местное звездное время (рад)

// из горизонтальной в первую экваториальную:

void GorEkv1d (double& x, double &y, double &z, double shirota)
{ Povorot(x,z,M_PI/2-shirota); y=y; }

void GorEkv1v(VECT3& v, double shirota)
{ GorEkv1d(v.x, v.y, v.z, shirota); }

void GorEkv1(double A, double H, double shirota,
             double *t, double *delta)
{ double x, y, z;
  SferDek(A,H,&x,&y,&z); GorEkv1d(x,y,z,shirota);
  DekSfer(x,y,z,t,delta);
}

// из первой экваториальной в горизонтальную:

void Ekv1Gord (double& x, double &y, double &z, double shirota)
{ Povorot(x,z,shirota-M_PI/2); y=y; }

void Ekv1Gorv(VECT3& v, double shirota)
{ Ekv1Gord(v.x, v.y, v.z, shirota); }

void Ekv1Gor(double t, double delta, double shirota,
             double *A, double *H)
{ double x, y, z;
  SferDek(t,delta,&x,&y,&z); Ekv1Gord(x,y,z,shirota);
  DekSfer(x,y,z,A,H);
}

// из первой экваториальной во вторую экваториальную и обратно
// (оба преобразования совпадают):

void Ekv1Ekv2d (double& x, double &y, double &z, double mzv)
{ Povorot(x,y,mzv); y=-y; z=z; }

void Ekv1Ekv2v(VECT3& v, double mzv)
{ Ekv1Ekv2d(v.x, v.y, v.z, mzv); }

void Ekv1Ekv2(double t, double mzv, double *alfa) //для дураков
{ *alfa = mzv-t; }

#define Ekv2Ekv1d Ekv1Ekv2d
#define Ekv2Ekv1v Ekv1Ekv2v
#define Ekv2Ekv1  Ekv1Ekv2

// из горизонтальной во вторую экваториальную:
void GorEkv2d (double& x, double &y, double &z,
               double shirota, double mzv)
{ Povorot(x,z,M_PI/2-shirota); Povorot(x,y,mzv); y=-y; }

void GorEkv2v (VECT3& v, double shirota, double mzv)
{ GorEkv2d (v.x, v.y, v.z, shirota, mzv); }

void GorEkv2(double A, double H, double shirota, double mzv,
             double *alfa, double *delta)
{ double x, y, z;
  SferDek(A,H,&x,&y,&z); GorEkv2d(x,y,z,shirota,mzv);
  DekSfer(x,y,z,alfa,delta);
}

// из второй экваториальной в горизонтальную:

void Ekv2Gord (double& x, double &y, double &z,
               double shirota, double mzv)
{ Povorot(x,y,mzv); y=-y; Povorot(x,z,shirota-M_PI/2); }

void Ekv2Gorv (VECT3& v, double shirota, double mzv)
{ Ekv2Gord (v.x, v.y, v.z, shirota, mzv); }

void Ekv2Gor(double alfa, double delta, double shirota, double mzv,
             double *A, double *H)
{ double x, y, z;
  SferDek(alfa,delta,&x,&y,&z); Ekv2Gord(x,y,z,shirota,mzv);
  DekSfer(x,y,z,A,H);
}

