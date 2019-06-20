#ifndef __MAIN_WIDGET__
#define __MAIN_WIDGET__

#include <QMainWindow>
#include <QApplication>

class MainWindow : public QMainWindow
{
public:
	MainWindow();

private:
	void buildLayout();
	void createMenuBar();

	const int m_height = 760;
	const int m_width = 1080;

private slots:
	void open();
	void quit();
};

#endif // __MAIN_WIDGET__