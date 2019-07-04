#ifndef __SLICE_H__
#define __SLICE_H__

#include "global.h"
#include "voxel.h"
#include <vector>

class Slice {
public:
	Slice();
	Slice(int, int);

	void refTransform(int, Eigen::Vector4d, double, double, double);
	void refResize(int, int);

	int getWidth();
	int getHeight();
	Eigen::Vector4d getVoxelCoord(int, int);
	void setVoxelIntensity(int, int, int);
	int getVoxelIntensity(int, int);
private:
	int m_width;
	int m_height;

	int m_ref_width;
	int m_ref_height;

	std::vector<std::vector<Voxel > > m_slice;
};

#endif // __SLICE_H__
