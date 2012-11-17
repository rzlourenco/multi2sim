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


#include <lib/mhandle/mhandle.h>
#include <lib/util/debug.h>
#include <lib/util/list.h>

#include "opengl.h"
#include "opengl-matrix.h"
#include "opengl-matrix-stack.h"


struct x86_opengl_matrix_stack_t *x86_opengl_matrix_stack_create(GLenum mode)
{
	struct x86_opengl_matrix_stack_t *stack;
	struct x86_opengl_matrix_t *mtx;

	/* Allocate */
	stack = xcalloc(1, sizeof(struct x86_opengl_matrix_stack_t));

	mtx = x86_opengl_matrix_create(MATRIX_GENERAL);

	/* Initialize */
	stack->stack = list_create();
	list_add(stack->stack, mtx);
	stack->depth = 0;

	switch (mode)
	{

	case GL_MODELVIEW:
	{
		stack->max_depth = MAX_MODELVIEW_STACK_DEPTH;
		break;
	}

	case GL_PROJECTION:
	{
		stack->max_depth = MAX_PROJECTION_STACK_DEPTH;
		break;
	}

	case GL_TEXTURE:
	{
		stack->max_depth = MAX_TEXTURE_STACK_DEPTH;
		break;
	}

	case GL_COLOR:
	{
		stack->max_depth = MAX_COLOR_STACK_DEPTH;
		break;
	}

	default:
		break;
	} 

	/* Return */	
	return stack;
}

void x86_opengl_matrix_stack_free(struct x86_opengl_matrix_stack_t *mtx_stack)
{
	/* Free matrices in the list */
	while (list_count(mtx_stack->stack))
		x86_opengl_matrix_free(list_remove_at(mtx_stack->stack, 0));
	/* Free list */
	list_free(mtx_stack->stack);
	/* Free stack */
	free(mtx_stack);
}

int x86_opengl_matrix_stack_push(struct x86_opengl_matrix_stack_t *mtx_stack, struct x86_opengl_matrix_t *mtx)
{
	if (mtx_stack->depth == mtx_stack->max_depth)
		fatal("Stack overflow, max depth = %d\n", mtx_stack->max_depth);
	x86_opengl_debug("\tpush: curr_depth = %d, max_depth = %d\n", 
		mtx_stack->depth, mtx_stack->max_depth);
	mtx_stack->depth += 1;
	list_push(mtx_stack->stack, mtx);
	return 0;
}

struct x86_opengl_matrix_t *x86_opengl_matrix_stack_pop(struct x86_opengl_matrix_stack_t *mtx_stack)
{
	/* Variables */
	struct x86_opengl_matrix_t *mtx;

	/* Pop from stack */
	if (mtx_stack->depth == 0 )
		fatal("Stack underflow, max depth = %d\n", mtx_stack->max_depth);
	x86_opengl_debug("\tpop: curr_depth = %d, max_depth = %d\n", 
		mtx_stack->depth, mtx_stack->max_depth);

	mtx_stack->depth -= 1;
	mtx = list_pop(mtx_stack->stack);
	if (mtx == NULL)
		fatal("Empty stack!\n");
	x86_opengl_matrix_free(mtx);

	/* Return */
	return mtx;
}

