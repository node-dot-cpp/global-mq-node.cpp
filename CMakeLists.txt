#-------------------------------------------------------------------------------------------
# Copyright (c) 2021, OLogN Technologies AG
#-------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.12.4)
project(NodeDotCpp CXX)
set(CMAKE_CXX_STANDARD 20)
include(CTest)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/memory-safe-cpp/library/src/iibmalloc/src/foundation/tools")
include(CommonCppFlags)


if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # coroutine needs libc++ at this time
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
    endif()
endif()



add_subdirectory(global-mq-marshalling)
add_subdirectory(memory-safe-cpp)


add_library(global-mq-nodecpp STATIC
    src/q_bqsed_infrastructure.cpp
    src/timeout_manager.cpp
    src/common.cpp
    src/fs.cpp
)

# target_compile_definitions(global-mq-nodecpp PUBLIC NODECPP_ENABLE_CLUSTERING)

if(TARGET EASTL)
	target_compile_definitions(global-mq-nodecpp PUBLIC NODECPP_USE_SAFE_MEMORY_CONTAINERS)
endif()

target_include_directories(global-mq-nodecpp
  PUBLIC include
  PUBLIC src
)

target_compile_definitions(global-mq-nodecpp PUBLIC GMQUEUE_CUSTOMIZED_Q_TYPES="../../../include/infrastructure/nodecpp_gmqueue_customized_types.h")

#target_compile_definitions(global-mq-nodecpp PUBLIC NODECPP_USE_SAFE_MEMORY_CONTAINERS)


target_link_libraries(global-mq-nodecpp safememory)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		target_compile_options(global-mq-nodecpp PUBLIC /await)
	endif()
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")

	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		target_compile_options(global-mq-nodecpp PUBLIC -fcoroutines-ts)
#        set_target_properties(foundation PROPERTIES LINK_OPTIONS -lc++)
	endif()
endif()


#-------------------------------------------------------------------------------------------
# Tests and samples
#-------------------------------------------------------------------------------------------
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(NODECPP_TEST ON CACHE BOOL "Enable NodeDotCpp tests")
else()
  set(NODECPP_TEST OFF CACHE BOOL "Enable NodeDotCpp tests")
endif()

if(NODECPP_TEST)
	add_executable(basic_test test/experimental/basic_test/user_code/main.cpp)
	target_link_libraries(basic_test global-mq-nodecpp)
endif()


# -----------------------------------------------------------------------------
# Below this line targets to run checker and dezombiefier intrumentation
# So far only tested with ninja generator under windows
# -----------------------------------------------------------------------------

set(DZ_INCLUDES 
    # this five path are for ubuntu libc++
    -resource-dir /usr/lib/llvm-9/lib/clang/9.0.0
    -isystem /usr/lib/llvm-9/bin/../include/c++/v1
    -isystem /usr/lib/llvm-9/lib/clang/9.0.0/include

    #this two path are for MaxOS libc++
    -isystem /Library/Developer/CommandLineTools/usr/include/c++/v1
    -isystem /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include

    #all path below are from global-mq-nodecpp
    -isystem ${CMAKE_CURRENT_SOURCE_DIR}/memory-safe-cpp/library/src/iibmalloc/src/foundation/3rdparty/fmt/include
    -isystem ${CMAKE_CURRENT_SOURCE_DIR}/memory-safe-cpp/library/src/iibmalloc/src/foundation/include 
    -isystem ${CMAKE_CURRENT_SOURCE_DIR}/memory-safe-cpp/library/src/iibmalloc/src 
    -isystem ${CMAKE_CURRENT_SOURCE_DIR}/memory-safe-cpp/library/include
    -isystem ${CMAKE_CURRENT_SOURCE_DIR}/memory-safe-cpp/library/src
    -isystem ${CMAKE_CURRENT_SOURCE_DIR}/global-mq-marshalling/library/include
    -isystem ${CMAKE_CURRENT_SOURCE_DIR}/include/nodecpp
    -isystem ${CMAKE_CURRENT_SOURCE_DIR}/include
    -isystem ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# set(DZ_DEFINES -DNDEBUG -DNODECPP_ENABLE_CLUSTERING)
set(DZ_DEFINES -DNDEBUG)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(DZ_FLAGS -std=c++17 -fcoroutines-ts)
else()
	set(DZ_FLAGS -std=c++17 -fcoroutines-ts -stdlib=libc++)
endif()


function(dz_add_executable ATARGET)

    get_target_property(TARGET_SOURCE_DIR ${ATARGET} SOURCE_DIR)
    get_target_property(TARGET_BINARY_DIR ${ATARGET} BINARY_DIR)
    get_target_property(TARGET_SOURCES ${ATARGET} SOURCES)

	foreach(EACH IN LISTS TARGET_SOURCES)

		get_filename_component(P ${EACH} DIRECTORY)
		get_filename_component(N ${EACH} NAME_WE)
		get_filename_component(E ${EACH} EXT)

#		string(CONCAT DZ_NAME ${TARGET_BINARY_DIR} "/" ${P} "/" ${N} ".dz" ${E})
# mb:use TARGET_SOURCE_DIR until we make local includes work
		string(CONCAT DZ_NAME ${TARGET_SOURCE_DIR} "/" ${P} "/" ${N} ".dz" ${E})
		string(CONCAT FULL_NAME ${TARGET_SOURCE_DIR} "/" ${EACH})

		add_custom_command(
		    OUTPUT  ${DZ_NAME}
		    COMMAND nodecpp-checker ${FULL_NAME} -- ${DZ_FLAGS} ${DZ_DEFINES} ${DZ_INCLUDES}
		    COMMAND nodecpp-instrument ${FULL_NAME} -o=${DZ_NAME} -- ${DZ_FLAGS} ${DZ_DEFINES} ${DZ_INCLUDES}
#			MAIN_DEPENDENCY ${FULL_NAME}
		)

		list(APPEND DZ_FILES ${DZ_NAME})

	endforeach()
	
    get_target_property(TARGET_NAME ${ATARGET} NAME)
    string(CONCAT DZ_TARGET_NAME ${TARGET_NAME} "-dz")
    
	add_executable(${DZ_TARGET_NAME} ${DZ_FILES})
	target_link_libraries(${DZ_TARGET_NAME} global-mq-nodecpp)

endfunction()

option(NODECPP_CHECK_AND_DZ_SAMPLES "Check and Dezombiefy samples" OFF)

if(NODECPP_TEST AND NODECPP_CHECK_AND_DZ_SAMPLES)
	dz_add_executable(basic_test)
endif()
