#ifndef __AXES_H__
#define __AXES_H__

#include "global.h"

namespace vdcm {
	class Axes
	{
	public:
		Axes();
		Axes(double, double, double);

		Eigen::Vector3d getAxis(int);
		void setCenter(double, double, double);
		Eigen::Vector4d getCenter();

		void setYaw(double);
		double getYaw();
		void setRoll(double);
		double getRoll();
		void setPitch(double);
		double getPitch();
	private:
		Eigen::Vector4d m_center;

		Eigen::Vector3d m_axis_axial;
		Eigen::Vector3d m_axis_coronal;
		Eigen::Vector3d m_axis_sagittal;

		// Degrees
		double m_degree_x;
		double m_degree_y;
		double m_degree_z;
	};
}

#endif // __AXES_H__