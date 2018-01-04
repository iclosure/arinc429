// TargetToolDoc.cpp : implementation of the CTargetToolDoc class
//

#include "stdafx.h"
#include "TargetTool.h"

#include "TargetToolDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTargetToolDoc

IMPLEMENT_DYNCREATE(CTargetToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CTargetToolDoc, CDocument)
END_MESSAGE_MAP()


// CTargetToolDoc construction/destruction

CTargetToolDoc::CTargetToolDoc()
{
	// TODO: add one-time construction code here

}

CTargetToolDoc::~CTargetToolDoc()
{
}

BOOL CTargetToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CTargetToolDoc serialization

void CTargetToolDoc::Serialize(CArchive& ar)
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


// CTargetToolDoc diagnostics

#ifdef _DEBUG
void CTargetToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTargetToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTargetToolDoc commands
