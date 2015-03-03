/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QtOpenGL>
#include <QDebug>

#include <math.h>
#include <QPointF>

#include "glwidget.h"
#include "rotatepoint.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
//    qDebug() << Q_FUNC_INFO;
//    gear1 = 0;
//    gear2 = 0;
//    gear3 = 0;
    xRot = 0;
    yRot = 0;
    zRot = 0;
    gear1Rot = 0;
    zoomFactor = -16;

    n = 8;
    evenEdgeAngle = 180;
    oddEdgeAngle = 90;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(advanceGears()));
    timer->start(20);

    faces << Face(0,1,2)
            << Face(0,1,3)
            << Face(0,2,3)
            << Face(1,2,3);
    init_colorArray();
    calculateVerticies();
}

GLWidget::~GLWidget()
{
//    makeCurrent();
//    glDeleteLists(gear1, 1);
//    glDeleteLists(gear2, 1);
//    glDeleteLists(gear3, 1);
}

void GLWidget::closeGapUsingOddAngle()
{
    // this modifies the odd angle to close the gap
    closeGap(false);
    updateGL();
//    qDebug() << "New odd angle:"<< oddEdgeAngle;
    emit oddAngleChanged(oddEdgeAngle);
}

void GLWidget::closeGapUsingEvenAngle()
{
    // This modifies the even angle to close the gap
    closeGap(true);
    updateGL();
//    qDebug() << "New even angle:"<< evenEdgeAngle;
    emit evenAngleChanged(evenEdgeAngle);
}

qreal GLWidget::calculatedGap()
{
    if(false)
    {
        // Find the smallest distance between the verticies of the first and last pairs
        return qMin(qMin(t.length(v.first(), v.last()),t.length(v.first(), v.at(v.size()-2))),
                    qMin(t.length(v.at(1), v.last()),t.length(v.at(1), v.at(v.size()-2))));
    }
    else
    {
        return t.length(v.first() + (v.at(1) - v.first()), v.last() + (v.at(v.size()-2) - v.last()));
    }
}

void GLWidget::closeGap(bool usingEven)
{
    if(n < 6)
        return;
    TriangleMeasure t;
    qreal gap = calculatedGap();
//    qDebug() << "Gap before close" << gap;

    QVector <QPointF> anglesToGaps;


    if((usingEven? evenEdgeAngle:oddEdgeAngle) == 0)
    {
        (usingEven? evenEdgeAngle:oddEdgeAngle) = 10;
    }

    calculateVerticies();

    anglesToGaps.append(QPointF((usingEven? evenEdgeAngle:oddEdgeAngle),
                                calculatedGap()));


    int count = 1;

    if(true)
    {

        qreal step = 10;

        do
        {
            do
            {
                (usingEven? evenEdgeAngle:oddEdgeAngle) -= step;
                calculateVerticies();
                count++;
                anglesToGaps.append(QPointF((usingEven? evenEdgeAngle : oddEdgeAngle),
                                            calculatedGap()));

            } while(anglesToGaps.last().y() < anglesToGaps.at(anglesToGaps.size() - 2).y());

            anglesToGaps.pop_back();
            (usingEven? evenEdgeAngle:oddEdgeAngle) = anglesToGaps.last().x();
            step /= -10.;

        } while(qAbs(step) > .001);
//        qDebug() << "Best Angle" << anglesToGaps.last() << v.count();
    }
    else
    {
        // Find all the gaps stepping 1/10th of degree from zero to 180

//        qreal best_angle = anglesToGaps.last().x();
//        qreal best_gap = anglesToGaps.last().y();
//        for(qreal i = 0.1; i < 180; i+=0.1)
//        {
//            (usingEven? evenEdgeAngle:oddEdgeAngle) = i;
//            qDebug() << (usingEven? evenEdgeAngle:oddEdgeAngle);
//            calculateVerticies();
//            count++;
//            updateGL();
//            anglesToGaps.append(QPointF((usingEven? evenEdgeAngle : oddEdgeAngle),
//                                        calculatedGap()));
//            if(best_gap > anglesToGaps.last().y())
//            {
//                best_gap = anglesToGaps.last().y();
//                best_angle = anglesToGaps.last().x();
//            }
//        }

//        (usingEven? evenEdgeAngle:oddEdgeAngle) = best_angle;
//        calculateVerticies();
//        updateGL();
//        qDebug() << "Best Angle" << best_angle;

    }

}

void GLWidget::setN(int n)
{
    this->n = n;
    calculateVerticies();
    updateGL();
}

void GLWidget::setOddAngle(double angle)
{
    oddEdgeAngle = angle;

    if(autoCloseGap)
    {
        closeGapUsingEvenAngle();
    }
    else
    {
        calculateVerticies();
        updateGL();
    }
}

void GLWidget::setEvenAngle(double angle)
{
    evenEdgeAngle = angle;
    if(autoCloseGap)
    {
        closeGapUsingOddAngle();
    }
    else
    {
        calculateVerticies();
        updateGL();
    }
}

void GLWidget::setXRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    normalizeAngle(&angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::initializeGL()
{
//    qDebug() << Q_FUNC_INFO;
    static const GLfloat lightPos[4] = { 5.0f, 5.0f, 10.0f, 1.0f };
    static const GLfloat reflectance1[4] = { 0.8f, 0.1f, 0.0f, 1.0f };
    static const GLfloat reflectance2[4] = { 0.0f, 0.8f, 0.2f, 1.0f };
    static const GLfloat reflectance3[4] = { 0.2f, 0.2f, 1.0f, 1.0f };

//    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    glEnable(GL_DEPTH_TEST);

////    gear1 = makeGear(reflectance1, 1.0, 4.0, 1.0, 0.7, 20);
////    gear2 = makeGear(reflectance2, 0.5, 2.0, 2.0, 0.7, 10);
////    gear3 = makeGear(reflectance3, 1.3, 2.0, 0.5, 0.7, 10);

//    glEnable(GL_NORMALIZE);
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glShadeModel(GL_SMOOTH); // Enable smooth shading
    qglClearColor(Qt::black); // Set the clear color to a black background

    glClearDepth(1.0f); // Depth buffer setup
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    glDepthFunc(GL_LEQUAL); // Set type of depth test

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really nice perspective calculations

}

// calculate vertex array
void GLWidget::calculateVerticies()
{
//    qDebug() << Q_FUNC_INFO;
    v.clear();


    // Make 4 initial 4 Verticies
    // TODO make these editable from the GUI
    v.append(Vertex(0,0,0));//A
    v.append(Vertex(0,0,1));//B
    v.append(Vertex(1, -0.5, 0.5));//C
    v.append(Vertex(1,  0.5, 0.5));//D

    // calculate the minimum angle that works for this tetrahedron



    RotatePoint r;
    r.useDegrees();


    qreal angle;
//    int n = 8;// how many tetrahedons to make
    // n is now a member variable defaulted to 8
    for(int i = 0; i < n - 1; i++)
    {
        // use the last two verticies added as the pivot axis
        r.setAxis(v.at(v.size()-2), v.at(v.size()-1));

        switch(i%4)
        {
        case 0:
            angle = evenEdgeAngle;
            break;
        case 1:
            angle = oddEdgeAngle;
            break;
        case 2:
            angle = -evenEdgeAngle;
            break;
        case 4:
            angle = -oddEdgeAngle;
            break;
        default:
            break;
        }

        // Perform the rotation
        Vertex v1 = r.rotate(angle,v.at(v.size() - 4));
        Vertex v2 = r.rotate(angle,v.at(v.size() - 3));

        // Add onto the end of the array of verticies
        v.append(v1);
        v.append(v2);

    }
}

void GLWidget::paintGL()
{
//    qDebug() << Q_FUNC_INFO;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslated(0, 0, zoomFactor);
    glPushMatrix();
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

    drawTetrahedra();
//    drawGear(gear1, -3.0, -2.0, 0.0, gear1Rot / 16.0);
//    drawGear(gear2, +3.1, -2.0, 0.0, -2.0 * (gear1Rot / 16.0) - 9.0);

//    glRotated(+90.0, 1.0, 0.0, 0.0);
//    drawGear(gear3, -3.1, -1.8, -2.2, +2.0 * (gear1Rot / 16.0) - 2.0);



    glPopMatrix();
}

void GLWidget::drawFace(const Face f, int offset)
{
    nextColor();
    drawVertex(v.at(f.v1 + offset));
    drawVertex(v.at(f.v2 + offset));
    drawVertex(v.at(f.v3 + offset));
//    qDebug() << f.v1 + offset << f.v2 + offset << f.v3 + offset;
}

void GLWidget::drawVertex(const Vertex v)
{
    glVertex3f(v.x(), v.y(), v.z());
}

// Draw a pyramid using OpenGL
void GLWidget::drawTetrahedra()
{
//    qDebug() << Q_FUNC_INFO;
    resetColorIndex();


    for(int i = 0; i <= v.size() - 4; i+=2)
    {
//        qDebug() << "i =" << i;
        glBegin(GL_TRIANGLES);
        drawFace(faces.at(0), i);
        drawFace(faces.at(1), i);
        drawFace(faces.at(2), i);
        drawFace(faces.at(3), i);
        glEnd();
    }

}

void GLWidget::nextColor()
{
    QColor c = colors.at(color_index%colors.size());
    glColor3f(c.redF(), c.greenF(), c.blueF());
    color_index++;
}

void GLWidget::resetColorIndex()
{
    color_index = 0;
}

void GLWidget::init_colorArray()
{
//    qDebug() << Q_FUNC_INFO;
    colors
//            << Qt::lightGray
            << Qt::red
            << Qt::green
            << Qt::blue
            << Qt::cyan
            << Qt::magenta
            << Qt::yellow
//            << Qt::gray
            << Qt::darkRed
            << Qt::darkGreen
            << Qt::darkBlue
            << Qt::darkCyan
            << Qt::darkMagenta
            << Qt::darkYellow
//            << QColor()
               ;
//            << Qt::darkGray;
}

void GLWidget::resizeGL(int width, int height)
{
//    qDebug() << Q_FUNC_INFO;
    if(false)
    {
        // make the viewport a fitted square
        int side = qMin(width, height);
        glViewport((width - side) / 2, (height - side) / 2, side, side);
    }
    else
    {
        glViewport(0,0,width,height);//(width - side) / 2, (height - side) / 2, side, side);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // left, right, top bottom near far
    glFrustum(-1.0*(qreal)width/(qreal)height, +1.0*(qreal)width/(qreal)height, -1.0, 1.0, 5.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -40.0);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}

void GLWidget::advanceGears()
{
//    gear1Rot += 2 * 16;
//    updateGL();
}

//GLuint GLWidget::makeGear(const GLfloat *reflectance, GLdouble innerRadius,
//                          GLdouble outerRadius, GLdouble thickness,
//                          GLdouble toothSize, GLint toothCount)
//{
//    const double Pi = 3.14159265358979323846;

//    GLuint list = glGenLists(1);
//    glNewList(list, GL_COMPILE);
//    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, reflectance);

//    GLdouble r0 = innerRadius;
//    GLdouble r1 = outerRadius - toothSize / 2.0;
//    GLdouble r2 = outerRadius + toothSize / 2.0;
//    GLdouble delta = (2.0 * Pi / toothCount) / 4.0;
//    GLdouble z = thickness / 2.0;
//    int i, j;

//    glShadeModel(GL_FLAT);

//    for (i = 0; i < 2; ++i) {
//        GLdouble sign = (i == 0) ? +1.0 : -1.0;

//        glNormal3d(0.0, 0.0, sign);

//        glBegin(GL_QUAD_STRIP);
//        for (j = 0; j <= toothCount; ++j) {
//            GLdouble angle = 2.0 * Pi * j / toothCount;
//	    glVertex3d(r0 * cos(angle), r0 * sin(angle), sign * z);
//	    glVertex3d(r1 * cos(angle), r1 * sin(angle), sign * z);
//	    glVertex3d(r0 * cos(angle), r0 * sin(angle), sign * z);
//	    glVertex3d(r1 * cos(angle + 3 * delta), r1 * sin(angle + 3 * delta),
//                       sign * z);
//        }
//        glEnd();

//        glBegin(GL_QUADS);
//        for (j = 0; j < toothCount; ++j) {
//            GLdouble angle = 2.0 * Pi * j / toothCount;
//	    glVertex3d(r1 * cos(angle), r1 * sin(angle), sign * z);
//	    glVertex3d(r2 * cos(angle + delta), r2 * sin(angle + delta),
//                       sign * z);
//	    glVertex3d(r2 * cos(angle + 2 * delta), r2 * sin(angle + 2 * delta),
//                       sign * z);
//	    glVertex3d(r1 * cos(angle + 3 * delta), r1 * sin(angle + 3 * delta),
//                       sign * z);
//        }
//        glEnd();
//    }

//    glBegin(GL_QUAD_STRIP);
//    for (i = 0; i < toothCount; ++i) {
//        for (j = 0; j < 2; ++j) {
//            GLdouble angle = 2.0 * Pi * (i + (j / 2.0)) / toothCount;
//            GLdouble s1 = r1;
//            GLdouble s2 = r2;
//            if (j == 1)
//                qSwap(s1, s2);

//	    glNormal3d(cos(angle), sin(angle), 0.0);
//	    glVertex3d(s1 * cos(angle), s1 * sin(angle), +z);
//	    glVertex3d(s1 * cos(angle), s1 * sin(angle), -z);

//	    glNormal3d(s2 * sin(angle + delta) - s1 * sin(angle),
//                       s1 * cos(angle) - s2 * cos(angle + delta), 0.0);
//	    glVertex3d(s2 * cos(angle + delta), s2 * sin(angle + delta), +z);
//	    glVertex3d(s2 * cos(angle + delta), s2 * sin(angle + delta), -z);
//        }
//    }
//    glVertex3d(r1, 0.0, +z);
//    glVertex3d(r1, 0.0, -z);
//    glEnd();

//    glShadeModel(GL_SMOOTH);

//    glBegin(GL_QUAD_STRIP);
//    for (i = 0; i <= toothCount; ++i) {
//	GLdouble angle = i * 2.0 * Pi / toothCount;
//	glNormal3d(-cos(angle), -sin(angle), 0.0);
//	glVertex3d(r0 * cos(angle), r0 * sin(angle), +z);
//	glVertex3d(r0 * cos(angle), r0 * sin(angle), -z);
//    }
//    glEnd();

//    glEndList();

//    return list;
//}

//void GLWidget::drawGear(GLuint gear, GLdouble dx, GLdouble dy, GLdouble dz,
//                        GLdouble angle)
//{
////    glPushMatrix();
////    glTranslated(dx, dy, dz);
////    glRotated(angle, 0.0, 0.0, 1.0);
////    glCallList(gear);
////    glPopMatrix();
//}

void GLWidget::normalizeAngle(int *angle)
{
    while (*angle < 0)
        *angle += 360 * 16;
    while (*angle > 360 * 16)
        *angle -= 360 * 16;
}

void GLWidget::wheelEvent(QWheelEvent *e)
{
//    e->delta();
    zoomFactor += e->delta()/15/8;

//    qDebug() << "ZoomFactor" << zoomFactor;
    updateGL();
}

void GLWidget::setAutoCloseGap(bool in)
{
    autoCloseGap = in;
}
