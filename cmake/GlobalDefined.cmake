#   设置系统宏定义
if(CMAKE_SYSTEM_NAME STREQUAL "Windows" OR (NOT CMAKE_SYSTEM_NAME AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows"))
    set(OS_WIN 1)           #   windows系统
    add_compile_definitions(OS_WIN)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR (NOT CMAKE_SYSTEM_NAME AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux"))
    set(OS_LINUX 1)         #   linux系统
    add_compile_definitions(OS_LINUX)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR (NOT CMAKE_SYSTEM_NAME AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin"))
    set(OS_MAC 1)           #   MacOS系统
    add_compile_definitions(OS_MAC)
elseif((CMAKE_SYSTEM_NAME STREQUAL "DragonFly") OR (CMAKE_SYSTEM_NAME STREQUAL "FreeBSD") OR 
    (CMAKE_SYSTEM_NAME STREQUAL "OpenBSD") OR (CMAKE_SYSTEM_NAME STREQUAL "NetBSD"))
    set(OS_BSD 1)           #   BSD系统，包括DragonFlyBSD, FreeBSD, OpenBSD 或 NetBSD
    add_compile_definitions(OS_BSD)
elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS" OR (NOT CMAKE_SYSTEM_NAME AND CMAKE_HOST_SYSTEM_NAME STREQUAL "iOS"))
    set(OS_IOS 1)           #   ios系统
    add_compile_definitions(OS_IOS)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android" OR (NOT CMAKE_SYSTEM_NAME AND CMAKE_HOST_SYSTEM_NAME STREQUAL "Android"))
    set(OS_ANDROID 1)       #   android系统
    add_compile_definitions(OS_ANDROID)
else()
    set(OS_OTHER 1)         #   其它系统
endif()

#   设置windows中的工具链宏定义
if(OS_WIN)
    #   在windows平台时，文本使用utf-8编码
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
    if(CMAKE_SYSTEM_NAME STREQUAL "CYGWIN")
        set(TOOLCHAIN_CYGWIN 1)      #   windows系统中的cygwin构建系统
        add_compile_definitions(TOOLCHAIN_CYGWIN)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "MSYS")
        set(TOOLCHAIN_MSYS 1)        #   msys构建系统
        add_compile_definitions(TOOLCHAIN_MSYS)
    else()
        set(TOOLCHAIN_MSVC 1)       #   windows系统中微软提供的工具链
        add_compile_definitions(TOOLCHAIN_MSVC)
    endif()
endif()

#   设置编译器宏定义
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(COMPILER_CLANG 1)   #   Clang编译器
    add_compile_definitions(COMPILER_CLANG)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(COMPILER_GNU 1)     #   gcc和g++编译器
    add_compile_definitions(COMPILER_GNU)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(COMPILER_MSVC 1)    #   msvc编译器
    add_compile_definitions(COMPILER_MSVC)
else()
    set(COMPILER_OTHER 1)
endif()

#   根据不同平台设置不同的参数
if(COMPILER_MSVC)
    #   使用MSVC编译器时，将依赖的系统库以动态库的形式链接，
    #   若设置为/MT则会将依赖的系统库以静态库的形式编译到项目中
    #   注：整个项目需要统一设置
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

    #   /MP启用并行编译工程
    #   /W4启用所有标准警告、所有额外警告和一些更加严格的警告
    #   /permissive- 禁用一些Microsoft扩展
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /W4 /permissive-")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /W4 /permissive-")
else()
    #   非MSVC编译器生成compile_commands.json文件，供clangd使用
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    #   在非MSVC编译器启用并行编译
    set(CMAKE_BUILD_PARALLEL_LEVEL $(nproc))

    #   为gcc或clang编译器设置编译提示选项，
    #   all表示主要警告提示，-W表示启用
    #   extra表示额外警告提示，-W表示启用
    #   pedantic表示使用了非标准的扩展语法，-W表示启用
    #   c++98-compat表示不支持c++98标准的扩展语法，包含在pedantic选项中，-Wno-表示禁用
    #   shadow表示局部变量覆盖另一个局部变量或全局变量，-W表示启用
    if(COMPILER_GNU OR COMPILER_CLANG)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Wshadow")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wshadow")
    endif()
    if(COMPILER_CLANG)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-c++98-compat")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++98-compat")
    endif()
endif()