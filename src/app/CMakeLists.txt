add_executable(mlp
    mlp_window.cc
    grid_scene.cc
    ../network/network.cc
    ../loader/loader.cc
    main.cc
)

target_link_libraries(mlp PRIVATE Qt5::Widgets)

# set_target_properties(mlp PROPERTIES
#     WIN32_EXECUTABLE ON
#     MACOSX_BUNDLE ON
# )
