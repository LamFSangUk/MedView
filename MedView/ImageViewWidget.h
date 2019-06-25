#ifndef __IMAGE_VIEW_WIDGET__
#define __IMAGE_VIEW_WIDGET__

#include <QLabel>
#include "volume.h"

class ImageViewWidget : public QLabel
{
public:
	ImageViewWidget();

	void setVolume(vdcm::Volume* m_volume);

private:
	vdcm::Volume* m_volume;
	void draw();
};

#endif // __IMAGE_VIEW_WIDGET__