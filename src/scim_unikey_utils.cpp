/**
   Scim-Unikey Input Method

   Copyright (C) 2008-2009 Ubuntu-VN <http://www.ubuntu-vn.org>
   Author: Le Quoc Tuan <mr.lequoctuan@gmail.com>
   Home: http://scim-unikey.googlecode.com
   License: GNU LESSER GENERAL PUBLIC LICENSE v2.1
*/

#include <scim.h>
#include "scim_unikey_const.h"

const char *getMacroFile()
{
	scim::String s;
	bool t;

	s = getenv("HOME");
	s.append(SCIM_IMENGINE_UNIKEY_MACROPATH);

	int k=s.length()-1;
	if ((s.at(0)=='\"' && s.at(k)=='\"'))
	{
		s.erase(k, 1);
		s.erase(0, 1);
	}

	return s.c_str();
}

// code from x-unikey, for convert charset that not is XUtf-8
int latinToUtf(unsigned char *dst, unsigned char *src, int inSize, int * pOutSize)
{
	int i;
	int outLeft;
	unsigned char ch;

	outLeft = *pOutSize;

	for (i=0; i<inSize; i++)
	{
		ch = *src++;
		if (ch < 0x80)
		{
			outLeft -= 1;
			if (outLeft >= 0)
				*dst++ = ch;
		}
		else
		{
			outLeft -= 2;
			if (outLeft >= 0)
			{
				*dst++ = (0xC0 | ch >> 6);
				*dst++ = (0x80 | (ch & 0x3F));
			}
		}
	}

	*pOutSize = outLeft;
	return (outLeft >= 0);
}
