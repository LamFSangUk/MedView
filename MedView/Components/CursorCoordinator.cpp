#include "CursorCoordinator.h"

CursorCoordinator::CursorCoordinator(QWidget* parent)
	:QTextEdit(parent)
{
	// Block to modify text value
	this->setEnabled(false);

	// Set background transparent
	this->viewport()->setAutoFillBackground(false);

	// Set no frame
	this->setFrameStyle(QFrame::NoFrame);

	this->setFixedSize(QSize(200, 30));
}

void CursorCoordinator::setCoord(int x, int y, int z, int intensity) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->intensity = intensity;

	this->setText(makeCoordString().c_str());
	this->show();
}

std::string CursorCoordinator::makeCoordString() {
	std::string str = "(x:" + std::to_string(this->x)
		+ " y:" + std::to_string(this->y)
		+ " z:" + std::to_string(this->z) + "): "
		+ std::to_string(this->intensity);

	return str;
}