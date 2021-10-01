# Create a library which includes the source files
# Any number of sources could be listed here.
add_library (GPS GPS.cpp minmea.c)

# Make sure the compiler can find include files for our library
# when other libraries or executables link to it
target_include_directories (GPS PRIVATE .)