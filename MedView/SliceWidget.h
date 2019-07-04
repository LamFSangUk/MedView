#ifndef __SLICE_WIDGET_H__
#define __SLICE_WIDGET_H__

#include <QLabel>
#include <QImage>
#include "CursorCoordinator.h"


class SliceWidget : public QLabel {
	Q_OBJECT
public:
	SliceWidget(QWidget*);
	void mouseMoveEvent(QMouseEvent *e) override;
	void drawSlice(QImage*);

signals:
	void changeCoords(int, int, int);
private:
	QImage* m_slice;
	CursorCoordinator* m_cur_coord;

};

#endif //__SLICE_WIDGET_H__
