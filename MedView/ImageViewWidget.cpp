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
	this->_buildLayout();

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
	connect(m_slice_view, &SliceWidget::changeWindowing, [this](int delta_width, int delta_level) {
		this->m_dicom_manager->updateWindowing(delta_width, delta_level);
	});
	connect(m_slice_view, &SliceWidget::changeAxesCenter, [this](int x, int y) {
		QSize view_size = this->m_slice_view->getSize();
		this->m_dicom_manager->updateAxesCenter(this->m_mode, x, y, view_size.width(), view_size.height());
	});
	connect(m_slice_view, &SliceWidget::changeZoom, [this](float delta) {
		this->m_dicom_manager->updateZoom(delta);
	});

	connect(m_dicom_manager, &DicomManager::changeSlice, this, &ImageViewWidget::updateView);

}

void ImageViewWidget::_buildLayout() {
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);

	// SliceWidget size
	QSize min_size_slice(512, 512);

	m_slice_view = new SliceWidget(this);
	m_slice_view->setMinimumSize(min_size_slice);
	m_slice_view->showMaximized();
	m_slice_view->setMargin(0);

	m_slider = new QSlider(Qt::Horizontal, this);
	m_slider->setMaximumHeight(10);
	m_slider->setEnabled(false);
	m_slider->setObjectName("slider");
	m_slider->showMaximized();

	layout->addWidget(m_slice_view, 0);
	layout->addWidget(m_slider, 0);

	this->setLayout(layout);
}

void ImageViewWidget::initView() {
	m_slider->setEnabled(true);
	m_slider->setMinimum(0);
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
		DicomManager::SlicePacket data = pos->second;
		
		m_idx_slice = data.cur_idx;
		m_idx_max = data.max_idx;

		// Update slider value
		m_slider->blockSignals(true);
		m_slider->setMaximum(m_idx_max);
		m_slider->setValue(m_idx_slice);
		m_slider->blockSignals(false);

		// Update Windowing Value
		m_slice_view->updateWindowingValue(data.windowing_level, data.windowing_width);

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