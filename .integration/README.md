## Continuous integration

Continuous integration (CI) tools, in the case of SimTaDyn project, is a pratice for building code source on different architectures (Linux, OS X, Window) and run unit tests for each git commit or github pull requests. This allows:
* to detect code regressions (running unit tests or static code analysis).
* to detect compilation regressions.
* to compile the project on several different architectures with different kind and version of compilators.

SimTaDyn uses five tools:
* [Travis-CI](https://travis-ci.org/) for launching compilation on Linux and OS X (file ../.travis.yml).
* [Appveyor](https://www.appveyor.com/) for launching compilation on Windows (file ../appveyor.yml).
* [Coverity Scan](https://scan.coverity.com/) for performing static code analysis (file ../Makefile).
* [Coveralls](https://coveralls.io/) for displaying code coverage (file ../.travis.yml).
* [OpenSUSE Build Service](https://build.opensuse.org/) (also named OBS) for generating RPM and deb packages (file ../Makefile).

Coverity Scan is called manually:
```
cd .. && make coverity-scan
```

As well as OBS:
```
cd .. && make obs
```

Travis-CI and Appveyor are triggered on each git commit or github pull request. On success, Travis-CI will trigger Coveralls.

__Note:__ Travis-CI only uses Ubuntu 14.04 which is an outdated version of Ubuntu. Therefore, a docker is used with Travis-CI with Ubuntu 17.04 in the aim to have the correct version of gtkmm needed by SimTaDyn (>= 3.22).

## Files

In this directory, here the description of the following files (grouped as theme):

##### Travis-CI

* travis-install-linux.sh: bash script called by the Travis-CI ../.travis.yml file for installing Ubuntu packages, needed by SimTaDyn. Indeed, Ubuntu runs inside a docker with an empty Ubuntu distribution.
* travis-install-osx.sh: bash script called by the Travis-CI ../.travis.yml file for installing OSX packages needed by SimTaDyn.
* travis-launch_tests.sh: bash script called by the Travis-CI ../.travis.yml file for launching the SimTaDyn compilation and running unit tests (for Linux and OSX).
* travis-deploy.sh: if travis-launch_tests.sh success: perform some specific deployment tasks like calling coveralls (can also launch OBS).

##### Appveyor

appveyor-install-window.sh: bash script called by the Appveyor ../appveyor.yml file for installing Windows packages needed by SimTaDyn.
appveyor-launch_tests.sh: bash script called by the Travis-CI ../appveyor.yml file for launching the SimTaDyn compilation and running unit tests for Windows.

##### OpenSUSE Build Service (OBS)

* opensuse-build-service.sh: bash script for commiting source files into OBS and starting building the newly package.
* SimTaDyn.spec: file needed by OBS for building RPM packages.

Note: This will use my OBS repo https://build.opensuse.org/package/show/home:Lecrapouille/SimTaDyn protected by my own password.
Therefore adapt this script for your own repo.
