// TargetToolDoc.h : interface of the CTargetToolDoc class
//


#pragma once


class CTargetToolDoc : public CDocument
{
protected: // create from serialization only
	CTargetToolDoc();
	DECLARE_DYNCREATE(CTargetToolDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CTargetToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


