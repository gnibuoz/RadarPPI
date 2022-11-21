#pragma once

#include "QCustomPlot.h"

struct VGRadarPPIData;

class VGRadarPPI : public QCustomPlot
{
	Q_OBJECT

public:
	using DataVector = QVector<qreal>;

	VGRadarPPI(QWidget *parent = Q_NULLPTR);
	~VGRadarPPI();

	/**
	 * �������ݣ�ֱ������
	 */
	void setData(const DataVector& x, const DataVector& y);

	/**
	* ������ɫ
	*/
	void setColor(const QColor& color);

	/**
	* ��ѯP�Է�Χ(m)
	*/
	qreal getRange() const;

	/**
	* ��ѯP��ɨ����ٶȣ���/s��
	*/
	qreal getSpeed() const;

public Q_SLOTS:
	/**
	 * ����P�Է�Χ(m)
	 */
	void setRange(qreal range_m);

	/**
	* ����ɨ����ٶȣ���/s��
	*/
	void setSpeed(qreal speed_deg_s);

	/**
	* ��ʼɨ��
	*/
	void start();

	/**
	* ��ͣɨ��
	*/
	void pause();

	/**
	* ֹͣɨ��
	*/
	void stop();

protected:
	void paintEvent(QPaintEvent *event) override;

	void timerEvent(QTimerEvent *event) override;

private:
	VGRadarPPIData *_{ Q_NULLPTR };
};