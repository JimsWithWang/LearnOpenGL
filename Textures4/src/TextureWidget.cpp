#include "TextureWidget.h"
#include <QFile>
#include <QDebug>
#include <QImage>

TextureWidget::TextureWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    setWindowTitle(QStringLiteral("LearnOpenGL"));
}

TextureWidget::~TextureWidget()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(m_nShaderProgram);
    glDeleteTextures(1, &m_nTexture1);
    glDeleteTextures(1, &m_nTexture2);
}

void TextureWidget::initializeGL()
{
    initializeOpenGLFunctions();
    initShaderProgram();
    initObject();
    initTexture(m_nTexture1, QStringLiteral(":/texture/textures/container.jpg"), GL_RGB);
    initTexture(m_nTexture2, QStringLiteral(":/texture/textures/awesomeface.png"), GL_RGBA);

    glUseProgram(m_nShaderProgram);
    glUniform1i(glGetUniformLocation(m_nShaderProgram, "texture1"), 0);
    glUniform1i(glGetUniformLocation(m_nShaderProgram, "texture2"), 1);
}

void TextureWidget::resizeGL(int width, int height)
{
    Q_UNUSED(width)
    Q_UNUSED(height)
}

void TextureWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_nTexture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_nTexture2);

    glUseProgram(m_nShaderProgram);
    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void TextureWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (Qt::Key_Escape == event->key())
        close();
}

void TextureWidget::initShaderProgram()
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

void TextureWidget::initObject()
{
    float vertices[] = {
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void TextureWidget::initTexture(unsigned& nTexture, const QString& strFilePath, int type)
{
    glGenTextures(1, &nTexture);
    glBindTexture(GL_TEXTURE_2D, nTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    QImage img(strFilePath);
    if (GL_RGB == type)
    {
        QImage glImg = img.mirrored().convertToFormat(QImage::Format_RGB888);
        if (glImg.isNull())
        {
            qDebug() << "Failed to load image!" << Qt::endl;
            return;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, glImg.width(), glImg.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, glImg.constBits());
    }
    else if (GL_RGBA == type)
    {
        QImage glImg = img.mirrored().convertToFormat(QImage::Format_RGBA8888);
        if (glImg.isNull())
        {
            qDebug() << "Failed to load image!" << Qt::endl;
            return;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glImg.width(), glImg.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, glImg.constBits());
    }
    else  
    {
        qDebug() << "Image type is error!" << Qt::endl;
    }
    glGenerateMipmap(GL_TEXTURE_2D);
}

QByteArray TextureWidget::readShaderFile(const QString& strShaderFile)
{
    QFile file(strShaderFile);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open file!" << Qt::endl;
        return QByteArray("");
    }
    QByteArray strContent = file.readAll();
    file.close();
    return strContent;
}

void TextureWidget::checkCompileLinkStatus(unsigned value, int type)
{
    int success;
    char infoLog[512];
    if (GL_VERTEX_SHADER == type || GL_FRAGMENT_SHADER == type)
    {
        glGetShaderiv(value, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(value, 512, nullptr, infoLog);
            if (GL_VERTEX_SHADER == type)
                qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << Qt::endl;
            else 
                qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << Qt::endl;
        }
    }
    else  
    {
        glGetProgramiv(value, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(value, 512, nullptr, infoLog);
            qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << Qt::endl;
        }
    }
}

