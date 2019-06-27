#ifndef __IMAGE_VIEW_WIDGET__
#define __IMAGE_VIEW_WIDGET__

#include <QLabel>
#include <QSlider>
#include <QTextEdit>

#include "volume.h"
#include "CursorCoordinator.h"

#define MODE_AXIAL 0
#define MODE_SAGITTAL 1
#define MODE_CORONAL 2

class ImageViewWidget : public QLabel
{
	Q_OBJECT
public:
	ImageViewWidget(int);

	void setVolume(vdcm::Volume*);

	void setSlider(QSlider*);

	void mouseMoveEvent(QMouseEvent *e) override;

signals:
	void changeCoords(int, int, int);

private slots:
	void setIndex(int);

private:
	int m_mode;

	int m_idx_slice;
	int m_idx_max;
	vdcm::Volume* m_volume;
	
	QSlider* m_slider;
	CursorCoordinator* m_cur_coord;

	void draw();
};

#endif // __IMAGE_VIEW_WIDGET__