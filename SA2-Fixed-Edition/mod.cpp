#include "pch.h"
#include "SA2ModLoader.h"
#include "UsercallFunctionHandler.h"
#include "Trampoline.h"
#include "config.h"
#include "util.h"
#include "mod.h"

static char __cdecl PResetStuff_r(ObjectMaster* tp, EntityData1* twp, EntityData2* mwp, CharObj2Base* pwp);
Trampoline PResetStuff_t(0x472F70, 0x472F78, PResetStuff_r);
static char __cdecl PResetStuff_r(ObjectMaster* tp, EntityData1* twp, EntityData2* mwp, CharObj2Base* pwp)
{
    // This is not always reset properly, fixes crash for pointer dereference in some places (like Pyramid Cave snake rail)
    if (pwp->CurrentDyncolTask)
    {
        if (!(twp->Status & (Status_OnObjectColli | Status_Ground)) || !(pwp->CurrentDyncolTask->Data1.Entity->Status & 0x100))
        {
            pwp->CurrentDyncolTask = nullptr;
        }
    }

    return ((decltype(PResetStuff_r)*)PResetStuff_t.Target())(tp, twp, mwp, pwp);
}

void TransporterMachineBGFix() //Credit: Shaddatic
{
    static float vRes = 480.0;
    static float hRes = 853.0;
    WriteData((float**)0x005A74C3, &vRes);   //Departure Machine Scrolling fix
    WriteData((float**)0x005A74D1, &hRes);
}

void Remove_SA2ErrorMSG()
{
    WriteData<5>((void*)0x688c4c, 0x90); //Remove controller and internet error thing
    WriteData<5>((void*)0x664d6e, 0x90);
    WriteData<5>((void*)0x439a66, 0x90);
}

//Sonic Light Dash Afterimage Shadow Bugfix code
//Credit: End User
FunctionPointer(void, sub_476C20, (int a1, int a2, int a3, float a4, char a5), 0x476C20);
void _cdecl sub_71E460ShadowFix(EntityData1* a1, CharObj2Base* a2, SonicCharObj2* a3)
{
	char v3; // cl
	NJS_OBJECT* v4; // eax
	double v5; // st7
	signed int v6; // edi
	float* v7; // ebx
	int v8; // [esp+18h] [ebp-4h]

	if (!(FrameCountIngame & 1))
	{
		v3 = a2->CharID;
		if (v3 == 0 && (v4 = CharacterModels[30].Model) != 0)
		{
			v5 = a2->AnimInfo.field_10 * 4096.0;
		}
		else
		{
			if (v3 != 1)
			{
				return;
			}
			v4 = CharacterModels[103].Model;
			if (!CharacterModels[103].Model)
			{
				return;
			}
			v5 = a2->AnimInfo.field_10 * 8192.0;
		}
		v8 = (int)v4;
		v6 = (signed int)v5;
		njPushUnitMatrix();
		njTranslate(0, a1->Position.x, a1->Position.y, a1->Position.z);
		if (a1->Rotation.z)
		{
			njRotateZ(0, a1->Rotation.z);
		}
		if (a1->Rotation.x)
		{
			njRotateX(0, a1->Rotation.x);
		}
		if (a1->Rotation.y != 0x8000)
		{
			njRotateY(0, 0x8000 - a1->Rotation.y);
		}
		if (!TwoPlayerMode)
		{
			sub_476C20(v8, 0, (int)a3->TextureList, 0.0, a2->PlayerNum);
		}
		njPopMatrixEx();
	}
}

static void __declspec(naked) ShadowLightDashFix()
{
	__asm
	{
		push[esp + 08h] // a3
		push[esp + 08h] // a2
		push esi // a1

		// Call your __cdecl function here:
		call sub_71E460ShadowFix

		pop esi // a1
		add esp, 4 // a2
		add esp, 4 // a3
		retn
	}
}

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		config::read(path);

		// Fix City Escape light field task missing
		CityEscape_ObjectArray[58].Function = (ObjectFuncPtr)LIGHT_SW;
		DowntownRace_ObjectArray_[58].Function = (ObjectFuncPtr)LIGHT_SW;

        Remove_SA2ErrorMSG();
        TransporterMachineBGFix();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}