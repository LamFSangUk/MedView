#include "voxel.h"

Voxel::Voxel() {
	Voxel(0.0, 0.0, 0.0, 0);
}

Voxel::Voxel(float x, float y, float z, int intensity = 0) {
	this->setCoord(Eigen::Vector4f(x, y, z, 1.0));

	this->m_intensity = intensity;
}

void Voxel::setCoord(Eigen::Vector4f pos) {
	this->m_coord = pos;
}

Eigen::Vector4f Voxel::getCoord() {
	return m_coord;
}

void Voxel::setIntensity(int intensity) {
	this->m_intensity = intensity;
}

int Voxel::getIntensity() {
	return m_intensity;
}