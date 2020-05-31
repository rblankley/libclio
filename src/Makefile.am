lib_LTLIBRARIES = libclio.la

libclio_la_LDFLAGS = -version-info 1:0:0

libclio_la_SOURCES = \
	appenders/consoleappender.cpp \
	appenders/fileappender.cpp \
	appenders/rollingfileappender.cpp \
	layouts/patternlayout.cpp \
	appender.cpp \
	appenderfactory.cpp \
	hexdump.cpp \
	clio.cpp \
	layout.cpp \
	layoutfactory.cpp \
	logger.cpp \
	loggermanager.cpp \
	loglevel.cpp \
	logline.cpp \
	tinyxml2.cpp

otherincludedir = $(includedir)/clio

nobase_otherinclude_HEADERS = \
	appender.h \
	hexdump.h \
	clio.h \
	logger.h \
	loglevel.h \
	logline.h \
	propertymap.h

