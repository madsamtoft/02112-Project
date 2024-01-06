# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/mads/esp/esp-idf/components/bootloader/subproject"
  "/Users/mads/Desktop/Git/02112/Exercises/Exercise11-Timers/High-resolution_timer/build/bootloader"
  "/Users/mads/Desktop/Git/02112/Exercises/Exercise11-Timers/High-resolution_timer/build/bootloader-prefix"
  "/Users/mads/Desktop/Git/02112/Exercises/Exercise11-Timers/High-resolution_timer/build/bootloader-prefix/tmp"
  "/Users/mads/Desktop/Git/02112/Exercises/Exercise11-Timers/High-resolution_timer/build/bootloader-prefix/src/bootloader-stamp"
  "/Users/mads/Desktop/Git/02112/Exercises/Exercise11-Timers/High-resolution_timer/build/bootloader-prefix/src"
  "/Users/mads/Desktop/Git/02112/Exercises/Exercise11-Timers/High-resolution_timer/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/mads/Desktop/Git/02112/Exercises/Exercise11-Timers/High-resolution_timer/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/mads/Desktop/Git/02112/Exercises/Exercise11-Timers/High-resolution_timer/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
