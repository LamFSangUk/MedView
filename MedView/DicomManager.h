#ifndef __DICOM_MANAGER_H__
#define __DICOM_MANAGER_H__

#include <QObject>
#include <QImage>

#include "global.h"
#include "volume.h"
#include "axes.h"
#include "slice.h"

class DicomManager : public QObject{
	Q_OBJECT
public:
	DicomManager(QObject*);
	void readDicom(const char*);
	void extractSlice(int);
	std::vector<int> getVoxelInfo(int, int, int);
	inline QSize getStandardSliceSize();

	int axial_idx;
	int coronal_idx;
	int sagittal_idx;
	int max_axial_idx;
	int max_coronal_idx;
	int max_sagittal_idx;

	Slice* axial_slice;
	Slice* coronal_slice;
	Slice* sagittal_slice;

	std::vector<QLine> getAxesLines(int, int, int);

	void reset();

signals:
	void changeVolume();
	void changeSlice(int, QImage*);
	void changeAxes();
private slots:
	void setSliceIdx(int, int);
	void setDegree(int, float);
private:
	const char* m_filename;

	vdcm::Volume* m_volume;
	vdcm::Axes* m_axes;

	const QSize m_standard_slice_size = QSize(600, 600);
	void calculateIdxes();
	bool isPointOutOfVolume(Eigen::Vector3f);
};

inline QSize  DicomManager::getStandardSliceSize() {
	return m_standard_slice_size;
}

#endif // __DICOM_MANAGER_H__