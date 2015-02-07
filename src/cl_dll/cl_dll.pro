CONFIG(debug, debug|release): CONFIG -= release
else: CONFIG -= debug

QT -= core gui

TEMPLATE = lib
TARGET = client
CONFIG += create_prl link_prl c++11 plugin

debug:DESTDIR = $$PWD/../../build_d/valve/cl_dlls
else:DESTDIR = $$PWD/../../build/valve/cl_dlls

debug:DLLDESTDIR = $$PWD/../../build_d/valve/cl_dlls
else:DLLDESTDIR = $$PWD/../../build/valve/cl_dlls

debug:DEFINES += _DEBUG _MBCS _WINDOWS WIN32 CLIENT_DLL CLIENT_WEAPONS
else:DEFINES += NDEBUG _WINDOWS WIN32 CLIENT_DLL CLIENT_WEAPONS

INCLUDEPATH += \
	$$PWD/../dlls \
	$$PWD/../common \
	$$PWD/../pm_shared \
	$$PWD/../engine \
	$$PWD/../utils/vgui/include

debug:LIBS += -L$$PWD/../utils/vgui/lib/win32_vc6
else:LIBS += -L$$PWD/../utils/vgui/lib/win32_vc6

LIBS += -lvgui
LIBS += -luser32 -lgdi32 -lwinspool -lcomdlg32 \
		-ladvapi32 -lshell32 -lole32 -loleaut32 \
		-lwinmm -lwsock32 -luuid -ladvapi32

HEADERS += \
	ammo.h \
	ammohistory.h \
	camera.h \
	cl_dll.h \
	cl_util.h \
	com_weapons.h \
	demo.h \
	ev_hldm.h \
	eventscripts.h \
	GameStudioModelRenderer.h \
	#GameStudioModelRenderer_Sample.h \
	health.h \
	hud.h \
	hud_iface.h \
	hud_servers.h \
	hud_servers_priv.h \
	hud_spectator.h \
	in_defs.h \
	kbutton.h \
	#overview.h \
	parsemsg.h \
	studio_util.h \
	StudioModelRenderer.h \
	tf_defs.h \
	util_vector.h \
	vgui_ConsolePanel.h \
	vgui_ControlConfigPanel.h \
	vgui_int.h \
	vgui_SchemeManager.h \
	vgui_ScorePanel.h \
	vgui_ServerBrowser.h \
	vgui_SpectatorPanel.h \
	vgui_TeamFortressViewport.h \
	view.h \
	wrect.h \
	..\common\itrackeruser.h \
	..\pm_shared\pm_debug.h \
	..\pm_shared\pm_defs.h \
	..\pm_shared\pm_info.h \
	..\pm_shared\pm_materials.h \
	..\pm_shared\pm_movevars.h \
	..\pm_shared\pm_shared.h \
	..\game_shared\voice_banmgr.h \
	..\game_shared\voice_status.h \
	..\game_shared\voice_vgui_tweakdlg.h \
	../game_shared/vgui_scrollbar2.h \
	../game_shared/vgui_slider2.h

SOURCES += \
	ammo.cpp \
	ammo_secondary.cpp \
	ammohistory.cpp \
	battery.cpp \
	cdll_int.cpp \
	com_weapons.cpp \
	death.cpp \
	demo.cpp \
	entity.cpp \
	ev_common.cpp \
	ev_hldm.cpp \
	events.cpp \
	flashlight.cpp \
	GameStudioModelRenderer.cpp \
	#GameStudioModelRenderer_Sample.cpp \
	geiger.cpp \
	health.cpp \
	hl/hl_baseentity.cpp \
	hl/hl_events.cpp \
	hl/hl_objects.cpp \
	hl/hl_weapons.cpp \
	hud.cpp \
	hud_msg.cpp \
	hud_redraw.cpp \
	hud_servers.cpp \
	hud_spectator.cpp \
	hud_update.cpp \
	in_camera.cpp \
	input.cpp \
	inputw32.cpp \
	menu.cpp \
	message.cpp \
	#overview.cpp \
	parsemsg.cpp \
	saytext.cpp \
	#soundsystem.cpp \
	status_icons.cpp \
	statusbar.cpp \
	studio_util.cpp \
	StudioModelRenderer.cpp \
	text_message.cpp \
	train.cpp \
	tri.cpp \
	util.cpp \
	vgui_ClassMenu.cpp \
	vgui_ConsolePanel.cpp \
	vgui_ControlConfigPanel.cpp \
	vgui_CustomObjects.cpp \
	vgui_int.cpp \
	vgui_MOTDWindow.cpp \
	vgui_SchemeManager.cpp \
	vgui_ScorePanel.cpp \
	vgui_ServerBrowser.cpp \
	vgui_SpectatorPanel.cpp \
	vgui_TeamFortressViewport.cpp \
	vgui_teammenu.cpp \
	view.cpp \
	..\dlls\glock.cpp \
	..\dlls\hornetgun.cpp \
	..\dlls\mp5.cpp \
	..\dlls\python.cpp \
	..\dlls\rpg.cpp \
	..\dlls\satchel.cpp \
	..\dlls\shotgun.cpp \
	..\dlls\squeakgrenade.cpp \
	..\dlls\tripmine.cpp \
	..\game_shared\vgui_scrollbar2.cpp \
	..\game_shared\vgui_slider2.cpp \
	..\game_shared\voice_banmgr.cpp \
	..\game_shared\voice_status.cpp \
	..\pm_shared\pm_debug.c \
	..\pm_shared\pm_math.c \
	..\pm_shared\pm_shared.c \
	..\game_shared\vgui_checkbutton2.cpp \
	..\game_shared\vgui_grid.cpp \
	..\game_shared\vgui_helpers.cpp \
	..\game_shared\vgui_listbox.cpp \
	..\game_shared\vgui_loadtga.cpp \
	../dlls/crossbow.cpp \
	../dlls/crowbar.cpp \
	../dlls/egon.cpp \
	../dlls/gauss.cpp \
	../dlls/handgrenade.cpp
