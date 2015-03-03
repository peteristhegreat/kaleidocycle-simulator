#ifndef TRIANGLEMEASURE_H
#define TRIANGLEMEASURE_H

#include <QVector3D>
#include <qmath.h>
#include <QList>
#include <QVector>

#define PI 3.141592653589793

typedef QVector3D Vertex;

class TriangleMeasure
{
public:
    TriangleMeasure();
    void setVertices(const Vertex a, const Vertex b, const Vertex c);
    qreal angleA();
    qreal angleB();
    qreal angleC();
    qreal lengthAB();
    qreal lengthAC();
    qreal lengthBC();
    qreal length(const Vertex v1, const Vertex v2);
    qreal angle(const Vertex v, const Vertex v1, const Vertex v2);
    void useRadians();
    void useDegrees();
private:
    bool inRadians;
    Vertex a;
    Vertex b;
    Vertex c;
};

#endif // TRIANGLEMEASURE_H
