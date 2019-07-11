#include "ImageViewWidget.h"
#include "DicomManager.h"

#include <QLayout>

#include <algorithm>
#include <climits>

#include <QDebug>

ImageViewWidget::ImageViewWidget(int mode, DicomManager *dicom_manager,QWidget* parent=NULL)
 : QWidget(parent)
{
	this->m_mode = mode;

	this->m_dicom_manager = dicom_manager;

	this->m_idx_slice = 0;
	this->m_idx_max = 0;

	// Set Layout
	this->buildLayout();

	connect(m_dicom_manager, SIGNAL(changeVolume()), this, SLOT(initView()));
}

void ImageViewWidget::buildLayout() {
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);

	// Slider size
	QSize max_size_slider(1000, 10);
	QSize min_size_slider(100, 10);

	// SliceWidget size
	//QSize max_size_label(m_width / 2,m_height / 2);
	QSize min_size_slice(512, 512);

	m_slice_view = new SliceWidget(this);
	m_slice_view->setMinimumSize(min_size_slice);

	m_slider = new QSlider(Qt::Horizontal, this);
	m_slider->setMaximumSize(max_size_slider);
	m_slider->setMinimumSize(min_size_slider);
	m_slider->setEnabled(false);
	m_slider->setObjectName("slider");

	layout->addWidget(m_slice_view, 0, Qt::AlignCenter);
	layout->addWidget(m_slider, 0, Qt::AlignBottom);

	this->setLayout(layout);
}

void ImageViewWidget::initView() {

	connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(setIndex(int)));
	connect(this, SIGNAL(changeSliceIdx(int, int)), m_dicom_manager, SLOT(setSliceIdx(int, int)));
	connect(m_dicom_manager, SIGNAL(changeSlice(int, QImage*)), this, SLOT(draw(int, QImage*)));
	connect(m_dicom_manager, SIGNAL(changeAxes()), this, SLOT(setLines()));

	connect(m_slice_view, SIGNAL(requestIncIndex()), this, SLOT(increaseIndex()));
	connect(m_slice_view, SIGNAL(requestDecIndex()), this, SLOT(decreaseIndex()));
	
	connect(m_slice_view, SIGNAL(changeDegree(int,float)), m_dicom_manager, SLOT(setDegree(int, float)));

	m_slider->setEnabled(true);
	m_slider->setMinimum(0);
	switch (this->m_mode) {
		case MODE_AXIAL:
			m_slider->setMaximum(m_dicom_manager->max_axial_idx);
			m_slider->setValue(m_dicom_manager->axial_idx);
			break;
		case MODE_CORONAL:
			m_slider->setMaximum(m_dicom_manager->max_coronal_idx);
			m_slider->setValue(m_dicom_manager->coronal_idx);
			break;
		case MODE_SAGITTAL:
			m_slider->setMaximum(m_dicom_manager->max_sagittal_idx);
			m_slider->setValue(m_dicom_manager->sagittal_idx);
			break;
		default:
			throw std::runtime_error("Not assigned Mode");
			return;
	}
}

void ImageViewWidget::draw(int mode, QImage* img) {
	qDebug() << mode << m_mode;
	if (mode == m_mode)
		m_slice_view->drawSlice(img);
	else
		m_slice_view->drawSlice(nullptr);
}

void ImageViewWidget::setSlider(QSlider* slider) {
	this->m_slider = slider;
}

/**
 * Get voxel information from 2D slice about specific axes
 *
 * @param int x : point coordinate x on label
 * @param int y : point coordinate y on label
 * @param int label_size : label frame size
 */
std::vector<int> ImageViewWidget::getPixelInfo(int x, int y) {
	QSize real_size = m_dicom_manager->getStandardSliceSize();

	int r_x = (int)(x * (float)real_size.width() / m_slice_view->size().width());
	int r_y = (int)(y * (float)real_size.height() / m_slice_view->size().height());

	return m_dicom_manager->getVoxelInfo(m_mode, r_x, r_y);
}

void ImageViewWidget::setIndex(int idx) {
	this->m_idx_slice = idx;

	emit changeSliceIdx(m_mode, idx);
}


void ImageViewWidget::setLines() {
	std::vector<QLine> lines = this->m_dicom_manager->getAxesLines(m_mode, 512, 512);

	switch (this->m_mode) {
		case MODE_AXIAL:
			this->m_slice_view->setLines(lines[0], lines[1], QColor(Qt::blue), QColor(Qt::green));
			break;
		case MODE_CORONAL:
			this->m_slice_view->setLines(lines[0], lines[1], QColor(Qt::red), QColor(Qt::blue));
			break;
		case MODE_SAGITTAL:
			this->m_slice_view->setLines(lines[0], lines[1], QColor(Qt::red), QColor(Qt::green));
			break;
		default:
			return;
	}

}

void ImageViewWidget::increaseIndex() {
	m_slider->setValue(m_slider->value() + 1);
}
void ImageViewWidget::decreaseIndex() {
	m_slider->setValue(m_slider->value() - 1);
}