TEMPLATE = subdirs

SUBDIRS += \
	dlls \
	cl_dll \
	mainui \
	engine \
	game

game.depends = cl_dll dlls mainui engine
