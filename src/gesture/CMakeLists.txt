cmake_minimum_required(VERSION 2.8.3)
project(gesture)

find_package(catkin REQUIRED COMPONENTS
   roscpp qt_build cv_bridge sensor_msgs std_msgs image_transport
)
set(OpenCV_DIR /usr/local/share/OpenCV)
message(STATUS "    version: ${OpenCV_VERSION}")
find_package(OpenCV)
include_directories(
  include ${catkin_INCLUDE_DIRS}${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

catkin_package()


include_directories(
  include ${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS}
)



add_executable(gesturecon_pub
  src/gesturecon_pub.cpp
)
add_dependencies(gesturecon_pub ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(gesturecon_pub
  ${catkin_LIBRARIES}  ${OpenCV_INCLUDE_DIRS}
)

