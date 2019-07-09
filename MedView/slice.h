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

inline int Slice::getWidth() {
	return m_width;
}
inline int Slice::getHeight() {
	return m_height;
}

inline Eigen::Vector4d Slice::getVoxelCoord(int x, int y) {
	return m_slice[y][x].getCoord();
}

inline void Slice::setVoxelIntensity(int x, int y, int intensity) {
	m_slice[y][x].setIntensity(intensity);
}

inline int Slice::getVoxelIntensity(int x, int y) {
	return m_slice[y][x].getIntensity();
}

#endif // __SLICE_H__
