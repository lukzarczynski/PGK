# CMake entry point
cmake_minimum_required(VERSION 2.8)
project (PGK)

find_package(OpenGL REQUIRED)

# Compile external dependencies
add_subdirectory (external)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	message(STATUS "Using clang.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
	-std=c++11
	-g
	-Wall
	-Wextra
	-Werror
)

if(APPLE)
	SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -framework QuartzCore -framework IOKit -framework Cocoa" )
endif()

include_directories(
	${GLFW_GLEW_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
)

if   (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(OPENGL_LIBRARY ${OPENGL_LIBRARY} -lGL -lGLU -lXrandr -lXext -lX11 -lrt)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

link_directories(
	${GLFW_GLEW_LIBRARY_DIRS}
)

# ZADANIE 2
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/zadanie2")
	add_executable(zadanie2
		zadanie2/main.cpp
		zadanie2/shader.cpp
		zadanie2/shader.hpp
		zadanie2/render.cpp
		zadanie2/render.hpp
		zadanie2/map.cpp
		zadanie2/map.hpp
		zadanie2/controls.cpp
		zadanie2/controls.hpp
		zadanie2/track.cpp
		zadanie2/track.hpp

		zadanie2/Map.fragmentshader
		zadanie2/Map.vertexshader
	)

	# Files needed by zadanie2 on runtime.
	set( ZAD2_RUNTIME_FILES
		zadanie2/Map.fragmentshader
		zadanie2/Map.vertexshader
	)

	if(GLFW_DEPS)
	add_dependencies(zadanie2 GLEW GLFW_303)
	endif()
	if(GLM_DEPS)
	add_dependencies(zadanie2 GLM)
	endif()


	target_link_libraries(zadanie2
		${GLFW_GLEW_LIBRARIES}
		${OPENGL_LIBRARY}
		${CMAKE_DL_LIBS}
	)

	# Copy the binary
	add_custom_command(
		TARGET zadanie2 POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/zadanie2${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/zadanie2/"
	)

	# Copy runtime files
	foreach( file ${ZAD2_RUNTIME_FILES})
	    add_custom_command(
		    TARGET zadanie2 POST_BUILD
		    COMMAND ${CMAKE_COMMAND}
		    ARGS -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${file}" "${CMAKE_CURRENT_BINARY_DIR}"
	)
	endforeach( file )
endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/zadanie2")
