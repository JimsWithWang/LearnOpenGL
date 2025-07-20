if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    if(QT_RESOURCE_FILES)
        qt6_add_resources(QRC_FILES ${QT_RESOURCE_FILES})
    endif()
    if(QT_REP_FILES)
        qt6_add_repc_sources(REPC_FILES ${QT_REP_FILES})
    endif()
    qt_add_executable(${TARGET_NAME}
        MANUAL_FINALIZATION
        ${SOURCES_FILES}
        ${QRC_FILES}
        ${REPC_FILES}
    )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET DSystemSetup APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    if(QT_TS_FILES)
        qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${QT_TS_FILES})
    endif()
else()
    if(QT_RESOURCE_FILES)
        qt5_add_resources(QRC_FILES ${QT_RESOURCE_FILES})
    endif()
    if(QT_REP_FILES)
        qt5_generate_repc(REPC_FILES ${QT_REP_FILES} SOURCE)
    endif()
    if(ANDROID)
        add_library(${TARGET_NAME} SHARED
            ${SOURCES_FILES}
            ${QRC_FILES}
            ${REPC_FILES}
        )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${TARGET_NAME}
            ${SOURCES_FILES}
            ${QRC_FILES}
            ${REPC_FILES}
        )
    endif()

    if(QT_TS_FILES)
        qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${QT_TS_FILES})
    endif()
endif()

unset(TARGET_NAME)          # add_library或add_executable设定的程序名称
unset(SOURCES_FILES)        # 源码文件列表，*.h、*.hpp、*.cpp、*.c等格式文件
unset(QT_RESOURCE_FILES)    # qrc文件列表，管理代码中可以直接调用资源文件的*.qrc文件
unset(QT_TS_FILES)          # ts文件列表，*.ts格式文件，用于多语言显示支持
unset(QT_REP_FILES)         # rep文件列表，*.rep格式文件，用于Qt5RO模块进行rpc通信