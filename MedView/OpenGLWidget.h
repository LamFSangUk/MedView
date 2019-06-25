#ifndef __OPENGLWIDGET__
#define __OPENGLWIDGET__

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class QOpenGLShaderProgram;

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	OpenGLWidget();
	//~OpenGLWindow();

	void loadObject();

protected:
	void initializeGL();
	void paintGL() Q_DECL_OVERRIDE;
	//void resizeGL(int w, int h) Q_DECL_OVERRIDE;

private:

	QOpenGLBuffer m_vertex;
	QOpenGLVertexArrayObject m_object;
	QOpenGLShaderProgram *m_program;

};

#endif // __OPENGLWINDOW__