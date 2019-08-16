#ifndef __VOXEL_H__
#define __VOXEL_H__

#include <Eigen/Dense>

class Voxel {
public:
	Voxel();
	Voxel(float, float, float, int);

	void setCoord(Eigen::Vector4f);
	Eigen::Vector4f getCoord();
	void setIntensity(int);
	int getIntensity();
private:
	Eigen::Vector4f m_coord;
	int m_intensity;
};

#endif // __VOXEL_H__