#include "slice.h"
#include <iostream>

#include <cmath>
#define M_PI       3.14159265358979323846

Slice::Slice() {

}

Slice::Slice(int width, int height) {
	Slice(width, height, 0.0f, 0.0f);
}

Slice::Slice(int width, int height, float origin_x, float origin_y) {
	this->m_width = width;
	this->m_height = height;
	this->m_ref_width = width;
	this->m_ref_height = height;

	for (int i = 0; i < m_height; i++) {
		std::vector<Voxel> v;
		for (int j = 0; j < m_width; j++) {
			Voxel voxel(j-origin_x, i-origin_y, 0.0f, 0);
			v.push_back(voxel);
		}
		m_slice.push_back(v);
	}
}

Slice::Slice(const Slice& s) {
	this->m_width = s.m_width;
	this->m_height = s.m_height;
	this->m_ref_width = s.m_ref_width;
	this->m_ref_height = s.m_ref_height;

	this->m_slice = s.m_slice;
}
Slice& Slice::operator=(const Slice& s) {
	this->m_width = s.m_width;
	this->m_height = s.m_height;
	this->m_ref_width = s.m_ref_width;
	this->m_ref_height = s.m_ref_height;

	this->m_slice = s.m_slice;

	return *this; 
}

std::tuple<int, int> Slice::getPositionOfVoxel(float x, float y, float z) {
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			Eigen::Vector4f pos = getVoxelCoord(j, i);
			if ((int)x == (int)pos(0) && (int)y == (int)pos(1) && (int)z == (int)pos(2)) {
				std::cout << j << i << std::endl;
				return std::make_tuple(j, i);
			}
		}
	}
}


void Slice::refTransform(Mode mode, Eigen::Vector4d axes_center_d, Eigen::Vector3f center,
						float degree_yaw, float degree_roll, float degree_pitch) {
	Eigen::Matrix4f mat;		// the final affine matrix for transformations
	Eigen::Matrix4f transform;	// temporal affine matrix for single transformation(translate, rotate)
	double cos_v, sin_v;

	// TODO: temporal conversion
	Eigen::Vector4f axes_center = axes_center_d.cast<float>();

	/*
		0. Create a plane(origin at center of image)
		1. Rotate plane for mode
		2. Rotate a plane about rotation axis
		3. Move a plane to center of axes
	*/

	mat.setIdentity();

	// 1. is not needed for axial
	if (mode == Mode::MODE_CORONAL) {

		// 1.
		transform.setIdentity();
		transform(1, 1) = transform(2, 2) = 0;
		transform(1, 2) = -1;
		transform(2, 1) = 1;
		mat = transform * mat;

	} else if (mode == Mode::MODE_SAGITTAL) {

		// 1.
		transform.setIdentity();
		transform(0, 0) = transform(2, 2) = 0;
		transform(0, 2) = -1;
		transform(2, 0) = 1;
		mat = transform * mat;
	}

	// 2.
	// Rotate about z-axis
	if (mode != Mode::MODE_AXIAL) {
		transform.setIdentity();
		cos_v = std::cos(degree_yaw * M_PI / 180);
		sin_v = std::sin(degree_yaw * M_PI / 180);
		transform(0, 0) = transform(1, 1) = cos_v;
		transform(0, 1) = -sin_v;
		transform(1, 0) = sin_v;
		mat = transform * mat;
	}
	

	// Rotate about y-axis
	if (mode != Mode::MODE_CORONAL) {
		transform.setIdentity();
		cos_v = std::cos(degree_roll * M_PI / 180);
		sin_v = std::sin(degree_roll * M_PI / 180);
		transform(0, 0) = transform(2, 2) = cos_v;
		transform(0, 2) = -sin_v;
		transform(2, 0) = sin_v;
		mat = transform * mat;
	}

	// Rotate about x-axis
	if (mode != Mode::MODE_SAGITTAL) {
		transform.setIdentity();
		cos_v = std::cos(degree_pitch * M_PI / 180);
		sin_v = std::sin(degree_pitch * M_PI / 180);
		transform(1, 1) = transform(2, 2) = cos_v;
		transform(1, 2) = -sin_v;
		transform(2, 1) = sin_v;
		mat = transform * mat;
	}

	//3.
	transform.setIdentity();
	transform.col(3) = axes_center;
	mat = transform * mat;

	std::cout << mat << std::endl;

	// create postion matrix
	Eigen::MatrixXf pos(m_height*m_width,4);
	Eigen::MatrixXf pos_t(4, m_height*m_width);

	int cnt = 0;
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			pos.row(cnt++) = m_slice[i][j].getCoord();
		}
	}
	pos_t = mat * pos.transpose();
	pos = pos_t.transpose();

	// Move img to center
	Eigen::Vector4f center_pixel = pos.row(m_height / 2.0f*m_width + m_width / 2.0f);
	center_pixel = center_pixel / center_pixel(3);
	mat.setIdentity();
	mat(0, 3) = center(0) - center_pixel(0);
	mat(1, 3) = center(1) - center_pixel(1);
	mat(2, 3) = center(2) - center_pixel(2);

	pos_t = mat * pos.transpose();
	pos = pos_t.transpose();

	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			Eigen::Vector4f new_pos = pos.row(i*m_width + j);
			new_pos = new_pos / new_pos(3);
			m_slice[i][j].setCoord(new_pos);
		}
	}
}

void Slice::refResize(int width, int height) {

	m_ref_width = width;
	m_ref_height = height;

	double width_rate = (double)width/m_width;
	double height_rate = (double)height/m_height;
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			Eigen::Vector4f pos=m_slice[i][j].getCoord();
			pos(0) = pos(0) * width_rate;		// x
			pos(1) = pos(1) * height_rate;		// y
			m_slice[i][j].setCoord(pos);
		}
	}

}