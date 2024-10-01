#pragma once
// Network driver headers to abstract network hardware from network drivers
// Specifies a standard interface between layered network drivers
#define NDIS630

#include <ndis.h>
#include <ndis/nblapi.h>
// WFP headers
#include <fwpsk.h>
#include <fwpmk.h>
#include <fwpmu.h>

// GUID headers
#define INITGUID
#include <guiddef.h>

DEFINE_GUID(CALLOUT_GUID, 0x8aadb11d, 0xe10e, 0x480d, 0xa7, 0x69, 0x61, 0xdb, 0xcc, 0x86, 0x58, 0xe6);
DEFINE_GUID(SUB_LAYER_GUID, 0xa7e76cdd, 0x5b4f, 0x4ffd, 0xa9, 0x9d, 0xf5, 0x69, 0x91, 0x17, 0x56, 0xe7);

// Sublayers are like containers holding multiple filtering rules within a single layer

PDEVICE_OBJECT	filterDeviceObject;
HANDLE			engineHandle;
UINT32			registerCalloutId;
UINT32			addCalloutId;
UINT64			filterId;


NTSTATUS WfpInit(PDRIVER_OBJECT driverObject);

NTSTATUS CalloutRegister();
NTSTATUS CalloutAdd();

VOID CalloutFilter(const FWPS_INCOMING_VALUES* inFixedValues, const FWPS_INCOMING_METADATA_VALUES* inMetaValues,
	void* layerData, const void* classifyContext, const FWPS_FILTER* filter,
	UINT64 flowContext, FWPS_CLASSIFY_OUT* classifyOut);
NTSTATUS CalloutNotify(FWPS_CALLOUT_NOTIFY_TYPE notifyType, const GUID* filterKey, FWPS_FILTER* filter);

NTSTATUS SublayerAdd();
NTSTATUS FilterAdd();

VOID TermFilterDeviceObject();
VOID TermCalloutIds();
VOID TermWfpEngine();
VOID Cleanup();

