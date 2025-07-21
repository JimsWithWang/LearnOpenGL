#pragma once
#ifndef RECTANGLEWIDGET_H
#define RECTANGLEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QKeyEvent>

class RectangleWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit RectangleWidget(QWidget* parent = nullptr);
    virtual ~RectangleWidget();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void keyReleaseEvent(QKeyEvent* event) override;

private:
    void initShaderProgram();
    void initObject();

    QByteArray readShaderFile(const QString& strShaderFile);
    void checkCompileLinksStatus(unsigned value, int type = 0);

    unsigned VAO, VBO, EBO;
    unsigned m_nShaderProgram;
};

#endif  // RECTANGLEWIDGET_H

