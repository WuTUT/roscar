cmake_minimum_required(VERSION 2.8.3)
project(cargui)


find_package(catkin REQUIRED COMPONENTS
   roscpp qt_build
)
include_directories(${catkin_INCLUDE_DIRS})

catkin_package()
rosbuild_prepare_qt4(QtCore QtGui)
file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/cargui/*.hpp)
QT4_WRAP_CPP(QT_MOC_HPP ${QT_MOC})
file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp)


add_executable(cargui ${QT_SOURCES} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP})
target_link_libraries(cargui ${QT_LIBRARIES} ${catkin_LIBRARIES})
install(TARGETS cargui RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
