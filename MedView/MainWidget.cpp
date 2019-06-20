#include "MainWidget.h"
#include "ImageViewWidget.h"
#include "OpenGLWindow.h"

#include <QLayout>
#include <QMenuBar>

MainWindow::MainWindow() {
	this->resize(m_width, m_height);

	buildLayout();

	createMenuBar();
}

void MainWindow::buildLayout() {
	QWidget *window = new QWidget();

	QGridLayout *layout = new QGridLayout(window);

	QWidget *axialView = new ImageViewWidget();
	QWidget *coronialView = new ImageViewWidget();
	QWidget *sagittalView = new ImageViewWidget();
	QWidget *vrView = new OpenGLWidget();

	layout->addWidget(axialView, 0, 0);
	layout->addWidget(coronialView, 0, 1);
	layout->addWidget(sagittalView, 1, 0);
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

}

void MainWindow::quit() {
	exit(0);
}