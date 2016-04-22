

#README#
This is the repo for mathlibra, an calculator backend library that parsers and interprets mathematical expressions. It is designed to serve as an backend for developing calculator software, but due to it's modularity and extensibility it could just
as well be used to parse settings files or provide a application scripting enviroment. 

#### Main features ####
*  Minimal dependencies - Only requires a C++11 compiler and runtime.
*  Several language bindings - Supports both C, C++ and Java (and in some extent python).
*  Extensible - Plugins can be created in no time using template generated wrappers.
*  Several supported platforms - Mathlibra has been sucessfully tested on Windows, OSX,Android and several Linux distributions.
  
The rest of this readme will concern the practical details of how to setup, build, and contribute to mathlibra. To read more about mathlibra as a library please head to the [Wiki](https://github.com/VirtualRaven/mathlibra/wiki).

### How do I get set up? ###
#### The quick way ####
 `cd /to/projet/root/`

`./run.sh` which builds and runs mathlibra.
#### The longer way ####
Mathlibra have no dependency and as such should build on most platforms, though it requires compiler support for c++11.
If it is built for any other platform than windows, linux, mac osx or android, plugins will currently be disabled. This is due to the lack of plugin loaders for any other platforms.

The project is built using gradle which requires that java is installed.
To build the project run 
 `cd /to/projet/root/`
 `./gradlew build`
 
 This creates the build directory and builds all of mathlibra.
 To run the interactive mathlibra client included in the project run
 `./build/exe/core_tester/current/debug/core_tester`
 
 Gradle can also package all important binaries and headers in an zip file by writing
 `./gradlew zip`
 which creates an zip named mathlibra.zip in the root of the build directory


For vim users all directories include config files for youcomplete me to enable autocompletion. To just install youcompleteme and when opening an file in the repo accept when it asks if you want to load the config files.

### Running Tests ###

Mathlibra has three main testing tools. The first tester is python expression generator. Secondly it uses predefined tests defined in `tests/main.txt` which is parsed using mathlibra by the mathlibra_CXX_client executable. Finaly a interactive
console application is provided. To run all available automatic tests execute `./gradlew test`.

The project includes an random test case generator. The tester works by generating random expressions and passing them to mathlibra through the python api. The result is then compared to the calculated expected result, if they differ or a exception is thrown, the test fails. To run the tester write `./gradlew runPyTest` If the tester fails it will generate an report `py_test/test_report.txt` in the build folder. Be aware the this tester does only test an very limted part of mathlibra at the moment,for example the generator does not produce any assigment or matrix expression. So while it can detect some errors it does not guarentee that mathlibra works.

The interactive testing enviroment is provided by an executable named core_tester (run by executing `./run.sh`). Writing menu instead of an expression will bring up the menu.From which it is possible to run tests, get help and  inspect the status of the interpreter at any time. If you use the the debug version of core_tester it will also preform memory tests on exit.
To trigger these, start core_tester, run your expression and after enter `menu 7`in the prompt which will present memory statistics before exiting the tester.

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


An important trait of mathlibra is that it has no external dependencies and that it is platform independent. Any code breaking these traits will not be accepted. For example, do not write code that requires the c++ boost library. Any functions needed for mathlibra not provided by the standard c library or the standard template library should be implemented inside of the project.
The reason behind this is that as stated above this is a educational project, there are no time limits or deadlines to adhere to. So we have time to implement our own functions and learn something more that way. Implementing stuff by yourself may not always be the best option, but it is an great opportunity to learn, so for your own sake please follows these recommendations.  
An exception has been made to platform independent rule regarding the plug-in system which calls platform specific function for loading shared libraries, but it does so by hiding this functions from the rest of the library and implements an generic interface for loading external code. In this way the plug-in loader supports both unix and windows.

Code separation is also important. For example the code dealing with the external api implementation should be separated into the wrapper folder. Mathematical runtime functions should not be implemented inside mathlibra, instead they should be provided as plug-ins.

Early optimization is never good as it makes it harder to develop. But that does not mean that you can completely ignore the issue. If you have implemented something but knows ways to improve it's performance, raise an issue to notify everyone. 
That way, everyone will know what parts of the code should be optimized when time is given. 

Remember 
Have fun and learn.

### Who do I talk to? ###
If you want to ask something, get help or just chat about the project please contact me at l [dot] rahmn [at] gmail [dot] com

//raven (mathlibra creator)
