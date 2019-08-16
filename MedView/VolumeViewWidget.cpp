#include "VolumeViewWidget.h"
#include <QToolBar>

VolumeViewWidget::VolumeViewWidget(DicomManager* dicom_manager, QWidget* parent = NULL) 
	:QWidget(parent)
{
	m_dicom_manager = dicom_manager;

	_buildLayout();
}

void VolumeViewWidget::_buildLayout() {
	QVBoxLayout *layout = new QVBoxLayout();
	layout->setContentsMargins(8, 0, 0, 23);

	QHBoxLayout *tool_bar_layout = new QHBoxLayout();
	tool_bar_layout->setContentsMargins(0, 0, 0, 0);
	tool_bar_layout->setAlignment(Qt::AlignLeft);

	QAction* click_mip_action = new QAction();
	QToolButton *tb_mip = new QToolButton(this);
	tb_mip->setMinimumSize(10, 30);
	tool_bar_layout->addWidget(tb_mip);
	tb_mip->setToolButtonStyle(Qt::ToolButtonTextOnly);
	click_mip_action->setText("MIP");
	tb_mip->setDefaultAction(click_mip_action);

	QAction* click_otf_action = new QAction();
	QToolButton *tb_otf = new QToolButton(this);
	tb_otf->setMinimumSize(10, 30);
	tool_bar_layout->addWidget(tb_otf);
	tb_otf->setToolButtonStyle(Qt::ToolButtonTextOnly);
	click_otf_action->setText("OTF");
	tb_otf->setDefaultAction(click_otf_action);

	connect(tb_mip, &QToolButton::triggered, [this] {
		m_gl_window->setRenderMode("MIP");
	});
	connect(tb_otf, &QToolButton::triggered, [this] {
		m_gl_window->setRenderMode("OTF");
	});

	m_gl_window = new OpenGLWindow(m_dicom_manager, 0);

	container = QWidget::createWindowContainer(m_gl_window);
	container->setContentsMargins(0, 0, 0, 0);

	layout->addLayout(tool_bar_layout);
	layout->addSpacing(30);
	layout->addWidget(container);

	this->setLayout(layout);
}

void VolumeViewWidget::reset() {
	m_gl_window->reset();
}