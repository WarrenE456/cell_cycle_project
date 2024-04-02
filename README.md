# OpenGL template wtih GLFW and GLAD
Setting up OpenGL can be annoying. This project offers a simple template for using OpenGL with GLFW and GLAD, along with CMake for building.

# How to use
If you are on linux (or an operating system that supports bash files), run the following bash files. Otherwise, open each file in a text editor and copy and paste the commands into the terminal.
```
./config.sh
./build.sh
./run.sh
```
To build the program in release, run these commands.
```
./config.sh release
./build.sh release
./run.sh release
```
config.sh runs CMake and generates the make file in out/build. build.sh runs the make file and generates the executable. run.sh runs the executable. Run config.sh when the CMakeLists.txt changes and build.sh when the source files change.

Source .c/.cpp files should go in ```src/``` and header .h/.hpp files should go in ```src/headers```. All dependencies go in ```external/``` and external headers can be put in ```include/```. All files in ```src/``` are added to the executable.

To change the project name open the CMakeLists.txt and change the project name where it says project([project name]). Also update the project_name variable in run.sh to match the CMakeLists.txt project name.
