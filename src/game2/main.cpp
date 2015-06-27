#include <QApplication>

#include <xash.hpp>

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	CHost h;
	h.main("valve", false);

	return app.exec();
}
