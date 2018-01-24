###############################################################
######                                                   ######
######			Author: Pablo R.S. (aka. Bardo91)        ######
######                                                   ######
###############################################################
 
cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project(ual_joystick)

# Clasify sources according to folder structure.
# This creates a nice layout for the project in visual studio
# This macro is courtesy of http://www.cmake.org/pipermail/cmake/2013-November/056336.html
macro(GroupSources curdir)
	file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir}
		${PROJECT_SOURCE_DIR}/${curdir}/*)
	foreach(child ${children})
		if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
			GroupSources(${curdir}/${child})
		else()
			string(REPLACE "/" "\\" groupname ${curdir})
			source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
		endif()
	endforeach()
endmacro()

GroupSources(src)

#########################################
###### Loading 3rd party libraries.######
#########################################

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/" ${CMAKE_MODULE_PATH})

find_package(catkin REQUIRED COMPONENTS
			geometry_msgs
			std_msgs
			roscpp
			uav_abstraction_layer
			argument_parser
			simple_sockets_cpp
			)

catkin_package(
  CATKIN_DEPENDS roscpp std_msgs  uav_abstraction_layer geometry_msgs argument_parser joystick_controller simple_sockets_cpp
)



#########################################
######			Executable		   ######
#########################################
IF(UNIX)
	add_definitions(-std=c++11)
ENDIF()

GroupSources(src)

add_executable(main_ual "src/main_ual.cpp")
target_include_directories(main_ual PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_include_directories(main_ual PUBLIC ${catkin_INCLUDE_DIRS})
target_link_libraries(main_ual ${catkin_LIBRARIES} simple_sockets_cpp)

add_dependencies(main_ual ${catkin_EXPORTED_TARGETS} ${PROJECT_NAME}_generate_messages_cpp)


add_executable(main_joy "src/main_joy.cpp")
target_include_directories(main_joy PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_include_directories(main_joy PUBLIC ${catkin_INCLUDE_DIRS})
target_link_libraries(main_joy ${catkin_LIBRARIES} simple_sockets_cpp joystick_controller)

add_dependencies(main_joy ${catkin_EXPORTED_TARGETS} ${PROJECT_NAME}_generate_messages_cpp)
