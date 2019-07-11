#include "axes.h"

#include <cmath>
#define M_PI       3.14159265358979323846

#include<iostream>

namespace vdcm {
	Axes::Axes() {
		Axes(0.0, 0.0, 0.0);
	}

	Axes::Axes(double x, double y, double z) {
		m_axis_axial << 0.0, 0.0, 1.0;
		m_axis_coronal << 1.0, 0.0, 0.0;
		m_axis_sagittal << 0.0, 1.0, 0.0;

		m_degree_x = 0.0f;
		m_degree_y = 0.0f;
		m_degree_z = 0.0f;

		this->setCenter(x, y, z);
	}

	Eigen::Vector3f Axes::getAxis(int mode) {
		switch (mode) {
		case MODE_AXIAL:
			return m_axis_axial;
		case MODE_CORONAL:
			return m_axis_coronal;
		case MODE_SAGITTAL:
			return m_axis_sagittal;
		default:
			break;
		}
		return Eigen::Vector3f(0, 0, 0);

	}

	void Axes::reset(double x, double y, double z) {
		m_axis_axial << 0.0, 0.0, 1.0;
		m_axis_coronal << 1.0, 0.0, 0.0;
		m_axis_sagittal << 0.0, 1.0, 0.0;

		m_degree_x = 0.0f;
		m_degree_y = 0.0f;
		m_degree_z = 0.0f;

		this->setCenter(x, y, z);
	}

	void Axes::setCenter(double x, double y, double z) {
		m_center = Eigen::Vector4d(x, y, z, 1.0);
	}

	Eigen::Vector4d Axes::getCenter() {
		return m_center;
	}

	void Axes::addYaw(float angle) {
		m_degree_z += angle;

		// Handle overflow and underflow
		if (m_degree_z < 0) m_degree_z = 360 - m_degree_z;
		else if (m_degree_z >= 360) m_degree_z -= 360;

		Eigen::Matrix4f mat;
		float cos_v;
		float sin_v;

		mat.setIdentity();
		cos_v = std::cos(angle * M_PI / 180);
		sin_v = std::sin(angle * M_PI / 180);
		mat(0, 0) = mat(1,1) = cos_v;
		mat(0, 1) = -sin_v;
		mat(1, 0) = sin_v;

		Eigen::Vector4f dir;

		// Direction of Y-axis
		dir << m_axis_coronal(0), m_axis_coronal(1), m_axis_coronal(2), 1;
		dir = mat * dir;
		dir /= dir(3);
		m_axis_coronal << dir(0), dir(1), dir(2);
		m_axis_coronal.normalize();

		std::cout << "Coronal"<< m_axis_coronal << std::endl;

		// Direction of X-axis
		dir << m_axis_sagittal(0), m_axis_sagittal(1), m_axis_sagittal(2), 1;
		dir = mat * dir;
		dir /= dir(3);
		m_axis_sagittal << dir(0), dir(1), dir(2);
		m_axis_sagittal.normalize();
	}
	void Axes::addRoll(float angle) {
		m_degree_y += angle;

		// Handle overflow and underflow
		if (m_degree_y < 0) m_degree_y = 360 - m_degree_y;
		else if (m_degree_y >= 360) m_degree_y -= 360;

		Eigen::Matrix4f mat;
		float cos_v;
		float sin_v;

		mat.setIdentity();
		cos_v = std::cos(angle * M_PI / 180);
		sin_v = std::sin(angle * M_PI / 180);
		mat(0, 0) = mat(2, 2) = cos_v;
		mat(0, 2) = -sin_v;
		mat(2, 0) = sin_v;

		Eigen::Vector4f dir;

		// Direction of X-axis
		dir << m_axis_coronal(0), m_axis_coronal(1), m_axis_coronal(2), 1;
		dir = mat * dir;
		dir /= dir(3);
		m_axis_coronal << dir(0), dir(1), dir(2);
		m_axis_coronal.normalize();
		
		// Direction of Z-axis
		dir << m_axis_axial(0), m_axis_axial(1), m_axis_axial(2), 1;
		dir = mat * dir;
		dir /= dir(3);
		m_axis_axial << dir(0), dir(1), dir(2);
		m_axis_axial.normalize();
	}
	void Axes::addPitch(float angle) {
		m_degree_x += angle;

		// Handle overflow and underflow
		if (m_degree_x < 0) m_degree_x = 360 - m_degree_x;
		else if (m_degree_x >= 360) m_degree_x -= 360;

		Eigen::Matrix4f mat;
		float cos_v;
		float sin_v;

		mat.setIdentity();
		cos_v = std::cos(angle * M_PI / 180);
		sin_v = std::sin(angle * M_PI / 180);
		mat(1, 1) = mat(2, 2) = cos_v;
		mat(1, 2) = -sin_v;
		mat(2, 1) = sin_v;

		Eigen::Vector4f dir;

		// Direction of Y-axis
		dir << m_axis_sagittal(0), m_axis_sagittal(1), m_axis_sagittal(2), 1;
		dir = mat * dir;
		dir /= dir(3);
		m_axis_sagittal << dir(0), dir(1), dir(2);
		m_axis_sagittal.normalize();

		// Direction of Z-axis
		dir << m_axis_axial(0), m_axis_axial(1), m_axis_axial(2), 1;
		dir = mat * dir;
		dir /= dir(3);
		m_axis_axial << dir(0), dir(1), dir(2);
		m_axis_axial.normalize();
	}
}