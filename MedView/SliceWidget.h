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
	void drawAxes(QPainter*);

	void setLines(QLine, QLine, QColor, QColor);
signals:
	void changeCoords(int, int, int);
private:
	QImage m_slice;
	CursorCoordinator* m_cur_coord;

	QLine m_line_horizontal;
	QLine m_line_vertical;
	QColor m_color_line_horizontal;
	QColor m_color_line_vertical;
};

#endif //__SLICE_WIDGET_H__
