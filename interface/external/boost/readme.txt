
Instructions for adding the Boost library to Interface
David Rowe, 6 Jul 2016

1.  Download the Boost version 1.57 library from https://sourceforge.net/projects/boost/files/boost-binaries/ and unzip or
    install.

    Though not strictly necessary, version 1.57 is used to match that used by the suggested OSVR SDK downloads linked to from 
    https://github.com/OSVR/OSVR-Docs/blob/master/Developing/Windows-Build-Environment.md.

2.  Copy the required Boost folders from the installation directory into the \interface\externals\boost folder. This readme.txt
    should be there as well.

    The folders needed are:
    - boost
    - lib64-msvc-12.0

   You may optionally choose to copy the folders to a location outside the repository. If so, set the ENV variable 
   'HIFI_LIB_DIR' to a directory containing a subfolder "boost" that contains the folders mentioned.
    
3. Clear your build directory, run cmake, and build.