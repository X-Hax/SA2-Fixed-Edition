#include "pch.h"

HelperFunctions HelperFunctionsGlobal;

extern "C" {

	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		Sleep(10);

		HelperFunctionsGlobal = helperFunctions;


		ReadConfig(path); //get mod settings by the user
	}

	__declspec(dllexport) void __cdecl OnFrame() {

	}



	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}