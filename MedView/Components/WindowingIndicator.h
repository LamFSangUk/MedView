#ifndef __WINDOWING_INDICATOR_H__
#define __WINDOWING_INDICATOR_H__

#include <QTextEdit>

class WindowingIndicator : public QTextEdit {
	Q_OBJECT;
public:
	WindowingIndicator(QWidget*);
	void setWindowingValue(int, int);

private:
	int m_windowing_level;
	int m_windowing_width;

	std::string WindowingIndicator::makeString();
};

#endif // __WINDOWING_INDICATOR_H__