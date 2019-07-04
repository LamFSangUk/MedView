#include "axes.h"

namespace vdcm {
	Axes::Axes() {
		Axes(0.0, 0.0, 0.0);
	}

	Axes::Axes(double x, double y, double z) {
		m_axis_axial << 0.0, 0.0, 1.0;
		m_axis_coronal << 1.0, 0.0, 0.0;
		m_axis_sagittal << 0.0, 1.0, 0.0;

		m_degree_x = 0.0;
		m_degree_y = 0.0;
		m_degree_z = 0.0;

		this->setCenter(x, y, z);
	}

	Eigen::Vector3d Axes::getAxis(int mode) {
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
		return Eigen::Vector3d(0, 0, 0);

	}

	void Axes::setCenter(double x, double y, double z) {
		m_center = Eigen::Vector4d(x, y, z, 1.0);
	}

	Eigen::Vector4d Axes::getCenter() {
		return m_center;
	}

	void Axes::setYaw(double angle) {
		m_degree_z = angle;
	}
	double Axes::getYaw() {
		return m_degree_z;
	}
	void Axes::setRoll(double angle) {
		m_degree_y = angle;
	}
	double Axes::getRoll() {
		return m_degree_y;
	}
	void Axes::setPitch(double angle) {
		m_degree_x = angle;
	}
	double Axes::getPitch() {
		return m_degree_x;
	}
}