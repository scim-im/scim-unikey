/**
   Scim-Unikey Input Method

   Copyright (C) 2008-2009 Ubuntu-VN <http://www.ubuntu-vn.org>
   Author: Le Quoc Tuan <mr.lequoctuan@gmail.com>
   Home: http://scim-unikey.googlecode.com
   License: GNU LESSER GENERAL PUBLIC LICENSE v2.1
*/

#if !defined (__SCIM_UNIKEY_IMENGINE_UTILS_H)
#define __SCIM_UNIKEY_IMENGINE_UTILS_H

const char *getMacroFile();
int latinToUtf(unsigned char *dst, unsigned char *src, int inSize, int * pOutSize);

#endif
