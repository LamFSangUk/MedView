#ifndef __CURSOR_COORDINATOR_H__
#define __CURSOR_COORDINATOR_H__

#include <QTextEdit>

#include <string>

class CursorCoordinator : public QTextEdit
{
	Q_OBJECT
public:
	CursorCoordinator(QWidget*);

private slots:
	void setCoord(int, int, int, int);

private:
	int x, y, z;		// Values for showing cursor coordinate
	int intensity;

	std::string makeCoordString();
};

#endif // __CURSOR_COORDINATOR_H__