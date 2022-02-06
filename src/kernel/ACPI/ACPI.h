#pragma once

#include <stdint.h>

struct RSDP2
{
    uint8_t Signature[8];
    uint8_t CheckSum;
    uint8_t OEMID[6];
    uint8_t Revision;
    uint32_t RSDTAddress;
    uint32_t Length;
    uint64_t XSDTAddress;
    uint8_t ExtendedCheckSum;
    uint8_t Reserved[3];
} __attribute__((packed));

struct SDTHeader
{
    uint8_t Signature[4];
    uint32_t Length;
    uint8_t Revision;
    uint8_t CheckSum;
    uint8_t OEMID[6];
    uint8_t OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision; 
} __attribute__((packed));

struct MCFGHeader
{
    SDTHeader Header;
    uint64_t Reserved;
} __attribute__((packed));

namespace ACPI
{
    void Prepare(RSDP2* In_RSDP);

    void* FindTable(const char* Signature);
}