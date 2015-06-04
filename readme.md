#README#
This is the repo for mathlibra, an calculator backend library that parsers and interprets mathematical expressions. It is designed to serve as an backend for developing calculator software.

### How do I get set up? ###

Mathlibra have no dependency and as such should build on most platforms, though it requires compiler support for c++11.
If core is built for any other platform than windows or linux plug ins will be disabled as currently. Do enable plug-ins on these to platforms the program most be built with LINUX or WINDOWS preprocessor definitions.The core has no loader for other files than windows DLL and linux shared libraries.

On windows the core can be compiled visual studio 2013. The solution has build scripts to include the git version inside the binary and export built release binaries to the folder build_bin inside your document folder. These scripts depend on the VS 3rd party git tools. If not available these build scripts needs to be disabled.

On Linux use the included makefile. Inside the source directory write "make" to compile it. Type "make clean" to clean the repository. Type "make DEBUG=true" to compile with symbols. Make files are included


### Contribution guidelines ###



Style
Try to follow the overall code style of the project

brackets are expanded in all context. After an if statement or function definition a newline is to be used. Do not add ad an opening bracket on the same line.
When using brackets indent the code using one tab. 

Example

* Correct
```
#!c++
if(true)
{
   do_work();
}

```

* Incorrect
```
#!c++
if(true){
   do_work();
}
```
 
An important trait of mathlibra is that it has no external dependecys and that it is platform independent. Any code breaking these traits will not be accepted. For example, do not write code that requiires the c++ boost library. Any functions needed for mathlibra not provided by the standard c library or the standard template library should be implemented inside of the project.
An exception has been made to platform independet rule regarding the plug-in system which calls platform specific function for loading shared libaries, but it does so by hiding this functions from the rest of the library and implements an generic interface for loadinng external code. I this way the plug-in loader supports both unix and windows.

Code seperation is also important. For example the code dealing with the external api implementation should be seperated into the wrapper folder. Mathematical runtime functions should not be implemented inside mathlibra, instead they should be provided as plug-ins.

### Who do I talk to? ###

Owner

l [dot] rahmn [at] gmail [dot] com