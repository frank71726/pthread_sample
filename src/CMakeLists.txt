#ADD_SUBDIRECTORY(lib)

#ADD_LIBRARY(RPI_LED_LIB PJ_RPI.c)

MESSAGE("1111111111111111111111111111111111")
find_package(Threads REQUIRED)
MESSAGE("2222222222222222222222222222222222")
add_executable(Pthread main.c llist.c)
target_link_libraries(Pthread ${CMAKE_THREAD_LIBS_INIT})

#target_link_libraries(LedBlink RPI_LED_LIB)

install(TARGETS Pthread DESTINATION bin)
