/**
   Scim-Unikey Input Method
   Version: 0.2.0a

   Copyright (C) 2008-2009 Ubuntu-VN <http://www.ubuntu-vn.org>
   Author: Le Quoc Tuan <mr.lequoctuan@gmail.com>
   Home: http://scim-unikey.googlecode.com
   License: GNU LESSER GENERAL PUBLIC LICENSE v2.1
*/

#if !defined (__SCIM_UNIKEY_IMENGINE_H)
#define __SCIM_UNIKEY_IMENGINE_H

#include "unikey.h"

using namespace scim;

class UnikeyFactory : public IMEngineFactoryBase
{
	friend class UnikeyInstance;

public:
	UnikeyFactory();
	virtual ~UnikeyFactory ();

	virtual WideString  get_name () const;
	virtual WideString  get_authors () const;
	virtual WideString  get_credits () const;
	virtual WideString  get_help () const;
	virtual String      get_uuid () const;
	virtual String      get_icon_file () const;

	virtual IMEngineInstancePointer create_instance (const String& encoding, int id = -1);
};

class UnikeyInstance : public IMEngineInstanceBase
{
public:
	UnikeyInstance(UnikeyFactory *factory, const String& encoding, int id = -1);
	virtual ~UnikeyInstance ();

	virtual bool process_key_event(const KeyEvent& key);
	virtual void reset();
	virtual void focus_in();
	virtual void focus_out();
	virtual void trigger_property(const String &property);

private:
	bool Unikey_process_key_event_direct(const KeyEvent& key);
	bool Unikey_process_key_event_preedit(const KeyEvent& key);
	void Unikey_send_backspace(int nBackspace);
	void Unikey_update_preedit_string(const WideString s, const bool visible);
	PropertyList CreatePropertyList();

	int m_im;				// input method
	int m_oc;				// output charset
	UnikeyOptions m_ukopt;			// Unikey Options
	WideString m_preeditstring;		// store PreEdit String
	bool m_preedit;				// is PreEdit On?
	bool m_codertelex;			// is CoderTelex on?
	bool m_process_w_AtBeginWord;		// process W at Begin Word?
	bool m_auto_commit;			// auto commit?
	std::vector<KeyEvent> m_preeditskey;	// keys to switch preedit and non preedit mode
	bool m_lastkey_with_shift;		// last press key with modifier is shift
};

#endif

