#include "pointcloud.h"
#include "settings.h"

#include <interpolation.h>
using namespace alglib;

#define CLOUDSIZE 1024

Pointcloud::Pointcloud() {

}

Pointcloud::Pointcloud(QList<QPoint> d) {
	data = d;
}

Pointcloud::Pointcloud(const char* n) {
	name = n;

	QList<QVariant> list = settings->value(name).toList();
	for (QList<QVariant>::iterator i = list.begin(); i != list.end(); i++) {
		data.push_back(i->toPoint());
	}
}

static bool sortX(QPoint& lhs, QPoint& rhs) {
	return lhs.x() < rhs.x() ? true : lhs.y() < rhs.y();
}
void Pointcloud::add(QPoint point) {
	data.push_back(point);
	std::sort(data.begin(), data.end(), sortX);
	while (data.size() > CLOUDSIZE) {
		// TODO: Change this to remove the least-useful point.
		// Currently removes a random entry, but never the first or last.
		data.removeAt((rand() % (data.size()-2)) + 1);
	}
	save();
}

void Pointcloud::save() {
	QList<QVariant> list;
	for (QList<QPoint>::iterator i = data.begin(); i != data.end(); i++) {
		list.push_back(*i);
	}
	settings->setValue(name, list);
}

int Pointcloud::get(int x) {
	for (QList<QPoint>::iterator i = data.begin(); i != data.end(); i++) {

	}
}
