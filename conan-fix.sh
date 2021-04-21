#!/usr/bin/env bash
BASEDIR=$(dirname "$0")


mkdir -p build

# Download the most recent 
curl "https://raw.githubusercontent.com/bincrafters/community/main/recipes/sdl2/all/conanfile.py" > "${BASEDIR}/build/conan_sdl2.py"
curl "https://raw.githubusercontent.com/bincrafters/community/main/recipes/sdl2_ttf/all/conanfile.py" > "${BASEDIR}/build/conan_sdl2ttf.py"

conan remote add bincrafters "https://api.bintray.com/conan/bincrafters/public-conan"
conan install sdl2/2.0.14@bincrafters/stable
conan install sdl2_ttf/2.0.15@bincrafters/stable

\cp ${BASEDIR}/build/conan_sdl2.py ~/.conan/data/sdl2/2.0.14/bincrafters/stable/export/conanfile.py
cmake -H. -Bbuild -GNinja

\cp ${BASEDIR}/build/conan_sdl2ttf.py ~/.conan/data/sdl2_ttf/2.0.15/bincrafters/stable/export/conanfile.py 
cmake -H. -Bbuild -GNinja

cd ${BASEDIR}/build
ninja
