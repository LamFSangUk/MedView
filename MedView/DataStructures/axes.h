#ifndef __AXES_H__
#define __AXES_H__

#include "common.h"

#include <Eigen/Dense>

namespace vdcm {
	class Axes
	{
	public:
		Axes();
		Axes(double, double, double);

		Eigen::Vector3f getAxis(Mode);
		float getAngle(Mode);
		void setCenter(double, double, double);
		Eigen::Vector4d getCenter();
		std::tuple<double, double> getPlaneCenter(Mode);

		void setYaw(float);
		float getYaw();
		void setRoll(float);
		float getRoll();
		void setPitch(float);
		float getPitch();

		void addYaw(float);
		void addRoll(float);
		void addPitch(float);

		void reset(double, double, double);
	private:
		Eigen::Vector4d m_center;

		Eigen::Vector3f m_axis_axial;
		Eigen::Vector3f m_axis_coronal;
		Eigen::Vector3f m_axis_sagittal;

		// Degrees
		float m_degree_x;
		float m_degree_y;
		float m_degree_z;
	};

	inline void Axes::setYaw(float angle) {
		m_degree_z = angle;
	}
	inline float Axes::getYaw() {
		return m_degree_z;
	}
	inline void Axes::setRoll(float angle) {
		m_degree_y = angle;
	}
	inline float Axes::getRoll() {
		return m_degree_y;
	}
	inline void Axes::setPitch(float angle) {
		m_degree_x = angle;
	}
	inline float Axes::getPitch() {
		return m_degree_x;
	}
}



#endif // __AXES_H__