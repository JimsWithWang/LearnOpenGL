#pragma once
#ifndef COORDINATEWIDGET_H
#define COORDINATEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QKeyEvent>

class CoordinateWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit CoordinateWidget(QWidget* parent = nullptr);
    virtual ~CoordinateWidget();

protected:
    virtual void initializeGL() override;
    virtual void resizeGL(int width, int height) override;
    virtual void paintGL() override;

    virtual void keyReleaseEvent(QKeyEvent* event) override;

private:
    void initShaderProgram();
    void initObject();
    void initTexture(unsigned& texture, const QString& strImgFile, int nImgType);

    QByteArray readShaderFile(const QString& strShaderFile);
    void checkCompileLinkStatus(unsigned& shader, int nShaderType = -1);

    unsigned VAO, VBO;
    unsigned m_nShaderProgram;
    unsigned m_nTexture1, m_nTexture2;
};

#endif  // COORDINATEWIDGET_H

