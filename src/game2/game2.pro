CONFIG(debug, debug|release): CONFIG -= release
else: CONFIG -= debug

QT += core gui opengl widgets

TEMPLATE = app
TARGET = hl2
CONFIG += link_prl c++11

RC_FILE = game.rc

INCLUDEPATH += \
	$$PWD/../engine/common2

debug:LIBS += -L$$PWD/../../build_d/
else:LIBS += -L$$PWD/../../build/
LIBS += -lxash

debug:DESTDIR = $$PWD/../../build_d/
else:DESTDIR = $$PWD/../../build/

debug:DEFINES += _DEBUG _WINDOWS WIN32
else:DEFINES += NDEBUG _MBCS _WINDOWS WIN32
DEFINES -= UNICODE

QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:libcmt
QMAKE_LFLAGS_RELEASE += /NODEFAULTLIB:libcmt

LIBS += -luser32 -lgdi32 -lwinspool -lcomdlg32 \
		-ladvapi32 -lshell32 -lole32 -loleaut32 \
		-lwinmm -lwsock32 -luuid -lodbc32 -lodbccp32

SOURCES += \
	main.cpp

DISTFILES += \
	game.rc

