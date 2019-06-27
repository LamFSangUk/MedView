#ifndef __CURSOR_COORDINATOR_H__
#define __CURSOR_COORDINATOR_H__

#include <QObject>
#include <QTextEdit>

#include <string>

class CursorCoordinator : public QTextEdit
{
	Q_OBJECT
public:
	CursorCoordinator(QWidget*);

private slots:
	void setCoord(int, int, int);

private:
	int loc_x, loc_y;	// Location of this components on parent's area
	int x, y, z;		// Values for showing cursor coordinate

	std::string makeCoordString();
};

#endif // __CURSOR_COORDINATOR_H__