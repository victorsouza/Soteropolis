add_executable(Soteropolis main.cpp Engine.cpp Utils.cpp Voronoi2D.cpp stdafx.cpp)

target_link_libraries(Soteropolis
    ${CGAL_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    ${GLEW_LIBRARY_RELEASE}
    ${OPENGL_opengl_LIBRARY}
    SDL2)

