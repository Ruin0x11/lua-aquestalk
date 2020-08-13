cmake_minimum_required(VERSION 2.8)

set(LINUX ${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(LINUX)
    # On Linux the AquesTalk10 library was compiled with GCC as a C++ library, so
    # it expects GCC's exception unwinder to be present (__gxx_personality_v0).
    # Due to this we have to compile the code as C++ under GCC so it gets
    # linked with its libstdc++.
    set(CMAKE_C_COMPILER "gcc")
    set(CMAKE_CXX_COMPILER "g++")
    project(lua-aquestalk CXX)
    set_source_files_properties(lua-aquestalk.c PROPERTIES LANGUAGE CXX )
else()
    project(lua-aquestalk C)
endif()

set(BUILD_SHARED_LIBS TRUE)

if (MSVC)
    set(LUA_BUILD_AS_DLL TRUE)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lua")

    set(LUA_LIBRARIES liblua)
    set(LUA_INCLUDE_DIR ${lua_SOURCE_DIR}/src)
    set(LUA_BINARY_DIR ${lua_BINARY_DIR})

	if("${CMAKE_GENERATOR_PLATFORM}" MATCHES "x64")
	   set(AQUESTALK_LIB_DIR "lib64")
	else()
	   set(AQUESTALK_LIB_DIR "lib")
	endif()
    find_library(AQUESTALK_LIB AquesTalk PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/aqtk10_win/${AQUESTALK_LIB_DIR}" REQUIRED)
    find_path(AQUESTALK_INCLUDES AquesTalk.h PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/aqtk10_win/${AQUESTALK_LIB_DIR}" REQUIRED)
elseif(LINUX)
    find_package(Lua "5.1" EXACT REQUIRED)

	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	   set(AQUESTALK_LIB_DIR "lib64")
	else()
	   set(AQUESTALK_LIB_DIR "lib32")
	endif()
    find_library(AQUESTALK_LIB libAquesTalk10.so.1.1 PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/aqtk10_lnx/${AQUESTALK_LIB_DIR}" REQUIRED)
    find_path(AQUESTALK_INCLUDES AquesTalk.h PATHS "${CMAKE_CURRENT_SOURCE_DIR}/lib/aqtk10_lnx/${AQUESTALK_LIB_DIR}" REQUIRED)
endif()

set(WARN_CFLAGS "-Wall -Wextra -Wshadow -W -pedantic -Wno-overlength-strings")
if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe ${WARN_CFLAGS} -std=gnu99 -fgnu89-inline")
    set(CMAKE_C_FLAGS_RELEASE        "${CMAKE_C_FLAGS_RELEASE}     -O3 -march=native -g")
    set(CMAKE_C_FLAGS_DEBUG          "${CMAKE_C_FLAGS_DEBUG}       -O0 -g")
    set(CMAKE_C_FLAGS_PROFILE        "${CMAKE_C_FLAGS_PROFILE}     -O2 -g -DNDEBUG")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_WITHDEBINFO} -O2 -g")
endif(CMAKE_COMPILER_IS_GNUCC)

## setup git submodules

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${AQUESTALK_INCLUDES}
                    ${LUA_INCLUDE_DIR}
                    ${LUA_BINARY_DIR})

add_library(lua-aquestalk MODULE lua-aquestalk.c)
target_link_libraries(lua-aquestalk ${LUA_LIBRARIES})
target_link_libraries(lua-aquestalk ${AQUESTALK_LIB})
if (LINUX)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
endif()
set_target_properties(lua-aquestalk PROPERTIES PREFIX "")
set_target_properties(lua-aquestalk PROPERTIES COMPILE_FLAGS "${CFLAGS}")
set_target_properties(lua-aquestalk PROPERTIES OUTPUT_NAME lua_aquestalk)

add_custom_command(TARGET lua-aquestalk POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${AQUESTALK_LIB}
    $<TARGET_FILE_DIR:lua-aquestalk>)

if(LINUX)
    # CMake has no way of telling the linker to link against lib.so.1.1 if
    # there is no symlink to that library. It always links against lib.so.1.

    add_custom_command(TARGET lua-aquestalk POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        $<TARGET_FILE_DIR:lua-aquestalk>/libAquesTalk10.so.1.1
        $<TARGET_FILE_DIR:lua-aquestalk>/libAquesTalk10.so.1)
endif()

## Setup test stuff
include(CTest)
enable_testing()
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
list(APPEND CMAKE_CTEST_ARGUMENTS "--verbose")
add_test(test ${lua_BINARY_DIR}/lua.exe ${CMAKE_CURRENT_SOURCE_DIR}/test.lua ${CMAKE_CURRENT_SOURCE_DIR}/ ${CMAKE_BINARY_DIR}/Release/)
#add_test(benchmark ${LUA} ${CMAKE_CURRENT_SOURCE_DIR}/benchmark.lua ${CMAKE_CURRENT_SOURCE_DIR}/ ${CMAKE_BINARY_DIR}/Release/)
set_tests_properties(test
                     PROPERTIES
                     FAIL_REGULAR_EXPRESSION
                     "not ok")
