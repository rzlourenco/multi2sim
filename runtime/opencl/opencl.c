/*
 *  Multi2Sim
 *  Copyright (C) 2012  Rafael Ubal (ubal@ece.neu.edu)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "debug.h"
#include "event.h"
#include "mhandle.h"
#include "misc.h"
#include "object.h"
#include "opencl.h"
#include "string-util.h"



/*
 * Global Variables
 */

char *opencl_err_not_impl =
	"\tThis error message is reported by the Multi2Sim OpenCL Runtime library\n"
	"\tlinked to your OpenCL application. The runtime only supports partial\n"
	"\timplementation of OpenCL. To request support for this feature, please\n"
	"\temail 'development@multi2sim.org'.\n";

char *opencl_err_note =
	"\tThis error message is generated by the Multi2Sim OpenCL Runtime library linked\n"
	"\twith your OpenCL host application.\n"
	"\tThis implementation only provides a subset of the OpenCL specification. Please\n"
	"\temail 'development@multi2sim.org' for further support.\n";

char *opencl_err_param_note =
	"\tThis error message is generated by the Multi2Sim OpenCL Runtime library\n"
	"\tlinked with your OpenCL host application.\n"
	"\tWhile a complete OpenCL implementation would return an error code to\n"
	"\tyour application, the Multi2Sim OpenCL library will make your program\n"
	"\tfail with an error code.\n";


int opencl_device_shared_memory;

/* Native mode */
int opencl_native_mode;



/*
 * Debug
 *
 * If environment variable 'M2S_OPENCL_DEBUG' is set, the Multi2Sim OpenCL Runtime
 * library will dump debug information about OpenCL calls, argument values,
 * intermeidate actions, and return values.
 */

static int opencl_debug_initialized;
static FILE *opencl_debug_file;

void opencl_debug(char *fmt, ...)
{
	va_list va;
	char *file_name;
	char str[MAX_LONG_STRING_SIZE];

	/* Initialize debug */
	if (!opencl_debug_initialized)
	{
		opencl_debug_initialized = 1;
		file_name = getenv("M2S_OPENCL_DEBUG");
		if (file_name)
		{
			opencl_debug_file = file_open_for_write(file_name);
			if (!opencl_debug_file)
				fatal("%s: Cannot open file", file_name);
		}
	}

	/* Exit if not debugging */
	if (!opencl_debug_file)
		return;
	
	/* Reconstruct message in 'str' first. This is done to avoid multiple
	 * calls to 'printf', that can have race conditions among threads. */
	va_start(va, fmt);
	vsnprintf(str, sizeof str, fmt, va);
	fprintf(opencl_debug_file, "[%llu] [libm2s-opencl-ioctl] %s\n", opencl_get_time(), str);
}


cl_int opencl_set_param(const void *src_value, size_t src_size,
	size_t dest_size, void *dest_value, size_t *size_ret)
{
	assert(src_value);
	if (dest_value && src_size > dest_size)
		return CL_INVALID_VALUE;
	if (size_ret)
		*size_ret = src_size;
	if (dest_value)
		memcpy(dest_value, src_value, src_size);
	return CL_SUCCESS;
}


cl_int opencl_set_string(const char *src_string, size_t dest_size,
	void *dest_string, size_t *size_ret)
{
	assert(src_string);
	size_t src_size = strlen(src_string) + 1;
	return opencl_set_param(src_string, src_size, dest_size,
		dest_string, size_ret);
}


int opencl_is_valid_device_type(cl_device_type device_type)
{
	return device_type == CL_DEVICE_TYPE_ALL
		|| (device_type & 
			(CL_DEVICE_TYPE_GPU 
			| CL_DEVICE_TYPE_CPU 
			| CL_DEVICE_TYPE_ACCELERATOR 
			| CL_DEVICE_TYPE_DEFAULT));
}


int opencl_event_wait_list_check(
	unsigned int num_events, 
	const cl_event *event_wait_list)
{
	int i;

	if ((!event_wait_list && num_events) 
		|| (event_wait_list && !num_events))
		return CL_INVALID_EVENT_WAIT_LIST;

	/* Verify that the parameter list is valid up-front */
	for (i = 0; i < num_events; i++)
	{
		if (!opencl_object_verify(event_wait_list[i], OPENCL_OBJECT_EVENT))
			return CL_INVALID_EVENT_WAIT_LIST;
	}
	return CL_SUCCESS;
}


cl_ulong opencl_get_time()
{
	cl_ulong cltime;
	struct timespec tmp;

	clock_gettime(CLOCK_MONOTONIC, &tmp);
	cltime = (cl_ulong)tmp.tv_sec;
	cltime *= 1000000000;
	cltime += (cl_ulong)tmp.tv_nsec;

	return cltime;
}


/*
 * OpenCL API Functions
 */

void *clGetExtensionFunctionAddress(
	const char *func_name)
{
	__OPENCL_NOT_IMPL__
	return NULL;
}

