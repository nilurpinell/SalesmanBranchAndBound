#pragma once


// Cuadro de di�logo de CGraphDimensionsDlg

class CGraphDimensionsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGraphDimensionsDlg)

public:
	CGraphDimensionsDlg(CWnd* pParent = NULL);   // Constructor est�ndar
	virtual ~CGraphDimensionsDlg();

// Datos del cuadro de di�logo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GRAPH_DIM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	int m_Vertices;
	int m_Aristas;
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};
