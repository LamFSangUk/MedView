#include "DicomManager.h"
#include<iostream>

#include<QDebug>
DicomManager::DicomManager(QObject *parent = NULL) 
: QObject(parent) {
	this->m_volume = nullptr;
	this->m_axes = nullptr;

	this->m_axial_slice = nullptr;
	this->m_coronal_slice = nullptr;
	this->m_sagittal_slice = nullptr;

	// Listening value has changed, then update slices.
	connect(this, &DicomManager::changeValue, [=]() {
		this->_updateSlice();
	});

	connect(this, &DicomManager::changeVolume, this, &DicomManager::changeValue);
}

void DicomManager::readDicom(const char* filename) {
	this->m_filename = filename;

	this->m_volume = vdcm::read(filename);

	// Set default axes
	this->_initValues();

	emit changeVolume();
}

/**
 * Return the raw volume buffer
 */
float* DicomManager::getVolumeBuffer() {
	return m_volume->getBuffer();
}

/**
 * Get voxel information including real coordinate and intensity using coordinate of slice.
 *
 * @param int mode : Mode for viewing slice(Axial, Coronal, Sagittal)
 * @param int slice_x : x coordinate of slice
 * @param int slice_y : y coordinate of slice
 * @return vector<int> : coordinate(x,y,z) and intensity of the voxel
 */
std::vector<int> DicomManager::getVoxelInfo(Mode mode, int slice_x, int slice_y) {
	Eigen::Vector4f position;
	int intensity;

	if (slice_x < 0) slice_x = 0;
	else if (slice_x >= m_standard_slice_size.width()) slice_x = m_standard_slice_size.width();
	if (slice_y < 0) slice_y = 0;
	else if (slice_y >= m_standard_slice_size.height()) slice_y = m_standard_slice_size.height();

	switch (mode) {
		case Mode::MODE_AXIAL:
			position = m_axial_slice->getVoxelCoord(slice_x, slice_y);
			intensity = m_axial_slice->getVoxelIntensity(slice_x, slice_y);
			break;
		case Mode::MODE_CORONAL:
			position = m_coronal_slice->getVoxelCoord(slice_x, slice_y);
			intensity = m_coronal_slice->getVoxelIntensity(slice_x, slice_y);
			break;
		case Mode::MODE_SAGITTAL:
			position = m_sagittal_slice->getVoxelCoord(slice_x, slice_y);
			intensity = m_sagittal_slice->getVoxelIntensity(slice_x, slice_y);
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

/**
 * Update the slice index
 */
void DicomManager::updateSliceIndex(Mode m, int value) {
	Eigen::Vector4d c = m_axes->getCenter();
	Eigen::Vector3f dir = m_axes->getAxis(m);
	Eigen::Vector4d rdir;
	rdir << dir(0), dir(1), dir(2), 0;

	//int mode_flag = m;
	int prev_idx;

	switch (m) {
		case Mode::MODE_AXIAL:
			prev_idx = m_axial_idx;
			m_axial_idx = value;
			if (m_axial_idx < 0) m_axial_idx = 0;
			if (m_axial_idx > m_max_axial_idx) m_axial_idx = m_max_axial_idx;

			c = c + rdir * (m_axial_idx - prev_idx);
			m_axial_center = m_axial_center + dir * (m_axial_idx - prev_idx);
			break;
		case Mode::MODE_CORONAL:
			prev_idx = m_coronal_idx;
			m_coronal_idx = value;
			if (m_coronal_idx < 0) m_coronal_idx = 0;
			if (m_coronal_idx > m_max_coronal_idx) m_coronal_idx = m_max_coronal_idx;

			c = c + rdir * (m_coronal_idx - prev_idx);
			m_coronal_center = m_coronal_center + dir * (m_coronal_idx - prev_idx);
			break;
		case Mode::MODE_SAGITTAL:
			prev_idx = m_sagittal_idx;
			m_sagittal_idx = value;
			if (m_sagittal_idx < 0) m_sagittal_idx = 0;
			if (m_sagittal_idx > m_max_sagittal_idx) m_sagittal_idx = m_max_sagittal_idx;

			c = c + rdir * (m_sagittal_idx - prev_idx);
			m_sagittal_center = m_sagittal_center + dir * (m_sagittal_idx - prev_idx);
			break;
		default:
			break;
	}
	m_axes->setCenter(c(0), c(1), c(2));

	emit changeValue();
}

/**
 * Update the angle about axes
 */
void DicomManager::updateAngle(Mode m, float degree) {

	//int mode_flag = m;

	switch (m) {
		case Mode::MODE_AXIAL:
			m_axes->addYaw(degree);
			break;
		case Mode::MODE_CORONAL:
			m_axes->addRoll(degree);
			break;
		case Mode::MODE_SAGITTAL:
			m_axes->addPitch(degree);
			break;
		default:
			break;
	}
	calculateIdxes();

	emit changeValue();
}

/**
 * Update the window width and level
 */
void DicomManager::updateWindowing(int delta_level, int delta_width) {
	m_window_level += delta_level;
	m_window_width += delta_width;

	emit changeValue();
}

/**
 * Update Zoom in and out parameter
 */
void DicomManager::updateZoom(float delta) {
	m_zoom -= delta * m_zoom_speed *m_zoom;
	m_zoom = std::max(m_zoom, m_zoom_min);
	m_zoom = std::min(m_zoom, m_zoom_max);

	emit changeValue();
}

void DicomManager::updateAxesCenter(Mode m, int x, int y, int slice_width, int slice_height) {

	x = (int)((float)x * m_standard_slice_size.width() / slice_width);
	y = (int)((float)y * m_standard_slice_size.height() / slice_height);

	Eigen::Vector4f new_center;

	switch (m) {
		case Mode::MODE_AXIAL:
			new_center = m_axial_slice->getVoxelCoord(x, y);
			break;
		case Mode::MODE_CORONAL:
			new_center = m_coronal_slice->getVoxelCoord(x, y);
			break;
		case Mode::MODE_SAGITTAL:
			new_center = m_sagittal_slice->getVoxelCoord(x, y);
			break;
		default:
			return;
	}

	m_axes->setCenter(new_center(0), new_center(1), new_center(2));

	// Update slice center too
	m_axial_center << new_center(0), new_center(1), new_center(2);
	m_coronal_center << new_center(0), new_center(1), new_center(2);
	m_sagittal_center << new_center(0), new_center(1), new_center(2);

	calculateIdxes();

	emit changeValue();
}

void DicomManager::updateSliceCenter(Mode m, int x, int y) {
	Eigen::Vector4f new_slice_center;

	switch (m) {
		case Mode::MODE_AXIAL:
			new_slice_center = m_axial_slice->getVoxelCoord(x, y);
			m_axial_center << new_slice_center(0), new_slice_center(1), new_slice_center(2);
			break;
		case Mode::MODE_CORONAL:
			new_slice_center = m_coronal_slice->getVoxelCoord(x, y);
			m_coronal_center << new_slice_center(0), new_slice_center(1), new_slice_center(2);
			break;
		case Mode::MODE_SAGITTAL:
			new_slice_center = m_sagittal_slice->getVoxelCoord(x, y);
			m_sagittal_center << new_slice_center(0), new_slice_center(1), new_slice_center(2);
			break;
		default:
			return;
	}

	emit changeValue();
}

/**
 * Calculate Max, Min and current index of slices about axis.
 */
void DicomManager::calculateIdxes() {
	Eigen::Vector3f dir_axial = m_axes->getAxis(Mode::MODE_AXIAL);
	Eigen::Vector3f dir_coronal = m_axes->getAxis(Mode::MODE_CORONAL);
	Eigen::Vector3f dir_sagittal = m_axes->getAxis(Mode::MODE_SAGITTAL);

	Eigen::Vector4d center_d = m_axes->getCenter();
	Eigen::Vector3f center;
	center << center_d(0), center_d(1), center_d(2);
	Eigen::Vector3f point;

	// Axial plane
	point = center;
	int count = 0;
	while (!_isPointOutOfVolume(point)) {
		point -= dir_axial;
		count++;
	}
	count--;

	m_axial_idx = count;

	point = center;
	while (!_isPointOutOfVolume(point)) {
		point += dir_axial;
		count++;
	}
	count--;

	m_max_axial_idx = count;

	// Coronal plane
	point = center;
	count = 0;
	while (!_isPointOutOfVolume(point)) {
		point -= dir_coronal;
		count++;
	}
	count--;

	m_coronal_idx = count;

	point = center;
	while (!_isPointOutOfVolume(point)) {
		point += dir_coronal;
		count++;
	}
	count--;

	m_max_coronal_idx = count;

	// Sagittal plane
	point = center;
	count = 0;
	while (!_isPointOutOfVolume(point)) {
		point -= dir_sagittal;
		count++;
	}
	count--;

	m_sagittal_idx = count;

	point = center;
	while (!_isPointOutOfVolume(point)) {
		point += dir_sagittal;
		count++;
	}
	count--;

	m_max_sagittal_idx = count;
}



void DicomManager::reset() {
	_initValues();

	emit changeValue();
}

/**
 * Initialize axes and slice indexes.
 */
void DicomManager::_initValues() {
	double center_x = (double)(m_volume->getWidth()) / 2;
	double center_y = (double)(m_volume->getHeight()) / 2;
	double center_z = (double)(m_volume->getDepth()) / 2;
	this->m_axes = new vdcm::Axes(center_x, center_y, center_z);

	m_max_axial_idx = m_volume->getDepth() - 2;
	m_max_coronal_idx = m_volume->getHeight() - 2;
	m_max_sagittal_idx = m_volume->getWidth() - 2;

	m_axial_idx = (int)(center_z + 0.5);
	m_coronal_idx = (int)(center_y + 0.5);
	m_sagittal_idx = (int)(center_x + 0.5);

	m_axial_center << center_x, center_y, center_z;
	m_coronal_center << center_x, center_y, center_z;
	m_sagittal_center << center_x, center_y, center_z;

	// Default windowing
	m_window_width = 350;
	m_window_level = 50;
}

/**
 * Convert Slice object to QImage object
 * 
 * @param Slice* s : target slice
 * @return QImage : converted slice image
 */
QImage DicomManager::_convertSliceToImage(Slice* s) {
	QImage slice(m_standard_slice_size, QImage::Format_Grayscale8);

	// Windowding 
	int window_min = m_window_level - m_window_width / 2;
	int window_max = m_window_level + m_window_width / 2;

	for (int i = 0; i < m_standard_slice_size.height(); i++) {
		for (int j = 0; j < m_standard_slice_size.width(); j++) {
			Eigen::Vector4f org_coord = s->getVoxelCoord(j, i);
			Eigen::Vector3f org_coord_3;
			org_coord_3 << org_coord(0), org_coord(1), org_coord(2);

			double org_val = (double)s->getVoxelIntensity(j, i);

			if (_isPointOutOfVolume(org_coord_3)) {
				org_val = 0;
			}
			else {
				// thresholding
				if (org_val < window_min) org_val = window_min;
				if (org_val > window_max) org_val = window_max;

				// Normailize
				org_val = (org_val - window_min) / (window_max - window_min) * 255;
			}

			QRgb val = qRgb((int)(org_val + 0.5), (int)(org_val + 0.5), (int)(org_val + 0.5));
			slice.setPixel(j, i, val);
		}
	}

	return slice;
}

/**
 * Extract the slice from volume data and update it.
 */
void DicomManager::_updateSlice() {

	// Axial plane
	if (m_axial_slice) {
		delete m_axial_slice;
		m_axial_slice = nullptr;
	}
	m_axial_slice = m_volume->getSlice(Mode::MODE_AXIAL, m_axes,
		m_standard_slice_size.width(), m_standard_slice_size.height(), m_axial_center, m_zoom);

	// Coronal plane
	if (m_coronal_slice) {
		delete m_coronal_slice;
		m_coronal_slice = nullptr;
	}
	m_coronal_slice = m_volume->getSlice(Mode::MODE_CORONAL, m_axes,
		m_standard_slice_size.width(), m_standard_slice_size.height(), m_coronal_center, m_zoom);

	// Sagittal plane
	if (m_sagittal_slice) {
		delete m_sagittal_slice;
		m_sagittal_slice = nullptr;
	}
	m_sagittal_slice = m_volume->getSlice(Mode::MODE_SAGITTAL, m_axes,
		m_standard_slice_size.width(), m_standard_slice_size.height(), m_sagittal_center, m_zoom);
	
	// create parameter packets
	std::map<Mode, SlicePacket> packets;
	SlicePacket packet;

	Eigen::Vector4d center = m_axes->getCenter();
	std::tuple<int, int> plane_center;
	packet.size = m_standard_slice_size;

	packet.cur_idx = m_axial_idx;
	packet.max_idx = m_max_axial_idx;
	packet.slice = _convertSliceToImage(m_axial_slice);
	plane_center = m_axial_slice->getPositionOfVoxel(center(0), center(1), center(2));
	packet.line_center = QPoint(std::get<0>(plane_center),std::get<1>(plane_center));
	packet.angle = m_axes->getAngle(Mode::MODE_AXIAL);
	packets.insert(std::pair<Mode, SlicePacket>(Mode::MODE_AXIAL, packet));

	packet.cur_idx = m_coronal_idx;
	packet.max_idx = m_max_coronal_idx;
	packet.slice = _convertSliceToImage(m_coronal_slice);
	plane_center = m_coronal_slice->getPositionOfVoxel(center(0), center(1), center(2));
	packet.line_center = QPoint(std::get<0>(plane_center), std::get<1>(plane_center));
	packet.angle = m_axes->getAngle(Mode::MODE_CORONAL);
	packets.insert(std::pair<Mode, SlicePacket>(Mode::MODE_CORONAL, packet));

	packet.cur_idx = m_sagittal_idx;
	packet.max_idx = m_max_sagittal_idx;
	packet.slice = _convertSliceToImage(m_sagittal_slice);
	plane_center = m_sagittal_slice->getPositionOfVoxel(center(0), center(1), center(2));
	packet.line_center = QPoint(std::get<0>(plane_center), std::get<1>(plane_center));
	packet.angle = m_axes->getAngle(Mode::MODE_SAGITTAL);
	packets.insert(std::pair<Mode, SlicePacket>(Mode::MODE_SAGITTAL, packet));

	qDebug() << "Finish Rendering Slices";
	emit changeSlice(packets);
}

/**
 * Check a point is out of range of volume.
 * 
 * @param Eigen::Vector3f p : a point coordinate
 * @return bool : return true if the point is outside of volume.
 */
bool DicomManager::_isPointOutOfVolume(Eigen::Vector3f p) {
	if ((p(0) < 0) || (p(0) >= m_volume->getWidth())
		|| (p(1) < 0) || (p(1) >= m_volume->getHeight())
		|| (p(2) < 0) || (p(2) >= m_volume->getDepth())) {
		return true;
	}
	return false;
}