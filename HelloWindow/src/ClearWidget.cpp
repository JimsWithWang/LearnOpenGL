#include "ClearWidget.h"

ClearWidget::ClearWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    setWindowTitle(QStringLiteral("LearnOpenGL"));
}

ClearWidget::~ClearWidget()
{

}

void ClearWidget::initializeGL()
{
    initializeOpenGLFunctions();
}

void ClearWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void ClearWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void ClearWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (Qt::Key_Escape == event->key())
        close();
}

