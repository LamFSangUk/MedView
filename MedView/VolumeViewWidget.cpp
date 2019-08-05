#include "VolumeViewWidget.h"

VolumeViewWidget::VolumeViewWidget(DicomManager* dicom_manager, QWidget* parent = NULL) 
	:QWidget(parent)
{
	m_dicom_manager = dicom_manager;

	_buildLayout();
}

void VolumeViewWidget::_buildLayout() {
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(8, 31, 0, 24);

	m_gl_window = new OpenGLWindow(m_dicom_manager, 0);
	m_gl_window->setMinimumSize(QSize(512,512));

	QWidget *container = QWidget::createWindowContainer(m_gl_window);
	container->setContentsMargins(10, 10, 10, 10);
	container->setMinimumSize(QSize(512, 512));

	layout->addWidget(container, 0);

	this->setLayout(layout);
}