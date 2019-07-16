#ifndef __IMAGE_VIEW_WIDGET__
#define __IMAGE_VIEW_WIDGET__

#include <QWidget>
#include <QSlider>
#include <QTextEdit>

#include "volume.h"
#include "SliceWidget.h"
#include "DicomManager.h"

#include "common.h"

class ImageViewWidget : public QWidget
{
	Q_OBJECT
public:
	ImageViewWidget(Mode, DicomManager*, QWidget*);

	void setSlider(QSlider*);
	std::vector<int> getPixelInfo(int, int);		// Trnaslate label pixel to voxel information

	Mode getMode();

signals:

private slots:
	void initView();
	void setLines();
	void updateView(std::map<Mode, DicomManager::SlicePacket>);

	void increaseIndex();
	void decreaseIndex();

private:
	Mode m_mode;

	int m_idx_slice;
	int m_idx_max;
	DicomManager *m_dicom_manager;
	
	SliceWidget* m_slice_view;
	QSlider* m_slider;

	void buildLayout();
	void draw(Mode, QImage*);

};

inline Mode ImageViewWidget::getMode() {
	return m_mode;
}

#endif // __IMAGE_VIEW_WIDGET__