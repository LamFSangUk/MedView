#include "MainWindow.h"
#include "OpenGLWidget.h"

#include <QLayout>
#include <QMenuBar>
#include <QSize>

#include "volume.h"

MainWindow::MainWindow() {
	this->resize(m_width, m_height);

	buildLayout();

	createMenuBar();
}

void MainWindow::buildLayout() {
	QWidget *window = new QWidget();

	QGridLayout *layout = new QGridLayout(window);

	// Slider size
	QSize max_size_slider(1000, 10);
	QSize min_size_slider(100, 10);

	// Label size
	QSize max_size_label(m_width / 2, m_height / 2);
	QSize min_size_label(512, 512);
	
	/* Axial View*/
	QVBoxLayout *axial_layout = new QVBoxLayout();

	this->m_axial_view = new ImageViewWidget(MODE_AXIAL);
	this->m_axial_view->setMaximumSize(max_size_label);
	this->m_axial_view->setMinimumSize(min_size_label);

	m_axial_slider = new QSlider(Qt::Horizontal, m_axial_view);
	m_axial_slider->setMaximumSize(max_size_slider);
	m_axial_slider->setMinimumSize(min_size_slider);
	m_axial_slider->setEnabled(false);
	m_axial_slider->setObjectName("slider");
	m_axial_view->setSlider(m_axial_slider);


	axial_layout->addWidget(m_axial_view,Qt::AlignCenter);
	axial_layout->addWidget(m_axial_slider,0,Qt::AlignBottom);
	m_axial_slider->setParent(m_axial_view);

	/* Coronal View*/
	QVBoxLayout *coronal_layout = new QVBoxLayout();

	this->m_coronal_view = new ImageViewWidget(MODE_CORONAL);
	this->m_coronal_view->setMaximumSize(max_size_label);
	this->m_coronal_view->setMinimumSize(min_size_label);

	m_coronal_slider = new QSlider(Qt::Horizontal, m_coronal_view);
	m_coronal_slider->setMaximumSize(max_size_slider);
	m_coronal_slider->setMinimumSize(min_size_slider);
	m_coronal_slider->setEnabled(false);
	m_coronal_slider->setObjectName("slider");
	m_coronal_view->setSlider(m_coronal_slider);

	coronal_layout->addWidget(m_coronal_view, Qt::AlignCenter);
	coronal_layout->addWidget(m_coronal_slider, 0, Qt::AlignBottom);

	/* Sagittal View*/
	QVBoxLayout *sagittal_layout = new QVBoxLayout();

	this->m_sagittal_view = new ImageViewWidget(MODE_SAGITTAL);
	this->m_sagittal_view->setMaximumSize(max_size_label);
	this->m_sagittal_view->setMinimumSize(min_size_label);

	m_sagital_slider = new QSlider(Qt::Horizontal, m_sagittal_view);
	m_sagital_slider->setMaximumSize(max_size_slider);
	m_sagital_slider->setMinimumSize(min_size_slider);
	m_sagital_slider->setEnabled(false);
	m_sagital_slider->setObjectName("slider");
	m_sagittal_view->setSlider(m_sagital_slider);


	sagittal_layout->addWidget(m_sagittal_view, Qt::AlignCenter);
	sagittal_layout->addWidget(m_sagital_slider, 0, Qt::AlignBottom);


	QWidget *vrView = new OpenGLWidget();

	layout->addLayout(axial_layout, 0, 0);
	layout->addLayout(coronal_layout, 0, 1);
	layout->addLayout(sagittal_layout, 1, 0);
	layout->addWidget(vrView,1,1);

	window->setLayout(layout);

	setCentralWidget(window);
}

void MainWindow::createMenuBar() {


	QMenu *file;
	file = menuBar()->addMenu("&File");

	QAction *openAction = new QAction("&Open", this);
	file->addAction(openAction);

	QAction *quitAction = new QAction("&Quit", this);
	file->addAction(quitAction);

	connect(openAction, &QAction::triggered, this, &MainWindow::open);
	connect(quitAction, &QAction::triggered, this, &MainWindow::quit);
}

void MainWindow::open() {
	vdcm::Volume* vol = vdcm::read("./35515591");

	this->m_axial_view->setVolume(vol);
	this->m_coronal_view->setVolume(vol);
	this->m_sagittal_view->setVolume(vol);
}

void MainWindow::quit() {
	exit(0);
}