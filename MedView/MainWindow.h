#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__

#include <QMainWindow>
#include <QApplication>
#include <QSlider>

#include "ImageViewWidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();

private slots:
	void open();
	void quit();

private:
	void buildLayout();
	void createMenuBar();

	const int m_height = 1080;
	const int m_width = 1280;

	ImageViewWidget *m_axial_view;
	ImageViewWidget *m_coronal_view;
	ImageViewWidget *m_sagittal_view;
	QSlider *m_axial_slider;
	QSlider *m_coronal_slider;
	QSlider *m_sagital_slider;
};

#endif //__MAIN_WINDOW__