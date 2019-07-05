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

void SliceWidget::setLines(QLine lh, QLine lv) {
	m_line_horizontal = lh;
	m_line_vertical = lv;
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

	drawAxes(&painter);
	//drawAxes(&painter);
	//QPen pen(Qt::red);

	//pen.setWidth(4);
	//painter.setPen(pen);


	//painter.drawLine(m_line_horizontal);
	//qDebug() << this->m_line_horizontal;
	//painter.drawLine(m_line_vertical);
	//qDebug() << this->m_line_vertical;

	
	this->setAlignment(Qt::AlignCenter);
	this->setPixmap(*pixmap);

	//drawAxes(QPoint(0, 256), QPoint(512, 256), QPoint(125,125),QPoint(125,125));
}

void SliceWidget::drawAxes(QPainter *painter) {
	QPen pen(Qt::red);

	pen.setWidth(3);
	painter->setPen(pen);

	painter->drawLine(m_line_horizontal);
	qDebug() << this->m_line_horizontal;
	painter->drawLine(m_line_vertical);
	qDebug() << this->m_line_vertical;
}

void SliceWidget::mouseMoveEvent(QMouseEvent *e) {
	int x = e->x();
	int y = e->y();
	int z = 0;

	emit changeCoords(x, y, z);
}