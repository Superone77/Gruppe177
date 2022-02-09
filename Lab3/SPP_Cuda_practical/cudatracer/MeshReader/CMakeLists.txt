
add_library(MeshReaderLib list.cpp
MeshFileReader.cpp
objLoader.cpp
obj_parser.cpp
rtmLoader.cpp
string_extra.cpp
string_util.cpp
)

#add_subdirectory(alglib)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(BoundSphereLib PUBLIC OpenMP::OpenMP_CXX)
endif()

#target_link_libraries(SimAnMoLib PUBLIC 
#	AlgLib	
#)

target_include_directories(MeshReaderLib PUBLIC
   "${PROJECT_BINARY_DIR}/CUDATracing"
#	${PROJECT_SOURCE_DIR}/SimulatedAnnealingExtraP
                           )
