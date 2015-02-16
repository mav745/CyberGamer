#include "game.hpp"

#include <QApplication>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	Game g;

	return app.exec();
}
