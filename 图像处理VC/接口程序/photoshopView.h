// photoshopView.h : interface of the CPhotoshopView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHOTOSHOPVIEW_H__415A3AF4_CCAD_4002_A38A_6963416C1F3E__INCLUDED_)
#define AFX_PHOTOSHOPVIEW_H__415A3AF4_CCAD_4002_A38A_6963416C1F3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPhotoshopView : public CView
{
protected: // create from serialization only
	CPhotoshopView();
	DECLARE_DYNCREATE(CPhotoshopView)

// Attributes
public:
	CPhotoshopDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhotoshopView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPhotoshopView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPhotoshopView)
	afx_msg void OnEdgeRobert();
	afx_msg void OnEdgePrewitt();
	afx_msg void OnEdgeKirsch();
	afx_msg void OnEdgeGauss();
	afx_msg void OnEdgeSobel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in photoshopView.cpp
inline CPhotoshopDoc* CPhotoshopView::GetDocument()
   { return (CPhotoshopDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHOTOSHOPVIEW_H__415A3AF4_CCAD_4002_A38A_6963416C1F3E__INCLUDED_)
