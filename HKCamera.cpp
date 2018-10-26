#include "stdafx.h"
#include "HKCamera.h"
#include <string.h>

HKCamera::HKCamera()
{
	m_hDevHandle = NULL;

	m_mode = 0;
	m_frameRate = 0.0;
	m_exposuretime = 0.0;
	m_gain = 0.0;
}

HKCamera::~HKCamera()
{
	if (m_hDevHandle)
	{
		MV_CC_DestroyHandle(m_hDevHandle);
		m_hDevHandle = NULL;
	}
}

int HKCamera::EnumDevices(MV_CC_DEVICE_INFO_LIST* pstDevList)
{
	int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, pstDevList);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	return MV_OK;
}

// ch:打开设备 | en:Open Device
int     HKCamera::Open(MV_CC_DEVICE_INFO* pstDeviceInfo)
{
	if (NULL == pstDeviceInfo)
	{
		return MV_E_PARAMETER;
	}

	int nRet = MV_OK;
	if (m_hDevHandle == NULL)
	{
		nRet = MV_CC_CreateHandle(&m_hDevHandle, pstDeviceInfo);
		if (MV_OK != nRet)
		{
			return nRet;
		}
	}

	nRet = MV_CC_OpenDevice(m_hDevHandle);
	if (MV_OK != nRet)
	{
		MV_CC_DestroyHandle(m_hDevHandle);
		m_hDevHandle = NULL;

		return nRet;
	}

	return MV_OK;
}


// ch:关闭设备 | en:Close Device
int     HKCamera::Close()
{
	int nRet = MV_OK;

	if (NULL == m_hDevHandle)
	{
		return MV_E_PARAMETER;
	}

	MV_CC_CloseDevice(m_hDevHandle);
	nRet = MV_CC_DestroyHandle(m_hDevHandle);
	m_hDevHandle = NULL;

	return nRet;
}


// ch:开启抓图 | en:Start Grabbing
int     HKCamera::StartGrabbing()
{
	return MV_CC_StartGrabbing(m_hDevHandle);
}


// ch:停止抓图 | en:Stop Grabbing
int     HKCamera::StopGrabbing()
{
	return MV_CC_StopGrabbing(m_hDevHandle);
}

int     HKCamera::GetOneFrameTimeout(unsigned char* pData, unsigned int* pnDataLen, unsigned int nDataSize, MV_FRAME_OUT_INFO_EX* pFrameInfo, int nMsec)
{
	if (NULL == pnDataLen)
	{
		return MV_E_PARAMETER;
	}

	int nRet = MV_OK;

	*pnDataLen = 0;

	nRet = MV_CC_GetOneFrameTimeout(m_hDevHandle, pData, nDataSize, pFrameInfo, nMsec);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	*pnDataLen = pFrameInfo->nFrameLen;

	return nRet;
}


// ch:设置显示窗口句柄 | en:Set Display Window Handle
int     HKCamera::Display(void* hWnd)
{
	return MV_CC_Display(m_hDevHandle, hWnd);
}


int HKCamera::SaveImage(MV_SAVE_IMAGE_PARAM_EX* pstParam)
{
	if (NULL == pstParam)
	{
		return MV_E_PARAMETER;
	}

	return MV_CC_SaveImageEx2(m_hDevHandle, pstParam);
}

// ch:注册图像数据回调 | en:Register Image Data CallBack
int HKCamera::RegisterImageCallBack(void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo,
	void* pUser), void* pUser)
{
	return MV_CC_RegisterImageCallBackForBGR(m_hDevHandle, cbOutput, pUser);
	//return MV_CC_RegisterImageCallBackEx(m_hDevHandle, cbOutput, pUser);
}


// ch:注册消息异常回调 | en:Register Message Exception CallBack
int     HKCamera::RegisterExceptionCallBack(void(__stdcall* cbException)(unsigned int nMsgType, void* pUser), void* pUser)
{
	return MV_CC_RegisterExceptionCallBack(m_hDevHandle, cbException, pUser);
}


// ch:获取Int型参数，如 Width和Height，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
// en:Get Int type parameters, such as Width and Height, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     HKCamera::GetIntValue(IN const char* strKey, OUT unsigned int *pnValue)
{
	if (NULL == strKey || NULL == pnValue)
	{
		return MV_E_PARAMETER;
	}

	MVCC_INTVALUE stParam;
	memset(&stParam, 0, sizeof(MVCC_INTVALUE));
	int nRet = MV_CC_GetIntValue(m_hDevHandle, strKey, &stParam);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	*pnValue = stParam.nCurValue;

	return MV_OK;
}


// ch:设置Int型参数，如 Width和Height，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
// en:Set Int type parameters, such as Width and Height, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     HKCamera::SetIntValue(IN const char* strKey, IN unsigned int nValue)
{
	if (NULL == strKey)
	{
		return MV_E_PARAMETER;
	}

	return MV_CC_SetIntValue(m_hDevHandle, strKey, nValue);
}


// ch:获取Float型参数，如 ExposureTime和Gain，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
// en:Get Float type parameters, such as ExposureTime and Gain, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     HKCamera::GetFloatValue(IN const char* strKey, OUT float *pfValue)
{
	if (NULL == strKey || NULL == pfValue)
	{
		return MV_E_PARAMETER;
	}

	MVCC_FLOATVALUE stParam;
	memset(&stParam, 0, sizeof(MVCC_FLOATVALUE));
	int nRet = MV_CC_GetFloatValue(m_hDevHandle, strKey, &stParam);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	*pfValue = stParam.fCurValue;

	return MV_OK;
}


// ch:设置Float型参数，如 ExposureTime和Gain，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
// en:Set Float type parameters, such as ExposureTime and Gain, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     HKCamera::SetFloatValue(IN const char* strKey, IN float fValue)
{
	if (NULL == strKey)
	{
		return MV_E_PARAMETER;
	}

	return MV_CC_SetFloatValue(m_hDevHandle, strKey, fValue);
}


// ch:获取Enum型参数，如 PixelFormat，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
// en:Get Enum type parameters, such as PixelFormat, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     HKCamera::GetEnumValue(IN const char* strKey, OUT unsigned int *pnValue)
{
	if (NULL == strKey || NULL == pnValue)
	{
		return MV_E_PARAMETER;
	}

	MVCC_ENUMVALUE stParam;
	memset(&stParam, 0, sizeof(MVCC_ENUMVALUE));
	int nRet = MV_CC_GetEnumValue(m_hDevHandle, strKey, &stParam);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	*pnValue = stParam.nCurValue;

	return MV_OK;
}


// ch:设置Enum型参数，如 PixelFormat，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
// en:Set Enum type parameters, such as PixelFormat, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     HKCamera::SetEnumValue(IN const char* strKey, IN unsigned int nValue)
{
	if (NULL == strKey)
	{
		return MV_E_PARAMETER;
	}

	return MV_CC_SetEnumValue(m_hDevHandle, strKey, nValue);
}


// ch:获取Bool型参数，如 ReverseX，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
// en:Get Bool type parameters, such as ReverseX, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     HKCamera::GetBoolValue(IN const char* strKey, OUT bool *pbValue)
{
	if (NULL == strKey || NULL == pbValue)
	{
		return MV_E_PARAMETER;
	}

	return MV_CC_GetBoolValue(m_hDevHandle, strKey, pbValue);
}


// ch:设置Bool型参数，如 ReverseX，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
// en:Set Bool type parameters, such as ReverseX, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     HKCamera::SetBoolValue(IN const char* strKey, IN bool bValue)
{
	if (NULL == strKey)
	{
		return MV_E_PARAMETER;
	}

	return MV_CC_SetBoolValue(m_hDevHandle, strKey, bValue);
}


// ch:获取String型参数，如 DeviceUserID，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件UserSetSave
// en:Get String type parameters, such as DeviceUserID, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     HKCamera::GetStringValue(IN const char* strKey, IN OUT char* strValue, IN unsigned int nSize)
{
	if (NULL == strKey || NULL == strValue)
	{
		return MV_E_PARAMETER;
	}

	MVCC_STRINGVALUE stParam;
	memset(&stParam, 0, sizeof(MVCC_STRINGVALUE));
	int nRet = MV_CC_GetStringValue(m_hDevHandle, strKey, &stParam);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	strcpy_s(strValue, nSize, stParam.chCurValue);

	return MV_OK;
}


// ch:设置String型参数，如 DeviceUserID，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件UserSetSave
// en:Set String type parameters, such as DeviceUserID, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     HKCamera::SetStringValue(IN const char* strKey, IN const char* strValue)
{
	if (NULL == strKey)
	{
		return MV_E_PARAMETER;
	}

	return MV_CC_SetStringValue(m_hDevHandle, strKey, strValue);
}


// ch:执行一次Command型命令，如 UserSetSave，详细内容参考SDK安装目录下的 MvCameraNode.xlsx 文件
// en:Execute Command once, such as UserSetSave, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     HKCamera::CommandExecute(IN const char* strKey)
{
	if (NULL == strKey)
	{
		return MV_E_PARAMETER;
	}

	return MV_CC_SetCommandValue(m_hDevHandle, strKey);
}

int HKCamera::GetOptimalPacketSize()
{
	return MV_CC_GetOptimalPacketSize(m_hDevHandle);
}

int     HKCamera::GetAllMatchInfo(OUT unsigned int *nLostFrame, OUT unsigned int *nFrameCount)
{
	MV_CC_DEVICE_INFO stDevInfo = { 0 };
	int nRet = MV_CC_GetDeviceInfo(m_hDevHandle, &stDevInfo); //该接口u3暂不支持，用此方式来判断u3还是GigE
	if (MV_E_SUPPORT == nRet)
	{
		stDevInfo.nTLayerType = MV_USB_DEVICE;
	}

	if (MV_GIGE_DEVICE == stDevInfo.nTLayerType)
	{
		MV_ALL_MATCH_INFO struMatchInfo = { 0 };
		MV_MATCH_INFO_NET_DETECT stMatchInfoNetDetect;
		struMatchInfo.pInfo = &stMatchInfoNetDetect;

		struMatchInfo.nType = MV_MATCH_TYPE_NET_DETECT; // ch:网络流量和丢包信息 | en:Net flow and lsot packet information
		memset(struMatchInfo.pInfo, 0, sizeof(MV_MATCH_INFO_NET_DETECT));
		struMatchInfo.nInfoSize = sizeof(MV_MATCH_INFO_NET_DETECT);

		nRet = MV_CC_GetAllMatchInfo(m_hDevHandle, &struMatchInfo);
		if (MV_OK != nRet)
		{
			return nRet;
		}

		MV_MATCH_INFO_NET_DETECT *pInfo = (MV_MATCH_INFO_NET_DETECT*)struMatchInfo.pInfo;
		*nFrameCount = pInfo->nNetRecvFrameCount;
		*nLostFrame = stMatchInfoNetDetect.nLostFrameCount;
	}
	else if (MV_USB_DEVICE == stDevInfo.nTLayerType)
	{
		MV_ALL_MATCH_INFO struMatchInfo = { 0 };
		MV_MATCH_INFO_USB_DETECT stMatchInfoNetDetect;
		struMatchInfo.pInfo = &stMatchInfoNetDetect;

		struMatchInfo.nType = MV_MATCH_TYPE_USB_DETECT; // ch:网络流量和丢包信息 | en:Net flow and lsot packet information
		memset(struMatchInfo.pInfo, 0, sizeof(MV_MATCH_INFO_USB_DETECT));
		struMatchInfo.nInfoSize = sizeof(MV_MATCH_INFO_USB_DETECT);

		nRet = MV_CC_GetAllMatchInfo(m_hDevHandle, &struMatchInfo);
		if (MV_OK != nRet)
		{
			return nRet;
		}

		MV_MATCH_INFO_NET_DETECT *pInfo = (MV_MATCH_INFO_NET_DETECT*)struMatchInfo.pInfo;
		*nFrameCount = pInfo->nNetRecvFrameCount;
		*nLostFrame = stMatchInfoNetDetect.nErrorFrameCount;
	}

	return MV_OK;
}


int HKCamera::SetTriggerON_Mode()
{
	int nRet = SetEnumValue("TriggerMode", MV_TRIGGER_MODE_ON);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_mode = MV_TRIGGER_MODE_ON;
	return MV_OK;
}

int HKCamera::SetContinuosMode()
{
	int nRet = SetEnumValue("TriggerMode", MV_TRIGGER_MODE_OFF);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_mode = MV_TRIGGER_MODE_OFF;
	return MV_OK;
}

int HKCamera::SetExposureTime(double exposure)
{
	// ch:调节这两个曝光模式，才能让曝光时间生效
	// en:Adjust these two exposure mode to allow exposure time effective
	int nRet = SetEnumValue("ExposureMode", MV_EXPOSURE_MODE_TIMED);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	nRet = SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);

	nRet = SetFloatValue("ExposureTime", exposure);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_exposuretime = exposure;
	return MV_OK;
}

int HKCamera::SetFrameRate(double frameRate)
{
	int nRet = SetBoolValue("AcquisitionFrameRateEnable", true);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	nRet = SetFloatValue("AcquisitionFrameRate", frameRate);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_frameRate = frameRate;
	return  MV_OK;
}

int HKCamera::SetGain(double gain)
{
	// ch:设置增益前先把自动增益关闭，失败无需返回
	//en:Set Gain after Auto Gain is turned off, this failure does not need to return
	int nRet = SetEnumValue("GainAuto", 0);

	nRet =  SetFloatValue("Gain", gain);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	m_gain = gain;
	return  MV_OK;
}

int HKCamera::GetExposureTime(double &exposure)
{
	float  fFloatValue = 0.0;
	int nRet = GetFloatValue("ExposureTime", &fFloatValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	exposure = fFloatValue;

	m_exposuretime = fFloatValue;
	return MV_OK;
}
int HKCamera::GetFrameRate(double &frameRate)
{
	float  fFloatValue = 0.0;

	int nRet = GetFloatValue("ResultingFrameRate", &fFloatValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	frameRate = fFloatValue;
	m_frameRate = fFloatValue;
	return MV_OK;
	
}
int HKCamera::GetGain(double& gain)
{
	float  fFloatValue = 0.0;

	int nRet = GetFloatValue("Gain", &fFloatValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}
	gain = (int)fFloatValue;
	m_gain = (int)fFloatValue;
	return MV_OK;
}
int HKCamera::GetTriggerMode(int &mode)// 0: continuos ,1 : trigger on
{
	unsigned int nEnumValue = 0;

	int nRet = GetEnumValue("TriggerMode", &nEnumValue);
	if (MV_OK != nRet)
	{
		return nRet;
	}

	mode = nEnumValue;
	m_mode = nEnumValue;
	return MV_OK;
}

int HKCamera::ExpsureTimePlus()
{
	return SetExposureTime(m_exposuretime+100);
}
int HKCamera::ExposureTimeMinus()
{
	return SetExposureTime(m_exposuretime - 100);
}

int HKCamera::FrameRatePlus()
{
	return SetFrameRate(m_frameRate + 10);
}
int HKCamera::FrameRateMinus()
{
	return SetFrameRate(m_frameRate - 10);
}

int HKCamera::GainPlus()
{
	return SetGain(m_gain + 1);
}
int HKCamera::GainMinus()
{
	return SetGain(m_gain - 1);
}
int HKCamera::ImgWH(UINT& width, UINT& height)
{ 
	int ret = GetIntValue("WidthMax", &width);
	ret = GetIntValue("HeightMax", &height);	
	return ret;
}
int HKCamera::setImgWH(UINT  width, UINT  height)
{
	
	int ret = SetIntValue("Width",  width);
	ret = SetIntValue("Height",  height);
	return ret;
}
int HKCamera::SetWB(UINT nValue)
{	
	int    ret = SetEnumValue("BalanceWhiteAuto",nValue);

	//int ret= SetIntValue("BalanceRatio", nValue);	
	return ret;
}