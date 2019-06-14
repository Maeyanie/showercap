#ifndef POINTCLOUD_H
#define POINTCLOUD_H

#include <QList>
#include <QPoint>
#include <vector>
#include <list>
#include <algorithm>

#include "spline.h"

class Pointcloud {
private:
	const char* name;
    std::list<QPoint> data;

public:
	Pointcloud();
    Pointcloud(std::list<QPoint> data);
	Pointcloud(const char* name);

	void add(QPoint point);
	void save();
	int get(int x);
};

#endif // POINTCLOUD_H
