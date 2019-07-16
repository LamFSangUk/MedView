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

	// Set default background color
	this->setStyleSheet("QLabel { background-color : black; }");

	// For tracking mouse
	this->setMouseTracking(true);

	// Display mouse coordinate
	this->m_cur_coord = new CursorCoordinator(this);

	//this->m_cur_coord->move(QPoint(40, 40));
	//this->m_cur_coord->show();
	connect(this, SIGNAL(changeCoords(int, int, int, int)), this->m_cur_coord, SLOT(setCoord(int, int, int, int)));
}

void SliceWidget::setLines(QLine lh, QLine lv, QColor ch, QColor cv) {
	m_line_horizontal = lh;
	m_line_vertical = lv;

	// Set line color
	m_color_line_horizontal = ch;
	m_color_line_vertical = cv;
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
	qDebug() << this->m_line_horizontal;

	// Vertical line
	pen.setColor(m_color_line_vertical);
	painter->setPen(pen);
	painter->drawLine(m_line_vertical);
	qDebug() << this->m_line_vertical;
}



void SliceWidget::mouseMoveEvent(QMouseEvent *e) {

	if (m_is_slice_loaded && m_is_cursor_on) {

		ImageViewWidget* manager = qobject_cast<ImageViewWidget*>(parent());		// Access parent widget

		if (m_is_left_pressed && m_is_point_on_lines) {
			if (m_prev_cursor_point == e->pos()) return;			// To prevent impossible acos

			// TODO:: Handle this logic that get degree of movement.
			QPointF center;
			QLineF(m_line_horizontal).intersect(QLineF(m_line_vertical), &center);	// Find intersection of lines
			
			qDebug() << "prev" << m_prev_cursor_point;

			QVector2D v_prev(m_prev_cursor_point - center);
			QVector2D v_cur(e->pos()- center);
			QVector2D v_dir(e->pos() - m_prev_cursor_point);
			v_prev.normalize();
			v_cur.normalize();
			v_dir.normalize();

			qDebug() << v_prev;
			qDebug() << QVector2D::dotProduct(v_dir, QVector2D(1, 0));
			float prev_degree = acos(QVector2D::dotProduct(v_prev, QVector2D(1, 0))) * 180 / M_PI;
			float cur_degree = acos(QVector2D::dotProduct(v_cur, QVector2D(1, 0))) * 180 / M_PI;
			float degree = cur_degree - prev_degree;

			qDebug() << prev_degree;
			qDebug() << cur_degree;
			qDebug() << degree;
			emit changeDegree(degree);
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
		}
	}
}

void SliceWidget::mouseReleaseEvent(QMouseEvent *e) {

	if (m_is_slice_loaded && m_is_cursor_on) {

		if (e->button() == Qt::LeftButton) {
			qDebug() << "Left mouse Released";
			m_is_left_pressed = false;
			m_is_point_on_lines = false;
		}
	}
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
}

/**
 * Check the mouse cursor have just moved into label
 */
void SliceWidget::enterEvent(QEvent *e) {
	m_is_cursor_on = true;

	// Move Coordinator dynamically based on label size.
	this->m_cur_coord->move(QPoint(m_size.width() - 150, m_size.height() - 20));
	this->m_cur_coord->setVisible(true);
}

/**
 * Check the mouse cursor have just left label
 */
void SliceWidget::leaveEvent(QEvent *e) {
	m_is_cursor_on = false;

	this->m_cur_coord->setVisible(false);
}

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

bool SliceWidget::_isPointNearPoint(QPointF a, QPointF b, int range) {
	return std::sqrt((a.x() - b.x()) *(a.x() - b.x()) + (a.y() - b.y()) *(a.y() - b.y())) <= range;
}