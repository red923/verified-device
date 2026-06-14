cmake_minimum_required(VERSION 3.16)
project(VisualIdentification)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS 
    Core 
    Gui 
    Widgets 
    Sql 
    REQUIRED
)

add_executable(VisualIdentification
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/database.cpp
    src/database.h
    src/identifier.cpp
    src/identifier.h
    src/objectmodel.h
)

target_link_libraries(VisualIdentification
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Sql
)
