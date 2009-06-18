INCLUDES		    = -I$(top_builddir) \
	    		      -I$(top_srcdir) \
			      -I$(top_srcdir)/src \
			      -I$(top_srcdir)/ukengine \
                              -I$(top_srcdir)/intl \
                              -DPACKAGE_NAME=\"@PACKAGE_NAME@\" \
			      -DSCIM_ICONDIR=\"@SCIM_ICONDIR@\" \
                              -DSCIM_UNIKEY_VERSION=\"@PACKAGE_VERSION@\" \
                              -DSCIM_LOCALEDIR=\"@SCIM_LOCALEDIR@\"

#IME module
imenginedir 		    = @SCIM_MODULEDIR@/IMEngine
imengine_LTLIBRARIES 	    = unikey.la
unikey_la_SOURCES 	    = scim_unikey_imengine.cpp \
                              scim_unikey_imengine.h \
                              scim_unikey_const.h \
                              scim_unikey_utils.cpp \
                              scim_unikey_utils.h \
                              scimkeyselection.h

unikey_la_LIBADD 	    = $(top_builddir)/ukengine/libunikey-zmod.la
unikey_la_CFLAGS 	    = @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@
unikey_la_CXXFLAGS 	    = @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@
unikey_la_LDFLAGS 	    = -avoid-version -rpath $(imenginedir) -module @LIBTOOL_EXPORT_OPTIONS@ @SCIM_LIBS@

#Setup module
if SCIM_BUILD_SETUP

setupdir                    = @SCIM_MODULEDIR@/SetupUI
setup_LTLIBRARIES           = unikey-setup.la
unikey_setup_la_SOURCES     = scim_unikey_setup.cpp \
                              scim_unikey_const.h

unikey_setup_la_LIBADD      = $(top_builddir)/ukengine/libunikey-zmod.la
unikey_setup_la_CFLAGS      = @SCIM_GTKUTILS_CFLAGS@ @SCIM_CFLAGS@
unikey_setup_la_CXXFLAGS    = @SCIM_GTKUTILS_CFLAGS@ @SCIM_CFLAGS@
unikey_setup_la_LDFLAGS     = -avoid-version -rpath $(setupdir) \
                              -module @LIBTOOL_EXPORT_OPTIONS@ \
                              @SCIM_GTKUTILS_LIBS@ @SCIM_LIBS@ \
                              unikey_la-scim_unikey_utils.lo

endif


