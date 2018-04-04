#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <QList>
#include <QPoint>
#include <vector>
#include <algorithm>

#include "spline.h"

class Pointcloud {
private:
	const char* name;
	QList<QPoint> data;

public:
	Pointcloud();
	Pointcloud(QList<QPoint> data);
	Pointcloud(const char* name);

	void add(QPoint point);
	void save();
	int get(int x);
};

#endif // POINTCLOUD_H
