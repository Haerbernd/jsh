add_executable(jsh ./main.cpp)
target_link_libraries(jsh readline jsh_core haer_ctl_logging)


add_library(jsh_random SHARED ./rng.cpp)

set_target_properties(jsh PROPERTIES
        INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}"
)

install(
        TARGETS jsh
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
        DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT jshTargets
        FILE jshTargets.cmake
        NAMESPACE jsh::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/jsh
)