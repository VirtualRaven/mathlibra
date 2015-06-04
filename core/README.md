# README #

### What is this repository for? ###
core is the an static library which implements all of mathlibra's functionality.Mathlibra is a back-end library for developing calculator software which implements core functions as as lexical analysis, abstract syntax tree construction and tree evaluation and exposes an plug-in api to extend functions. 

This folder contains all library related code. Unit tests and external interfaces should be included in the other folders.

### Math core is further split into some submodules.###

* Core, the most basic code. Implements the lexcial ananalysis, tree construction, tree evaluation, and defines data structures.
* Modules, functionality as variables, runtime functions and operators are provided by the modules. These modules can be switched and serves as internal plug-ins
* Plug-ins, To expose the functionality of the function module to other users the plug-in subsystem defines ways of loading external dynamic libraries. The plug-in subsystem is also responsible for plug-in meta-data and  version handling. 

