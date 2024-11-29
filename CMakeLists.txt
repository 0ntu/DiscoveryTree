cmake_minimum_required (VERSION 3.11)
 
project(DiscoveryTree LANGUAGES CXX VERSION 1.0.0)
include(FetchContent)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(FTXUI REQUIRED)
 
add_executable(DiscoveryTree src/main.cpp src/fetcher.cpp)
target_link_libraries(DiscoveryTree
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
