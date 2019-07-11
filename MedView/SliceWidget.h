#ifndef __SLICE_WIDGET_H__
#define __SLICE_WIDGET_H__

#include <QLabel>
#include <QImage>
#include "CursorCoordinator.h"


class SliceWidget : public QLabel {
	Q_OBJECT
public:
	SliceWidget(QWidget*);

	// Qt Event handler
	void mouseMoveEvent(QMouseEvent *) override;
	void mousePressEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	void wheelEvent(QWheelEvent *) override;
	void resizeEvent(QResizeEvent *) override;
	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;

	void drawSlice(QImage*);
	void drawAxes(QPainter*);

	void setLines(QLine, QLine, QColor, QColor);
signals:
	void changeCoords(int, int, int, int);
	void changeDegree(int, float);
	void requestIncIndex();
	void requestDecIndex();
private:
	bool m_is_slice_loaded;		// check the slice to show is loaded
	bool m_is_cursor_on;		// check the mouse cursor is on the label
	bool m_is_left_pressed;		// check the left mouse button is pressed
	bool m_is_point_on_lines;

	QImage m_slice;
	CursorCoordinator* m_cur_coord;

	QLine m_line_horizontal;
	QLine m_line_vertical;
	QColor m_color_line_horizontal;
	QColor m_color_line_vertical;

	QSize m_size;

	QPoint m_prev_cursor_point;

	bool is_point_on_lines(QPoint);
};

#endif //__SLICE_WIDGET_H__
