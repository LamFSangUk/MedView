#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__

#include <QMainWindow>
#include <QApplication>
#include <QSlider>

#include "ImageViewWidget.h"
#include "VolumeViewWidget.h"
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
	void _buildLayout();
	void _createMenuBar();

	/* default size of window */
	const int m_height = 1200;
	const int m_width = 1200;

	/* widgets */
	ImageViewWidget *m_axial_view;
	ImageViewWidget *m_coronal_view;
	ImageViewWidget *m_sagittal_view;
	VolumeViewWidget *m_vr_view;

	/* data manager */
	DicomManager *m_dicom_manager;
};

#endif //__MAIN_WINDOW__