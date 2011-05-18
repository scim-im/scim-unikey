/**
   Scim-Unikey Input Method

   Copyright (C) 2008-2009 Ubuntu-VN <http://www.ubuntu-vn.org>
   Author: Le Quoc Tuan <mr.lequoctuan@gmail.com>
   Home: http://scim-unikey.googlecode.com
   License: GNU GENERAL PUBLIC LICENSE v2
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define Uses_SCIM_CONFIG_BASE
#define Uses_SCIM_CONFIG_PATH
#define Uses_SCIM_MODULE
#define Uses_SCIM_IMENGINE_MODULE
#define Uses_SCIM_HELPER
#define Uses_STL_MAP

#include <libintl.h>
#define _(String) dgettext(PACKAGE_NAME,String)

#include <scim.h>
#include "scim_unikey_const.h"

#define scim_module_init                        unikey_helper_LTX_scim_module_init
#define scim_module_exit                        unikey_helper_LTX_scim_module_exit
#define scim_helper_module_number_of_helpers    unikey_helper_LTX_scim_helper_module_number_of_helpers
#define scim_helper_module_get_helper_info      unikey_helper_LTX_scim_helper_module_get_helper_info
#define scim_helper_module_run_helper           unikey_helper_LTX_scim_helper_module_run_helper

using namespace scim;

static HelperInfo __helper_info (String("d64561de-c638-427e-9f9c-dd84e15043ab"),
								 String(_("Unikey Setup")),
								 String(SCIM_ICONDIR)+String(SCIM_UNIKEY_ICON_MAIN),
								 String(""),
								 SCIM_HELPER_STAND_ALONE);

extern "C"
{
	void scim_module_init()
	{
	}

	void scim_module_exit()
	{
	}

	unsigned int scim_helper_module_number_of_helpers()
	{
		return 1;
	}

	bool scim_helper_module_get_helper_info(unsigned int idx, HelperInfo &info)
	{
		if (idx == 0)
		{
			info = __helper_info;
			return true;
		}
		return false;
	}

	void scim_helper_module_run_helper(const String &uuid, const ConfigPointer &config, const String &display)
	{
		if (uuid == "d64561de-c638-427e-9f9c-dd84e15043ab")
		{
			system(LIBEXECDIR "/scim-setup-unikey &");
		}
	}
}

