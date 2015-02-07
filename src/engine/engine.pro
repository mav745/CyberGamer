CONFIG(debug, debug|release): CONFIG -= release
else: CONFIG -= debug

QT -= core gui

TEMPLATE = lib
TARGET = xash
CONFIG += create_prl link_prl c++11 plugin

debug:DESTDIR = $$PWD/../../build_d/
else:DESTDIR = $$PWD/../../build/

debug:DLLDESTDIR = $$PWD/../../build_d/
else:DLLDESTDIR = $$PWD/../../build/

debug:DEFINES += _DEBUG _WINDOWS WIN32
else:DEFINES += NDEBUG _WINDOWS WIN32
DEFINES -= UNICODE

INCLUDEPATH += \
	$$PWD \
	$$PWD/common \
	$$PWD/common/imagelib \
	$$PWD/common/soundlib \
	$$PWD/server \
	$$PWD/client \
	$$PWD/client/vgui \
	$$PWD/../common \
	$$PWD/../game_shared \
	$$PWD/../pm_shared \
	$$PWD/../utils/vgui/include

LIBS += -L$$PWD/../utils/vgui/lib/win32_vc6 -L$$PWD/common/soundlib
LIBS += -lvgui -lmpeg

QMAKE_LFLAGS_DEBUG += /NODEFAULTLIB:libc
QMAKE_LFLAGS_RELEASE += /NODEFAULTLIB:libc

LIBS += -luser32 -lgdi32 -lwinspool -lcomdlg32 \
		-ladvapi32 -lshell32 -lole32 -loleaut32 \
		-lwinmm -lwsock32 -luuid -lodbc32 -lodbccp32

HEADERS += \
	client/cl_tent.h \
	client/client.h \
	common/common.h \
	common/crtlib.h \
	common/filesystem.h \
	client/gl_export.h \
	client/gl_local.h \
	common/imagelib/imagelib.h \
	common/library.h \
	common/mathlib.h \
	common/mod_local.h \
	common/net_buffer.h \
	common/net_encode.h \
	common/protocol.h \
	server/server.h \
	client/sound.h \
	common/soundlib/soundlib.h \
	client/vgui/vgui_draw.h \
	client/vgui/vgui_main.h \
	client/vox.h \
	common/world.h

SOURCES += \
	common/avikit.c \
	common/build.c \
	client/cl_cmds.c \
	client/cl_demo.c \
	client/cl_events.c \
	client/cl_frame.c \
	client/cl_game.c \
	client/cl_main.c \
	client/cl_menu.c \
	client/cl_parse.c \
	client/cl_pmove.c \
	client/cl_remap.c \
	client/cl_scrn.c \
	client/cl_tent.c \
	client/cl_video.c \
	client/cl_view.c \
	common/cmd.c \
	common/common.c \
	common/con_utils.c \
	common/console.c \
	common/crclib.c \
	common/crtlib.c \
	common/cvar.c \
	common/filesystem.c \
	common/gamma.c \
	client/gl_backend.c \
	client/gl_beams.c \
	client/gl_cull.c \
	client/gl_decals.c \
	client/gl_draw.c \
	client/gl_image.c \
	client/gl_mirror.c \
	client/gl_refrag.c \
	client/gl_rlight.c \
	client/gl_rmain.c \
	client/gl_rmath.c \
	client/gl_rmisc.c \
	client/gl_rpart.c \
	client/gl_rsurf.c \
	client/gl_sprite.c \
	client/gl_studio.c \
	client/gl_vidnt.c \
	client/gl_warp.c \
	common/host.c \
	common/hpak.c \
	common/imagelib/img_bmp.c \
	common/imagelib/img_main.c \
	common/imagelib/img_quant.c \
	common/imagelib/img_tga.c \
	common/imagelib/img_utils.c \
	common/imagelib/img_wad.c \
	common/infostring.c \
	common/input.c \
	common/keys.c \
	common/library.c \
	common/mathlib.c \
	common/matrixlib.c \
	common/mod_studio.c \
	common/model.c \
	common/net_buffer.c \
	common/net_chan.c \
	common/net_encode.c \
	common/net_huff.c \
	common/network.c \
	common/pm_surface.c \
	common/pm_trace.c \
	common/random.c \
	client/s_backend.c \
	client/s_dsp.c \
	client/s_load.c \
	client/s_main.c \
	client/s_mix.c \
	client/s_mouth.c \
	client/s_stream.c \
	client/s_utils.c \
	client/s_vox.c \
	common/soundlib/snd_main.c \
	common/soundlib/snd_mp3.c \
	common/soundlib/snd_utils.c \
	common/soundlib/snd_wav.c \
	server/sv_client.c \
	server/sv_cmds.c \
	server/sv_custom.c \
	server/sv_frame.c \
	server/sv_game.c \
	server/sv_init.c \
	server/sv_main.c \
	server/sv_move.c \
	server/sv_phys.c \
	server/sv_pmove.c \
	server/sv_save.c \
	server/sv_world.c \
	common/sys_con.c \
	common/sys_win.c \
	common/titles.c \
	client/vgui/vgui_clip.cpp \
	client/vgui/vgui_draw.c \
	client/vgui/vgui_font.cpp \
	client/vgui/vgui_input.cpp \
	client/vgui/vgui_int.cpp \
	client/vgui/vgui_surf.cpp \
	common/world.c \
	common/zone.c
