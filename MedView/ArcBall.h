#ifndef __ARCBALL_H__
#define __ARCBALL_H__

#include <Eigen/Dense>

typedef struct ScreenCoord {
	float x, y;
}ScreenCoord;

typedef struct SpehreCoord {
	float x, y, z;
}SphereCoord;

class ArcBall {
public:
	ArcBall(int, int);
	~ArcBall();

	void setBounds(int, int);
	void setStart(int, int);
	float* getRotationMatrix(int, int);
private:
	int m_window_width;
	int m_window_height;

	SpehreCoord m_start;

	SphereCoord _toSphereCoord(ScreenCoord);
};

#endif //__ARCBALL_H__