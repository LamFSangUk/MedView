#ifndef __SLICE_WIDGET_H__
#define __SLICE_WIDGET_H__

#include <QLabel>
#include <QImage>
#include "CursorCoordinator.h"


class SliceWidget : public QLabel {
	Q_OBJECT
public:
	SliceWidget(QWidget*);

	void mouseMoveEvent(QMouseEvent *) override;
	void wheelEvent(QWheelEvent *) override;
	void resizeEvent(QResizeEvent *) override;
	void enterEvent(QEvent *) override;
	void leaveEvent(QEvent *) override;

	void drawSlice(QImage*);
	void drawAxes(QPainter*);

	void setLines(QLine, QLine, QColor, QColor);
signals:
	void changeCoords(int, int, int, int);
	void requestIncIndex();
	void requestDecIndex();
private:
	bool m_is_slice_loaded;
	bool m_is_cursor_on;

	QImage m_slice;
	CursorCoordinator* m_cur_coord;

	QLine m_line_horizontal;
	QLine m_line_vertical;
	QColor m_color_line_horizontal;
	QColor m_color_line_vertical;

	QSize m_size;
};

#endif //__SLICE_WIDGET_H__
