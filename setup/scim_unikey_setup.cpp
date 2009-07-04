/**
   Scim-Unikey Input Method

   Copyright (C) 2008-2009 Ubuntu-VN <http://www.ubuntu-vn.org>
   Author: Le Quoc Tuan <mr.lequoctuan@gmail.com>
   Home: http://scim-unikey.googlecode.com
   License: GNU LESSER GENERAL PUBLIC LICENSE v2.1
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#define Uses_SCIM_CONFIG_BASE

#include <libintl.h>
#define _(String) dgettext(PACKAGE_NAME,String)

#include <iostream>
#include <gtk/gtk.h>
#include <string.h>

#include <scim.h>

using namespace scim;

#define scim_module_init unikey_setup_LTX_scim_module_init
#define scim_module_exit unikey_setup_LTX_scim_module_exit

#define scim_setup_module_create_ui       unikey_setup_LTX_scim_setup_module_create_ui
#define scim_setup_module_get_category    unikey_setup_LTX_scim_setup_module_get_category
#define scim_setup_module_get_name        unikey_setup_LTX_scim_setup_module_get_name
#define scim_setup_module_get_description unikey_setup_LTX_scim_setup_module_get_description
#define scim_setup_module_load_config     unikey_setup_LTX_scim_setup_module_load_config
#define scim_setup_module_save_config     unikey_setup_LTX_scim_setup_module_save_config
#define scim_setup_module_query_changed   unikey_setup_LTX_scim_setup_module_query_changed

void run_setup_button_cb(GtkButton* btn, gpointer user_data);

static GtkWidget* create_setup_window();

extern "C"
{
    void scim_module_init(void)
    {
    }

    void scim_module_exit(void)
    {
    }

    GtkWidget * scim_setup_module_create_ui (void)
    {
        return create_setup_window();
    }

    String scim_setup_module_get_category (void)
    {
        return String("IMEngine");
    }

    String scim_setup_module_get_name(void)
    {
        return String(_("Unikey Setup"));
    }

    String scim_setup_module_get_description(void)
    {
        return String("Setup scim-unikey v" SCIM_UNIKEY_VERSION " IMEngine.");
    }

    void scim_setup_module_load_config(const ConfigPointer &config)
    {
    }

    void scim_setup_module_save_config(const ConfigPointer &config)
    {
    }

    bool scim_setup_module_query_changed()
    {
        return FALSE;
    }
} // extern "C"

static GtkWidget* create_setup_window()
{
    static GtkWidget *window = 0;
    if (!window)
    {
        window = gtk_vbox_new(TRUE, 0);

        GtkWidget* label = gtk_label_new(_(
                "This setup has been detach from scim-setup.\n"
                "Please run Setup by right click on scim tray icon,\n"
                "choose Unikey setup or click on below button."));

        gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);

        gtk_box_pack_start(GTK_BOX(window), label, TRUE, FALSE, 0);

        GtkWidget* btn_runsetup = gtk_button_new_with_mnemonic(_("_Run setup"));
        gtk_signal_connect(GTK_OBJECT(btn_runsetup), "clicked", G_CALLBACK(run_setup_button_cb), NULL);
        gtk_box_pack_start(GTK_BOX(window), btn_runsetup, TRUE, FALSE, 0);

        gtk_widget_show_all(window);
    }
    return window;
}

void run_setup_button_cb(GtkButton* btn, gpointer user_data)
{
    system(LIBEXECDIR "/scim-setup-unikey &");
}

