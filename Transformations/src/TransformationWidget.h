#pragma once
#ifndef TRANSFORMATIONWIDGET_H
#define TRANSFORMATIONWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QKeyEvent>

class TransformationWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit TransformationWidget(QWidget* parent = nullptr);
    virtual ~TransformationWidget();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void keyReleaseEvent(QKeyEvent* event) override;

private:
    void initShaderProgram();
    void initObject();
    void initTexture(unsigned& nTexture, const QString& strFilePath, int type);

    QByteArray readShaderFile(const QString& strShaderFile);
    void checkCompileLinkStatus(unsigned nShader, int type = 0);

    unsigned VAO, VBO, EBO;
    unsigned m_nShaderProgram;
    unsigned m_nTexture1, m_nTexture2;
};

#endif  // TRANSFORMATIONWIDGET_H

