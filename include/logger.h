#pragma once

#include <stdio.h>
#include <matrix.h>

#ifdef DEBUG
# define debug_vec2(name, vec) fprintf(stderr, "%s: {% 10f, % 10f}\n",\
	name, vec[_v_x], vec[_v_y])

# define debug_vec3(name, vec) fprintf(stderr, "%s: {% 10f, % 10f, % 10f}\n",\
	name, vec[_v_x], vec[_v_y], vec[_v_z])

# define debug_mat4(name, mat) fprintf(stderr, "%s: ", name); \
	matrix_print(&mat[0][0], 4, 4, stderr);

# define debug(...) fprintf(stderr, __VA_ARGS__)

# define info(...) fprintf(stderr, __VA_ARGS__)

# define error(...) fprintf(stderr, __VA_ARGS__)
#else
# define debug_vec2(name, vec) (void)0;
# define debug_vec3(name, vec) (void)0;
# define debug_mat4(name, mat) (void)0;

# define debug(...) (void)0
# define info(...) fprintf(stderr, __VA_ARGS__)
# define error(...) fprintf(stderr, __VA_ARGS__)
#endif
