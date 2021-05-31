// ATLProject1.cpp: WinMain 的实现


#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "ATLProject1_i.h"


using namespace ATL;


class CATLProject1Module : public ATL::CAtlExeModuleT< CATLProject1Module >
{
public :
	DECLARE_LIBID(LIBID_ATLProject1Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLPROJECT1, "{c3b8c8e3-9cd5-4d32-be43-f5e6230a25f9}")
};

CATLProject1Module _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

