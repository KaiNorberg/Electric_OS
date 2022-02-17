#include "Graphics.h"

namespace STL
{
    PSF_FONT** Fonts;
    uint8_t FontAmount;
    uint8_t SelectedFont = 0;

    void SetFonts(PSF_FONT** NewFonts, uint8_t NewFontAmount)
    {
        Fonts = NewFonts;
        FontAmount = NewFontAmount;
    }

    const PSF_FONT* GetFont()
    {
        if (SelectedFont >= FontAmount)
        {
            SelectedFont = 0;
        }

        return Fonts[SelectedFont];
    }
}