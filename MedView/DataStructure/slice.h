#ifndef __SLICE_H__
#define __SLICE_H__

#include "voxel.h"
#include <vector>

#include "common.h"

#include <Eigen/Dense>

class Slice {
public:
	Slice();
	Slice(int, int);
	Slice(int, int, float, float);
	Slice(const Slice&);
	Slice& operator=(const Slice&);
	~Slice();

	void refTransform(Mode, Eigen::Vector4d, Eigen::Vector3f, float, float, float);
	void refResize(int, int);

	int getWidth();
	int getHeight();
	Eigen::Vector4f getVoxelCoord(int, int);
	void setVoxelIntensity(int, int, int);
	int getVoxelIntensity(int, int);
	std::tuple<int, int> getPositionOfVoxel(float, float, float);
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

inline Eigen::Vector4f Slice::getVoxelCoord(int x, int y) {
	return m_slice[y][x].getCoord();
}

inline void Slice::setVoxelIntensity(int x, int y, int intensity) {
	m_slice[y][x].setIntensity(intensity);
}

inline int Slice::getVoxelIntensity(int x, int y) {
	return m_slice[y][x].getIntensity();
}

#endif // __SLICE_H__
