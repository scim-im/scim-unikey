if SCIM_BUILD_SETUP

INCLUDES = \
    -I$(top_srcdir)/ukengine \
    -I$(top_srcdir)/src \
    -DSCIM_ICONDIR=\"@SCIM_ICONDIR@\" \
    -DGLADE_DATA_DIR=\"$(pkgdatadir)/setup\" \
    -DLOCALEDIR=\"$(localedir)\" \
    -DLIBEXECDIR=\"$(libexecdir)\" \
    -Wall

noinst_HEADERS = \
    dlg_main_setup.h \
    dlg_macro_table.h \
    config_utils.h

glade_DATA = \
    setup-main.glade \
    setup-macro.glade

gladedir = $(pkgdatadir)/setup

EXTRA_DIST = $(glade_DATA)


libexec_PROGRAMS = scim-setup-unikey

scim_setup_unikey_SOURCES = \
    unikey_setup.cpp \
    dlg_main_setup.cpp \
    dlg_macro_table.cpp \
    config_utils.cpp

scim_setup_unikey_CXXFLAGS = \
    @SCIM_GTKUTILS_CFLAGS@ \
    @GIO_CFLAGS@

scim_setup_unikey_LDFLAGS = \
    @SCIM_GTKUTILS_LIBS@ \
    @GIO_LIBS@ \
    $(top_builddir)/ukengine/libunikey-scim.la

# helper module: for launch setup
helperdir                   = @SCIM_MODULEDIR@/Helper
helper_LTLIBRARIES          = unikey-helper.la
unikey_helper_la_SOURCES    = scim_unikey_helper.cpp

unikey_helper_la_CFLAGS     = @SCIM_CFLAGS@
unikey_helper_la_CXXFLAGS   = $(unikey_helper_la_CFLAGS)
unikey_helper_la_LDFLAGS    = -avoid-version -rpath $(helperdir) \
                              -module @LIBTOOL_EXPORT_OPTIONS@

endif
