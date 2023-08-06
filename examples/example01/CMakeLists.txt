project(example01)

find_package(Qt5 REQUIRED COMPONENTS Sql)

#Headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

#Sources
file(GLOB HDRS ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)
file(GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

#Wrapping MOC Objects
qt5_wrap_cpp(MOC_SRCS ${HDRS})

include_directories(${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/exporter/inc
    ${CMAKE_SOURCE_DIR}/src/importer/inc)

link_directories(${CMAKE_SOURCE_DIR}/lib)

#Defining Executable
add_executable(${PROJECT_NAME} ${SRCS} ${MOC_SRCS})
target_link_libraries(${PROJECT_NAME} ImportExport${BUILD_POSTFIX})
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Sql)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}${BUILD_POSTFIX})
