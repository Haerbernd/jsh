add_library(jsh_core SHARED src/builtins.cpp src/os.cpp src/completion.cpp)
target_link_libraries(jsh_core readline)

install(
        TARGETS jsh_core
        EXPORT jshTargets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)