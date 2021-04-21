add_library(libminui INTERFACE)

target_sources(libminui INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/events.cpp
        ${CMAKE_CURRENT_LIST_DIR}/graphics.cpp
#        ${CMAKE_CURRENT_LIST_DIR}/graphics_adf.cpp
        ${CMAKE_CURRENT_LIST_DIR}/graphics_drm.cpp
        ${CMAKE_CURRENT_LIST_DIR}/graphics_fbdev.cpp
        ${CMAKE_CURRENT_LIST_DIR}/resources.cpp
        )

target_include_directories(libminui INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include/
        /usr/include/libdrm
        )

target_link_libraries(libminui INTERFACE
        drm
        png
        )
