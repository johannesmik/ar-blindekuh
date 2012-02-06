#ifndef MARKER_H
#define MARKER_H

#include <QObject>

class marker : public QObject
{
    Q_OBJECT
public:
    explicit marker(int id, float x, float y, float z, QObject *parent = 0);
    float* getMarkerPos();
    int getMarkerID();
signals:

public slots:
private:
    float markerPos[3];
    int markerID;
};

#endif // MARKER_H
