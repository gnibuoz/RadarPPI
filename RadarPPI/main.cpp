#include <QtWidgets/QApplication>
#include "VGRadarPPIViewer.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	VGRadarPPIViewer p;
	p.setWindowTitle(QString::fromLocal8Bit("雷达PPI显示效果"));
	p.show();

	return a.exec();
}