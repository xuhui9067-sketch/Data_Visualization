cmake_minimum_required(VERSION 3.16)

project(Data_Visualization VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Network Widgets PrintSupport Sql)

qt_standard_project_setup(REQUIRES 6.8)

# 统一目标名称为 appData_Visualization
qt_add_executable(appData_Visualization
    main.cpp
)

qt_add_qml_module(appData_Visualization
    URI Data_Visualization
    QML_FILES
        Main.qml
    SOURCES
        datareceiver.h datareceiver.cpp
        dataprocessor.h dataprocessor.cpp
        qcustomplot.h qcustomplot.cpp
        customplotitem.h customplotitem.cpp
        databasemanager.h databasemanager.cpp
)

set_target_properties(appData_Visualization PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 这里的名字必须和上面 add_executable 保持一致！
target_link_libraries(appData_Visualization
    PRIVATE Qt6::Quick Qt6::Network Qt6::Widgets Qt6::PrintSupport Qt6::Sql
)

include(GNUInstallDirs)
install(TARGETS appData_Visualization
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
