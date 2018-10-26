#include "stdafx.h"
#include "CameraConfig.h"


CameraConfig::CameraConfig()
{

	m_MySEQ.clear();
	//	string ss = "C:\\log.txt";
	//	m_CLog.setlogFile(ss);
	//pm_CLog = NULL;
}


CameraConfig::~CameraConfig()
{
	
}
void CameraConfig::Save()
{
	INIFile ini;
	if (!ini.Create(m_cameraConfigFile))
		return;
	string sTmp;
	for (int indexCam = 0; indexCam < m_MySEQ.size(); indexCam++)
	{

		MySEQ* sp = m_MySEQ[indexCam];

		char section[50] = { 0 };
		char keywords[50] = { 0 };
		char values[150] = { 0 };
		sprintf(section, "Cam%d", indexCam);

		//	sprintf(keywords, "%d", sp->m_exp);

		//SetVar(string strSection, string varName, string varvalue, bool breplace)
		//
		//	ini.SetVar(section, "exp", keywords, true);

		//	sprintf(keywords, "%d", sp->m_gain);
		//	ini.SetVar(section, "gain", keywords, true);


		sprintf(keywords, "%d", sp->m_trigger);
		ini.SetVar(section, "ztrigger", keywords, true);





		sprintf(section, "Cam%d_DeFect", indexCam);
		ini.GetVar(section, "dCnt", "1", sTmp);
		sp->m_DefectCnt = atoi(sTmp.c_str());



		for (int indexDefct = 0; indexDefct < sp->m_DefectCnt; indexDefct++)
		{
			DeFectPara dfPara;


			sprintf(keywords, "dItem_%dCname", indexDefct);
			ini.GetVar(section, keywords, "缺陷null", sTmp);
			dfPara.defectName = sTmp;


			sprintf(keywords, "dItem_%dCnt", indexDefct);
			ini.GetVar(section, keywords, "0", sTmp);
			dfPara.defectParaCnt = atoi(sTmp.c_str());



			for (int indexOneDef = 0; indexOneDef < dfPara.defectParaCnt; indexOneDef++)
			{


				sprintf(keywords, "dItem_%d_%d", indexDefct, indexOneDef);
				//ini.GetVar(section, ctmp, "NULL", sTmp);
				//dfPara.defectParaArrStr.push_back(sTmp);
				sprintf(values, "%s", sp->m_DeFectParaStrArr[indexDefct].defectParaArrStr[indexOneDef].c_str());


				ini.SetVar(section, keywords, values, true);

			}

		}

	}

	ini.Save(m_cameraConfigFile);
}

void CameraConfig::getApppath()
{
	char exe[256];
	::GetModuleFileNameA(NULL, exe, 255);//得到绝对路径
	m_cameraConfigFile = exe;
	int i = m_cameraConfigFile.find_last_of('\\');
	m_cameraConfigFile = m_cameraConfigFile.substr(0, i);
	m_appPath = m_cameraConfigFile;
}
void CameraConfig::Load()
{

	getApppath();
	m_cameraConfigFile = m_appPath + ("\\用户数据\\Admin\\出厂参数\\CameraNew.ini");
	string logFile = m_appPath + ("\\用户数据\\Admin\\出厂参数\\Log.txt");

	
	LOG_Init(logFile);
 
	INIFile ini;
	if (!ini.Create(m_cameraConfigFile))
		return;
	m_MySEQ.clear();

	string sTmp;
	ini.GetVar("CamNo", "Camcnt", "4", sTmp);
	int camNo = atoi(sTmp.c_str());
	for (int indexCam = 0; indexCam < camNo; indexCam++)
	{
		MySEQ* sp = new MySEQ;
		char section[20] = { 0 };
		char keywords[20] = { 0 };

		sprintf(section, "Cam%d", indexCam);





		ini.GetVar(section, "SerNo", "13", sp->m_Serio);

		ini.GetVar(section, "Info", "0", sp->m_camInfo);


		ini.GetVar(section, "IP", "192.16.8.1", sp->m_IP);

		ini.GetVar(section, "whiteB", "2", sTmp);
		sp->m_WhiteBalance = atoi(sTmp.c_str());


		//ini.GetVar(section, "saveImg", "0", sTmp);
		//sp->m_SaveImg = atoi(sTmp.c_str());

		//ini.GetVar(section, "exp", "10", sTmp);// sp->m_exp);
		//sp->m_exp = atoi(sTmp.c_str());





		//ini.GetVar(section, "gain", "10", sTmp);// sp->m_exp);
		//sp->m_gain = atoi(sTmp.c_str());




		ini.GetVar(section, "width", "10", sTmp);// sp->m_width);
		sp->m_width = atoi(sTmp.c_str());
		ini.GetVar(section, "hight", "10", sTmp);
		sp->m_hight = atoi(sTmp.c_str());
		ini.GetVar(section, "bitdepth", "3", sTmp);
		sp->m_bitdepth = atoi(sTmp.c_str());

		ini.GetVar(section, "ztrigger", "1", sTmp);
		sp->m_trigger = atoi(sTmp.c_str());

		ini.GetVar(section, "zvalidSaveSize", "1", sTmp);
		sp->m_validSaveSize = atoi(sTmp.c_str());

		ini.GetVar(section, "zmcnt", "0", sTmp);
		sp->m_cnt = atoi(sTmp.c_str());

		//	ini.GetVar(section, "ParCnt", "8", sTmp);
		//	sp->m_ParCnt = atoi(sTmp.c_str());



		//	for (int parIndex = 0; parIndex < sp->m_ParCnt; parIndex++)
		//{
		//	sprintf(keywords, "Par%d", indexCam);
		//	ini.GetVar(section, keywords, "8", sTmp);

		//	sp->m_paraArr.push_back(atoi(sTmp.c_str()));
		//} 
		//


		sprintf(section, "Cam%d_DeFect", indexCam);
		ini.GetVar(section, "dCnt", "1", sTmp);
		sp->m_DefectCnt = atoi(sTmp.c_str());


		char ctmp[20] = { 0 };
		for (int indexDefct = 0; indexDefct < sp->m_DefectCnt; indexDefct++)
		{
			DeFectPara dfPara;


			sprintf(ctmp, "dItem_%dCname", indexDefct);
			ini.GetVar(section, ctmp, "缺陷null", sTmp);
			dfPara.defectName = sTmp;



			sprintf(ctmp, "dItem_%dCnt", indexDefct);
			ini.GetVar(section, ctmp, "0", sTmp);
			dfPara.defectParaCnt = atoi(sTmp.c_str());

			for (int indexOneDef = 0; indexOneDef < dfPara.defectParaCnt; indexOneDef++)
			{
				sprintf(ctmp, "dItem_%d_%d", indexDefct, indexOneDef);
				ini.GetVar(section, ctmp, "NULL", sTmp);
				dfPara.defectParaArrStr.push_back(sTmp);

			}

			sp->m_DeFectParaStrArr.push_back(dfPara);

		}


		m_MySEQ.push_back(sp);
	}

	getDeFectPara();
}


void  CameraConfig::getDeFectPara()
{
	m_mulDeFectPara.clear();
	m_mapParaKey.clear();
	for (int seqIndex = 0; seqIndex < m_MySEQ.size(); seqIndex++)
	{
		vector<DeFectPara>*  pDeFectPara = &m_MySEQ[seqIndex]->m_DeFectParaStrArr;

		for (int i = 0; i < pDeFectPara->size(); i++)
		{
			//int i = m_CamIdex;
			DeFectPara* _DeFectPara = &m_MySEQ[seqIndex]->m_DeFectParaStrArr[i];

			//	DeFectPara* _DeFectPara = &p_MyCamPara->m_DeFectParaStrArr[i];
			string m_camInfo = m_MySEQ[seqIndex]->m_camInfo;

			oneDeFectPara _oneDeFectPara;


			for (int oneindex = 0; oneindex < _DeFectPara->defectParaCnt; oneindex++)
			{

				_oneDeFectPara.camIndex = seqIndex;
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


				if (&m_mapParaKey != NULL)
				{
					CamIDType mapCamIndex = CamIDType::CAM_TOP;
					if (m_camInfo == "CAM_TOP")	mapCamIndex = CamIDType::CAM_TOP;
					if (m_camInfo == "CAM_LONGSIDE")	mapCamIndex = CamIDType::CAM_LONGSIDE;
					if (m_camInfo == "CAM_SHORTSIDE_0")	mapCamIndex = CamIDType::CAM_SHORTSIDE_0;
					if (m_camInfo == "CAM_SHORTSIDE_1")	mapCamIndex = CamIDType::CAM_SHORTSIDE_1;



					ParaKey parkey(mapCamIndex, _oneDeFectPara.parentName, _oneDeFectPara.name);
					auto m = m_mapParaKey.find(parkey);
					if (m == m_mapParaKey.end())
					{
						m_mapParaKey.insert(make_pair(parkey, _oneDeFectPara.paraVal));
					}
					else
					{
						m->second = _oneDeFectPara.paraVal;
					}
				}


				m_mulDeFectPara.push_back(_oneDeFectPara);
			}

		}
	}
}