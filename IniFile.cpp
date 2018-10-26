#include "stdafx.h"
#include "IniFile.h"
#include <fstream>

/*	���Դ���
	INIFile inifile;
	inifile.Create("abc.ini");
	string val;
	inifile.GetVar("SERVER","PORT",val);
	AfxMessageBox(val.c_str());
	inifile.SetVar("SERVER","PORT","test");
	inifile.Save("abc.ini");
*/

INIFile::INIFile()
{
	m_FileContent.erase(m_FileContent.begin(),m_FileContent.end());
	m_bCreated = false;
}

INIFile::INIFile(string filename)
{
	m_FileContent.erase(m_FileContent.begin(),m_FileContent.end());
	m_bCreated = false;
	Create(filename);
}

bool INIFile::Create(string filename)
{
	ifstream ifile;
	string strLine;
	if (filename == "")
	{
		return false;
	}
	ifile.open(filename.c_str());
	if (!ifile.is_open())
	{
		FILE *pfile = fopen(filename.c_str(),"wb+");
		if (pfile == NULL)
		{
			return false;
		}
		fclose(pfile);
		ifile.open(filename.c_str());
	}
	while (getline(ifile,strLine))
	{
		ProcessLine(strLine);
	}
	ifile.close();
	m_bCreated = true;
	return true;
}



bool INIFile::ProcessLine(string strLine)
{
	string key,val,sect;
	if (strLine[0] == '#')
	{
		//�����ע��ֱ�ӷ���
		return false;
	}
	if (strLine[0] == '[')
	{
		sect = strLine.substr(1,strLine.find("]")-1);
		SetVar(sect);
	}else if (strLine.find("=") != 0)
	{
		string::size_type ePos = strLine.find("=");
		if (ePos != std::string::npos)
		{
			SetVar(m_FileContent.back().SectionName, strLine.substr(0, ePos), strLine.substr(ePos + 1, strLine.size()), false);
 
		}
	}else
	{
		return false;
	}
	return true;
}

bool INIFile::SetVar(string strSection, string varName, string varvalue, bool breplace)
{
	//�Ȳ��Ҷ�
	itVector it = m_FileContent.begin();
	for (it;it!=m_FileContent.end();it++)
	{
		if (it->SectionName == strSection)
		{
			break;
		}
	}
	//����Ǽ����
	if (varName == "" && varvalue == "")
	{
		if (it != m_FileContent.end() && !breplace)
		{
			return false;
		}
		if (it != m_FileContent.end() && breplace)
		{
			m_FileContent.erase(it);
		}
		//���������
		Section t;
		t.SectionName = strSection;
		m_FileContent.push_back(t);
	}else
	{
		//�������
		if (it == m_FileContent.end())
		{
			Section t;
			t.SectionName = strSection;
			m_FileContent.push_back(t);
			it = m_FileContent.end()-1;
		}
		if (breplace)
		{
			it->dMap.erase(varName);
		}
		it->dMap.insert(make_pair(varName,varvalue));
	}
	return true;
}

bool INIFile::GetVar(string strSection, string varName, string default, string &varvalue)
{
	//�Ȳ��Ҷ�
	itVector it = m_FileContent.begin();
	for (it;it!=m_FileContent.end();it++)
	{
		if (it->SectionName == strSection)
		{
			break;
		}
	}
	//����β�����
	if (it == m_FileContent.end())
	{
		return false;
	}
	//�����ֵ�
	itmapDirectory it2 = it->dMap.find(varName);
	if (it2 == it->dMap.end())
	{
		varvalue = default;
		return false;
	}
	varvalue = it2->second;
	return true;
}

bool INIFile::Save(string filename)
{
	ofstream ofile;
	string strLine;
	if (filename == "")
	{
		return false;
	}
	ofile.open(filename.c_str());
	if (!ofile.is_open())
	{
		return false;
	}
	itVector it = m_FileContent.begin();
	for (it;it!=m_FileContent.end();it++)
	{
		ofile<<"["<<it->SectionName<<"]"<<endl;
		itmapDirectory itd = it->dMap.begin();
		for (itd;itd!=it->dMap.end();itd++)
		{
			if (itd->first == " " || itd->second==" ") continue;
			ofile<<itd->first<<"="<<itd->second<<endl;
		}
	}
	ofile.close();
	return false;
}

