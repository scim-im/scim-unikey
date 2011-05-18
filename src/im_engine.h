/**
   Scim-Unikey Input Method

   Copyright (C) 2008-2009 Ubuntu-VN <http://www.ubuntu-vn.org>
   Author: Le Quoc Tuan <mr.lequoctuan@gmail.com>
   Home: http://scim-unikey.googlecode.com
   License: GNU GENERAL PUBLIC LICENSE v2
*/

#if !defined (__SCIM_UNIKEY_IMENGINE_H)
#define __SCIM_UNIKEY_IMENGINE_H

#include "unikey.h"

using namespace scim;

class UnikeyFactory : public IMEngineFactoryBase
{
public:
    UnikeyFactory(int id);
    virtual ~UnikeyFactory();

    virtual WideString  get_name() const;
    virtual WideString  get_authors() const;
    virtual WideString  get_credits() const;
    virtual WideString  get_help() const;
    virtual String      get_uuid() const;
    virtual String      get_icon_file() const;

    virtual IMEngineInstancePointer create_instance(const String& encoding, int id = -1);

private:
    int m_id;
};


class UnikeyInstance : public IMEngineInstanceBase
{
    friend class UnikeyInstancePreedit;
    friend class UnikeyInstanceClassic;

public:
    UnikeyInstance(UnikeyFactory *factory, const String& encoding, int id);
    ~UnikeyInstance();

    virtual void reset();
    virtual void focus_in();
    virtual void trigger_property(const String &property);

private:
    PropertyList CreatePropertyList();

    int m_im;                             // input method
    int m_oc;                             // output charset
    UnikeyOptions m_ukopt;                // Unikey Options
    bool m_process_w_AtBeginWord;         // process W at Begin Word?
    bool m_lastkey_with_shift;            // last press key with modifier is shift
};


class UnikeyInstancePreedit : public UnikeyInstance
{
public:
    UnikeyInstancePreedit(UnikeyFactory *factory, const String& encoding, int id = -1);
    ~UnikeyInstancePreedit();

    virtual void reset();
    virtual void focus_in();
    virtual void focus_out();
    virtual bool process_key_event(const KeyEvent& key);
    virtual void trigger_property(const String &property);

private:
    bool unikey_process_key_event(const KeyEvent& key);
    void unikey_update_preedit_string(const WideString s, const bool visible);
    PropertyList CreatePropertyList();

    WideString m_preeditstring;           // store PreEdit String
    bool m_auto_commit;                   // auto commit?
};


class UnikeyInstanceClassic : public UnikeyInstance
{
public:
    UnikeyInstanceClassic(UnikeyFactory *factory, const String& encoding, int id = -1);
    ~UnikeyInstanceClassic();

    virtual void reset();
    virtual void focus_in();
    virtual void focus_out();
    virtual bool process_key_event(const KeyEvent& key);
    virtual void trigger_property(const String &property);

private:
    bool unikey_process_key_event(const KeyEvent& key);
    void unikey_send_backspace(int nBackspace);
    void unikey_commit_key_event(const KeyEvent& key);
    PropertyList CreatePropertyList();
};

#endif

