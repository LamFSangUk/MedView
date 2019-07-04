#include "DicomManager.h"
#include<iostream>

DicomManager::DicomManager(QObject *parent = NULL) 
: QObject(parent) {
	this->m_volume = nullptr;
	this->m_axes = nullptr;
}

void DicomManager::readDicom(const char* filename) {
	this->m_filename = filename;

	this->m_volume = vdcm::read(filename);

	// Set default axes
	double center_x = (double)(m_volume->getWidth()) / 2;
	double center_y = (double)(m_volume->getHeight()) / 2;
	double center_z = (double)(m_volume->getDepth()) / 2;
	this->m_axes = new vdcm::Axes(center_x, center_y, center_z);

	max_axial_idx = m_volume->getDepth() - 1;
	max_coronal_idx = m_volume->getHeight() - 1;
	max_sagittal_idx = m_volume->getWidth() - 1;

	axial_idx = (int)(center_z + 0.5);
	coronal_idx = (int)(center_y + 0.5);
	sagittal_idx = (int)(center_x + 0.5);

	emit changeVolume();
}

void DicomManager::extractSlice(int mode, int width, int height) {
	
	Slice s = m_volume->getSlice(mode, m_axes);

	QImage slice(600, 600, QImage::Format_RGBA64);
	for (int i = 0; i < 600; i++) {
		for (int j = 0; j < 600; j++) {
			QRgb val = qRgb(s.getVoxelIntensity(j, i), s.getVoxelIntensity(j, i), s.getVoxelIntensity(j, i));
			slice.setPixel(j, i, val);
		}
	}
	QImage r_slice = slice.scaled(width, height);

	emit changeSlice(mode, &r_slice);

}

void DicomManager::setSliceIdx(int mode, int idx, int width, int height) {
	Eigen::Vector4d c = m_axes->getCenter();
	Eigen::Vector3d dir = m_axes->getAxis(mode);
	Eigen::Vector4d rdir;
	rdir << dir(0), dir(1), dir(2), 0;
	switch (mode) {
		case MODE_AXIAL:
			c = c + rdir * (idx - axial_idx);		//diff
			std::cout << "AXIAL" << c << std::endl;
			axial_idx = idx;
			break;
		case MODE_CORONAL:
			c = c + rdir * (idx - coronal_idx);		//diff
			coronal_idx = idx;
			std::cout << "CORONAL" << c << std::endl;

			break;
		case MODE_SAGITTAL:
			c = c + rdir * (idx - sagittal_idx);	//diff
			sagittal_idx = idx;
			std::cout << "SAGITTAL"<< c << std::endl;

			break;
		default:
			break;
	}
	m_axes->setCenter(c(0),c(1),c(2));
	extractSlice(mode, width, height);

}