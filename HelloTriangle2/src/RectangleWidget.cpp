#include "RectangleWidget.h"
#include <QFile>
#include <QDebug>

RectangleWidget::RectangleWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    setWindowTitle(QStringLiteral("LearnOpenGL"));
}

RectangleWidget::~RectangleWidget()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(m_nShaderProgram);
}

void RectangleWidget::initializeGL()
{
    initializeOpenGLFunctions();
    initShaderProgram();
    initObject();
}

void RectangleWidget::resizeGL(int width, int height)
{
    Q_UNUSED(width)
    Q_UNUSED(height)
}

void RectangleWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_nShaderProgram);
    glBindVertexArray(VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    QMetaObject::invokeMethod(this, "update", Qt::QueuedConnection);
}

void RectangleWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (Qt::Key_Escape == event->key())
        close();
}

void RectangleWidget::initShaderProgram()
{
    unsigned vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    char* strVertexShader = readShaderFile(QStringLiteral(":shader/shaders/shader.vs")).data();
    glShaderSource(vertex_shader, 1, &strVertexShader, nullptr);
    glCompileShader(vertex_shader);
    checkCompileLinksStatus(vertex_shader, GL_VERTEX_SHADER);

    unsigned fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    char* strFragmentShader = readShaderFile(QStringLiteral(":shader/shaders/shader.fs")).data();
    glShaderSource(fragment_shader, 1,  &strFragmentShader, nullptr);
    glCompileShader(fragment_shader);
    checkCompileLinksStatus(fragment_shader, GL_FRAGMENT_SHADER);

    m_nShaderProgram = glCreateProgram();
    glAttachShader(m_nShaderProgram, vertex_shader);
    glAttachShader(m_nShaderProgram, fragment_shader);
    glLinkProgram(m_nShaderProgram);
    checkCompileLinksStatus(m_nShaderProgram);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void RectangleWidget::initObject()
{
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // 右上角
        0.5f, -0.5f, 0.0f,  // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f   // 左上角
    };
    unsigned int indices[] = {
        0, 1, 3, // 第一个三角形
        1, 2, 3  // 第二个三角形
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

QByteArray RectangleWidget::readShaderFile(const QString& strShaderFile)
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

void RectangleWidget::checkCompileLinksStatus(unsigned value, int type)
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

