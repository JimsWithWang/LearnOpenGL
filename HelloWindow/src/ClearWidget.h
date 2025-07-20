#pragma once
#ifndef CLEARWIDGET_H
#define CLEARWIDGET_H 

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QKeyEvent>

class ClearWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit ClearWidget(QWidget* parent = nullptr);
    virtual ~ClearWidget();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void keyReleaseEvent(QKeyEvent *event) override;
};

#endif  // CLEARWIDGET_H

