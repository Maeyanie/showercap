#include "pointcloud.h"
#include "settings.h"

//#include <interpolation.h>
//using namespace alglib;

#define CLOUDSIZE 64

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

static bool sortX(const QPoint& lhs, const QPoint& rhs) {
	return lhs.x() < rhs.x() ? true : lhs.y() < rhs.y();
}
void Pointcloud::add(QPoint point) {
    QList<QPoint>::iterator i = data.begin();
    while (true) {
        if (i == data.end()) {
            data.append(point);
            break;
        }
        if (sortX(*i, point)) {
            data.insert(i, point);
        }
        i++;
    }

    /*data.push_back(point);

    // TODO: Not the most efficient sort ever, but it'll do for now.
    // For some reason, despite it being the official recommendation,
    // using std::sort crashes.
    for (int i = 0; i < data.size()-1; i++) {
        if (!sortX(data[i], data[i+1])) {
            data.swap(i, i+1);
            i = -1;
        }
    }*/

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
    int total;
    int count;

	for (QList<QPoint>::iterator i = data.begin(); i != data.end(); i++) {
        if (i->x() == x) {
            total += i->y();
            count++;
        }
	}

    return (count > 0) ? (total / count) : 0;
}
