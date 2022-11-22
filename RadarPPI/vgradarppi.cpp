#include "vgradarppi.h"
#include "QPainter"
#include "QEvent"

struct VGRadarPPIPrivate
{
	VGRadarPPIPrivate()
	{
		dot = QPixmap(":/Resource/dot.png").scaledToWidth(12, Qt::SmoothTransformation);
	}

	inline QPixmap convertToColor(const QPixmap& background, const QColor& color)
	{
		auto b = background.toImage();
		auto ba = b.alphaChannel();
		b.fill(color);
		b.setAlphaChannel(ba);
		return QPixmap::fromImage(b);
	}

	inline void setColor(const QColor& color)
	{
		background = convertToColor(background, color);
		scan = convertToColor(scan, color);
		dot = convertToColor(dot, color);
	}

	QCPGraph* graph{ nullptr };
	qreal speed{ 6.0 };				// 扫描角速度，默认120°/s
	qreal range_m{ 10000.0 };		// 范围

	QPixmap background{ ":/Resource/background.png" };
	QPixmap scan{ ":/Resource/scan.png" };
	QPixmap dot;

	qreal angle{ 90.0 };
	QElapsedTimer timer;

	qint32 timerID{ -1 };
};

VGRadarPPI::VGRadarPPI(QWidget *parent)
	: QCustomPlot(parent)
	, _(new VGRadarPPIPrivate)
{
	resize(600, 600);

	setBackground(_->background);                  //设置背景为透明色

	setBackgroundScaledMode(Qt::IgnoreAspectRatio);

	setBackground(QColor(0, 0, 0, 255));									  //设置默认背景为黑色

	_->graph = addGraph();
	_->graph->setLineStyle(QCPGraph::lsNone);

	QCPScatterStyle style(QCPScatterStyle::ssPixmap, 1.0);
	style.setPixmap(QPixmap(":/Resource/dot.png").scaledToWidth(8, Qt::SmoothTransformation));
	_->graph->setScatterStyle(style);

	xAxis2->setVisible(false);  // 顶部坐标轴
	xAxis->setVisible(false);   // 底部坐标轴
	yAxis->setVisible(false);   // 左边坐标轴
	yAxis2->setVisible(false);  // 右边坐标轴

	setRange(10e3);		// 10km

	start();
}

VGRadarPPI::~VGRadarPPI()
{
	delete _;
	_ = Q_NULLPTR;
}

void VGRadarPPI::setData(const DataVector& x, const DataVector& y)
{
	Q_ASSERT(x.size() == y.size());

	DataVector xx, yy;
	for (auto i = 0; i < x.size(); ++i)
	{
		auto r = sqrt(x[i] * x[i] + y[i] * y[i]);
		if (r > _->range_m * 0.8)
			continue;

		xx.push_back(x[i]);
		yy.push_back(y[i]);
	}

	_->graph->setData(xx, yy);
	replot(QCustomPlot::rpQueuedReplot);
}

void VGRadarPPI::addData(const DataVector& x, const DataVector& y)
{
	Q_ASSERT(x.size() == y.size());

	DataVector xx, yy;
	for (auto i = 0; i < x.size(); ++i)
	{
		auto r = sqrt(x[i] * x[i] + y[i] * y[i]);
		if (r > _->range_m * 0.8)
			continue;

		xx.push_back(x[i]);
		yy.push_back(y[i]);
	}
	_->graph->addData(xx, yy);
	replot(QCustomPlot::rpQueuedReplot);
}

void VGRadarPPI::setRange(qreal range_m)
{
	_->range_m = range_m;
	xAxis->setRange(-range_m, range_m);
	yAxis->setRange(-range_m, range_m);
	replot(QCustomPlot::rpQueuedReplot);
}

void VGRadarPPI::setTheta(qreal theta)
{
	_->angle = theta;
}

void VGRadarPPI::setSpeed(qreal speed_deg_s)
{
	_->speed = speed_deg_s;
}

void VGRadarPPI::start()
{
	if (_->timerID == -1)
	{
		_->timerID = startTimer(20);
		_->timer.restart();
	}
}

void VGRadarPPI::pause()
{
	if (_->timerID == -1)return;

	killTimer(_->timerID);
	_->timerID = -1;
}

void VGRadarPPI::stop()
{
	if (_->timerID == -1)return;

	killTimer(_->timerID);
	_->timerID = -1;

	_->angle = -90.0;
}

void VGRadarPPI::setColor(const QColor& color)
{
	_->setColor(color);

	setBackground(_->background);                  //设置背景为透明色

	QCPScatterStyle style(QCPScatterStyle::ssPixmap, 1.0);
	style.setPixmap(_->dot);
	_->graph->setScatterStyle(style);

	repaint();
	replot();
}

qreal VGRadarPPI::getRange() const
{
	return _->range_m;
}

qreal VGRadarPPI::getSpeed() const
{
	return _->speed;
}

void VGRadarPPI::paintEvent(QPaintEvent *event)
{
	QCustomPlot::paintEvent(event);

	QPainter p(this);
	p.translate(width() / 2, height() / 2);
	p.rotate(_->angle);
	p.drawPixmap(-width() / 2, -height() / 2, width(), height(), _->scan);

	_->angle -= _->speed * _->timer.elapsed() / 1000.0;

	_->timer.restart();
}

void VGRadarPPI::timerEvent(QTimerEvent *event)
{
	repaint();
}