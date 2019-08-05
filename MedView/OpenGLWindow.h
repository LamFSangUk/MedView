#ifndef __OPENGL_WINDOW_H__
#define __OPENGL_WINDOW_H__

#include <QWindow>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QKeyEvent>
#include<QMatrix4x4>
#include<QQuaternion>

#include "DicomManager.h"
#include "Utils/ArcBall.h"

class QOpenGLShaderProgram;

class OpenGLWindow : public QWindow, protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:
	OpenGLWindow(DicomManager*, QWindow*);
	//~OpenGLWindow();

	void reset();

	void loadObject();
	void render();

	

protected:
	/* Qt Mouse Events */
	void mouseMoveEvent(QMouseEvent *) override;
	void mousePressEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;

	/* OpenGL Events */
	void exposeEvent(QExposeEvent *event);
	void resizeEvent(QResizeEvent *);
private:
	DicomManager *m_dicom_manager;

	QOpenGLContext *m_context;

	GLuint m_vao;
	QOpenGLShaderProgram *m_raycast_firstpass_shader;
	QOpenGLShaderProgram *m_raycast_shader;

	//TODO:: rename this
	bool volumeload = false;
	bool glInitialized = false;

	void _initializeGL();

	void _loadVolume();
	GLuint m_volume_texture;
	QOpenGLTexture *m_volume_texture_new;

	void _initializeTargetTexture(int, int);
	GLuint m_target_texture;
	void _initializeFramebuffer(int, int);
	GLuint m_depth_buffer;
	GLuint m_framebuffer;

	void _renderCube(QOpenGLShaderProgram*, GLuint);

	QMatrix4x4 m_model_mat;
	QMatrix4x4 m_view_mat;
	QMatrix4x4 m_rotation_mat;

	void _initializeMatrix();

	ArcBall* arc;

	/* Mouse cursor status */
	bool m_is_right_pressed;

	/* Cube vertices for drawing volume */
	const GLfloat m_cube_vertices[24] = {
		// front
		0.0, 0.0, 1.0,
		1.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		0.0, 1.0, 1.0,
		// back
		0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 1.0, 0.0,
		0.0, 1.0, 0.0
	};

	const GLuint m_cube_triangle_indices[36] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// top
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// bottom
		4, 0, 3,
		3, 7, 4,
		// left
		4, 5, 1,
		1, 0, 4,
		// right
		3, 2, 6,
		6, 7, 3
	};
};

#endif // __OPENGL_WINDOW_H__