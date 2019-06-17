#include "MedView.h"

MedView::MedView(QWindow *parent)
	: QWindow(parent)
	,m_animating(false)
	,m_context(0)
	,m_device(0)
{
	setSurfaceType(QWindow::OpenGLSurface);
}
