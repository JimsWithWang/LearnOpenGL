#include "ShaderWidget.h"
#include <QFile>
#include <QDebug>

ShaderWidget::ShaderWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    setWindowTitle(QStringLiteral("LearnOpenGL"));
}

ShaderWidget::~ShaderWidget()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(m_nShaderProgram);
}

void ShaderWidget::initializeGL()
{
    initializeOpenGLFunctions();
    initShaderProgram();
    initObject();
}

void ShaderWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

void ShaderWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_nShaderProgram);
    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void ShaderWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (Qt::Key_Escape == event->key())
        close();
}

void ShaderWidget::initShaderProgram()
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

void ShaderWidget::initObject()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

QByteArray ShaderWidget::readShaderFile(const QString& strShaderFile)
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

void ShaderWidget::checkCompileLinkStatus(unsigned value, int type)
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

