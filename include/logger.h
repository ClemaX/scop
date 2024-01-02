#pragma once

#include <stdio.h>

#ifdef DEBUG
# include <matrix.h>
# include <object.h>

# define debug_vec2(name, vec) fprintf(stderr, "%s: {% 10f, % 10f}\n",\
	name, vec[_v_x], vec[_v_y])

# define debug_vec3(name, vec) fprintf(stderr, "%s: {% 10f, % 10f, % 10f}\n",\
	name, vec[_v_x], vec[_v_y], vec[_v_z])

# define debug_mat4(name, mat) fprintf(stderr, "%s: ", name); \
	matrix_print(&mat[0][0], 4, 4, stderr);

# define debug_obj(name, obj) fprintf(stderr, "%s:\n", name); \
	object_write(&obj, stderr);

# define debug(fmt, ...) fprintf(stderr, \
	"debug:   "fmt __VA_OPT__(,) __VA_ARGS__)
#else
# define debug_vec2(name, vec) (void)0;
# define debug_vec3(name, vec) (void)0;
# define debug_mat4(name, mat) (void)0;
# define debug_obj(name, obj) (void)0;

# define debug(...) (void)0
#endif

# define info(fmt, ...) fprintf(stderr, \
	"info:    "fmt __VA_OPT__(,) __VA_ARGS__)

# define warning(fmt, ...) fprintf(stderr, \
	"warning: "fmt __VA_OPT__(,) __VA_ARGS__)

# define error(fmt, ...) fprintf(stderr, \
	"error:   "fmt  __VA_OPT__(,) __VA_ARGS__)