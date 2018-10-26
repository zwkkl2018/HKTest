#pragma once
#include"CameraConfig.h"
#include"Ctrol.h"
#include "afxcmn.h"
#define IDC_SLIDER1_PARA1 10000
#define SLIDERCNT   20

 #define WM_MYMSG_RegetParaV TVN_SELCHANGED
// CsetParSUB 对话框
//typedef void(*ONMessage2Ctr)(void* owner, MessageType msgType, void* data, int camIndex);

class CsetParSUB : public CDialogEx
{
	DECLARE_DYNAMIC(CsetParSUB)

public:
	CsetParSUB(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CsetParSUB();

	// 对话框数据
	enum { IDD = IDD_DIALOG_PARASUB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


	wchar_t* C2W(const char* str);
	char*  W2C(const wchar_t *pwstr);
public:
	CFont					 m_font;
	int				    	 m_CamInfoindex;
	bool *                   m_paraChange;

	MySEQ*					 p_MyCamPara;
	map<ParaKey, int>*	     p_MapParaKey;
	map<string,  int>	     MapMSGFuction;
	//void                     MapMSGFuction();
	vector<oneDeFectPara>	 m_oneDeFectPara;
	void					 getDeFectPara();

	afx_msg void			 OnBnClickedButton1();
	virtual BOOL			 OnInitDialog();
	afx_msg void             OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);


	CTreeCtrl	   m_TreeCtl;
	string         m_sliderParentName;

	vector<CSliderCtrl*> m_ppSlider;

	CSliderCtrl   *m_pSlider[SLIDERCNT];
	CStatic       *m_pSValue[SLIDERCNT];
	CStatic       *m_pSLabel[SLIDERCNT];
	int           m_pSliderMAXCnt;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	void          paraMerge2Config(int camId,string sliderParentName,string paraName,int value);
	bool         getMapValue(ParaKey parkey, double& value);

	void         initCamExpGain();
public:
	ONMessage  m_onMessage; 
	void*      m_owner;
};
