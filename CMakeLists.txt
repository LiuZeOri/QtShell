cmake_minimum_required(VERSION 3.24)

project(QtShell VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 COMPONENTS Quick REQUIRED)

qt_add_executable(appQtShell
    src/main.cpp
    src/controlflow.h src/controlflow.cpp
    src/controller.h src/controller.cpp
    src/shell.h src/shell.cpp
    src/varlib.h src/varlib.cpp
)

qt_add_qml_module(appQtShell
    URI QtShell
    VERSION 1.0
    QML_FILES qml/Main.qml
)

set_target_properties(appQtShell PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appQtShell
    PRIVATE Qt6::Quick)

install(TARGETS appQtShell
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
