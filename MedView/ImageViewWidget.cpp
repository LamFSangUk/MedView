#include "ImageViewWidget.h"
#include "DicomManager.h"

#include <QLayout>

#include <algorithm>
#include <climits>

#include <QDebug>

ImageViewWidget::ImageViewWidget(Mode mode, DicomManager *dicom_manager,QWidget* parent=NULL)
 : QWidget(parent)
{
	this->m_mode = mode;

	this->m_dicom_manager = dicom_manager;

	this->m_idx_slice = 0;
	this->m_idx_max = 0;

	// Set Layout
	this->buildLayout();

	connect(m_dicom_manager, SIGNAL(changeVolume()), this, SLOT(initView()));

	connect(m_slider, &QSlider::valueChanged, [this](int variation) {
		this->m_dicom_manager->updateSliceIndex(this->m_mode, variation);
	});
	connect(m_slice_view, &SliceWidget::requestIncIndex, [this] {
		this->m_dicom_manager->updateSliceIndex(this->m_mode, this->m_slider->value() + 1);
	});
	connect(m_slice_view, &SliceWidget::requestDecIndex, [this] {
		this->m_dicom_manager->updateSliceIndex(this->m_mode, this->m_slider->value() - 1);
	});
	connect(m_slice_view, &SliceWidget::changeDegree, [this](float degree) {
		this->m_dicom_manager->updateAngle(this->m_mode, degree);
	});

	connect(m_dicom_manager, &DicomManager::changeSlice, this, &ImageViewWidget::updateView);

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

	//connect(m_slider, SIGNAL(valueChanged(int)), this, SLOT(setIndex(int)));
	//connect(this, SIGNAL(changeSliceIdx(int, int)), m_dicom_manager, SLOT(setSliceIdx(int, int)));
	//connect(m_dicom_manager, SIGNAL(changeAxes()), this, SLOT(setLines()));

	//connect(m_slice_view, SIGNAL(requestIncIndex()), this, SLOT(increaseIndex()));
	//connect(m_slice_view, SIGNAL(requestDecIndex()), this, SLOT(decreaseIndex()));
	
	//connect(m_slice_view, SIGNAL(changeDegree(int,float)), m_dicom_manager, SLOT(setDegree(int, float)));

	
	
	m_slider->setEnabled(true);
	m_slider->setMinimum(0);
	/*switch (this->m_mode) {
		case Mode::MODE_AXIAL:
			m_slider->setMaximum(m_dicom_manager->max_axial_idx);
			m_slider->setValue(m_dicom_manager->axial_idx);
			break;
		case Mode::MODE_CORONAL:
			m_slider->setMaximum(m_dicom_manager->max_coronal_idx);
			m_slider->setValue(m_dicom_manager->coronal_idx);
			break;
		case Mode::MODE_SAGITTAL:
			m_slider->setMaximum(m_dicom_manager->max_sagittal_idx);
			m_slider->setValue(m_dicom_manager->sagittal_idx);
			break;
		default:
			throw std::runtime_error("Not assigned Mode");
			return;
	}*/
}

void ImageViewWidget::draw(Mode mode, QImage* img) {
	//qDebug() << mode << m_mode;
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

void ImageViewWidget::setLines() {
	std::vector<QLine> lines = this->m_dicom_manager->getAxesLines(m_mode, 512, 512);

	switch (this->m_mode) {
		case Mode::MODE_AXIAL:
			this->m_slice_view->setLines(lines[0], lines[1], QColor(Qt::blue), QColor(Qt::green));
			break;
		case Mode::MODE_CORONAL:
			this->m_slice_view->setLines(lines[0], lines[1], QColor(Qt::red), QColor(Qt::blue));
			break;
		case Mode::MODE_SAGITTAL:
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

void ImageViewWidget::updateView(std::map<Mode, DicomManager::SlicePacket> packets) {
	std::map<Mode, DicomManager::SlicePacket>::const_iterator pos = packets.find(m_mode);
	if (pos == packets.end()) {
		// Not found
	}
	else {
		qDebug() << "updated";
		DicomManager::SlicePacket data = pos->second;
		
		m_idx_slice = data.cur_idx;
		m_idx_max = data.max_idx;

		m_slider->blockSignals(true);
		m_slider->setMaximum(m_idx_max);
		m_slider->setValue(m_idx_slice);
		m_slider->blockSignals(false);

		// Resize the lines
		QSize show_size = m_slice_view->getSize();
		int show_width = show_size.width();
		int show_height = show_size.height();

		QPointF base_point = QPointF((float)data.line_center.x() * show_size.width() / data.size.width(),
									(float)data.line_center.y() * show_size.height() / data.size.height());

		float max_length = std::sqrt(show_width*show_width+show_height*show_height);

		QLineF line1 = QLineF(base_point, base_point + QPointF(1, 0));
		QLineF line2 = QLineF(base_point, base_point + QPointF(1, 0));

		line1.setLength(max_length);
		line1.setAngle(data.angle);

		line2.setLength(max_length);
		line2.setAngle(data.angle + 180);

		QLine lh = QLineF(line1.p2(), line2.p2()).toLine();

		data.angle += 90.0;
		line1 = QLineF(base_point, base_point + QPointF(1, 0));
		line2 = QLineF(base_point, base_point + QPointF(1, 0));

		line1.setLength(max_length);
		line1.setAngle(data.angle);

		line2.setLength(max_length);
		line2.setAngle(data.angle + 180);

		QLine lv = QLineF(line1.p2(), line2.p2()).toLine();

		switch (this->m_mode) {
		case Mode::MODE_AXIAL:
			this->m_slice_view->setLines(lh, lv, QColor(Qt::blue), QColor(Qt::green));
			break;
		case Mode::MODE_CORONAL:
			this->m_slice_view->setLines(lh, lv, QColor(Qt::red), QColor(Qt::green));
			break;
		case Mode::MODE_SAGITTAL:
			this->m_slice_view->setLines(lh, lv, QColor(Qt::blue), QColor(Qt::red));
			break;
		default:
			return;
		}

		draw(m_mode, &data.slice);
	}
}