#include "marker.h"

marker::marker(int id, float x, float y, float z, QObject *parent) :
    QObject(parent)
{
    markerPos[0] = x;
    markerPos[1] = y;
    markerPos[2] = z;

    markerID = id;
}

float *marker::getMarkerPos()
{
    return markerPos;
}

int marker::getMarkerID()
{
    return markerID;
}
