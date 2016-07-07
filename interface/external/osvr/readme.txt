
Instructions for adding the OSVR library to Interface
David Rowe, 6 Jul 2016

1.  Download the 64-bit OSVR SDK from http://access.osvr.com/binary/osvr-sdk-installer and install.

2.  Copy the required OSVR SDK folder contents from the installation directory C:\Program Files\OSVR\SDK into the 
    \interface\externals\osvr folder. This readme.txt should be there as well.

    The folders needed are:
    - bin: *.dll
    - include: all, including subfolders
    - lib: *.lib

   You may optionally choose to copy the folders to a location outside the repository. If so, set the ENV variable 
   'HIFI_LIB_DIR' to a directory containing a subfolder "osvr" that contains the folders mentioned.
    
3. Clear your build directory, run cmake, and build.