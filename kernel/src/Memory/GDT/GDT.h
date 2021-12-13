#pragma once

#include <stdint.h>

struct GDTDesc
{
    uint16_t Size;
    uint64_t Offset;
}__attribute__((packed)); //Dont add additional bytes

struct GDTEntry
{
    uint16_t Limit0;
    uint16_t Base0;
    uint8_t Base1;
    uint8_t AccessByte;
    uint8_t Limit1_Flags;
    uint8_t Base2;
}__attribute__((packed)); //Dont add additional bytes

struct GDT
{
    GDTEntry Null;
    GDTEntry KernelCode;
    GDTEntry KernelData;
    GDTEntry UserNull;
    GDTEntry UserCode;
    GDTEntry UserData;
}__attribute__((packed)) __attribute__((aligned(0x1000))); //Dont add additional bytes

extern GDT DefaultGDT;

extern "C" void LoadGDT(GDTDesc* GDTDescriptor);

void InitGDT();