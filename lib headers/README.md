# README #

### What is this repository for? ###
Math core is the core static library for the calculator software which all other products are built upon.
It provides the basic functionality of the calculator software. It provides a framework for the calculator and implement core functions as as interpretation and calculation and exposes an plug-in api to extend functions 



### How do I get set up? ###

The core have no dependency and as such should build on most platforms.
It requires compiler support for c++11.
If core is built for any other platform than windows plugins will be dissabled as currently
the core has no loader for other files than windows DLL.

On windows the core can be compiled visual studio 2013. The solution has build scripts to include the git version inside the binary and export built release binaries to the folder build_bin inside your document folder. These scripts depend on the VS 3rd party git tools. If not available these build scripts needs to be disabled.

On other platforms, compile everything into a static library and enable exceptions and c++11

### Contribution guidelines ###



Style
Try to follow the overall code style of the project

brackets are expanded in all contexts
example

Correct:
if(statement)
{
   do_work();
}

Incorrect
if(statement){
 do_work();
}


### Who do I talk to? ###

Owner

l.rahmn [at] gmail.com