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
 * 函数名称：
 *   FFT()
 *
 * 参数:
 *   complex<double> * TD	- 指向时域数组的指针
 *   complex<double> * FD	- 指向频域数组的指针
 *   r						－2的幂数，即迭代次数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现快速付立叶变换。
 *
 ************************************************************************/
VOID WINAPI FFT(complex<double> * TD, complex<double> * FD, int r)
{
	// 付立叶变换点数
	LONG	count;
	
	// 循环变量
	int		i,j,k;
	
	// 中间变量
	int		bfsize,p;
	
	// 角度
	double	angle;
	
	complex<double> *W,*X1,*X2,*X;
	
	// 计算付立叶变换点数
	count = 1 << r;
	
	// 分配运算所需存储器
	W  = new complex<double>[count / 2];
	X1 = new complex<double>[count];
	X2 = new complex<double>[count];
	
	// 计算加权系数
	for(i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	
	// 将时域点写入X1
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// 采用蝶形算法进行快速付立叶变换
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
	
	// 重新排序
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
	
	// 释放内存
	delete W;
	delete X1;
	delete X2;
}
/*************************************************************************
 *
 * 函数名称：
 *   Fourier()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行付立叶变换。
 *
 ************************************************************************/
BOOL WINAPI Fourier(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 中间变量
	double	dTemp;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 进行付立叶变换的宽度和高度（2的整数次方）
	LONG	w;
	LONG	h;
	
	int		wp;
	int		hp;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 赋初值
	w = 1;
	h = 1;
	wp = 0;
	hp = 0;
	
	// 计算进行付立叶变换的宽度和高度（2的整数次方）
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
	
	// 分配内存
	complex<double> *TD = new complex<double>[w * h];
	complex<double> *FD = new complex<double>[w * h];
	
	// 行
	for(i = 0; i < h; i++)
	{
		// 列
		for(j = 0; j < w; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 给时域赋值
			TD[j + w * i] = complex<double>(*(lpSrc), 0);
		}
	}
	
	for(i = 0; i < h; i++)
	{
		// 对y方向进行快速付立叶变换
		FFT(&TD[w * i], &FD[w * i], wp);
	}
	
	// 保存变换结果
	for(i = 0; i < h; i++)
	{
		for(j = 0; j < w; j++)
		{
			TD[i + h * j] = FD[j + w * i];
		}
	}
	
	for(i = 0; i < w; i++)
	{
		// 对x方向进行快速付立叶变换
		FFT(&TD[i * h], &FD[i * h], hp);
	}
	
	// 行
	for(i = 0; i < h; i++)
	{
		// 列
		for(j = 0; j < w; j++)
		{
			// 计算频谱
			dTemp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() + 
				         FD[j * h + i].imag() * FD[j * h + i].imag()) / 100;
			
			// 判断是否超过255
			if (dTemp > 255)
			{
				// 对于超过的，直接设置为255
				dTemp = 255;
			}
			
			// 指向DIB第(i<h/2 ? i+h/2 : i-h/2)行，第(j<w/2 ? j+w/2 : j-w/2)个象素的指针
			// 此处不直接取i和j，是为了将变换后的原点移到中心
			//lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * 
				(lHeight - 1 - (i<h/2 ? i+h/2 : i-h/2)) + (j<w/2 ? j+w/2 : j-w/2);
			
			// 更新源图像
			* (lpSrc) = (BYTE)(dTemp);
		}
	}
	
	// 删除临时变量
	delete TD;
	delete FD;
	
	// 返回
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
           	*lpDIBBits=*lpDIBBits+20;
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

	// 图像付立叶变换
	
	// 获取文档
	CTestDoc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR    lpDIBBits;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的付立叶变换，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的付立叶变换！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用Fourier()函数进行付立叶变换
	if (::Fourier(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB)))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
	// 恢复光标
	EndWaitCursor();
	
}

