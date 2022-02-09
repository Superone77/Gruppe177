
add_library(BoundSphereLib BoundSphereCreator.cpp
Miniball.cpp
Miniball_b.cpp)

#add_subdirectory(alglib)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(BoundSphereLib PUBLIC OpenMP::OpenMP_CXX)
endif()

#target_link_libraries(SimAnMoLib PUBLIC 
#	AlgLib	
#)

target_include_directories(BoundSphereLib PUBLIC
   "${PROJECT_BINARY_DIR}/CUDATracing"
#	${PROJECT_SOURCE_DIR}/SimulatedAnnealingExtraP
                           )
