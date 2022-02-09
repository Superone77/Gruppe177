cmake_minimum_required(VERSION 3.13)

# set the project name and version
project(CUDATracer VERSION 1.5)

# Initialize for C and C++ languages.
project(cmake_and_cuda)
# Also, enable CUDA language support.
enable_language(CUDA)

# Add new build types
message("* Adding build types...")
SET(CMAKE_CXX_FLAGS_ULTRAFAST
    "-ofast -DNDEBUG"
    CACHE STRING "Flags used by the C++ compiler during coverage builds."
    FORCE 
)

#set(CMAKE_BUILD_TYPE "ULTRAFAST1")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# configure a header file to pass some of the CMake settings
# to the source code
#configure_file(SimulatedAnnealingExtraP/LibraryInterface.h.in SimulatedAnnealingExtraP/VersionInfo.h)

#list(APPEND EXTRA_LIBS AlgLib)
list(APPEND EXTRA_LIBS BoundSphereLib MeshReaderLib)
list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR}/BoundSphere 
	${PROJECT_SOURCE_DIR}/MeshReader
	${PROJECT_SOURCE_DIR}/CUDATracing)

# add the libraries
add_subdirectory(BoundSphere)
add_subdirectory(MeshReader)

# add the executable
add_executable(CUDATracer "")
add_subdirectory(CUDATracing)


find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(CUDATracer PUBLIC OpenMP::OpenMP_CXX)
endif()

target_link_libraries(CUDATracer PUBLIC 
	BoundSphereLib
	MeshReaderLib
)

message(STATUS "foo include dir: ${CMAKE_BUILD_TYPE}")

# add the binary tree to the search path for include files
target_include_directories(CUDATracer PUBLIC
                           "${PROJECT_BINARY_DIR}"
							${PROJECT_SOURCE_DIR}/CUDATracing
                           ${PROJECT_SOURCE_DIR}/BoundSphere
							${PROJECT_SOURCE_DIR}/MeshReader
 )
