/**
   Scim-Unikey Input Method
   Version: 0.2.0a

   Copyright (C) 2008-2009 Ubuntu-VN <http://www.ubuntu-vn.org>
   Author: Le Quoc Tuan <mr.lequoctuan@gmail.com>
   Home: http://scim-unikey.googlecode.com
   License: GNU LESSER GENERAL PUBLIC LICENSE v2.1
*/

#if !defined (__SCIM_UNIKEY_IMENGINE_CONST_H)
#define __SCIM_UNIKEY_IMENGINE_CONST_H

#define SCIM_UNIKEY_ICON_FILENAME			"/scim-unikey.png"
#define SCIM_UNIKEY_ICON_CONFIGURE			"/scim-unikey-configure.png"
#define SCIM_UNIKEY_ICON_CHECK				"/scim-unikey-check.png"

#define SCIM_IMENGINE_UNIKEY_MACROPATH			"/.scim/scim-unikey/macro"

/** define the config string off the unikey engine */
#define SCIM_IMENGINE_UNIKEY_PREEDIT			"/IMEngine/Unikey/preEdit"
#define SCIM_IMENGINE_UNIKEY_PREEDIT_SWITCH_KEY		"/IMEngine/Unikey/preEditSwitchKey"
#define SCIM_IMENGINE_UNIKEY_PROCESSWATWORDBEGIN	"/IMEngine/Unikey/processWAtBeginWord"
#define SCIM_IMENGINE_UNIKEY_CODERTELEX			"/IMEngine/Unikey/useCoderTelex"

#define SCIM_IMENGINE_UNIKEY_FREEMARKING		"/IMEngine/Unikey/freeMarking"
#define SCIM_IMENGINE_UNIKEY_MODERNSTYLE		"/IMEngine/Unikey/modernStyle"
#define SCIM_IMENGINE_UNIKEY_MACROENABLED		"/IMEngine/Unikey/macroEnabled"
#define SCIM_IMENGINE_UNIKEY_SPELLCHECKENABLED		"/IMEngine/Unikey/spellCheckEnabled"
#define SCIM_IMENGINE_UNIKEY_AUTONONVNRESTORE		"/IMEngine/Unikey/autoNonVnRestore"

#define SCIM_IMENGINE_UNIKEY_INPUTMETHOD		"/IMEngine/Unikey/InputMethod"
#define SCIM_IMENGINE_UNIKEY_OUTPUTCHARSET		"/IMEngine/Unikey/OutputCharset"

/** define the default value off the unikey engine */
#define SCIM_IMENGINE_UNIKEY_PROCESSWATWORDBEGIN_DEF	true
#define SCIM_IMENGINE_UNIKEY_CODERTELEX_DEF		false

#define SCIM_IMENGINE_UNIKEY_PREEDIT_DEF		true
#define SCIM_IMENGINE_UNIKEY_PREEDIT_SWITCH_KEY_DEF	"Control+Alt+backslash"
#define SCIM_IMENGINE_UNIKEY_FREEMARKING_DEF		true
#define SCIM_IMENGINE_UNIKEY_MODERNSTYLE_DEF		false
#define SCIM_IMENGINE_UNIKEY_MACROENABLED_DEF		false
#define SCIM_IMENGINE_UNIKEY_SPELLCHECKENABLED_DEF	true
#define SCIM_IMENGINE_UNIKEY_AUTONONVNRESTORE_DEF	true

#endif
