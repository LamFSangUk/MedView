#include "voxel.h"

Voxel::Voxel() {
	Voxel(0.0, 0.0, 0.0, 0);
}

Voxel::Voxel(double x, double y, double z, int intensity = 0) {
	this->setCoord(Eigen::Vector4d(x, y, z, 1.0));

	this->m_intensity = intensity;
}

void Voxel::setCoord(Eigen::Vector4d pos) {
	this->m_coord = pos;
}

Eigen::Vector4d Voxel::getCoord() {
	return m_coord;
}

void Voxel::setIntensity(int intensity) {
	this->m_intensity = intensity;
}

int Voxel::getIntensity() {
	return m_intensity;
}