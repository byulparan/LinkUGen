cmake_minimum_required(VERSION 2.6)

project(Link)

#link_directories("${CMAKE_CURRENT_LIST_DIR}/../../alien/Python-3.5.2/PCBuild/amd64")

add_library(Link SHARED Link.cpp)

set_target_properties(Link PROPERTIES PREFIX "")
set_target_properties(Link PROPERTIES DEBUG_POSTFIX "_d")
set_property(TARGET Link PROPERTY LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/LinkUGen")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

if(APPLE)
	if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		set(DETECTED_OS "Mac OS X")
		set_target_properties(Link PROPERTIES SUFFIX ".scx")
		target_compile_definitions(Link PRIVATE LINK_PLATFORM_MACOSX)
	elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		message( FATAL_ERROR "Only Mac OS X supported when __APPLE__ is defined" )
	endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 
elseif(UNIX)
	if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
		set(DETECTED_OS "Linux")
		set (CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS_INIT} "-lpthread -latomic")
		set_target_properties(Link PROPERTIES SUFFIX ".so")
		target_compile_definitions(Link PRIVATE LINK_PLATFORM_LINUX=1)
	elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
		message( FATAL_ERROR "Only Linux supported for non-Apple Unix platforms" )
	endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
elseif(WIN32)
	set(DETECTED_OS "Windows")
	set_target_properties(Link PROPERTIES SUFFIX ".scx")
	target_compile_definitions(Link PRIVATE 
		LINK_PLATFORM_WINDOWS=1
		_CRT_SECURE_NO_WARNINGS
		WIN32
		_WINDOWS
		_WIN32_WINNT=0x600
		NOMINMAX=1
		$<$<CONFIG:Debug>:DEBUG>
		$<$<CONFIG:Debug>:_DEBUG>
		$<$<CONFIG:Release>:NDEBUG>
	)
endif(APPLE)

if(WIN32)
	# this detects the COMPILER TARGET number of bits, not the OS.
	if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
		set_property(TARGET Link PROPERTY LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_LIST_DIR}/bin/${DETECTED_OS}/x64/debug")
		set_property(TARGET Link PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_LIST_DIR}/bin/${DETECTED_OS}/x64/debug")
		set_property(TARGET Link PROPERTY LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_LIST_DIR}/bin/${DETECTED_OS}/x64/release")
		set_property(TARGET Link PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_LIST_DIR}/bin/${DETECTED_OS}/x64/release")
	else("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
		set_property(TARGET Link PROPERTY LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_LIST_DIR}/bin/${DETECTED_OS}/x86/debug")
		set_property(TARGET Link PROPERTY RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_LIST_DIR}/bin/${DETECTED_OS}/x86/debug")
		set_property(TARGET Link PROPERTY LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_LIST_DIR}/bin/${DETECTED_OS}/x86/release")
		set_property(TARGET Link PROPERTY RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_LIST_DIR}/bin/${DETECTED_OS}/x86/release")
	endif("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")

target_include_directories(Link PRIVATE 
	"${SC3_SRC}/include/plugin_interface"
	"${SC3_SRC}/common"
	"${LINK_SRC}/include"
	"${LINK_SRC}/modules/asio-standalone/asio/include"
)
endif(WIN32)


set(SC3_SRC "$ENV{SC3_SRC}")
if(SC3_SRC) 
	message("SC3_SRC set to ${SC3_SRC}")
else()
    message(FATAL_ERROR "Please define the SC3_SRC environment variable to point to SuperCollider source folder.")
endif()

set(LINK_SRC "$ENV{LINK_SRC}")
if(LINK_SRC) 
	message("LINK_SRC set to ${LINK_SRC}")
else()
    message(FATAL_ERROR "Please define the LINK_SRC environment variable to point to the Ableton Link sources.")
endif()



target_include_directories(Link PRIVATE
	"/usr/local/include"
	"${SC3_SRC}/include/plugin_interface"
	"${SC3_SRC}/include/common"
	"${LINK_SRC}/include"
	"${LINK_SRC}/modules/asio-standalone/asio/include"
)




