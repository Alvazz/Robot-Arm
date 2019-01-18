project(tm_socket_test)

set(CMAKE_MODULE_PATH ${ZBARCV_SOURCE_DIR})

find_package( OpenCV REQUIRED )
find_library(UEYE_LIBRARY ueye_api)

set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")

add_executable(main src/tm_print.cpp src/tm_robot_state_rt.cpp src/tm_communication.cpp src/tm_driver.cpp main.cpp)

install(TARGETS main RUNTIME DESTINATION bin)
target_link_libraries( main ${OpenCV_LIBS} ${UEYE_LIBRARY} ${ZBAR_LIBRARIES} zbar )