#include "DicomManager.h"
#include<iostream>

#include<QDebug>
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

	max_axial_idx = m_volume->getDepth() - 2;
	max_coronal_idx = m_volume->getHeight() - 2;
	max_sagittal_idx = m_volume->getWidth() - 2;

	axial_idx = (int)(center_z + 0.5);
	coronal_idx = (int)(center_y + 0.5);
	sagittal_idx = (int)(center_x + 0.5);

	emit changeVolume();
}

void DicomManager::extractSlice(int mode) {
	
	Slice* s = m_volume->getSlice(mode, m_axes);
	switch (mode) {
		case MODE_AXIAL:
			axial_slice = s;
			break;
		case MODE_CORONAL:
			coronal_slice = s;
			break;
		case MODE_SAGITTAL:
			sagittal_slice = s;
			break;
		default:
			break;
	}

	QImage slice(m_standard_slice_size, QImage::Format_Grayscale8);
	for (int i = 0; i < m_standard_slice_size.height(); i++) {
		for (int j = 0; j < m_standard_slice_size.width(); j++) {
			double org_val = (double)s->getVoxelIntensity(j, i);
			if (org_val < 0) org_val = 0;
			QRgb val = qRgb((int)(org_val+0.5), (int)(org_val + 0.5), (int)(org_val+0.5));
			//slice.setPixelColor(j, i, (uint)(org_val + 0.5));
			slice.setPixel(j, i, val);
		}
	}

	emit changeSlice(mode, &slice);
}

/**
 * Get voxel information including real coordinate and intensity using coordinate of slice.
 *
 * @param int mode : Mode for viewing slice(Axial, Coronal, Sagittal)
 * @param int slice_x : x coordinate of slice
 * @param int slice_y : y coordinate of slice
 * @return vector<int> : coordinate(x,y,z) and intensity of the voxel
 */
std::vector<int> DicomManager::getVoxelInfo(int mode, int slice_x, int slice_y) {
	Eigen::Vector4d position;
	int intensity;

	if (slice_x < 0) slice_x = 0;
	else if (slice_x >= m_standard_slice_size.width()) slice_x = m_standard_slice_size.width();
	if (slice_y < 0) slice_y = 0;
	else if (slice_y >= m_standard_slice_size.height()) slice_y = m_standard_slice_size.height();

	switch (mode) {
		case MODE_AXIAL:
			position = axial_slice->getVoxelCoord(slice_x, slice_y);
			intensity = axial_slice->getVoxelIntensity(slice_x, slice_y);
			break;
		case MODE_CORONAL:
			position = coronal_slice->getVoxelCoord(slice_x, slice_y);
			intensity = coronal_slice->getVoxelIntensity(slice_x, slice_y);
			break;
		case MODE_SAGITTAL:
			position = sagittal_slice->getVoxelCoord(slice_x, slice_y);
			intensity = sagittal_slice->getVoxelIntensity(slice_x, slice_y);
			break;
		default:
			break;
	}

	std::vector<int> voxel_info(4);
	voxel_info[0] = position(0);
	voxel_info[1] = position(1);
	voxel_info[2] = position(2);
	voxel_info[3] = intensity;

	return voxel_info;
}

void DicomManager::setSliceIdx(int mode, int idx) {
	Eigen::Vector4d c = m_axes->getCenter();
	Eigen::Vector3f dir = m_axes->getAxis(mode);
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
	emit changeAxes();

	extractSlice(mode);
}
void DicomManager::setDegree(int mode, float degree) {
	switch (mode) {
		case MODE_AXIAL:
			m_axes->addYaw(degree);
			calculateIdxes();
			
			extractSlice(MODE_CORONAL);
			extractSlice(MODE_SAGITTAL);
			break;
		case MODE_CORONAL:
			m_axes->addPitch(degree);
			calculateIdxes();

			extractSlice(MODE_AXIAL);
			extractSlice(MODE_SAGITTAL);
			break;
		case MODE_SAGITTAL:
			m_axes->addRoll(degree);
			calculateIdxes();

			extractSlice(MODE_AXIAL);
			extractSlice(MODE_CORONAL);
			break;
		default:
			break;
	}
	emit changeAxes();
}

std::vector<QLine> DicomManager::getAxesLines(int mode, int width, int height) {
	Eigen::Vector4d c = m_axes->getCenter();
	Eigen::Vector4d end_point;
	Eigen::Vector3f dir_axial = m_axes->getAxis(MODE_AXIAL);
	Eigen::Vector3f dir_coronal = m_axes->getAxis(MODE_CORONAL);
	Eigen::Vector3f dir_sagittal = m_axes->getAxis(MODE_SAGITTAL);

	Eigen::Vector4d rdir;

	QLine lh;
	QLine lv;

	double width_rate, height_rate;
	switch (mode) {
		case MODE_AXIAL:
			width_rate = (double)width / m_volume->getWidth();
			height_rate = (double)height / m_volume->getHeight();

			rdir << dir_coronal(0), dir_coronal(1), dir_coronal(2), 1;
			end_point = c - (coronal_idx * rdir);
			lh.setP1(QPoint((int)(end_point(0)*width_rate + 0.5), (int)(end_point(1)*height_rate + 0.5)));
			end_point = c + ((max_coronal_idx-coronal_idx) * rdir);
			lh.setP2(QPoint((int)(end_point(0)*width_rate + 0.5), (int)(end_point(1)*height_rate + 0.5)));

			rdir << dir_sagittal(0), dir_sagittal(1), dir_sagittal(2), 1;
			end_point = c - (sagittal_idx * rdir);
			lv.setP1(QPoint((int)(end_point(0)*width_rate + 0.5), (int)(end_point(1)*height_rate + 0.5)));
			end_point = c + ((max_sagittal_idx-sagittal_idx) * rdir);
			lv.setP2(QPoint((int)(end_point(0)*width_rate + 0.5), (int)(end_point(1)*height_rate + 0.5)));

			break;
		case MODE_CORONAL:
			width_rate = (double)width / m_volume->getHeight();
			height_rate = (double)height / m_volume->getDepth();

			rdir << dir_sagittal(0), dir_sagittal(1), dir_sagittal(2), 1;
			end_point = c - (sagittal_idx * rdir);
			std::cout << end_point << std::endl;
			lh.setP1(QPoint((int)(end_point(1)*width_rate + 0.5), (int)(end_point(2)*height_rate + 0.5)));
			end_point = c + ((max_sagittal_idx - sagittal_idx) * rdir);
			std::cout << end_point << std::endl;
			lh.setP2(QPoint((int)(end_point(1)*width_rate + 0.5), (int)(end_point(2)*height_rate + 0.5)));
			qDebug() << lh;


			rdir << dir_axial(0), dir_axial(1), dir_axial(2), 1;
			end_point = c - (axial_idx * rdir);
			std::cout << end_point << std::endl;
			lv.setP1(QPoint((int)(end_point(1)*width_rate + 0.5), (int)(end_point(2)*height_rate + 0.5)));
			end_point = c + ((max_axial_idx - axial_idx) * rdir);
			std::cout << end_point << std::endl;
			lv.setP2(QPoint((int)(end_point(1)*width_rate + 0.5), (int)(end_point(2)*height_rate + 0.5)));
			qDebug() << lv;


			break;
		case MODE_SAGITTAL:
			width_rate = (double)width / m_volume->getWidth();
			height_rate = (double)height / m_volume->getDepth();

			rdir << dir_coronal(0), dir_coronal(1), dir_coronal(2), 1;
			end_point = c - (coronal_idx * rdir);
			lh.setP1(QPoint((int)(end_point(0)*width_rate + 0.5), (int)(end_point(2)*height_rate + 0.5)));
			end_point = c + ((max_coronal_idx - coronal_idx) * rdir);
			lh.setP2(QPoint((int)(end_point(0)*width_rate + 0.5), (int)(end_point(2)*height_rate + 0.5)));

			rdir << dir_axial(0), dir_axial(1), dir_axial(2), 1;
			end_point = c - (axial_idx * rdir);
			lv.setP1(QPoint((int)(end_point(0)*width_rate + 0.5), (int)(end_point(2)*height_rate + 0.5)));
			end_point = c + ((max_axial_idx - axial_idx) * rdir);
			lv.setP2(QPoint((int)(end_point(0)*width_rate + 0.5), (int)(end_point(2)*height_rate + 0.5)));

			break;
		default:
			break;
	}

	std::vector<QLine> res;
	res.push_back(lh);
	res.push_back(lv);

	return res;
}

/**
 * Calculate Max, Min and current index of slices about axis.
 */
void DicomManager::calculateIdxes() {
	Eigen::Vector3f dir_axial = m_axes->getAxis(MODE_AXIAL);
	Eigen::Vector3f dir_coronal = m_axes->getAxis(MODE_CORONAL);
	Eigen::Vector3f dir_sagittal = m_axes->getAxis(MODE_SAGITTAL);

	Eigen::Vector4d center_d = m_axes->getCenter();
	Eigen::Vector3f center;
	center << center_d(0), center_d(1), center_d(2);
	Eigen::Vector3f point;

	// Axial plane
	point = center;
	int count = 0;
	while (!isPointOutOfVolume(point)) {
		point -= dir_axial;
		count++;
	}
	count--;

	axial_idx = count;

	point = center;
	while (!isPointOutOfVolume(point)) {
		point += dir_axial;
		count++;
	}
	count--;

	max_axial_idx = count;

	// Coronal plane
	point = center;
	count = 0;
	while (!isPointOutOfVolume(point)) {
		point -= dir_coronal;
		count++;
	}
	count--;

	coronal_idx = count;

	point = center;
	while (!isPointOutOfVolume(point)) {
		point += dir_coronal;
		count++;
	}
	count--;

	max_coronal_idx = count;

	// Sagittal plane
	point = center;
	count = 0;
	while (!isPointOutOfVolume(point)) {
		point -= dir_sagittal;
		count++;
	}
	count--;

	sagittal_idx = count;

	point = center;
	while (!isPointOutOfVolume(point)) {
		point += dir_sagittal;
		count++;
	}
	count--;

	max_sagittal_idx = count;
}

bool DicomManager::isPointOutOfVolume(Eigen::Vector3f p) {
	if ((p(0) < 0) || (p(0) >= m_volume->getWidth())
		|| (p(1) < 0) || (p(1) >= m_volume->getHeight())
		|| (p(2) < 0) || (p(2) >= m_volume->getDepth())) {
		return true;
	}
	return false;
}

void DicomManager::reset() {
	
	double center_x = (double)(m_volume->getWidth()) / 2;
	double center_y = (double)(m_volume->getHeight()) / 2;
	double center_z = (double)(m_volume->getDepth()) / 2;
	m_axes->reset(center_x, center_y, center_z);

	calculateIdxes();

	extractSlice(MODE_AXIAL);
	extractSlice(MODE_CORONAL);
	extractSlice(MODE_SAGITTAL);

	emit changeAxes();
}