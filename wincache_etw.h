//**********************************************************************`
//* This is an include file generated by Message Compiler.             *`
//*                                                                    *`
//* Copyright (c) Microsoft Corporation. All Rights Reserved.          *`
//**********************************************************************`
#pragma once
#include <wmistr.h>
#include <evntrace.h>
#include "evntprov.h"
//
//  Initial Defs
//
#if !defined(ETW_INLINE)
#define ETW_INLINE DECLSPEC_NOINLINE __inline
#endif

#if defined(__cplusplus)
extern "C" {
#endif

//
// Allow Diasabling of code generation
//
#ifndef MCGEN_DISABLE_PROVIDER_CODE_GENERATION
#if  !defined(McGenDebug)
#define McGenDebug(a,b)
#endif 


#if !defined(MCGEN_TRACE_CONTEXT_DEF)
#define MCGEN_TRACE_CONTEXT_DEF
typedef struct _MCGEN_TRACE_CONTEXT
{
    TRACEHANDLE            RegistrationHandle;
    TRACEHANDLE            Logger;
    ULONGLONG              MatchAnyKeyword;
    ULONGLONG              MatchAllKeyword;
    ULONG                  Flags;
    ULONG                  IsEnabled;
    UCHAR                  Level; 
    UCHAR                  Reserve;
    USHORT                 EnableBitsCount;
    PULONG                 EnableBitMask;
    const ULONGLONG*       EnableKeyWords;
    const UCHAR*           EnableLevel;
} MCGEN_TRACE_CONTEXT, *PMCGEN_TRACE_CONTEXT;
#endif

#if !defined(MCGEN_LEVEL_KEYWORD_ENABLED_DEF)
#define MCGEN_LEVEL_KEYWORD_ENABLED_DEF
FORCEINLINE
BOOLEAN
McGenLevelKeywordEnabled(
    _In_ PMCGEN_TRACE_CONTEXT EnableInfo,
    _In_ UCHAR Level,
    _In_ ULONGLONG Keyword
    )
{
    //
    // Check if the event Level is lower than the level at which
    // the channel is enabled.
    // If the event Level is 0 or the channel is enabled at level 0,
    // all levels are enabled.
    //

    if ((Level <= EnableInfo->Level) || // This also covers the case of Level == 0.
        (EnableInfo->Level == 0)) {

        //
        // Check if Keyword is enabled
        //

        if ((Keyword == (ULONGLONG)0) ||
            ((Keyword & EnableInfo->MatchAnyKeyword) &&
             ((Keyword & EnableInfo->MatchAllKeyword) == EnableInfo->MatchAllKeyword))) {
            return TRUE;
        }
    }

    return FALSE;

}
#endif

#if !defined(MCGEN_EVENT_ENABLED_DEF)
#define MCGEN_EVENT_ENABLED_DEF
FORCEINLINE
BOOLEAN
McGenEventEnabled(
    _In_ PMCGEN_TRACE_CONTEXT EnableInfo,
    _In_ PCEVENT_DESCRIPTOR EventDescriptor
    )
{

    return McGenLevelKeywordEnabled(EnableInfo, EventDescriptor->Level, EventDescriptor->Keyword);

}
#endif


//
// EnableCheckMacro
//
#ifndef MCGEN_ENABLE_CHECK
#define MCGEN_ENABLE_CHECK(Context, Descriptor) (Context.IsEnabled &&  McGenEventEnabled(&Context, &Descriptor))
#endif

#if !defined(MCGEN_CONTROL_CALLBACK)
#define MCGEN_CONTROL_CALLBACK

DECLSPEC_NOINLINE __inline
VOID
__stdcall
McGenControlCallbackV2(
    _In_ LPCGUID SourceId,
    _In_ ULONG ControlCode,
    _In_ UCHAR Level,
    _In_ ULONGLONG MatchAnyKeyword,
    _In_ ULONGLONG MatchAllKeyword,
    _In_opt_ PEVENT_FILTER_DESCRIPTOR FilterData,
    _Inout_opt_ PVOID CallbackContext
    )
/*++

Routine Description:

    This is the notification callback for Vista.

Arguments:

    SourceId - The GUID that identifies the session that enabled the provider. 

    ControlCode - The parameter indicates whether the provider 
                  is being enabled or disabled.

    Level - The level at which the event is enabled.

    MatchAnyKeyword - The bitmask of keywords that the provider uses to 
                      determine the category of events that it writes.

    MatchAllKeyword - This bitmask additionally restricts the category 
                      of events that the provider writes. 

    FilterData - The provider-defined data.

    CallbackContext - The context of the callback that is defined when the provider 
                      called EtwRegister to register itself.

Remarks:

    ETW calls this function to notify provider of enable/disable

--*/
{
    PMCGEN_TRACE_CONTEXT Ctx = (PMCGEN_TRACE_CONTEXT)CallbackContext;
    ULONG Ix;
#ifndef MCGEN_PRIVATE_ENABLE_CALLBACK_V2
    UNREFERENCED_PARAMETER(SourceId);
    UNREFERENCED_PARAMETER(FilterData);
#endif

    if (Ctx == NULL) {
        return;
    }

    switch (ControlCode) {

        case EVENT_CONTROL_CODE_ENABLE_PROVIDER:
            Ctx->Level = Level;
            Ctx->MatchAnyKeyword = MatchAnyKeyword;
            Ctx->MatchAllKeyword = MatchAllKeyword;
            Ctx->IsEnabled = EVENT_CONTROL_CODE_ENABLE_PROVIDER;

            for (Ix = 0; Ix < Ctx->EnableBitsCount; Ix += 1) {
                if (McGenLevelKeywordEnabled(Ctx, Ctx->EnableLevel[Ix], Ctx->EnableKeyWords[Ix]) != FALSE) {
                    Ctx->EnableBitMask[Ix >> 5] |= (1 << (Ix % 32));
                } else {
                    Ctx->EnableBitMask[Ix >> 5] &= ~(1 << (Ix % 32));
                }
            }
            break;

        case EVENT_CONTROL_CODE_DISABLE_PROVIDER:
            Ctx->IsEnabled = EVENT_CONTROL_CODE_DISABLE_PROVIDER;
            Ctx->Level = 0;
            Ctx->MatchAnyKeyword = 0;
            Ctx->MatchAllKeyword = 0;
            if (Ctx->EnableBitsCount > 0) {
                RtlZeroMemory(Ctx->EnableBitMask, (((Ctx->EnableBitsCount - 1) / 32) + 1) * sizeof(ULONG));
            }
            break;
 
        default:
            break;
    }

#ifdef MCGEN_PRIVATE_ENABLE_CALLBACK_V2
    //
    // Call user defined callback
    //
    MCGEN_PRIVATE_ENABLE_CALLBACK_V2(
        SourceId,
        ControlCode,
        Level,
        MatchAnyKeyword,
        MatchAllKeyword,
        FilterData,
        CallbackContext
        );
#endif
   
    return;
}

#endif
#endif // MCGEN_DISABLE_PROVIDER_CODE_GENERATION
//+
// Provider PHP-Wincache Event Count 10
//+
EXTERN_C __declspec(selectany) const GUID PhpWinCacheEtwProvider = {0xf7ad0093, 0xd5c3, 0x46b9, {0xbe, 0xea, 0xa9, 0xfc, 0xec, 0x7e, 0x14, 0x08}};
//
// Keyword
//
#define INIT_KEYWORD 0x1
#define MEMORY_KEYWORD 0x2
#define LOCKS_KEYWORD 0x4
#define OPCODE_KEYWORD 0x8
#define FCACHE_KEYWORD 0x10
#define ZCACHE_KEYWORD 0x20
#define SESSION_KEYWORD 0x40

//
// Event Descriptors
//
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR ModuleInitErrorEvent = {0x1, 0x0, 0x0, 0x2, 0x0, 0x0, 0x1};
#define ModuleInitErrorEvent_value 0x1
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR InitMutexErrorEvent = {0x2, 0x0, 0x0, 0x2, 0x0, 0x0, 0x1};
#define InitMutexErrorEvent_value 0x2
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR MemBlockNotInUse = {0xa, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define MemBlockNotInUse_value 0xa
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR MemFreeAddrNotInSegment = {0xb, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define MemFreeAddrNotInSegment_value 0xb
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR MemCombineNonFreeBlock = {0xc, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define MemCombineNonFreeBlock_value 0xc
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR MemFreeListCorrupt = {0xd, 0x0, 0x0, 0x2, 0x0, 0x0, 0x2};
#define MemFreeListCorrupt_value 0xd
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR LockAbandonedMutex = {0x14, 0x0, 0x0, 0x2, 0x0, 0x0, 0x4};
#define LockAbandonedMutex_value 0x14
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR LockFailedWaitForLock = {0x15, 0x0, 0x0, 0x2, 0x0, 0x0, 0x4};
#define LockFailedWaitForLock_value 0x15
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR UnlockAbandonedMutex = {0x16, 0x0, 0x0, 0x2, 0x0, 0x0, 0x4};
#define UnlockAbandonedMutex_value 0x16
EXTERN_C __declspec(selectany) const EVENT_DESCRIPTOR UnlockFailedWaitForLock = {0x17, 0x0, 0x0, 0x2, 0x0, 0x0, 0x4};
#define UnlockFailedWaitForLock_value 0x17

//
// Note on Generate Code from Manifest Windows Vista and above
//
//Structures :  are handled as a size and pointer pairs. The macro for the event will have an extra 
//parameter for the size in bytes of the structure. Make sure that your structures have no extra padding.
//
//Strings: There are several cases that can be described in the manifest. For array of variable length 
//strings, the generated code will take the count of characters for the whole array as an input parameter. 
//
//SID No support for array of SIDs, the macro will take a pointer to the SID and use appropriate 
//GetLengthSid function to get the length.
//

//
// Allow Diasabling of code generation
//
#ifndef MCGEN_DISABLE_PROVIDER_CODE_GENERATION

//
// Globals 
//


//
// Event Enablement Bits
//

EXTERN_C __declspec(selectany) DECLSPEC_CACHEALIGN ULONG PHP_WincacheEnableBits[1];
EXTERN_C __declspec(selectany) const ULONGLONG PHP_WincacheKeywords[3] = {0x1, 0x2, 0x4};
EXTERN_C __declspec(selectany) const UCHAR PHP_WincacheLevels[3] = {2, 2, 2};
EXTERN_C __declspec(selectany) MCGEN_TRACE_CONTEXT PhpWinCacheEtwProvider_Context = {0, 0, 0, 0, 0, 0, 0, 0, 3, PHP_WincacheEnableBits, PHP_WincacheKeywords, PHP_WincacheLevels};

EXTERN_C __declspec(selectany) REGHANDLE PHP_WincacheHandle = (REGHANDLE)0;

#if !defined(McGenEventRegisterUnregister)
#define McGenEventRegisterUnregister
DECLSPEC_NOINLINE __inline
ULONG __stdcall
McGenEventRegister(
    _In_ LPCGUID ProviderId,
    _In_opt_ PENABLECALLBACK EnableCallback,
    _In_opt_ PVOID CallbackContext,
    _Inout_ PREGHANDLE RegHandle
    )
/*++

Routine Description:

    This function register the provider with ETW USER mode.

Arguments:
    ProviderId - Provider Id to be register with ETW.

    EnableCallback - Callback to be used.

    CallbackContext - Context for this provider.

    RegHandle - Pointer to Registration handle.

Remarks:

    If the handle != NULL will return ERROR_SUCCESS

--*/
{
    ULONG Error;


    if (*RegHandle) {
        //
        // already registered
        //
        return ERROR_SUCCESS;
    }

    Error = EventRegister( ProviderId, EnableCallback, CallbackContext, RegHandle); 

    return Error;
}


DECLSPEC_NOINLINE __inline
ULONG __stdcall
McGenEventUnregister(_Inout_ PREGHANDLE RegHandle)
/*++

Routine Description:

    Unregister from ETW USER mode

Arguments:
            RegHandle this is the pointer to the provider context
Remarks:
            If Provider has not register RegHandle = NULL,
            return ERROR_SUCCESS
--*/
{
    ULONG Error;


    if(!(*RegHandle)) {
        //
        // Provider has not registerd
        //
        return ERROR_SUCCESS;
    }

    Error = EventUnregister(*RegHandle); 
    *RegHandle = (REGHANDLE)0;
    
    return Error;
}
#endif
//
// Register with ETW Vista +
//
#ifndef EventRegisterPHP_Wincache
#define EventRegisterPHP_Wincache() McGenEventRegister(&PhpWinCacheEtwProvider, McGenControlCallbackV2, &PhpWinCacheEtwProvider_Context, &PHP_WincacheHandle) 
#endif

//
// UnRegister with ETW
//
#ifndef EventUnregisterPHP_Wincache
#define EventUnregisterPHP_Wincache() McGenEventUnregister(&PHP_WincacheHandle) 
#endif

//
// Enablement check macro for ModuleInitErrorEvent
//

#define EventEnabledModuleInitErrorEvent() ((PHP_WincacheEnableBits[0] & 0x00000001) != 0)

//
// Event Macro for ModuleInitErrorEvent
//
#define EventWriteModuleInitErrorEvent(Error)\
        EventEnabledModuleInitErrorEvent() ?\
        Template_x(PHP_WincacheHandle, &ModuleInitErrorEvent, Error)\
        : ERROR_SUCCESS\

//
// Enablement check macro for InitMutexErrorEvent
//

#define EventEnabledInitMutexErrorEvent() ((PHP_WincacheEnableBits[0] & 0x00000001) != 0)

//
// Event Macro for InitMutexErrorEvent
//
#define EventWriteInitMutexErrorEvent(Name, OwnerPid, OwnerTid)\
        EventEnabledInitMutexErrorEvent() ?\
        Template_spp(PHP_WincacheHandle, &InitMutexErrorEvent, Name, OwnerPid, OwnerTid)\
        : ERROR_SUCCESS\

//
// Enablement check macro for MemBlockNotInUse
//

#define EventEnabledMemBlockNotInUse() ((PHP_WincacheEnableBits[0] & 0x00000002) != 0)

//
// Event Macro for MemBlockNotInUse
//
#define EventWriteMemBlockNotInUse(Block, AllocContext, File, Line)\
        EventEnabledMemBlockNotInUse() ?\
        Template_ppsq(PHP_WincacheHandle, &MemBlockNotInUse, Block, AllocContext, File, Line)\
        : ERROR_SUCCESS\

//
// Enablement check macro for MemFreeAddrNotInSegment
//

#define EventEnabledMemFreeAddrNotInSegment() ((PHP_WincacheEnableBits[0] & 0x00000002) != 0)

//
// Event Macro for MemFreeAddrNotInSegment
//
#define EventWriteMemFreeAddrNotInSegment(Block, AllocContext, File, Line)\
        EventEnabledMemFreeAddrNotInSegment() ?\
        Template_ppsq(PHP_WincacheHandle, &MemFreeAddrNotInSegment, Block, AllocContext, File, Line)\
        : ERROR_SUCCESS\

//
// Enablement check macro for MemCombineNonFreeBlock
//

#define EventEnabledMemCombineNonFreeBlock() ((PHP_WincacheEnableBits[0] & 0x00000002) != 0)

//
// Event Macro for MemCombineNonFreeBlock
//
#define EventWriteMemCombineNonFreeBlock(Block, AllocContext, File, Line)\
        EventEnabledMemCombineNonFreeBlock() ?\
        Template_ppsq(PHP_WincacheHandle, &MemCombineNonFreeBlock, Block, AllocContext, File, Line)\
        : ERROR_SUCCESS\

//
// Enablement check macro for MemFreeListCorrupt
//

#define EventEnabledMemFreeListCorrupt() ((PHP_WincacheEnableBits[0] & 0x00000002) != 0)

//
// Event Macro for MemFreeListCorrupt
//
#define EventWriteMemFreeListCorrupt(Block, AllocContext, File, Line)\
        EventEnabledMemFreeListCorrupt() ?\
        Template_ppsq(PHP_WincacheHandle, &MemFreeListCorrupt, Block, AllocContext, File, Line)\
        : ERROR_SUCCESS\

//
// Enablement check macro for LockAbandonedMutex
//

#define EventEnabledLockAbandonedMutex() ((PHP_WincacheEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for LockAbandonedMutex
//
#define EventWriteLockAbandonedMutex(LockName, LastReaderPid, LastWriterPid, File, Line)\
        EventEnabledLockAbandonedMutex() ?\
        Template_sqqsq(PHP_WincacheHandle, &LockAbandonedMutex, LockName, LastReaderPid, LastWriterPid, File, Line)\
        : ERROR_SUCCESS\

//
// Enablement check macro for LockFailedWaitForLock
//

#define EventEnabledLockFailedWaitForLock() ((PHP_WincacheEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for LockFailedWaitForLock
//
#define EventWriteLockFailedWaitForLock(LockName, LastReaderPid, LastWriterPid, File, Line)\
        EventEnabledLockFailedWaitForLock() ?\
        Template_sqqsq(PHP_WincacheHandle, &LockFailedWaitForLock, LockName, LastReaderPid, LastWriterPid, File, Line)\
        : ERROR_SUCCESS\

//
// Enablement check macro for UnlockAbandonedMutex
//

#define EventEnabledUnlockAbandonedMutex() ((PHP_WincacheEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for UnlockAbandonedMutex
//
#define EventWriteUnlockAbandonedMutex(LockName, LastReaderPid, LastWriterPid, File, Line)\
        EventEnabledUnlockAbandonedMutex() ?\
        Template_sqqsq(PHP_WincacheHandle, &UnlockAbandonedMutex, LockName, LastReaderPid, LastWriterPid, File, Line)\
        : ERROR_SUCCESS\

//
// Enablement check macro for UnlockFailedWaitForLock
//

#define EventEnabledUnlockFailedWaitForLock() ((PHP_WincacheEnableBits[0] & 0x00000004) != 0)

//
// Event Macro for UnlockFailedWaitForLock
//
#define EventWriteUnlockFailedWaitForLock(LockName, LastReaderPid, LastWriterPid, File, Line)\
        EventEnabledUnlockFailedWaitForLock() ?\
        Template_sqqsq(PHP_WincacheHandle, &UnlockFailedWaitForLock, LockName, LastReaderPid, LastWriterPid, File, Line)\
        : ERROR_SUCCESS\

#endif // MCGEN_DISABLE_PROVIDER_CODE_GENERATION


//
// Allow Diasabling of code generation
//
#ifndef MCGEN_DISABLE_PROVIDER_CODE_GENERATION

//
// Template Functions 
//
//
//Template from manifest : ModuleInitErrorTemplate
//
#ifndef Template_x_def
#define Template_x_def
ETW_INLINE
ULONG
Template_x(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_ unsigned __int64  Error
    )
{
#define ARGUMENT_COUNT_x 1

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_x];

    EventDataDescCreate(&EventData[0], &Error, sizeof(unsigned __int64)  );

    return EventWrite(RegHandle, Descriptor, ARGUMENT_COUNT_x, EventData);
}
#endif

//
//Template from manifest : InitMutexErrorTemplate
//
#ifndef Template_spp_def
#define Template_spp_def
ETW_INLINE
ULONG
Template_spp(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ LPCSTR  Name,
    _In_opt_ const void *  OwnerPid,
    _In_opt_ const void *  OwnerTid
    )
{
#define ARGUMENT_COUNT_spp 3

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_spp];

    EventDataDescCreate(&EventData[0], 
                        (Name != NULL) ? Name : "NULL",
                        (Name != NULL) ? (ULONG)((strlen(Name) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[1], &OwnerPid, sizeof(PVOID)  );

    EventDataDescCreate(&EventData[2], &OwnerTid, sizeof(PVOID)  );

    return EventWrite(RegHandle, Descriptor, ARGUMENT_COUNT_spp, EventData);
}
#endif

//
//Template from manifest : MemAllocBlockTemplate
//
#ifndef Template_ppsq_def
#define Template_ppsq_def
ETW_INLINE
ULONG
Template_ppsq(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ const void *  Block,
    _In_opt_ const void *  AllocContext,
    _In_opt_ LPCSTR  File,
    _In_ const unsigned int  Line
    )
{
#define ARGUMENT_COUNT_ppsq 4

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_ppsq];

    EventDataDescCreate(&EventData[0], &Block, sizeof(PVOID)  );

    EventDataDescCreate(&EventData[1], &AllocContext, sizeof(PVOID)  );

    EventDataDescCreate(&EventData[2], 
                        (File != NULL) ? File : "NULL",
                        (File != NULL) ? (ULONG)((strlen(File) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[3], &Line, sizeof(const unsigned int)  );

    return EventWrite(RegHandle, Descriptor, ARGUMENT_COUNT_ppsq, EventData);
}
#endif

//
//Template from manifest : LockMutexTemplate
//
#ifndef Template_sqqsq_def
#define Template_sqqsq_def
ETW_INLINE
ULONG
Template_sqqsq(
    _In_ REGHANDLE RegHandle,
    _In_ PCEVENT_DESCRIPTOR Descriptor,
    _In_opt_ LPCSTR  LockName,
    _In_ const unsigned int  LastReaderPid,
    _In_ const unsigned int  LastWriterPid,
    _In_opt_ LPCSTR  File,
    _In_ const unsigned int  Line
    )
{
#define ARGUMENT_COUNT_sqqsq 5

    EVENT_DATA_DESCRIPTOR EventData[ARGUMENT_COUNT_sqqsq];

    EventDataDescCreate(&EventData[0], 
                        (LockName != NULL) ? LockName : "NULL",
                        (LockName != NULL) ? (ULONG)((strlen(LockName) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[1], &LastReaderPid, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[2], &LastWriterPid, sizeof(const unsigned int)  );

    EventDataDescCreate(&EventData[3], 
                        (File != NULL) ? File : "NULL",
                        (File != NULL) ? (ULONG)((strlen(File) + 1) * sizeof(CHAR)) : (ULONG)sizeof("NULL"));

    EventDataDescCreate(&EventData[4], &Line, sizeof(const unsigned int)  );

    return EventWrite(RegHandle, Descriptor, ARGUMENT_COUNT_sqqsq, EventData);
}
#endif

#endif // MCGEN_DISABLE_PROVIDER_CODE_GENERATION

#if defined(__cplusplus)
};
#endif

#define MSG_Provider_Name                    0x90000001L
#define MSG_Event_ModInitError               0xB0000001L
#define MSG_Event_InitMutexError             0xB0000002L
#define MSG_Event_MemBlockNotInUseError      0xB000000AL
#define MSG_Event_MemFreeAddrNotInSegmentError 0xB000000BL
#define MSG_Event_MemCombineNonFreeBlockError 0xB000000CL
#define MSG_Event_MemFreeListCorruptError    0xB000000DL
#define MSG_Event_LockAbandonedMutexError    0xB0000014L
#define MSG_Event_LockFailedWaitForLockError 0xB0000015L
#define MSG_Event_UnlockAbandonedMutexError  0xB0000016L
#define MSG_Event_UnlockFailedWaitForLockError 0xB0000017L
