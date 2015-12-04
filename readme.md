[![Build Status](https://travis-ci.com/VirtualRaven/mathlibra.svg?token=CLuwuoYsSeFx17TVonxH&branch=master)](https://travis-ci.com/VirtualRaven/mathlibra)

#README#
This is the repo for mathlibra, an calculator backend library that parsers and interprets mathematical expressions. It is designed to serve as an backend for developing calculator software.

### How do I get set up? ###

Mathlibra have no dependency and as such should build on most platforms, though it requires compiler support for c++11.
If core is built for any other platform than windows or linux plug ins will be disabled as currently. 
The core has no loader for other files than windows DLL and linux shared libraries.

The project is easiest to build using cmake. Out of source builds are recommended but not enforced so be careful

Build commands for linux:
 `cd /path/to/repo/`
 `mkdir build`
 `cd build`
 `cmake ..`
 `make`

On Windows cmake can be used to create visual studio files. Install cmake for windows and follow the instructions.

For vim users all directories include config files for youcomplete me to enable autocompletion. To just install youcompleteme and when opening an file in the repo accept when it asks if you want to load the config files.

### Running Tests ###
The project includes an random tester which generates test cases. The tester works by generating random expressions and passing them to mathlibra through the C api. The result is then compared to the calculated expected result, if they differ or a exception is thrown, the test fails. To enable the tester use the command `cmake -DC_API_TEST=True ..` when configuring cmake  and then to run the tester with `make test`. If the tester fails it will generate an report `test/test_report.txt` in the build folder.

The project contains a tester executable which when run provides an interactive sessions for inputting expressions to be evaluated.
Writing menu instead of an expression will bring up an menu which can run tests, provide help and lets you inspect the status of the interpreter at any time.

### Contribution guidelines ###

First and foremost: This is an educational project, we are here to learn and have fun. If it happens to turn out to something awesome that is a nice bonus.


Style
Try to follow the overall code style of the project.

Brackets are expanded in all context. After an if statement or function definition a newline is to be used. Do not add ad an opening bracket on the same line.
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
All headers should only contain declarations and no implementations (but template are ok :) ). Headers providing abstract classes to be used an base classes and interfaces should be named classname_interface.h to clearly state it's content.

Comment in headers should be doxygen compatible using the java syntax. For more help see the doxygen homepage


```c++
/**
 *func, My awsome function. An very awsome function.
 *@param name The name of the entity to tests the awsomeness of.
 *@returns integer representing it's awsomeness   
 */
 int func(std::string name);
```

At the moment, cmake is not configured to generate documentations and thus it has to be done manually. Feel free to reconfigure cmake for doc gen.

An important trait of mathlibra is that it has no external dependencies and that it is platform independent. Any code breaking these traits will not be accepted. For example, do not write code that requires the c++ boost library. Any functions needed for mathlibra not provided by the standard c library or the standard template library should be implemented inside of the project.
The reason behind this is that this is as stated above ab educational project, there are no time limits or deadlines to adhere to. So we have time to implement our own functions and learn something more that way. Implementing stuff by yourself may not always be the best option, but it is an great opportunity to learn, so for your own sake please follows these recommendations.  
An exception has been made to platform independent rule regarding the plug-in system which calls platform specific function for loading shared libraries, but it does so by hiding this functions from the rest of the library and implements an generic interface for loading external code. In this way the plug-in loader supports both unix and windows.

Code separation is also important. For example the code dealing with the external api implementation should be separated into the wrapper folder. Mathematical runtime functions should not be implemented inside mathlibra, instead they should be provided as plug-ins.

Early optimization is never good as it makes it harder to develop. But that does not mean that you can completely ignore the issue. If you have implemented something but knows ways to improve it's performance, raise an issue to notify everyone. 
That way, everyone will know what parts of the code should be optimized when time is given. 

Remember 
Have fun and learn.

### Who do I talk to? ###
If you want to ask something, get help or just chat about the project please contact me at l [dot] rahmn [at] gmail [dot] com

//raven (mathlibra creator)
