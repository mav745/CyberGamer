TEMPLATE = subdirs

SUBDIRS += \
	dlls \
	cl_dll \
	mainui \
	engine \
	game \
	game2

game2.depends = engine
