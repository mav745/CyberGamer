CONFIG(debug, debug|release): CONFIG -= release
else: CONFIG -= debug

QT -= core gui

TEMPLATE = lib
TARGET = mainui
CONFIG += create_prl link_prl c++11 plugin

debug:DESTDIR = $$PWD/../../build_d/
else:DESTDIR = $$PWD/../../build/

debug:DLLDESTDIR = $$PWD/../../build_d/
else:DLLDESTDIR = $$PWD/../../build/

debug:DEFINES += _DEBUG _MBCS _WINDOWS WIN32 _USRDLL PLATFORM_EXPORTS
else:DEFINES += NDEBUG _MBCS _WINDOWS WIN32 _USRDLL PLATFORM_EXPORTS
DEFINES -= UNICODE

INCLUDEPATH += \
	$$PWD/../common \
	$$PWD/../engine \
	$$PWD/../pm_shared

#QMAKE_LFLAGS += /NODEFAULTLIB:msvcrt

LIBS += -luser32 -lgdi32 -lwinspool -lcomdlg32 \
		-ladvapi32 -lshell32 -lole32 -loleaut32 \
		-lwinmm -lwsock32 -luuid -lodbc32 -lodbccp32

HEADERS += \
	basemenu.h \
	enginecallback.h \
	extdll.h \
	menu_btnsbmp_table.h \
	menu_strings.h \
	menufont.H \
	ui_title_anim.h \
	utils.h

SOURCES += \
	basemenu.cpp \
	menu_advcontrols.cpp \
	menu_audio.cpp \
	menu_btns.cpp \
	menu_configuration.cpp \
	menu_controls.cpp \
	menu_creategame.cpp \
	menu_credits.cpp \
	menu_customgame.cpp \
	menu_gameoptions.cpp \
	menu_internetgames.cpp \
	menu_langame.cpp \
	menu_loadgame.cpp \
	menu_main.cpp \
	menu_multiplayer.cpp \
	menu_newgame.cpp \
	menu_playersetup.cpp \
	menu_savegame.cpp \
	menu_saveload.cpp \
	menu_strings.cpp \
	menu_video.cpp \
	menu_vidmodes.cpp \
	menu_vidoptions.cpp \
	udll_int.cpp \
	ui_title_anim.cpp \
	utils.cpp
