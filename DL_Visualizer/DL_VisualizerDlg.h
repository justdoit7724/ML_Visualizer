
#pragma once

#include "DXLib/DXLibExport.h"
#include "DXLib/Plot3DGraphic.h"
#include "DXLib/WorldGraphic.h"
#include "DXLib/Plot2DGraphic.h"
#include "MLLib/NeuralNetwork.h"
#include "MT_DL.h"
#include "InputPoints.h"


class CDLVisualizerDlg : public CDialogEx
{

	enum class OperatingStep
	{
		Stop,
		Work,
		Pause
	};


public:
	CDLVisualizerDlg(CWnd* pParent = nullptr);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DL_VISUALIZER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	


protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:

	MT_DL* m_dl;

	InputPoints* m_ip;

	std::vector<DX::Object*> m_outputObjs;
	ML::Matrix m_outputX;

	DX::Plot3DGraphic* m_3Input;
	DX::Plot3DGraphic* m_3Output;
	DX::Plot2DGraphic* m_2VerSlice;
	DX::Plot2DGraphic* m_2HorSlice;
	DX::Plot2DGraphic* m_2loss;
	DX::Graphic* m_curFocus;
	DirectX::XMFLOAT2 m_focusPt;
	
	DX::Object* m_objInputVer;
	DX::Object* m_objInputHor;
	DX::Object* m_objOutputVer;
	DX::Object* m_objOutputHor;

	bool m_click;
	DX::Object* m_clickPlane;
	DirectX::XMFLOAT3 m_clickPlanePt;
	DirectX::XMFLOAT2 m_clickScnPt;

	std::vector<CStatic*> m_layersNo;
	std::vector<CComboBox*> m_layersAct;
	std::vector<CEdit*> m_layersNum;
	CComboBox* m_loss;

	CEdit m_cNLayer;
	CButton m_cGenerate;
	CComboBox m_cAlpha;
	CEdit m_cEpoch;
	CComboBox m_cInput;

	OperatingStep m_step;
	int m_nLayer;

	std::vector<double> m_hist;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonLayerNumber();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void INPUT_SELCHANGE();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
