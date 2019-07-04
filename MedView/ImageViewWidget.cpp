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
	connect(this, SIGNAL(changeSliceIdx(int, int, int, int)), m_dicom_manager, SLOT(setSliceIdx(int, int, int, int)));
	connect(m_dicom_manager, SIGNAL(changeSlice(int, QImage*)), this, SLOT(draw(int, QImage*)));

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
	if(mode == m_mode)
		m_slice_view->drawSlice(img);
}

void ImageViewWidget::setSlider(QSlider* slider) {
	this->m_slider = slider;
}

void ImageViewWidget::setIndex(int idx) {
	this->m_idx_slice = idx;

	//TODO::dynamic slice size
	emit changeSliceIdx(m_mode, idx, 512, 512);

	//this->getSlice();
	//m_dicom_manager->getSlice(m_mode);
}
