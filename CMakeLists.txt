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
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/global-mq-marshalling/library/include)


add_library(global-mq-nodecpp STATIC
    src/q_based_infrastructure.cpp
    src/timeout_manager.cpp
    src/common.cpp
    src/fs.cpp
    # src/alt_comm_iface.cpp
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
    # no working tests currently
endif()


