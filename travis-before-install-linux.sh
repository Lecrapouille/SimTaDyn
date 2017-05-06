sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y || exit 1

sudo apt-get update || exit 1

sudo apt-get install -y freeglut3 freeglut3-dev mesa-common-dev libgtkglextmm-x11-1.2-dev libgtkmm-2.4-dev libgtksourceviewmm-3.0-dev libglu1-mesa-dev libglew-dev libcppunit-dev gcovr libdw-dev || exit 1


if [ $CXX == "g++" ] ; then
	sudo apt-get install -y gcc-4.9 g++-4.9 || exit 1
	sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.9 50 || exit 1
	sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 50 || exit 1
elif [ $CXX == "clang++" ] ; then
	sudo apt-get install -y clang || exit 1
fi
