cmake_minimum_required(VERSION 3.16)
project(GunED VERSION 0.3.5 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set up AUTOMOC and some sensible defaults for runtime execution
# When using Qt 6.3, you can replace the code block below with
# qt_standard_project_setup()
set(CMAKE_AUTOMOC ON)
include(GNUInstallDirs)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Xml)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

add_executable(GunED
    airmsg.cpp
    airmsg.h
    gunspecreader.cpp
    gunspecreader.h
    gunspecxmlreader.cpp
    gunspecxmlreader.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    propertyfield.h
    propertywidget.cpp
    propertywidget.h
)
target_compile_definitions(GunED PRIVATE
    QT_DEPRECATED_WARNINGS
    VER=\"'\\"0.3.5\\"'\"
)

target_link_libraries(GunED PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Xml
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(GunED PRIVATE
        Qt::Widgets
    )
endif()

install(TARGETS GunED
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
