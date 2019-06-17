#pragma once

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

class MedView : public QWindow, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit MedView(QWindow *parent = Q_NULLPTR);
	/*~MedView();

	virtual void render(QPainter *painter);
	virtual void render();

	virtual void initialize();

	void setAnimating(bool animating);
	*/
public slots:
	/*void renderLater();
	void renderNow();*/

protected:
	/*
	bool event(QEvent *event) override;

	void exposeEvent(QExposeEvent *event) override;
	*/

private:
	bool m_animating;
	QOpenGLContext *m_context;
	QOpenGLPaintDevice *m_device;
};
