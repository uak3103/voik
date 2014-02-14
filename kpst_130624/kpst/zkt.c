/*
// ��������� ��������� ����� ��������� ��������
struct CATHDR {
  short type;         // ��������������� ���� ����� - ������ ���� ����� 2
  char  name[8];      // ��� ��������
  short entrysz;
  float halfa, hdelta;// ������ ��������� �� �����, ������ (�)
  short nalfa, ndelta;// ����� ��������� �� �����, ������
  float deltalimit;   // ����������� �� ������ � ����� ���������
  float maglimit;     // ����������� �� �������� ��������
  long numzv;         // ����� ����� �����
  char reserved2[28];
};

// ��������� ��������� ��������:
struct CAT {
  FILE* f;    // ��������� �� �������� ���� ��������
  CATHDR hdr;  // ����� ��������� ����� � ����������� ������
  long* maddr; // ����� ��������� ������� � ����������� ������
  char *selected;
  unsigned nseg;
  long addr;
};

// ������������� ���������� �� ������
struct CATSTAR {
   long N;            // ����� ������ �� ��������
   double alfa,delta; // �����, ������ J2000.0 (���)
   float valfa,vdelta;// ����������� �������� �� �����, ������ (���/���)
   float mag;         // ��.���.
   char sp[2];        // ������.�����
   char flags;        // ��������;
   float BV;          // B-V .
};
*/

// ��������� ������ �� ������
struct CATENTRY {
   long N;          // ����� ������ �� ��������
   long RA,DE;      // �����, ������ (Right Ascension, Declination)
		    // ��� ����� J2000.0 � �������� 0.001"
   short pmRA,pmDE; // ����������� �������� (proper motion) �� �����,
		    // ������ - � �������� 0.001"/���
   short mag;       // �������� ������ (magnitude) - � ��. 0.001 ��.���.
   char sp[2];      // ������.�����
   char flags;      // ��������; ��������� ������������ �������:
		    // ��� 0 = 1 - ������ �� ������������� ������������
		    // ��� �������
   short BV;        // B-V - � ��. 0.001 ��.���.
};

int opencat(CAT& c, char* fname)
{
 if ((c.f=fopen(fname,"rb"))==NULL) return 0;
 if (fread(&c.hdr,1,sizeof(c.hdr),c.f)!=sizeof(c.hdr)) return 0;
 if (c.hdr.type!=2) return 0;
 unsigned sz1=c.hdr.nalfa*c.hdr.ndelta, sz2=(sz1+1)*sizeof(long);
 if ((c.selected=(char*)malloc(sz1))==NULL) return 0;
 if ((c.maddr=(long*)malloc(sz2))==NULL) return 0;
 if (fread(c.maddr,1,sz2,c.f)!=sz2) return 0;
 return 1;
}

void closecat(CAT& c)
{ free(c.maddr); fclose(c.f); c.f=NULL; }

void startreadseg(CAT& c,
		  double alfamin,  double alfamax,
		  double deltamin, double deltamax)
{
  alfamin*=180/M_PI; alfamax*=180/M_PI;
  deltamin*=180/M_PI; deltamax*=180/M_PI;
  double ha=c.hdr.halfa, hd=c.hdr.hdelta;
  int na=c.hdr.nalfa, nd=c.hdr.ndelta;
  int ia1=alfamin/ha, ia2=alfamax/ha;
  int id1=(90-deltamax)/hd, id2=(90-deltamin)/hd;
  if (ia2*ha<alfamax) ia2++;
  if (90-id2*hd>deltamin) id2++;
  int ia,id;
  memset(c.selected,0,nd*na);
  for(id=id1;id<id2;id++) {
    if (alfamin<alfamax)
       for(ia=ia1;ia<ia2;ia++) c.selected[id*na+ia]=1;
    else { for(ia=0;ia<ia2;ia++)  c.selected[id*na+ia]=1;
	   for(ia=ia1;ia<na;ia++) c.selected[id*na+ia]=1; }
  }
  c.nseg=0xFFFFFFFF;
}

void startreadcircle(CAT& c, double alfa, double delta, double radius)
{
  double amin,amax,dmin,dmax;
  dmin=delta-radius; dmax=delta+radius;
  if (dmax>M_PI/2) dmax=M_PI/2;
  if (dmin<-M_PI/2) dmin=-M_PI/2;
  if (dmax>M_PI/2-1e-8||dmin<-(M_PI/2-1e-8)) {
    amin=0; amax=2*M_PI;
  } else {
    double d=asin(sin(radius)/cos(delta));
    amin=alfa-d; if (amin<0) amin+=2*M_PI;
    amax=alfa+d; if (amax>2*M_PI) amax-=2*M_PI;
  }
  startreadseg(c,amin,amax,dmin,dmax);
}

int readentry(CAT& c, CATENTRY& e)
{
  if (c.nseg==0xFFFFFFFF||c.addr>=c.maddr[c.nseg+1]) {
    while(1) {
      if (++c.nseg==c.hdr.nalfa*c.hdr.ndelta) return 0;
      if (c.selected[c.nseg]&&c.maddr[c.nseg]<c.maddr[c.nseg+1]) break;
    }
    c.addr=c.maddr[c.nseg];
    fseek(c.f,c.addr,SEEK_SET);
  }
  if (fread(&e,1,c.hdr.entrysz,c.f)!=c.hdr.entrysz) errorproc(0);
  c.addr+=c.hdr.entrysz;
  return 1;
}

void unpackstar(CATENTRY& e, CATSTAR &s)
{
  double k=M_PI/1000/3600/180;
  s.N=e.N;
  s.alfa =e.RA*k;  s.valfa=e.pmRA*k;
  s.delta=e.DE*k;  s.vdelta=e.pmDE*k;
  s.mag=e.mag/1000.0;
  memcpy(s.sp,e.sp,2);
  s.flags=e.flags;
  s.BV=e.BV/1000.0;
}

int readstar(CAT& c, CATSTAR& s)
{ CATENTRY e;
  if (!readentry(c,e)) return 0;
  unpackstar(e,s);
  return 1;
}

int findstar(CAT& c, long Nkat, CATSTAR &s)
{
  CATENTRY e;
  fseek(c.f,c.maddr[0],SEEK_SET);
  while (1) {
    if (fread(&e,1,c.hdr.entrysz,c.f)!=c.hdr.entrysz) errorproc(0);
    if (e.N==Nkat) { unpackstar(e,s); break; }
  }
  return 1;
}
