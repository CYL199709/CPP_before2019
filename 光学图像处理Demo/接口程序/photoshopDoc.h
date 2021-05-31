// photoshopDoc.h : interface of the CPhotoshopDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHOTOSHOPDOC_H__0A9D8BA1_1370_442E_96C7_56708207CA93__INCLUDED_)
#define AFX_PHOTOSHOPDOC_H__0A9D8BA1_1370_442E_96C7_56708207CA93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dibapi.h"
#include "edgecontour.h"

class CPhotoshopDoc : public CDocument
{
protected: // create from serialization only
	CPhotoshopDoc();
	DECLARE_DYNCREATE(CPhotoshopDoc)

// Attributes
public:
	HDIB GetHDIB() const
		{ return m_hDIB; }
	CPalette* GetDocPalette() const
		{ return m_palDIB; }
	CSize GetDocSize() const
		{ return m_sizeDoc; }
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhotoshopDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nColorIndex;
	void ReplaceHDIB(HDIB hDIB);
	void InitDIBData();
	virtual ~CPhotoshopDoc();
	COLORREF m_refColorBKG;		// ±³¾°É«
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPhotoshopDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		protected:
	HDIB m_hDIB;
	CPalette* m_palDIB;
	CSize m_sizeDoc;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHOTOSHOPDOC_H__0A9D8BA1_1370_442E_96C7_56708207CA93__INCLUDED_)
