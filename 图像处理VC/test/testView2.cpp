// testView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "test.h"

#include "testDoc.h"
#include "testView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestView

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
		
		// 判断是否是打印
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
		
		// 输出DIB
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
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());/*找到 DIB 图像象素起始位置 */
	
	if (::DIBNumColors(lpDIB) != 256)
	{
		MessageBox("目前只支持256色位图的运算！", "系统提示" , MB_ICONINFORMATION | MB_OK);
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
           	*lpDIBBits=255-*lpDIBBits;
			*lpDIBBits++;
		}	
	// 调用RobertDIB()函数对DIB进行边缘检测
//	if (RobertDIB(lpDIBBits, WIDTHBYTES(::DIBWidth(lpDIB) * 8), ::DIBHeight(lpDIB)))
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
	}
///	else
//	{
//		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
//	}
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	EndWaitCursor();}

void CTestView::OnTest2() 
{
	// TODO: Add your command handler code here
	
}
#include <memory.h>
#include <stdio.h>
 
static unsigned char image[] = {
    50,     50,     50,     50,     50,
    50,     52,     48,     50,     50,
    50,     50,     50,     0,      50,
    50,     48,     53,     50,     50,
    50,     50,     50,     50,     50,
};
#define WIDTH  5
#define HEIGHT 5
 
void medianFilter (unsigned char* corrupted, unsigned char* smooth, int width, int height)  
{  
    memcpy ( smooth, corrupted, width*height*sizeof(unsigned char) );  
    for (int j=1;j<height-1;j++)  
    {  
        for (int i=1;i<width-1;i++)  
        {  
            int k = 0;  
            unsigned char window[9];  
            for (int jj = j - 1; jj < j + 2; ++jj)  
                for (int ii = i - 1; ii < i + 2; ++ii)  
                    window[k++] = corrupted[jj * width + ii];  
            //   Order elements (only half of them)  
            for (int m = 0; m < 5; ++m)  
            {  
                int min = m;  
                for (int n = m + 1; n < 9; ++n)  
                    if (window[n] < window[min])  
                        min = n;  
                //   Put found minimum element in its place  
                unsigned char temp = window[m];  
                window[m] = window[min];  
                window[min] = temp;  
            }  
            smooth[ j*width+i ] = window[4];  
        }  
    }  
}  
 
int main()
{
    unsigned char output[WIDTH * HEIGHT];
    int i, j;
    medianFilter(image, output, WIDTH, HEIGHT);
    for(i=0; i<HEIGHT; i++)
    {
        for(j=0; j<WIDTH; j++)
        {
            printf("%d\t", output[i*WIDTH+j]);
        }
        printf("\n");
    }
    return 0;
}

void CTestView::OnTest3() 
{
	// TODO: Add your command handler code here
	
}
void sharpenImage  (unsigned char* gray, unsigned char* smooth, int width, int height)  
{  
      
    int templates[25] = { -1, -4, -7, -4, -1,   
        -4, -16, -26, -16, -4,   
        -7, -26, 505, -26, -7,  
        -4, -16, -26, -16, -4,   
        -1, -4, -7, -4, -1 };         
    memcpy ( smooth, gray, width*height*sizeof(unsigned char) );  
    for (int j=2;j<height-2;j++)  
    {  
        for (int i=2;i<width-2;i++)  
        {  
            int sum = 0;  
            int index = 0;  
            for ( int m=j-2; m<j+3; m++)  
            {  
                for (int n=i-2; n<i+3; n++)  
                {  
                    sum += gray [ m*width + n] * templates[index++] ;  
                }  
            }  
            sum /= 273;  
            if (sum > 255)  
                sum = 255;  
            if (sum <0)  
                sum = 0;  
            smooth [ j*width+i ] = sum;  
        }  
    }  
}  

void CTestView::OnTest4() 
{
	// TODO: Add your command handler code here
	
}
