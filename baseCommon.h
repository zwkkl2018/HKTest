#pragma once
#include"Log.h"
 
#define BASECORE_API __declspec(dllexport)

extern"C" BASECORE_API void LOG_Init(string fileName);
extern"C" BASECORE_API CmLog* LogInstance();
