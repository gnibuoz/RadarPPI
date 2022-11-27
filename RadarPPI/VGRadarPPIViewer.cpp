#include "VGRadarPPIViewer.h"
#include "ui_VGRadarPPIViewer.h"

#include "QTime"
#include "QTimer"
#include "QFileDialog"

#include "QFile"
#include "QTextStream"

namespace RadarPPI {

	FrameDataVector loadPPIData(const QString& path)
	{
		FrameDataVector frames;

		QFile file(path);
		if (!file.open(QIODevice::ReadOnly | QFile::Text)) return frames;

		QTextStream s(&file);
		while (!s.atEnd())
		{
			auto values = s.readLine().simplified().trimmed().split(" ", QString::SkipEmptyParts);
			if (values.empty()) continue;

			Targets targets;
			auto n = values.size() / 2;
			for (auto i = 0; i < n; ++i)
			{
				auto r = values[i * 2].toDouble();
				auto t = values[i * 2 + 1].toDouble() * 3.1415926 / 180.0;

				if (r >= 0.0)
				{
					targets.push_back(Target{ r * cos(t), r * sin(t) });
				}
			}

			frames.push_back(targets);
		}

		file.close();
		return frames;
	}
};


VGRadarPPIViewer::VGRadarPPIViewer(QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::VGRadarPPIViewer();
	ui->setupUi(this);

	ui->widget->setColor(QColor("#00ffb2"));

	qsrand(QTime::currentTime().msecsSinceStartOfDay());
	auto timer = new QTimer(this);
	connect(timer, &QTimer::timeout, [&, this]() {
		if (_data.empty())
		{
			static int counter = 0;

			//if (counter++ % 2 == 0)
			//{
			//	static int idx = 0;
			static QVector<QColor> clrs{ QColor("#ff6666") ,QColor("#2aff36"), QColor("#ddff21"),QColor("#ffa621"),QColor("#0061ff"),QColor("#00ffb2") };
			//	ui->widget->setColor(clrs[idx++ % clrs.size()]);			// ²âÊÔÑÕÉ«±ä»¯
			//}

			const auto range = ui->doubleSpinBoxRange->value() * 1000.0;

			QVector<double> x, y;
			for (int i = 0; i < qrand() % 30; i += 1)
			{
				x.push_back(((qrand() % 2000 - 1000) / 1000.0) * range * 0.7);
				y.push_back(((qrand() % 2000 - 1000) / 1000.0) * range * 0.7);
			}
			ui->widget->addData(x, y);
		}
		else
		{
			_idx++;
			_idx %= _data.size();

			auto dd = _data[_idx];

			QVector<qreal> x, y;
			for (auto& d : dd)
			{
				x.push_back(d.x());
				y.push_back(d.y());
			}
			ui->widget->addData(x, y);
		}
	});
	timer->start(1000);

	connect(ui->pushButton, &QPushButton::clicked, [=]() {
		auto path = QFileDialog::getOpenFileName(this, "PPI Data", " ");

		if (path.isEmpty()) return;

		_data = RadarPPI::loadPPIData(path);
		_idx = 0u;

		if (!_data.empty())
		{
			ui->widget->setData({}, {});
		}
	});

	auto handler = [=]() {
		ui->widget->setRange(ui->doubleSpinBoxRange->value() * 1000.0);
		ui->widget->setSpeed(360.0 / ui->doubleSpinBoxT->value());
	};

	connect(ui->doubleSpinBoxRange, QOverload<double>::of(&QDoubleSpinBox::valueChanged), handler);
	connect(ui->doubleSpinBoxT, QOverload<double>::of(&QDoubleSpinBox::valueChanged), handler);

	connect(ui->doubleSpinBoxTheta, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [=]() {
		ui->widget->stop();
		ui->widget->setData({}, {});
		ui->widget->setTheta(-ui->doubleSpinBoxTheta->value() + 90.0);
		ui->widget->start();
	});

	QTimer::singleShot(1000, handler);
}

VGRadarPPIViewer::~VGRadarPPIViewer()
{
	delete ui;
}