#ifndef __IMAGE_VIEW_WIDGET__
#define __IMAGE_VIEW_WIDGET__

#include <QWidget>
#include <QSlider>
#include <QTextEdit>

#include "volume.h"
#include "SliceWidget.h"
#include "DicomManager.h"

#define MODE_AXIAL 0
#define MODE_SAGITTAL 1
#define MODE_CORONAL 2

class ImageViewWidget : public QWidget
{
	Q_OBJECT
public:
	ImageViewWidget(int, DicomManager*, QWidget*);

	//void setVolume(vdcm::Volume*);

	void setSlider(QSlider*);

signals:
	void changeSliceIdx(int,int,int,int);
private slots:
	void setIndex(int);
	void initView();
	void draw(int, QImage*);
	void setLines();

private:
	int m_mode;

	int m_idx_slice;
	int m_idx_max;
	//vdcm::Volume* m_volume;
	DicomManager *m_dicom_manager;
	
	SliceWidget* m_slice_view;
	QSlider* m_slider;

	void buildLayout();
	void getSlice();
};

#endif // __IMAGE_VIEW_WIDGET__