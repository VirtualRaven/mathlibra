# README #

### What is this repository for? ###
Math core is the core static library for the calculator software which all other products are built upon. It provides the basic functionality of the calculator software. It is a framework for the calculator which implements core functions as as lexical analysis, abstract syntax tree construction and tree evaluation and exposes an plug-in api to extend functions 

 
### Math core is further split into some submodules.###

* Core, the most basic code. Implements the lexcial ananalysis, tree construction, tree evaluation, and defines data structures.
* Modules, functionality as variables, runtime functions and operators are provided by the modules. These modules can be switched and serves as internal plug-ins
* Plug-ins, To expose the functionality of the function module to other users the plug-in subsystem defines ways of loading external dynamic libraries. The plug-in subsystem is also responsible for plug-in meta-data and  version handling. 


### How do I get set up? ###

The core have no dependency and as such should build on most platforms.
It requires compiler support for c++11.
If core is built for any other platform than windows or linux plug ins will be disabled as currently. Do enable plug-ins on these to platforms the program most be built with LINUX or WINDOWS preprocessor definitions.The core has no loader for other files than windows DLL.

On windows the core can be compiled visual studio 2013. The solution has build scripts to include the git version inside the binary and export built release binaries to the folder build_bin inside your document folder. These scripts depend on the VS 3rd party git tools. If not available these build scripts needs to be disabled.

On Linux use the included makefile. Inside the source directory write "make" to compile it. Type "make clean" to clean the repository. Type "make debug=true" to compile with symbols.

On other platforms, compile everything into a static library and enable exceptions and c++11

### Contribution guidelines ###



Style
Try to follow the overall code style of the project

brackets are expanded in all context. After an if statement or function definition a newline is to be used. Do not add ad an opening bracket on the same line.
When using brackets indent the code using one tab.

```
#!c++
if(true)
{
do_work();
}

```

### Who do I talk to? ###

Owner

l [dot] rahmn [at] gmail [dot] com