#ifndef __IMAGE_VIEW_WIDGET__
#define __IMAGE_VIEW_WIDGET__

#include <QLabel>
#include "volume.h"

#define MODE_AXIAL 0
#define MODE_SAGITTAL 1
#define MODE_CORONAL 2

class ImageViewWidget : public QLabel
{
public:
	ImageViewWidget(int);

	void setVolume(vdcm::Volume* m_volume);

private:
	int m_mode;
	vdcm::Volume* m_volume;

	void draw();
};

#endif // __IMAGE_VIEW_WIDGET__