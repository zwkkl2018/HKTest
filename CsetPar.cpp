// CsetPar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SoperCheck.h"
#include "CsetPar.h"
#include "afxdialogex.h"

CsetPar* pCsetPar = NULL;
// CsetPar �Ի���

IMPLEMENT_DYNAMIC(CsetPar, CDialogEx)

CsetPar::CsetPar(CWnd* pParent /*=NULL*/)
: CDialogEx(CsetPar::IDD, pParent)
{
	m_pcurrentCamID = -1;
	m_iSClose = true;
}

CsetPar::~CsetPar()
{
}

void CsetPar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SETPARA, m_TabCtrl);
	DDX_Control(pDX, IDC_LIST_SetPara, infolist_SetPara);
}


BEGIN_MESSAGE_MAP(CsetPar, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETPARA, &CsetPar::OnTcnSelchangeTabSetpara)
	//	ON_WM_CLOSE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_RETURN, &CsetPar::OnBnClickedBtnReturn)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CsetPar::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_RADIO_SOFTWARE, &CsetPar::OnBnClickedRadioSoftware)
	ON_BN_CLICKED(IDC_RADIO_HARDWARE, &CsetPar::OnBnClickedRadioHardware)
END_MESSAGE_MAP()


// CsetPar ��Ϣ�������


void CsetPar::OnTcnSelchangeTabSetpara(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	for (int i = 0; i < m_CameraConfig->m_MySEQ.size(); i++)
	{
		if (i == m_TabCtrl.GetCurSel())
		{
			p_CsetParSUB[i]->ShowWindow(SW_SHOW);
			m_pcurrentCamID = p_CsetParSUB[i]->m_CamInfoindex;
		}
		else
		{
			p_CsetParSUB[i]->ShowWindow(SW_HIDE);
		}

		m_paraChangeArr[i] = false;
	}

	*pResult = 0;
}


BOOL CsetPar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ�� 
	CString str;
	/*for (int i = 0; i < m_CameraConfig->m_MySEQ.size(); i++)
	{
	str.Format(_T("���%d����"), i + 1);
	m_TabCtrl.InsertItem(i, ((LPCTSTR)str));
	}*/

	for (int i = 0; i < m_CameraConfig->m_MySEQ.size(); i++)
	{
		CsetParSUB*	pS = new  CsetParSUB;

		if (m_CameraConfig->m_MySEQ[i]->m_camInfo == "CAM_TOP")			{ pS->m_CamInfoindex = CAM_TOP; str.Format(_T("�����������")); }
		if (m_CameraConfig->m_MySEQ[i]->m_camInfo == "CAM_LONGSIDE")	{ pS->m_CamInfoindex = CAM_LONGSIDE; str.Format(_T("�����������")); }
		if (m_CameraConfig->m_MySEQ[i]->m_camInfo == "CAM_SHORTSIDE_0")	{ pS->m_CamInfoindex = CAM_SHORTSIDE_0; str.Format(_T("�˷�0�������")); }
		if (m_CameraConfig->m_MySEQ[i]->m_camInfo == "CAM_SHORTSIDE_1")	{ pS->m_CamInfoindex = CAM_SHORTSIDE_1; str.Format(_T("�˷�1�������")); }


		m_TabCtrl.InsertItem(i, ((LPCTSTR)str));


		pS->p_MyCamPara = m_CameraConfig->m_MySEQ[i];
		pS->m_onMessage = m_onMessage2Sub;

		pS->Create(CsetParSUB::IDD, &m_TabCtrl);

		pS->p_MapParaKey = m_MapParaKey;
		m_pcurrentCamID = pS->m_CamInfoindex;

		pS->getDeFectPara();

		pS->m_paraChange = &m_paraChangeArr[i];
		//m_paraChangeArr[i] = false;
		p_CsetParSUB.push_back(pS);
	}

	//�趨��Tab����ʾ�ķ�Χ  
	CRect tabRect;
	m_TabCtrl.GetClientRect(tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 22;
	tabRect.bottom -= 1;

	for (int i = 0; i < p_CsetParSUB.size(); i++)
	{
		p_CsetParSUB[i]->MoveWindow(&tabRect);
		if (i == 0)
		{
			m_pcurrentCamID = 0;
			p_CsetParSUB[i]->ShowWindow(SW_SHOW);
		}
		else
		{
			p_CsetParSUB[i]->ShowWindow(SW_HIDE);

		}
	}
	pCsetPar = this;
	bool r = true;
	for (int i = 0; i < m_CameraConfig->m_MySEQ.size(); i++)
	{
		r &= m_CameraConfig->m_MySEQ[i]->m_trigger;
	}
	if (r)
	{
		CButton* radio = (CButton*)GetDlgItem(IDC_RADIO_HARDWARE);
		radio->SetCheck(1);
		OnBnClickedRadioHardware();
	}
	else
	{
		CButton* radio = (CButton*)GetDlgItem(IDC_RADIO_SOFTWARE);
		radio->SetCheck(1);
		OnBnClickedRadioSoftware();
	}


	return TRUE;

}
void CsetPar::OnlcMessage(void* owner, MessageType msgType, void* data, int camIndex)
{

	int camId = pCsetPar->m_pcurrentCamID;
	void* mm = (void*)data;
	imgInfo2Dlg* iminfo = (imgInfo2Dlg*)mm;
	cv::Mat rm;
	if (iminfo->depth == 1)
	{
		rm.create(iminfo->hight, iminfo->width, CV_8UC1);
	}
	else
	{
		rm.create(iminfo->hight, iminfo->width, CV_8UC3);
	}

	rm.data = (uchar*)iminfo->data;

	//cv::imwrite("110.jpg",rm);
	bool rmisEmpty = rm.empty();
	if (rmisEmpty)
	{
		iminfo->setparaInfo->push_back("imageData is empty");
	}
	pCsetPar->show_info_SetParaMessage(data);

	if (rmisEmpty) return;
	if (camId == camIndex && !pCsetPar->m_iSClose)
		//	if (camId == iminfo->camIndex&& !pCsetPar->m_iSClose)
	{
		//cv::imwrite("111.jpg", rm);
		pCsetPar->ShowVideoFrame(pCsetPar->GetDlgItem(IDC_STATIC_PICTURE_SUB)->m_hWnd, rm);
	}

}

void CsetPar::show_info_SetParaMessage(void* data)
{
	int reSetCnt = 100;
	void* mm = (void*)data;
	imgInfo2Dlg* iminfo = (imgInfo2Dlg*)mm;
	vector<string>   *pInfo = iminfo->setparaInfo;
	if (pInfo == NULL)	return;
	CString cS;
	for (int i = 0; i < pInfo->size(); i++)
	{
		string  sinfo = pInfo->at(i);
		cS = sinfo.c_str();
		infolist_SetPara.InsertString(i, cS);
		if (infolist_SetPara.GetCount() > reSetCnt)
		{
			infolist_SetPara.ResetContent();
		}
	}
}

void CsetPar::ShowVideoFrame(HWND wnd, Mat& image)
{
	//cv::imwrite("112.jpg", image);
	//cv::waitKey(10);
	if (wnd == NULL) return;
	static BITMAPINFO *bitMapinfo = NULL;
	static BYTE *bitBuffer;
	static bool first = true;
	if (first)
	{
		bitBuffer = new BYTE[40 + 4 * 256];
		first = false;
		memset(bitBuffer, 0, 40 + 4 * 256);
		bitMapinfo = (BITMAPINFO*)bitBuffer;
		bitMapinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitMapinfo->bmiHeader.biPlanes = 1;
		for (int i = 0; i < 256; i++)
		{
			bitMapinfo->bmiColors[i].rgbBlue = bitMapinfo->bmiColors[i].rgbGreen = bitMapinfo->bmiColors[i].rgbRed = (BYTE)i;
		}

	}
	bitMapinfo->bmiHeader.biHeight = -image.rows;
	bitMapinfo->bmiHeader.biWidth = image.cols;
	bitMapinfo->bmiHeader.biBitCount = image.channels() * 8;
	bitMapinfo->bmiHeader.biCompression = 0;
	bitMapinfo->bmiHeader.biSizeImage = 0;
	bitMapinfo->bmiHeader.biXPelsPerMeter = 0;
	bitMapinfo->bmiHeader.biYPelsPerMeter = 0;
	bitMapinfo->bmiHeader.biClrUsed = 0;
	bitMapinfo->bmiHeader.biClrImportant = 0;


	CRect rect;//���������
	::GetClientRect(wnd, &rect); //���pictrue�ؼ����ڵľ�������
	if (!rect.Width())
		return;
	HDC hDC = ::GetDC(wnd);
	SetStretchBltMode(hDC, HALFTONE/*COLORONCOLOR*/);
	StretchDIBits(hDC, 0, 0, rect.right, rect.bottom, 0, 0, image.cols, image.rows, image.data, bitMapinfo, DIB_RGB_COLORS, SRCCOPY);
	::ReleaseDC(wnd, hDC);
}

void CsetPar::gotoSetpar()
{
	m_onMessage2Sub(this, MSG_SetPar, NULL, NULL);
}

void CsetPar::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	bool paraCflg = false;
	for (int i = 0; i < m_CameraConfig->m_MySEQ.size(); i++)
	{
		paraCflg |= m_paraChangeArr[i];
	}

	if (paraCflg)  //�����޸ı�ʾ
	{
		UINT nRet = MessageBox(TEXT("�Ƿ񱣴�������ã�"), TEXT("��ʾ��Ϣ"), MB_OKCANCEL | MB_ICONERROR);
		if (nRet == IDOK)
		{
			m_CameraConfig->Save();
		}
		else
		{
			m_CameraConfig->Load();
			m_onMessage2Sub(this, MSG_ReSetAllGainExp, NULL, NULL);
#if 0
			for (int i = 0; i < m_CameraConfig->m_MySEQ.size(); i++)
			{
				int keyCamindex = 0;
				if (m_CameraConfig->m_MySEQ[i]->m_camInfo == "CAM_TOP")keyCamindex = 0;
				if (m_CameraConfig->m_MySEQ[i]->m_camInfo == "CAM_LONGSIDE")keyCamindex = 1;
				if (m_CameraConfig->m_MySEQ[i]->m_camInfo == "CAM_SHORTSIDE_0")keyCamindex =2;
				if (m_CameraConfig->m_MySEQ[i]->m_camInfo == "CAM_SHORTSIDE_1")keyCamindex = 3;


				ParaKey parkeyGain(keyCamindex, "�������", "gain");
				auto m = m_MapParaKey->find(parkeyGain);
				if (m != m_MapParaKey->end())
				{
					sub2Fun _sub2Fun;
					_sub2Fun.fparent = "�������";
					_sub2Fun.fpara = "gain";
					_sub2Fun.val = m->second;
					_sub2Fun.cam_Index = keyCamindex;

					m_onMessage2Sub(this, MSG_NULL, (void*)&_sub2Fun, keyCamindex);
				}
				Sleep(10);
				ParaKey parkeyExp(keyCamindex, "�������", "exp");
				m = m_MapParaKey->find(parkeyExp);
				if (m != m_MapParaKey->end())
				{
					sub2Fun _sub2Fun;
					_sub2Fun.fparent = "�������";
					_sub2Fun.fpara = "exp";// "exp";
					_sub2Fun.val = m->second;
					_sub2Fun.cam_Index = keyCamindex;

					m_onMessage2Sub(this, MSG_NULL, (void*)&_sub2Fun, keyCamindex);

				}
			}
#endif

		}
	}



	ShowWindow(SW_HIDE);
	m_onMessage2Sub(this, MSG_SubClose, NULL, NULL);

	m_iSClose = true;
	for (int i = 0; i < m_CameraConfig->m_MySEQ.size(); i++)
	{
		m_paraChangeArr[i] = false;
	}
}


void CsetPar::OnBnClickedBtnReturn()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	OnClose();
}


void CsetPar::OnBnClickedBtnSave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_CameraConfig->Save();
}


void CsetPar::OnBnClickedRadioSoftware()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ���������� 
	m_onMessage2Sub(this, MSG_SoftwareTrigger, NULL, NULL);
	for (int i = 0; i < m_CameraConfig->m_MySEQ.size(); i++)
	{
		m_CameraConfig->m_MySEQ[i]->m_trigger = 0;
	}
}


void CsetPar::OnBnClickedRadioHardware()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_onMessage2Sub(this, MSG_HardwareTrigger, NULL, NULL);
	for (int i = 0; i < m_CameraConfig->m_MySEQ.size(); i++)
	{
		m_CameraConfig->m_MySEQ[i]->m_trigger = 1;
	}
}
