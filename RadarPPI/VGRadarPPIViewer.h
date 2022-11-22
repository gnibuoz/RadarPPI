#pragma once

#include <QWidget>
namespace Ui { class VGRadarPPIViewer; };

#include "QPointF"
#include "QVector"

using Target = QPointF;						// 目标点
using Targets = QVector<Target>;			// 目标列表
using FrameDataVector = QVector<Targets>;	// 所有时刻数据

class VGRadarPPIViewer : public QWidget
{
	Q_OBJECT
public:
	VGRadarPPIViewer(QWidget *parent = Q_NULLPTR);
	~VGRadarPPIViewer();

private:
	Ui::VGRadarPPIViewer *ui;

	FrameDataVector _data;
	unsigned _idx{ 0u };
};