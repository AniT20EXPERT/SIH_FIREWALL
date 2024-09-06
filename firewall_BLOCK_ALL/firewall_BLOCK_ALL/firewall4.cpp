#include <Windows.h>
#include <fwpmu.h>
#include <fwptypes.h>
#include <iostream>

const GUID filterGuid = { 0x42acacee, 0x249, 0x40f1, { 0xa9, 0x49, 0xc, 0xa5, 0x38, 0x81, 0xf7, 0xbb } };

void PrintError(DWORD errorCode) {
    LPWSTR errorMessage = NULL;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        0,
        (LPWSTR)&errorMessage,
        0,
        NULL
    );
    wprintf(L"Error: %s\n", errorMessage);
    LocalFree(errorMessage);
}

int main() {
    HANDLE engineHandle;
    DWORD result = FwpmEngineOpen(NULL, RPC_C_AUTHN_WINNT, NULL, NULL, &engineHandle);
    if (result != ERROR_SUCCESS) {
        printf("Failed to open WFP engine, error code: %lu\n", result);
        PrintError(result);
        return 1;
    }

    result = FwpmTransactionBegin(engineHandle, 0);
    if (result != ERROR_SUCCESS) {
        printf("Failed to start transaction, error code: %lu\n", result);
        PrintError(result);
        FwpmEngineClose(engineHandle);
        return 1;
    }

    FWPM_FILTER filter = {};
    filter.filterKey = filterGuid;

    // Use wchar_t arrays and manually assign values
    wchar_t name[] = L"Block All ICMP Traffic";
    wchar_t description[] = L"A filter to block all inbound ICMP traffic";
    filter.displayData.name = name;
    filter.displayData.description = description;
    filter.flags = 0;

    // Specify the layer for the filter
    filter.layerKey = FWPM_LAYER_INBOUND_TRANSPORT_V4;

    // Define filter conditions to target ICMP traffic
    FWPM_FILTER_CONDITION condition[1];
    condition[0].fieldKey = FWPM_CONDITION_IP_PROTOCOL;
    condition[0].matchType = FWP_MATCH_EQUAL;
    condition[0].conditionValue.type = FWP_UINT8;
    condition[0].conditionValue.uint8 = IPPROTO_ICMP;
    filter.numFilterConditions = 1;
    filter.filterCondition = condition;

    filter.action.type = FWP_ACTION_BLOCK;

    result = FwpmFilterAdd(engineHandle, &filter, NULL, NULL);
    if (result != ERROR_SUCCESS) {
        printf("Failed to add filter, error code: %lu\n", result);
        PrintError(result);
        FwpmTransactionAbort(engineHandle);
        FwpmEngineClose(engineHandle);
        return 1;
    }

    result = FwpmTransactionCommit(engineHandle);
    if (result != ERROR_SUCCESS) {
        printf("Failed to commit transaction, error code: %lu\n", result);
        PrintError(result);
        FwpmTransactionAbort(engineHandle);
        FwpmEngineClose(engineHandle);
        return 1;
    }

    printf("Filter added successfully\n");

    getchar();

    FwpmEngineClose(engineHandle);

    return 0;
}
