#ifndef __VOLUME_H__
#define __VOLUME_H__

#include <vector>

namespace vdcm {
	class Volume
	{
	public:
		Volume();

		friend Volume* read(const char*);

		std::vector<uint16_t> getAxialSlice(int);
	private:
		std::vector<std::vector<uint16_t> > m_volume;
	};
}
#endif // __VOLUME_H__