cmake_minimum_required(VERSION 3.16)
project(ADCCollector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH "/opt/qt5/5.15.2/gcc_64")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets usb-1.0 stdc++fs)

find_package(PkgConfig REQUIRED)
pkg_check_modules(libusb REQUIRED libusb-1.0)

add_executable(${PROJECT_NAME} main.cpp
        mainwindow.cpp
        mainwindow.h
        centralwidget.cpp
        centralwidget.h
        resources.qrc
        chartwidget.cpp
        chartwidget.h
        settings.cpp
        settings.h
        settingsdialog.cpp
        settingsdialog.h
        globalsettingstabwidget.cpp
        globalsettingstabwidget.h
        channelssettingstabwidget.cpp
        channelssettingstabwidget.h
        directoryselector.cpp
        directoryselector.h
        channelsettingswidget.cpp
        channelsettingswidget.h
        adc.cpp
        adc.h
        infowidget.cpp
        infowidget.h)

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})
