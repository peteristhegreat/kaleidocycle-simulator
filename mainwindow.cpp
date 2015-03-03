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

//#include <QtGui>
#include <QtOpenGL>
//class QAction;
//class QLabel;
//class QMenu;
//class QScrollArea;
//class QSlider;
//class QSpinBox;
#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QScrollArea>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QSizePolicy>
#include <QButtonGroup>

#include "glwidget.h"
#include "mainwindow.h"

MainWindow::MainWindow()
{
    centralWidget = new QWidget;
    setCentralWidget(centralWidget);

    glWidget = new GLWidget;
    pixmapLabel = new QLabel;

    glWidgetArea = new QScrollArea;
    glWidgetArea->setWidget(glWidget);
    glWidgetArea->setWidgetResizable(true);
    glWidgetArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glWidgetArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glWidgetArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    glWidgetArea->setMinimumSize(50, 50);

    pixmapLabelArea = new QScrollArea;
    pixmapLabelArea->setWidget(pixmapLabel);
    pixmapLabelArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    pixmapLabelArea->setMinimumSize(50, 50);

//    xSlider = createSlider(SIGNAL(xRotationChanged(int)),
//                           SLOT(setXRotation(int)));
//    ySlider = createSlider(SIGNAL(yRotationChanged(int)),
//                           SLOT(setYRotation(int)));
//    zSlider = createSlider(SIGNAL(zRotationChanged(int)),
//                           SLOT(setZRotation(int)));
    xSlider = new QLabel("0");
    QObject::connect(glWidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setNum(int)));
    ySlider = new QLabel("0");
    QObject::connect(glWidget, SIGNAL(yRotationChanged(int)), ySlider, SLOT(setNum(int)));
    zSlider = new QLabel("0");
    QObject::connect(glWidget, SIGNAL(zRotationChanged(int)), zSlider, SLOT(setNum(int)));

    createActions();
    createMenus();

    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    QGridLayout *centralLayout = new QGridLayout;
    centralLayout->addWidget(glWidgetArea, 0, 0);
    glWidgetArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    centralLayout->addWidget(pixmapLabelArea, 0, 1);

    vBoxLayout->addStretch();

    vBoxLayout->addWidget(new QLabel("N Tetrahedrons"));
    nSpinBox = new QSpinBox;
    nSpinBox->setRange(0,40);
    nSpinBox->setSingleStep(1);
//    nSpinBox->setDecimals(1);
    nSpinBox->setValue(8);
    vBoxLayout->addWidget(nSpinBox);
    QObject::connect(nSpinBox, SIGNAL(valueChanged(int)), glWidget, SLOT(setN(int)));

    resetButton = new QPushButton("Reset");
    QObject::connect(resetButton, SIGNAL(clicked()), this, SLOT(on_resetButton()));
    vBoxLayout->addWidget(resetButton);

    autoMinimize = new QCheckBox("Auto Solve");
    vBoxLayout->addWidget(autoMinimize);
    QObject::connect(autoMinimize, SIGNAL(toggled(bool)), glWidget, SLOT(setAutoCloseGap(bool)));
    autoMinimize->setChecked(true);

    vBoxLayout->addWidget(new QLabel("Odd Angles"));
    oddAngleSpinBox = new QDoubleSpinBox;
    oddAngleSpinBox->setRange(-360,360);
    oddAngleSpinBox->setSingleStep(22.5);
    oddAngleSpinBox->setDecimals(3);
    oddAngleSpinBox->setValue(90);
    vBoxLayout->addWidget(oddAngleSpinBox);
    QObject::connect(oddAngleSpinBox, SIGNAL(valueChanged(double)), glWidget, SLOT(setOddAngle(double)));

    minGapOdd = new QPushButton("Minimize Gap");
    vBoxLayout->addWidget(minGapOdd);
    QObject::connect(minGapOdd, SIGNAL(clicked()), glWidget, SLOT(closeGapUsingOddAngle()));

    QHBoxLayout * hBoxLayout = new QHBoxLayout;

    jumpByOnes = new QPushButton("1");
    jumpByTenths = new QPushButton(".1");
    jumpByTens = new QPushButton("10");
    jumpByOnes->setCheckable(true);
    jumpByTenths->setCheckable(true);
    jumpByTens->setCheckable(true);
    QObject::connect(jumpByOnes, SIGNAL(clicked()),this, SLOT(on_jumpByOnes()));
    QObject::connect(jumpByTens, SIGNAL(clicked()),this, SLOT(on_jumpByTens()));
    QObject::connect(jumpByTenths, SIGNAL(clicked()),this, SLOT(on_jumpByTenths()));

    hBoxLayout->addWidget(jumpByTenths);
    hBoxLayout->addWidget(jumpByOnes);
    hBoxLayout->addWidget(jumpByTens);

    jumpButtons = new QButtonGroup;
    jumpButtons->addButton(jumpByOnes);
    jumpButtons->addButton(jumpByTens);
    jumpButtons->addButton(jumpByTenths);
    jumpButtons->setExclusive(true);

    vBoxLayout->addLayout(hBoxLayout);

    vBoxLayout->addWidget(new QLabel("Even Angles"));
    evenAngleSpinBox = new QDoubleSpinBox;
    evenAngleSpinBox->setRange(-360,360);
    evenAngleSpinBox->setSingleStep(22.5);
    evenAngleSpinBox->setDecimals(3);
    evenAngleSpinBox->setValue(180);
    vBoxLayout->addWidget(evenAngleSpinBox);
    QObject::connect(evenAngleSpinBox, SIGNAL(valueChanged(double)), glWidget, SLOT(setEvenAngle(double)));

    minGapEven = new QPushButton("Minimize Gap");
    vBoxLayout->addWidget(minGapEven);
    QObject::connect(minGapEven, SIGNAL(clicked()), glWidget, SLOT(closeGapUsingEvenAngle()));

    QObject::connect(glWidget, SIGNAL(evenAngleChanged(double)), this, SLOT(on_evenAngleChanged(double)));
    QObject::connect(glWidget, SIGNAL(oddAngleChanged(double)), this, SLOT(on_oddAngleChanged(double)));

    centralLayout->addLayout(vBoxLayout, 0, 1);


    centralLayout->addWidget(xSlider, 1, 0, 1, 2);
    centralLayout->addWidget(ySlider, 2, 0, 1, 2);
    centralLayout->addWidget(zSlider, 3, 0, 1, 2);
    centralWidget->setLayout(centralLayout);

    xSlider->setNum(15 * 16);
    ySlider->setNum(345 * 16);
    zSlider->setNum(0 * 16);

    setWindowTitle(tr("Grabber"));
//    resize(400, 300);
}

void MainWindow::renderIntoPixmap()
{
    QSize size = getSize();
    if (size.isValid()) {
        QPixmap pixmap = glWidget->renderPixmap(size.width(), size.height());
        setPixmap(pixmap);
    }
}

void MainWindow::grabFrameBuffer()
{
    QImage image = glWidget->grabFrameBuffer();
    setPixmap(QPixmap::fromImage(image));
}

void MainWindow::clearPixmap()
{
    setPixmap(QPixmap());
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Grabber"),
            tr("The <b>Grabber</b> example demonstrates two approaches for "
               "rendering OpenGL into a Qt pixmap."));
}

void MainWindow::createActions()
{
    renderIntoPixmapAct = new QAction(tr("&Render into Pixmap..."), this);
    renderIntoPixmapAct->setShortcut(tr("Ctrl+R"));
    connect(renderIntoPixmapAct, SIGNAL(triggered()),
            this, SLOT(renderIntoPixmap()));

    grabFrameBufferAct = new QAction(tr("&Grab Frame Buffer"), this);
    grabFrameBufferAct->setShortcut(tr("Ctrl+G"));
    connect(grabFrameBufferAct, SIGNAL(triggered()),
            this, SLOT(grabFrameBuffer()));

    clearPixmapAct = new QAction(tr("&Clear Pixmap"), this);
    clearPixmapAct->setShortcut(tr("Ctrl+L"));
    connect(clearPixmapAct, SIGNAL(triggered()), this, SLOT(clearPixmap()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(renderIntoPixmapAct);
    fileMenu->addAction(grabFrameBufferAct);
    fileMenu->addAction(clearPixmapAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

QSlider *MainWindow::createSlider(const char *changedSignal,
                                  const char *setterSlot)
{
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 360 * 16);
    slider->setSingleStep(16);
    slider->setPageStep(15 * 16);
    slider->setTickInterval(15 * 16);
    slider->setTickPosition(QSlider::TicksRight);
    connect(slider, SIGNAL(valueChanged(int)), glWidget, setterSlot);
    connect(glWidget, changedSignal, slider, SLOT(setValue(int)));
    return slider;
}

void MainWindow::setPixmap(const QPixmap &pixmap)
{
    pixmapLabel->setPixmap(pixmap);
    QSize size = pixmap.size();
    if (size - QSize(1, 0) == pixmapLabelArea->maximumViewportSize())
        size -= QSize(1, 0);
    pixmapLabel->resize(size);
}

QSize MainWindow::getSize()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Grabber"),
                                         tr("Enter pixmap size:"),
                                         QLineEdit::Normal,
                                         tr("%1 x %2").arg(glWidget->width())
                                                      .arg(glWidget->height()),
                                         &ok);
    if (!ok)
        return QSize();

    QRegExp regExp(tr("([0-9]+) *x *([0-9]+)"));
    if (regExp.exactMatch(text)) {
        int width = regExp.cap(1).toInt();
        int height = regExp.cap(2).toInt();
        if (width > 0 && width < 2048 && height > 0 && height < 2048)
            return QSize(width, height);
    }

    return glWidget->size();
}

void MainWindow::on_jumpByTenths()
{
    evenAngleSpinBox->setSingleStep(0.1);
    oddAngleSpinBox->setSingleStep(0.1);
}

void MainWindow::on_jumpByOnes()
{
    evenAngleSpinBox->setSingleStep(1);
    oddAngleSpinBox->setSingleStep(1);
}

void MainWindow::on_jumpByTens()
{
    evenAngleSpinBox->setSingleStep(10);
    oddAngleSpinBox->setSingleStep(10);
}

void MainWindow::on_resetButton()
{
    glWidget->setAutoCloseGap(false);
    evenAngleSpinBox->setValue(180);
    oddAngleSpinBox->setValue(180);
    glWidget->setAutoCloseGap(autoMinimize->isChecked());
}

void MainWindow::on_evenAngleChanged(double e)
{
    glWidget->setAutoCloseGap(false);
    evenAngleSpinBox->setValue(e);
//    oddAngleSpinBox->setValue(180);
    glWidget->setAutoCloseGap(autoMinimize->isChecked());
}

void MainWindow::on_oddAngleChanged(double o)
{
    glWidget->setAutoCloseGap(false);
//    evenAngleSpinBox->setValue(90);
    oddAngleSpinBox->setValue(o);
    glWidget->setAutoCloseGap(autoMinimize->isChecked());
}
