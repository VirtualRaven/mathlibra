# README #
 

### What is this repository for? ###

C# Interface DLL 

* Provides an wrapper for the native calculator library exposing an mostly stable api. It provides all the functions from core in the form of one single class. This class manages all other helper classes from the core and takes care of the library initializations. 

* This wrapper class is further extended by the managed class with wrapped it in a C# compatible interface and takes care of exception handling such automatically to expose an easy interface to the end user.

* Needs to be linked against the static core calculator library

###Setup ###
This library is only tested for windows and support for other platforms is unknown. It requires support for managed c++ and c++11  to compile.
Build scripts are dependent upon VS 3rd party git tools. If unnavigable these scripts need to be disabled