#pragma once
#include <vector>
#include "HKCamera.h"

#include"MvCameraControl.h"
#include"TlFactory.h"
using namespace MvCamCtrl;

#define G_DEFIMGWIDTH    672//1280//1296//656
#define G_DEFIMGHEIGHT   512//960//966//494
#define CAMERA_NUM           5

typedef struct CAMAIMAGEINFORMATION
{
	BYTE * m_imagebuf;   //图象数据
	int   width;        //图象的宽
	int   height;        //图象的高
	int   widthStep;
	int   nChannel;   //图像数据长度
}TCAMAIMAGEINFORMATION;


class CMvCamera
{
public:
	CMvCamera();
	~CMvCamera();
public:


	void*		m_handle;
public:
	char 		m_CamSero[20];
	
	int			m_camIndex;

	UINT			m_nWidth;
	UINT			m_nHeight;
	int			m_nBitDepth;

	//char*		m_seqImgBuf;
	char*		m_frameBuf;
	//char*	    m_txtPrebuffer;
public:
	HANDLE		m_frameEvent;
	SYSTEMTIME* m_psysTime;

public:
	//void InitialMvDevice(char* camSero,char* seqBuf);	      
    static void __stdcall ImageCallBack(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);
	bool InitialMvDevice(const char* pcSerNo, char* seqBuf);
	bool CloseDevice();
	bool SetTriggerMode(void * nhandle, int nTriggerMode);
	bool DoSoftWare(void *nhandle);
	bool SetExposureTime(void * nhandle, int nExposureTime);
	bool SetGain(void * nhandle, int nGain);
	bool SetWhiteBalance(void * nhandle, int nValue);

	bool SetRate(int rate);
	bool SetoutSigle();


	HKCamera*     m_pCamtem;
	MV_CC_DEVICE_INFO_LIST m_stDevList;
};

