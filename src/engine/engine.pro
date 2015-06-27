CONFIG(debug, debug|release): CONFIG -= release
else: CONFIG -= debug

QT += core gui widgets opengl

TEMPLATE = lib
TARGET = xash
CONFIG += create_prl link_prl c++11

debug:DESTDIR = $$PWD/../../build_d/
else:DESTDIR = $$PWD/../../build/

debug:DLLDESTDIR = $$PWD/../../build_d/
else:DLLDESTDIR = $$PWD/../../build/

debug:DEFINES += _DEBUG _WINDOWS WIN32
else:DEFINES += NDEBUG _WINDOWS WIN32
DEFINES -= UNICODE

DEFINES += XASH_LIB

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
	common/world.h \
	common2/xash.hpp \
#	common2/common.h \
#	common2/crtlib.h \
	common2/fs.hpp \
#	common2/input.h \
#	common2/library.h \
#	common2/mathlib.h \
#	common2/mod_local.h \
#	common2/net_buffer.h \
#	common2/net_encode.h \
#	common2/netchan.h \
#	common2/pm_local.h \
#	common2/protocol.h \
#	common2/system.h \
#	common2/world.h \
	common2/xash_types.hpp \
	common2/console2.hpp \
	common2/cmd2.hpp \
	common2/cvar2.hpp \
	anorms.h \
	cdll_exp.h \
	cdll_int.h \
	custom.h \
	customentity.h \
	edict.h \
	eiface.h \
	keydefs.h \
	menu_int.h \
	physint.h \
	progdefs.h \
	shake.h \
	sprite.h \
	studio.h \
	warpsin.h \
	common2/library2.hpp

SOURCES += \
	client/vgui/vgui_clip.cpp \
	client/vgui/vgui_font.cpp \
	client/vgui/vgui_input.cpp \
	client/vgui/vgui_int.cpp \
	client/vgui/vgui_surf.cpp \
	client/vgui/vgui_draw.cpp \
	client/cl_cmds.cpp \
	client/cl_demo.cpp \
	client/cl_events.cpp \
	client/cl_frame.cpp \
	client/cl_game.cpp \
	client/cl_main.cpp \
	client/cl_menu.cpp \
	client/cl_parse.cpp \
	client/cl_pmove.cpp \
	client/cl_remap.cpp \
	client/cl_scrn.cpp \
	client/cl_tent.cpp \
	client/cl_video.cpp \
	client/cl_view.cpp \
	client/gl_backend.cpp \
	client/gl_beams.cpp \
	client/gl_cull.cpp \
	client/gl_decals.cpp \
	client/gl_draw.cpp \
	client/gl_image.cpp \
	client/gl_mirror.cpp \
	client/gl_refrag.cpp \
	client/gl_rlight.cpp \
	client/gl_rmain.cpp \
	client/gl_rmath.cpp \
	client/gl_rmisc.cpp \
	client/gl_rpart.cpp \
	client/gl_rsurf.cpp \
	client/gl_sprite.cpp \
	client/gl_studio.cpp \
	client/gl_vidnt.cpp \
	client/gl_warp.cpp \
	client/s_backend.cpp \
	client/s_dsp.cpp \
	client/s_load.cpp \
	client/s_main.cpp \
	client/s_mix.cpp \
	client/s_mouth.cpp \
	client/s_stream.cpp \
	client/s_utils.cpp \
	client/s_vox.cpp \
	server/sv_client.cpp \
	server/sv_cmds.cpp \
	server/sv_custom.cpp \
	server/sv_frame.cpp \
	server/sv_game.cpp \
	server/sv_init.cpp \
	server/sv_main.cpp \
	server/sv_move.cpp \
	server/sv_phys.cpp \
	server/sv_pmove.cpp \
	server/sv_save.cpp \
	server/sv_world.cpp \
	common/avikit.cpp \
	common/build.cpp \
	common/cmd.cpp \
	common/common.cpp \
	common/con_utils.cpp \
	common/console.cpp \
	common/crclib.cpp \
	common/crtlib.cpp \
	common/cvar.cpp \
	common/filesystem.cpp \
	common/gamma.cpp \
	common/host.cpp \
	common/hpak.cpp \
	common/infostring.cpp \
	common/input.cpp \
	common/keys.cpp \
	common/library.cpp \
	common/mathlib.cpp \
	common/matrixlib.cpp \
	common/mod_studio.cpp \
	common/model.cpp \
	common/net_buffer.cpp \
	common/net_chan.cpp \
	common/net_encode.cpp \
	common/net_huff.cpp \
	common/network.cpp \
	common/pm_surface.cpp \
	common/pm_trace.cpp \
	common/random.cpp \
	common/sys_con.cpp \
	common/sys_win.cpp \
	common/titles.cpp \
	common/world.cpp \
	common/zone.cpp \
	common/imagelib/img_bmp.cpp \
	common/imagelib/img_main.cpp \
	common/imagelib/img_quant.cpp \
	common/imagelib/img_tga.cpp \
	common/imagelib/img_utils.cpp \
	common/imagelib/img_wad.cpp \
	common/soundlib/snd_main.cpp \
	common/soundlib/snd_mp3.cpp \
	common/soundlib/snd_utils.cpp \
	common/soundlib/snd_wav.cpp \
#	common2/avikit2.cpp \
#	common2/build2.cpp \
	common2/cmd2.cpp \
#	common2/common2.cpp \
#	common2/con_utils2.cpp \
	common2/console2.cpp \
#	common2/crclib2.cpp \
#	common2/crtlib2.cpp \
	common2/cvar2.cpp \
	common2/fs.cpp \
#	common2/gamma2.cpp \
#	common2/hpak2.cpp \
#	common2/infostring2.cpp \
#	common2/input2.cpp \
#	common2/keys2.cpp \
#	common2/library2.cpp \
#	common2/mathlib2.cpp \
#	common2/matrixlib2.cpp \
#	common2/mod_studio2.cpp \
#	common2/model2.cpp \
#	common2/net_buffer2.cpp \
#	common2/net_chan2.cpp \
#	common2/net_encode2.cpp \
#	common2/net_huff2.cpp \
#	common2/network2.cpp \
#	common2/pm_surface2.cpp \
#	common2/pm_trace2.cpp \
#	common2/random2.cpp \
#	common2/sys_con2.cpp \
#	common2/sys_win2.cpp \
#	common2/titles2.cpp \
#	common2/world2.cpp \
#	common2/zone2.cpp \
	common2/xash.cpp \
	common2/library2.cpp
