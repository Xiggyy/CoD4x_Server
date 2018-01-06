#include "dispatcher.hpp"
#include "macro.hpp"
#include <compiletime/ctcrc32.hpp>
#include "PluginHandler.hpp"

BEGIN_EXTERN_C
#include <core/qcommon_io.h>
#include <core/qcommon_mem.h>
#include <core/cmd.h>
#include <core/server.h>
END_EXTERN_C

using namespace phandler;

#define VARG(num, type) ((type)(((int*)&CodeHash_)[1 + num]))

void SysCallDispatcher(const unsigned int CodeHash_, ...)
{
    switch(CodeHash_)
    {
        case CRC32("Com_Printf"):
        {
            Com_Printf(VARG(0, const char*));
            break;
        }

        case CRC32("Com_Error"):
        {
            Com_Error(0, VARG(0, const char*));
            break;
        }

        case CRC32("Com_PrintError"):
        {
            Com_PrintError(VARG(0, const char*));
            break;
        }

        case CRC32("Com_DPrintf"):
        {
            Com_DPrintf(VARG(0, const char*));
            break;
        }

        case CRC32("Com_PrintWarning"):
        {
            Com_PrintWarning(VARG(0, const char*));
            break;
        }

        case CRC32("Cbuf_AddText"):
        {
            Cbuf_AddText(VARG(0, const char*));
            break;
        }

        case CRC32("SV_SetConfigString"):
        {
            SV_SetConfigstring(VARG(0, int), VARG(1, const char*));
            break;
        }

        case CRC32("SV_GetConfigString"):
        {
            SV_GetConfigstring(VARG(0, int), VARG(1, char*), VARG(2, int));
            break;
        }

        case CRC32("SV_GetStat"):
        {
            int* result = VARG(2, int*);
            *result = SV_GetClientStat(VARG(0, int), VARG(1, int));
            break;
        }

        case CRC32("SV_SetStat"):
        {
            SV_SetClientStat(VARG(0, int), VARG(1, int), VARG(2, int));
            break;
        }

        case CRC32("RemoveBanByIP"):
        {
            SV_RemoveBanByip(VARG(0, netadr_t*));
            break;
        }

        case CRC32("AddBanByIP"):
        {
            SV_PlayerAddBanByip(VARG(0, netadr_t*), VARG(1, char*), VARG(2, int));
            break;
        }

        case CRC32("MemAlloc"):
        {
            int iSize = VARG(0, int);
            void** ppAddress = VARG(1, void**);
            *ppAddress = Z_Malloc(iSize);
            if (*ppAddress)
                GetPluginHandler()->CurrentPlugin()->SaveMemoryAddress(*ppAddress, iSize);

            break;
        }

        case CRC32("MemFree"):
        {
            void* pAddress = VARG(0, void*);
            Z_Free(pAddress);
            if (pAddress)
                GetPluginHandler()->CurrentPlugin()->DeleteMemoryAddress(pAddress);

            break;
        }
        
        default: 
            Com_Error(0, "Unknown system call hash: 0x%X", CodeHash_);
    }
}