# Project libclio
A lightweight c++ logging library.

From Greek mythology Clio is the muse of history, a very fitting name for a logging library.

## Building
### Linux

You will need the following packages:
~~~~
$ sudo apt-get install build-essential autoconf libtool
~~~~

Then run the following to build:
~~~~
$ autoreconf -vfi
$ ./configure
$ make
~~~~

For cross compilation pass in your toolchain via HOST option:
~~~~
$ autoreconf -vfi
$ ./configure --host=arm-linux-gnueabihf
$ make
~~~~

You can also build a debian package if you would prefer. You will need the following packages:
~~~~
$ sudo apt-get install fakeroot devscripts dh-autoreconf
~~~~

Then you can build a dpkg directly:
~~~~
$ dpkg-buildpackage -uc -us
~~~~

### Windows

Visual Studio projects and solutions are provided. Simply open them up and build.

I use Visual Studio 2019 (the free one) and it works fine.

## Documentation

For documentation you will need the following packages:
~~~~
$ sudo apt-get install doxygen
~~~~

If you want PDF/PS support install these packages too (its quite large):
~~~~
$ sudo apt-get install texlive texlive-latex-extra
~~~~

Then run the following to generate all doxygen documentation:
~~~~
$ make doxygen-doc
~~~~

If you only want HTML output instead run the following:
~~~~
$ make doxygen-html
~~~~
