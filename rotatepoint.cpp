#include "rotatepoint.h"

RotatePoint::RotatePoint()
{

}

void RotatePoint::setAxis(const Vertex p1, const Vertex p2)
{
    a = p1.x();
    b = p1.y();
    c = p1.z();
    // uvw are not normalized in this setup
    u = p2.x() - p1.x();
    v = p2.y() - p1.y();
    w = p2.z() - p1.z();
    L = u*u + v*v + w*w;
}

//    QVector <Vertex> rotate(qreal angle, QVector <Vertex> );
//    QList <Vertex> rotate(qreal angle, QList <Vertex> );
Vertex RotatePoint::rotate(const qreal angle, const Vertex pt)
{
    theta = angle;
    if(!inRadians)
    {
        theta *= PI/180.;
    }
    qreal cosTheta = cos(theta);
    qreal sinTheta = sin(theta);
    qreal sqrtL = sqrt(L);

    qreal x = pt.x();
    qreal y = pt.y();
    qreal z = pt.z();

    // calculations generated from: http://inside.mines.edu/~gmurray/ArbitraryAxisRotation/

    Vertex out;
    out.setX(
                ((a*(v*v + w*w) - u * (b*v + c*w - u*x - v*y - w*z))*(1-cosTheta) + L*x*cosTheta
                + sqrtL*(-c*v + b*w - w*y + v*z)*sinTheta)/L

            );

    out.setY(
                ((b*(u*u + w*w) - v * (a*u + c*w - u*x - v*y - w*z))*(1-cosTheta) + L*y*cosTheta
                + sqrtL*(c*u - a*w + w*x - u*z)*sinTheta)/L

            );

    out.setZ(
                ((c*(u*u + v*v) - w * (a*u + b*v - u*x - v*y - w*z))*(1-cosTheta) + L*z*cosTheta
                + sqrtL*(-b*u + a*v - v*x + u*y)*sinTheta)/L

            );
    return out;
}

void RotatePoint::useRadians()
{
    inRadians = true;
}

void RotatePoint::useDegrees()
{
    inRadians = false;
}
