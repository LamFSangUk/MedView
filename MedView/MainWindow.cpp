#include "MainWindow.h"
#include "OpenGLWidget.h"

#include <QLayout>
#include <QMenuBar>
#include <QSize>
#include <QFileDialog>

#include "volume.h"

MainWindow::MainWindow() {
	this->resize(m_width, m_height);

	// Create Dicom data manager
	this->m_dicom_manager = new DicomManager(this);

	buildLayout();

	createMenuBar();

	connect(m_dicom_manager, &DicomManager::changeVolume, [this] {
		this->setWindowTitle(QString("MedView(")+QString(this->m_dicom_manager->getFilename())+QString(")"));
	});
}

void MainWindow::buildLayout() {
	QWidget *window = new QWidget();

	QGridLayout *layout = new QGridLayout(window);
	
	/* Axial View*/
	this->m_axial_view = new ImageViewWidget(Mode::MODE_AXIAL, this->m_dicom_manager, this);
	this->m_axial_view->setMinimumWidth(512);

	/* Coronal View*/
	this->m_coronal_view = new ImageViewWidget(Mode::MODE_CORONAL, this->m_dicom_manager, this);
	this->m_coronal_view->setMinimumWidth(512);

	/* Sagittal View*/
	this->m_sagittal_view = new ImageViewWidget(Mode::MODE_SAGITTAL, this->m_dicom_manager, this);
	this->m_sagittal_view->setMinimumWidth(512);

	

	QWidget *vrView = new OpenGLWidget();
	vrView->setMinimumWidth(512);

	layout->addWidget(m_axial_view, 0, 0);
	layout->addWidget(m_coronal_view, 1, 0);
	layout->addWidget(m_sagittal_view, 0, 1);
	layout->addWidget(vrView,1,1);

	window->setLayout(layout);

	setCentralWidget(window);
}

void MainWindow::createMenuBar() {


	QMenu *file;
	file = menuBar()->addMenu("&File");

	QAction *openAction = new QAction("&Open", this);
	openAction->setShortcut(Qt::Key_O | Qt::CTRL);
	file->addAction(openAction);

	file->addSeparator();

	QAction *quitAction = new QAction("&Quit", this);
	quitAction->setShortcut(Qt::Key_Q | Qt::CTRL);
	file->addAction(quitAction);

	connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
	connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

	QMenu *view;
	view = menuBar()->addMenu("&View");
	
	QAction *resetAction = new QAction("&Reset", this);
	resetAction->setShortcut(Qt::Key_R | Qt::CTRL);
	view->addAction(resetAction);

	connect(resetAction, SIGNAL(triggered()), this, SLOT(reset()));

}

void MainWindow::open() {
	QString str_dicom_folder = QFileDialog::getExistingDirectory(this, "Open DICOM folder", QDir::currentPath(), QFileDialog::ShowDirsOnly);

	if (!str_dicom_folder.isEmpty() && !str_dicom_folder.isNull()) { // Prevent cancelation

		m_dicom_manager->readDicom(str_dicom_folder.toUtf8().toStdString().c_str());
	}
}

void MainWindow::quit() {
	exit(0);
}

void MainWindow::reset() {
	m_dicom_manager->reset();
}