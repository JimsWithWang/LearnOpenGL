#include "CoordinateWidget.h"
#include <QFile>
#include <QDebug>
#include <QImage>
#include <QtMath>
#include <QDateTime>
#include <QMatrix4x4>

CoordinateWidget::CoordinateWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    setWindowTitle(QStringLiteral("LearnOpenGL"));
}

CoordinateWidget::~CoordinateWidget()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(m_nShaderProgram);
    glDeleteTextures(1, &m_nTexture1);
    glDeleteTextures(1, &m_nTexture2);
}

void CoordinateWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);
    initShaderProgram();
    initObject();
    initTexture(m_nTexture1, QStringLiteral(":/texture/textures/container.jpg"), GL_RGB);
    initTexture(m_nTexture2, QStringLiteral(":/texture/textures/awesomeface.png"), GL_RGBA);

    glUseProgram(m_nShaderProgram);
    auto textureLoc1 = glGetUniformLocation(m_nShaderProgram, "texture1");
    glUniform1i(textureLoc1, 0);
    auto textureLoc2 = glGetUniformLocation(m_nShaderProgram, "texture2");
    glUniform1i(textureLoc2, 1);
}

void CoordinateWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void CoordinateWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_nTexture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_nTexture2);

    glUseProgram(m_nShaderProgram);
    QMatrix4x4 model = QMatrix4x4();
    float radius = static_cast<float>(QDateTime::currentDateTime().toSecsSinceEpoch() % 360);
    model.rotate(radius, 0.5f, 1.0f, 0.0f);
    QMatrix4x4 view = QMatrix4x4();
    view.translate(0.0f, 0.0f, -3.0f);
    QMatrix4x4 projection = QMatrix4x4();
    projection.perspective(45.0f, (float)width() / height(), 0.1f, 100.0f);

    unsigned modelLoc = glGetUniformLocation(m_nShaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.constData());
    unsigned viewLoc = glGetUniformLocation(m_nShaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.constData());
    unsigned ProjectionLoc = glGetUniformLocation(m_nShaderProgram, "projection");
    glUniformMatrix4fv(ProjectionLoc, 1, GL_FALSE, projection.constData());

    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void CoordinateWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (Qt::Key_Escape == event->key())
        close();
}

void CoordinateWidget::initShaderProgram()
{
    unsigned vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    char* strVertexShader = readShaderFile(QStringLiteral(":/shader/shaders/shader.vs")).data();
    glShaderSource(vertex_shader, 1, &strVertexShader, nullptr);
    glCompileShader(vertex_shader);
    checkCompileLinkStatus(vertex_shader, GL_VERTEX_SHADER);

    unsigned fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    char* strFragmentShader = readShaderFile(QStringLiteral(":/shader/shaders/shader.fs")).data();
    glShaderSource(fragment_shader, 1, &strFragmentShader, nullptr);
    glCompileShader(fragment_shader);
    checkCompileLinkStatus(fragment_shader, GL_FRAGMENT_SHADER);

    m_nShaderProgram = glCreateProgram();
    glAttachShader(m_nShaderProgram, vertex_shader);
    glAttachShader(m_nShaderProgram, fragment_shader);
    glLinkProgram(m_nShaderProgram);
    checkCompileLinkStatus(m_nShaderProgram);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void CoordinateWidget::initObject()
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void CoordinateWidget::initTexture(unsigned& texture, const QString& strImgFile, int nImgType)
{
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    QImage img(strImgFile);
    switch (nImgType)
    {
    case GL_RGB:
    {
        QImage glImg = img.mirrored().convertToFormat(QImage::Format_RGB888);
        if (glImg.isNull())
            qDebug() << QStringLiteral("Failed to open image!") << Qt::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, glImg.width(), glImg.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, glImg.constBits());
    }
        break;
    case GL_RGBA:
    {
        QImage glImg = img.mirrored().convertToFormat(QImage::Format_RGBA8888);
        if (glImg.isNull())
            qDebug() << QStringLiteral("Failed to open image!") << Qt::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImg.width(), glImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImg.constBits());
    }
        break;
    default:
        qDebug() << QStringLiteral("Unsuported image type!") << Qt::endl;
        break;
    }
    glGenerateMipmap(GL_TEXTURE_2D);
}

QByteArray CoordinateWidget::readShaderFile(const QString& strShaderFile)
{
    QFile file(strShaderFile);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << QStringLiteral("Failed to read shader file!") << Qt::endl;
        return QByteArray("");
    }
    QByteArray strContent = file.readAll();
    file.close();
    return strContent;
}

void CoordinateWidget::checkCompileLinkStatus(unsigned& nShader, int type)
{
    int success;
    char infoLog[512];
    if (GL_VERTEX_SHADER == type || GL_FRAGMENT_SHADER == type)
    {
        glGetShaderiv(nShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(nShader, 512, nullptr, infoLog);
            if (GL_VERTEX_SHADER == type)
                qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << Qt::endl;
            else 
                qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << Qt::endl;
        }
    }
    else  
    {
        glGetProgramiv(nShader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(nShader, 512, nullptr, infoLog);
            qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << Qt::endl;
        }
    }
}

