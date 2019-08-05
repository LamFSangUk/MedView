#ifndef __SLICE_WIDGET_H__
#define __SLICE_WIDGET_H__

#include <QLabel>
#include <QImage>
#include "CursorCoordinator.h"

#include "common.h"

class SliceWidget : public QLabel {
	Q_OBJECT
public:
	SliceWidget(QWidget*);

	// Qt Event handler
	void mouseMoveEvent(QMouseEvent *) override;
	void mousePressEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	void mouseDoubleClickEvent(QMouseEvent*) override;
	void wheelEvent(QWheelEvent *) override;
	void resizeEvent(QResizeEvent *) override;
	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;

	void drawSlice(QImage*);
	void drawAxes(QPainter*);

	void setLines(QLine, QLine, QColor, QColor);

	QSize getSize();
signals:
	void changeCoords(int, int, int, int);
	void changeDegree(float);
	void requestIncIndex();
	void requestDecIndex();
	void changeWindowing(int, int);
	void changeAxesCenter(int, int);

private:
	bool m_is_slice_loaded;		// check the slice to show is loaded
	bool m_is_cursor_on;		// check the mouse cursor is on the label
	bool m_is_left_pressed;		// check the left mouse button is pressed
	bool m_is_right_pressed;	// check the right mouse button is pressed
	bool m_is_point_on_lines;	// check the cursor is on lines
	bool m_is_point_inside_circle;

	QImage m_slice;
	CursorCoordinator* m_cur_coord;

	QLine m_line_horizontal;
	QLine m_line_vertical;
	QColor m_color_line_horizontal;
	QColor m_color_line_vertical;
	QPointF m_line_meet;		// The meet point of two lines

	const int m_circle_radius = 10;	// radius of circle which is on meet of lines

	QSize m_size;

	QPoint m_prev_cursor_point;

	bool _isPointOnLines(QPoint);
	bool _isPointInsideCircle(QPoint);
	bool _isPointNearPoint(QPointF, QPointF, int);
};

inline QSize SliceWidget::getSize() {
	return m_size;
}

#endif //__SLICE_WIDGET_H__
