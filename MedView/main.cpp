#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	/*QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3, 3);*/

	MainWindow window;
	//window.setFormat(format);
	//window.resize(640, 480);

	window.show();

	return app.exec();
}
