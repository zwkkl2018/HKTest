#pragma once
#include<vector>
#include "CsetParSUB.h"
#include "afxcmn.h"
#include "CameraConfig.h"
#include "afxwin.h"
using namespace std;
// CsetPar 对话框

class CsetPar : public CDialogEx
{
	DECLARE_DYNAMIC(CsetPar)

public:
	CsetPar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CsetPar();

	// 对话框数据
	enum { IDD = IDD_DIALOG_PARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTabSetpara(NMHDR *pNMHDR, LRESULT *pResult);
	static void OnlcMessage(void* owner, MessageType msgType, void* data, int camIndex);
	void show_info_SetParaMessage( void* data);
	void ShowVideoFrame(HWND wnd, Mat& image);
	void gotoSetpar();
	int  m_pcurrentCamID;
public:
	CTabCtrl m_TabCtrl;
	vector<CsetParSUB*> p_CsetParSUB;

	virtual BOOL  OnInitDialog();

	bool m_paraChangeArr[maxCamcnt];

	CameraConfig* m_CameraConfig;
	map<ParaKey, int>*	 m_MapParaKey;


	ONMessage      m_onMessage2Sub; //message
//	afx_msg void OnClose();
	afx_msg void OnClose();
	bool    m_iSClose;
	afx_msg void OnBnClickedBtnReturn();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedRadioSoftware();
	afx_msg void OnBnClickedRadioHardware();
	CListBox infolist_SetPara;
};
