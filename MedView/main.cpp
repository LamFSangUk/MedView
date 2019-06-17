#include "MedView.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	MedView w;
	w.show();
	return app.exec();
}
