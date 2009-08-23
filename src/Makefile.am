INCLUDES = \
    -I$(top_srcdir)/ukengine \
    -DSCIM_ICONDIR=\"@SCIM_ICONDIR@\" \
    -DLIBEXECDIR=\"$(libexecdir)\" \
    -Wall

imenginedir             = @SCIM_MODULEDIR@/IMEngine
imengine_LTLIBRARIES    = unikey.la
unikey_la_SOURCES       = \
    im_engine.cpp \
    unikey_instance.cpp \
    utils.cpp

noinst_HEADERS = \
    im_engine.h \
    scim_unikey_const.h \
    utils.h

unikey_la_LIBADD        = $(top_builddir)/ukengine/libunikey-scim.la
unikey_la_CFLAGS        = @SCIM_CFLAGS@
unikey_la_CXXFLAGS      = @SCIM_CFLAGS@
unikey_la_LDFLAGS       = -avoid-version -rpath $(imenginedir) -module @LIBTOOL_EXPORT_OPTIONS@ @SCIM_LIBS@

