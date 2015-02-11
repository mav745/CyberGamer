CONFIG(debug, debug|release): CONFIG -= release
else: CONFIG -= debug

QT -= core gui

TEMPLATE = app
TARGET = hl
CONFIG += link_prl c++11

RC_FILE = game.rc

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
	game.cpp

DISTFILES += \
	game.rc
