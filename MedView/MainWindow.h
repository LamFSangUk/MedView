#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__

#include <QMainWindow>
#include <QApplication>
#include "ImageViewWidget.h"

class MainWindow : public QMainWindow
{
public:
	MainWindow();

private:
	void buildLayout();
	void createMenuBar();

	const int m_height = 1080;
	const int m_width = 1280;

private slots:
	void open();
	void quit();

	ImageViewWidget *m_axial_view;
	ImageViewWidget *m_coronial_view;
	ImageViewWidget *m_sagittal_view;
};

#endif //__MAIN_WINDOW__