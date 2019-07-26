#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__

#include <QMainWindow>
#include <QApplication>
#include <QSlider>

#include "ImageViewWidget.h"
#include "DicomManager.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();

signals:

private slots:
	void open();
	void quit();
	void reset();

private:
	void buildLayout();
	void createMenuBar();

	const int m_height = 1080;
	const int m_width = 1080;

	ImageViewWidget *m_axial_view;
	ImageViewWidget *m_coronal_view;
	ImageViewWidget *m_sagittal_view;
	
	QSlider *m_axial_slider;
	QSlider *m_coronal_slider;
	QSlider *m_sagital_slider;

	QWidget *m_vr_view;

	DicomManager *m_dicom_manager;
};

#endif //__MAIN_WINDOW__