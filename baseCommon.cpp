#include "stdafx.h"
#include "baseCommon.h"

static CmLog* g_log = NULL;
BASECORE_API void LOG_Init(string fileName)
{
	if (g_log == NULL)
	{
		g_log = new CmLog(fileName);
	}
}
BASECORE_API CmLog* LogInstance()
{
	return g_log;
}
