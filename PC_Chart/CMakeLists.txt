cmake_minimum_required(VERSION 3.10)
project(PTP)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_PREFIX_PATH /opt/Qt/5.12.0/gcc_64)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

#find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Charts REQUIRED)

set(PTP_SRCS
        #mainwindow.ui
        mainwindow.cpp
        main.cpp
        Chart.cpp Chart.h SerialPort.cpp SerialPort.h)

add_executable(PTP ${PTP_SRCS})

qt5_use_modules(PTP Core SerialPort)
target_link_libraries(PTP ${Qt5Widgets_LIBRARIES} ${Qt5Charts_LIBRARIES})