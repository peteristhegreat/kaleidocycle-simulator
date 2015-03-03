#include "trianglemeasure.h"

TriangleMeasure::TriangleMeasure()
{
    inRadians = false;
}

void TriangleMeasure::setVertices(Vertex a, Vertex b, Vertex c)
{
    this->a = a;
    this->b = b;
    this->c = c;
}

qreal TriangleMeasure::angleA()
{
    return angle(a, b, c);
}

qreal TriangleMeasure::angleB()
{
    return angle(b, a, c);
}

qreal TriangleMeasure::angleC()
{
    return angle(c, a, b);
}

qreal TriangleMeasure::lengthAB()
{
    return length(a, b);
}

qreal TriangleMeasure::lengthAC()
{
    return length(a, c);
}

qreal TriangleMeasure::lengthBC()
{
    return length(b, c);
}

qreal TriangleMeasure::length(const Vertex v1, const Vertex v2)
{
    QVector3D v = v1 - v2;
    return v.length();
}

qreal TriangleMeasure::angle(const Vertex v, const Vertex v1, const Vertex v2)
{
    // calculate U and V
    QVector3D U,V;
    U = v - v1;
    V = v - v2;

    double lhs = QVector3D::dotProduct(U,V) / (U.length() * V.length());
    double min_theta = acos(lhs);
//    qDebug() << min_theta * 180./PI;

    // keep angles under 180 degrees
    if(min_theta > PI)
    {
        min_theta -= PI;
    }


    if(inRadians)
    {
        // keep it
    }
    else
    {
        min_theta *= 180./PI;
    }
    return min_theta;
}

void TriangleMeasure::useDegrees()
{
    inRadians = false;
}

void TriangleMeasure::useRadians()
{
    inRadians = true;
}
