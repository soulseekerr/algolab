#pragma once

// g++ -DDEBUG_CONTAINER ...
// # or for CMake:
// target_compile_definitions(your_target PRIVATE DEBUG_CONTAINER)
#ifdef DEBUG_CONTAINER
    #define DEBUG_LOG(msg) std::cout << msg << std::endl
#else
    #define DEBUG_LOG(msg) ((void)0)
#endif