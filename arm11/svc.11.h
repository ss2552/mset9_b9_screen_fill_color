#ifndef SVC_11_H_
#define SVC_11_H_

void svc_sleepThread(s64 ns);
s32 svc_connectToPort(Handle* out, const char* portName);
s32 svc_sendSyncRequest(Handle session);
s32 svc_getProcessId(u32 *out, Handle handle);

s32 svc_kernelSetState(u32 type, u32 p0, u32 p1, u32 p2);
#endif//SVC_11_H_
