#pragma once
//#include <python.h>
#include <string>
#include <vector>
#pragma  warning(disable:4996)
using namespace  std;
class pythonTool
{
	std::vector<std::string>  m_fileList;
public:

	pythonTool()
	{
	}

	~pythonTool()
	{
		//Py_Finalize();
	}

	//void Pythoninit()
	//{
	//	Py_SetPythonHome(L"C:\\ProgramData\\Anaconda3");
	//	Py_Initialize();
	//	PyRun_SimpleString("import sys");

	//
	//	//PyRun_SimpleString("sys.path.append( 'F:/project/TissueProject/TissueApp/bin/upython' )");
	//	PyRun_SimpleString("sys.path.append( 'F:/project/TissueProject/TissueApp/bin/upython' )");
	//	// PyRun_SimpleString("sys.path.append( '../upython' )");
	//	PyRun_SimpleString("sys.path");
	//	PyRun_SimpleString("print ('------------------------------------') ");
	//}
	//bool pyReadfilelist(std::vector<std::string>  filelist)
	//{
	//	PyObject* pModule = NULL;
	//	PyObject* pFun = NULL;
	//	PyObject* pFun_List = NULL;
	//	PyObject * pArg = NULL;
	//	char listff[50] = { 0 };
	//	pModule = PyImport_ImportModule("readfilelist");
	//	if (NULL != pModule)
	//	{
	//	//	printf("python module ok\n");
	//		//pFun = PyObject_GetAttrString(pModule, "Hello");
	//		pArg = Py_BuildValue("(s)", "F:\project\TissueProject\nature");
	//		pFun_List = PyObject_GetAttrString(pModule, "get_files");
	//		PyObject* pRet = PyEval_CallObject(pFun_List, pArg);

	//		int list_len = PyObject_Size(pRet);
	//		if (list_len>=1)
	//		{
	//			PyObject* list_item = NULL;
	//			char *str_item = NULL;
	//			str_item = new char[255];

	//			for (int i = 0; i < list_len; i++)
	//			{
	//				PyObject* keylist = PyList_GetItem(pRet, i);
	//				sprintf(str_item, "%s", _PyUnicode_AsString(keylist));
	//				printf("%s\n", str_item);

	//				filelist.push_back(str_item);
	//			}
	//			return true;
	//		}
	//		
	//		
	//	}
	//	 
	//	return false;
	//}
};

