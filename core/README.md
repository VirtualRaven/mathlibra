# Core folder #

### What is in this folder? ###
This folder contains all the code for the core functionality of mathlibra. All code except external wrappers and tester tools are located in this folder.

### Core is further split into some submodules.###

* Core, the most basic code. Implements the lexcial ananalysis, tree construction, tree evaluation, and defines data structures.
* Modules, functionality as variables, runtime functions and operators are provided by the modules. These modules can be switched and serves as internal plug-ins
* Plug-ins, Provides a plugin-loader and APIs for external plug-ins.The plug-in subsystem is also responsible for plug-in meta-data and  version handling. 

