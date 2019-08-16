#ifndef __VOLUME_VIEW_WIDGET_H__
#define __VOLUME_VIEW_WIDGET_H__

#include <QWidget>
#include <QLayout>
#include <QToolButton>
#include <QPushButton>

#include "OpenGLWindow.h"
#include "DicomManager.h"

class VolumeViewWidget : public QWidget
{
	Q_OBJECT
public:
	VolumeViewWidget(DicomManager*, QWidget*);
	//~VolumeViewWidget();

	void reset();
private:
	OpenGLWindow* m_gl_window;
	DicomManager* m_dicom_manager;

	QWidget* container;

	void _buildLayout();

};

#endif // __VOLUME_VIEW_WIDGET_H__