#ifndef __VOLUME_H__
#define __VOLUME_H__

#include <vector>
#include "global.h"
#include "slice.h"
#include "axes.h"

namespace vdcm {
	class Volume
	{
	public:
		Volume();

		friend Volume* read(const char*);

		std::vector<uint16_t> getAxialSlice(int);
		std::vector<uint16_t> getCoronalSlice(int);
		std::vector<uint16_t> getSagittalSlice(int);
		Slice* getSlice(int, Axes*);

		int getWidth();
		int getHeight();
		int getDepth();
	private:
		std::vector<std::vector<uint16_t> > m_volume_data;
		int m_width;
		int m_height;
		int m_depth;
	};
}
#endif // __VOLUME_H__