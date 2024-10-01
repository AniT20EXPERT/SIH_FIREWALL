#include "Trigger.h"
#include "Config.h"
#include <fwpmk.h>
#include <stdio.h>

NTSTATUS WfpInit(PDRIVER_OBJECT driverObject)
{
	engineHandle = NULL;
	filterDeviceObject = NULL;

	NTSTATUS status = IoCreateDevice(driverObject, 0, NULL, FILE_DEVICE_UNKNOWN, 0, FALSE, &filterDeviceObject);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Failed to create the filter device object (0x%X).\n", status));
		return status;
	}

	status = FwpmEngineOpen(NULL, RPC_C_AUTHN_WINNT, NULL, NULL, &engineHandle);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Failed to open the filter engine (0x%X).\n", status));
		return status;
	}

	// register a callout to the filter engine
	status = CalloutRegister();
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Failed to register the filter callout (0x%X).\n", status));
		return status;
	}

	// add a callout to the system
	status = CalloutAdd();
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Failed to add the filter callout (0x%X)", status));
		return status;
	}

	// add a sublayer to the system
	status = SublayerAdd();
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Failed to add the sublayer (0x%X).\n", status));
		return status;
	}

	// add a filter rule to the added sublayer
	status = FilterAdd();
	if (!NT_SUCCESS(status))
	{
		KdPrint(("Failed to add the filter (0x%X).\n", status));
		return status;
	}

	KdPrint(("Hi!"));
	return TRUE;
}


NTSTATUS CalloutRegister()
{
	KdPrint(("Registering callout"));
	registerCalloutId = 0;
	FWPS_CALLOUT callout = {
		.calloutKey = CALLOUT_GUID,
		.flags = 0,
		.classifyFn = CalloutFilter,	// callout function to process network data (in this case ipv4 packets)
		.notifyFn = CalloutNotify,	// callout function that receives notifications from the filter engine
		.flowDeleteFn = NULL				// callout function used to process terminated data 
	};
	return FwpsCalloutRegister(filterDeviceObject, &callout, &registerCalloutId);
}

VOID CalloutFilter(
	const FWPS_INCOMING_VALUES* inFixedValues,
	const FWPS_INCOMING_METADATA_VALUES* inMetaValues,
	void* layerData,
	const void* classifyContext,
	const FWPS_FILTER* filter,
	UINT64 flowContext,
	FWPS_CLASSIFY_OUT* classifyOut
) {
	UNREFERENCED_PARAMETER(inFixedValues);
	UNREFERENCED_PARAMETER(inMetaValues);
	UNREFERENCED_PARAMETER(layerData);
	UNREFERENCED_PARAMETER(classifyContext);
	UNREFERENCED_PARAMETER(filter);
	UNREFERENCED_PARAMETER(flowContext);
	UNREFERENCED_PARAMETER(classifyOut);

	FWPS_STREAM_CALLOUT_IO_PACKET* packet;
	FWPS_STREAM_DATA* streamdata;
	UCHAR string[201] = { 0 };
	PUTF8_STRING string_utf8[201] = { 0 };
	PVOID length;
	SIZE_T bytes;
	ULONG LocalIP, RemoteIP;
	NTSTATUS result;
	packet = (FWPS_STREAM_CALLOUT_IO_PACKET*)layerData;
	streamdata = packet->streamData;
	
	RtlZeroMemory(classifyOut, sizeof(FWPS_CLASSIFY_OUT));

	LocalIP = inFixedValues->incomingValue[FWPS_FIELD_STREAM_PACKET_V4_IP_LOCAL_ADDRESS].value.uint16;
	RemoteIP = inFixedValues->incomingValue[FWPS_FIELD_STREAM_PACKET_V4_IP_REMOTE_ADDRESS].value.uint16;

	

	if ((streamdata->flags & FWPS_STREAM_FLAG_RECEIVE))
	{
		length = streamdata->dataLength <= 200 ? streamdata->dataLength : 200;
		FwpsCopyStreamDataToBuffer(streamdata->netBufferListChain->FirstNetBuffer, string, length, &bytes);
		result = RtlUnicodeStringToUTF8String(string_utf8, (PWCHAR)string, 0);

		KdPrint(("%x status", result));
		KdPrint(("data is %S\r\n. %d\r\n", (PWCHAR)string_utf8, (INT)streamdata->dataLength));
	}
	KdPrint(("Remote: %u.%u.%u.%u\r\n", (RemoteIP >> 24) & 0xFF, (RemoteIP >> 16) & 0xFF, (RemoteIP >> 8) & 0xFF, (RemoteIP) & 0xFF));
	packet->streamAction = FWPS_STREAM_ACTION_NONE;
	classifyOut->actionType = FWP_ACTION_PERMIT;
	if (filter->flags & FWPS_FILTER_FLAG_CLEAR_ACTION_RIGHT)
	{
		classifyOut->rights &= ~FWPS_RIGHT_ACTION_WRITE;
	}
}

NTSTATUS CalloutNotify(
	FWPS_CALLOUT_NOTIFY_TYPE notifyType,
	const GUID* filterKey,
	FWPS_FILTER* filter
)
{

	UNREFERENCED_PARAMETER(notifyType);
	UNREFERENCED_PARAMETER(filterKey);
	UNREFERENCED_PARAMETER(filter);

	return STATUS_SUCCESS;
}

NTSTATUS CalloutAdd()
{
	addCalloutId = 0;
	FWPM_CALLOUT callout = {
		.flags = 0,
		.displayData.name = L"Auth Connect Callout",
		.displayData.description = L"This is the callout for testing WFP code",
		.calloutKey = CALLOUT_GUID,
		.applicableLayer = FWPM_LAYER_STREAM_V6
	};

	return FwpmCalloutAdd(engineHandle, &callout, NULL, &addCalloutId);
}

NTSTATUS SublayerAdd()
{
	FWPM_SUBLAYER sublayer = {
		.displayData.name = L"ImpressionistsSublayer",
		.displayData.description = L"This is the sublayer for the filter",
		.subLayerKey = SUB_LAYER_GUID,
		.weight = 65535 // maximum priority, UINT16
	};
	return FwpmSubLayerAdd(engineHandle, &sublayer, NULL);
}

NTSTATUS FilterAdd()
{
	filterId = 0;
	UINT64 weightValue = 0xFFFFFFFFFFFFFFFF;
	FWP_VALUE weight = { .type = FWP_UINT64, .uint64 = &weightValue };
	FWPM_FILTER_CONDITION conditions[1] = { 0 }; //we want to process every packet

	// now we define our filter that will be added to the filterengine

	FWPM_FILTER filter = {
		.displayData.name = L"TheImpressionistsFilter",
		.displayData.description = L"This is the filter which will inspect packets",
		.layerKey = FWPM_LAYER_STREAM_V6,
		.subLayerKey = SUB_LAYER_GUID,
		.weight = weight,
		.numFilterConditions = 0, // we dont want any filtering right now
		.filterCondition = conditions, // we only want to inspect the packet
		.action.type = FWP_ACTION_CALLOUT_INSPECTION,
		.action.calloutKey = CALLOUT_GUID
	};
	return FwpmFilterAdd(engineHandle, &filter, NULL, &filterId);

	/*
	For our purposes, we have configured the type as FWP_ACTION_CALLOUT_INSPECTION,
	which just inspects incoming packets and forwards them to other filters in the chain.
	*/

	/*
	If you however wanted to outright permit or deny packets, you have the option of using
	the FWP_ACTION_PERMIT or FWP_ACTION_BLOCK flags.

	This we will use later when we pass on the packets that match malicious urls into
	another filter which will check which application has generated that package
	*/
}
// FWPM_LAYER_STREAM_V4
VOID Cleanup()
{
	TermCalloutIds();
	TermWfpEngine();
	TermFilterDeviceObject();
}

VOID TermCalloutIds()
{
	if (engineHandle)
	{
		if (filterId)
		{
			FwpmFilterDeleteById(engineHandle, filterId);
			FwpmSubLayerDeleteByKey(engineHandle, &SUB_LAYER_GUID);
			filterId = 0;
		}

		if (addCalloutId)
		{
			FwpmCalloutDeleteById(engineHandle, addCalloutId);
			addCalloutId = 0;
		}

		if (registerCalloutId)
		{
			FwpsCalloutUnregisterById(registerCalloutId);
			registerCalloutId = 0;
		}
	}
}

VOID TermWfpEngine()
{
	if (engineHandle)
	{
		FwpmEngineClose(engineHandle);
		engineHandle = NULL;
	}
}

VOID TermFilterDeviceObject()
{
	if (filterDeviceObject)
	{
		IoDeleteDevice(filterDeviceObject);
		filterDeviceObject = NULL;
	}
}