/**
   Scim-Unikey Input Method

   Copyright (C) 2008-2009 Ubuntu-VN <http://www.ubuntu-vn.org>
   Author: Le Quoc Tuan <mr.lequoctuan@gmail.com>
   Home: http://scim-unikey.googlecode.com
   License: GNU LESSER GENERAL PUBLIC LICENSE v2.1
*/

#define Uses_SCIM_CONFIG_BASE

#include <libintl.h>
#define _(String) dgettext(PACKAGE_NAME,String)

#include <iostream>
#include <gtk/gtk.h>
#include <string.h>

#include <scim.h>
#include "scim_unikey_const.h"
#include "scim_unikey_utils.h"
#include "keycons.h"
#include "mactab.h"
#include "scimkeyselection.h"

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

static bool    __have_changed                       = false;

static bool    __unikey_preedit                     = SCIM_IMENGINE_UNIKEY_PREEDIT_DEF;
static bool    __unikey_freemarking                 = SCIM_IMENGINE_UNIKEY_FREEMARKING_DEF;
static bool    __unikey_modernstyle                 = SCIM_IMENGINE_UNIKEY_MODERNSTYLE_DEF;
static bool    __unikey_macroenabled                = SCIM_IMENGINE_UNIKEY_MACROENABLED_DEF;
static bool    __unikey_spellcheckenabled           = SCIM_IMENGINE_UNIKEY_SPELLCHECKENABLED_DEF;
static bool    __unikey_autononvnrestore            = SCIM_IMENGINE_UNIKEY_AUTONONVNRESTORE_DEF;
static bool    __unikey_processwatwordbegin       = SCIM_IMENGINE_UNIKEY_PROCESSWATWORDBEGIN_DEF;

static GtkTooltips  *__widget_tooltips              = 0;
static GtkWidget    *__widget_preedit               = 0;
static GtkWidget    *__widget_preedit_skey          = 0;
static GtkWidget    *__widget_freemarking           = 0;
static GtkWidget    *__widget_modernstyle           = 0;
static GtkWidget    *__widget_macroenabled          = 0;
static GtkWidget    *__widget_spellcheckenabled     = 0;
static GtkWidget    *__widget_autononvnrestore      = 0;
static GtkWidget    *__widget_processwatwordbegin   = 0;
static CMacroTable  __macStore;

static GtkWidget* create_setup_window();
static void load_config(const ConfigPointer &config);
static void save_config(const ConfigPointer &config);
static bool query_changed();

static void setup_widget_value();
static GtkListStore* create_and_fill_list_store();

static void on_default_toggle_button_toggled(GtkToggleButton *togglebutton, gpointer user_data);
static void on_macrotable_button_clicked(GtkButton *button, gpointer user_data);
static void on_key_edited (GtkCellRendererText *celltext, const gchar *string_path, const gchar *new_text, gpointer data);
static void on_replace_edited (GtkCellRendererText *celltext, const gchar *string_path, const gchar *new_text, gpointer data);
static void on_end_macro_table(GtkListStore *liststore);
static void on_del_macro_clicked(GtkButton *button, gpointer user_data);
static void on_delall_macro_clicked(GtkButton *button, gpointer user_data);
static gboolean iter_2_macro(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer data);
static void on_hotkey_button_clicked(GtkButton *button, gpointer user_data);

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
        load_config(config);
    }

    void scim_setup_module_save_config(const ConfigPointer &config)
    {
        save_config(config);
    }

    bool scim_setup_module_query_changed()
    {
        return query_changed();
    }
} // extern "C"

static GtkWidget* create_setup_window()
{
    static GtkWidget *window = 0;
    if (!window)
    {
        gtk_window_set_default_icon_from_file(SCIM_ICONDIR SCIM_UNIKEY_ICON_FILENAME, NULL);
        __widget_tooltips = gtk_tooltips_new();

// Create the toplevel box.
        window = gtk_hbox_new(false, 0);

// create vbox
        GtkWidget *vbox0 = gtk_vbox_new(false, 0);
        gtk_box_pack_start(GTK_BOX(window), vbox0, false, false, 0);

// create Common Frame
        GtkWidget *fr = gtk_frame_new(_("Unikey option"));
        gtk_box_pack_start(GTK_BOX(vbox0), fr, false, false, 0);

        GtkWidget *vbox = gtk_vbox_new(false, 0);
        gtk_container_add(GTK_CONTAINER(fr), vbox);
        gtk_container_set_border_width(GTK_CONTAINER(fr), 5);

// create spellcheck checkbox
        __widget_spellcheckenabled = gtk_check_button_new_with_label(_("Enable spell check"));
        gtk_box_pack_start(GTK_BOX(vbox), __widget_spellcheckenabled, false, false, 0);
        gtk_container_set_border_width(GTK_CONTAINER(__widget_spellcheckenabled), 5);
        g_signal_connect(__widget_spellcheckenabled, "toggled", G_CALLBACK(on_default_toggle_button_toggled), &__unikey_spellcheckenabled);

        gtk_tooltips_set_tip (__widget_tooltips, __widget_spellcheckenabled,
                              _("If enable, you can decrease mistake when typing"), NULL);

// create autononvnrestore checkbox
        __widget_autononvnrestore = gtk_check_button_new_with_label(_("Auto restore keys with invalid words"));
        gtk_box_pack_start(GTK_BOX(vbox), __widget_autononvnrestore, false, false, 0);
        gtk_container_set_border_width(GTK_CONTAINER(__widget_autononvnrestore), 5);
        g_signal_connect(__widget_autononvnrestore, "toggled", G_CALLBACK(on_default_toggle_button_toggled), &__unikey_autononvnrestore);

        gtk_tooltips_set_tip (__widget_tooltips, __widget_autononvnrestore,
                              _("When typing a word not in Vietnamese,\n"
                              "it will auto restore keystroke into orginal"), NULL);

// create modernstyle checkbox
        __widget_modernstyle = gtk_check_button_new_with_label(_("Use oà, uý (instead of òa, úy)"));
        gtk_box_pack_start(GTK_BOX(vbox), __widget_modernstyle, false, false, 0);
        gtk_container_set_border_width(GTK_CONTAINER(__widget_modernstyle), 5);
        g_signal_connect(__widget_modernstyle, "toggled", G_CALLBACK(on_default_toggle_button_toggled), &__unikey_modernstyle);

// create freemarking checkbox
        __widget_freemarking = gtk_check_button_new_with_label(_("Allow type with more freedom"));
        gtk_box_pack_start(GTK_BOX(vbox), __widget_freemarking, false, false, 0);
        gtk_container_set_border_width(GTK_CONTAINER(__widget_freemarking), 5);
        g_signal_connect(__widget_freemarking, "toggled", G_CALLBACK(on_default_toggle_button_toggled), &__unikey_freemarking);

/*
// create preedit frame
*/
        GtkWidget *prefr = gtk_frame_new(_("Typing mode"));
        gtk_container_set_border_width(GTK_CONTAINER(prefr), 5);
        gtk_box_pack_start(GTK_BOX(vbox0), prefr, false, false, 0);

        vbox = gtk_vbox_new(false, 0);
        gtk_container_add(GTK_CONTAINER(prefr), vbox);

// create preedit switch key
        GtkWidget *psbox = gtk_hbox_new(false, 0);
        gtk_box_pack_start(GTK_BOX(vbox), psbox, false, false, 0);

        GtkWidget *lpskey = gtk_label_new(_("Mode switch key:"));
        gtk_box_pack_start(GTK_BOX(psbox), lpskey, false, false, 5);

        __widget_preedit_skey = gtk_entry_new();
        gtk_entry_set_editable(GTK_ENTRY(__widget_preedit_skey), false);
        gtk_box_pack_start(GTK_BOX(psbox), __widget_preedit_skey, true, false, 0);

        GtkWidget *bpskey = gtk_button_new_with_label("...");
        gtk_box_pack_start(GTK_BOX(psbox), bpskey, true, true, 0);
        gtk_container_set_border_width(GTK_CONTAINER(bpskey), 5);
        g_signal_connect(bpskey, "clicked", G_CALLBACK(on_hotkey_button_clicked), __widget_preedit_skey);


// create preedit checkbox
        __widget_preedit = gtk_check_button_new_with_label(_("Preedit is default"));
        gtk_box_pack_start(GTK_BOX(vbox), __widget_preedit, false, false, 0);
        gtk_container_set_border_width(GTK_CONTAINER(__widget_preedit), 5);
        g_signal_connect(__widget_preedit, "toggled", G_CALLBACK(on_default_toggle_button_toggled), &__unikey_preedit);

        gtk_tooltips_set_tip (__widget_tooltips, __widget_preedit,
                              _("This option is best for most application\n"
                              "But you may don't like it because it have an underline when typing"), NULL);



//
        vbox = gtk_vbox_new(false, 0);
        gtk_box_pack_start(GTK_BOX(window), vbox, true, true, 0);

// right frame 1
        fr = gtk_frame_new(_("Macro option"));
        gtk_box_pack_start(GTK_BOX(vbox), fr, false, true, 0);
        gtk_container_set_border_width(GTK_CONTAINER(fr), 5);

        // (frame 2)
        GtkWidget *fr2 = gtk_frame_new(_("Telex option"));
        gtk_box_pack_start(GTK_BOX(vbox), fr2, false, false, 0);
        gtk_container_set_border_width(GTK_CONTAINER(fr2), 5);

        vbox = gtk_vbox_new(false, 0);
        gtk_container_add(GTK_CONTAINER(fr), vbox);

// create macroenabled checkbox
        __widget_macroenabled = gtk_check_button_new_with_label(_("Enable Macro"));
        gtk_box_pack_start(GTK_BOX(vbox), __widget_macroenabled, false, false, 0);
        gtk_container_set_border_width(GTK_CONTAINER(__widget_macroenabled), 5);
        g_signal_connect(__widget_macroenabled, "toggled", G_CALLBACK(on_default_toggle_button_toggled), &__unikey_macroenabled);

// create macroedit button
        GtkWidget* __widget_macrotable = gtk_button_new_with_label(_("Macro Table"));
        gtk_box_pack_start(GTK_BOX(vbox), __widget_macrotable, false, false, 0);
        gtk_container_set_border_width(GTK_CONTAINER(__widget_macrotable), 5);
        g_signal_connect(__widget_macrotable, "clicked", G_CALLBACK(on_macrotable_button_clicked), NULL);

        gtk_tooltips_set_tip (__widget_tooltips, __widget_macrotable,
                              _("Edit the macro table for Macro function"), NULL);


// right frame 2
        vbox = gtk_vbox_new(false, 0);
        gtk_container_add(GTK_CONTAINER(fr2), vbox);

// create process w at word begin checkbox
        __widget_processwatwordbegin = gtk_check_button_new_with_label(_("Process W at word begin"));
        gtk_box_pack_start(GTK_BOX(vbox), __widget_processwatwordbegin, false, false, 0);
        gtk_container_set_border_width(GTK_CONTAINER(__widget_processwatwordbegin), 5);
        g_signal_connect(__widget_processwatwordbegin, "toggled", G_CALLBACK(on_default_toggle_button_toggled), &__unikey_processwatwordbegin);

        gtk_tooltips_set_tip (__widget_tooltips, __widget_processwatwordbegin,
                              _("If enable, type W at begin\n"
                              "of word will change to Ư."), NULL);


        setup_widget_value();
        gtk_widget_show_all(window);
    }
    return window;
}

static void setup_widget_value()
{
    if (__widget_preedit)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(__widget_preedit), __unikey_preedit);

    if (__widget_spellcheckenabled)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(__widget_spellcheckenabled), __unikey_spellcheckenabled);

    if (__widget_autononvnrestore)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(__widget_autononvnrestore), __unikey_autononvnrestore);

    if (__widget_modernstyle)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(__widget_modernstyle), __unikey_modernstyle);

    if (__widget_freemarking)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(__widget_freemarking), __unikey_freemarking);

    if (__widget_macroenabled)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(__widget_macroenabled), __unikey_macroenabled);

    if (__widget_processwatwordbegin)
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(__widget_processwatwordbegin), __unikey_processwatwordbegin);
}

static void load_config(const ConfigPointer &config)
{
    if (!config.null())
    {
        bool t;
        t = config->read(SCIM_IMENGINE_UNIKEY_PREEDIT, &__unikey_preedit);
        if (!t) __unikey_preedit = SCIM_IMENGINE_UNIKEY_PREEDIT_DEF;

        String s;        
        t = config->read(SCIM_IMENGINE_UNIKEY_PREEDIT_SWITCH_KEY, &s);
        gtk_entry_set_text(GTK_ENTRY(__widget_preedit_skey), t?s.c_str():SCIM_IMENGINE_UNIKEY_PREEDIT_SWITCH_KEY_DEF);

        t = config->read(SCIM_IMENGINE_UNIKEY_FREEMARKING, &__unikey_freemarking);
        if (!t) __unikey_freemarking = SCIM_IMENGINE_UNIKEY_FREEMARKING_DEF;

        t = config->read(SCIM_IMENGINE_UNIKEY_MODERNSTYLE, &__unikey_modernstyle);
        if (!t) __unikey_modernstyle = SCIM_IMENGINE_UNIKEY_MODERNSTYLE_DEF;

        t = config->read(SCIM_IMENGINE_UNIKEY_MACROENABLED, &__unikey_macroenabled);
        if (!t) __unikey_macroenabled = SCIM_IMENGINE_UNIKEY_MACROENABLED_DEF;

        t = config->read(SCIM_IMENGINE_UNIKEY_SPELLCHECKENABLED, &__unikey_spellcheckenabled);
        if (!t) __unikey_spellcheckenabled = SCIM_IMENGINE_UNIKEY_SPELLCHECKENABLED_DEF;

        t = config->read(SCIM_IMENGINE_UNIKEY_AUTONONVNRESTORE, &__unikey_autononvnrestore);
        if (!t) __unikey_autononvnrestore = SCIM_IMENGINE_UNIKEY_AUTONONVNRESTORE_DEF;

        t = config->read(SCIM_IMENGINE_UNIKEY_PROCESSWATWORDBEGIN, &__unikey_processwatwordbegin);
        if (!t) __unikey_processwatwordbegin = SCIM_IMENGINE_UNIKEY_PROCESSWATWORDBEGIN_DEF;

        __macStore.init();
        __macStore.loadFromFile(getMacroFile());

        setup_widget_value();
        __have_changed = false;
    }
}

static void save_config(const ConfigPointer &config)
{
    if (!config.null())
    {
        config->write(SCIM_IMENGINE_UNIKEY_PREEDIT, __unikey_preedit);
        String s = String(gtk_entry_get_text(GTK_ENTRY(__widget_preedit_skey)));
        config->write(SCIM_IMENGINE_UNIKEY_PREEDIT_SWITCH_KEY, s);
        config->write(SCIM_IMENGINE_UNIKEY_FREEMARKING, __unikey_freemarking);
        config->write(SCIM_IMENGINE_UNIKEY_MODERNSTYLE, __unikey_modernstyle);
        config->write(SCIM_IMENGINE_UNIKEY_MACROENABLED, __unikey_macroenabled);
        config->write(SCIM_IMENGINE_UNIKEY_SPELLCHECKENABLED, __unikey_spellcheckenabled);
        config->write(SCIM_IMENGINE_UNIKEY_AUTONONVNRESTORE, __unikey_autononvnrestore);
        config->write(SCIM_IMENGINE_UNIKEY_AUTONONVNRESTORE, __unikey_autononvnrestore);
        config->write(SCIM_IMENGINE_UNIKEY_PROCESSWATWORDBEGIN, __unikey_processwatwordbegin);

        s = String(getMacroFile());
        FILE *f = fopen(s.c_str(), "wt");
        if (f == NULL)
        {
            s = s.substr(0, s.rfind('/'));
            int tmp=system((String("mkdir ") + s).c_str());
        }
        else
            fclose(f);

        __macStore.writeToFile(getMacroFile());

        __have_changed = false;
    }
}

static bool query_changed()
{
    return __have_changed;
}

static void on_default_toggle_button_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    bool *toggle = static_cast<bool*> (user_data);

    if (toggle)
    {
        *toggle = gtk_toggle_button_get_active (togglebutton);
        __have_changed = true;
    }
}

enum {COL_KEY = 0, COL_REPLACE, NUM_COLS};

static void on_macrotable_button_clicked(GtkButton *button, gpointer user_data)
{
    GtkWidget           *dialog;
    GtkWidget           *treeview;
    GtkTreeModel        *model;
    GtkTreeViewColumn   *col;
    GtkWidget           *contentarea;
    GtkWidget           *hbox, *vbox;
    GtkCellRenderer     *renderer;

// create main dialog
    dialog = gtk_dialog_new();
    gtk_window_set_default_size(GTK_WINDOW(dialog), 600, 300);
    gtk_window_set_title(GTK_WINDOW(dialog), _("Macro table definition"));
    gtk_dialog_add_buttons(GTK_DIALOG(dialog),
                           GTK_STOCK_OK, GTK_RESPONSE_OK,
                           GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                           NULL);

    //contentarea = gtk_dialog_get_content_area(GTK_DIALOG(dialog)); // only for GTK >= 2.14
    contentarea = GTK_DIALOG(dialog)->vbox;

    hbox = gtk_hbox_new (false, 5);
    gtk_container_add(GTK_CONTAINER(contentarea), hbox);

// create scroll window and tree view
    model = GTK_TREE_MODEL(create_and_fill_list_store());

    treeview = gtk_tree_view_new_with_model (model);

    GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    gtk_box_pack_start(GTK_BOX(hbox), scroll, true, true, 0);
    gtk_container_add(GTK_CONTAINER(scroll), treeview);

// create key column
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", true, NULL);
    g_object_set(renderer, "width-chars", MAX_MACRO_KEY_LEN+4, NULL);
    g_signal_connect(renderer, "edited", G_CALLBACK(on_key_edited), model);
    col = gtk_tree_view_column_new_with_attributes(_("Word"), renderer, "text", COL_KEY, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW(treeview), col);

// create replace column
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer, "editable", true, NULL);
    //g_object_set(renderer, "width-chars", MAX_MACRO_KEY_LEN*3, NULL);
    g_signal_connect(renderer, "edited", G_CALLBACK(on_replace_edited), model);
    col = gtk_tree_view_column_new_with_attributes(_("Replace with"), renderer, "text", COL_REPLACE, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW(treeview), col);

    vbox = gtk_vbox_new (false, 0);
    gtk_box_pack_end(GTK_BOX(hbox), vbox, false, false, 6);

// create Del button
    GtkWidget* btDel = gtk_button_new_with_label (_("Delete"));
    gtk_box_pack_start(GTK_BOX(vbox), btDel, false, true, 3);
    g_signal_connect(btDel, "clicked", G_CALLBACK(on_del_macro_clicked), treeview);

// create DelAll button
    GtkWidget *btDelAll = gtk_button_new_with_label (_("Delete All"));
    gtk_box_pack_start(GTK_BOX(vbox), btDelAll, false, true, 3);
    g_signal_connect(btDelAll, "clicked", G_CALLBACK(on_delall_macro_clicked), model);

    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));

// save Macro Table
    if (result == GTK_RESPONSE_OK)
    {
        __macStore.init();
        gtk_tree_model_foreach(model, iter_2_macro, NULL);

        __have_changed = true;
    }

    g_object_unref(model);
    gtk_widget_destroy(dialog);
}

static void on_end_macro_table(GtkListStore *liststore)
{
    GtkTreeIter     iter;
    gchar           *lastkey;
    gint n;

    n = gtk_tree_model_iter_n_children(GTK_TREE_MODEL(liststore), NULL);
    if (n)
    {
        gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(liststore), &iter, NULL, n-1);
    }
    else
    {
        gtk_list_store_append(liststore, &iter);
        gtk_list_store_set(liststore, &iter, COL_KEY, "...", COL_REPLACE, "...", -1);
        return;
    }

    gtk_tree_model_get(GTK_TREE_MODEL(liststore), &iter, COL_KEY, &lastkey, -1);
    if (strcmp(lastkey, "...") || n==0)
    {
        gtk_list_store_append(liststore, &iter);
        gtk_list_store_set(liststore, &iter, COL_KEY, "...", COL_REPLACE, "...", -1);
    }
}

static void on_del_macro_clicked(GtkButton *button, gpointer user_data)
{
    GtkTreeView     *treeview;
    GtkListStore    *liststore;
    GtkTreeSelection*tselect;
    GtkTreeIter     iter;
    gchar           *lastkey;

    treeview = GTK_TREE_VIEW(user_data);
    tselect = gtk_tree_view_get_selection(treeview);

    if (gtk_tree_selection_get_selected(tselect, NULL, &iter))
    {
        liststore = GTK_LIST_STORE(gtk_tree_view_get_model(treeview));
        gtk_tree_model_get(GTK_TREE_MODEL(liststore), &iter, COL_KEY, &lastkey, -1);
        if (strcmp(lastkey, "..."))
            gtk_list_store_remove(liststore, &iter);
    }
}

static void on_delall_macro_clicked(GtkButton *button, gpointer user_data)
{
    GtkListStore    *liststore;
    GtkTreeIter     iter;

    liststore = GTK_LIST_STORE(user_data);

    gtk_list_store_clear(liststore);
    on_end_macro_table(liststore);
}

static GtkListStore* create_and_fill_list_store()
{
    GtkListStore*   liststore;
    GtkTreeIter     iter;
    FILE            *f;
    char            key[MAX_MACRO_KEY_LEN*3], replace[MAX_MACRO_TEXT_LEN*3];
    UKBYTE          *p;
    int             i, inLen, maxOutLen, ret;

    liststore = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);

// add key and replace to liststore
    for (int i=0 ; i < __macStore.getCount() ; i++)
    {
        p = (UKBYTE *)__macStore.getKey(i);
        inLen = -1;
        maxOutLen = sizeof(key);
        ret = VnConvert(CONV_CHARSET_VNSTANDARD, CONV_CHARSET_UNIUTF8,
                        (UKBYTE *) p, (UKBYTE *)key,
                        &inLen, &maxOutLen);
        if (ret != 0)
            continue;

        p = (UKBYTE *)__macStore.getText(i);
        inLen = -1;
        maxOutLen = sizeof(replace);
        ret = VnConvert(CONV_CHARSET_VNSTANDARD, CONV_CHARSET_UNIUTF8,
                        p, (UKBYTE *)replace,
                        &inLen, &maxOutLen);
        if (ret != 0)
            continue;

        gtk_list_store_append(liststore, &iter);
        gtk_list_store_set(liststore, &iter, COL_KEY, key, COL_REPLACE, replace, -1);
    }

    on_end_macro_table(liststore);
    return liststore;
}

static void on_key_edited (GtkCellRendererText *celltext, const gchar *string_path, const gchar *new_text, gpointer data)
{
    GtkTreeModel    *model;
    GtkTreeIter     iter;
    gchar           *curkey, *oldkey, *oldreplace;
    bool            b;

    model = GTK_TREE_MODEL (data);

    if (!strcmp(new_text, "..."))
        return;

    b = gtk_tree_model_get_iter_first(model, &iter);
    while (b)
    {
        gtk_tree_model_get(model, &iter, COL_KEY, &curkey, -1);
        if (!strcasecmp(curkey, new_text))
            return;

        b = gtk_tree_model_iter_next(model, &iter);
    }
    gtk_tree_model_get_iter_from_string(model, &iter, string_path);
    gtk_tree_model_get(model, &iter, COL_KEY, &oldkey, COL_REPLACE, &oldreplace, -1);

    gtk_list_store_set(GTK_LIST_STORE(model), &iter, COL_KEY, new_text, -1);
    if (!strcmp(oldkey, "..."))
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, COL_REPLACE, _("(replace text)"));

    on_end_macro_table(GTK_LIST_STORE(model));
}

static void on_replace_edited (GtkCellRendererText *celltext, const gchar *string_path, const gchar *new_text, gpointer data)
{
    GtkTreeModel    *model;
    GtkTreeIter     iter;
    gchar           *oldkey;

    model = GTK_TREE_MODEL (data);
    gtk_tree_model_get_iter_from_string(model, &iter, string_path);

    gtk_tree_model_get(model, &iter, COL_KEY, &oldkey, -1);
    if (strcmp(oldkey, "..."))
        gtk_list_store_set(GTK_LIST_STORE(model), &iter, COL_REPLACE, new_text, -1);
}

static gboolean iter_2_macro(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer data)
{
    gchar   *key, *replace;
    GtkTreeIter c;

    c = *iter;

    if (!gtk_tree_model_iter_next(model, &c))
        return true;
    gtk_tree_model_get(model, iter, COL_KEY, &key, COL_REPLACE, &replace, -1);

    __macStore.addItem(key, replace, CONV_CHARSET_XUTF8);

    return false;
}

static void on_hotkey_button_clicked (GtkButton *button, gpointer data)
{
    GtkWidget *wid = (GtkWidget*)data;

    GtkWidget *dialog = scim_key_selection_dialog_new("Edit hotkey");
    gint r;
    const gchar *hotkeys = gtk_entry_get_text(GTK_ENTRY(wid));
    
    if (hotkeys)
    {
        scim_key_selection_dialog_set_keys(
            SCIM_KEY_SELECTION_DIALOG(dialog), hotkeys);
    }

    r = gtk_dialog_run(GTK_DIALOG(dialog));

    if (r == GTK_RESPONSE_OK)
    {
        const gchar *newkeys = scim_key_selection_dialog_get_keys(SCIM_KEY_SELECTION_DIALOG(dialog));
        gtk_entry_set_text(GTK_ENTRY(wid), newkeys?newkeys:"");
        __have_changed = true;
    }

    gtk_widget_destroy(dialog);
}
