/*
This will always generate a compile time error message from the pre-processor.
It is okay to include more than once.  Hence, no include guards
*/

#error The build target must be defined at compile time. \
       Please define the BUILD_ROVER or BUILD_SURFACE \
       preprocessor macros.  Thise can be accomplished from the \
       arduino-cli command line using the --build-property \
       command line argument.  A sample build command would \
       look something like this \
           $ arduino-cli compile --fqbn arduino:avr:mega --build-property compiler.cpp.extra_flags=-DBUILD_ROVER --libraries libraries/
