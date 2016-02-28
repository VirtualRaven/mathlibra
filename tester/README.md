# README #


### What is this folder for? ###


* This is the tester program for the core, it serves as an test client which implements most of mathlibra's functionality  and as an interactive console program. It contains all the code to initialize the core, it also contains performance tests which can be run to see how changes in the core affects the overall performance. 

* Low level unit tests needing complete access to the core should be implemented in this project. Higher level unit tests not concerned with implementation details should be added in wrapper/c++_wrapper_client

* core-tester i a great tool to easily debug the core library without any unnecessary wrapper and interfaces in between. It provides raw access to the whole of the library. Thus any type of test or debugging can easily be implemented


