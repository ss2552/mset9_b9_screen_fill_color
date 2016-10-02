#pragma once

#include "types.h"

inline void* getThreadLocalStorage(void)
{
	void* ret;
	__asm__ volatile
	(
		"mrc p15, 0, %[data], c13, c0, 3"
		:[data] "=r" (ret)
	);
	return ret;
}

inline u32* getThreadCommandBuffer(void)
{
	return ((u32*)getThreadLocalStorage() + 0x80/sizeof(u32));
}

Result srvRegisterProcess(Handle *handle, u32 procid, u32 count, const void *serviceaccesscontrol);

Result srvUnregisterProcess(Handle *handle, u32 procid);

Result srv_RegisterClient(Handle* handleptr);

Result srv_getServiceHandle(Handle* handleptr, Handle* out, char* server);

Result PS_VerifyRsaSha256(Handle *handle, u32 *fb); //Exploit

extern u32 *work_buffer;
