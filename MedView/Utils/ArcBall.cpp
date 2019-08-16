#include "ArcBall.h"
#include <iostream>

ArcBall::ArcBall(int width, int height) {
	this->setBounds(width, height);

	this->m_quat_anchor.setIdentity();
	this->m_quat_now.setIdentity();
}

ArcBall::~ArcBall() {

}

void ArcBall::reset() {
	this->m_quat_anchor.setIdentity();
	this->m_quat_now.setIdentity();
}

/**
 * Resize screen and arcball
 */
void ArcBall::setBounds(int width, int height) {
	this->m_window_width = width;
	this->m_window_height = height;

	this->m_radius = std::min(width, height)/2.0f;
	this->m_origin << width / 2.0, height / 2.0, 0;
}

void ArcBall::setStart(int x, int y) {
	ScreenCoord screen_coord;
	screen_coord.x = x; screen_coord.y = y;

	m_anchor = _toSphereCoord(screen_coord);

	m_quat_anchor = m_quat_now;
}

void ArcBall::rotate(int cur_x, int cur_y) {
	ScreenCoord cur_screen;
	cur_screen.x = cur_x; cur_screen.y = cur_y;

	SpehreCoord prev, cur;
	prev = m_anchor;
	cur = _toSphereCoord(cur_screen);

	Eigen::Vector3f prev_vector = Eigen::Vector3f(prev.x, prev.y, prev.z).normalized();
	Eigen::Vector3f cur_vector = Eigen::Vector3f(cur.x, cur.y, cur.z).normalized();

	float dot = prev_vector.dot(cur_vector);
	float angle = 2 * acos(std::min(1.0f, dot));

	Eigen::Vector3f cam_axis = prev_vector.cross(cur_vector).normalized();

	Eigen::Quaternion<float> rot;
	rot = Eigen::AngleAxis<float>(angle, cam_axis);
	m_quat_now = rot * m_quat_anchor;
}

float* ArcBall::getRotationMatrix() {

	Eigen::Matrix4f mat4 = Eigen::Matrix4f::Identity();
	mat4.block(0,0,3,3) = m_quat_now.normalized().toRotationMatrix();

	/* Type conversion from Eigen Matrix to raw c++ array */
	float* res = new float[16];
	Eigen::Map<Eigen::Matrix<float, 4, 4, Eigen::RowMajor> >(res, 4, 4) = mat4;

	return res;
}


/**
 * Translate coordinate
 */
SphereCoord ArcBall::_toSphereCoord(ScreenCoord screen) {
	SphereCoord sphere;
	sphere.x = (screen.x - m_origin(0)) / m_radius;
	sphere.y = -(screen.y - m_origin(1)) / m_radius;

	float length_squared = (sphere.x * sphere.x) + (sphere.y * sphere.y);

	if (length_squared > 1.0f) {
		float norm = (float)(1.0f / sqrt(length_squared));

		sphere.x = sphere.x * norm;
		sphere.y = sphere.y * norm;
		sphere.z = 0.0f;
	}
	else {
		sphere.z = sqrt(1.0f - length_squared);
	}

	return sphere;
}
