#pragma once
#ifndef TRIANGLEWIDGET_H
#define TRIANGLEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QKeyEvent>

class TriangleWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core 
{
    Q_OBJECT
public:
    explicit TriangleWidget(QWidget* parent = nullptr);
    virtual ~TriangleWidget();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void keyReleaseEvent(QKeyEvent* event) override;

private:
    void initShaderProgram();
    void initObject();

    QByteArray readShaderFromFile(const QString& strFileName);
    void checkLinkCompileStatus(unsigned value, int type = 0);

    unsigned VAO, VBO;
    unsigned m_nShaderPrograme;
};

#endif  // TRIANGLEWIDGET_H

