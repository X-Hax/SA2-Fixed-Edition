#include "pch.h"
#include "UsercallFunctionHandler.h"
#include "config.h"
#include "util.h"
#include "mod.h"

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

// Add additional safety checks for sub_5E3D90 to fix NaN car positions
BOOL CalcCarPath_r(int a1, LoopHead* path, float* a3, float* a4, float a5, NJS_POINT3* out_pos, Angle3* out_ang)
{
    NJS_POINT3 out1, out2;

    if (sub_5E3D90(path, a1, a3, a5, &out1) ||
        sub_5E3D90(path, a1, a3, a4[15], &out1) ||
        sub_5E3D90(path, a1, a3, a4[16], &out2)) {
        return TRUE;
    }

    NJS_POINT3 vec;
    vec.x = out1.x - out2.x;
    vec.y = out1.y - out2.y;
    vec.z = out1.z - out2.z;

    float dist = vec.x * vec.x + vec.z * vec.z;

    if (dist <= 0.0f)
        dist = 0.0f;

    out_ang->x = NJM_RAD_ANG(atan2f(-vec.y, sqrtf(dist)));
    out_ang->y = NJM_RAD_ANG(atan2f(vec.x, vec.z));
    out_ang->z = 0;

    vec.x = vec.x * 0.5f;
    vec.y = vec.y * 0.5f;
    vec.z = vec.z * 0.5f;

    out_pos->x = out2.x + vec.x;
    out_pos->y = out2.y + vec.y;
    out_pos->z = out2.z + vec.z;

    return FALSE;
}

extern "C"
{
	__declspec(dllexport) void __cdecl Init(const char* path, const HelperFunctions& helperFunctions)
	{
		config::read(path);

        // Fix City Escape car bug
		GenerateUsercallHook(CalcCarPath_r, rEAX, 0x5DE0B0, rECX, rEDX, rEAX, stack4, stack4, stack4, stack4);

        Remove_SA2ErrorMSG();
        TransporterMachineBGFix();
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}