#include "SliceWidget.h"

#include <QMouseEvent>
#include <QDebug>
#include <QPainter>

SliceWidget::SliceWidget(QWidget* parent=NULL)
	: QLabel(parent)
{
	// Set default background color
	this->setStyleSheet("QLabel { background-color : black; }");
	qDebug() << this->height();
	this->resize(parent->frameSize());

	// For tracking mouse
	this->setMouseTracking(true);

	// Display mouse coordinate
	this->m_cur_coord = new CursorCoordinator(this);

	//TODO:: change coordinate dynamically
	this->m_cur_coord->move(QPoint(40, 40));
	this->m_cur_coord->show();
	connect(this, SIGNAL(changeCoords(int, int, int)), this->m_cur_coord, SLOT(setCoord(int, int, int)));
}

void SliceWidget::setLines(QLine lh, QLine lv, QColor ch, QColor cv) {
	m_line_horizontal = lh;
	m_line_vertical = lv;

	// Set line color
	m_color_line_horizontal = ch;
	m_color_line_vertical = cv;
}

/*void SliceWidget::drawSlice() {
	drawSlice(m_slice);
}*/

void SliceWidget::drawSlice(QImage* img){
	//if (m_slice != nullptr) {
	//	delete m_slice;
	//}

	QPixmap *pixmap = new QPixmap();

	if (img == nullptr) {
		qDebug("here");
		*pixmap = QPixmap::fromImage(m_slice, Qt::AutoColor);
		//*img = m_slice;
	}
	else {
		*pixmap = QPixmap::fromImage(*img, Qt::AutoColor);
		this->m_slice = *img;
	}
	qDebug("there");
	QPainter painter(pixmap);

	this->drawAxes(&painter);

	
	this->setAlignment(Qt::AlignCenter);
	this->setPixmap(*pixmap);
}

void SliceWidget::drawAxes(QPainter *painter) {
	QPen pen;
	pen.setWidth(3);

	pen.setColor(m_color_line_horizontal);
	painter->setPen(pen);
	painter->drawLine(m_line_horizontal);
	qDebug() << this->m_line_horizontal;

	pen.setColor(m_color_line_vertical);
	painter->setPen(pen);
	painter->drawLine(m_line_vertical);
	qDebug() << this->m_line_vertical;
}

void SliceWidget::mouseMoveEvent(QMouseEvent *e) {
	int x = e->x();
	int y = e->y();
	int z = 0;

	emit changeCoords(x, y, z);
}