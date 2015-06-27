CONFIG(debug, debug|release): CONFIG -= release
else: CONFIG -= debug

QT -= core gui

TEMPLATE = lib
TARGET = hl
CONFIG += create_prl link_prl c++11 plugin

debug:DESTDIR = $$PWD/../../build_d/valve/dlls
else:DESTDIR = $$PWD/../../build/valve/dlls

debug:DLLDESTDIR = $$PWD/../../build_d/valve/dlls
else:DLLDESTDIR = $$PWD/../../build/valve/dlls

debug:DEFINES += _DEBUG _MBCS _WINDOWS WIN32 QUIVER VOXEL QUAKE2 VALVE_DLL CLIENT_WEAPONS
else:DEFINES += NDEBUG _WINDOWS WIN32 QUIVER VOXEL QUAKE2 VALVE_DLL CLIENT_WEAPONS
DEFINES -= UNICODE

QMAKE_LFLAGS += /def:"$$PWD/../dlls/hl.def"

INCLUDEPATH += \
	$$PWD/../dlls \
	$$PWD/../common \
	$$PWD/../pm_shared \
	$$PWD/../game_shared \
	$$PWD/../engine \
	$$PWD/../utils/vgui/include

LIBS += -luser32 -lgdi32 -lwinspool -lcomdlg32 \
		-ladvapi32 -lshell32 -lole32 -loleaut32 \
		-lwinmm -lwsock32 -luuid

HEADERS += \
	activity.h \
	activitymap.h \
	animation.h \
	basemonster.h \
	cbase.h \
	cdll_dll.h \
	client.h \
	decals.h \
	defaultai.h \
	doors.h \
	effects.h \
	../engine/eiface.h \
	enginecallback.h \
	explode.h \
	extdll.h \
	flyingmonster.h \
	func_break.h \
	gamerules.h \
	hornet.h \
	items.h \
	monsterevent.h \
	monsters.h \
	nodes.h \
	plane.h \
	player.h \
	../pm_shared/pm_debug.h \
	../pm_shared/pm_defs.h \
	../pm_shared/pm_info.h \
	../pm_shared/pm_materials.h \
	../pm_shared/pm_movevars.h \
	../pm_shared/pm_shared.h \
	saverestore.h \
	schedule.h \
	scripted.h \
	scriptevent.h \
	skill.h \
	soundent.h \
	spectator.h \
	squadmonster.h \
	talkmonster.h \
	teamplay_gamerules.h \
	trains.h \
	util.h \
	vector.h \
	weapons.h

SOURCES += \
	aflock.cpp \
	agrunt.cpp \
	airtank.cpp \
	animating.cpp \
	animation.cpp \
	apache.cpp \
	barnacle.cpp \
	barney.cpp \
	bigmomma.cpp \
	bloater.cpp \
	bmodels.cpp \
	bullsquid.cpp \
	buttons.cpp \
	cbase.cpp \
	client.cpp \
	combat.cpp \
	controller.cpp \
	crossbow.cpp \
	crowbar.cpp \
	defaultai.cpp \
	doors.cpp \
	effects.cpp \
	egon.cpp \
	explode.cpp \
	flyingmonster.cpp \
	func_break.cpp \
	func_tank.cpp \
	game.cpp \
	gamerules.cpp \
	gargantua.cpp \
	gauss.cpp \
	genericmonster.cpp \
	ggrenade.cpp \
	globals.cpp \
	glock.cpp \
	gman.cpp \
	h_ai.cpp \
	h_battery.cpp \
	h_cine.cpp \
	h_cycler.cpp \
	h_export.cpp \
	handgrenade.cpp \
	hassassin.cpp \
	headcrab.cpp \
	healthkit.cpp \
	hgrunt.cpp \
	hornet.cpp \
	hornetgun.cpp \
	houndeye.cpp \
	ichthyosaur.cpp \
	islave.cpp \
	items.cpp \
	leech.cpp \
	lights.cpp \
	maprules.cpp \
	monstermaker.cpp \
	monsters.cpp \
	monsterstate.cpp \
	mortar.cpp \
	mp5.cpp \
	multiplay_gamerules.cpp \
	nihilanth.cpp \
	nodes.cpp \
	osprey.cpp \
	pathcorner.cpp \
	plane.cpp \
	plats.cpp \
	player.cpp \
	../pm_shared/pm_debug.cpp \
	../pm_shared/pm_math.cpp \
	../pm_shared/pm_shared.cpp \
	python.cpp \
	rat.cpp \
	roach.cpp \
	rpg.cpp \
	satchel.cpp \
	schedule.cpp \
	scientist.cpp \
	scripted.cpp \
	shotgun.cpp \
	singleplay_gamerules.cpp \
	skill.cpp \
	sound.cpp \
	soundent.cpp \
	spectator.cpp \
	squadmonster.cpp \
	squeakgrenade.cpp \
	subs.cpp \
	talkmonster.cpp \
	teamplay_gamerules.cpp \
	tempmonster.cpp \
	tentacle.cpp \
	triggers.cpp \
	tripmine.cpp \
	turret.cpp \
	util.cpp \
	../game_shared/voice_gamemgr.cpp \
	weapons.cpp \
	world.cpp \
	xen.cpp \
	zombie.cpp
