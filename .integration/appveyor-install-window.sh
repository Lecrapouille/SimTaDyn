#!/bin/bash

packages=""
function mingw_packages
{
    arch=x86_64
    if [ "$PLATFORM" == "x86" ];
    then
        arch=i686
    fi

    prefix="mingw-w64-${arch}-"
    for i in $1
    do
        packages="$prefix""$i "$packages
    done
}

mingw_packages "cmake toolchain clang gtkmm3 gtksourceviewmm3 cppunit glm glew glfw"
echo ">>>>>>>>>>>>>>>>>$packages"
sh -c "pacman -S --noconfirm git make $packages"
