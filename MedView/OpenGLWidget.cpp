#include "OpenGLWidget.h"
#include <QOpenGLShaderProgram>
#include "vertex.h"

// Create a colored triangle
const static Vertex sg_vertexes[] = {
  Vertex(QVector3D(0.00f,  0.75f, 1.0f), QVector3D(1.0f, 0.0f, 0.0f)),
  Vertex(QVector3D(-0.75f, -0.75f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f)),
  Vertex(QVector3D(0.75f, -0.75f, 1.0f), QVector3D(0.0f, 0.0f, 1.0f))
};

OpenGLWidget::OpenGLWidget()
	: QOpenGLWidget()
{
	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	this->setFormat(format);

	qDebug("create");
}

void OpenGLWidget::initializeGL() {
	qDebug("init");

	initializeOpenGLFunctions();

	// set global information
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Application-specific initialization
	{
		// Create Shader
		m_program = new QOpenGLShaderProgram();

		m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/simple.vert");
		m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/simple.frag");
		m_program->link();
		loadObject();
	}
}

void OpenGLWidget::loadObject() {

	m_program->bind();
	{
		// Create Buffer
		m_vertex.create();
		m_vertex.bind();
		m_vertex.setUsagePattern(QOpenGLBuffer::StaticDraw);
		m_vertex.allocate(sg_vertexes, sizeof(sg_vertexes));

		// Create VAO
		m_object.create();
		m_object.bind();
		m_program->enableAttributeArray(0);
		m_program->enableAttributeArray(1);
		m_program->setAttributeBuffer(0, GL_FLOAT, Vertex::positionOffset(), Vertex::PositionTupleSize, Vertex::stride());
		m_program->setAttributeBuffer(1, GL_FLOAT, Vertex::colorOffset(), Vertex::ColorTupleSize, Vertex::stride());

		//Release all
		m_object.release();
		m_vertex.release();
	}
	m_program->release();
	qDebug("loaded");
}

void OpenGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_program->bind();
	{
		m_object.bind();
		glDrawArrays(GL_TRIANGLES, 0, sizeof(sg_vertexes) / sizeof(sg_vertexes[0]));
		m_object.release();
	}
	m_program->release();
}