cmake_minimum_required(VERSION 2.8.3)
project(gg_mavlink)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  rospy
  std_msgs
  message_generation
)

################################################
## Declare ROS messages, services and actions ##
################################################

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  Num.msg
)

## Generate services in the 'srv' folder
add_service_files(
  FILES
  AddTwoInts.srv
)

## Generate added messages and services with any dependencies listed here
generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES beginner_tutorials
   CATKIN_DEPENDS message_runtime	#roscpp rospy std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########


include_directories(include ${catkin_INCLUDE_DIRS})

# Create test pulisher executables for ROS messages to mavlink_ros
# By default all executables go into the devel space at ~/catkin_ws/devel/lib/share/<package name>
# These can be invoked directly by rosrun <package name> <executable name> e.g rosrun beginner_tutorials talker
add_executable(mavlink_send src/mavlink_send.cpp)
target_link_libraries(mavlink_send ${catkin_LIBRARIES})
add_dependencies(mavlink_send gg_mavlink_generate_messages_cpp)

add_executable(mavlink_listen src/mavlink_listen.cpp)
target_link_libraries(mavlink_listen ${catkin_LIBRARIES})
add_dependencies(mavlink_listen gg_mavlink_generate_messages_cpp)

# Create Server and Client executables for ROS services
#add_executable(add_two_ints_server src/add_two_ints_server.cpp)
#target_link_libraries(add_two_ints_server ${catkin_LIBRARIES})
#add_dependencies(add_two_ints_server beginner_tutorials_gencpp)

#add_executable(add_two_ints_client src/add_two_ints_client.cpp)
#target_link_libraries(add_two_ints_client ${catkin_LIBRARIES})
#add_dependencies(add_two_ints_client beginner_tutorials_gencpp)
