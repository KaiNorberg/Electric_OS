#pragma once

struct PSF_HEADER
{
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charsize;
};

struct PSF_FONT
{
	PSF_HEADER* PSF_header;
	char* glyphBuffer;
};
