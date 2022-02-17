#pragma once

#include "Font.h"

#include <stdint.h>

namespace STL
{    
    extern uint8_t SelectedFont;

    void SetFonts(PSF_FONT** NewFonts, uint8_t NewFontAmount);

    const PSF_FONT* GetFont();
}