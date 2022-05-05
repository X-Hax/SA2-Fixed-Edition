#pragma once

DataArray(ObjectListEntry, DowntownRace_ObjectArray_, 0x14079D8, 102);

static const void* const loc_5E3D90 = (void*)0x5E3D90;
static inline BOOL sub_5E3D90(LoopHead* path, int a2, float* a3, float a4, NJS_POINT3* out_pos)
{
    int value;
    __asm
    {
        push[out_pos]
        push[a4]
        push[a3]
        push[a2]
        mov eax, [path]
        call loc_5E3D90
        mov value, eax
        add esp, 16
    }
    return value;
}
