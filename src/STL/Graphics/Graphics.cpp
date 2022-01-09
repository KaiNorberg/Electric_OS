#include "Graphics.h"

namespace STL
{
    PSF_FONT* CurrentFont = nullptr;

    void SetFont(PSF_FONT* NewFont)
    {
        CurrentFont = NewFont;
    }

    const PSF_FONT* GetFont()
    {
        return CurrentFont;
    }
}