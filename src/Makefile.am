INCLUDES = \
	-I$(top_srcdir)/ukengine \
	-DSCIM_ICONDIR=\"@SCIM_ICONDIR@\" \
	-DSCIM_UNIKEY_VERSION=\"@PACKAGE_VERSION@\" \
    -DLIBEXECDIR=\"$(libexecdir)\"

imenginedir 		    = @SCIM_MODULEDIR@/IMEngine
imengine_LTLIBRARIES 	    = unikey.la
unikey_la_SOURCES 	    = scim_unikey_imengine.cpp \
                              scim_unikey_imengine.h \
                              scim_unikey_const.h \
                              scim_unikey_utils.cpp \
                              scim_unikey_utils.h

unikey_la_LIBADD 	    = $(top_builddir)/ukengine/libunikey-scim.la
unikey_la_CFLAGS 	    = @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@
unikey_la_CXXFLAGS 	    = @SCIM_CFLAGS@ @SCIM_DEBUG_FLAGS@
unikey_la_LDFLAGS 	    = -avoid-version -rpath $(imenginedir) -module @LIBTOOL_EXPORT_OPTIONS@ @SCIM_LIBS@

