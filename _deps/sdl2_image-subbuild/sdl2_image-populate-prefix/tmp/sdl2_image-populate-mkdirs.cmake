# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/cyantus/code/oopl/BloonTD2-OOPL/PTSD/lib/sdl2_image")
  file(MAKE_DIRECTORY "/home/cyantus/code/oopl/BloonTD2-OOPL/PTSD/lib/sdl2_image")
endif()
file(MAKE_DIRECTORY
  "/home/cyantus/code/oopl/BloonTD2-OOPL/_deps/sdl2_image-build"
  "/home/cyantus/code/oopl/BloonTD2-OOPL/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix"
  "/home/cyantus/code/oopl/BloonTD2-OOPL/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/tmp"
  "/home/cyantus/code/oopl/BloonTD2-OOPL/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/src/sdl2_image-populate-stamp"
  "/home/cyantus/code/oopl/BloonTD2-OOPL/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/src"
  "/home/cyantus/code/oopl/BloonTD2-OOPL/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/src/sdl2_image-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/cyantus/code/oopl/BloonTD2-OOPL/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/src/sdl2_image-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/cyantus/code/oopl/BloonTD2-OOPL/_deps/sdl2_image-subbuild/sdl2_image-populate-prefix/src/sdl2_image-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
