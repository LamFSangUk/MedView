#include "ArcBall.h"
#include <iostream>

ArcBall::ArcBall(int width, int height) {
	this->m_window_width = width;
	this->m_window_height = height;
}

ArcBall::~ArcBall() {

}

/**
 * Resize screen and arcball
 */
void ArcBall::setBounds(int width, int height) {
	this->m_window_width = width;
	this->m_window_height = height;
}

void ArcBall::setStart(int x, int y) {
	ScreenCoord screen_coord;
	screen_coord.x = x; screen_coord.y = y;

	m_start = _toSphereCoord(screen_coord);
}

float* ArcBall::getRotationMatrix(int cur_x, int cur_y) {
	ScreenCoord cur_screen;
	cur_screen.x = cur_x; cur_screen.y = cur_y;

	SpehreCoord prev, cur;
	prev = m_start;
	cur = _toSphereCoord(cur_screen);

	Eigen::Vector3f prev_vector = Eigen::Vector3f(prev.x, prev.y, prev.z);
	Eigen::Vector3f cur_vector = Eigen::Vector3f(cur.x, cur.y, cur.z);

	float dot = prev_vector.dot(cur_vector);
	float angle = acos(std::min(1.0f, dot));

	Eigen::Vector3f cam_axis = prev_vector.cross(cur_vector);
	cam_axis.normalize();
	std::cout << cam_axis << std::endl;

	Eigen::Affine3f rotation;
	rotation = Eigen::AngleAxisf(angle/10, cam_axis);

	Eigen::Matrix4f affine_rotation;
	affine_rotation = rotation.matrix();

	/* Type conversion from Eigen Matrix to raw c++ array */
	float* res = new float[16];
	Eigen::Map<Eigen::Matrix<float, 4, 4, Eigen::RowMajor> >(res, 4, 4) = affine_rotation;

	return res;
}


/**
 * Translate coordinate
 */
SphereCoord ArcBall::_toSphereCoord(ScreenCoord screen) {
	SphereCoord sphere;
	sphere.x = (2 * screen.x - m_window_width) / m_window_width;
	sphere.y = -(2 * screen.y - m_window_height) / m_window_height;

	float length_squared = (sphere.x * sphere.x) + (sphere.y * sphere.y);

	if (length_squared > 1.0f) {
		float norm = (float)(1.0 / sqrt(length_squared));

		sphere.x = sphere.x * norm;
		sphere.y = sphere.y * norm;
		sphere.z = 0.0f;
	}
	else {
		sphere.z = sqrt(1.0f - length_squared);
	}

	return sphere;
}
