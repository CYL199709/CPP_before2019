// testDoc.cpp : implementation of the CTestDoc class
//

#include "stdafx.h"
#include "test.h"

#include "testDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestDoc

IMPLEMENT_DYNCREATE(CTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CTestDoc, CDocument)
	//{{AFX_MSG_MAP(CTestDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDoc construction/destruction

CTestDoc::CTestDoc()
{
	// TODO: add one-time construction code here
	// ��ʼ������
	m_hDIB = NULL;
	m_palDIB = NULL;
	m_sizeDoc = CSize(1,1);
}

CTestDoc::~CTestDoc()
{
}

BOOL CTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTestDoc serialization

void CTestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CTestDoc diagnostics

#ifdef _DEBUG
void CTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestDoc commands

BOOL CTestDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
CFile file;
	CFileException fe;

	// ���ļ�
	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// ʧ��
		ReportSaveLoadException(lpszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		return FALSE;
	}
	
	DeleteContents();
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ���Ե���ReadDIBFile()��ȡͼ��
	TRY
	{
		m_hDIB = ::ReadDIBFile(file);
	}
	CATCH (CFileException, eLoad)
	{
		// ��ȡʧ��
		file.Abort();
		
		// �ָ������״
		EndWaitCursor();
		
		// ����ʧ��
		ReportSaveLoadException(lpszPathName, eLoad,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		
		m_hDIB = NULL;
		
		return FALSE;
	}
	END_CATCH
	
	// ��ʼ��DIB
	InitDIBData();
	
	// �ָ������״
	EndWaitCursor();
	
	// �ж϶�ȡ�ļ��Ƿ�ɹ�
	if (m_hDIB == NULL)
	{
		// ʧ�ܣ����ܷ�BMP��ʽ
		CString strMsg;
		strMsg = "��֧�ָ����͵�ͼ���ļ���";
		
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		return FALSE;
	}
	
	// �����ļ�����
	SetPathName(lpszPathName);
	
	// ��ʼ���ͱ��ΪFALSE
	SetModifiedFlag(FALSE);
	return TRUE;}

void CTestDoc::InitDIBData()
{
	// ��ʼ��DIB����

	// �жϵ�ɫ���Ƿ�Ϊ��
	if (m_palDIB != NULL)
	{
 		delete m_palDIB;
		m_palDIB = NULL;
	}
	
	// ���DIB����Ϊ�գ�ֱ�ӷ���
	if (m_hDIB == NULL)
	{
		return;
	}
	
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	
	// �ж�ͼ���Ƿ����
	if (::DIBWidth(lpDIB) > INT_MAX ||::DIBHeight(lpDIB) > INT_MAX)
	{
		::GlobalUnlock((HGLOBAL) m_hDIB);
		
		// �ͷ�DIB����
		::GlobalFree((HGLOBAL) m_hDIB);
		
		// ����DIBΪ��
		m_hDIB = NULL;
		
		CString strMsg;
		strMsg = "BMPͼ��̫��";
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		return;
	}
	
	// �����ĵ���С
	m_sizeDoc = CSize((int) ::DIBWidth(lpDIB), (int) ::DIBHeight(lpDIB));
	
	::GlobalUnlock((HGLOBAL) m_hDIB);
	
	// �����µ�ɫ��
	m_palDIB = new CPalette;
	
	// �ж��Ƿ񴴽��ɹ�
	if (m_palDIB == NULL)
	{
		// ʧ�ܣ��������ڴ治��
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB = NULL;
		return;
	}
	
	// ����CreateDIBPalette��������ɫ��
	if (::CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
	{
		delete m_palDIB;
		m_palDIB = NULL;
		return;
	}
}
