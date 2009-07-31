/**
   Scim-Unikey Input Method

   Copyright (C) 2008-2009 Ubuntu-VN <http://www.ubuntu-vn.org>
   Author: Le Quoc Tuan <mr.lequoctuan@gmail.com>
   Home: http://scim-unikey.googlecode.com
   License: GNU LESSER GENERAL PUBLIC LICENSE v2.1
*/

#define Uses_SCIM_IMENGINE
#define Uses_SCIM_ICONV
#define Uses_SCIM_CONFIG_BASE
#define Uses_SCIM_CONFIG_PATH

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <libintl.h>
#define _(String) dgettext(PACKAGE_NAME,String)

#include <scim.h>
#include "scim_unikey_const.h"
#include "im_engine.h"

#include <string.h>
#include "unikey.h"

#define scim_module_init                            unikey_LTX_scim_module_init
#define scim_module_exit                            unikey_LTX_scim_module_exit
#define scim_imengine_module_init                   unikey_LTX_scim_imengine_module_init
#define scim_imengine_module_create_factory         unikey_LTX_scim_imengine_module_create_factory

ConfigPointer   __config;

using namespace scim;

extern "C"
{
    void scim_module_init(void)
    {
        UnikeySetup();
    }

    void scim_module_exit(void)
    {
        UnikeyCleanup();
    }

    unsigned int scim_imengine_module_init(ConfigPointer &config)
    {
        if (config.null())
            return 0;
        __config = config;

        return 2;
    }

    IMEngineFactoryPointer scim_imengine_module_create_factory(unsigned int index)
    {
        return new UnikeyFactory(index);
    }
}

UnikeyFactory::UnikeyFactory(int id)
{
    m_id = id;
    set_languages("vi_VN");
}
UnikeyFactory::~UnikeyFactory()
{
}
WideString UnikeyFactory::get_name() const
{
    return utf8_mbstowcs(m_id==0?"Unikey":"Unikey Classic");
}
String UnikeyFactory::get_uuid() const
{
    return String("16ef5139-de02-494f-8d98-ddfcd60bbae1-") + String(m_id==0?"PREEDIT":"CLASSIC");
}
WideString UnikeyFactory::get_authors() const
{
    return utf8_mbstowcs("Le Quoc Tuan <mr.lequoctuan@gmail.com>");
}
WideString UnikeyFactory::get_credits() const
{
    return utf8_mbstowcs(String("Scim-Unikey Input Method\n"
                                "Version: " SCIM_UNIKEY_VERSION "\n"
                                "Copyright © 2008-2009 Ubuntu-VN\n"
                                "http://www.ubuntu-vn.org\n\n"
                                "Thanks to Pham Kim Long for ukengine")
        );
}
WideString UnikeyFactory::get_help() const
{
    return utf8_mbstowcs(_("This IME work best when environment variable are\n"
                           "- GTK_IM_MODULE=scim-bridge\n"
                           "- QT_IM_MODULE=xim\n\n"

                           "Some time, in some application, you \"must\"\n"
                           "use Preedit to input properly\n\n"

                           "In some application, scim can't enable,\n"
                           "this is not a bug of scim-unikey.\n"
                           "You can try this in terminal: app_name | scim\n\n"

                           "This IME come into being not intend to\n"
                           "replace scim-m17n, just a choose, I recommend\n"
                           "use this combine with scim-m17n\n\n"

                           "For other support goto:\n"
                           "  http://code.google.com/p/scim-unikey/wiki/Support"));
}
String UnikeyFactory::get_icon_file() const
{
    return String(SCIM_ICONDIR SCIM_UNIKEY_ICON_MAIN);
}

IMEngineInstancePointer UnikeyFactory::create_instance(const String & encoding, int id)
{
    if (m_id==0)
    {
        return new UnikeyInstancePreedit(this, encoding, id);
    }
    else
    {
        return new UnikeyInstanceClassic(this, encoding, id);
    }
}

