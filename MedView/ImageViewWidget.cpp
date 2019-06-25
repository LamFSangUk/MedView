#include "ImageViewWidget.h"

#include <algorithm>
#include <climits>

ImageViewWidget::ImageViewWidget() {
	this->setText("hello");
}

void ImageViewWidget::setVolume(vdcm::Volume* vol) {
	this->m_volume = vol;

	this->draw();
}

void ImageViewWidget::draw() {

	qDebug("drawing");
	std::vector<uint16_t> slice = m_volume->getAxialSlice(0);
	qDebug("%d", slice.size());

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

	uint16_t max = 0;
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

	QImage img(512,512,QImage::Format_RGBA64);
	//img = QImage::fromData(byteArray);

	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			QRgb val = qRgb(slice[i * 512 + j], slice[i * 512 + j], slice[i * 512 + j]);
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