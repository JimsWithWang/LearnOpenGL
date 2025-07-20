#   启用Qt工具，查找Qt模块
macro(addQtLibraryBefore)
    set(USE_QT 1)

    set(CMAKE_AUTOUIC ON)   #   启用uic工具，可以将Qt的ui文件编译为C++文件
    set(CMAKE_AUTOMOC ON)   #   启用moc工具，可以将Qt代码中的非标准代码编译为标准C++代码
    set(CMAKE_AUTORCC ON)   #   启用rcc工具，可以将Qt中的qrc资源文件编译为二进制的rcc文件，或者编译到程序中

    foreach(var ${ARGV})
        find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${var})
        find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${var})
        string(TOUPPER ${var} upper_var)
        if(("${upper_var}" STREQUAL "WEBENGINEWIDGETS") AND OS_LINUX)
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=gold")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
        endif()
    endforeach()
endmacro()

#   链接Qt库，设置Qt属性
function(addQtLibraryBehind targetName)
    set(i 0)
    foreach(var ${ARGV})
        if(NOT (i EQUAL 0))
            target_link_libraries(${targetName} PRIVATE Qt${QT_VERSION_MAJOR}::${var})
        endif()
        math(EXPR i "${i} + 1")
    endforeach()
    unset(i)

    set_target_properties(${targetName} PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        # MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
    )

    install(TARGETS ${targetName}
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    if(QT_VERSION_MAJOR EQUAL 6)
        qt_finalize_executable(${targetName})
    endif()

    unset(USE_QT)
endfunction()