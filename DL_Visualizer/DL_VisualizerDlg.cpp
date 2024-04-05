
// DL_VisualizerDlg.cpp: 구현 파일
//

#include "pch.h"
#include <random>
#include "framework.h"

#include "DL_Visualizer.h"
#include "DL_VisualizerDlg.h"
#include "afxdialogex.h"

#include "DXLib/Object.h"
#include "DXLib/Transform.h"
#include "DXLib/Mesh.h"
#include "DXLib/Collider.h"
#include "DXLib/Camera.h"
#include "DXLib/SphereMesh.h"
#include "DXLib/Text.h"
#include "MLLib/LogisticRegression.h"

#include "IP_Simple.h"
#include "IP_Uniform.h"
#include "IP_Radius.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_GENERAL 101
#define TIMER_ML 102 
#define PLOT_SCATTER_RAD 3.3

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDLVisualizerDlg 대화 상자



CDLVisualizerDlg::CDLVisualizerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DL_VISUALIZER_DIALOG, pParent), m_3Input(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDLVisualizerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX); 
	DDX_Control(pDX, IDC_EDIT_LAYER_SIZE, m_cNLayer); 
	DDX_Control(pDX, IDC_BUTTON_LAYER_NUMBER, m_cGenerate);
	DDX_Control(pDX, IDC_COMBO_ALPHA, m_cAlpha);
	DDX_Control(pDX, IDC_EDIT_EPOCH, m_cEpoch);
	DDX_Control(pDX, IDC_COMBO_INPUT, m_cInput);
	DDX_Control(pDX, IDC_COMBO_WEIGHTS, m_cbWeights);
	DDX_Control(pDX, IDC_LIST_WEIGHTS, m_lbWeights);
	

}

BEGIN_MESSAGE_MAP(CDLVisualizerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_LAYER_NUMBER, &CDLVisualizerDlg::OnBnClickedButtonLayerNumber)
	ON_BN_CLICKED(IDC_BUTTON_START, &CDLVisualizerDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CDLVisualizerDlg::OnBnClickedButtonStop)
	ON_CBN_SELCHANGE(IDC_COMBO_INPUT, &CDLVisualizerDlg::INPUT_SELCHANGE)
	ON_WM_MOUSEWHEEL()
	ON_CBN_SELCHANGE(IDC_COMBO_WEIGHTS, &CDLVisualizerDlg::CBWEIGHTS_SELCHANGE)
END_MESSAGE_MAP()



BOOL CDLVisualizerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	SetIcon(m_hIcon, TRUE);	
	SetIcon(m_hIcon, FALSE);



	srand(time(NULL));


	SetTimer(TIMER_GENERAL, 15, nullptr);
	SetTimer(TIMER_ML, 40, nullptr);
	


	m_dl = new MT_DL();
	
	m_ip = nullptr;
	m_clickPlane = nullptr;
	m_2loss = nullptr;

	m_3Input = new DX::Plot3DGraphic(GetDlgItem(IDC_STATIC_PLOT_INPUT)->m_hWnd);
	m_3Input->EnableCamMovement(true);
	
	auto origin = m_3Input->GetOrigin();
	auto size = m_3Input->GetPlotSize();
	auto end = origin;
	end.x += size.x;
	end.y += size.y;
	end.z += size.z;

	auto center = origin;
	center.x += size.x / 2;
	center.y += size.y / 2;
	center.z += size.z / 2;

	m_3Output = new DX::Plot3DGraphic(GetDlgItem(IDC_STATIC_PLOT_OUTPUT)->m_hWnd);
	m_3Output->EnableCamMovement(true);
	

	m_2VerSlice = new DX::Plot2DGraphic(GetDlgItem(IDC_STATIC_PLOT_SLICE_VER)->m_hWnd);


	m_2HorSlice = new DX::Plot2DGraphic(GetDlgItem(IDC_STATIC_PLOT_SLICE_HOR)->m_hWnd);
	
	m_2loss = new DX::Plot2DGraphic(GetDlgItem(IDC_STATIC_PLOT_LOSS)->m_hWnd);
	m_2loss->EnableXUnit(false);


	DX::Actor* tmp;
	m_3Input->CreateActor(DX::ActorKind::Object, &tmp);
	m_objInputHor = (DX::Object*)tmp;
	m_3Input->CreateActor(DX::ActorKind::Object, &tmp);
	m_objInputVer = (DX::Object*)tmp;

	m_3Output->CreateActor(DX::ActorKind::Object, &tmp);
	m_objOutputHor = (DX::Object*)tmp;
	m_3Output->CreateActor(DX::ActorKind::Object, &tmp);
	m_objOutputVer = (DX::Object*)tmp;

	m_objInputHor->SetPriority(1);
	m_objInputHor->SetUnlit(true);
	m_objInputHor->GetTransform()->SetTranslation(center);
	DirectX::XMFLOAT3 sliceHorScale = { 100, 100, 2 };
	DirectX::XMFLOAT3 sliceVerScale = { 100, 50, 2 };
	m_objInputHor->GetTransform()->SetScale(sliceHorScale);
	m_objInputHor->GetTransform()->SetRot({ 0,1,0 });
	m_objInputHor->SetOutlineColor(DirectX::XMFLOAT4(1, 0, 1, 1));
	m_objInputHor->GetShape()->SetColor(DirectX::XMFLOAT4(1, 0, 0, 0.1));
	m_objInputHor->GetCollider()->SetScale(sliceHorScale);

	m_objInputVer->SetPriority(1);
	m_objInputVer->SetUnlit(true);
	m_objInputVer->GetTransform()->SetTranslation(center);
	m_objInputVer->GetTransform()->SetScale(sliceVerScale);
	m_objInputVer->GetTransform()->SetRot({ 1,0,0 });
	m_objInputVer->SetOutlineColor(DirectX::XMFLOAT4(1, 0, 1, 1));
	m_objInputVer->GetShape()->SetColor(DirectX::XMFLOAT4(1, 0, 0, 0.1));
	m_objInputVer->GetCollider()->SetScale(sliceVerScale);

	m_objOutputHor->SetPriority(1);
	m_objOutputHor->SetUnlit(true);
	m_objOutputHor->GetTransform()->SetTranslation(center);
	m_objOutputHor->GetTransform()->SetScale(sliceHorScale);
	m_objOutputHor->GetTransform()->SetRot({ 0,1,0 });
	m_objOutputHor->SetOutlineColor(DirectX::XMFLOAT4(1, 0, 1, 1));
	m_objOutputHor->GetShape()->SetColor(DirectX::XMFLOAT4(1, 0, 0, 0.1));
	m_objOutputHor->GetCollider()->SetScale(sliceHorScale);

	m_objOutputVer->SetPriority(1);
	m_objOutputVer->SetUnlit(true);
	m_objOutputVer->GetTransform()->SetTranslation(center);
	m_objOutputVer->GetTransform()->SetScale(sliceVerScale);
	m_objOutputVer->GetTransform()->SetRot({ 1,0,0 });
	m_objOutputVer->SetOutlineColor(DirectX::XMFLOAT4(1, 0, 1, 1));
	m_objOutputVer->GetShape()->SetColor(DirectX::XMFLOAT4(1, 0, 0, 0.1));
	m_objOutputVer->GetCollider()->SetScale(sliceVerScale);


	m_layersNo.push_back((CStatic*)GetDlgItem(IDC_STATIC_LAYER1_NO));
	m_layersNo.push_back((CStatic*)GetDlgItem(IDC_STATIC_LAYER2_NO));
	m_layersNo.push_back((CStatic*)GetDlgItem(IDC_STATIC_LAYER3_NO));
	m_layersNo.push_back((CStatic*)GetDlgItem(IDC_STATIC_LAYER4_NO));
	m_layersNo.push_back((CStatic*)GetDlgItem(IDC_STATIC_LAYER5_NO));
	m_layersNo.push_back((CStatic*)GetDlgItem(IDC_STATIC_LAYER6_NO));
	m_layersNo.push_back((CStatic*)GetDlgItem(IDC_STATIC_LAYER7_NO));
	for (int i = 0; i < m_layersNo.size(); ++i)
	{
		m_layersNo[i]->SetWindowTextW(std::to_wstring(i + 1).c_str());
	}
	m_layersAct.push_back((CComboBox*)GetDlgItem(IDC_COMBO_LAYER1_ACT));
	m_layersAct.push_back((CComboBox*)GetDlgItem(IDC_COMBO_LAYER2_ACT));
	m_layersAct.push_back((CComboBox*)GetDlgItem(IDC_COMBO_LAYER3_ACT));
	m_layersAct.push_back((CComboBox*)GetDlgItem(IDC_COMBO_LAYER4_ACT));
	m_layersAct.push_back((CComboBox*)GetDlgItem(IDC_COMBO_LAYER5_ACT));
	m_layersAct.push_back((CComboBox*)GetDlgItem(IDC_COMBO_LAYER6_ACT));
	m_layersAct.push_back((CComboBox*)GetDlgItem(IDC_COMBO_LAYER7_ACT));
	for (int i = 0; i < m_layersAct.size(); ++i)
	{
		m_layersAct[i]->AddString(L"Linear");
		m_layersAct[i]->AddString(L"Sigmoid");
		m_layersAct[i]->AddString(L"Relu");
		m_layersAct[i]->AddString(L"Softmax");
	}
	m_layersNum.push_back((CEdit*)GetDlgItem(IDC_EDIT_LAYER1_NUM));
	m_layersNum.push_back((CEdit*)GetDlgItem(IDC_EDIT_LAYER2_NUM));
	m_layersNum.push_back((CEdit*)GetDlgItem(IDC_EDIT_LAYER3_NUM));
	m_layersNum.push_back((CEdit*)GetDlgItem(IDC_EDIT_LAYER4_NUM));
	m_layersNum.push_back((CEdit*)GetDlgItem(IDC_EDIT_LAYER5_NUM));
	m_layersNum.push_back((CEdit*)GetDlgItem(IDC_EDIT_LAYER6_NUM));
	m_layersNum.push_back((CEdit*)GetDlgItem(IDC_EDIT_LAYER7_NUM));

	m_loss = (CComboBox*)GetDlgItem(IDC_COMBO_LOSS);
	m_loss->AddString(L"Mean Squared");
	m_loss->AddString(L"Binary cross entropy");
	m_loss->AddString(L"Category cross entropy");
	m_loss->SetCurSel(0);

	m_cInput.AddString(L"Simple");
	m_cInput.AddString(L"Block");
	m_cInput.AddString(L"Radius");

	m_nLayer = 0;

	for (int i = -10; i <= 10; ++i)
	{
		auto strI = std::to_wstring(i);
		m_cAlpha.AddString(strI.c_str());
	}
	m_cAlpha.SetCurSel(5);
	SetText(&m_cEpoch, "1");

	OnBnClickedButtonStop();


	return TRUE;  
}

void CDLVisualizerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDLVisualizerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDLVisualizerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDLVisualizerDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case TIMER_GENERAL:
	{
		float spf = 1.0 / 10;
		if (!m_3Input)
			break;

		m_3Input->Update(spf);
		m_3Output->Update(spf);
		m_2VerSlice->Update(spf);
		m_2HorSlice->Update(spf);
		m_2loss->Update(spf);


		//sync
		if (m_curFocus == m_3Input)
		{
			m_objOutputVer->GetTransform()->SetTranslation(m_objInputVer->GetTransform()->GetPos());
			m_objOutputHor->GetTransform()->SetTranslation(m_objInputHor->GetTransform()->GetPos());

			m_3Output->m_camAngleX = m_3Input->m_camAngleX;
			m_3Output->m_camAngleY = m_3Input->m_camAngleY;
		}
		else if (m_curFocus == m_3Output)
		{
			m_objInputVer->GetTransform()->SetTranslation(m_objOutputVer->GetTransform()->GetPos());
			m_objInputHor->GetTransform()->SetTranslation(m_objOutputHor->GetTransform()->GetPos());

			m_3Input->m_camAngleX = m_3Output->m_camAngleX;
			m_3Input->m_camAngleY = m_3Output->m_camAngleY;
		}

		//Weights
		switch (m_step)
		{
		case OperatingStep::Work:
		{
			CBWEIGHTS_SELCHANGE();
		}
		break;
		}


		//UI activating
		switch (m_step)
		{
		case OperatingStep::Stop:
			SetDlgItemText(IDC_BUTTON_START, L"▶");
			m_cNLayer.EnableWindow();
			m_cGenerate.EnableWindow();
			for (int i = 0; i < m_nLayer; ++i)
			{
				m_layersNo[i]->EnableWindow();
				m_layersAct[i]->EnableWindow();
				m_layersNum[i]->EnableWindow();
			}
			m_loss->EnableWindow();
			m_cEpoch.EnableWindow();
			m_cAlpha.EnableWindow();
			m_cInput.EnableWindow();
			break;
		case OperatingStep::Work:
		{
			SetDlgItemText(IDC_BUTTON_START, L"❚❚");
			m_cNLayer.EnableWindow(FALSE);
			m_cGenerate.EnableWindow(FALSE);

			for (int i = 0; i < m_nLayer; ++i)
			{
				m_layersNo[i]->EnableWindow(false);
				m_layersAct[i]->EnableWindow(false);
				m_layersNum[i]->EnableWindow(false);
			}
			m_loss->EnableWindow(false);
			m_cEpoch.EnableWindow(false);
			m_cAlpha.EnableWindow(false);
			m_cInput.EnableWindow(false);
		}
		break;
		case OperatingStep::Pause:
			SetDlgItemText(IDC_BUTTON_START, L"▶");
			m_cNLayer.EnableWindow(FALSE);
			m_cGenerate.EnableWindow(FALSE);
			for (int i = 0; i < m_nLayer; ++i)
			{
				m_layersNo[i]->EnableWindow(false);
				m_layersAct[i]->EnableWindow(false);
				m_layersNum[i]->EnableWindow(false);
			}
			m_loss->EnableWindow(false);
			m_cEpoch.EnableWindow(false);
			m_cAlpha.EnableWindow(false);
			m_cInput.EnableWindow(false);
			break;
		default:
			break;
		}
	}
		break;
	case TIMER_ML:
	{
		if (!m_3Input)
			break;

		if (m_step == OperatingStep::Work || m_step == OperatingStep::Pause)
		{
			if (m_step == OperatingStep::Work)
			{
				//Loss plot
				m_2loss->ClearPlot();
				auto hist = m_dl->GetHist();
				for (int i = 0; i < hist.size(); i++)
				{
					m_hist.push_back(hist[i]);
				}
#ifdef NDEBUG
				const int refreshMax = 16000;
#else
				const int refreshMax = 3000;
#endif
				if (m_hist.size() > refreshMax) // maintain size
				{
					m_hist.erase(m_hist.begin(), m_hist.begin() + refreshMax / 2);
				}

				std::vector<DirectX::XMFLOAT2> lossLines;

				for (int i = 0; i < m_hist.size(); i++)
				{
					if ((i % 3) == 0)
					{
						DirectX::XMFLOAT2 input;
						input.x = i;
						input.y = m_hist[i];
						lossLines.push_back(input);
					}
				}
				if (lossLines.size() > 1)
					m_2loss->Plot(lossLines, DirectX::XMFLOAT4(0, 0, 1, 1), 3, true);


				//ouput scatter
				if (m_outputObjs.size())
				{
					auto pred = m_dl->GetPredict(m_outputX);
					for (int i = 0; i < pred.size(); ++i)
					{
						m_outputObjs[i]->GetShape()->SetColor(m_ip->GetColor(pred[i]));
					}
				}
			}

			//slice plot

			static bool slice1 = true;
			slice1 = !slice1;

			float res = 40;
			auto origin = m_ip->GetMinPt();
			auto size = m_ip->GetMaxPt();
			size.x -= origin.x;
			size.y -= origin.y;
			size.z -= origin.z;
			ML::Matrix xInput;

			std::vector<std::vector<float>> surX;
			std::vector<std::vector<float>> surY;
			std::vector<std::vector<DirectX::XMFLOAT4>> surV;
			if (slice1) {
				m_2HorSlice->ClearSurface();
				float y = size.y * (m_objInputHor->GetTransform()->GetPos().y - m_3Input->GetOrigin().y) / m_3Input->GetPlotSize().y;
				for (int z = 0; z <= res; ++z)
				{
					std::vector<float> lineX;
					std::vector<float> lineY;
					for (int x = 0; x <= res; ++x)
					{
						auto pt = origin;
						pt.x += (x / res) * size.x;
						pt.y += y;
						pt.z += (z / res) * size.z;

						xInput.push_back({ pt.x, pt.y,pt.z });
						lineX.push_back(pt.x);
						lineY.push_back(pt.z);
					}
					surX.push_back(lineX);
					surY.push_back(lineY);
				}

				//pred
				auto pred = m_dl->GetPredict(xInput);
				if (pred.size())
				{
					for (int z = 0; z <= res; ++z)
					{
						std::vector<DirectX::XMFLOAT4> lineCol;
						for (int x = 0; x <= res; ++x)
						{
							int index = z * (res + 1) + x;

							lineCol.push_back(m_ip->GetColor(pred[index]));
						}
						surV.push_back(lineCol);
					}

					m_2HorSlice->Surface(surX, surY, surV);
				}
			}
			else
			{
				m_2VerSlice->ClearSurface();
				surX.clear();
				surY.clear();
				surV.clear();
				xInput.clear();
				float x = size.x * (m_objInputVer->GetTransform()->GetPos().x - m_3Input->GetOrigin().x) / m_3Input->GetPlotSize().x;
				for (int y = 0; y <= res; ++y)
				{
					std::vector<float> lineX;
					std::vector<float> lineY;
					for (int z = 0; z <= res; ++z)
					{
						auto pt = origin;
						pt.x += x;
						pt.y += (y / res) * size.y;
						pt.z += (z / res) * size.z;

						xInput.push_back({ pt.x, pt.y,pt.z });
						lineX.push_back(pt.z);
						lineY.push_back(pt.y);
					}
					surX.push_back(lineX);
					surY.push_back(lineY);
				}

				//pred
				auto pred = m_dl->GetPredict(xInput);
				if (pred.size())
				{
					for (int z = 0; z <= res; ++z)
					{
						std::vector<DirectX::XMFLOAT4> lineCol;
						for (int x = 0; x <= res; ++x)
						{
							int index = z * (res + 1) + x;

							lineCol.push_back(m_ip->GetColor(pred[index]));
						}
						surV.push_back(lineCol);
					}
					m_2VerSlice->Surface(surX, surY, surV);
				}
			}
		}

			
		
	}
	default:
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CDLVisualizerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_curFocus && (m_clickPlane = m_curFocus->PickObj(nullptr)))
	{
		m_click = true;

		CPoint rPoint = point;

		CRect rc=m_curFocus->GetWndSize();
		ScreenToClient(&rc);
		rPoint.x -= rc.left;
		rPoint.y -= rc.top;
		m_clickScnPt.x = rPoint.x;
		m_clickScnPt.y = rPoint.y;
		m_clickPlanePt = m_clickPlane->GetTransform()->GetPos();
	}
	

	if(m_curFocus)
		m_curFocus->MouseLClick(true);

		/*DX::Actor* tmp;
		m_3Input->CreateActor(DX::ActorKind::Object, &tmp);
		DX::Object* rayObj = (DX::Object*)tmp;
		DX::Geometrics::Ray ray;

		CPoint rPoint = point;
		rPoint.x -= rc.left;
		rPoint.y -= rc.top;

		m_3Input->MainCamera()->Pick({ (float)rPoint.x,(float)rPoint.y }, &ray);
		rayObj->GetTransform()->SetRot(ray.d);
		rayObj->GetTransform()->SetScale(2, 2, 1000);
		rayObj->GetTransform()->SetTranslation(ray.o);
		rayObj->GetShape()->SetColor(DirectX::XMFLOAT4(0, 0, 0, 1));
		rayObj->SetUnlit(true);*/

	


	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDLVisualizerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_curFocus)
	{
		m_curFocus->MouseLClick(false);
	}

	m_click = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDLVisualizerDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_curFocus)
	{
		m_curFocus->MouseRClick();
	}



	CDialogEx::OnRButtonDown(nFlags, point);
}


void CDLVisualizerDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_curFocus)
	{
		m_curFocus->MouseRClick(false);
	}

	CDialogEx::OnRButtonUp(nFlags, point);
}


void CDLVisualizerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc1, rc2, rc3, rc4;
	m_curFocus = nullptr;
	m_focusPt = { -1,-1 };
	GetDlgItem(IDC_STATIC_PLOT_INPUT)->GetWindowRect(&rc1);
	ScreenToClient(&rc1);
	GetDlgItem(IDC_STATIC_PLOT_OUTPUT)->GetWindowRect(&rc2);
	ScreenToClient(&rc2);
	GetDlgItem(IDC_STATIC_PLOT_SLICE_VER)->GetWindowRect(&rc3);
	ScreenToClient(&rc3);
	GetDlgItem(IDC_STATIC_PLOT_SLICE_HOR)->GetWindowRect(&rc4);
	ScreenToClient(&rc4);
	if (rc1.left <= point.x && point.x <= rc1.right && rc1.top <= point.y && point.y <= rc1.bottom)
	{
		m_curFocus = m_3Input;
		m_focusPt = { point.x - (float)rc1.left, point.y - (float)rc1.top };
	}
	else if (rc2.left <= point.x && point.x <= rc2.right && rc2.top <= point.y && point.y <= rc2.bottom)
	{
		m_curFocus = m_3Output;
		m_focusPt = { point.x - (float)rc2.left, point.y - (float)rc2.top };
	}
	else if (rc3.left <= point.x && point.x <= rc3.right && rc3.top <= point.y && point.y <= rc3.bottom)
	{
		m_curFocus = m_2VerSlice;
		m_focusPt = { point.x - (float)rc3.left, point.y - (float)rc3.top };
	}
	else if (rc4.left <= point.x && point.x <= rc4.right && rc4.top <= point.y && point.y <= rc4.bottom)
	{
		m_curFocus = m_2HorSlice;
		m_focusPt = { point.x - (float)rc4.left, point.y - (float)rc4.top };
	}


	if (m_curFocus)
	{
		m_curFocus->MousePT(m_focusPt.x, m_focusPt.y);
	}

	if (m_click && m_clickPlane)
	{
		DirectX::XMFLOAT3 planeDir = m_clickPlane->GetTransform()->GetForward();
		DirectX::XMFLOAT2 scnDir = m_3Input->Dir2Screen(planeDir);
		
		DirectX::XMFLOAT2 diff = { m_clickScnPt.x- m_focusPt.x,m_clickScnPt.y- m_focusPt.y };
		float dist = diff.x * scnDir.x + diff.y * scnDir.y;
		dist *= -0.6;

		auto plotSize = m_3Input->GetPlotSize();
		auto plotOrigin =m_3Input->GetOrigin();


		DirectX::XMFLOAT3 newPos = m_clickPlanePt;
		newPos.x += planeDir.x * dist;
		newPos.y += planeDir.y * dist;
		newPos.z += planeDir.z * dist;
		newPos.x = max(plotOrigin.x, min(plotOrigin.x + plotSize.x, newPos.x));
		newPos.y = max(plotOrigin.y, min(plotOrigin.y + plotSize.y, newPos.y));
		m_clickPlane->GetTransform()->SetTranslation(newPos);
	}

	
	
	//highlight
	m_objInputHor->m_outlineMode = false;
	m_objInputVer->m_outlineMode = false;
	m_objOutputHor->m_outlineMode = false;
	m_objOutputVer->m_outlineMode = false;
	{
		if (m_click && m_clickPlane)
			m_clickPlane->m_outlineMode = true;
		else
		{
			auto picked = m_3Input->PickObj(nullptr);
			if (picked)
				picked->m_outlineMode = true;
			picked = m_3Output->PickObj(nullptr);
			if (picked)
				picked->m_outlineMode = true;
		}
	}


	CDialogEx::OnMouseMove(nFlags, point);
}


void CDLVisualizerDlg::OnBnClickedButtonLayerNumber()
{
	auto str =GetText(&m_cNLayer);
	m_nLayer = min(m_layersAct.size(), max(atoi(str.c_str()), 1));

	float interval = 40;

	RECT groupRC;
	GetDlgItem(IDC_STATIC_LAYERS)->GetWindowRect(&groupRC);
	ScreenToClient(&groupRC);
	groupRC.left += 15;
	groupRC.top += 30;

	RECT actRC;
	m_layersAct[0]->GetWindowRect(&actRC);

	for (int i = 0; i < m_layersAct.size(); ++i)
	{
		m_layersNo[i]->ShowWindow(SW_HIDE);
		m_layersAct[i]->ShowWindow(SW_HIDE);
		m_layersAct[i]->SetCurSel(0);
		m_layersNum[i]->ShowWindow(SW_HIDE);
		SetText(m_layersNum[i],"1");
	}
	for (int i = 0; i < m_nLayer; ++i)
	{
		m_layersNo[i]->ShowWindow(SW_SHOW);
		m_layersAct[i]->ShowWindow(SW_SHOW);
		m_layersNum[i]->ShowWindow(SW_SHOW);
		m_layersNo[i]->SetWindowPos(nullptr, groupRC.left, groupRC.top + i * interval, 20, 30, 0);
		m_layersAct[i]->SetWindowPos(nullptr, groupRC.left+25, groupRC.top + i * interval, 90, actRC.bottom - actRC.top, 0);
		m_layersNum[i]->SetWindowPos(nullptr, groupRC.left+130, groupRC.top + i * interval, 40, 30, 0);
	}

	m_cNLayer.SetWindowTextW(std::to_wstring(m_nLayer).c_str());
	UpdateData();
}

void CDLVisualizerDlg::OnBnClickedButtonStart()
{
	auto btn = (CButton*)GetDlgItem(IDC_BUTTON_START);

	switch (m_step)
	{
	case OperatingStep::Pause:


		m_step = OperatingStep::Work;


		m_dl->EnableTraining(true);
		break;
	case OperatingStep::Stop:
	{
		if (m_nLayer == 0)
		{
			MessageBox(L"Generate layer!", L"Warning");
			return;
		}
		if (!m_ip)
		{
			MessageBox(L"Select input", L"Warning");
			return;
		}

		auto selInput = GetText(&m_cInput);

		std::vector<std::pair<ML::ActKind, int>> layers;
		for (int i = 0; i < m_nLayer; ++i)
		{
			auto strAct=GetText(m_layersAct[i]);
			ML::ActKind act;
			if (strAct == "Linear")
				act = ML::ActKind::Linear;
			else if (strAct == "Sigmoid")
				act = ML::ActKind::Logistic;
			else if (strAct == "Relu")
				act = ML::ActKind::Relu;
			else if (strAct == "Softmax")
				act = ML::ActKind::Softmax;

			int num = atoi(GetText(m_layersNum[i]).c_str());
			layers.push_back({ act ,num });
		}

		if (layers.back().second != m_ip->m_nLabel)
		{
			MessageBox(L"Output layer", L"Layer violation");
			return;
		}

		m_cbWeights.ResetContent();
		for (int i = 0; i < m_nLayer; ++i)
		{
			m_cbWeights.AddString(std::to_wstring(i+1).c_str());
		}
		m_cbWeights.SetCurSel(0);

		auto strLoss = GetText(m_loss);
		ML::LossKind loss;
		if (strLoss == "Mean Squared")
			loss = ML::LossKind::MeanSqure;
		else if (strLoss == "Binary cross entropy")
			loss = ML::LossKind::BinaryCrossEntropy;
		else if (strLoss == "Category cross entropy")
			loss = ML::LossKind::CategoryCrossEntropy;


		m_step = OperatingStep::Work;


		int epoch = atoi(GetText(&m_cEpoch).c_str());
		double alpha = pow(10, atoi(GetText(&m_cAlpha).c_str()));
		m_dl->Initialize(
			layers,
			m_ip,
			loss,
			alpha,
			epoch);

		m_dl->EnableTraining(true);
	}
		break;
	case OperatingStep::Work:
		m_step = OperatingStep::Pause;

		m_dl->EnableTraining(false);


		break;
	default:
		break;
	}
}


void CDLVisualizerDlg::OnBnClickedButtonStop()
{
	m_step = OperatingStep::Stop;

	m_hist.clear();
	if (m_2loss);
		m_2loss->ClearPlot();

	if(m_dl)
	m_dl->EnableTraining(false);
}


void CDLVisualizerDlg::INPUT_SELCHANGE()
{
	if (m_ip)
		delete m_ip;
	m_ip = nullptr;

	auto selInput = GetText(&m_cInput);
	if (selInput == "Simple")
	{
		m_ip = new IP_Simple({ 0,0,0 }, { 5,5,5 }, 200,2);
	}
	else if (selInput=="Radius")
	{
		m_ip = new IP_Radius({ 0,0,0 }, { 5,5,5 }, 400);
	}
	else if (selInput == "Block")
	{
		m_ip = new IP_Uniform({ 0,0,0 }, { 5,5,5 }, 8,8);
	}

	if (m_ip)
	{
		auto x = m_ip->GetData();
		auto y = m_ip->GetLabel();
		std::vector<DirectX::XMFLOAT3> pts;
		std::vector<float> rads;
		std::vector<DirectX::XMFLOAT4> cols;

		for (int i = 0; i < x.size(); ++i)
		{
			pts.push_back({ (float)x[i][0], (float)x[i][1], (float)x[i][2] });
			rads.push_back(PLOT_SCATTER_RAD);
			cols.push_back(m_ip->GetColor(y[i]));
		}

		m_3Input->ClearScatters();
		m_3Input->Scatter(pts, rads, cols);


		auto origin = m_ip->GetMinPt();
		auto size = m_ip->GetMaxPt();
		size.x -= origin.x;
		size.y -= origin.y;
		size.z -= origin.z;
		float res = 7;
		m_outputX.clear();
		rads.clear();
		cols.clear();
		pts.clear();
		if (true)
		{
			for (int i = 0; i < x.size(); ++i)
			{
				DirectX::XMFLOAT3 pt = { (float)x[i][0], (float)x[i][1], (float)x[i][2] };
				pts.push_back(pt);
				m_outputX.push_back({ pt.x,pt.y,pt.z });
				rads.push_back(PLOT_SCATTER_RAD);
				cols.push_back({ 0,0,0,1 });
			}
		}
		else
		{
			for (int z = 0; z < res; ++z)
			{
				for (int y = 0; y < res; ++y)
				{
					for (int x = 0; x < res; ++x)
					{
						DirectX::XMFLOAT3 pt = origin;
						pt.x += size.x * (x / res);
						pt.y += size.y * (y / res);
						pt.z += size.z * (z / res);
						pts.push_back(pt);
						m_outputX.push_back({ pt.x,pt.y,pt.z });
						rads.push_back(PLOT_SCATTER_RAD);
						cols.push_back({ 0,0,0,1 });
					}
				}
			}
		}


		m_3Output->ClearScatters();
		m_outputObjs = m_3Output->Scatter(pts, rads, cols);
	}
}


BOOL CDLVisualizerDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{

	if (m_curFocus)
	{
		m_curFocus->MouseWheel(zDelta);
	}


	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CDLVisualizerDlg::CBWEIGHTS_SELCHANGE()
{
	int iLayer = atoi(GetText(&m_cbWeights).c_str());
	if (iLayer >= 0)
	{
		m_lbWeights.ResetContent();

		ML::Matrix w;
		ML::Vector b;
		m_dl->GetWeights(m_cbWeights.GetCurSel(), w, b);

		int width = 0;
		m_lbWeights.AddString(L"Weight");
		for (int i = 0; i < w.size(); ++i)
		{
			auto strW = ML::ToString(w[i]);
			m_lbWeights.AddString(std::wstring(strW.begin(), strW.end()).c_str());
			width = max(width, strW.size() * 10);
		}

		auto strB = ML::ToString(b);
		m_lbWeights.AddString(L"Bias");
		m_lbWeights.AddString(std::wstring(strB.begin(), strB.end()).c_str());
		width = max(width, strB.size() * 10);

		m_lbWeights.SetHorizontalExtent(width);

	}
}
