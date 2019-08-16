#include "WindowingIndicator.h"

WindowingIndicator::WindowingIndicator(QWidget* parent=NULL)
	: QTextEdit(parent) 
{
	// Block to modify text value
	this->setEnabled(false);

	// Set background transparent
	this->viewport()->setAutoFillBackground(false);

	// Set no frame
	this->setFrameStyle(QFrame::NoFrame);
}

void WindowingIndicator::setWindowingValue(int level, int width) {
	m_windowing_level = level;
	m_windowing_width = width;

	this->setText(makeString().c_str());
	this->show();
}

std::string WindowingIndicator::makeString() {
	std::string str = "WL: " + std::to_string(this->m_windowing_level)
		+ "\nWW: " + std::to_string(this->m_windowing_width);

	return str;
}