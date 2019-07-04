#ifndef __VOXEL_H__
#define __VOXEL_H__

#include "global.h"

class Voxel {
public:
	Voxel();
	Voxel(double, double, double, int);

	void setCoord(Eigen::Vector4d);
	Eigen::Vector4d getCoord();
	void setIntensity(int);
	int getIntensity();
private:
	Eigen::Vector4d m_coord;
	int m_intensity;
};

#endif // __VOXEL_H__