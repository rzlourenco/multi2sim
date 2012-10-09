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

#include <arch/x86/emu/x86-emu.h>
#include <mem-system/mem-system.h>

#include "fermi-emu.h"


/* Debug */
int frm_cuda_debug_category;

/* Error Messages */
static char *err_frm_cuda_code =
	"\tAn invalid function code was generated by your application in a CUDA system\n"
	"\tcall. Probably, this means that your application is using an incompatible\n"
	"\tversion of the Multi2Sim CUDA driver/runtime library ('libm2s-cuda'). Please\n"
	"\trecompile your application and try again.\n";




/*
 * CUDA Driver API Implementation
 */

/* List of CUDA driver calls */
enum frm_cuda_call_t
{
	frm_cuda_call_invalid = 0,
#define FRM_CUDA_DEFINE_CALL(name) frm_cuda_call_##name,
#include "cuda.dat"
#undef FRM_CUDA_DEFINE_CALL
	frm_cuda_call_count
};

/* List of CUDA driver call names */
char *frm_cuda_call_name[frm_cuda_call_count + 1] =
{
	NULL,
#define FRM_CUDA_DEFINE_CALL(name) #name,
#include "cuda.dat"
#undef FRM_CUDA_DEFINE_CALL
	NULL
};

/* Forward declarations of CUDA driver functions */
#define FRM_CUDA_DEFINE_CALL(name) \
	static int frm_cuda_func_##name(struct x86_ctx_t *ctx);
#include "cuda.dat"
#undef FRM_CUDA_DEFINE_CALL

/* List of CUDA driver functions */
typedef int (*frm_cuda_func_t)(struct x86_ctx_t *ctx);
static frm_cuda_func_t frm_cuda_func_table[frm_cuda_call_count + 1] =
{
	NULL,
#define FRM_CUDA_DEFINE_CALL(name) frm_cuda_func_##name,
#include "cuda.dat"
#undef FRM_CUDA_DEFINE_CALL
	NULL
};




/*
 * CUDA global functions
 */

int frm_cuda_call(struct x86_ctx_t *ctx)
{
	struct x86_regs_t *regs = ctx->regs;

	int code;
	int ret;

	/* Function code */
	code = regs->ebx;
	if (code <= frm_cuda_call_invalid || code >= frm_cuda_call_count)
		fatal("%s: invalid CUDA function (code %d).\n%s",
			__FUNCTION__, code, err_frm_cuda_code);

	/* Debug */
	frm_cuda_debug("CUDA call '%s' (code %d)\n",
		frm_cuda_call_name[code], code);

	/* Call */
	assert(frm_cuda_func_table[code]);
	ret = frm_cuda_func_table[code](ctx);

	/* Return value */
	return ret;
}




/*
 * CUDA call - versionCheck
 *
 * @param struct frm_cuda_version_t *version;
 *	Structure where the version of the CUDA driver implementation will be
 *	dumped. To succeed, the major version should match in the driver
 *	library (guest) and driver implementation (host), whereas the minor
 *	version should be equal or higher in the implementation (host).
 *
 *	Features should be added to the CUDA driver (guest and host) using the
 *	following rules:
 *	1)  If the guest library requires a new feature from the host
 *	    implementation, the feature is added to the host, and the minor
 *	    version is updated to the current Multi2Sim SVN revision both in
 *	    host and guest.
 *          All previous services provided by the host should remain available
 *          and backward-compatible. Executing a newer library on the older
 *          simulator will fail, but an older library on the newer simulator
 *          will succeed.
 *      2)  If a new feature is added that affects older services of the host
 *          implementation breaking backward compatibility, the major version is
 *          increased by 1 in the host and guest code.
 *          Executing a library with a different (lower or higher) major version
 *          than the host implementation will fail.
 *
 * @return
 *	The runtime implementation version is return in argument 'version'.
 *	The return value is always 0.
 */

#define FRM_CUDA_VERSION_MAJOR	1
#define FRM_CUDA_VERSION_MINOR	901

struct frm_cuda_version_t
{
	int major;
	int minor;
};

static int frm_cuda_func_versionCheck(struct x86_ctx_t *ctx)
{
	struct x86_regs_t *regs = ctx->regs;
	struct mem_t *mem = ctx->mem;

	struct frm_cuda_version_t version;

	version.major = FRM_CUDA_VERSION_MAJOR;
	version.minor = FRM_CUDA_VERSION_MINOR;

	frm_cuda_debug("\tout: version.major=%d\n", version.major);
	frm_cuda_debug("\tout: version.minor=%d\n", version.minor);

	/* Return */
	mem_write(mem, regs->ecx, sizeof version, &version);

	return 0;
}




/*
 * CUDA call - cuCtxCreate
 *
 * @param CUcontext *pctx;
 *      Returned context handle of the new context.
 *
 * @param CUdevice dev;
 *      Device to create context on.
 *
 * @return
 *	The return value is always 0 on success.
 */

static int frm_cuda_func_cuCtxCreate(struct x86_ctx_t *ctx)
{
	struct x86_regs_t *regs = ctx->regs;
	struct mem_t *mem = ctx->mem;

	unsigned int args[2];
	unsigned int pctx;
	unsigned int dev;

	struct frm_cuda_context_t *context;

	mem_read(mem, regs->ecx, 2 * sizeof(unsigned int), args);
	mem_read(mem, args[0], sizeof(unsigned int), &pctx);
	dev = args[1];

	frm_cuda_debug("\tin: dev=%u\n", dev);

	/* Create context */
	context = frm_cuda_context_create();
	context->device_id = dev;

	frm_cuda_debug("\tout: context.id=0x%08x\n", context->id);

	/* Return */
	mem_write(mem, pctx, sizeof(unsigned int), &context->id);

	return 0;
}




/*
 * CUDA call - cuCtxDestroy
 *
 * @param CUcontext ctx;
 *      Context to destroy.
 *
 * @return
 *	The return value is always 0 on success.
 */

static int frm_cuda_func_cuCtxDestroy(struct x86_ctx_t *ctx)
{
	struct x86_regs_t *regs = ctx->regs;
	struct mem_t *mem = ctx->mem;

	unsigned int args[1];
	unsigned int context_id;

	struct frm_cuda_context_t *context;

	mem_read(mem, regs->ecx, sizeof(unsigned int), args);
	mem_read(mem, args[0], sizeof(unsigned int), &context_id);

	frm_cuda_debug("\tin: context.id=%u\n", context_id);

	/* Destroy context */
	context = frm_cuda_object_get(FRM_CUDA_OBJ_CONTEXT, context_id);
	assert(context->ref_count > 0);
	if (!--context->ref_count)
		frm_cuda_context_free(context);

	return 0;
}




/*
 * CUDA call - cuModuleGetFunction
 *
 * @param CUfunction *pfunc;
 *      Returned function handle.
 *
 * @param CUmodule mod;
 *      Module to retrieve function from.
 *
 * @param const char *name;
 *      Name of function to retrieve.
 *
 * @return
 *	The return value is always 0 on success.
 */

static int frm_cuda_func_cuModuleGetFunction(struct x86_ctx_t *ctx)
{
	struct x86_regs_t *regs = ctx->regs;
	struct mem_t *mem = ctx->mem;

	unsigned int args[3];
	unsigned int pfunc;
	unsigned int module_id;
	char function_name[MAX_STRING_SIZE];

	struct frm_cuda_module_t *module;
	struct frm_cuda_function_t *function;
	char section_name[MAX_STRING_SIZE];
	struct elf_section_t *section;
	int i;

	mem_read(mem, regs->ecx, 3 * sizeof(unsigned int), args);
	mem_read(mem, args[0], sizeof(unsigned int), &pfunc);
	mem_read(mem, args[1], sizeof(unsigned int), &module_id);
	mem_read(mem, args[2], MAX_STRING_SIZE, function_name);

	frm_cuda_debug("\tin: module.id=0x%08x\n", module_id);
	frm_cuda_debug("\tin: function_name=%s\n", function_name);

	/* Get module */
	module = frm_cuda_object_get(FRM_CUDA_OBJ_MODULE, module_id);

	/* Create function */
	function = frm_cuda_function_create();
	function->module_id = module->id;
	snprintf(function->name, MAX_STRING_SIZE, "%s", function_name);

	/* Load function */
	snprintf(section_name, MAX_STRING_SIZE, ".text.%s", function_name);
	for (i = 0; i < list_count(module->elf_file->section_list); ++i)
	{
		section = (struct elf_section_t *)list_get(module->elf_file->section_list, i);
		if (!strncmp(section->name, section_name, MAX_STRING_SIZE))
			break;
	}
	if (i == list_count(module->elf_file->section_list))
		fatal("%s section not found!\n", section_name);
	function->function_buffer.ptr = section->buffer.ptr;
	function->function_buffer.size = section->buffer.size;

	frm_cuda_debug("\tout: function.id=0x%08x\n", function->id);

	/* Return */
	mem_write(mem, pfunc, sizeof(unsigned int), &function->id);

	return 0;
}




/*
 * CUDA call - cuModuleLoad
 *
 * @param CUmodule *pmod;
 *      Returned module.
 *
 * @param const char *fname;
 *      Filename of module to load.
 *
 * @return
 *	The return value is always 0 on success.
 */

static int frm_cuda_func_cuModuleLoad(struct x86_ctx_t *ctx)
{
	struct x86_regs_t *regs = ctx->regs;
	struct mem_t *mem = ctx->mem;

	unsigned int args[2];
	unsigned int pmod;
	char fname[MAX_STRING_SIZE];

	struct frm_cuda_module_t *module;

	mem_read(mem, regs->ecx, 2 * sizeof(unsigned int), args);
	mem_read(mem, args[0], sizeof(unsigned int), &pmod);
	mem_read(mem, args[1], MAX_STRING_SIZE, fname);

	frm_cuda_debug("\tin: filename=%s\n", fname);

	/* Create module */
	module = frm_cuda_module_create();
	module->elf_file = elf_file_create_from_path(fname);

	frm_cuda_debug("\tout: module.id=0x%08x\n", module->id);

	/* Return */
	mem_write(mem, pmod, sizeof(unsigned int), &module->id);

	return 0;
}




/*
 * CUDA call - cuMemAlloc
 *
 * @param CUdeviceptr *dptr;
 *      Returned device pointer.
 *
 * @param size_t bytesize;
 *      Requested allocation size in bytes.
 *
 * @return
 *	The return value is always 0 on success.
 */

static int frm_cuda_func_cuMemAlloc(struct x86_ctx_t *ctx)
{
	struct x86_regs_t *regs = ctx->regs;
	struct mem_t *mem = ctx->mem;

	unsigned int args[2];
	unsigned int pdptr;
	unsigned int bytesize;

	struct frm_cuda_memory_t *cuda_mem;

	mem_read(mem, regs->ecx, 2 * sizeof(unsigned int), args);
	pdptr = args[0];
	bytesize = args[1];

	frm_cuda_debug("\tin: bytesize=%u\n", bytesize);

	/* Create memory object */
	cuda_mem = frm_cuda_memory_create();
	cuda_mem->size = bytesize;

	/* Assign position in device global memory */
	cuda_mem->device_ptr = frm_emu->global_mem_top;
	frm_emu->global_mem_top += bytesize;
	frm_emu->free_global_mem_size -= bytesize;

	frm_cuda_debug("\tout: dptr=0x%08x\n", cuda_mem->device_ptr);

	/* Return */
	mem_write(mem, pdptr, sizeof(unsigned int), &(cuda_mem->device_ptr));

	return 0;
}




/*
 * CUDA call - cuMemFree
 *
 * @param CUdeviceptr dptr;
 *      Pointer to memory to free.
 *
 * @return
 *	The return value is always 0 on success.
 */

static int frm_cuda_func_cuMemFree(struct x86_ctx_t *ctx)
{
	struct x86_regs_t *regs = ctx->regs;
	struct mem_t *mem = ctx->mem;

	unsigned int args[1];
	unsigned int dptr;

        void *cuda_object;
	unsigned int object_id;
        unsigned int device_ptr;
	unsigned int mem_id = 0;
	struct frm_cuda_memory_t *cuda_mem;

	mem_read(mem, regs->ecx, sizeof(unsigned int), args);
	dptr = args[0];

	frm_cuda_debug("\tin: dptr=0x%08x\n", dptr);

	/* Look for memory object */        
        LINKED_LIST_FOR_EACH(frm_cuda_object_list)
        {
		if (!(cuda_object = linked_list_get(frm_cuda_object_list)))
			fatal("%s: empty object", __FUNCTION__);
		object_id = *((unsigned int *)cuda_object);
		if (object_id >> 16 == FRM_CUDA_OBJ_MEMORY)
		{
			device_ptr = ((struct frm_cuda_memory_t *)cuda_object)->device_ptr;
                	if (device_ptr == dptr)
				mem_id = object_id;
		}
        }
	if (mem_id == 0)
		fatal("%s: requested CUDA object does not exist (id=0x%08x)",
			__FUNCTION__, mem_id);

	/* Free memory object */
	cuda_mem = frm_cuda_object_get(FRM_CUDA_OBJ_MEMORY, mem_id);
	assert(cuda_mem->ref_count > 0);
	if (!--cuda_mem->ref_count)
		frm_cuda_memory_free(cuda_mem);

	return 0;
}




/*
 * CUDA call - cuMemGetInfo
 *
 * @param size_t *free;
 *      Returned free memory in bytes
 *
 * @param size_t *total;
 *      Returned total memory in bytes
 *
 * @return
 *	The return value is always 0 on success.
 */

static int frm_cuda_func_cuMemGetInfo(struct x86_ctx_t *ctx)
{
	struct x86_regs_t *regs = ctx->regs;
	struct mem_t *mem = ctx->mem;

	unsigned int args[2];
	unsigned int free;
	unsigned int total;

	mem_read(mem, regs->ecx, 2 * sizeof(unsigned int), args);
	free = args[0];
	total = args[1];

	frm_cuda_debug("\tout: free=%u\n", frm_emu->free_global_mem_size);
	frm_cuda_debug("\tout: total=%u\n", frm_emu->total_global_mem_size);

	/* Return */
	mem_write(mem, free, sizeof(unsigned int), &(frm_emu->free_global_mem_size));
	mem_write(mem, total, sizeof(unsigned int), &(frm_emu->total_global_mem_size));

	return 0;
}




/*
 * CUDA call - cuMemcpyDtoH
 *
 * @param void *dstHost;
 *      Destination host pointer.
 *
 * @param CUdeviceptr srcDevice;
 *      Source device pointer.
 *
 * @param size_t ByteCount;
 *      Size of memory copy in bytes.
 *
 * @return
 *	The return value is always 0 on success.
 */

static int frm_cuda_func_cuMemcpyDtoH(struct x86_ctx_t *ctx)
{
	struct x86_regs_t *regs = ctx->regs;
	struct mem_t *mem = ctx->mem;

	unsigned int args[3];
	unsigned int dstHost;
	unsigned int srcDevice;
	unsigned int ByteCount;

	void *buf;

	mem_read(mem, regs->ecx, 3 * sizeof(unsigned int), args);
	dstHost = args[0];
	srcDevice = args[1];
	ByteCount = args[2];

	frm_cuda_debug("\tin: dstHost=0x%08x\n", dstHost);
	frm_cuda_debug("\tin: srcDevice=0x%08x\n", srcDevice);
	frm_cuda_debug("\tin: ByteCount=%u\n", ByteCount);

	/* Copy */
	buf = malloc(ByteCount);
	if (!buf)
		fatal("%s: out of memory", __FUNCTION__);
	mem_read(frm_emu->global_mem, srcDevice, ByteCount, buf);
	mem_write(mem, dstHost, ByteCount, buf);
	free(buf);

	return 0;
}




/*
 * CUDA call - cuMemcpyHtoD
 *
 * @param CUdeviceptr dstDevice;
 *      Destination device pointer.
 *
 * @param const void *srcHost;
 *      Source host pointer.
 *
 * @param size_t ByteCount;
 *      Size of memory copy in bytes.
 *
 * @return
 *	The return value is always 0 on success.
 */

static int frm_cuda_func_cuMemcpyHtoD(struct x86_ctx_t *ctx)
{
	struct x86_regs_t *regs = ctx->regs;
	struct mem_t *mem = ctx->mem;

	unsigned int args[3];
	unsigned int dstDevice;
	unsigned int srcHost;
	unsigned int ByteCount;

	void *buf;

	mem_read(mem, regs->ecx, 3 * sizeof(unsigned int), args);
	dstDevice = args[0];
	srcHost = args[1];
	ByteCount = args[2];

	frm_cuda_debug("\tin: dstDevice=0x%08x\n", dstDevice);
	frm_cuda_debug("\tin: srcHost=0x%08x\n", srcHost);
	frm_cuda_debug("\tin: ByteCount=%u\n", ByteCount);

	/* Copy */
	buf = malloc(ByteCount);
	if (!buf)
		fatal("%s: out of memory", __FUNCTION__);
	mem_read(mem, srcHost, ByteCount, buf);
	mem_write(frm_emu->global_mem, dstDevice, ByteCount, buf);
	free(buf);

	return 0;
}




/*
 * CUDA call - cuLaunchKernel
 *
 * @param CUfunction f;
 *      Kernel to launch.
 *
 * @param unsigned int gridDimX;
 *      Width of grid in blocks.
 *
 * @param unsigned int gridDimY;
 *      Height of grid in blocks.
 *
 * @param unsigned int gridDimZ;
 *      Depth of grid in blocks.
 *
 * @param unsigned int blockDimX;
 *      X dimension of each thread block.
 *
 * @param unsigned int blockDimY;
 *      Y dimension of each thread block.
 *
 * @param unsigned int blockDimZ;
 *      Z dimension of each thread block.
 *
 * @param unsigned int sharedMemBytes;
 *      Dynamic shared-memory size per thread block in bytes.
 *
 * @param CUstream hStream;
 *      Stream identifier.
 *
 * @param void **kernelParams;
 *      Array of pointers to kernel parameters.
 *
 * @param void **extra;
 *      Extra options.
 *
 * @return
 *	The return value is always 0 on success.
 */

static int frm_cuda_func_cuLaunchKernel(struct x86_ctx_t *ctx)
{
	struct x86_regs_t *regs = ctx->regs;
	struct mem_t *mem = ctx->mem;

	unsigned int args[11];
	unsigned int function_id;
	unsigned int gridDimX;
	unsigned int gridDimY;
	unsigned int gridDimZ;
	unsigned int blockDimX;
	unsigned int blockDimY;
	unsigned int blockDimZ;
	unsigned int sharedMemBytes;
	unsigned int hStream;
	unsigned int kernelParams;
	unsigned int extra;

	struct frm_cuda_function_t *function;
	struct frm_cuda_function_arg_t *arg;
	char arg_name[MAX_STRING_SIZE];
	unsigned int arg_ptr;
	unsigned int arg_value;
	int i;

	mem_read(mem, regs->ecx, 11 * sizeof(unsigned int), args);
	mem_read(mem, args[0], sizeof(unsigned int), &function_id);
	gridDimX = args[1];
	gridDimY = args[2];
	gridDimZ = args[3];
	blockDimX = args[4];
	blockDimY = args[5];
	blockDimZ = args[6];
	sharedMemBytes = args[7];
	hStream = args[8];
	kernelParams = args[9];
	extra = args[10];

	frm_cuda_debug("\tfunction_id=0x%08x\n", function_id);
	frm_cuda_debug("\tgridDimX=%u\n", gridDimX);
	frm_cuda_debug("\tgridDimY=%u\n", gridDimY);
	frm_cuda_debug("\tgridDimZ=%u\n", gridDimZ);
	frm_cuda_debug("\tblockDimX=%u\n", blockDimX);
	frm_cuda_debug("\tblockDimY=%u\n", blockDimY);
	frm_cuda_debug("\tblockDimZ=%u\n", blockDimZ);
	frm_cuda_debug("\tsharedMemBytes=%u\n", sharedMemBytes);
	frm_cuda_debug("\thStream=0x%08x\n", hStream);
	frm_cuda_debug("\tkernelParams=0x%08x\n", kernelParams);
	frm_cuda_debug("\textra=%u\n", extra);

	/* Get function */
	function = frm_cuda_object_get(FRM_CUDA_OBJ_FUNCTION, function_id);

	/* Create and setup grid */
	function->grid = frm_grid_create(function);
	function->global_size3[0] = gridDimX*blockDimX;
	function->global_size3[1] = gridDimY*blockDimY;
	function->global_size3[2] = gridDimZ*blockDimZ;
	function->global_size = function->global_size3[0] * function->global_size3[1] * 
					function->global_size3[2];
	function->local_size3[0] = blockDimX;
	function->local_size3[1] = blockDimY;
	function->local_size3[2] = blockDimZ;
	function->local_size = function->local_size3[0] * function->local_size3[1] * 
					function->local_size3[2];
	function->group_count3[0] = gridDimX;
	function->group_count3[1] = gridDimY;
	function->group_count3[2] = gridDimZ;
	function->group_count = function->group_count3[0] * function->group_count3[1] * 
					function->group_count3[2];

	/* Create arguments */
	for (i = 0; i < sizeof(kernelParams); ++i)
	{
		mem_read(mem, kernelParams + i * 4, sizeof(unsigned int), &arg_ptr);
		mem_read(mem, arg_ptr, sizeof(unsigned int), &arg_value);

		snprintf(arg_name, MAX_STRING_SIZE, "arg_%d", i);
		arg = frm_cuda_function_arg_create(arg_name);
		arg->kind = FRM_CUDA_FUNCTION_ARG_KIND_POINTER;
		arg->mem_scope = FRM_CUDA_MEM_SCOPE_GLOBAL;
		arg->access_type = FRM_CUDA_FUNCTION_ARG_READ_WRITE;
		arg->value = arg_value;
		list_add(function->arg_list, arg);
	}

	/* Setup threads, constant memory and arguments */
	frm_grid_setup_threads(function->grid);
	frm_grid_setup_const_mem(function->grid);
	frm_grid_setup_args(function->grid);

	/* Setup status */
	frm_grid_set_status(function->grid, frm_grid_pending);

	/* Run */
	frm_grid_run(function->grid);

	/* Free grid */
	frm_grid_free(function->grid);

	return 0;
}

