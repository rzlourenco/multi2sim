#ifndef RUNTIME_OPENCL_LLVM_H
#define RUNTIME_OPENCL_LLVM_H

#include "opencl.h"

#ifdef __cplusplus
extern "C" {
#endif

cl_uint opencl_llvm_compile_kernel(
    char const *source,
    char const *options,
    char **output_binary,
    unsigned int *output_binary_size);

#ifdef __cplusplus
}
#endif

#endif // RUNTIME_OPENCL_LLVM_H
