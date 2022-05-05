#include "pch.h"
#include "UsercallFunctionHandler.h"
#include "config.h"
#include "util.h"
#include "mod.h"

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
	}

	__declspec(dllexport) ModInfo SA2ModInfo = { ModLoaderVer };
}