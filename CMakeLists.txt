# CMakeList.txt : CMake project for poc-opencv, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("poc-opencv")

# Find required packages
find_package(fmt CONFIG REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

# Add source to this project's executable.
add_executable (poc-opencv "poc-opencv.cpp" "poc-opencv.h" "ImageTransformer.cpp" "ImageTransformer.h" "UIWindow.h" "UIWindow.cpp")

# Target needed libraries
target_link_libraries(poc-opencv PRIVATE fmt::fmt)
target_link_libraries(poc-opencv PRIVATE ${OpenCV_LIBS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET poc-opencv PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
