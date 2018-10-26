// CsetParSUB.cpp : 实现文件
//

#include "stdafx.h"
#include "SoperCheck.h"
#include "CsetParSUB.h"
#include "afxdialogex.h"

CsetParSUB *pCsetParSUB;

// CsetParSUB 对话框

IMPLEMENT_DYNAMIC(CsetParSUB, CDialogEx)

CsetParSUB::CsetParSUB(CWnd* pParent /*=NULL*/)
: CDialogEx(CsetParSUB::IDD, pParent)
{
	m_font.CreateFont(22, 0, 0, 0, FW_HEAVY, 0, 0, 0,
		DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("微软雅黑"));
		
	m_pSliderMAXCnt = 0;
	//m_CamIdex = -1;
}

CsetParSUB::~CsetParSUB()
{
}

void CsetParSUB::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtl);
}


BEGIN_MESSAGE_MAP(CsetParSUB, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CsetParSUB::OnBnClickedButton1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CsetParSUB::OnTvnSelchangedTree1)
	//ON_MESSAGE(WM_MYMSG_RegetParaV, &CsetParSUB::OnTvnSelchangedTree1)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CsetParSUB 消息处理程序


void CsetParSUB::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码

	CString str;
	str.Format(_T("测试[%d]"), m_CamInfoindex);
		AfxMessageBox(str);
		return;
	//CWnd * pCWnd;
	//HWND Handle;
	//Handle = pCWnd->GetSafeHwnd();

	//pCWnd = FromHandle(Handle);

//
//	// hWnd;
//	CWnd* pchWnd = GetActiveWindow();// 
//	HWND hWnd = pchWnd->GetSafeHwnd();
//	hWnd = GetDlgItem(IDD_DIALOG_PARASUB)->GetSafeHwnd();
//	hWnd = pCsetParSUB->m_hWnd;
//	//	hWnd = pchWnd->GetSafeHandle();
//
//
//	//int m = PostMessageW(TVN_SELCHANGED, NULL, NULL);
//
//	//PostMessage(hWnd, TVN_SELCHANGED, NULL, NULL);
//	////SendMessage(m_hWnd, DM_SETDEFID, nID, 0);
//	//SendMessage(hWnd, TVN_SELCHANGED, NULL, NULL);
//
//	//int ret = SendMessageW(TVN_SELCHANGED); 
//	//int ret = ::SendMessageW(hWnd, TVN_SELCHANGED, NULL, NULL);
//	//int k = GetLastError();
//	//int u = 99;
//
//	NMTREEVIEW tv;
//	memset((void*)&tv, 0, sizeof(tv));
//	tv.hdr.hwndFrom = m_TreeCtl.m_hWnd;
//	tv.hdr.idFrom = ::GetClassLong(m_TreeCtl.m_hWnd, GWL_ID);
//	tv.hdr.code = TVN_SELCHANGED;
//	tv.action = TVN_SELCHANGED;
//
////HANDLE ffhandle=	m_TreeCtl.GetParentItem()
//	//::SendMessage(m_TreeCtl.m_hWnd, WM_NOTIFY, (WPARAM)tv.hdr.idFrom, (LPARAM)&tv);
//	::SendMessage(hWnd, WM_NOTIFY, (WPARAM)tv.hdr.idFrom, (LPARAM)&tv);
//
//	
//	int k = GetLastError();
//	int u = 99;
}


BOOL CsetParSUB::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_TreeCtl.SetFont(&m_font);
	m_TreeCtl.SetLineColor(RGB(0, 150, 220));// 设置TreeCtrl线条颜色  	

	m_TreeCtl.SetTextColor(RGB(40, 101, 181));//设置tree的背景色
	m_TreeCtl.SetBkColor(RGB(240, 240, 240));//设置tree的背景色


	MySEQ* pC = p_MyCamPara;
	HTREEITEM rootItem = NULL;
	for (int i = 0; i < pC->m_DeFectParaStrArr.size(); i++)
	{
		string defectName = pC->m_DeFectParaStrArr[i].defectName;
		rootItem = m_TreeCtl.InsertItem(C2W(defectName.c_str()), 0, 0, NULL);

	}
	//getDeFectPara();
	//rootItem = m_TreeCtl.InsertItem(TEXT("相机参数"), 0, 0, NULL);
	//m_TreeCtl.SelectItem(rootItem);
	pCsetParSUB = this;
	return TRUE;  // return TRUE unless you set the focus to a control

}

void  CsetParSUB::getDeFectPara()
{

	vector<DeFectPara>*  pDeFectPara = (vector<DeFectPara>*)(void*)&p_MyCamPara->m_DeFectParaStrArr;



	for (int i = 0; i < pDeFectPara->size(); i++)
	{
		//int i = m_CamIdex;
		DeFectPara* _DeFectPara = &p_MyCamPara->m_DeFectParaStrArr[i];


		oneDeFectPara _oneDeFectPara;


		for (int oneindex = 0; oneindex < _DeFectPara->defectParaCnt; oneindex++)
		{

			_oneDeFectPara.camIndex = m_CamInfoindex;
			_oneDeFectPara.paraIndex = i;
			_oneDeFectPara.parentName = _DeFectPara->defectName;
			string sTmp = _DeFectPara->defectParaArrStr[oneindex];

			int begain = sTmp.find("'");

			int pos1 = sTmp.find(",");
			_oneDeFectPara.name = sTmp.substr(begain + 1, pos1 - begain - 1);

			int pos2 = sTmp.find(",", pos1 + 1);
			_oneDeFectPara.paraMax = atoi(sTmp.substr(pos1 + 1, pos2 - pos1 - 1).c_str());

			int end = sTmp.find("'", pos2 + 1);

			_oneDeFectPara.paraVal = atoi(sTmp.substr(pos2 + 1, end - pos2 - 1).c_str());


			if (p_MapParaKey != NULL)
			{
				ParaKey parkey(m_CamInfoindex, _oneDeFectPara.parentName, _oneDeFectPara.name);
				auto m = p_MapParaKey->find(parkey);
				if (m == p_MapParaKey->end())
				{
					p_MapParaKey->insert(make_pair(parkey, _oneDeFectPara.paraVal));
				}
				else
				{
					m->second = _oneDeFectPara.paraVal;
				}
			}


			m_oneDeFectPara.push_back(_oneDeFectPara);
		}

	}
	initCamExpGain();
}

void CsetParSUB::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	m_oneDeFectPara.clear();
	getDeFectPara();

	// TODO:  在此添加控件通知处理程序代码
	CRect tabRect;
	m_TreeCtl.GetClientRect(tabRect);

	CRect CRectq(tabRect);
	CRectq.top = tabRect.bottom;
	CRectq.left = tabRect.left + 80;
	CRectq.right = tabRect.right + 5;
	CRectq.bottom = CRectq.top + 60;

	CString strText; // 树节点的标签文本字符串   

	// 获取当前选中节点的句柄   
	HTREEITEM hItem = m_TreeCtl.GetSelectedItem();
	// 获取选中节点的标签文本字符串   
	strText = m_TreeCtl.GetItemText(hItem);
	string strNm = CW2A(strText.GetString());

	m_sliderParentName = strNm;
	for (int i = 0; i < m_pSliderMAXCnt + 1; i++)
	{
		if (m_pSlider[i] != NULL){ m_pSlider[i]->ShowWindow(SW_HIDE);	delete m_pSlider[i];	m_pSlider[i] = NULL; }
		if (m_pSLabel[i] != NULL){ m_pSLabel[i]->ShowWindow(SW_HIDE);	delete m_pSLabel[i];	m_pSLabel[i] = NULL; }
		if (m_pSValue[i] != NULL){ m_pSValue[i]->ShowWindow(SW_HIDE);	delete m_pSValue[i];	m_pSValue[i] = NULL; }

	}

	int slidercnt = 0;
	char tmpC[50] = { 0 };
	for (int oneindex = 0; oneindex < m_oneDeFectPara.size(); oneindex++)
	{
		if (m_oneDeFectPara[oneindex].parentName == m_sliderParentName)
		{

			if (m_pSliderMAXCnt < slidercnt)m_pSliderMAXCnt = slidercnt;

			CRectq.top = CRectq.bottom + 20;
			CRectq.bottom = CRectq.top + 20;


			m_pSlider[slidercnt] = new CSliderCtrl;
			m_pSlider[slidercnt]->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_NOTICKS | TBS_BOTH,
				CRectq, this, IDC_SLIDER1_PARA1);

			m_pSlider[slidercnt]->ShowWindow(SW_SHOW);

			string deFn = m_oneDeFectPara[oneindex].name;
			int maxRange = m_oneDeFectPara[oneindex].paraMax;
			int pos = m_oneDeFectPara[oneindex].paraVal;
			itoa(pos, tmpC, 10);

			if (pos > maxRange)	pos = maxRange;

			m_pSlider[slidercnt]->SetRange(0, maxRange);
			m_pSlider[slidercnt]->SetPos(pos);


			m_pSLabel[slidercnt] = new CStatic;
			m_pSValue[slidercnt] = new CStatic;
			CRect CRectLable(CRectq), CRectLValue(CRectq);

			CRectLable.left = CRectq.left - 80;     CRectLable.right = CRectLable.left + 75;
			CRectLValue.left = CRectq.right;    CRectLValue.right = CRectLValue.left + 200;
			m_pSLabel[slidercnt]->Create(C2W(deFn.c_str()), WS_CHILD | WS_VISIBLE, CRectLable, this);
			m_pSValue[slidercnt]->Create(C2W(tmpC), WS_CHILD | WS_VISIBLE, CRectLValue, this);


			slidercnt++;
		}
	}

	// 将字符串显示到编辑框中   
	SetDlgItemText(IDC_STATIC, strText);
	//AfxMessageBox(strText);
	*pResult = 0;
}

wchar_t* CsetParSUB::C2W(const char* str)
{
	int len = MultiByteToWideChar(CP_OEMCP, 0, str, -1/*null terminated*/, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	MultiByteToWideChar(CP_OEMCP, 0, str, -1/*null terminated*/, wstr, len);
	return wstr;
}

char* CsetParSUB::W2C(const wchar_t *pwstr)
{
	int nlength = wcslen(pwstr);
	//获取转换后的长度  
	int nbytes = WideCharToMultiByte(0, 0, pwstr, nlength, NULL, 0, NULL, NULL);
	char* pcstr = new char[nbytes + 1];
	// 通过以上得到的结果，转换unicode 字符为ascii 字符  
	WideCharToMultiByte(0, 0, pwstr, nlength, pcstr, nbytes, NULL, NULL);
	pcstr[nbytes] = '\0';
	return pcstr;
}
bool CsetParSUB::getMapValue(ParaKey parkey, double& value)
{
	auto m = p_MapParaKey->find(parkey);
	if (m != p_MapParaKey->end())
	{
		value = m->second;
		return true;
	}
	value = 0;
	return false;
}
void CsetParSUB::initCamExpGain()
{
	sub2Fun _sub2Fun; 
	_sub2Fun.fparent = "相机参数";
	_sub2Fun.fpara = "exp";	 
	_sub2Fun.cam_Index = m_CamInfoindex;
	m_onMessage(this, MSG_SetOneGainExp, (void*)&_sub2Fun, m_CamInfoindex);
	_sub2Fun.fpara = "gain";
	m_onMessage(this, MSG_SetOneGainExp, (void*)&_sub2Fun, m_CamInfoindex);
	m_onMessage(this, MSG_SubsubClose, NULL, NULL);	 

	//double value = 0.0;
	//ParaKey parkeyExp(m_CamInfoindex, "相机参数", "exp");
	//getMapValue(parkeyExp, value);
	// 
	//ParaKey parkeyGain(m_CamInfoindex, "相机参数", "gain");
	//getMapValue(parkeyGain, value);
}
void CsetParSUB::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	char tmpC[20] = { 0 };
	CString strLable;
	string paraName;
	int  value = 0;
	for (int i = 0; i < m_pSliderMAXCnt + 1; i++)
	{
		if (m_pSlider[i] == NULL || m_pSValue[i] == NULL) continue;
		if (pScrollBar->m_hWnd == m_pSlider[i]->m_hWnd)
		{
			value = m_pSlider[i]->GetPos();
			itoa(value, tmpC, 10);
			m_pSValue[i]->SetWindowText(C2W(tmpC));
			m_pSLabel[i]->GetWindowText(strLable);
			paraName = CT2A(strLable.GetBuffer());
			break;
		}
	}
	paraMerge2Config(m_CamInfoindex, m_sliderParentName, paraName, value);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}



void  CsetParSUB::paraMerge2Config(int camInfoId, string sliderParentName, string paraName, int value)
{

	//make map better
	char valueC[20] = { 0 };
	itoa(value, valueC, 10);

	ParaKey parkey(camInfoId, sliderParentName, paraName);
	auto m = p_MapParaKey->find(parkey);
	if (m == p_MapParaKey->end())
	{
		p_MapParaKey->insert(make_pair(parkey, value));
	}
	else
	{
		m->second = value;
	}

	sub2Fun _sub2Fun;//没有将max传入
	_sub2Fun.fparent = sliderParentName;
	_sub2Fun.fpara = paraName;
	_sub2Fun.val = value;
	_sub2Fun.cam_Index = camInfoId;

	if (paraName == "gain" || paraName == "exp")
	{
		m_onMessage(this, MSG_SetOneGainExp, (void*)&_sub2Fun, m_CamInfoindex);
	}
	else
	{
		m_onMessage(this, MSG_NULL, (void*)&_sub2Fun, m_CamInfoindex);
	}


	*m_paraChange = true;
	for (int i = 0; i < p_MyCamPara->m_DeFectParaStrArr.size(); i++)
	{
		DeFectPara* _DeFectPara = &p_MyCamPara->m_DeFectParaStrArr[i];
		if (sliderParentName == _DeFectPara->defectName)
		{
			for (int j = 0; j < _DeFectPara->defectParaArrStr.size(); j++)
			{
				string sTmp = _DeFectPara->defectParaArrStr[j];
				int pos1 = sTmp.find(",", 1);
				string name = sTmp.substr(1, pos1 - 1);
				int pos2 = sTmp.find(",", pos1 + 1);
				string paraMax = sTmp.substr(pos1 + 1, pos2 - pos1 - 1);

				if (name == paraName)
				{
					string paraStr = "'" + name + "," + paraMax + "," + valueC + "'";
					_DeFectPara->defectParaArrStr[j] = paraStr;
					break;
				}

			}
		}

	}
}