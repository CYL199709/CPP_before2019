// testView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "test.h"

#include "testDoc.h"
#include "testView.h"
#include "math.h"
#include<complex>
using namespace std;
#define PI 3.14159
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTestView
/*************************************************************************
 *
 * �������ƣ�
 *   FFT()
 *
 * ����:
 *   complex<double> * TD	- ָ��ʱ�������ָ��
 *   complex<double> * FD	- ָ��Ƶ�������ָ��
 *   r						��2������������������
 *
 * ����ֵ:
 *   �ޡ�
 *
 * ˵��:
 *   �ú�������ʵ�ֿ��ٸ���Ҷ�任��
 *
 ************************************************************************/
VOID WINAPI FFT(complex<double> * TD, complex<double> * FD, int r)
{
	// ����Ҷ�任����
	LONG	count;
	
	// ѭ������
	int		i,j,k;
	
	// �м����
	int		bfsize,p;
	
	// �Ƕ�
	double	angle;
	
	complex<double> *W,*X1,*X2,*X;
	
	// ���㸶��Ҷ�任����
	count = 1 << r;
	
	// ������������洢��
	W  = new complex<double>[count / 2];
	X1 = new complex<double>[count];
	X2 = new complex<double>[count];
	
	// �����Ȩϵ��
	for(i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	
	// ��ʱ���д��X1
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// ���õ����㷨���п��ٸ���Ҷ�任
	for(k = 0; k < r; k++)
	{
		for(j = 0; j < 1 << k; j++)
		{
			bfsize = 1 << (r-k);
			for(i = 0; i < bfsize / 2; i++)
			{
				p = j * bfsize;
				X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
				X2[i + p + bfsize / 2] = (X1[i + p] - X1[i + p + bfsize / 2]) * W[i * (1<<k)];
			}
		}
		X  = X1;
		X1 = X2;
		X2 = X;
	}
	
	// ��������
	for(j = 0; j < count; j++)
	{
		p = 0;
		for(i = 0; i < r; i++)
		{
			if (j&(1<<i))
			{
				p+=1<<(r-i-1);
			}
		}
		FD[j]=X1[p];
	}
	
	// �ͷ��ڴ�
	delete W;
	delete X1;
	delete X2;
}
/*************************************************************************
 *
 * �������ƣ�
 *   Fourier()
 *
 * ����:
 *   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
 *   LONG  lWidth       - Դͼ���ȣ���������
 *   LONG  lHeight      - Դͼ��߶ȣ���������
 *
 * ����ֵ:
 *   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
 *
 * ˵��:
 *   �ú���������ͼ����и���Ҷ�任��
 *
 ************************************************************************/
BOOL WINAPI Fourier(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// ָ��Դͼ���ָ��
	unsigned char*	lpSrc;
	
	// �м����
	double	dTemp;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ����ֵ
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// ������и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2�������η���
	while(w * 2 <= lWidth)
	{
		w *= 2;
		wp++;
	}
	
	while(h * 2 <= lHeight)
	{
		h *= 2;
		hp++;
	}
	
	// �����ڴ�
	complex<double> *TD = new complex<double>[w * h];
	complex<double> *FD = new complex<double>[w * h];
	
	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < w; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// ��ʱ��ֵ
			TD[j + w * i] = complex<double>(*(lpSrc), 0);
		}
	}
	
	for(i = 0; i < h; i++)
	{
		// ��y������п��ٸ���Ҷ�任
		FFT(&TD[w * i], &FD[w * i], wp);
	}
	
	// ����任���
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			TD[i + h * j] = FD[j + w * i];
		}
	}
	
	for(i = 0; i < w; i++)
	{
		// ��x������п��ٸ���Ҷ�任
		FFT(&TD[i * h], &FD[i * h], hp);
	}
	
	// ��
	for(i = 0; i < h; i++)
	{
		// ��
		for(j = 0; j < w; j++)
		{
			// ����Ƶ��
			dTemp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() + 
				         FD[j * h + i].imag() * FD[j * h + i].imag()) / 100;
			
			// �ж��Ƿ񳬹�255
			if (dTemp > 255)
			{
				// ���ڳ����ģ�ֱ������Ϊ255
				dTemp = 255;
			}
			
			// ָ��DIB��(i<h/2 ? i+h/2 : i-h/2)�У���(j<w/2 ? j+w/2 : j-w/2)�����ص�ָ��
			// �˴���ֱ��ȡi��j����Ϊ�˽��任���ԭ���Ƶ�����
			//lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * 
				(lHeight - 1 - (i<h/2 ? i+h/2 : i-h/2)) + (j<w/2 ? j+w/2 : j-w/2);
			
			// ����Դͼ��
			* (lpSrc) = (BYTE)(dTemp);
		}
	}
	
	// ɾ����ʱ����
	delete TD;
	delete FD;
	
	// ����
	return TRUE;
}
IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	//{{AFX_MSG_MAP(CTestView)
	ON_COMMAND(ID_TEST1, OnTest1)
	ON_COMMAND(ID_TEST2, OnTest2)
	ON_COMMAND(ID_TEST3, OnTest3)
	ON_COMMAND(ID_TEST4, OnTest4)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestView construction/destruction

CTestView::CTestView()
{
	// TODO: add construction code here

}

CTestView::~CTestView()
{
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestView drawing

void CTestView::OnDraw(CDC* pDC)
{
	BeginWaitCursor();

	CTestDoc* pDoc = GetDocument();
	
	ASSERT_VALID(pDoc);
	
	HDIB hDIB = pDoc->GetHDIB();
	
	if (hDIB != NULL)
	{
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		
		int cxDIB = (int) ::DIBWidth(lpDIB);
		int cyDIB = (int) ::DIBHeight(lpDIB);

		::GlobalUnlock((HGLOBAL) hDIB);
		
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		
		CRect rcDest;
		
		// �ж��Ƿ��Ǵ�ӡ
		if (pDC->IsPrinting())
		{
		
			int cxPage = pDC->GetDeviceCaps(HORZRES);
		
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
			
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch)
					/ ((double)cxDIB * cxInch));
			rcDest.right = cxPage;
			
			int temp = cyPage - (rcDest.bottom - rcDest.top);
			rcDest.bottom += temp/2;
			rcDest.top += temp/2;

		}
		else   
		{
			rcDest = rcDIB;
		}
		
		// ���DIB
		::PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());
	}
	
	EndWaitCursor();}

/////////////////////////////////////////////////////////////////////////////
// CTestView printing

BOOL CTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestView diagnostics

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDoc* CTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestView message handlers

void CTestView::OnTest1() 
{
	CTestDoc* pDoc = GetDocument();
	int i,j;
	int iH,iW;
	LPSTR	lpDIB;
	LPSTR   lpDIBBits;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());/*�ҵ� DIB ͼ��������ʼλ�� */
	
	if (::DIBNumColors(lpDIB) != 256)
	{
		MessageBox("Ŀǰֻ֧��256ɫλͼ�����㣡", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		return;
	}


	BeginWaitCursor();
	lpDIBBits = ::FindDIBBits(lpDIB);

	iH = ::DIBHeight(lpDIB);
	iW = ::DIBWidth(lpDIB); 

	for(i=0;i<iH;i++)
		for(j=0;j<iW;j++)
		{
           	*lpDIBBits=*lpDIBBits+20;
			*lpDIBBits++;
		}	
	// ����RobertDIB()������DIB���б�Ե���
//	if (RobertDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}
///	else
//	{
//		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
//	}
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	EndWaitCursor();
}

void CTestView::OnTest2() 
{
	// TODO: Add your command handler code here
	
}


void CTestView::OnTest3() 
{	

	// TODO: Add your command handler code here
	
}



void CTestView::OnTest4() 
{

	// ͼ����Ҷ�任
	
	// ��ȡ�ĵ�
	CTestDoc* pDoc = GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;
	
	// ָ��DIB����ָ��
	LPSTR    lpDIBBits;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ�ĸ���Ҷ�任�������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ�ĸ���Ҷ�任��", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����Fourier()�������и���Ҷ�任
	if (::Fourier(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		
		// ��������
		pDoc->SetModifiedFlag(TRUE);
		
		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
	
	// �������
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
	// �ָ����
	EndWaitCursor();
	
}

