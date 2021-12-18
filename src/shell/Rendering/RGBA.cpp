#include "RGBA.h"

RGBA::RGBA(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
{
    this->A = A;
    this->R = R;
    this->G = G;
    this->B = B;
}

RGBA::RGBA(uint8_t T)
{    
    this->A = T;
    this->R = T;
    this->G = T;
    this->B = T;
}

uint32_t RGBA::ToInt()
{
    uint32_t A32 = A;
    uint32_t R32 = R;
    uint32_t G32 = G;
    uint32_t B32 = B;

    return B32 + (G32 << 8) + (R32 << 16) + (A32 << 24);
}