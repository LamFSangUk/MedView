#include "SliceWidget.h"

#include <QMouseEvent>
#include <QDebug>

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

	m_slice = nullptr;
}

void SliceWidget::drawSlice(QImage* img){
	//if (m_slice != nullptr) {
	//	delete m_slice;
	//}

	this->m_slice = img;
	this->setAlignment(Qt::AlignCenter);
	this->setPixmap(QPixmap::fromImage(*img, Qt::AutoColor));
}

void SliceWidget::mouseMoveEvent(QMouseEvent *e) {
	int x = e->x();
	int y = e->y();
	int z = 0;

	emit changeCoords(x, y, z);
}