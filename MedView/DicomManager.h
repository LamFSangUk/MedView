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
	void extractSlice(int, int, int);

	int axial_idx;
	int coronal_idx;
	int sagittal_idx;
	int max_axial_idx;
	int max_coronal_idx;
	int max_sagittal_idx;

	Slice* axial_slice;
	Slice* coronal_slice;
	Slice* sagittal_slice;

signals:
	void changeVolume();
	void changeSlice(int, QImage*);
private slots:
	void setSliceIdx(int, int, int, int);
private:
	const char* m_filename;

	vdcm::Volume* m_volume;
	vdcm::Axes* m_axes;
};

#endif // __DICOM_MANAGER_H__