#include "GDT.h"

__attribute__((aligned(0x1000)))
GDT DefaultGDT = {
    {0, 0, 0, 0x00, 0x00, 0},
    {0, 0, 0, 0x9a, 0xa0, 0},
    {0, 0, 0, 0x92, 0xa0, 0},
    {0, 0, 0, 0x00, 0x00, 0},
    {0, 0, 0, 0x9a, 0xa0, 0},
    {0, 0, 0, 0x92, 0xa0, 0},
};

void InitGDT()
{
    static GDTDesc GDTDescriptor;
	GDTDescriptor.Size = sizeof(GDT) - 1;
	GDTDescriptor.Offset = (uint64_t)&DefaultGDT;
	LoadGDT(&GDTDescriptor);
}