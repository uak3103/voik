#include "base.h"

Base::Base()
{
    W_KADR=512;
    H_KADR=512;
    PLOSH=6;
    DISTANT1 = 19;
    DISTANT2 = 4;
    FIND1 = 2;
    correct_by_speed=1;
    MAXNEVYAZ =0.2;
    MAXNEVYAZBIK =0.3; // 1.0;
    DISTPHM=5;
    NEARPHM=5;
    KOLZV_SHPK=15;
    KOLZV_BIK=15;
    MAXKO=10;
    m_kadr=(char*)malloc(W_KADR*H_KADR*sizeof(short)+1024);

    q_ob=0;
    m_swrt=(tObj*)malloc(OBJ_MAX*sizeof(tObj));
    memset(m_swrt,0,OBJ_MAX*sizeof(tObj));
    m_objs=(sObj*)malloc(OBJ_MAX*sizeof(sObj));
    memset(m_objs,0,OBJ_MAX*sizeof(sObj));
    memset(m_objsn,0,OBJ_MAX*OBRAB_MAX*sizeof(tObj));
    obrab=0;
    u_id=0;

    time_t rawtime;
    time(&rawtime);
    tm *ltm = localtime(&rawtime);
    int mmon=1 + ltm->tm_mon,dday=ltm->tm_mday;
//    sprintf(nftr,"dump/tr_%02d%02d.txt",mmon,dday);
    sprintf(nftr,"/tmp/tr_%02d%02d.txt",mmon,dday);
    ftr=fopen(nftr,"w");
}

Base::~Base()
{
    free(m_kadr);
    if (ftr!=NULL)
        fclose(ftr);
}

double getmks(void)
{
 struct timeval tv;
  gettimeofday(&tv,0);
  return tv.tv_sec+tv.tv_usec*1.0e-6;
}

/***********************************************************************/
void Base::trace(const char *fmt,...)
{ char str[256];

  va_list arg;
  va_start(arg,fmt);
  vsprintf(str,fmt,arg);
  va_end(arg);

  fputs(str,ftr);
}

int Base::NumKadrs()
{
    int fff;
    char sFile[50];
    int n11;
    int n;

    for (n=0;;n++)
    {
        n11=sprintf(sFile,"%s%d%s","/tmp/ForApoi",n,".dat");
        fff=open(sFile,O_RDONLY);
        if (fff<0)
        {
            break;
        }
        close(fff);
    }
    return n;
}

bool Base::ReadKadr(int n)
{
    int fff;
    char sFile[50];
    int n11;
    char *m_Temp;
    int VideoOffset;
    m_Temp=(char*)malloc(1024);
    n11=sprintf(sFile,"%s%d%s","/tmp/ForApoi",n,".dat");
    fff=open(sFile,O_RDONLY);
    if (fff<0)
    {
        trace("ERROR %s\n",sFile);
        return false;
    }
    read(fff,m_Temp,1024);
    tex_inf=(TEX_INFO*)(m_Temp+16);
    trace("kadr%6d file:%s\n",tex_inf->nCadrNum,sFile);
    if (tex_inf->wSizeX!=W_KADR || tex_inf->wSizeY!=H_KADR)
    {
      W_KADR=tex_inf->wSizeX;
      H_KADR=tex_inf->wSizeY;
      VideoOffset=tex_inf->nVideoOffset;
      m_kadr=(char*)realloc(m_kadr,W_KADR*H_KADR*sizeof(short)+VideoOffset);
    }
    memcpy(m_kadr,m_Temp,1024);
    tex_inf=(TEX_INFO*)(m_kadr+16);
    VideoOffset=tex_inf->nVideoOffset;
    read(fff,m_kadr+1024,W_KADR*H_KADR*sizeof(short)+VideoOffset-1024);
    close(fff);
    delete [] m_Temp;
    return true;
}

void Base::sort_swrt(int num,tObj *obj)
{
    int _X,_Y;
    int i,nnn;

    memset(s_kol,0,sizeof(s_kol));
    SETKA_X=W_KADR/16;
    ///    ctk->SETKA_Y=ctk->H_KADR/16;
    SETKA_Y=H_KADR/8;

    for (i=0;i<q_sw;i++)
    {
        _X=int((obj[i].X-tex_inf->wOffsetX)/SETKA_X);
        _Y=int((obj[i].Y-tex_inf->wOffsetY)/SETKA_Y);
        if ((_X<0) || (_X>15)) continue;
        ///        if ((_Y<0) || (_Y>15)) continue;
        ///        nnn=_Y*16+_X;
        if ((_Y<0) || (_Y>7)) continue;
        nnn=_Y*16+_X;               //8???

        s_ndx[nnn][s_kol[nnn]]=i;
        if (s_kol[nnn]<256) s_kol[nnn]++;
    }
}

double Base::fun(int num,int obj,sObj *mas)
{
    double _VX,_VY,_T,_vv, P;
    int kolzv;
    float vx=OBRAB.VXzv;
    float vy=OBRAB.VYzv;
    int t_resh=OBRAB.Nkadr/2.+1.51;  // +0.51
    if(tex_inf->btCamType==0) // BIK
        kolzv=KOLZV_BIK;
    else kolzv=KOLZV_SHPK;
    _T =mas[obj].N;
    _VX=mas[obj].VX;
    _VY=mas[obj].VY;
    _vv=(t_resh*t_resh)/(_T*_T);
    if(q_ob>kolzv) //po srednei skorosti
    {
        vx= VX_aver;
        vy= VY_aver;
    }

        P=((_VX-vx)*(_VX-vx)+(_VY-vy)*(_VY-vy))/(_vv*_vv)/10;  //po apriornoi/srednei skorosti

    return P;
}
/////////////////////////////////////////////////////////////////////////////
int Base::is_ko(int num,int obj,sObj *mas)
{
    double P,dx,dy,d,D=0;
    int cnt=0;
    int t_resh=OBRAB.Nkadr/2.+1.51;  // +0.51
    int res=1;
    P=fun(num,obj,mas);
    if (P<1) res=0;
    if (mas[obj].N<t_resh) res=0;
    if(res>0)
    {
        trace("ctk%d for ID=%d:\n",num+1,m_objs[obj].ID);
        for(int k=0;k<OBRAB.Nkadr;k++)
        {
            if(m_objs[obj].T0<=m_tob[k] && m_objsn[m_objs[obj].ID][k].S>=PLOSH)
            {
                cnt++;
                dx=fabsf((float)(m_objs[obj].DX+(float)(m_objs[obj].VX)*(m_tob[k]-m_objs[obj].T0))-
                         (float)m_objsn[m_objs[obj].ID][k].X);
                dy=fabsf((float)(m_objs[obj].DY+(float)(m_objs[obj].VY)*(m_tob[k]-m_objs[obj].T0))-
                         (float)m_objsn[m_objs[obj].ID][k].Y);
                d=sqrt(dx*dx+dy*dy);
                D+=d;
                trace("prg%d: _d=%8.3f\n",k,d);
            }
        }
        if (cnt>0)
        {
            trace("ctk%d AD=%8.3f\n",num+1,D/cnt);
            float dist=0;
            if (tex_inf->btCamType==1 || tex_inf->btCamType==3) //SHPK or BPK
            {
                float add=0;
                if(sqrt((m_objs[obj].VX*m_objs[obj].VX)+(m_objs[obj].VY*m_objs[obj].VY))>20)
                {
                if(m_objs[obj].S>40) add=max(add,m_objs[obj].S/100);
                if(m_objs[obj].V>200000) add=max(add,m_objs[obj].V/1000000);
                if(m_objs[obj].N==OBRAB.Nkadr)           //было =
                  add=min(add,0.6);
                else
                  add=min(add,0.4);
                 }
                   dist=MAXNEVYAZ*m_objs[obj].N/OBRAB.Nkadr+add;
                  if (D/cnt>dist) res=0;
            }
            else
            {
                  dist=MAXNEVYAZBIK*m_objs[obj].N/OBRAB.Nkadr;
                 if (D/cnt>dist) res=0;
            }
        }
        else res=0;
    }
    mas[obj].KO=res;
    return mas[obj].KO;
}
/////////////////////////////////////////////////////////////////////////////
int Base::is_zv(int num,int obj,sObj *mas)
{
    double P,dx,dy,d,D=0;
    int cnt=0;
    int t_resh=OBRAB.Nkadr/2.+0.51;
    int res=1;
    P=fun(num,obj,mas);
    if (P>=1) res=0;
    if (mas[obj].N<t_resh) res=0;
    if(res>0)
    {
        for(int k=0;k<OBRAB.Nkadr;k++)
        {
            if(m_objs[obj].T0<=m_tob[k] && m_objsn[m_objs[obj].ID][k].S>=PLOSH)
            {
                cnt++;
                dx=fabsf((float)(m_objs[obj].DX+(float)(m_objs[obj].VX)*(m_tob[k]-m_objs[obj].T0))-
                         (float)m_objsn[m_objs[obj].ID][k].X);
                dy=fabsf((float)(m_objs[obj].DY+(float)(m_objs[obj].VY)*(m_tob[k]-m_objs[obj].T0))-
                         (float)m_objsn[m_objs[obj].ID][k].Y);
                d=sqrt(dx*dx+dy*dy);
                D+=d;
            }
        }
        if (cnt>0)
        {
            if (tex_inf->btCamType==1 || tex_inf->btCamType==3) //SHPK or BPK
            {
                if (D/cnt>MAXNEVYAZ) res=0;
            }
            else if (D/cnt>MAXNEVYAZBIK) res=0;
        }
        else res=0;
    }
    return res;
}

void Base::fill_conv(int obj)
{
    CONVOLUTION conv;
    CONVOLUTION *cnv=&conv;
    if (obj<=0) return;
    char *m_dubl;
    //int nobj=obj;
    int si=lst.size();
    if (si>0) si=1;             //1 обьект
    int r,VideoOffset;
    r=(256+(obj+si)*sizeof(CONVOLUTION))/1024+1;
    VideoOffset=1024*r;                 //VideoOffset
    TEX_INFO tex1;
    TEX_INFO *tex=&tex1;
    nObj *nob;

    m_dubl=(char*)malloc(VideoOffset);
    memcpy(tex,tex_inf,sizeof(TEX_INFO));
    tex->nVideoOffset=VideoOffset;

    nob=(nObj*)malloc((obj+si)*sizeof(nObj));

    for (int i=0;i<obj;i++)
    {
        //memset(cnv,0,sizeof(CONVOLUTION));
        cnv->nID='CONV';
        cnv->wNum=i+1;
        cnv->nS=(int)m_objs[i].S;
        cnv->nV=(int)m_objs[i].V;
        cnv->nX=(int)m_objs[i].X;
        cnv->nY=(int)m_objs[i].Y;
        cnv->wReserv=(int)m_objs[i].KO;
        memcpy(m_dubl+256+(cnv->wNum-1)*sizeof(CONVOLUTION),cnv,sizeof(CONVOLUTION));

        nob[i].X =m_objs[i].X ;
        nob[i].Y =m_objs[i].Y ;
        nob[i].DX=m_objs[i].VX;
        nob[i].DY=m_objs[i].VY;
        nob[i].S =m_objs[i].S ;
        nob[i].V =m_objs[i].V ;
        nob[i].T =m_objs[i].T ;
        nob[i].KO=m_objs[i].KO;
    }
    //теперь список обьектов VOIK
    tex->wConvolutionsCount=(obj+si);
    memcpy(m_dubl,m_kadr,16);
    memcpy(m_dubl+16,tex,sizeof(TEX_INFO));
    //просмотр
    if (si>0)
    {
        list <nObj>::iterator  Iter  = lst.begin();
        int k=0;
        float S1=0,V1=0,X1=0,Y1=0,X0=-1,Y0=-1,XK,YK;
        double T0,TK;
        for(; Iter != lst.end(); ++Iter)
        {
          nObj t=*Iter;
          S1+=t.S;
          V1+=t.V;
          X1+=t.X;
          Y1+=t.Y;
          if (X0<0)
          {
              X0=t.X;
              Y0=t.Y;
              T0=t.T;
          }
          XK=t.X;YK=t.Y;TK=t.T;
          k++;
        }
        //средние только 1 объект
        int i=obj+1;
        cnv->nID='VOIK';
        cnv->wNum=i;
        cnv->nS=S1/k;
        cnv->nV=V1/k;
        cnv->nX=X1/k;
        cnv->nY=Y1/k;
        cnv->wReserv=1;
        memcpy(m_dubl+256+(cnv->wNum-1)*sizeof(CONVOLUTION),cnv,sizeof(CONVOLUTION));
        i++;

        nob[obj].X =X1/k;
        nob[obj].Y =Y1/k;
        nob[obj].S =S1/k;
        nob[obj].V =V1/k;
        nob[obj].T =m_objs[obj-1].T;
        nob[obj].KO=1;
        if ((OBRAB.Nkadr==1) || (TK<=T0))
        {
            nob[obj].DX=0;
            nob[obj].DY=0;
        }
        else
        {
            nob[obj].DX=(XK-X0)/(TK-T0);
            nob[obj].DY=(YK-Y0)/(TK-T0);
        }
    }

    FILE *fp;

    if ((fp = fopen("/tmp/testVOIK.dat", "wb"))==NULL) {
      printf("He удается открыть файл tesVOIK.dat\n");
      return;
    }
    fwrite(m_dubl, sizeof(char),VideoOffset , fp);
    fwrite(m_kadr+tex_inf->nVideoOffset,sizeof(short),W_KADR*H_KADR,fp);

    fclose(fp);
    //obj
    if ((fp = fopen("/tmp/testVOIK.obj", "wb"))==NULL) {
      printf("He удается открыть файл tesVOIK.obj\n");
      return;
    }
    fwrite(nob, sizeof(nObj),(obj+si), fp);
    fclose(fp);
r=sizeof(nObj);
    free(m_dubl);
    free(nob);
}

void Base::send_objs(int fff,int num)
{
    int cnt_KO=0,cnt_ZV=0,nnn=0;
    int res;
    nObj *nob; int obj;

    if(1)
    {
        for (obj=0;obj<q_ob;obj++)
        {
            if (is_ko(num,obj,m_objs))
            {
                m_objs[obj].KO= 1;
                cnt_KO++;
                trace(" ctk%d KO (%d): X=%8.3f; Y=%8.3f; VX=%8.3f; VY=%8.3f; S=%8.3f; V=%12.3f; N=%d\n",
                      num+1,m_objs[obj].ID,m_objs[obj].X,m_objs[obj].Y,m_objs[obj].VX,m_objs[obj].VY,m_objs[obj].S,m_objs[obj].V,m_objs[obj].N);
              }
            else if (is_zv(num,obj,m_objs))
            {
                m_objs[obj].KO=-1;
                cnt_ZV++;
                trace(" ctk%d ZV(%3d): X=%8.3f; Y=%8.3f; VX=%8.3f; VY=%8.3f; S=%8.3f; V=%12.3f; N=%d\n",
                      num+1,m_objs[obj].ID,m_objs[obj].X,m_objs[obj].Y,m_objs[obj].VX,m_objs[obj].VY,m_objs[obj].S,m_objs[obj].V,m_objs[obj].N);
            }
            else
            { trace(" ctk%d NNN(%4d): X=%8.3f; Y=%8.3f; VX=%8.3f; VY=%8.3f; S=%8.3f; V=%12.3f; N=%d\n",
                    num+1,m_objs[obj].ID,m_objs[obj].X,m_objs[obj].Y,m_objs[obj].VX,m_objs[obj].VY,m_objs[obj].S,m_objs[obj].V,m_objs[obj].N);
                continue;
            }
            memcpy(&m_objs[nnn],&m_objs[obj],sizeof(sObj));
            nnn++;
        }
        q_ob=nnn;

        if(cnt_KO>0)
        {
         for (obj=0;obj<q_ob;obj++)
          {
            if(m_objs[obj].KO == 1)
             {
               if(cnt_KO<MAXKO)
                {
                   trace(" ctk%d KO (%d): X=%8.3f; Y=%8.3f; VX=%8.3f; VY=%8.3f; S=%8.3f; V=%12.3f; N=%d\n",
                         num+1,m_objs[obj].ID,m_objs[obj].X,m_objs[obj].Y,m_objs[obj].VX,m_objs[obj].VY,m_objs[obj].S,m_objs[obj].V,m_objs[obj].N);
                   //fill_convolution(num,obj);
                 }
               else m_objs[obj].KO = 0;
             }
           }
        }
    }
    else if(q_ob==1)
        cnt_KO=1;

    fill_conv(q_ob);
/*
    *(int*)msg.mark='APOI';
    *(int*)msg.type='CONV';
    msg.nctk=num+1;
    msg.size=ctk->q_ob*sizeof(nObj);
    pthread_mutex_lock(&send_mutex);
    res=send(fff,&msg,sizeof(APOI_MSG),0);
    nob=(nObj*)malloc(msg.size);
    for (obj=0;obj<ctk->q_ob;obj++)
    {
        nob[obj].X =ctk->m_objs[obj].X ;
        nob[obj].Y =ctk->m_objs[obj].Y ;
        nob[obj].DX=ctk->m_objs[obj].VX;
        nob[obj].DY=ctk->m_objs[obj].VY;
        nob[obj].S =ctk->m_objs[obj].S ;
        nob[obj].V =ctk->m_objs[obj].V ;
        nob[obj].T =ctk->m_objs[obj].T ;
        nob[obj].KO=ctk->m_objs[obj].KO;
    }
*/
        trace(" ctk%d KO=%d ZV=%d\n",num+1,cnt_KO,cnt_ZV);
//        free(nob);
}

int Base::find_match2(int num,double _x,double _y,double _v,tObj *obj,double maxdist,double *dd)
{
    double _dX,_dY,_dV,dist,dist_m;
    int _X,_Y,_ox,_oy,res=-1;
    int i,j,nnn[4],idx;

    dist_m=W_KADR*H_KADR*sizeof(short);
    _ox=tex_inf->wOffsetX;
    _oy=tex_inf->wOffsetY;
    if ((_x<_ox) || (_x>_ox+W_KADR)) return -1;
    if ((_y<_oy) || (_y>_oy+H_KADR)) return -1;
    if(OBRAB.VXob<-10)
        _X=(int)((_x-_ox)/SETKA_X);
    else
        _X=(int)((_x-_ox)/SETKA_X+0.5);
    if(OBRAB.VYob<-10)
        _Y=(int)((_y-_oy)/SETKA_Y);
    else
        _Y=(int)((_y-_oy)/SETKA_Y+0.5);
    ///    nnn[0]=((_X<16) && (_Y<16))?(_Y  )*16+(_X  ):-1;
    ///    nnn[1]=((_X>=1) && (_Y<16))?(_Y  )*16+(_X-1):-1;
    ///    nnn[2]=((_X<16) && (_Y>=1))?(_Y-1)*16+(_X  ):-1;
    ///    nnn[3]=((_X>=1) && (_Y>=1))?(_Y-1)*16+(_X-1):-1;
    nnn[0]=((_X<16) && (_Y<8))?(_Y  )*16+(_X  ):-1;          //8
    nnn[1]=((_X>=1) && (_Y<8))?(_Y  )*16+(_X-1):-1;          //8
    nnn[2]=((_X<16) && (_Y>=1))?(_Y-1)*16+(_X  ):-1;         //8
    nnn[3]=((_X>=1) && (_Y>=1))?(_Y-1)*16+(_X-1):-1;         //8
    for (j=0;j<4;j++)
    {
        if (nnn[j]<0) continue;
        for (i=0;i<s_kol[nnn[j]];i++)
        {
            idx=s_ndx[nnn[j]][i];
            if ((idx<0) || (idx>=OBJ_MAX)) continue;
            if (obj[idx].V<0) continue;
            _dX=(_x-obj[idx].X);
            _dY=(_y-obj[idx].Y);

            if(maxdist>100)
                _dV=(sqrt(_v)-sqrt(obj[idx].V))/10;
            else
                _dV=0;
            dist=_dX*_dX+_dY*_dY+fabs(_dV);
            if (maxdist)
            {
                if (dist>maxdist) continue;

            }
            if (dist<dist_m)
            {
                res=idx;
                dist_m=dist;
            }
        }
    }
    *dd=dist_m;
    return res;
}

int Base::process_swrt2(int num)
{
    int res,res0,obj,nnn;
    double _fX,_dX;
    double _fY,_dY;
    double _fV,_fS;
    int DIST=DISTANT1;
    int nizm=obrab;

    if (tex_inf->btCamType==1 || tex_inf->btCamType==3) //SHPK or BPK
    {
        correct_by_speed=1;
        if(ind_bigobj>0)
            pre_obrab(num);
    }
    else {
        correct_by_speed=0;
    }
    sort_swrt(num,m_swrt);
    if (nizm==1)
    {
        if ((m_tob[1]-m_tob[0])>1)
        {
            DIST=(int)DIST*(m_tob[1]-m_tob[0]);
            trace("DIST=%d, t1=%8.3f, t2=%8.3f\n",DIST,m_tob[1],m_tob[0]);
        }
        if((OBRAB.VXob*OBRAB.VXob+OBRAB.VYob*OBRAB.VYob)*(m_tob[1]-m_tob[0])>(DIST*DIST))
            DIST=(int)sqrt((OBRAB.VXob*OBRAB.VXob+OBRAB.VYob*OBRAB.VYob)*(m_tob[1]-m_tob[0]))+2;
        trace("ctk%d DIST=%d, binX=%d\n",num+1,DIST,tex_inf->btBinX);
    }
    for (obj=0;obj<q_ob;obj++)
    {

        if (nizm==1 && (tex_inf->btCamType==1 || tex_inf->btCamType==3)) // uchet apriornoi skorosti zvezd
        {
            _fX=m_objs[obj].X+OBRAB.VXzv*(m_tob[1]-m_tob[0]);
            _fY=m_objs[obj].Y+OBRAB.VYzv*(m_tob[1]-m_tob[0]);
            //            }
            _fV=m_objs[obj].V;
            _fS=m_objs[obj].S;
        }
        else
        {
            _fX=m_objs[obj].X;
            _fY=m_objs[obj].Y;
            _fV=m_objs[obj].V;
            _fS=m_objs[obj].S;
        }
        double dd0=0,dd=-1.0;
        if (nizm<FIND1)
        {
            res=find_match2(num,_fX,_fY,_fV,m_swrt,DIST*DIST,&dd);
            if(((OBRAB.VXob*OBRAB.VXob+OBRAB.VYob*OBRAB.VYob) > 400)  && (_fS>20))
            {
                _fX=m_objs[obj].X+OBRAB.VXob*(m_tob[1]-m_tob[0]);
                _fY=m_objs[obj].Y+OBRAB.VYob*(m_tob[1]-m_tob[0]);
                res0=find_match2(num,_fX,_fY,_fV,m_swrt,DIST*DIST,&dd0);
                if(dd>dd0) res=res0;
            }

        }
        else
            res=find_match2(num,_fX,_fY,_fV,m_swrt,DISTANT2*DISTANT2,&dd);

        if (res>=0)
        { // EQUAL
            // save
            int ind=m_objs[obj].ID;
            m_objsn[ind][nizm] =m_swrt[res];
            m_objs[obj].N++;
            if(m_swrt[res].S>60)
                trace("ID=%4d X=%6.1f Y=%6.1f S=%5d V=%8d XX=%10.1f XY=%10.1f YY=%10.1f\n",
                      ind,m_swrt[res].X,m_swrt[res].Y,
                      m_swrt[res].S,m_swrt[res].V,m_swrt[res].XX,
                      m_swrt[res].XY,m_swrt[res].YY);

            trace("ID=%d X=%8.3f Y=%8.3f S=%8d V=%8d\n",ind,m_swrt[res].X,m_swrt[res].Y,m_swrt[res].S,m_swrt[res].V);

            _dX=m_swrt[res].X-_fX;
            _dY=m_swrt[res].Y-_fY;
            if (m_swrt[res].V>200000)
                trace("ctk%d ID=%d: _X=%8.3f _Y=%8.3f _S=%8d _V=%8d \n",
                      num+1,ind,m_swrt[res].X,m_swrt[res].Y,m_swrt[res].S,m_swrt[res].V);

            trace("ID=%d: _dX=%8.3f _dY=%8.3f _dS=%8.3f _dV=%8.3f \n",
                  ind,fabsf(_dX),fabsf(_dY),fabsf(_fS-m_swrt[res].S),fabsf(_fV-m_swrt[res].V));

            m_swrt[res].V=-1; // dont duplicate

            double _S=0,_V=0,_VX=0,_VY=0;
            double _sumT=0,_sumTqrt=0;
            double _sumx=-1.0,_sumy=-1.0;
            double _T0=0,_Z=0,_X0,_Y0;

            for (int i=0;i<=nizm;i++)
            {
                if (m_objsn[ind][i].S<PLOSH) continue;
                _S +=(float)m_objsn[ind][i].S;
                _V +=(float)m_objsn[ind][i].V;
                if(_sumx<0)
                { _sumx=m_objsn[ind][i].X;
                    _sumy=m_objsn[ind][i].Y;
                    _T0=m_tob[i];
                }
                else
                {
                    _sumT +=(m_tob[i]-_T0);
                    _sumTqrt  +=(float)((m_tob[i]-_T0)*(m_tob[i]-_T0));
                    _VX +=(float)m_objsn[ind][i].X*(m_tob[i]-_T0);
                    _VY +=(float)m_objsn[ind][i].Y*(m_tob[i]-_T0);
                    _sumx +=m_objsn[ind][i].X;
                    _sumy +=m_objsn[ind][i].Y;
                }
            }

            m_objs[obj].V =(float)(_V/m_objs[obj].N);
            m_objs[obj].S =(float)(_S/m_objs[obj].N);
            m_objs[obj].SKO=(float)(_dX*_dX+_dY*_dY);
            _Z=m_objs[obj].N*_sumTqrt-_sumT*_sumT;
            if (_Z != 0)
            {
                m_objs[obj].VX=(float)((m_objs[obj].N*_VX - _sumx*_sumT)/_Z);
                m_objs[obj].VY=(float)((m_objs[obj].N*_VY - _sumy*_sumT)/_Z);
                _X0=(_sumx*_sumTqrt-_VX*_sumT)/_Z;
                _Y0=(_sumy*_sumTqrt-_VY*_sumT)/_Z;
                m_objs[obj].DX =(float)_X0;
                m_objs[obj].DY =(float)_Y0;
            }
            else
            { trace("ctk%d Z=0,nizm=%d;uid=%d\n",num+1,nizm,obj);
                for (int i=0;i<=nizm;i++)
                {
                    trace("ctk%d i=%d, t=%8.3f,T0=%8.3f \n",num+1,i,m_tob[i],_T0);
                }
                continue;}
        } //end res>0

        //:::::: prognoz
        double _T0=m_objs[obj].T;  // first time for object
        if (nizm != (OBRAB.Nkadr-1))
        {
            m_objs[obj].X =(float)(m_objs[obj].DX+(float)(m_objs[obj].VX)*(m_tob[nizm]-_T0+m_tob[1]-m_tob[0]));
            m_objs[obj].Y =(float)(m_objs[obj].DY+(float)(m_objs[obj].VY)*(m_tob[nizm]-_T0+m_tob[1]-m_tob[0]));
        }
        else
        {

            m_objs[obj].T=(double)((m_tob[nizm]+_T0)/2.0); // aver time
            m_objs[obj].X =(float)(m_objs[obj].DX+(float)(m_objs[obj].VX)*(m_objs[obj].T-_T0));
            m_objs[obj].Y =(float)(m_objs[obj].DY+(float)(m_objs[obj].VY)*(m_objs[obj].T-_T0));
        }
        trace("izm=%d[%4d] X=%8.3f, Y=%8.3f, VX=%8.3f, VY=%8.3f, S=%8.3f, V=%8.3f, N=%2d, ID=%3d\n",
              nizm,obj,m_objs[obj].X,m_objs[obj].Y,
              m_objs[obj].VX,m_objs[obj].VY,
              m_objs[obj].S,m_objs[obj].V,
              m_objs[obj].N,m_objs[obj].ID);
    } // end for(obj<q_ob)
    nnn=q_ob;
    sObj *po;
    int del_v=0, del_s=0;
    for (res=0;res<q_sw;res++)
    { // INSERT
        if (m_swrt[res].V<0){del_v++; continue;}
        if (m_swrt[res].S<PLOSH) {del_s++;continue;}
        po=&m_objs[nnn];
        m_objsn[u_id][nizm]=m_swrt[res];
        po->X =m_swrt[res].X;
        po->Y =m_swrt[res].Y;
        po->S =m_swrt[res].S;
        po->V =m_swrt[res].V;
        po->VX=po->VY=0;
        po->DX=m_swrt[res].X;
        po->DY=m_swrt[res].Y;
        po->T=po->T0 =m_tob[nizm];
        po->N =1;
        po->KO=0;
        po->ID=u_id;
        if(m_swrt[res].S>60)
            trace("ID=%4d X=%6.1f Y=%6.1f S=%5d V=%8d XX=%10.1f XY=%10.1f YY=%10.1f\n",
                  u_id,m_swrt[res].X,m_swrt[res].Y,
                  m_swrt[res].S,m_swrt[res].V,m_swrt[res].XX,
                  m_swrt[res].XY,m_swrt[res].YY);
        nnn++;
        if (nizm==0 && m_swrt[res].V>200000)
            trace("ctk%d nizm=%d ID=%d: _X=%8.3f _Y=%8.3f _S=%8d _V=%8d \n",
                  num+1,nizm+1,u_id,m_swrt[res].X,m_swrt[res].Y,m_swrt[res].S,m_swrt[res].V);
        u_id++;


        if (nnn>=OBJ_MAX) break;
    }
    if (nizm==(OBRAB.Nkadr-1)) // all done, delete noise
    {
        //int end_obrab(int num,int n);
        nnn=end_obrab(num,nnn);
    }
    return nnn;
}

void Base::init_lines(int num)
{
  TmpType  T;

  T.Xleft = T.Xright =10000;
  T.C = T.A =0;
  T.SA = T.SxA = T.SyA =0;
  T.SxxA = T.SxyA = T.SyyA =0;

  for (int i=0;i<Q_TMP;i++)
  {
     Tmp1[i] = T;
     Tmp2[i] = T;
   }
}

int Base::process_lines(int num
                  ,int X0 ,int DX
                  ,int Y0 ,int DY
                  ,u_short *inp
                  ,int por[4]
                  ,int aver[4]
                  ,tObj *out)
{
    register int  AAA,AAAn,count_out=0; //count=0,
    register TmpType *TmpIn, *TmpOut;
    register u_short *p, *stop, *line;
    TmpType  T;

    float _X,_Y,_S;
    int    X, Y;
    int Xrightup;
    int end_of_line;
    int qwarta,dqw,cqw;
    int halfW=W_KADR/2,halfH=H_KADR/2;

    trace("lines:kadr - %d inf - %d \n",tex_inf->nCadrNum,tex_inf->btCamType);

    init_lines(num);
    Y=Y0; inp+=W_KADR*Y;
    if (inp)
    {
        while (DY)
        {
            if (Y&1) { TmpIn=Tmp1; TmpOut=Tmp2; }
            else     { TmpIn=Tmp2; TmpOut=Tmp1; }

            line=inp;
            inp+=W_KADR;
            p = line+X0;

            Xrightup=-1;
            end_of_line=0;
            stop = line+X0+DX;

            if(Y<halfH)
                dqw=0;
            else dqw=2;

            for (;;)
            {
                for (X=0;X<DX;X++)
                {
                    if (tex_inf->btCamType!=1 && tex_inf->btCamType!=3) // BIK or LIK
                        qwarta=0;
                    else
                    {
                        if ((p-line)<halfW)
                            qwarta=dqw;
                        else
                            qwarta=dqw+1;
                    }
                    if (tex_inf->btCamType==0)
                    {
                        if((((*p)>>2)&0x3FFF)>por[qwarta]) break;
                    }
                    else if (*p>por[qwarta]) break;

                    p++;

                    if (p>=stop)
                    {
                        end_of_line = 1;
                        T.Xleft = T.Xright = 9999;
                        goto label;
                    }
                }

                X = (p-line);
                T.Xleft = X;
                T.C   =0;
                T.A   =0;
                T.SA  =0;
                T.SxA =0;
                T.SyA =0;
                T.SxxA=0;
                T.SxyA=0;
                T.SyyA=0;

                if (tex_inf->btCamType==0)
                {
                    while ( (AAA=((*p)>>2)&0x3FFF)>por[qwarta])
                    {
                        AAAn = AAA-aver[qwarta];
                        T.C++;
                        T.SA  += AAAn;
                        T.SxA += AAAn*X;
                        T.SyA += AAAn*Y;
                        T.SxxA+= AAAn*X*X;
                        T.SxyA+= AAAn*X*Y;
                        T.SyyA+= AAAn*Y*Y;
                        X++;

                        if (++p==stop) { end_of_line=1; break; }
                    }
                }
                else
                {
                    if ((p-line)<halfW)
                        cqw=1;
                    else cqw=0;

                    while ( (AAA=*p)>por[qwarta])
                    {
                        AAAn = AAA-aver[qwarta];
                        T.C++;
                        T.SA  += AAAn;
                        T.SxA += AAAn*X;
                        T.SyA += AAAn*Y;
                        T.SxxA+= AAAn*X*X;
                        T.SxyA+= AAAn*X*Y;
                        T.SyyA+= AAAn*Y*Y;
                        X++;
                        if (tex_inf->btCamType==1 || tex_inf->btCamType==3) // BPK or SHPK
                        {
                            if(cqw==1 && X>=halfW)
                            { cqw=0; qwarta++;}
                        }

                        if (++p==stop) { end_of_line=1; break; }
                    }
                }
                T.Xright = X-1;
                T.A=T.SA/T.C;

                if (T.Xleft <= Xrightup)
                {
                    TmpOut--;
                     T.C    +=TmpOut->C;
                    T.SA   +=TmpOut->SA;
                    T.SxA  +=TmpOut->SxA;
                    T.SyA  +=TmpOut->SyA;
                    T.SxxA +=TmpOut->SxxA;
                    T.SxyA +=TmpOut->SxyA;
                    T.SyyA +=TmpOut->SyyA;
                    T.Xleft =TmpOut->Xleft;
                }
label:
                while (TmpIn->Xleft <= T.Xright)
                {
                    if (TmpIn->Xright < T.Xleft)
                    {
                        if (count_out+1>=OBJ_MAX) break;
                        if (out!=NULL && TmpIn->C>=PLOSH)
                        {
                            out->S=TmpIn->C;
                            out->V=TmpIn->SA;
                            _S=TmpIn->SA;
                            _X=TmpIn->SxA/_S;
                            _Y=TmpIn->SyA/_S;
                            out->X =(float)_X;
                            out->Y =(float)_Y;
                            out->XX=(float)(TmpIn->SxxA-_X*_X*TmpIn->SA)/_S;
                            out->XY=(float)(TmpIn->SxyA-_X*_Y*TmpIn->SA)/_S;
                            out->YY=(float)(TmpIn->SyyA-_Y*_Y*TmpIn->SA)/_S;
                            out++;
                            count_out++;
                        }
                        //count++;
                    }
                    else
                    {
                        T.C   += TmpIn->C   ;
                        T.SA  += TmpIn->SA  ;
                        T.SxA += TmpIn->SxA ;
                        T.SyA += TmpIn->SyA ;
                        T.SxxA+= TmpIn->SxxA;
                        T.SxyA+= TmpIn->SxyA;
                        T.SyyA+= TmpIn->SyyA;
                        Xrightup= TmpIn->Xright;
                    }
                    TmpIn++;
                }
                if (T.Xleft==9999) break;
                if (count_out+1<OBJ_MAX)
                {
                    memcpy(TmpOut, &T, sizeof(T) );
                    TmpOut++;
                }
                if (end_of_line) break;
            }
            TmpOut->Xleft = 10000;
            DY--;
            Y++;
        }
        // save all with down board
        if (Y&1) TmpIn=Tmp1;
        else     TmpIn=Tmp2;

        while (TmpIn->Xleft < 9999)
        {
            if (count_out+1>=OBJ_MAX) break;
            if (out!=NULL && TmpIn->C>=PLOSH)
            {
                out->S=TmpIn->C;
                out->V=TmpIn->SA;
                _S=TmpIn->SA;
                _X=TmpIn->SxA/_S;
                _Y=TmpIn->SyA/_S;
                out->X =(float)_X;
                out->Y =(float)_Y;
                out->XX=(float)(TmpIn->SxxA-_X*_X*TmpIn->SA)/_S;
                out->XY=(float)(TmpIn->SxyA-_X*_Y*TmpIn->SA)/_S;
                out->YY=(float)(TmpIn->SyyA-_Y*_Y*TmpIn->SA)/_S;
                out++;
                count_out++;
            }
            TmpIn++;
        }
    }
    return count_out;
}

int Base::process_fon (int num,int X0, int DX,int Y0, int DY,u_short *inp,int &aver)
{
    register int i,n,/*_Q,*/_CC;
    register u_short *p,*line,*kadr;
    static   int gist[0x10000];
    double   _SKO=0,_MO=0;
    double   _min,_max=0;
    int mx=0,my=0,dcc=0;
    u_short _Q;

    _CC=DX*DY;
    memset(gist,0,sizeof(gist));
    inp+=W_KADR*Y0;
    kadr=inp;

    {
        while (DY)
        {
            line=inp;
            inp+=W_KADR;
            p = line+X0;
            for (i=0;i<DX;i++)
            {
                _Q=*p++;
                if (tex_inf->btCamType==0) // BIK
                    _Q=(_Q>>2)&0x3FFF;
                if(_Q>_max) {_max=_Q;mx=X0+i;my=Y0;}
                if (_Q>=50000)
                {
                    if (ind_bigobj==0)
                    {
                        obrab_bigobj(num,X0+i,Y0);
                        ind_bigobj++;
                    }
                    else
                    {
                        if(ind_bigobj==128)
                            break;
                        else
                            for(int ib=0;ib<=ind_bigobj;ib++)
                            {
                                if(m_bigobj[ib].Xmin<=X0+i && m_bigobj[ib].Xmax>=X0+i &&
                                        m_bigobj[ib].Ymin<=Y0 && m_bigobj[ib].Ymax>=Y0)
                                    break;
                                if(ib==ind_bigobj)
                                {
                                    obrab_bigobj(num,X0+i,Y0);
                                    ind_bigobj++;
                                    break;
                                }
                            }
                    }
                    dcc++;
                }
                gist[_Q]++;
            }
/*
            if(X0!=0 && Y0!=0)
            {
                char buf[255];
                p = line+X0;
                sprintf(buf,"%d  %d   %d   %d   %d   %d   %d   %d   %d   %d   %d   %d   %d   %d   %d   %d   %d   %d   %d   %d\n",
                        *p++,*p++,*p++,*p++,*p++,*p++,*p++,*p++,*p++,*p++,*p++,*p++,*p++,*p++,*p++,*p++,*p++,*p++,*p++,*p++);
                trace("%s",buf);
            }
*/
            DY--;
            Y0++;
        }
    }
    trace(" ctk%d MAX_Q=%8.3f (%d,%d) dcc=%d\n",num+1,_max,mx,my,dcc);
    LVL_MAX=_max;
    _min=  0;
    if (tex_inf->btCamType==0) // BIK
        _max=0x3FFF;
    else
        _max=0xFFFF;
    for (n=0;n<10;n++)
    {
        _CC=0;  // Calc CC
        for (i=(int)_min;i<=(int)_max;i++)
        {
            _CC+=gist[i];
        }
        _MO=0;  // Calc MO
        for (i=(int)_min;i<=(int)_max;i++)
        {
            _Q=i;
            _MO+=_Q*gist[i];
        }
        _MO/=_CC;
        aver=(int)_MO;
        _SKO=0; // Calc SKO
        for (i=(int)_min;i<=(int)_max;i++)
        {
            _Q=i;
            _SKO+=(_Q-_MO)*(_Q-_MO)*gist[i];
        }
        _SKO=sqrt(_SKO/_CC);
        _min=_MO-_SKO*3;
        if (_min<0x0000) _min=0x0000;
        if (_min>0x3FFF) _min=0x3FFF;
        _max=_MO+_SKO*3;
        if (_max<0x0000) _max=0x0000;
        if (_max>0xFFFF) _max=0xFFFF;
    } // for (n=0;n<10;n++)
    return (int)(_MO+_SKO*3.0+0.5);
}

int Base::end_obrab(int num,int nnn)
{
    int qqq=0,n=0,is_false=0;
    float VX_aver=0,sko=0;
    float VY_aver=0,_dV,v1=0,v2=0; //,max1=0,max2=0;

    // find avers Vx
    for (int i=0;i<nnn;i++)
    {
        if (m_objs[i].N < OBRAB.Nkadr/2.+0.51) continue;
        VX_aver+=m_objs[i].VX;
        n++;
    }
    if (n>0)
    {
        VX_aver=VX_aver/n;
        if(fabsf(OBRAB.VXzv-VX_aver)>0.5)
            trace("ctk%d VXzv=%8.3f VX_aver=%8.3f DVX=%8.3f\n",num+1,OBRAB.VXzv,VX_aver,fabsf(VX_aver-OBRAB.VXzv));
    }

    for (int i=0;i<nnn;i++)
    { // DELETE
        is_false=0;
        if (m_objs[i].N < OBRAB.Nkadr/2.+0.51) continue;
        if (m_objs[i].T>24*3600.0) //new day
            m_objs[i].T-=24*3600.0;
        if(correct_by_speed)
        {
            if(fabsf(m_objs[i].VX-VX_aver)<1) //  && fabsf(ctk->m_objs[i].VY-vy)>1) //while vy-VY_aver may be >1
            {// may be mistake
                for(int j=0;j<nnn;j++)
                {
                    if(j==i)  continue;
                    if(m_objs[j].V<200000 || m_objs[j].N<OBRAB.Nkadr-1) continue;
                    if(fabsf(m_objs[i].X-m_objs[j].X+(m_objs[j].T-m_objs[i].T)*m_objs[i].VX)<5 &&
                       ((m_objs[j].Y<=H_KADR/2 && m_objs[i].Y<=H_KADR/2) ||
                        (m_objs[j].Y>=H_KADR/2 && m_objs[i].Y>=H_KADR/2)))
                    {is_false=1;
                        trace("ctk%d NNN(%d): X=%8.3f; Y=%8.3f; VX=%8.3f; VY=%8.3f; S=%8.3f; V=%8.3f; N=%d\n",
                              num+1,m_objs[i].ID,m_objs[i].X,m_objs[i].Y,m_objs[i].VX,m_objs[i].VY,m_objs[i].S,m_objs[i].V,m_objs[i].N);
                        trace("ctk%d NZv(%d): X=%8.3f; Y=%8.3f; VX=%8.3f; VY=%8.3f; S=%8.3f; V=%8.3f; N=%d\n",
                              num+1,m_objs[j].ID,m_objs[j].X,m_objs[j].Y,m_objs[j].VX,m_objs[j].VY,m_objs[j].S,m_objs[j].V,m_objs[j].N);

                        break;}
                }
            }
        }
        if (is_false==1) continue;
        v2+=m_objs[i].VY;
        memcpy(&m_objs[qqq],&m_objs[i],sizeof(sObj));
        qqq++;
    }
    if(qqq>0)
       {
        VY_aver=v2/qqq;
       }
    if(fabsf(OBRAB.VYzv-VY_aver)>0.5)
        trace("ctk%d VYzv=%8.3f VY_aver=%8.3f DVY=%8.3f\n",num+1,OBRAB.VYzv,VY_aver,fabsf(VY_aver-OBRAB.VYzv));
        for (int i=0;i<qqq;i++)
    {
        _dV= (m_objs[i].VX-VX_aver)*(m_objs[i].VX-VX_aver)+(m_objs[i].VY-VY_aver)*(m_objs[i].VY-VY_aver);
        trace("i=%d ID=%d _dV=%8.3f \n",i,m_objs[i].ID,_dV);
        sko+=_dV;
    }
    if(qqq==0) return 0;
    sko=sqrt(sko/qqq);

    sko_V=sko;
    VX_aver=VX_aver;
    VY_aver=VY_aver;

    v1=v2=0;
    sko=0; n=0;
    for (int i=0;i<qqq;i++)
    {
        _dV= (m_objs[i].VX-VX_aver)*(m_objs[i].VX-VX_aver)+(m_objs[i].VY-VY_aver)*(m_objs[i].VY-VY_aver);
        if(sqrt(_dV)>3*sko_V) continue;
        v1+=m_objs[i].VX;
        v2+=m_objs[i].VY;
        sko+=_dV;
        n++;
    }
    if(n>0)
    {
     VX_aver=v1/n;
     VY_aver=v2/n;
     sko=sqrt(sko/n);
     sko_V=sko;
     VX_aver=VX_aver;
     VY_aver=VY_aver;
    }
    trace("Average: VX= %8.3f VY= %8.3f sko_V=%8.3f \n",sko,VX_aver,VY_aver);
    return qqq;
}

void Base::pre_obrab(int num)
{
    int i,j,qqq,d=0;
    qqq=0;
    trace("preobrab enter ind_bigob=%d \n",ind_bigobj);
    if (fabs(OBRAB.VXzv)>10)
        d=fabs(OBRAB.VXzv)-9;
    for (i=0;i<q_sw;i++)
    {
        for (j=0;j<ind_bigobj;j++)
        {
            if(m_bigobj[j].S<2*PLOSH) continue;

            if (m_swrt[i].X<m_bigobj[j].Xmax+d && m_swrt[i].X>m_bigobj[j].Xmin-d &&
                ((m_swrt[i].Y<H_KADR/2 && m_bigobj[j].Ymin<H_KADR/2) ||
                 (m_swrt[i].Y>=H_KADR/2 && m_bigobj[j].Ymax>=H_KADR/2))
                )
                break;
        }
        if (j<ind_bigobj)
        {
            trace("ctk%d Delete m_swrt X=%8.3f Y=%8.3f S=%4d bigob=%d\n",num+1,m_swrt[i].X,m_swrt[i].Y,m_swrt[i].S,j);
            continue;
        }
        memcpy(&m_swrt[qqq],&m_swrt[i],sizeof(tObj));
        qqq++;
    }
    for (j=0;j<ind_bigobj;j++)
    {
        if(m_bigobj[j].S<2*PLOSH) continue;
        m_swrt[qqq].S=m_bigobj[j].S;
        m_swrt[qqq].V=m_bigobj[j].V;
        m_swrt[qqq].X=m_bigobj[j].X;
        m_swrt[qqq].Y=m_bigobj[j].Y;
        m_swrt[qqq].XX=m_swrt[qqq].XY=m_swrt[qqq].YY=0;
        trace("ctk%d Insert m_swrt X=%8.3f Y=%8.3f S=%4d bigob=%d\n",num+1,m_swrt[qqq].X,m_swrt[qqq].Y,m_swrt[qqq].S,j);
        qqq++;
    }
    q_sw=qqq;
}

void Base::obrab_bigobj(int num, int x0, int y0)
{
    bObj *ob=m_bigobj+ind_bigobj;
    u_short *kadr,*line,*p;
    u_short A;
    int left=x0,right=W_KADR,cnt_line=0;
    float SX=0,SY=0;
    kadr=(u_short*)(m_kadr+tex_inf->nVideoOffset);
    line=kadr+y0*W_KADR;
    p=line+x0;
    if (tex_inf->btCamType==0)
        A=((*p)>>2)&0x3FFF;
    else
        A=*p;
    if (A<50000)
    {
        trace("Error obrab_bigobj ctk%d val=%d\n",num+1,A);
        return;
    }
    left=right=x0;
    ob->X=ob->Xmin=ob->Xmax=x0;
    ob->Y=ob->Ymin=ob->Ymax=y0;
    ob->S=1;
    ob->V=A;
    ob->X=x0;
    ob->Y=y0;
    SX+=x0*A;
    SY+=y0*A;
    cnt_line++;

    for(int j=y0;j<H_KADR;j++)
    {
        if (j>y0)
        {
            p=line+right+1;
            for(int i=right+1;i>=0;i--)
            {
                if (tex_inf->btCamType==0)
                {
                    A=((*p)>>2)&0x3FFF;
                    p--;
                }
                else
                    A=*p--;

                if(A<50000)
                {
                    if(i>=left)
                        continue;
                    else
                        break;
                }
                if (i<left) left=i;
                if (i>right) right=i;
                cnt_line++;
                ob->S++;
                ob->V+=A;
                SX+=i*A;
                SY+=j*A;
            }
            if (p==line-1 && A>=50000) left=0;
            if(ob->Xmin>left) ob->Xmin=left;
            if(ob->Xmax<right) ob->Xmax=right;
        }
        if (cnt_line==0) break;

        p=line+right+1;
        for(int i=right+1;i<W_KADR;i++)
        {
            if (tex_inf->btCamType==0)
            {
                A=((*p)>>2)&0x3FFF;
                p++;
            }
            else
                A=*p++;

            if(A<50000)
            {
                right=i-1;
                break;
            }

            cnt_line++;
            ob->S++;
            ob->V+=A;
            SX+=i*A;
            SY+=j*A;
        }

        if (cnt_line==0) break;
        if (p==line+W_KADR && A>=50000) right=W_KADR-1;
        if(ob->Xmax<right) ob->Xmax=right;

        ob->Ymax=j;
        line+=W_KADR;
        cnt_line=0;
    }
    ob->X=(float)(SX/ob->V);
    ob->Y=(float)(SY/ob->V);
    trace(" ctk%d BIGOBJ=%d S=%d V=%d X=%8.3f(%d,%d) Y=%8.3f(%d,%d)\n",
          num+1,ind_bigobj,ob->S,ob->V,ob->X,ob->Xmin,ob->Xmax,ob->Y,ob->Ymin,ob->Ymax);
}

int Base::process_convolutions(int n)
{
    int LVLX[500],LVLY[500],m9[9],sum;
    nObj out;
    double FON;
    u_short _Q;
    int ii;
    CONVOLUTION conv;
    CONVOLUTION *cnv=&conv;
    u_short *m_prep,*p,*line,*inp;
    double maxy=0,maxx=0;
    int mx,my;
    int xx,yy,ww,hh;          //ww*hh=cnv->s???
    m_prep=(u_short*)(m_kadr+tex_inf->nVideoOffset);

    for (int i=0;i<n;i++)           //n=ConvolutionCount
    {
        memcpy(cnv,m_kadr+256+i*sizeof(CONVOLUTION),sizeof(CONVOLUTION));
        if (cnv->nID!=0x564f494b) continue;    //VOIK
        ww=hh=sqrt(cnv->nS);
        xx=cnv->nX-ww/2;
        yy=cnv->nY-hh/2;

        //сумма по строкам
        inp=m_prep;
        inp+=W_KADR*yy;
        ii=0;
        while (hh)
        {
            LVLX[ii]=0;
            line=inp;
            inp+=W_KADR;
            p=line+xx;
            for (int i=0;i<ww;i++)
            {
                _Q=*p++;
                if (tex_inf->btCamType==0) // BIK
                    _Q=(_Q>>2)&0x3FFF;
                LVLX[ii]+=_Q;
            }
            if(LVLX[ii]>maxx) {maxx=LVLX[ii];my=yy;}
            ii++;
            hh--;
            yy++;
        }

        ww=hh=sqrt(cnv->nS);
        xx=cnv->nX-ww/2;
        yy=cnv->nY-hh/2;

        //сумма по столбцам
        inp=m_prep;
        inp+=W_KADR*yy;
        ii=0;
        while (ww)
        {
            LVLY[ii]=0;
            line=inp;
            p=line+xx;
            for (int i=0;i<hh;i++)
            {
                _Q=*p;
                p+=W_KADR;
                if (tex_inf->btCamType==0) // BIK
                    _Q=(_Q>>2)&0x3FFF;
                LVLY[ii]+=_Q;
            }
            if(LVLY[ii]>maxy) {maxy=LVLY[ii];mx=xx;}
            ii++;
            ww--;
            xx++;
        }
        ww=hh=sqrt(cnv->nS);
        xx=cnv->nX-ww/2;
        yy=cnv->nY-hh/2;
        //центр объекта - mx,my maxx,maxy
        FON=min(LVLX[0]/ww,LVLY[0]/hh);

        memset(&out,0,sizeof(tObj));
        out.X=mx;out.Y=my;
        out.T=m_tob[obrab];
        xx++;yy++;ww-=2;hh-=2;          //крайние строки и столбцы - фон
        inp=m_prep;
        inp+=W_KADR*(yy);
        while (hh)
        {
            line=inp;
            inp+=W_KADR;
            p=line+xx;
            for (int i=0;i<ww;i++)
            {
                m9[0]=*(p-W_KADR-1);m9[1]=*(p-W_KADR);m9[2]=*(p-W_KADR+1);
                m9[3]=*(p-1);m9[4]=*(p);m9[5]=*(p+1);
                m9[6]=*(p+W_KADR-1);m9[7]=*(p+W_KADR);m9[8]=*(p+W_KADR+1);
                sum=0;
                for (int k=0;k<9;k++)
                    sum+=m9[k];
                sum=sum/9;
                _Q=*p++;
                if (tex_inf->btCamType==0) // BIK
                    _Q=(_Q>>2)&0x3FFF;
                if (sum>FON)                //берем точку
                {
                    out.S++;
                    out.V+=(_Q-FON);
                    out.X+=(_Q-FON)*(xx+i);
                    out.Y+=(_Q-FON)*yy;
                }
            }
            hh--;
            yy++;
        }
        out.X=out.X/out.V;      //центр тяжести
        out.Y=out.Y/out.V;
        lst.push_back(out);
/*
        int si=lst.size();
        //просмотр
        list <nObj>::iterator  Iter  = lst.begin();
        int i=0;
        for(; Iter != lst.end(); ++Iter)
        {
          nObj t=*Iter;
          //printf("Element No:%d  value:%d\n", i ,*Iter);
          i++;
        }
*/
    }
    return n;
}

void Base::fun_obrb(int num)
{
    int aver[4]={0,0,0,0};
    double t_1,t_2;
    double A;
    u_short *m_prep;

    trace("fun_obrb[%d] enter\n",num+1);

    OBRAB.Nkadr=NumKadrs();         //кадры для обработки
    if (OBRAB.Nkadr==0)
        return;
    is_OBRAB=1;
    OBRAB.VXzv=OBRAB.VYzv=OBRAB.VXob=OBRAB.VYob=0;          //?????
    t_1=getmks();
    for (int iN=0;;iN++)
    {
        t_2=getmks();
        if (!ReadKadr(iN)) break;
        if (is_OBRAB) trace("ctk%d obr_pipe read: t=%8.3f\n",num+1,getmks()-t_2);

        if (OBRAB.Nkadr)
        {
            m_tob[obrab]=tex_inf->btExpHour*3600+tex_inf->btExpMinutes*60+tex_inf->btExpSeconds+tex_inf->nExpMicroseconds/1000000.0+tex_inf->nExposition/2000000.0;
            trace("ctk%d Time obrab set: obrab=%d time=%8.3f\n",num+1,obrab,m_tob[obrab]);
            if (obrab>0)
            {
                if (m_tob[obrab]<m_tob[0] && (m_tob[0]-m_tob[obrab])>36000)  //new day
                    m_tob[obrab]+=24*3600.0;
            }
            A=(tex_inf->iDatchA%524288)*360./524288.;
            if(A>215) A=A-360;
            trace("ctk%d kadr=%d: obrab=%d; time=%d:%d:%d.%06d\n",
                  num+1,
                  tex_inf->nCadrNum,obrab,
                  tex_inf->btExpHour,tex_inf->btExpMinutes,tex_inf->btExpSeconds,tex_inf->nExpMicroseconds);

            trace("A=%8.3f: L=%8.3f; R=%8.3f\n",
                  A,
                  (tex_inf->iDatchL%524288)*360./524288.,
                  (tex_inf->iDatchR%524288)*360./524288.
                  );
            m_prep=(u_short*)(m_kadr+tex_inf->nVideoOffset);

            int xx, yy, ww, hh;
            ind_bigobj=0;
            trace(" ctk%d before process_fon: W=%d H=%d Cam=%d\n",num+1,W_KADR,H_KADR,tex_inf->btCamType);

            if (tex_inf->btCamType==1 || tex_inf->btCamType==3) //SHPK or BPK
            {
                ww=W_KADR/2;
                hh=H_KADR/2;

                for(int i=0;i<4;i++)
                {
                    if(i==0 || i==2)
                        xx=0;
                    else xx=W_KADR/2;
                    if(i==0 || i==1)
                        yy=0;
                    else yy=H_KADR/2;

                    if (tex_inf->btCamType==3) { yy+=2; hh-=4; }  //BPK

                    LVL_OBR[i]=process_fon(num,xx,ww,yy,hh,m_prep,aver[i]);
                    trace("ctk%d LVL_OBR=%d  LVL_MAX=%d MaxBright=%d aver=%d\n",num+1,LVL_OBR[i],LVL_MAX,tex_inf->wMaxBright,aver[i]);
                    LVL_OBR[i]=(2*LVL_OBR[i]+aver[i])/3;   //a+2*SKO
                }
            }
            else
            {
                xx=0;ww=W_KADR;
                yy=0;hh=H_KADR;
                LVL_OBR[0]=process_fon(num,xx,ww,yy,hh,m_prep,aver[0]);
//                ctk->LVL_OBR[0]=(2*ctk->LVL_OBR[0]+aver[0])/3;   //a+2*SKO
                trace("ctk%d LVL_OBR=%d  LVL_MAX=%d  aver=%d\n",num+1,LVL_OBR[0],LVL_MAX,aver[0]);
            }

            xx=0;ww=W_KADR;
            yy=0;hh=H_KADR;
            q_sw=process_lines(num,xx,ww,yy,hh,m_prep,LVL_OBR,aver,m_swrt);
            trace("ctk%d after process_lines\n",num+1);
            if (OBRAB.Nkadr)
            {
                q_ob=process_swrt2(num);
                trace("\tOBRAB ctk%d[%6d] q_sw=%4d q_ob=%4d\n"
                      ,num+1,n_kadr,q_sw,q_ob);
                process_convolutions(tex_inf->wConvolutionsCount);      //UAK
                obrab++;
                if (obrab==OBRAB.Nkadr)
                {
                    if (obrab==1) q_ob=q_sw;        //UAK
//                    send_tex(f_tcp,num);
                    send_objs(1,num);
                    OBRAB.Nkadr=0;
                    obrab=0;
                    is_OBRAB=0;
                    trace("OBRAB_READY ctk%d[%6d] q_ob=%4d\n"
                          ,num+1,n_kadr,q_ob);
                    break;                      //exit
                }
            }
        }
        t_1=getmks();

    }
    trace("fun_obrb[%d] leave\n",num+1);
    return;
}

