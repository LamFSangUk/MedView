#ifndef __DICOM_MANAGER_H__
#define __DICOM_MANAGER_H__

#include <QObject>
#include <QImage>

#include "volume.h"
#include "axes.h"
#include "slice.h"

#include "common.h"

class DicomManager : public QObject{
	Q_OBJECT
public:
	DicomManager(QObject*);
	void readDicom(const char*);
	std::vector<int> getVoxelInfo(Mode, int, int);
	inline QSize getStandardSliceSize();

	/* Data Structure for sending data to receiver */
	class SlicePacket {
	public:
		QSize size;
		int cur_idx;
		int max_idx;
		QImage slice;
		QPoint line_center;
		float angle;
	};


	std::vector<QLine> getAxesLines(Mode, int, int);

	void reset();

	void updateSliceIndex(Mode, int);
	void updateAngle(Mode, float);

	const char* getFilename();
	
signals:
	void changeVolume();
	void changeSlice(std::map<Mode, SlicePacket>);
	void changeAxes();

	void changeValue();

private slots:
	void setDegree(Mode, float);
private:
	const char* m_filename;

	vdcm::Volume* m_volume;
	vdcm::Axes* m_axes;

	void calculateIdxes();

	void _initValues();


	/* Slices */
	const QSize m_standard_slice_size = QSize(600, 600);

	int m_axial_idx;
	int m_coronal_idx;
	int m_sagittal_idx;
	int m_max_axial_idx;
	int m_max_coronal_idx;
	int m_max_sagittal_idx;

	Eigen::Vector3f m_axial_center;
	Eigen::Vector3f m_coronal_center;
	Eigen::Vector3f m_sagittal_center;
	
	Slice* m_axial_slice;
	Slice* m_coronal_slice;
	Slice* m_sagittal_slice;

	QImage _convertSliceToImage(Slice*);

	void _updateSlice();

	/* utils */
	bool _isPointOutOfVolume(Eigen::Vector3f);
};

inline QSize DicomManager::getStandardSliceSize() {
	return m_standard_slice_size;
}

inline const char* DicomManager::getFilename() {
	return m_filename;
}

#endif // __DICOM_MANAGER_H__