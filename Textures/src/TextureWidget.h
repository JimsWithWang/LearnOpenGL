#pragma once
#ifndef TEXTUREWIDGET_H
#define TEXTUREWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QKeyEvent>

class TextureWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit TextureWidget(QWidget* parent = nullptr);
    virtual ~TextureWidget();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    void keyReleaseEvent(QKeyEvent* event) override;

private:
    void initShaderProgram();
    void initObject();
    void initTexture();

    QByteArray readShaderFile(const QString& strShaderFile);
    void checkCompileLinkStatus(unsigned value, int type = 0);

    unsigned VAO, VBO;
    unsigned m_nShaderProgram;
    unsigned m_nTexture;
};

#endif  // TEXTUREWIDGET_H

