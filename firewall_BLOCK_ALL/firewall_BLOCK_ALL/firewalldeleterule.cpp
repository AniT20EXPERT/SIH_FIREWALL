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

int main()
{
    HANDLE engineHandle;
    DWORD result = FwpmEngineOpen(NULL, RPC_C_AUTHN_WINNT, NULL, NULL, &engineHandle);
    if (result != ERROR_SUCCESS)
    {
        printf("Failed to open WFP engine, error code: %lu\n", result);
        PrintError(result);
        return 1;
    }

    // Start a transaction
    result = FwpmTransactionBegin(engineHandle, 0);
    if (result != ERROR_SUCCESS)
    {
        printf("Failed to start transaction, error code: %lu\n", result);
        PrintError(result);
        FwpmEngineClose(engineHandle);
        return 1;
    }

    // Delete the existing filter
    result = FwpmFilterDeleteByKey(engineHandle, &filterGuid);
    if (result != ERROR_SUCCESS)
    {
        printf("Failed to delete filter, error code: %lu\n", result);
        PrintError(result);
        FwpmTransactionAbort(engineHandle);
        FwpmEngineClose(engineHandle);
        return 1;
    }

    // Commit the transaction
    result = FwpmTransactionCommit(engineHandle);
    if (result != ERROR_SUCCESS)
    {
        printf("Failed to commit transaction, error code: %lu\n", result);
        PrintError(result);
        FwpmTransactionAbort(engineHandle);
        FwpmEngineClose(engineHandle);
        return 1;
    }

    printf("Filter deleted successfully\n");

    // Clean up
    FwpmEngineClose(engineHandle);

    return 0;
}
