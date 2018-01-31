###############################################################
######                                                   ######
######			Author: Pablo R.S. (aka. Bardo91)        ######
######                                                   ######
###############################################################
 
cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project(ual_pad_control)

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



find_package(OpenCV 3 REQUIRED)
set_target_properties(${OpenCV_LIBS} PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE)

#########################################
######			Executable		   ######
#########################################
IF(UNIX)
	add_definitions(-std=c++11)
ENDIF()

GroupSources(src)


add_executable(ual_pad_control "src/ual_pad_control.cpp")
target_include_directories(ual_pad_control PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_include_directories(ual_pad_control PUBLIC ${catkin_INCLUDE_DIRS})
target_link_libraries(ual_pad_control ${catkin_LIBRARIES} simple_sockets_cpp joystick_controller)

add_dependencies(ual_pad_control ${catkin_EXPORTED_TARGETS} ${PROJECT_NAME}_generate_messages_cpp)

target_include_directories(ual_pad_control PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(ual_pad_control LINK_PUBLIC ${OpenCV_LIBS})