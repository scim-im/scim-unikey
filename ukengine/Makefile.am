EXTRA_DIST                  = diff README COPYING

unikey_scimdir		    = $(libdir)
unikey_scim_LTLIBRARIES	    = libunikey-scim.la

libunikey_scim_la_LDFLAGS   = -version-info $(UNIKEY_CURRENT):$(UNIKEY_REVISION):$(UNIKEY_AGE)

libunikey_scim_la_SOURCES = \
	byteio.cpp \
	charset.cpp \
	convert.cpp \
	data.cpp \
	error.cpp \
	inputproc.cpp \
	mactab.cpp \
	pattern.cpp \
	ukengine.cpp \
	unikey.cpp \
	usrkeymap.cpp

noinst_HEADERS = \
	byteio.h \
	charset.h \
	data.h \
	inputproc.h \
	keycons.h \
	mactab.h \
	pattern.h \
	ukengine.h \
	unikey.h \
	usrkeymap.h \
	vnconv.h \
	vnlexi.h

