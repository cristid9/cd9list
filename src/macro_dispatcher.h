#ifndef MACRO_DISPATCHER_H___
#define MACRO_DISPATCHER_H___

#include "va_numargs.h"

#define MACRO_DISPATCHER(func, ...) \
    MACRO_DISPATCHER_(func, __VA_NARG__(__VA_ARGS__))(__VA_ARGS__) 

#define MACRO_DISPATCHER_(func, num) \
    MACRO_DISPATCHER__(func, num)

#define MACRO_DISPATCHER__(func, num) \
    func ## num

#endif // MACRO_DISPATCHER_H__
