#include "ImageViewWidget.h"

#include <algorithm>
#include <climits>

ImageViewWidget::ImageViewWidget(int mode) {
	this->setText("hello");

	this->m_mode = mode;
}

void ImageViewWidget::setVolume(vdcm::Volume* vol) {
	this->m_volume = vol;

	this->draw();
}

void ImageViewWidget::draw() {

	qDebug("drawing");
	std::vector<uint16_t> slice;
	int width = 0;
	int height = 0;

	switch (this->m_mode) {
		case MODE_AXIAL:
			slice = m_volume->getAxialSlice(50);
			width = m_volume->getWidth();
			height = m_volume->getHeight();
			break;
		case MODE_CORONAL:
			slice = m_volume->getCoronalSlice(250);
			width = m_volume->getWidth();
			height = m_volume->getDepth();
			break;
		case MODE_SAGITTAL:
			slice = m_volume->getSagittalSlice(300);
			width = m_volume->getHeight();
			height = m_volume->getDepth();
			break;
		default:
			throw std::runtime_error("Not assigned Mode");
			return;
	}
	qDebug("w,h:%d %d\n",width,height);

	// Clamp vector
	/*for (int i = 0; i < slice.size(); i++) {
		if (slice[i] > 4096) slice[i] = 4096;
		//else if (min > slice[i]) min = slice[i];
	}*/
	// Normalize
	/*int16_t max = 4096;
	int16_t min = 0;
	for (int i = 0; i < slice.size(); i++) {
		int16_t val = (int16_t)slice[i];
		if (max < val) slice[i] = max;
		if (min > val) slice[i] = min;
	}*/

	/*uint16_t max = 0;
	uint16_t min = UINT16_MAX;
	for (int i = 0; i < slice.size(); i++) {
		uint16_t val = (uint16_t)slice[i];
		if (max < val) max = val;
		if (min > val) min = val;
	}

	printf("max min:%d %d\n", max, min);

	for (int i = 0; i < slice.size(); i++) {
		uint16_t val = (uint16_t)slice[i];
		slice[i] = (uint16_t)round((val-min) / (double)(max - min) * UINT16_MAX);
	}

	/*const QByteArray byteArray = QByteArray::fromRawData(
		reinterpret_cast<const char*>(slice.data()),
		sizeof(uint16_t)*slice.size());

	printf("ba:%d", byteArray.size());*/

	QImage img(width,height,QImage::Format_RGBA64);
	//img = QImage::fromData(byteArray);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			QRgb val = qRgb(slice[i * width + j], slice[i * width + j], slice[i * width + j]);
			img.setPixel(j, i, val);
		}
	}

	this->setAlignment(Qt::AlignCenter);
	this->setPixmap(QPixmap::fromImage(img, Qt::AutoColor));



	/*QPixmap pixmap;
	pixmap.loadFromData(byteArray,flags=Qt::);

	printf("pix:%d\n", pixmap.size());

	pixmap = pixmap.scaled(this->size(), Qt::KeepAspectRatio);
	this->setPixmap(pixmap);*/
	//this->show();
}