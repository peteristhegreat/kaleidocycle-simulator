#ifndef ROTATEPOINT_H
#define ROTATEPOINT_H

#include <QVector3D>
#include <qmath.h>
#include <QList>
#include <QVector>

#define PI 3.141592653589793

typedef QVector3D Vertex;

class RotatePoint
{
public:
    RotatePoint();
    void setAxis(Vertex p1, Vertex p2);
//    QVector <Vertex> rotate(qreal angle, QVector <Vertex> );
//    QList <Vertex> rotate(qreal angle, QList <Vertex> );
    Vertex rotate(const qreal angle, const Vertex);
    void useRadians();
    void useDegrees();
private:
    qreal a, b, c, u, v, w, theta;
    qreal L;
    bool inRadians;
};

#endif // ROTATEPOINT_H
