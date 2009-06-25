if SCIM_BUILD_SETUP

INCLUDES = \
	-I$(top_srcdir)/ukengine \
	-I$(top_srcdir)/src \
	-DSCIM_ICONDIR=\"@SCIM_ICONDIR@\" \
	-DSCIM_UNIKEY_VERSION=\"@PACKAGE_VERSION@\"

setupdir                    = @SCIM_MODULEDIR@/SetupUI
setup_LTLIBRARIES           = unikey-setup.la
unikey_setup_la_SOURCES     = scim_unikey_setup.cpp

noinst_HEADERS = scimkeyselection.h

unikey_setup_la_LIBADD      = $(top_builddir)/ukengine/libunikey-scim.la

unikey_setup_la_CFLAGS      = @SCIM_GTKUTILS_CFLAGS@ @SCIM_CFLAGS@
unikey_setup_la_CXXFLAGS    = @SCIM_GTKUTILS_CFLAGS@ @SCIM_CFLAGS@
unikey_setup_la_LDFLAGS     = -avoid-version -rpath $(setupdir) \
                              -module @LIBTOOL_EXPORT_OPTIONS@ \
                              @SCIM_GTKUTILS_LIBS@ @SCIM_LIBS@ \
                              $(top_builddir)/src/unikey_la-scim_unikey_utils.lo
endif

