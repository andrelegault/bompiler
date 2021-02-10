cmake_minimum_required(VERSION 3.10)

project(bompiler VERSION 0.1.0 HOMEPAGE_URL https://github.com/andrelegault/bompiler.git)

set(CMAKE_INSTALL_PREFIX build)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(BOMPILER_SOURCE_DIR src)
set(BOMPILER_INCLUDE_DIR include)
set(BOMPILER_LIBS_DIR libs)
set(BOMPILER_MAIN Driver.cpp)
#set(BOMPILER_LIBRARIES gtest)

configure_file(${BOMPILER_INCLUDE_DIR}/bompilerConfig.h.in ${BOMPILER_INCLUDE_DIR}/bompilerConfig.h)

foreach(LIBRARY ${BOMPILER_LIBRARIES})
    add_subdirectory(${BOMPILER_LIBS_DIR}/${LIBRARY})
endforeach(LIBRARY)

# generate an executable called ``bompiler`` that's based off ``Driver.cpp``
add_executable(lexdriver ${BOMPILER_SOURCE_DIR}/${BOMPILER_MAIN} ${BOMPILER_SOURCE_DIR}/LexicalAnalyzer.cpp ${BOMPILER_SOURCE_DIR}/Token.cpp ${BOMPILER_SOURCE_DIR}/Utils.cpp)

# propagate that this project needs the gtest library (optional)
#target_link_libraries(bompiler PUBLIC gtest)

# include the headers
include_directories(${PROJECT_SOURCE_DIR}/${BOMPILER_INCLUDE_DIR})

# make the bompiler's binary dir visible
target_include_directories(lexdriver PUBLIC "${PROJECT_BINARY_DIR}")

# redirect built thing to /bin
install(TARGETS lexdriver DESTINATION bin)

# redirect required headers to /include
install(FILES ${PROJECT_BINARY_DIR}/${BOMPILER_INCLUDE_DIR}/bompilerConfig.h DESTINATION include)

enable_testing()
find_package(GTest REQUIRED)
target_link_libraries(lexdriver GTest::GTest GTest::Main)

# create platform-specific installers (w/ ``cpack``)
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${bompiler_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${bompiler_VERSION_MINOR}")
include(CPack)