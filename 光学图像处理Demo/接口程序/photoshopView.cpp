// photoshopView.cpp : implementation of the CPhotoshopView class
//

#include "stdafx.h"
#include "photoshop.h"

#include "photoshopDoc.h"
#include "photoshopView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhotoshopView

IMPLEMENT_DYNCREATE(CPhotoshopView, CView)

BEGIN_MESSAGE_MAP(CPhotoshopView, CView)
	//{{AFX_MSG_MAP(CPhotoshopView)
	ON_COMMAND(ID_EDGE_ROBERT, OnEdgeRobert)
	ON_COMMAND(ID_EDGE_PREWITT, OnEdgePrewitt)
	ON_COMMAND(ID_EDGE_KIRSCH, OnEdgeKirsch)
	ON_COMMAND(ID_EDGE_GAUSS, OnEdgeGauss)
	ON_COMMAND(ID_EDGE_SOBEL, OnEdgeSobel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhotoshopView construction/destruction

CPhotoshopView::CPhotoshopView()
{
	// TODO: add construction code here

}

CPhotoshopView::~CPhotoshopView()
{
}

BOOL CPhotoshopView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPhotoshopView drawing

void CPhotoshopView::OnDraw(CDC* pDC)
{
	BeginWaitCursor();
	CPhotoshopDoc* pDoc = GetDocument();
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
	
	EndWaitCursor();
}

/////////////////////////////////////////////////////////////////////////////
// CPhotoshopView diagnostics

#ifdef _DEBUG
void CPhotoshopView::AssertValid() const
{
	CView::AssertValid();
}

void CPhotoshopView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPhotoshopDoc* CPhotoshopView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPhotoshopDoc)));
	return (CPhotoshopDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPhotoshopView message handlers

void CPhotoshopView::OnEdgeRobert() 
{
	// TODO: Add your command handler code here
	//Robert��Ե�������

	CPhotoshopDoc* pDoc = GetDocument();
	LPSTR	lpDIB;
	LPSTR   lpDIBBits;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	if (::DIBNumColors(lpDIB) != 256)
	{
		MessageBox("Ŀǰֻ֧��256ɫλͼ�����㣡", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		return;
	}
	
	BeginWaitCursor();
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ����RobertDIB()������DIB���б�Ե���
	if (RobertDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	EndWaitCursor();
		
}

void CPhotoshopView::OnEdgePrewitt() 
{
	// TODO: Add your command handler code here
		//Prewitt��Ե�������

	CPhotoshopDoc* pDoc = GetDocument();
	
	LPSTR	lpDIB;
	LPSTR   lpDIBBits;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	if (::DIBNumColors(lpDIB) != 256)
	{
		MessageBox("Ŀǰֻ֧��256ɫλͼ�����㣡", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		return;
	}
	
	BeginWaitCursor();
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ����PrewittDIB()������DIB���б�Ե���
	if (PrewittDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}

	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	EndWaitCursor();
	
}

void CPhotoshopView::OnEdgeKirsch() 
{
	// TODO: Add your command handler code here
	//Kirsch��Ե�������
	CPhotoshopDoc* pDoc = GetDocument();
	
	LPSTR	lpDIB;
    LPSTR   lpDIBBits;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	if (::DIBNumColors(lpDIB) != 256)
	{
		MessageBox("Ŀǰֻ֧��256ɫλͼ�����㣡", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		return;
	}

	BeginWaitCursor();

	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ����KirschDIB()������DIB���б�Ե���
	if (KirschDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}

	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	EndWaitCursor();
	
}

void CPhotoshopView::OnEdgeGauss() 
{
	// TODO: Add your command handler code here
		//Gauss��Ե�������
	CPhotoshopDoc* pDoc = GetDocument();
	
	LPSTR	lpDIB;
	LPSTR   lpDIBBits;
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());

	if (::DIBNumColors(lpDIB) != 256)
	{
		MessageBox("Ŀǰֻ֧��256ɫλͼ�����㣡", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		return;
	}
	

	BeginWaitCursor();
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ����GaussDIB()������DIB���б�Ե���
	if (GaussDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	EndWaitCursor();
	
}

void CPhotoshopView::OnEdgeSobel() 
{
	// TODO: Add your command handler code here
	//Sobel��Ե�������

	CPhotoshopDoc* pDoc = GetDocument();
	
	LPSTR	lpDIB;
	LPSTR   lpDIBBits;

	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	if (::DIBNumColors(lpDIB) != 256)
	{
		MessageBox("Ŀǰֻ֧��256ɫλͼ�����㣡", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		return;
	}
	
	BeginWaitCursor();

	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ����SobelDIB()������DIB���б�Ե���
	if (SobelDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	EndWaitCursor();
		
	
}
