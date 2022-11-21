#include <QtWidgets/QApplication>
#include "vgradarppi.h"
#include "QTimer"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	VGRadarPPI p;

	p.setWindowTitle(QString::fromLocal8Bit("雷达PPI显示效果"));


	p.show();

	// 示例
#if 1
	qsrand(QTime::currentTime().msecsSinceStartOfDay());
	QTimer timer;
	QObject::connect(&timer, &QTimer::timeout, [&]() {

		static int counter = 0;

		if ( counter ++ % 2 == 0 )
		{
			static int idx = 0;
			static QVector<QColor> clrs{ QColor("#ff6666") ,QColor("#2aff36"), QColor("#ddff21"),QColor("#ffa621"),QColor("#0061ff"),QColor("#00ffb2")};
			p.setColor(clrs[idx++ % clrs.size()]);			// 测试颜色变化
		}

		QVector<double> x, y;
		for (int i = 0; i < qrand() % 30; i += 1)
		{
			x.push_back((qrand() % 20000 - 10000) * 0.7);
			y.push_back((qrand() % 20000 - 10000) * 0.7);
		}
		p.setData(x, y);
	});
	timer.start(1000);
#endif

	return a.exec();
}