/*
 * pomdp-solve
 *
 * Copyright © 1998-2003, Anthony R. Cassandra
 * Copyright © 1994-1997, Brown University
 *
 * Licensed under the Creative Commons Attribution-NonCommercial 4.0 
 * International License (CC BY-NC 4.0).
 *
 * See the LICENSE.txt file for details.
 */

#ifndef LASPACK_UTIL_H
#define LASPACK_UTIL_H

/* The LAPACK modules */
#include <laspack/qmatrix.h>
#include <laspack/vector.h>

/* Arguments for the LASPACK iterative solver. */
#define ITER_SOLVE_ACCURACY                1e-5
#define JACOBI_PRECON_OMEGA                0.8
#define MAX_JACOBI_ITERATIONS              1000

extern void showVector( Vector *v );
extern void showQMatrix( QMatrix *M );
  
#endif
