#include "SliceWidget.h"

#include <QMouseEvent>
#include <QDebug>
#include <QPainter>

#include "ImageViewWidget.h"

SliceWidget::SliceWidget(QWidget* parent=NULL)
	: QLabel(parent)
{
	m_is_cursor_on = false;
	m_is_slice_loaded = false;

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
	//if (m_slice != nullptr) {
	//	delete m_slice;
	//}

	QPixmap *pixmap = new QPixmap();

	if (img == nullptr) {
		*pixmap = QPixmap::fromImage(m_slice, Qt::AutoColor);
		m_is_slice_loaded = true;
	}
	else {
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
}

void SliceWidget::wheelEvent(QWheelEvent *e) {
	qDebug() << e->delta();

	if (m_is_slice_loaded && m_is_cursor_on) {
		if (e->delta() >= 0) emit requestIncIndex();
		else emit requestDecIndex();
	}
}

void SliceWidget::resizeEvent(QResizeEvent* e) {
	qDebug() << "Resized: " << e->size();
	this->m_size = e->size();
	qDebug() << m_size.height() << m_size.rheight();
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