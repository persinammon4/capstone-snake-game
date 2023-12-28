# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/monica/capstone-snake-game/build/cryptopp-cmake/cryptopp"
  "/home/monica/capstone-snake-game/build/_deps/cryptopp-build"
  "/home/monica/capstone-snake-game/build/_deps/cryptopp-subbuild/cryptopp-populate-prefix"
  "/home/monica/capstone-snake-game/build/_deps/cryptopp-subbuild/cryptopp-populate-prefix/tmp"
  "/home/monica/capstone-snake-game/build/_deps/cryptopp-subbuild/cryptopp-populate-prefix/src/cryptopp-populate-stamp"
  "/home/monica/capstone-snake-game/build/_deps/cryptopp-subbuild/cryptopp-populate-prefix/src"
  "/home/monica/capstone-snake-game/build/_deps/cryptopp-subbuild/cryptopp-populate-prefix/src/cryptopp-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/monica/capstone-snake-game/build/_deps/cryptopp-subbuild/cryptopp-populate-prefix/src/cryptopp-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/monica/capstone-snake-game/build/_deps/cryptopp-subbuild/cryptopp-populate-prefix/src/cryptopp-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
