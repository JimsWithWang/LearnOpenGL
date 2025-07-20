cmake_minimum_required(VERSION 3.15)

project(HelloWindow)

set(EXE_NAME HelloWindow)
set(CODE_SOURCES 
    src/main.cpp 
    src/ClearWidget.cpp 
    src/ClearWidget.h 
)

include(AddQtLibrary)

addQtLibraryBefore(Widgets)
if(USE_QT)
    set(TARGET_NAME ${EXE_NAME})          # add_library或add_executable设定的程序名称
    set(SOURCES_FILES ${CODE_SOURCES})        # 源码文件列表，*.h、*.hpp、*.cpp、*.c等格式文件
    set(QT_RESOURCE_FILES)    # qrc文件列表，管理代码中可以直接调用资源文件的*.qrc文件
    set(QT_TS_FILES)          # ts文件列表，*.ts格式文件，用于多语言显示支持
    set(QT_REP_FILES)         # rep文件列表，*.rep格式文件，用于Qt5RO模块进行rpc通信

    include(GenerateQtExe)
endif(USE_QT)
addQtLibraryBehind(${EXE_NAME} Widgets)