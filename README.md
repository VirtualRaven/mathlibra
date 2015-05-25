# README #

This README would normally document whatever steps are necessary to get your application up and running.

### What is this repository for? ###

* This is the tester library for the core, it serves as an test client which implements most  current functions as an console program. It contains all the code to start initialize the core, it also contains performance tests which can be run to see how changes in the core affects the overall performance. 

* Tests for the core should be added into this project.

* calculator-core-tester i a great tool to easily debug the core library without any unnecessary wrapper and interfaces in between. It provides raw access to the whole of the library. Thus any type of test or debugging can easily be implemented

* This is just a tester library and is under no circumstances to be considers a complete application and as such can never be considers stable.

### How do I get set up? ###

Compile the link with core and compile into an executable. If on non windows platform check if get_platform_specific_manager returns a null pointer. If so, your platform is not supported by the core plug-in loader.

Solution files for VS 2013 are included into the repo. These have build scripts that depend on VS 3rd party git tools. If unavailable please disable the scripts.

For linux please use the included make file. This make file automatically imports libcore.a which is the resulting file from compilation of core. This file is imported from "../../calculator-core/CalcLib/" so put the library there.

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

### Who do I talk to? ###

Owner
l dot rahmn [at] gmail dotcom