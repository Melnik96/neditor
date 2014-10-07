include_directories("../")
include_directories("../ngine/")

add_executable(neditor source/main.c)
target_link_libraries(neditor
  /usr/local/lib/libngine_core.so
  /usr/local/lib/libngine_scenegraph.so
  /usr/local/lib/libngine_render.so
  /usr/local/lib/libngine_import.so
  pthread
  glfw
)
