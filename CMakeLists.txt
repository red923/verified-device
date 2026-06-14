cmake_minimum_required(VERSION 3.16)
project(ObjectIdentification)

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

add_executable(ObjectIdentification
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/database.cpp
    src/database.h
    src/identifier.cpp
    src/identifier.h
    src/objectmodel.cpp
    src/objectmodel.h
)

target_link_libraries(ObjectIdentification
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Sql
)
