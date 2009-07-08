#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "dlg_main_setup.h"

#include "scim_unikey_const.h"

#define get_macro_file() (g_build_filename(g_getenv("HOME"), SCIM_IMENGINE_UNIKEY_MACROPATH, NULL))
void scim_set_config(const gchar* key, GType t, gpointer data);
gboolean scim_get_config(const gchar* key, GType t, gpointer data);

void set_default_config(UnikeyMainSetupOptions* opt)
{
    opt->input_method = 0;
    opt->output_charset = 0;

    opt->enableSpellcheck = SCIM_IMENGINE_UNIKEY_SPELLCHECKENABLED_DEF;
    opt->autoRestoreNonVn = SCIM_IMENGINE_UNIKEY_AUTONONVNRESTORE_DEF;
    opt->modernStyle = SCIM_IMENGINE_UNIKEY_MODERNSTYLE_DEF;
    opt->freeMarking = SCIM_IMENGINE_UNIKEY_FREEMARKING_DEF;
    opt->enableMacro = SCIM_IMENGINE_UNIKEY_MACROENABLED_DEF;

    opt->processwatbegin = SCIM_IMENGINE_UNIKEY_PROCESSWATWORDBEGIN_DEF;
    opt->enablepreedit = SCIM_IMENGINE_UNIKEY_PREEDIT_DEF;

    opt->macrofile = get_macro_file();
}

void read_config(UnikeyMainSetupOptions* opt)
{
    gboolean b;
    int k;
    gboolean t;
    //gchar* s;

    // get Input method
    b = scim_get_config(SCIM_IMENGINE_UNIKEY_INPUTMETHOD, G_TYPE_INT, &k);
    if (b == TRUE)
    {
        opt->input_method = k;
    }
    // END get Input method

    // get Output charset
    b = scim_get_config(SCIM_IMENGINE_UNIKEY_OUTPUTCHARSET, G_TYPE_INT, &k);
    if (b == TRUE)
    {
        opt->output_charset = k;
    }
    // END get Output charset

    // get Spellcheck
    b = scim_get_config(SCIM_IMENGINE_UNIKEY_SPELLCHECKENABLED, G_TYPE_BOOLEAN, &t);
    if (b == TRUE)
    {
        opt->enableSpellcheck = t;
    }
    // END get Spellcheck

    // get autoRestoreNonVn
    b = scim_get_config(SCIM_IMENGINE_UNIKEY_AUTONONVNRESTORE, G_TYPE_BOOLEAN, &t);
    if (b == TRUE)
    {
        opt->autoRestoreNonVn = t;
    }
    // END get autoRestoreNonVn

    // get modernStyle
    b = scim_get_config(SCIM_IMENGINE_UNIKEY_MODERNSTYLE, G_TYPE_BOOLEAN, &t);
    if (b == TRUE)
    {
        opt->modernStyle = t;
    }
    // END get modernStyle

    // get freeMarking
    b = scim_get_config(SCIM_IMENGINE_UNIKEY_FREEMARKING, G_TYPE_BOOLEAN, &t);
    if (b == TRUE)
    {
        opt->freeMarking = t;
    }
    // END get freeMarking

    // get enableMacro
    b = scim_get_config(SCIM_IMENGINE_UNIKEY_MACROENABLED, G_TYPE_BOOLEAN, &t);
    if (b == TRUE)
    {
        opt->enableMacro = t;
    }
    // END get enableMacro

    // get ProcessWAtBegin
    b = scim_get_config(SCIM_IMENGINE_UNIKEY_PROCESSWATWORDBEGIN, G_TYPE_BOOLEAN, &t);
    if (b == TRUE)
    {
        opt->processwatbegin = t;
    }
    // END get ProcessWAtBegin

    // get EnablePreedit
    b = scim_get_config(SCIM_IMENGINE_UNIKEY_PREEDIT, G_TYPE_BOOLEAN, &t);
    if (b == TRUE)
    {
        opt->enablepreedit = t;
    }
    // END get EnablePreedit
}

void write_config(UnikeyMainSetupOptions* opt)
{
    scim_set_config(SCIM_IMENGINE_UNIKEY_INPUTMETHOD, G_TYPE_INT, &opt->input_method);
    scim_set_config(SCIM_IMENGINE_UNIKEY_OUTPUTCHARSET, G_TYPE_INT, &opt->output_charset);
    scim_set_config(SCIM_IMENGINE_UNIKEY_SPELLCHECKENABLED, G_TYPE_BOOLEAN, &opt->enableSpellcheck);
    scim_set_config(SCIM_IMENGINE_UNIKEY_AUTONONVNRESTORE, G_TYPE_BOOLEAN, &opt->autoRestoreNonVn);
    scim_set_config(SCIM_IMENGINE_UNIKEY_MODERNSTYLE, G_TYPE_BOOLEAN, &opt->modernStyle);
    scim_set_config(SCIM_IMENGINE_UNIKEY_FREEMARKING, G_TYPE_BOOLEAN, &opt->freeMarking);
    scim_set_config(SCIM_IMENGINE_UNIKEY_MACROENABLED, G_TYPE_BOOLEAN, &opt->enableMacro);
    scim_set_config(SCIM_IMENGINE_UNIKEY_PROCESSWATWORDBEGIN, G_TYPE_BOOLEAN, &opt->processwatbegin);
    scim_set_config(SCIM_IMENGINE_UNIKEY_PREEDIT, G_TYPE_BOOLEAN, &opt->enablepreedit);
}

int force_engine_to_reload_config()
{
    return system("scim-config-agent --reload > /dev/null");
}

void scim_set_config(const gchar* key, GType t, gpointer data)
{
    gchar** argv;
    gchar s[1024];
    gchar* output;

    argv = (gchar**)g_malloc(sizeof(gchar*)*4);
    argv[0] = (gchar*)"scim-config-agent";
    argv[1] = (gchar*)"--set";
    argv[3] = (gchar*)NULL;

    strcpy(s, key);
    strcat(s, "=");

    switch (t)
    {
    case G_TYPE_BOOLEAN:
        strcat(s, *(gboolean*)data?"true":"false");
        break;
    case G_TYPE_INT:
        sprintf(s+strlen(s), "%d", *(gint*)data);
        break;
    case G_TYPE_STRING:
        strcat(s, *(gchar**)data);
        break;
    }

    argv[2] = (gchar*)s;

    g_spawn_sync(NULL, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &output, NULL, NULL, NULL);
    free(output);
}

gboolean scim_get_config(const gchar* key, GType t, gpointer data)
{
    gchar** argv;
    gchar* output;

    argv = (gchar**)g_malloc(sizeof(gchar*)*4);
    argv[0] = (gchar*)"scim-config-agent";
    argv[1] = (gchar*)"--get";
    argv[2] = (gchar*)key;
    argv[3] = (gchar*)NULL;

    g_spawn_sync(NULL, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &output, NULL, NULL, NULL);

    free(argv);

    output[strlen(output)-1] = 0;

    if (strcmp(output, "Failed to get key value.") == 0)
    {
        free(output);
        return FALSE;
    }

    gboolean* pb;
    gint* pi;
    gchar** pc;

    switch(t)
    {
    case G_TYPE_BOOLEAN:
        pb = (gboolean*)data;
        if (strcmp(output, "true") == 0)
            *pb = TRUE;
        else if (strcmp(output, "false") == 0)
            *pb = FALSE;
        else
        {
            free(output);
            return FALSE;
        }
        break;
    case G_TYPE_INT:
        pi = (gint*)data;
        if (isalpha(output[0]))
        {
            free(output);
            return FALSE;
        }
        else
            *pi = atoi(output);
        break;
    case G_TYPE_STRING:
        pc = (gchar**)data;
        *pc = output;
        return TRUE;
    }

    free(output);

    return TRUE;
}

