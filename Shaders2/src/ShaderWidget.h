#pragma once
#ifndef SHADERWIDGET_H
#define SHADERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QKeyEvent>

class ShaderWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit ShaderWidget(QWidget* parent = nullptr);
    virtual ~ShaderWidget();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void keyReleaseEvent(QKeyEvent* event) override;

private:
    void initShaderProgram();
    void initObject();

    QByteArray readShaderFile(const QString& strShaderFile);
    void checkCompileLinkStatus(unsigned value, int type = 0);

    unsigned VAO, VBO;
    unsigned m_nShaderProgram;
};

#endif  // SHADERWIDGET_H

