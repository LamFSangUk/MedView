#ifndef __VOLUME_H__
#define __VOLUME_H__

#include <vector>
#include "slice.h"
#include "axes.h"

#include "common.h"
#include <Eigen/Dense>

namespace vdcm {
	class Volume
	{
	public:
		Volume();

		friend Volume* read(const char*);

		Slice* getSlice(Mode, Axes*, int, int, Eigen::Vector3f);

		int getWidth();
		int getHeight();
		int getDepth();

		float* getBuffer();
	private:
		std::vector<std::vector<int16_t> > m_volume_data;
		int m_width;
		int m_height;
		int m_depth;

		int m_rescale_slope;
		int m_rescale_intercept;
	};
}
#endif // __VOLUME_H__