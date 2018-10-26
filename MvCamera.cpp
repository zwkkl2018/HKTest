#include "stdafx.h"
#include "MvCamera.h"
#include <stdio.h>
#include <windows.h>
#include <process.h> 
#include <string.h>
#pragma warning(disable:4996)
#include "global.h"
//#include <opencv2\opencv.hpp>
using namespace cv;
void __stdcall CMvCamera::ImageCallBack(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser)
{
	CMvCamera* mvs = (CMvCamera*)pUser;
	if (pFrameInfo&&mvs)
	{
		if (pData)
		{
			GetLocalTime(mvs->m_psysTime);
			memmove(mvs->m_frameBuf, pData, pFrameInfo->nFrameLen);
			SetEvent(mvs->m_frameEvent);
		}
	}
}

CMvCamera::CMvCamera()
{

}

CMvCamera::~CMvCamera()
{

}


//void CMvCamera::InitialMvDevice()
bool CMvCamera::InitialMvDevice(const char* pcSerNo, char* seqBuf)
{

	int nRet = MV_OK;
	memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));
	// ch:枚举子网内所有设备 | en:Enumerate all devices within subnet
	nRet = HKCamera::EnumDevices(&m_stDevList);
	if (MV_OK != nRet)
	{
		return false;
	}

	if (0 == m_stDevList.nDeviceNum)
	{
		AfxMessageBox(TEXT("no camera found!\n"));
		return false;
	}


	for (int i = 0; i < m_stDevList.nDeviceNum; i++)
	{

		char *m_currSerialNumber;
		m_currSerialNumber = (char*)m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.chSerialNumber;
		if (strcmp(m_currSerialNumber, pcSerNo) == 0)
		{
			m_pCamtem = new HKCamera;
			int nRet = m_pCamtem->Open(m_stDevList.pDeviceInfo[i]);
			if (MV_OK != nRet)
			{
				delete m_pCamtem;
				return nRet;
			}

			// ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
			if (m_stDevList.pDeviceInfo[i]->nTLayerType == MV_GIGE_DEVICE)
			{
				int nPacketSize = m_pCamtem->GetOptimalPacketSize();
				if (nPacketSize > 0)
				{
					nRet = m_pCamtem->SetIntValue("GevSCPSPacketSize", nPacketSize);
					if (nRet != MV_OK)
					{
						AfxMessageBox(TEXT("Warning: Set Packet Size fail!"), nRet);
					}
				}
				else
				{
					AfxMessageBox(TEXT("Warning: Get Packet Size fail!"), nPacketSize);
				}
			}

			nRet |= m_pCamtem->SetEnumValue("LineSelector", 1);// 输出为1);
			nRet |= m_pCamtem->SetBoolValue("LineInverter", 0);//低电平
			nRet |= m_pCamtem->SetEnumValue("TriggerMode", MV_TRIGGER_MODE_ON);
			if (MV_OK != nRet)
			{

				AfxMessageBox(TEXT("Set Trigger Mode Fail"), nRet);
				return nRet;
			}


			nRet = m_pCamtem->RegisterImageCallBack(ImageCallBack, this);
			nRet = m_pCamtem->StartGrabbing();
			//nRet = m_pCamtem->setImgWH(672, 512);
			nRet = m_pCamtem->ImgWH(m_nWidth, m_nHeight);
			m_nBitDepth = 24;
			return true;
		}

	}
	return false;

}
bool CMvCamera::SetoutSigle()
{

	int nRet = m_pCamtem->SetBoolValue("LineInverter", 1);// MV_TRIGGER_SOURCE_SOFTWARE);
	Sleep(10);
	nRet |= m_pCamtem->SetBoolValue("LineInverter", 0);// MV_TRIGGER_SOURCE_SOFTWARE);
	if (MV_OK != nRet)
	{

		AfxMessageBox(TEXT("Set Trigger Mode Fail"), nRet);
		return nRet;
	}
	return true;
}
bool CMvCamera::DoSoftWare(void *nhandle)
{


	int nRet = m_pCamtem->CommandExecute("TriggerSoftware");


	if (MV_OK != nRet)
	{
		//	AfxMessageBox(TEXT("Set TriggerSoftware failed[%x]!\n"), nRet);
		return false;
	}

	return true;
}
bool CMvCamera::SetTriggerMode(void * nhandle, int nTriggerMode)
{


	if (nTriggerMode == 1)///软件触发
	{


		int nRet = m_pCamtem->SetEnumValue("TriggerSource", MV_TRIGGER_SOURCE_SOFTWARE);

		if (MV_OK != nRet)
		{
			AfxMessageBox(TEXT("Set TriggerSource failed[%x]!\n"), nRet);
			return false;
		}


	}
	else if (nTriggerMode == 0)///硬件触发
	{
		int nRet = m_pCamtem->SetEnumValue("TriggerSource", MV_TRIGGER_SOURCE_LINE0);
		if (MV_OK != nRet)
		{
			//AfxMessageBox(TEXT("Set TriggerSource failed[%x]!\n"), nRet);
			return 0;
		}
	}
	return 1;
}
bool CMvCamera::SetExposureTime(void * nhandle, int nExposureTime)
{

	int nRet = m_pCamtem->SetExposureTime(nExposureTime);
	if (MV_OK == nRet)
		return true;
	return false;

}
bool CMvCamera::SetGain(void * nhandle, int nGain)
{

	int nRet = m_pCamtem->SetGain(nGain);
	if (MV_OK == nRet)
		return true;
	return false;

}
bool CMvCamera::CloseDevice()
{
	//停止采集图像 
	int nRet = m_pCamtem->StopGrabbing();
	nRet = m_pCamtem->Close();
	if (MV_OK == nRet)
		return true;
	return false;

}

bool CMvCamera::SetWhiteBalance(void * nhandle, int nValue)
{
	int nRet = m_pCamtem->SetWB(nValue);
	if (MV_OK == nRet)
		return true;
	return false;
}

bool CMvCamera::SetRate(int rate)
{
	int nRet = m_pCamtem->SetFrameRate(rate);
	if (MV_OK == nRet)
		return true;
	return false;
}