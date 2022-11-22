#pragma once

#include "QCustomPlot.h"

struct VGRadarPPIPrivate;

/**
 * 雷达PPI显示
 */
class VGRadarPPI : public QCustomPlot
{
	Q_OBJECT

public:
	using DataVector = QVector<qreal>;

	VGRadarPPI(QWidget *parent = Q_NULLPTR);

	~VGRadarPPI();

	/**
	 * 替换更新数据，直角坐标
	 */
	void setData(const DataVector& x, const DataVector& y);

	/**
	 * 追加更新数据，直角坐标
	*/
	void addData(const DataVector& x, const DataVector& y);

	/**
	* 设置颜色
	*/
	void setColor(const QColor& color);

	/**
	* 查询P显范围(m)
	*/
	qreal getRange() const;

	/**
	* 查询P显扫描角速度（°/s）
	*/
	qreal getSpeed() const;

public Q_SLOTS:
	/**
	 * 设置P显范围(m)
	 */
	void setRange(qreal range_m);

	/**
	* 设置P显当前扫描角度
	*/
	void setTheta(qreal theta);

	/**
	* 设置扫描角速度（°/s）
	*/
	void setSpeed(qreal speed_deg_s);

	/**
	* 开始扫描
	*/
	void start();

	/**
	* 暂停扫描
	*/
	void pause();

	/**
	* 停止扫描
	*/
	void stop();

protected:
	void paintEvent(QPaintEvent *event) override;

	void timerEvent(QTimerEvent *event) override;

private:
	VGRadarPPIPrivate *_{ Q_NULLPTR };
};