#pragma once

#include <stdint.h>

#define EFIAPI __attribute__((ms_abi))

enum class EfiResetType 
{
	Cold,
	Warm,
	Shutdown
};

struct EfiTableHeader 
{
	uint64_t Signature;
	uint32_t Rev;
	uint32_t Size;
	uint32_t Crc;
	uint32_t Reserved;
};

typedef uint64_t(EFIAPI *EfiResetSystem)
(EfiResetType, uint64_t, uint64_t, uint16_t*);

struct EfiRuntimeServices 
{
	EfiTableHeader Header;
	void* GetTime;
	void* SetTime;
	void* GetWakeupTime;
	void* SetWakeupTime;
	void* SetVirtualAddressMap;
	void* ConvertPointer;
	void* GetVariable;
	void* GetNextVariableName;
	void* SetVariable;
	void* GetNextHighMonotonicCount;
	EfiResetSystem ResetSystem;
	void* UpdateCapsule;
	void* QueryCapsuleCapabilities;
	void* QueryVariableInfo;
};

namespace UEFI
{
    void Init(EfiRuntimeServices* In_RT);

    EfiRuntimeServices* GetRT();    
}