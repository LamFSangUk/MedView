#include "MainWindow.h"
#include "OpenGLWidget.h"

#include <QLayout>
#include <QMenuBar>

#include "volume.h"

MainWindow::MainWindow() {
	this->resize(m_width, m_height);

	buildLayout();

	createMenuBar();
}

void MainWindow::buildLayout() {
	QWidget *window = new QWidget();

	QGridLayout *layout = new QGridLayout(window);

	this->m_axial_view = new ImageViewWidget(MODE_AXIAL);
	this->m_coronial_view = new ImageViewWidget(MODE_CORONAL);
	this->m_sagittal_view = new ImageViewWidget(MODE_SAGITTAL);
	QWidget *vrView = new OpenGLWidget();

	layout->addWidget((QWidget*)m_axial_view, 0, 0);
	layout->addWidget((QWidget*)m_coronial_view, 0, 1);
	layout->addWidget((QWidget*)m_sagittal_view, 1, 0);
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
	this->m_coronial_view->setVolume(vol);
	this->m_sagittal_view->setVolume(vol);

}

void MainWindow::quit() {
	exit(0);
}