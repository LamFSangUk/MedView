#include "slice.h"
#include <iostream>

#include <cmath>
#define M_PI       3.14159265358979323846

Slice::Slice() {

}

Slice::Slice(int width, int height) {
	this->m_width = width;
	this->m_height = height;
	this->m_ref_width = width;
	this->m_ref_height = height;

	for (int i = 0; i < m_height; i++) {
		std::vector<Voxel> v;
		for (int j = 0; j < m_width; j++) {
			Voxel voxel((double)j, (double)i, 0.0, 0);
			v.push_back(voxel);
		}
		m_slice.push_back(v);
	}
}

void Slice::refTransform(int mode, Eigen::Vector4d axes_center, double degree_yaw, double degree_roll, double degree_pitch) {
	Eigen::Matrix4d mat;		// the final affine matrix for transformations
	Eigen::Matrix4d transform;	// temporal affine matrix for single transformation(translate, rotate)
	double cos_v, sin_v;

	/*
		0. Create a plane(Slice object and using refResize)
		1. Rotate plane for mode
			1-1. Move a plane to axes center to rotate about center of axes
			1-2. Do Yaw, Roll or Pitch
			1-3. Move a plane to previous origin
		2. Move a plane to origin coordinate of rotation axes
		3. Rotate a plane about rotation axis
		4. Move a plane to previous coordinate
	*/

	//Translation
	Eigen::Vector4d img_center;

	mat.setIdentity();

	// 1-1.
	img_center << -m_ref_width, -m_ref_height, 0, 1;
	mat.col(3) = img_center;

	double x_move = 0;
	double y_move = 0;
	double z_move = 0;

	if (mode == MODE_AXIAL) {
		// 1-2. Not needed for axial
		// 1-3.
		img_center << m_ref_width, m_ref_height, 0, 1;
		transform.setIdentity();
		transform.col(3) = img_center;
		mat = transform * mat;

		// 2.
		z_move = axes_center(2);

		axes_center(0) *= -1;
		axes_center(1) *= -1;
		axes_center(2) = 0;
		transform.setIdentity();
		transform.col(3) = axes_center;
		mat = transform * mat;

		axes_center(2) = -z_move;

	} else if (mode == MODE_CORONAL) {

		// 1-2.
		transform.setIdentity();
		transform(1, 1) = transform(2, 2) = 0;
		transform(1, 2) = -1;
		transform(2, 1) = 1;
		mat = transform * mat;

		transform.setIdentity();
		transform(0, 0) = transform(1, 1) = 0;
		transform(0, 1) = -1;
		transform(1, 0) = 1;
		mat = transform * mat;

		// 1-3.
		img_center << 0, m_ref_width, m_ref_height, 1;
		transform.setIdentity();
		transform.col(3) = img_center;
		mat = transform * mat;

		// 2.
		x_move = axes_center(0);

		axes_center(0) = 0;
		axes_center(1) *= -1;
		axes_center(2) *= -1;
		transform.setIdentity();
		transform.col(3) = axes_center;
		mat = transform * mat;

		axes_center(0) = -x_move;

	} else if (mode == MODE_SAGITTAL) {

		// 1-2.
		transform.setIdentity();
		transform(1, 1) = transform(2, 2) = 0;
		transform(1, 2) = -1;
		transform(2, 1) = 1;
		mat = transform * mat;

		// 1-3.
		img_center << m_ref_width, 0, m_ref_height, 1;
		transform.setIdentity();
		transform.col(3) = img_center;
		mat = transform * mat;

		// 2.
		y_move = axes_center(1);

		axes_center(0) *= -1;
		axes_center(1) = 0;
		axes_center(2) *= -1;
		transform.setIdentity();
		transform.col(3) = axes_center;
		mat = transform * mat;

		axes_center(1) = -y_move;
	}

	// 3.

	// Rotate about z-axis
	transform.setIdentity();
	cos_v = std::cos(degree_yaw * M_PI / 180);
	sin_v = std::sin(degree_yaw * M_PI / 180);
	transform(0, 0) = transform(1, 1) = cos_v;
	transform(0, 1) = -sin_v;
	transform(1, 0) = sin_v;
	mat = transform * mat;

	// Rotate about y-axis
	transform.setIdentity();
	cos_v = std::cos(degree_roll * M_PI / 180);
	sin_v = std::sin(degree_roll * M_PI / 180);
	transform(0, 0) = transform(2, 2) = cos_v;
	transform(0, 2) = -sin_v;
	transform(2, 0) = sin_v;
	mat = transform * mat;

	// Rotate about x-axis
	transform.setIdentity();
	cos_v = std::cos(degree_pitch * M_PI / 180);
	sin_v = std::sin(degree_pitch * M_PI / 180);
	transform(1, 1) = transform(2, 2) = cos_v;
	transform(1, 2) = -sin_v;
	transform(2, 1) = sin_v;
	mat = transform * mat;

	//4.
	transform.setIdentity();
	axes_center = axes_center * -1;
	axes_center(3) = 1;
	transform.col(3) = axes_center;
	mat = transform * mat;

	std::cout << mat << std::endl;

	// create postion matrix
	Eigen::MatrixXd pos(m_height*m_width,4);
	Eigen::MatrixXd pos_t(4, m_height*m_width);

	int cnt = 0;
	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			pos.row(cnt++) = m_slice[i][j].getCoord();
		}
	}
	pos_t = mat * pos.transpose();
	pos = pos_t.transpose();


	for (int i = 0; i < m_height; i++) {
		for (int j = 0; j < m_width; j++) {
			Eigen::Vector4d new_pos = pos.row(i*m_width + j);
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
			Eigen::Vector4d pos=m_slice[i][j].getCoord();
			pos(0) = pos(0) * width_rate;		// x
			pos(1) = pos(1) * height_rate;		// y
			m_slice[i][j].setCoord(pos);
		}
	}

}