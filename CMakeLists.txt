cmake_minimum_required (VERSION 3.11)
 
include(FetchContent)
# set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)
 
project(DiscoveryTree LANGUAGES CXX VERSION 1.0.0)
add_executable(DiscoveryTree src/main.cpp)
target_link_libraries(DiscoveryTree
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
