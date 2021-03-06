#include "SliceWidget.h"

#include <QMouseEvent>
#include <QDebug>
#include <QPainter>

#include "ImageViewWidget.h"

#include <cmath>
#define M_PI       3.14159265358979323846


SliceWidget::SliceWidget(QWidget* parent=NULL)
	: QLabel(parent)
{
	m_is_cursor_on = false;
	m_is_slice_loaded = false;
	m_is_left_pressed = false;
	m_is_right_pressed = false;
	m_is_point_on_lines = false;
	m_is_point_inside_circle = false;

	// Set default background color
	this->setStyleSheet("QLabel { background-color : black; }");

	// For tracking mouse
	this->setMouseTracking(true);

	// Display mouse coordinate
	this->m_cur_coord_indicator = new CursorCoordinator(this);

	// Display Windowing Value
	this->m_windowing_indicator = new WindowingIndicator(this);

	connect(this, SIGNAL(changeCoords(int, int, int, int)), this->m_cur_coord_indicator, SLOT(setCoord(int, int, int, int)));
}

void SliceWidget::setLines(QLine lh, QLine lv, QColor ch, QColor cv) {
	m_line_horizontal = lh;
	m_line_vertical = lv;

	// Set line color
	m_color_line_horizontal = ch;
	m_color_line_vertical = cv;

	// Find meet point of two lines
	QLineF(m_line_horizontal).intersect(QLineF(m_line_vertical), &m_line_meet);
}

void SliceWidget::updateWindowingValue(int level, int width) {
	m_windowing_indicator->setWindowingValue(level, width);
}

void SliceWidget::drawSlice(QImage* img){

	QPixmap *pixmap = new QPixmap();

	if (img == nullptr) {
		*pixmap = QPixmap::fromImage(m_slice, Qt::AutoColor);
	}
	else {
		m_is_slice_loaded = true;

		*img = img->scaled(m_size);
		*pixmap = QPixmap::fromImage(*img, Qt::AutoColor);
		this->m_slice = *img;
	}
	QPainter painter(pixmap);

	this->drawAxes(&painter);

	
	this->setAlignment(Qt::AlignCenter);
	this->setPixmap(*pixmap);
}

void SliceWidget::drawAxes(QPainter *painter) {
	QPen pen;
	pen.setWidth(3);			// Thickness of pen

	// Horizontal line
	pen.setColor(m_color_line_horizontal);
	painter->setPen(pen);
	painter->drawLine(m_line_horizontal);

	// Vertical line
	pen.setColor(m_color_line_vertical);
	painter->setPen(pen);
	painter->drawLine(m_line_vertical);
}

/* ===========================================================================
   ===========================================================================
   
								QT Listeners

   ===========================================================================
   =========================================================================== */

/**
 * Listen for cursor moving on screen.
 */
void SliceWidget::mouseMoveEvent(QMouseEvent *e) {
	if (m_is_slice_loaded && m_is_cursor_on) {

		ImageViewWidget* manager = qobject_cast<ImageViewWidget*>(parent());		// Access parent widget

		// Change the Cursor shape
		if (_isPointInsideCircle(e->pos())) {
			this->setCursor(Qt::SizeAllCursor);
		}
		else if (_isPointOnLines(e->pos())) {
			if (m_is_left_pressed) this->setCursor(Qt::ClosedHandCursor);
			else this->setCursor(Qt::OpenHandCursor);
		}
		else {
			this->setCursor(Qt::ArrowCursor);
		}

		/* Move the center of axes */
		if (m_is_left_pressed && m_is_point_inside_circle) {
			QVector2D delta(e->pos() - m_prev_cursor_point);
			emit changeAxesCenter(e->pos().x(), e->pos().y());
		}
		/* Rotate axes with mouse */
		else if (m_is_left_pressed && m_is_point_on_lines) {
			if (m_prev_cursor_point == e->pos()) return;							// To prevent impossible acos

			QVector2D v_prev(m_prev_cursor_point - m_line_meet);
			QVector2D v_cur(e->pos() - m_line_meet);
			v_prev.normalize();
			v_cur.normalize();

			float prev_degree = acos(QVector2D::dotProduct(v_prev, QVector2D(1, 0))) * 180 / M_PI;
			float cur_degree = acos(QVector2D::dotProduct(v_cur, QVector2D(1, 0))) * 180 / M_PI;

			if (m_prev_cursor_point.y() > m_line_meet.y()) prev_degree *= -1;
			if (e->y() > m_line_meet.y()) cur_degree *= -1;

			float degree = cur_degree - prev_degree;
			// TODO:: make this more accurate
			if ((m_prev_cursor_point.y() > m_line_meet.y() && e->y() <= m_line_meet.y()) ||
				m_prev_cursor_point.y() <= m_line_meet.y() && e->y() > m_line_meet.y()) {
				if (degree > 0) {
					degree = degree - 360;
				}
				else {
					degree = degree + 360;
				}
			}

			emit changeDegree(degree);
		}
		/* Change Zoom in and out parameter */
		else if (m_is_left_pressed) {
			QVector2D delta(e->pos() - m_prev_cursor_point);
			emit changeZoom(delta.x() + delta.y());
		}
		/* Change Windowing with mouse */
		else if (m_is_right_pressed) {
			QVector2D delta(e->pos() - m_prev_cursor_point);
			emit changeWindowing(delta.x(), delta.y());
		}

		// Default values
		int x = e->x();
		int y = e->y();
		int z = 0;
		int intensity = 0;

		if (manager != nullptr) {
			std::vector<int> info = manager->getPixelInfo(x, y);
			x = info[0];
			y = info[1];
			z = info[2];
			intensity = info[3];
		}

		emit changeCoords(x, y, z, intensity);
	}

	m_prev_cursor_point = e->pos();
}

void SliceWidget::mousePressEvent(QMouseEvent *e) {

	if (m_is_slice_loaded && m_is_cursor_on) {

		if (e->buttons() & Qt::LeftButton) {
			qDebug() << "Left mouse pressed";
			m_is_left_pressed = true;
			m_is_point_on_lines = _isPointOnLines(e->pos());
			m_is_point_inside_circle = _isPointInsideCircle(e->pos());

		}

		if (e->buttons() & Qt::RightButton) {
			qDebug() << "Right mouse pressed";
			m_is_right_pressed = true;
		}
	}
}

void SliceWidget::mouseReleaseEvent(QMouseEvent *e) {

	if (m_is_slice_loaded && m_is_cursor_on) {

		if (e->button() == Qt::LeftButton) {
			qDebug() << "Left mouse Released";
			m_is_left_pressed = false;
			m_is_point_on_lines = false;
			m_is_point_inside_circle = false;
		}
		if (e->button() == Qt::RightButton) {
			qDebug() << "Right mouse Released";
			m_is_right_pressed = false;
		}
	}
}

void SliceWidget::mouseDoubleClickEvent(QMouseEvent *e) {
	qDebug() << e->pos();
	emit changeAxesCenter(e->x(), e->y());
}

void SliceWidget::wheelEvent(QWheelEvent *e) {
	
	if (m_is_slice_loaded && m_is_cursor_on) {
		qDebug() << e->delta();

		if (e->delta() >= 0) emit requestIncIndex();
		else emit requestDecIndex();
	}
}

void SliceWidget::resizeEvent(QResizeEvent* e) {
	qDebug() << "Resized: " << e->size();
	this->m_size = e->size();

	// Move Windowing indicator
	this->m_windowing_indicator->move(QPoint(5, m_size.height() - 40));	//Fixed Position
}

/**
 * Check the mouse cursor have just moved into label
 */
void SliceWidget::enterEvent(QEvent *e) {
	m_is_cursor_on = true;

	// Move Coordinator dynamically based on label size.
	this->m_cur_coord_indicator->move(QPoint(m_size.width() - 150, m_size.height() - 25));
	this->m_cur_coord_indicator->setVisible(true);
}

/**
 * Check the mouse cursor have just left label
 */
void SliceWidget::leaveEvent(QEvent *e) {
	m_is_cursor_on = false;

	this->m_cur_coord_indicator->setVisible(false);
}

/* ===========================================================================
   ===========================================================================

							Utils for Cursor positon

   ===========================================================================
   =========================================================================== */

bool SliceWidget::_isPointOnLines(QPoint p) {


	// Horizontal line
	QPointF ps = m_line_horizontal.p1();
	QPointF pe = m_line_horizontal.p2();
	QVector2D v_line(pe - ps);
	v_line.normalize();

	QPointF center = ps;
	while (!_isPointNearPoint(center, pe, 1)) {
		if(_isPointNearPoint(p, center, 3)) return true;
		center = center + v_line.toPointF();
	}

	// Vertical line
	ps = m_line_vertical.p1();
	pe = m_line_vertical.p2();
	v_line = QVector2D(pe - ps);
	v_line.normalize();

	center = ps;
	while (!_isPointNearPoint(center, pe, 1)) {
		if (_isPointNearPoint(p, center, 3)) return true;
		center = center + v_line.toPointF();
	}

	return false;
}

bool SliceWidget::_isPointInsideCircle(QPoint p) {
	return (p.x() - m_line_meet.x()) * (p.x() - m_line_meet.x()) 
			+ (p.y() - m_line_meet.y()) * (p.y() - m_line_meet.y()) 
			<= m_circle_radius * m_circle_radius;
}

bool SliceWidget::_isPointNearPoint(QPointF a, QPointF b, int range) {
	return (a.x() - b.x()) *(a.x() - b.x()) 
			+ (a.y() - b.y()) *(a.y() - b.y())
			<= range * range;
}