#include "PKVIDef.h"
#include "ScreenWindow.h"
#include "PKVIMain.h"

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif

#include <GL/glext.h>


GLfloat g_pFilter[3][3]={
	{1,1,1},
	{1,2,1},
	{1,1,1}
};

const GLubyte* pErr;
GLenum err=0;

void zvezda(float fCoordX,float fCoordY,float m_fMashtab,float fMarkerSize)
{
    float pfPoint[8];
    pfPoint[0]=fCoordX-fMarkerSize*m_fMashtab;
    pfPoint[1]=fCoordY;
    pfPoint[2]=fCoordX+fMarkerSize*m_fMashtab;
    pfPoint[3]=fCoordY;
    pfPoint[4]=fCoordX;
    pfPoint[5]=fCoordY-fMarkerSize*m_fMashtab;
    pfPoint[6]=fCoordX;
    pfPoint[7]=fCoordY+fMarkerSize*m_fMashtab;
    glBegin(GL_LINES);
        glVertex2i(pfPoint[0], pfPoint[1]);
        glVertex2i(pfPoint[2], pfPoint[3]);
        glVertex2i(pfPoint[4], pfPoint[5]);
        glVertex2i(pfPoint[6], pfPoint[7]);
    glEnd();
    pfPoint[0]=fCoordX-fMarkerSize*m_fMashtab/1.4;
    pfPoint[1]=fCoordY-fMarkerSize*m_fMashtab/1.4;
    pfPoint[2]=fCoordX+fMarkerSize*m_fMashtab/1.4;
    pfPoint[3]=fCoordY+fMarkerSize*m_fMashtab/1.4;
    pfPoint[4]=fCoordX+fMarkerSize*m_fMashtab/1.4;
    pfPoint[5]=fCoordY-fMarkerSize*m_fMashtab/1.4;
    pfPoint[6]=fCoordX-fMarkerSize*m_fMashtab/1.4;
    pfPoint[7]=fCoordY+fMarkerSize*m_fMashtab/1.4;
    glBegin(GL_LINES);
        glVertex2i(pfPoint[0], pfPoint[1]);
        glVertex2i(pfPoint[2], pfPoint[3]);
        glVertex2i(pfPoint[4], pfPoint[5]);
        glVertex2i(pfPoint[6], pfPoint[7]);
    glEnd();
}

DEFINE_EVENT_TYPE(wxEVT_CHANGE_ORIGIN)
DEFINE_EVENT_TYPE(wxEVT_CHANGE_MASHTAB)

BEGIN_EVENT_TABLE(CScreenWindow,wxWindow)
	//(*EventTable(CScreenWindow)
	EVT_PAINT(CScreenWindow::OnPaint)


	EVT_CLOSE(CScreenWindow::OnClose)
	EVT_SIZE(CScreenWindow::OnSize)
	EVT_LEFT_DOWN(CScreenWindow::OnLBtnDown)
	EVT_LEFT_UP(CScreenWindow::OnLBtnUp)
	EVT_MOTION(CScreenWindow::OnMotion)
	EVT_RIGHT_DOWN(CScreenWindow::OnRBtnDown)
	EVT_MIDDLE_DOWN(CScreenWindow::OnMBtnDown)
	EVT_MOUSEWHEEL(CScreenWindow::OnMouseWheel)
	EVT_ENTER_WINDOW(CScreenWindow::OnEnterWindow)
	EVT_LEAVE_WINDOW(CScreenWindow::OnLeaveWindow)
	//*)
	EVT_COMMAND(0, wxEVT_CHANGE_CADR_SIZE, CScreenWindow::OnChangeCadrSize)
END_EVENT_TABLE()

CScreenWindow::CScreenWindow(wxWindow* parent, wxWindowID id,	const wxPoint& pos,
							 const wxSize& size, long style, const wxString& name):
							 wxGLCanvas(parent, id,NULL,pos,size,style)
{
	m_bZoomed=FALSE;
	m_bCaptured=FALSE;
	m_bMarkerMode=false;
	m_pSmallWnd=NULL;
	m_marker.bVisible=false;
	m_marker.Coord=wxPoint(m_szCadrSize.x/2,m_szCadrSize.y/2);
	m_marker.nType=cross;
	m_fMashtab=1;
	m_fOldMashtab=1;
	m_bFirstData=true;
	m_glRC=NULL;                       //Rendering Context
	m_szCadrSize.x=m_szCadrSize.y=0;
	m_pBmpData=NULL;
	m_bLIK=false;
	m_fSmallMashtab=1;
	m_nRealCadrCY=0;
//	Bind(wxEVT_CHANGE_CADR_SIZE, &CScreenWindow::OnChangeCadrSize, this, id);

}

CScreenWindow::~CScreenWindow()
{
	if(m_glRC)
	{
		delete m_glRC;
		m_glRC=NULL;
	}
	if(m_pBmpData)
		delete [] m_pBmpData;

}

void CScreenWindow::OnPaint(wxPaintEvent& event)
{
	wxPaintDC dc(this);

	if(m_glRC==NULL || m_szCadrSize.x==0 || m_szCadrSize.y==0)
	{
		wxDCBrushChanger br(dc,*wxBLACK_BRUSH);
		dc.DrawRectangle(GetClientRect());
		return;
	}

	SetCurrent(*m_glRC);
 	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(2);

	if(m_bZoomed)
	{
		glDrawPixels(m_szCadrSize.x-m_fOriginX,m_szCadrSize.y-m_fOriginY,GL_RGB,GL_UNSIGNED_BYTE,m_pBmpData);
		glColor3d(0,1,0);
		float fOriginX=-m_fOriginX*m_fMashtab+m_fOffsetX;
		float fOriginY=-m_fOriginY*m_fMashtab+m_fOffsetY+1;
		glBegin(GL_LINE_LOOP);
			glVertex2i(fOriginX,fOriginY);
			glVertex2i(fOriginX,fOriginY+(m_szCadrSize.y-1)*m_fMashtab);
			glVertex2i(fOriginX+(m_szCadrSize.x-1)*m_fMashtab,fOriginY+(m_szCadrSize.y-1)*m_fMashtab);
			glVertex2i(fOriginX+(m_szCadrSize.x-1)*m_fMashtab,fOriginY);
		glEnd();

        glColor3d(1,1,0);

        for(int i=0; i<g_pFrame->m_nConvolutionCount; i++)
        {
			float fCoordX=g_pFrame->m_pConvolution[i].nX*m_fMashtab-m_fOriginX*m_fMashtab+m_fOffsetX;
			float fCoordY=g_pFrame->m_pConvolution[i].nY*m_fMashtab-m_fOriginY*m_fMashtab+m_fOffsetY;
			float pfPoint[8];
			float fMarkerSize=m_marker.nSize/2;

			if (g_pFrame->m_pConvolution[i].nID=='VOIK')
                glColor3d(1,0,0);
            else if (g_pFrame->m_pConvolution[i].nID=='KTZV')
            {
                glColor3d(1,1,1);
                continue;
            }
            else if (g_pFrame->m_pConvolution[i].nID=='KZ++')
            {
                //отрицательные координаты
                int32_t nx=g_pFrame->m_pConvolution[i].nX;
                int32_t ny=g_pFrame->m_pConvolution[i].nY;
                fCoordX=nx*m_fMashtab-m_fOriginX*m_fMashtab+m_fOffsetX;
                fCoordY=ny*m_fMashtab-m_fOriginY*m_fMashtab+m_fOffsetY;
                //каталожные звезды
                glColor3d(0,1,0);
                zvezda(fCoordX,fCoordY,m_fMashtab,fMarkerSize);
                //измеренные звезды
                fCoordX=g_pFrame->m_pConvolution[i].nS_XX*m_fMashtab-m_fOriginX*m_fMashtab+m_fOffsetX;
                fCoordY=g_pFrame->m_pConvolution[i].nS_YY*m_fMashtab-m_fOriginY*m_fMashtab+m_fOffsetY;
                glColor3d(0,0.5,1);
                zvezda(fCoordX,fCoordY,m_fMashtab,fMarkerSize);
                continue;
            }
            else
                glColor3d(1,1,0);
            pfPoint[0]=fCoordX-fMarkerSize*m_fMashtab;
            pfPoint[1]=fCoordY-fMarkerSize*m_fMashtab;
            pfPoint[2]=fCoordX+fMarkerSize*m_fMashtab;
            pfPoint[3]=fCoordY-fMarkerSize*m_fMashtab;
            pfPoint[4]=fCoordX+fMarkerSize*m_fMashtab;
            pfPoint[5]=fCoordY+fMarkerSize*m_fMashtab;
            pfPoint[6]=fCoordX-fMarkerSize*m_fMashtab;
            pfPoint[7]=fCoordY+fMarkerSize*m_fMashtab;
            glBegin(GL_LINE_LOOP);
                glVertex2i(pfPoint[0], pfPoint[1]);
                glVertex2i(pfPoint[2], pfPoint[3]);
                glVertex2i(pfPoint[4], pfPoint[5]);
                glVertex2i(pfPoint[6], pfPoint[7]);
            glEnd();
		}

		if(m_marker.bVisible)
		{
			glColor3d(1,0,0);
			float fMarkerSize=m_marker.nSize/2;
			float pfPoint[8];
			float fCoordX=m_marker.Coord.x*m_fMashtab-m_fOriginX*m_fMashtab+m_fOffsetX;
			float fCoordY=m_marker.Coord.y*m_fMashtab-m_fOriginY*m_fMashtab+m_fOffsetY;
			switch(m_marker.nType)
			{
			case cross:
				pfPoint[0]=fCoordX-fMarkerSize;
				pfPoint[1]=pfPoint[3]=fCoordY;
				pfPoint[2]=fCoordX+fMarkerSize;
				pfPoint[4]=pfPoint[6]=fCoordX;
				pfPoint[5]=fCoordY-fMarkerSize;
				pfPoint[7]=fCoordY+fMarkerSize;
				glBegin(GL_LINE);
					glVertex2i(pfPoint[0], pfPoint[1]);
					glVertex2i(pfPoint[2], pfPoint[3]);
					glVertex2i(pfPoint[4], pfPoint[5]);
					glVertex2i(pfPoint[6], pfPoint[7]);
				glEnd();
				break;
			case cross1:
				pfPoint[0]=fCoordX-fMarkerSize;
				pfPoint[1]=fCoordY-fMarkerSize;
				pfPoint[2]=fCoordX+fMarkerSize;
				pfPoint[3]=fCoordY+fMarkerSize;
				pfPoint[4]=fCoordX+fMarkerSize;
				pfPoint[5]=fCoordY-fMarkerSize;
				pfPoint[6]=fCoordX-fMarkerSize;
				pfPoint[7]=fCoordY+fMarkerSize;
				glBegin(GL_LINE);
					glVertex2i(pfPoint[0], pfPoint[1]);
					glVertex2i(pfPoint[2], pfPoint[3]);
					glVertex2i(pfPoint[4], pfPoint[5]);
					glVertex2i(pfPoint[6], pfPoint[7]);
				glEnd();
				break;
			case rect:
				pfPoint[0]=fCoordX-fMarkerSize;
				pfPoint[1]=fCoordY-fMarkerSize;
				pfPoint[2]=fCoordX+fMarkerSize;
				pfPoint[3]=fCoordY-fMarkerSize;
				pfPoint[4]=fCoordX+fMarkerSize;
				pfPoint[5]=fCoordY+fMarkerSize;
				pfPoint[6]=fCoordX-fMarkerSize;
				pfPoint[7]=fCoordY+fMarkerSize;
				glBegin(GL_LINE_LOOP);
					glVertex2i(pfPoint[0], pfPoint[1]);
					glVertex2i(pfPoint[2], pfPoint[3]);
					glVertex2i(pfPoint[4], pfPoint[5]);
					glVertex2i(pfPoint[6], pfPoint[7]);
				glEnd();
				break;
			case circle:
				break;
			}

		}
	}
	else
	{
		glDrawPixels(m_szCadrSize.x,m_szCadrSize.y,GL_RGB,GL_UNSIGNED_BYTE,m_pBmpData);
		glColor3d(0,1,0);
		float fOriginX=m_fOriginX*m_fSmallMashtab+m_fSmallOriginX;
		float fOriginY=m_fOriginY*m_fSmallMashtab+m_fSmallOriginY+1;

		glBegin(GL_LINE_LOOP);
			glVertex2i(fOriginX,fOriginY);
			glVertex2i(fOriginX,fOriginY+m_fDimY*m_fSmallMashtab-1);
			glVertex2i(fOriginX+m_fDimX*m_fSmallMashtab-1,fOriginY+m_fDimY*m_fSmallMashtab-1);
			glVertex2i(fOriginX+m_fDimX*m_fSmallMashtab-1,fOriginY);
		glEnd();
	}
	SwapBuffers();

}

void CScreenWindow::OnClose(wxCloseEvent& event)
{
}

void CScreenWindow::AddPaintData(u_char* pData, DWORD dwSize)
{
	if(m_pBmpData==NULL)
		return;
	if(!m_bLIK && dwSize!=m_szCadrSize.x*m_nRealCadrCY*3)
		return;
	if(m_bLIK)
	{
		memmove(m_pBmpData+dwSize,m_pBmpData,m_szCadrSize.x*m_szCadrSize.y*3-dwSize);
		memcpy(m_pBmpData,pData,dwSize);
	}
	else
		memcpy(m_pBmpData,pData,dwSize);
//	wxStopWatch sw;
//	if(sw.Time()>=30)
//	{
		Refresh(false);
		Update();
//		sw.Start();
//	}
}


void CScreenWindow::OnLBtnDown(wxMouseEvent& event)
{
	if(g_pFrame->GetSource()==0 || g_pFrame->m_bUU)
		return;

	if(m_bZoomed && !m_bMarkerMode)
	{
		m_bCaptured=TRUE;
		m_pStartCapture=event.GetPosition();
		::wxSetCursor(wxCursor(wxCURSOR_SIZING));
	}
	else if(m_bZoomed && m_bMarkerMode)
	{
		wxClientDC dc(this);

		dc.SetUserScale(m_fMashtab,m_fMashtab);
		dc.SetDeviceOrigin(-m_fOriginX*m_fMashtab+m_fOffsetX,-m_fOriginY*m_fMashtab+m_fOffsetY);

		m_marker.Coord=event.GetLogicalPosition(dc);
		m_marker.bVisible=true;
		g_pFrame->ChangeMarkerParams(&m_marker);

		Refresh();
	}
}

void CScreenWindow::OnLBtnUp(wxMouseEvent& event)
{
	if(g_pFrame->GetSource()==0 || g_pFrame->m_bUU)
		return;

	if(m_bZoomed && m_bCaptured)
	{
		m_bCaptured=FALSE;
		::wxSetCursor(wxCURSOR_HAND);
	}
}

void CScreenWindow::OnMotion(wxMouseEvent& event)
{
	if(g_pFrame->GetSource()==0 || g_pFrame->m_bUU)
		return;

	if(m_bZoomed && m_bCaptured && m_glRC)
	{
		wxPoint pt(event.GetPosition());
		wxPoint pOffset=m_pStartCapture-pt;
		m_pStartCapture=pt;
		m_fOriginX+=(pOffset.x/(m_fMashtab));
		m_fOriginY+=(pOffset.y/(m_fMashtab));
		wxSize sz=GetClientSize();
		if(m_fOriginX>m_szCadrSize.x-sz.x/m_fMashtab)
			m_fOriginX=m_szCadrSize.x-sz.x/m_fMashtab;
		if(m_fOriginY>m_szCadrSize.y-sz.y/m_fMashtab)
			m_fOriginY=m_szCadrSize.y-sz.y/m_fMashtab;
		if(m_fOriginX<0)
			m_fOriginX=0;
		if(m_fOriginY<0)
			m_fOriginY=0;
		SetCurrent(*m_glRC);

		glPixelStorei(GL_UNPACK_SKIP_PIXELS,m_fOriginX);
		glPixelStorei(GL_UNPACK_SKIP_ROWS,m_fOriginY);
		glPixelStorei(GL_UNPACK_ROW_LENGTH,m_szCadrSize.x);
		glPixelStorei(GL_UNPACK_IMAGE_HEIGHT,m_szCadrSize.y);
		Refresh();
		m_pSmallWnd->SetOrigin(m_fOriginX,m_fOriginY);
		wxPoint pOrigin;
		pOrigin.x=m_fOriginX;
		pOrigin.y=m_fOriginY;
		wxSize szDim;
		szDim.x=m_fDimX;
		szDim.y=m_fDimY;
		g_pFrame->ChangeViewParams(pOrigin, szDim,true);
	}
	event.Skip();
}

void CScreenWindow::SetMarkerParams(MARKER* pMarker)
{
	memcpy(&m_marker,pMarker,sizeof(MARKER));
	Refresh();
}

void CScreenWindow::OnMouseWheel(wxMouseEvent& event)
{
	if(g_pFrame->GetSource()==0 || g_pFrame->m_bUU)
		return;
	if(m_bZoomed && m_bMarkerMode)
	{
		m_marker.nSize+=event.GetWheelRotation()>0?1:-1;
		if(m_marker.nSize<3)
            m_marker.nSize=3;
        else if(m_marker.nSize>255)
            m_marker.nSize=255;
		g_pFrame->ChangeMarkerParams(&m_marker);
		Refresh();
	}
	else if(m_bZoomed)
	{
		m_fOldMashtab=m_fMashtab;
		m_fMashtab+=((event.GetWheelRotation()>0)?0.1:-0.1);
		if(m_fMashtab<0.1)
			m_fMashtab=0.1;
		Change();
	}
}

void CScreenWindow::OnMBtnDown(wxMouseEvent& event)
{
	if(g_pFrame->GetSource()==0 || g_pFrame->m_bUU)
		return;

	if(m_bZoomed && m_bMarkerMode)
	{
		int nType=(int)m_marker.nType;
		nType++;
		if(nType>2)
		{
			nType=0;
		    m_marker.bVisible=false;
		}
		m_marker.nType=(marker_type)nType;
		g_pFrame->ChangeMarkerParams(&m_marker);

		Refresh();
	}
}

void CScreenWindow::OnRBtnDown(wxMouseEvent& event)
{
	if(g_pFrame->GetSource()==0 || g_pFrame->m_bUU)
		return;

	if(m_bZoomed && !m_bMarkerMode)
	{
		if(!m_bCaptured)
		{
			::wxSetCursor(wxCURSOR_MAGNIFIER);
			m_bMarkerMode=true;
		}
	}
	else if(m_bZoomed && m_bMarkerMode)
	{
			::wxSetCursor(wxCURSOR_HAND);
			m_bMarkerMode=false;

	}
}


void CScreenWindow::OnEnterWindow(wxMouseEvent& event)
{
	if(g_pFrame->GetSource()==0 || g_pFrame->m_bUU)
        ::wxSetCursor(wxCURSOR_ARROW);
	else if(m_bZoomed && !m_bMarkerMode && m_bCaptured)
		::wxSetCursor(wxCURSOR_SIZING);
	else if(m_bZoomed && m_bMarkerMode && !m_bCaptured)
		::wxSetCursor(wxCURSOR_MAGNIFIER);
	else if(m_bZoomed)
		::wxSetCursor(wxCURSOR_HAND);
}

void CScreenWindow::OnLeaveWindow(wxMouseEvent& event)
{
	::wxSetCursor(wxNullCursor);
}

void CScreenWindow::SetOrigin(float fOriginX, float fOriginY)
{
	m_fOriginX=fOriginX;
	m_fOriginY=fOriginY;
	Change();
}


void CScreenWindow::SetMashtab(const float fMashtab)
{
	m_fMashtab=fMashtab;
	if(m_bZoomed)
	{
		m_pSmallWnd->SetMashtab(fMashtab);
	}
	Change();
}

void CScreenWindow::OnSize(wxSizeEvent& event)
{
	if(m_glRC && m_pBmpData)
	{
		const GLubyte* pErr;
		GLenum err=0;
		SetCurrent(*m_glRC);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho( 0, event.GetSize().x, event.GetSize().y, 0, 0, 1 );
		glViewport(0,0,(GLsizei)event.GetSize().x,(GLsizei)event.GetSize().y);

//		gluOrtho2D(0,(GLdouble)event.GetSize().x,(GLdouble)event.GetSize().y,0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glViewport(0,0,(GLsizei)event.GetSize().x,(GLsizei)event.GetSize().y);


		Change();
	}
}

void CScreenWindow::SetDimention(float fDimX, float fDimY)
{
	if(!m_bZoomed)
	{
		m_fDimX=fDimX;
		m_fDimY=fDimY;
		Refresh();
	}
}

void CScreenWindow::OnChangeCadrSize(wxCommandEvent& event)
{
	wxSize* pCadrSize=(wxSize*)event.GetClientData();
	SetCadrSize(*pCadrSize);
}

void CScreenWindow::SetContrast(float fContrast)
{
	if(m_glRC)
	{
		SetCurrent(*m_glRC);
		glPixelTransferf(GL_RED_SCALE,fContrast);
		glPixelTransferf(GL_BLUE_SCALE,fContrast);
		glPixelTransferf(GL_GREEN_SCALE,fContrast);
		Refresh();
	}
}

void CScreenWindow::SetBright(float fBright)
{
	if(m_glRC)
	{
		SetCurrent(*m_glRC);
		glPixelTransferf(GL_RED_BIAS,fBright);
		glPixelTransferf(GL_BLUE_BIAS,fBright);
		glPixelTransferf(GL_GREEN_BIAS,fBright);
		Refresh();
	}

}

void CScreenWindow::SetCadrSize(wxSize szCadrSize)
{
	bool bLIK=g_pFrame->m_bLIK;
	wxSize ClientSize=GetClientSize();
	if(m_bFirstData && IsShownOnScreen())
	{
//		const GLubyte* pErr;
//		GLenum err=0;
		float pMap[256];
		for(int i=0; i<256; i++)
			pMap[i]=i/256.0;
		m_glRC = new wxGLContext(this);
		SetCurrent(*m_glRC);
		glClearColor(0,0,0,0);
		glDisable(GL_DITHER);                   //dither - дрожь
		glDisable(GL_MULTISAMPLE);              //быстрый и совершенный алгоритм сглаживания
//		if(!m_bZoomed)
//			glEnable(GL_CONVOLUTION_2D);

//		glPixelTransferi(GL_MAP_COLOR,true);
		float* pfMapGamma=g_pFrame->m_pfMapGamma;
//		glPixelMapfv(GL_PIXEL_MAP_R_TO_R,256,pfMapGamma);
//		glPixelMapfv(GL_PIXEL_MAP_G_TO_G,256,pfMapGamma);
//		glPixelMapfv(GL_PIXEL_MAP_B_TO_B,256,pfMapGamma);

		glClear(GL_COLOR_BUFFER_BIT);       //очистить color buffer
		glShadeModel(GL_FLAT);              //select flat or smooth shading
//		if(!m_bZoomed)
//		{
//			glConvolutionFilter2D(GL_CONVOLUTION_2D,GL_LUMINANCE,3,3,GL_LUMINANCE,GL_FLOAT,g_pFilter);
//			glPixelTransferf(GL_POST_CONVOLUTION_RED_SCALE,0.1);
//			glPixelTransferf(GL_POST_CONVOLUTION_GREEN_SCALE,0.1);
//			glPixelTransferf(GL_POST_CONVOLUTION_BLUE_SCALE,0.1);
//		}
		glMatrixMode(GL_PROJECTION);        //команды относятся к проекту.
		glLoadIdentity();                   //считывает текущую матрицу
		glOrtho(0,(GLdouble)ClientSize.x,(GLdouble)ClientSize.y,0,0,1);     //проективную матрицу таким образом, что ты получаеш ортогональные преобразования своего пространства на плоскость эрана(без эффекта перспективы)
        //Parameters left, right The coordinates for the left-and right-vertical clipping planes. bottom, top The coordinates for the bottom- and top-horizontal clipping planes. zNear, zFar The distances to the nearer and farther depth clipping planes. These distances are negative if the plane is to be behind the viewer.
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glViewport(0,0,ClientSize.x,ClientSize.y);
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);

// Reset The Current Matrix to E

//		glViewport(0,0,(GLsizei)ClientSize.x,(GLsizei)ClientSize.y);
//		glMatrixMode(GL_PROJECTION);
//		glLoadIdentity();
//		err=glGetError();
//		if(err!=0)
//		{
//			pErr=gluErrorString(err);
//		}
		m_bFirstData=false;
	}
	if(m_szCadrSize.x*m_nRealCadrCY!=szCadrSize.x*szCadrSize.y || bLIK!=m_bLIK)
	{
		m_bLIK=bLIK;
		if(m_bLIK)
		{
			m_szCadrSize.x=szCadrSize.x;
//			m_szCadrSize.y=max(ClientSize.y,szCadrSize.y);

			float fMashtab=(float)ClientSize.x/szCadrSize.x;
			m_szCadrSize.y=ClientSize.y/fMashtab;
		}
		else
			m_szCadrSize=szCadrSize;
		m_nRealCadrCY=szCadrSize.y;
		if(m_pBmpData)
			delete [] m_pBmpData;
		m_pBmpData=new u_char[m_szCadrSize.x*m_szCadrSize.y*3];
		Change();
	}
}

void CScreenWindow::SetGamma(float* pfMapGamma)
{
	if(m_glRC)
	{
//		SetCurrent(*m_glRC);
//		glPixelMapfv(GL_PIXEL_MAP_R_TO_R,256,pfMapGamma);
//		glPixelMapfv(GL_PIXEL_MAP_G_TO_G,256,pfMapGamma);
//		glPixelMapfv(GL_PIXEL_MAP_B_TO_B,256,pfMapGamma);
//		Refresh();
	}
}

void CScreenWindow::Change()
{

	if(m_glRC)
	{
		wxSize ClientSize=GetClientSize();
		SetCurrent(*m_glRC);
		if(m_bZoomed)
		{
			m_fOffsetX=(ClientSize.x-m_szCadrSize.x*m_fMashtab)/2;
			m_fOffsetY=(ClientSize.y-m_szCadrSize.y*m_fMashtab)/2;
			if(m_fOffsetX>0)
				m_fOriginX=0;
			else
			{
				m_fOriginX+=(ClientSize.x/m_fOldMashtab-ClientSize.x/m_fMashtab)/m_fMashtab;
				if(m_fOriginX>m_szCadrSize.x-ClientSize.x/m_fMashtab)
					m_fOriginX=m_szCadrSize.x-ClientSize.x/m_fMashtab;
				if(m_fOriginX<0)
					m_fOriginX=0;

				m_fOffsetX=0;
			}
			if(m_fOffsetY>0)
				m_fOriginY=0;
			else
			{
				m_fOriginY+=(ClientSize.y/m_fOldMashtab-ClientSize.y/m_fMashtab)/m_fMashtab;
				if(m_fOriginY>m_szCadrSize.y-ClientSize.y/m_fMashtab)
					m_fOriginY=m_szCadrSize.y-ClientSize.y/m_fMashtab;
				if(m_fOriginY<0)
					m_fOriginY=0;
				m_fOffsetY=0;
			}
			glRasterPos2i(m_fOffsetX,m_fOffsetY);           //установки положения нижнего левого угла
			glPixelZoom(m_fMashtab,-m_fMashtab);            //переворот по У
			glPixelStorei(GL_UNPACK_SKIP_PIXELS,m_fOriginX);    //количество строк и пикселов, которые следует пропустить
			glPixelStorei(GL_UNPACK_SKIP_ROWS,m_fOriginY);
			glPixelStorei(GL_UNPACK_ROW_LENGTH,m_szCadrSize.x);     //длина строки
			glPixelStorei(GL_UNPACK_IMAGE_HEIGHT,m_szCadrSize.y);
			m_fDimX=m_szCadrSize.x*m_fMashtab>ClientSize.x?ClientSize.x/m_fMashtab:m_szCadrSize.x;
			m_fDimY=m_szCadrSize.y*m_fMashtab>ClientSize.y?ClientSize.y/m_fMashtab:m_szCadrSize.y;
			m_pSmallWnd->SetDimention(m_fDimX,m_fDimY);

			wxPoint pOrigin;
			pOrigin.x=m_fOriginX;
			pOrigin.y=m_fOriginY;
			wxSize szDim;
			szDim.x=m_fDimX;
			szDim.y=m_fDimY;
			g_pFrame->ChangeViewParams(pOrigin, szDim,true);
			m_pSmallWnd->SetOrigin(m_fOriginX,m_fOriginY);
		}
		else
		{
			m_fSmallMashtab=min((float)ClientSize.x/m_szCadrSize.x,(float)ClientSize.y/m_szCadrSize.y);
			m_fSmallOriginX=(ClientSize.x-m_szCadrSize.x*m_fSmallMashtab)/2;
			m_fSmallOriginY=(ClientSize.y-m_szCadrSize.y*m_fSmallMashtab)/2;
			glRasterPos2i(m_fSmallOriginX,m_fSmallOriginY);
			glPixelZoom(m_fSmallMashtab,-m_fSmallMashtab);
		}
		Refresh();
	}
}

