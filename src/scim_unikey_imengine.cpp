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
#include "scim_unikey_utils.h"
#include "scim_unikey_imengine.h"

#include <string.h>
#include "unikey.h"
#include "vnconv.h"

#define scim_module_init                            unikey_LTX_scim_module_init
#define scim_module_exit                            unikey_LTX_scim_module_exit
#define scim_imengine_module_init                   unikey_LTX_scim_imengine_module_init
#define scim_imengine_module_create_factory         unikey_LTX_scim_imengine_module_create_factory

static unsigned char WordBreakSyms[] =
{
    ',', ';', ':', '.', '\"', '\'', '!', '?', ' ',
    '<', '>', '=', '+', '-', '*', '/', '\\',
    '_', '~', '`', '@', '#', '$', '%', '^', '&', '(', ')', '{', '}', '[', ']',
    '|'
};

static unsigned char WordAutoCommit[] =
{
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'b', 'c', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n',
    'p', 'q', 'r', 's', 't', 'v', 'x', 'z',
    'B', 'C', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N',
    'P', 'Q', 'R', 'S', 'T', 'V', 'X', 'Z'
};

const String          Unikey_IMNames[]    = {"Telex", "Vni", "STelex", "STelex2"};
const UkInputMethod   Unikey_IM[]         = {UkTelex, UkVni, UkSimpleTelex, UkSimpleTelex2};
const unsigned int    NUM_INPUTMETHOD     = sizeof(Unikey_IM)/sizeof(Unikey_IM[0]);

const String          Unikey_OCNames[]    = {"Unicode",
                                             "TCVN3",
                                             "VNI Win",
                                             "VIQR",
                                             "CString",
                                             "NCR Decimal",
                                             "NCR Hex"};

const unsigned int    Unikey_OC[]         = {CONV_CHARSET_XUTF8,
                                             CONV_CHARSET_TCVN3,
                                             CONV_CHARSET_VNIWIN,
                                             CONV_CHARSET_VIQR,
                                             CONV_CHARSET_UNI_CSTRING,
                                             CONV_CHARSET_UNIREF,
                                             CONV_CHARSET_UNIREF_HEX};
const unsigned int    NUM_OUTPUTCHARSET   = sizeof(Unikey_OC)/sizeof(Unikey_OC[0]);

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

        return 1;
    }

    IMEngineFactoryPointer scim_imengine_module_create_factory(unsigned int index)
    {
        return new UnikeyFactory();
    }
}

UnikeyFactory::UnikeyFactory()
{
    set_languages("vi_VN");
}
UnikeyFactory::~UnikeyFactory()
{
}
WideString UnikeyFactory::get_name() const
{
    return utf8_mbstowcs("Unikey");
}
String UnikeyFactory::get_uuid() const
{
    return String(String("16ef5139-de02-494f-8d98-ddfcd60bbae1"));
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
                           "- QT_IM_MODULE=xim\n"
                           " If you use preedit, you don't not need that\n\n"
                           
                           "Some time, in some application, you \"must\"\n"
                           "enable Preedit to type best\n\n"

                           "In some application, scim can't enable,\n"
                           "this is not a bug of scim-unikey.\n"
                           "You can try this in terminal: app_name | scim\n\n"

                           "This IME come into being not intend to\n"
                           "replace scim-m17n, just a choose, I recommend\n"
                           "use this combine with scim-m17n\n\n"

                           "For other support goto:\n"
                           "  http://forum.ubuntu-vn.org/viewforum.php?f=85"));
}
String UnikeyFactory::get_icon_file() const
{
    return String(SCIM_ICONDIR SCIM_UNIKEY_ICON_FILENAME);
}

IMEngineInstancePointer UnikeyFactory::create_instance(const String & encoding, int id)
{
    return new UnikeyInstance(this, encoding, id);
}

UnikeyInstance::UnikeyInstance(UnikeyFactory *factory, const String &encoding, int id)
    :IMEngineInstanceBase(factory, encoding, id),
     m_auto_commit(false)
{
    static bool t, o;  //temp variable

    m_preeditstring.clear();

    CreateDefaultUnikeyOptions(&m_ukopt);

    /* Read config
       if can't read config, set it to default value*/

    t = __config->read(SCIM_IMENGINE_UNIKEY_INPUTMETHOD, &m_im);
    if (!t) m_im = 0;

    t = __config->read(SCIM_IMENGINE_UNIKEY_OUTPUTCHARSET, &m_oc);
    if (!t) m_oc = 0;

    t = __config->read(SCIM_IMENGINE_UNIKEY_PREEDIT, &m_preedit);
    if (!t) m_preedit = SCIM_IMENGINE_UNIKEY_PREEDIT_DEF;

    // read preedit switch key
    m_preeditskey.clear();
    String s;
    int i = 0, j;
    t = __config->read(SCIM_IMENGINE_UNIKEY_PREEDIT_SWITCH_KEY, &s);
    if (!t) s=SCIM_IMENGINE_UNIKEY_PREEDIT_SWITCH_KEY_DEF;
    if (s.length())
    {
        while (true)
        {
            j=s.find(',', i);
            if (j!=String::npos)
                m_preeditskey.push_back(KeyEvent(s.substr(i, j-i)));
            else
                break;
            i = j+1;
        }
        m_preeditskey.push_back(KeyEvent(s.substr(i)));
    } // end read preedit switch key

    t = __config->read(SCIM_IMENGINE_UNIKEY_PROCESSWATWORDBEGIN, &o);
    m_process_w_AtBeginWord = t?o:SCIM_IMENGINE_UNIKEY_PROCESSWATWORDBEGIN_DEF;

// Unikey Options
    t = __config->read(SCIM_IMENGINE_UNIKEY_FREEMARKING, &o);
    m_ukopt.freeMarking = t?o:SCIM_IMENGINE_UNIKEY_FREEMARKING_DEF;

    t = __config->read(SCIM_IMENGINE_UNIKEY_MODERNSTYLE, &o);
    m_ukopt.modernStyle = t?o:SCIM_IMENGINE_UNIKEY_MODERNSTYLE_DEF;

    t = __config->read(SCIM_IMENGINE_UNIKEY_MACROENABLED, &o);
    m_ukopt.macroEnabled = t?o:SCIM_IMENGINE_UNIKEY_MACROENABLED_DEF;

    t = __config->read(SCIM_IMENGINE_UNIKEY_SPELLCHECKENABLED, &o);
    m_ukopt.spellCheckEnabled = t?o:SCIM_IMENGINE_UNIKEY_SPELLCHECKENABLED_DEF;

    t = __config->read(SCIM_IMENGINE_UNIKEY_AUTONONVNRESTORE, &o);
    m_ukopt.autoNonVnRestore = t?o:SCIM_IMENGINE_UNIKEY_AUTONONVNRESTORE_DEF;

    UnikeySetOptions(&m_ukopt);

    if (m_ukopt.macroEnabled)
    {
        UnikeyLoadMacroTable(getMacroFile());
    }

    reset();
}

UnikeyInstance::~UnikeyInstance()
{
}

void UnikeyInstance::focus_in()
{
    register_properties(CreatePropertyList());

    UnikeySetInputMethod(Unikey_IM[m_im]);
    UnikeySetOutputCharset(Unikey_OC[m_oc]);
    UnikeySetOptions(&m_ukopt);
}

void UnikeyInstance::focus_out()
{
    reset();
}

void UnikeyInstance::reset()
{
    UnikeyResetBuf();

    m_lastkey_with_shift = false;

    if (m_preedit)
    {
        if (m_preeditstring.length())
        {
            commit_string(m_preeditstring);
            hide_preedit_string();
            m_preeditstring.clear();
        }
        m_auto_commit = false;
    }
}

void UnikeyInstance::Unikey_send_backspace(int nBackspace)
{
    static WideString ws;
    static int n;

    //  if surrounding text was provided, use it instead of send backspace
    if (get_surrounding_text(ws, n, nBackspace, 0))
    {
        // for type in Auto Complete in OpenOffice
        // Hope this not rise bugs in others application
        // not use SCIM_KEY_NullKey, because GTK application crash when GTK_IM_MODULE=scim

        forward_key_event(SCIM_KEY_VoidSymbol);

        delete_surrounding_text(-ws.length(), ws.length());
    }
    else
    {
        for (int i=0; i < nBackspace; i++)
            forward_key_event(SCIM_KEY_BackSpace);
    }
}

void UnikeyInstance::Unikey_update_preedit_string(const WideString s, const bool visible)
{
    AttributeList attlist;
    Attribute att;

    // underline preedit string
    att = Attribute(0, s.length(), SCIM_ATTR_DECORATE, SCIM_ATTR_DECORATE_UNDERLINE);
    attlist.push_back(att);

    if (m_ukopt.spellCheckEnabled==1 && UnikeyLastWordIsNonVn())
    {
        // red preedit string
        att = Attribute(0, s.length(), SCIM_ATTR_FOREGROUND, 0xff0000);
        attlist.push_back(att);
    }

    update_preedit_string(s, attlist);
    update_preedit_caret(s.length());

    if (visible == true)
    {
        show_preedit_string();
    }
    else
    {
        hide_preedit_string();
    }
}

bool UnikeyInstance::process_key_event(const KeyEvent& key)
{
    bool tmp;

    for (int i = 0; i<m_preeditskey.size(); i++)
    {
        if (key==m_preeditskey.at(i))
        {
            reset();
            m_preedit=!m_preedit;
            register_properties(CreatePropertyList());
            return true;
        }
    }

    tmp = m_preedit?Unikey_process_key_event_preedit(key)
        :Unikey_process_key_event_direct(key);

    if ((key.code >= SCIM_KEY_space && key.code <= SCIM_KEY_asciitilde)
        || (key.code >= SCIM_KEY_KP_Multiply && key.code <= SCIM_KEY_KP_9))
    {
        m_lastkey_with_shift = key.is_shift_down();
    }
    else
    {
        m_lastkey_with_shift = false;
    }

    return tmp;
}

/** This method not use preedit string */
bool UnikeyInstance::Unikey_process_key_event_direct(const KeyEvent& key)
{
    if (key.is_key_release())
    {
        return false;
    }

    if (key.is_control_down() || key.mask & SCIM_KEY_AltMask)
    {
        reset();
        return false;
    }

    if (key.code >= SCIM_KEY_Shift_L && key.code <= SCIM_KEY_Hyper_R)
    {
        return false;
    }

    if (key.code == SCIM_KEY_BackSpace)
    {
        UnikeyBackspacePress();     // xu ly phim backspace
        if (UnikeyBackspaces==0)    // neu ukengine bao khong can xoa ky tu nao,
            return false;  // thi tra lai backspace cho chuong trinh khach
        else
        {
            Unikey_send_backspace(UnikeyBackspaces);

            // append key that need change tone pos after press backspace
            if (UnikeyBufChars)
            {
                if (Unikey_OC[m_oc] == CONV_CHARSET_XUTF8)
                    commit_string(utf8_mbstowcs((const char*)UnikeyBuf, UnikeyBufChars));  // send the solved string to client
                else
                {
                    static unsigned char buf[1024];
                    int bufSize=sizeof(buf)/sizeof(buf[0]);

                    latinToUtf(buf, UnikeyBuf, UnikeyBufChars, &bufSize);
                    commit_string(utf8_mbstowcs((const char*)buf, sizeof(buf)/sizeof(buf[0]) - bufSize));
                }
            }

            return true;
        }
    }

    if (   (key.code >= SCIM_KEY_space && key.code <= SCIM_KEY_asciitilde)
           || (key.code >= SCIM_KEY_KP_Multiply && key.code <= SCIM_KEY_KP_9)
        )
    {
        UnikeySetCapsState(key.mask & SCIM_KEY_ShiftMask,
                           key.mask & SCIM_KEY_CapsLockMask);

        // shift + space to restore keystroke
        if (m_lastkey_with_shift == false
            && key.mask & SCIM_KEY_ShiftMask
            && key.code == SCIM_KEY_space
            && !UnikeyAtWordBeginning())
        {
            UnikeyRestoreKeyStrokes();
            if (UnikeyBackspaces == 0)
            {
                UnikeyPutChar(key.code);
            }
        }

        else if ((key.code >= SCIM_KEY_KP_0 &&
                  key.code <= SCIM_KEY_KP_9))
        {
            UnikeyPutChar(key.code);
        }

        else if ((Unikey_IM[m_im] == UkTelex || Unikey_IM[m_im] == UkSimpleTelex2)
                 && m_process_w_AtBeginWord == false
                 && UnikeyAtWordBeginning()
                 && (key.code == SCIM_KEY_w || key.code == SCIM_KEY_W))
        {
            UnikeyPutChar(key.code);
        }

        else
        {
            UnikeyFilter(key.code);
        }

        if (UnikeyBackspaces)
        {
            Unikey_send_backspace(UnikeyBackspaces);
        }

        if (UnikeyBufChars)
        {
            if (Unikey_OC[m_oc] == CONV_CHARSET_XUTF8)
            {
                commit_string(utf8_mbstowcs((const char*)UnikeyBuf, UnikeyBufChars));  // send the solved string to client
            }
            else
            {
                static unsigned char buf[1024];
                int bufSize=sizeof(buf)/sizeof(buf[0]);

                latinToUtf(buf, UnikeyBuf, UnikeyBufChars, &bufSize);
                commit_string(utf8_mbstowcs((const char*)buf, sizeof(buf)/sizeof(buf[0]) - bufSize));
            }
        }
        else
        {
            forward_key_event(key);
            return true;
        }

        return true;
    }

    // else
    reset();
    return false;
}

/** This method use preedit string */
bool UnikeyInstance::Unikey_process_key_event_preedit(const KeyEvent& key)
{
    static int i;

    if (key.code == SCIM_KEY_Tab
        || key.mask & SCIM_KEY_ControlMask
        || key.mask & SCIM_KEY_AltMask)
    {
        if (m_preeditstring.length())
        {
            commit_string(m_preeditstring);
            hide_preedit_string();
            m_preeditstring.clear();
        }
        reset();
        return false;
    }

    if (key.is_key_release())
        return false;

    if (key.code == SCIM_KEY_Tab
             || key.code == SCIM_KEY_Return
             || key.code == SCIM_KEY_Delete
             || key.code == SCIM_KEY_KP_Enter
             || (key.code >= SCIM_KEY_Home && key.code <= SCIM_KEY_Insert)
             || (key.code >= SCIM_KEY_KP_Home && key.code <= SCIM_KEY_KP_Delete))
    {
        if (m_preeditstring.length())
        {
            commit_string(m_preeditstring);
            hide_preedit_string();
            m_preeditstring.clear();
        }

        reset();
        return false;
    }

    else if (key.code >= SCIM_KEY_Shift_L && key.code <= SCIM_KEY_Hyper_R)
        return false;

    else if (key.code == SCIM_KEY_BackSpace)
    {
        UnikeyBackspacePress();     // process Backspace

        if (UnikeyBackspaces==0 || m_preeditstring.empty())    // if Unikey tell no Backspace
        {
            reset();
            return false;  // Return Backspace to client
        }
        else
        {
            static int len;

            len = m_preeditstring.length();
            if (len <= UnikeyBackspaces)
            {
                m_preeditstring.clear();
                hide_preedit_string();
                m_auto_commit = true;
            }
            else
            {
                m_preeditstring.erase(len - UnikeyBackspaces, UnikeyBackspaces);
                Unikey_update_preedit_string(m_preeditstring, true);
            }

            // append key that need change tone pos after press backspace
            if (UnikeyBufChars)
            {
                if (Unikey_OC[m_oc] == CONV_CHARSET_XUTF8)
                    m_preeditstring.append(utf8_mbstowcs((const char*)UnikeyBuf, UnikeyBufChars));
                else
                {
                    static unsigned char buf[1024];
                    int bufSize=sizeof(buf)/sizeof(buf[0]);

                    latinToUtf(buf, UnikeyBuf, UnikeyBufChars, &bufSize);
                    m_preeditstring.append(utf8_mbstowcs((const char*)buf, sizeof(buf)/sizeof(buf[0])-bufSize));
                }

                m_auto_commit = false;
                Unikey_update_preedit_string(m_preeditstring, true);
            }
        }

        return true;
    }

    else if (   (key.code >= SCIM_KEY_space && key.code <= SCIM_KEY_asciitilde)
                || (key.code >= SCIM_KEY_KP_Multiply && key.code <= SCIM_KEY_KP_9)
        )
    {
        UnikeySetCapsState(key.mask & SCIM_KEY_ShiftMask, key.mask & SCIM_KEY_CapsLockMask);

        // auto commit char: commit char that never change later in preedit string
        // if macro enabled, then not auto commit. Because macro may change any word
        if (m_ukopt.macroEnabled==0 && (UnikeyAtWordBeginning() || m_auto_commit))
        {
            for (i = 0; i < sizeof(WordAutoCommit); i++)
                if (key.code == WordAutoCommit[i])
                {
                    UnikeyPutChar(key.code);
                    m_auto_commit = true;
                    forward_key_event(key); // forward keyevent instead of return false
                    return true;
                }
        } // end auto commit char

        if ((Unikey_IM[m_im] == UkTelex || Unikey_IM[m_im] == UkSimpleTelex2)
                 && m_process_w_AtBeginWord == false
                 && UnikeyAtWordBeginning()
                 && (key.code == SCIM_KEY_w || key.code == SCIM_KEY_W))
        {
            UnikeyPutChar(key.code);
            if (m_ukopt.macroEnabled==0)
            {
                forward_key_event(key);
            }
            else
            {
                m_preeditstring.push_back(key.code);
                Unikey_update_preedit_string(m_preeditstring, true);
            }

            m_auto_commit = true;
            
            return true;
        }

        m_auto_commit = false;

        if (m_lastkey_with_shift == false
            && key.mask & SCIM_KEY_ShiftMask
            && key.code == SCIM_KEY_space
            && !UnikeyAtWordBeginning())
        {
            UnikeyRestoreKeyStrokes();
        }

        else if (key.code >= SCIM_KEY_KP_Multiply && key.code <= SCIM_KEY_KP_9)
        {
            UnikeyPutChar(key.code);
        }

        else
        {
            UnikeyFilter(key.code);
        }

        if (UnikeyBackspaces > 0)
        {
            static int len;

            len = m_preeditstring.length();

            if (len <= UnikeyBackspaces)
                m_preeditstring.clear();
            else
                m_preeditstring.erase(len - UnikeyBackspaces, UnikeyBackspaces);
        }

        if (UnikeyBufChars > 0)
        {
            if (Unikey_OC[m_oc] == CONV_CHARSET_XUTF8)
                m_preeditstring.append(utf8_mbstowcs((const char*)UnikeyBuf, UnikeyBufChars));
            else
            {
                static unsigned char buf[1024];
                int bufSize=sizeof(buf)/sizeof(buf[0]);

                latinToUtf(buf, UnikeyBuf, UnikeyBufChars, &bufSize);
                m_preeditstring.append(utf8_mbstowcs((const char*)buf, sizeof(buf)/sizeof(buf[0])-bufSize));
            }
        }
        else
        {    
            m_preeditstring.push_back(key.get_unicode_code());
        }

        if (m_preeditstring.length())
        {
            if (key.code >= SCIM_KEY_KP_Multiply && key.code <= SCIM_KEY_KP_9)
            {
                commit_string(m_preeditstring);
                hide_preedit_string();
                m_preeditstring.clear();
                reset();
                return true;
            }
            else
            {
                for (i=0; i < sizeof(WordBreakSyms); i++)
                    if (WordBreakSyms[i] == m_preeditstring[m_preeditstring.length()-1] && key.code == WordBreakSyms[i])
                    {
                        commit_string(m_preeditstring);
                        hide_preedit_string();
                        m_preeditstring.clear();
                        reset();
                        return true;
                    }
            }
        }

        Unikey_update_preedit_string(m_preeditstring, true);
        return true;
    }

    // else (key non process)
    reset();
    return false;
}

PropertyList UnikeyInstance::CreatePropertyList()
{
    PropertyList props;
    int i;

    Property prop("", "", "", "");

// input method
    prop.set_key("/Unikey/InputMethod");
    prop.set_label(Unikey_IMNames[m_im]);
    prop.set_icon("");
    prop.set_tip(_("Choose input method"));
    props.push_back(prop);

    for (i=0; i<NUM_INPUTMETHOD; i++)
    {
        prop.set_key(String("/Unikey/InputMethod/") + Unikey_IMNames[i] + String(m_im==i?"-Checked":""));
        prop.set_label(Unikey_IMNames[i]);
        prop.set_icon(m_im==i?SCIM_ICONDIR SCIM_UNIKEY_ICON_CHECK:"");
        prop.set_tip("");
        props.push_back(prop);
    }

// output charset

    prop.set_key("/Unikey/OutputCharset");
    prop.set_label(Unikey_OCNames[m_oc]);
    prop.set_icon("");
    prop.set_tip(_("Choose output charset"));
    props.push_back(prop);

    for (i=0; i<NUM_OUTPUTCHARSET; i++)
    {
        prop.set_key(String("/Unikey/OutputCharset/") + Unikey_OCNames[i] + String(m_oc==i?"-Checked":""));
        prop.set_label(Unikey_OCNames[i]);
        prop.set_icon(m_oc==i?SCIM_ICONDIR SCIM_UNIKEY_ICON_CHECK:"");
        prop.set_tip("");
        props.push_back(prop);
    }

// unikey options
    // menu
    prop.set_key("/Unikey/Options");
    prop.set_label(_("Options"));
    prop.set_icon(SCIM_ICONDIR SCIM_UNIKEY_ICON_CONFIGURE);
    prop.set_tip(_("Configure Unikey here"));
    props.push_back(prop);

    // spellcheck
    prop.set_key(m_ukopt.spellCheckEnabled?
                 "/Unikey/Options/SpellCheck/Disable":"/Unikey/Options/SpellCheck/Enable");
    prop.set_label(_("Enable spell check"));
    prop.set_icon(m_ukopt.spellCheckEnabled?SCIM_ICONDIR SCIM_UNIKEY_ICON_CHECK:"");
    prop.set_tip(_("If enable, you can decrease mistake when typing"));
    props.push_back(prop);
    
    // auto restore keystroke
    prop.set_key(m_ukopt.autoNonVnRestore?
                 "/Unikey/Options/AutoRestoreKeys/Disable":"/Unikey/Options/AutoRestoreKeys/Enable");
    prop.set_label(_("Auto restore keys with invalid words"));
    prop.set_icon(m_ukopt.autoNonVnRestore?SCIM_ICONDIR SCIM_UNIKEY_ICON_CHECK:"");
    prop.set_tip(_("When typing a word not in Vietnamese,\n"
                   "it will auto restore keystroke into orginal"));
    props.push_back(prop);

    // modern style
    prop.set_key(m_ukopt.modernStyle?
                 "/Unikey/Options/ModernStyle/Disable":"/Unikey/Options/ModernStyle/Enable");
    prop.set_label(_("Use oà, uý (instead of òa, úy)"));
    prop.set_icon(m_ukopt.modernStyle?SCIM_ICONDIR SCIM_UNIKEY_ICON_CHECK:"");
    prop.set_tip("");
    props.push_back(prop);

    // freeMarking
    prop.set_key(m_ukopt.freeMarking?
                 "/Unikey/Options/FreeMarking/Disable":"/Unikey/Options/FreeMarking/Enable");
    prop.set_label(_("Allow type with more freedom"));
    prop.set_icon(m_ukopt.freeMarking?SCIM_ICONDIR SCIM_UNIKEY_ICON_CHECK:"");
    prop.set_tip("");
    props.push_back(prop);

    // macro
    prop.set_key(m_ukopt.macroEnabled?
                 "/Unikey/Options/EnabledMacro/Disable":"/Unikey/Options/EnabledMacro/Enable");
    prop.set_label(_("Enable Macro"));
    prop.set_icon(m_ukopt.macroEnabled?SCIM_ICONDIR SCIM_UNIKEY_ICON_CHECK:"");
    prop.set_tip("");
    props.push_back(prop);

    // preedit
    prop.set_key(m_preedit?"/Unikey/Options/Preedit/Disable":"/Unikey/Options/Preedit/Enable");
    prop.set_label(_("Enable PreEdit"));
    prop.set_icon(m_preedit?SCIM_ICONDIR SCIM_UNIKEY_ICON_CHECK:"");
    prop.set_tip(_("This option is best for most application\n"
                   "But you may don't like it because it have an underline when typing"));
    props.push_back(prop);

    // process w at begin
    prop.set_key(m_process_w_AtBeginWord?
                 "/Unikey/Options/ProcessWAtBegin/Disable":"/Unikey/Options/ProcessWAtBegin/Enable");
    prop.set_label(_("Process W at word begin"));
    prop.set_icon(m_process_w_AtBeginWord?SCIM_ICONDIR SCIM_UNIKEY_ICON_CHECK:"");
    prop.set_tip(_("If enable, type W at begin\n"
                   "of word will change to Ư."));
    props.push_back(prop);

    return props;
}

void UnikeyInstance::trigger_property(const String &property)
{
    bool change = false;
    int i;

// input method
    if (!property.compare(0, strlen("/Unikey/InputMethod/"), "/Unikey/InputMethod/"))
    {
        for (i=0; i<NUM_INPUTMETHOD; i++)
            if (!property.compare(strlen("/Unikey/InputMethod/"),
                                  property.length() - strlen("/Unikey/InputMethod/"),
                                  Unikey_IMNames[i]))
            {
                m_im = i;
                __config->write(SCIM_IMENGINE_UNIKEY_INPUTMETHOD, m_im);
                change = true;
                break;
            }
    }

// output charset
    else if (!property.compare(0, strlen("/Unikey/OutputCharset/"), "/Unikey/OutputCharset/"))
    {
        for (i=0; i<NUM_OUTPUTCHARSET; i++)
            if (!property.compare(strlen("/Unikey/OutputCharset/"),
                                  property.length() - strlen("/Unikey/OutputCharset/"),
                                  Unikey_OCNames[i]))
            {
                m_oc = i;
                __config->write(SCIM_IMENGINE_UNIKEY_OUTPUTCHARSET, m_oc);
                change = true;
                break;
            }
    }

// spellcheck
    else if (property == "/Unikey/Options/SpellCheck/Enable")
    {
        m_ukopt.spellCheckEnabled = true;
        __config->write(SCIM_IMENGINE_UNIKEY_SPELLCHECKENABLED, true);
        change = true;
    }
    else if (property == "/Unikey/Options/SpellCheck/Disable")
    {
        m_ukopt.spellCheckEnabled = false;
        __config->write(SCIM_IMENGINE_UNIKEY_SPELLCHECKENABLED, false);
        change = true;
    }

// auto restore keystroke
    else if (property == "/Unikey/Options/AutoRestoreKeys/Enable")
    {
        m_ukopt.autoNonVnRestore = true;
        __config->write(SCIM_IMENGINE_UNIKEY_AUTONONVNRESTORE, true);
        change = true;
    }
    else if (property == "/Unikey/Options/AutoRestoreKeys/Disable")
    {
        m_ukopt.autoNonVnRestore = false;
        __config->write(SCIM_IMENGINE_UNIKEY_AUTONONVNRESTORE, false);
        change = true;
    }

// modern style
    else if (property == "/Unikey/Options/ModernStyle/Enable")
    {
        m_ukopt.modernStyle = true;
        __config->write(SCIM_IMENGINE_UNIKEY_MODERNSTYLE, true);
        change = true;
    }
    else if (property == "/Unikey/Options/ModernStyle/Disable")
    {
        m_ukopt.modernStyle = false;
        __config->write(SCIM_IMENGINE_UNIKEY_MODERNSTYLE, false);
        change = true;
    }

// free Marking
    else if (property == "/Unikey/Options/FreeMarking/Enable")
    {
        m_ukopt.freeMarking = true;
        __config->write(SCIM_IMENGINE_UNIKEY_FREEMARKING, true);
        change = true;
    }
    else if (property == "/Unikey/Options/FreeMarking/Disable")
    {
        m_ukopt.freeMarking = false;
        __config->write(SCIM_IMENGINE_UNIKEY_FREEMARKING, false);
        change = true;
    }
// macro 
    else if (property == "/Unikey/Options/EnabledMacro/Enable")
    {
        m_ukopt.macroEnabled = true;
        UnikeyLoadMacroTable(getMacroFile());
        __config->write(SCIM_IMENGINE_UNIKEY_MACROENABLED, true);
        change = true;
    }
    else if (property == "/Unikey/Options/EnabledMacro/Disable")
    {
        m_ukopt.macroEnabled = false;
        __config->write(SCIM_IMENGINE_UNIKEY_MACROENABLED, false);
        change = true;
    }

// preedit
    else if (property == "/Unikey/Options/Preedit/Enable")
    {
        m_preedit = true;
        __config->write(SCIM_IMENGINE_UNIKEY_PREEDIT, true);
        change = true;
    }
    else if (property == "/Unikey/Options/Preedit/Disable")
    {
        m_preedit = false;
        __config->write(SCIM_IMENGINE_UNIKEY_PREEDIT, false);
        change = true;
    }

// process w at begin
    else if (property == "/Unikey/Options/ProcessWAtBegin/Enable")
    {
        m_process_w_AtBeginWord = true;
        __config->write(SCIM_IMENGINE_UNIKEY_PROCESSWATWORDBEGIN, true);
        change = true;
    }
    else if (property == "/Unikey/Options/ProcessWAtBegin/Disable")
    {
        m_process_w_AtBeginWord = false;
        __config->write(SCIM_IMENGINE_UNIKEY_PROCESSWATWORDBEGIN, false);
        change = true;
    }

    if (change)
    {
        __config->flush();
        focus_out();
        focus_in();
    }
}
