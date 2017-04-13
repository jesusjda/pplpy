/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * gmpy.h                                                                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Python interface to the GMP or MPIR, MPFR, and MPC multiple precision   *
 * libraries.                                                              *
 *                                                                         *
 * Copyright 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007,               *
 *           2008, 2009 Alex Martelli                                      *
 *                                                                         *
 * Copyright 2008, 2009, 2010, 2011, 2012, 2013, 2014,                     *
 *           2015, 2016, 2017 Case Van Horsen                              *
 *                                                                         *
 * This file is part of GMPY2.                                             *
 *                                                                         *
 * GMPY2 is free software: you can redistribute it and/or modify it under  *
 * the terms of the GNU Lesser General Public License as published by the  *
 * Free Software Foundation, either version 3 of the License, or (at your  *
 * option) any later version.                                              *
 *                                                                         *
 * GMPY2 is distributed in the hope that it will be useful, but WITHOUT    *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or   *
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public    *
 * License for more details.                                               *
 *                                                                         *
 * You should have received a copy of the GNU Lesser General Public        *
 * License along with GMPY2; if not, see <http://www.gnu.org/licenses/>    *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/*
  gmpy C API extension header file.
  Part of Python's gmpy module since version 0.4

  Created by Pearu Peterson <pearu@cens.ioc.ee>, November 2000.
  Edited by A. Martelli <aleaxit@yahoo.com>, December 2000.
  Edited by Case Van Horsen <casevh@gmail.com>, 2009, 2010, 2011.

  Version 1.02, February 2007.
  Version 1.03, June 2008
  Version 1.04, June 2008 (no changes)
  Version 1.05, February 2009 (support MPIR)
  Version 1.20, January 2010 (remove obsolete MS hacks) casevh
  Version 2.00, April 2010 (change to gmpy2) casevh
                October 2010 (added Py_hash_t) casevh
                December 2010 (added mpfr, mpc) casevh
                January 2011 (add Pygmpy_context) casevh
                April 2011 (split into multiple files) casevh
  Version 2.10  August 2014 (reflect major rewrite during 2013/2014) casevh
 */

#ifndef Py_GMPYMODULE_H
#define Py_GMPYMODULE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Check for Python version requirements. */

#if PY_VERSION_HEX < 0x02060000
#  error "GMPY2 requires Python 2.6 or later."
#endif

#if PY_VERSION_HEX < 0x030200A4
typedef long Py_hash_t;
typedef unsigned long Py_uhash_t;
#  define _PyHASH_IMAG 1000003
#endif

/* Define various macros to deal with differences between Python 2 and 3. */

#if (PY_MAJOR_VERSION == 3)
#define PY3
#define Py2or3String_FromString     PyUnicode_FromString
#define Py2or3String_FromFormat     PyUnicode_FromFormat
#define Py2or3String_Check          PyUnicode_Check
#define Py2or3String_Format         PyUnicode_Format
#define Py2or3String_AsString       PyUnicode_AS_DATA
#define PyStrOrUnicode_Check(op)    (PyBytes_Check(op) || PyUnicode_Check(op))
#define PyIntOrLong_FromLong        PyLong_FromLong
#define PyIntOrLong_Check(op)       PyLong_Check(op)
#define PyIntOrLong_FromSize_t      PyLong_FromSize_t
#define PyIntOrLong_FromSsize_t     PyLong_FromSsize_t
#define PyIntOrLong_AsSsize_t       PyLong_AsSsize_t
#define PyIntOrLong_AsLong          PyLong_AsLong
#else
#define PY2
#define Py2or3String_FromString     PyString_FromString
#define Py2or3String_FromFormat     PyString_FromFormat
#define Py2or3String_Check          PyString_Check
#define Py2or3String_Format         PyString_Format
#define Py2or3String_AsString       PyString_AsString
#define PyStrOrUnicode_Check(op)    (PyString_Check(op) || PyUnicode_Check(op))
#define PyIntOrLong_FromLong        PyInt_FromLong
#define PyIntOrLong_Check(op)       (PyInt_Check(op) || PyLong_Check(op))
#define PyIntOrLong_FromSize_t      PyInt_FromSize_t
#define PyIntOrLong_FromSsize_t     PyInt_FromSsize_t
#define PyIntOrLong_AsSsize_t       PyInt_AsSsize_t
#define PyIntOrLong_AsLong          PyInt_AsLong
#endif

/* Support MPIR, if requested. */

#ifdef MPIR
#  include <mpir.h>
#else
#  include <gmp.h>
#endif

#include <mpfr.h>
#include <mpc.h>

#ifndef ABS
#define ABS(a)  (((a) < 0) ? -(a) : (a))
#endif

#if defined(MS_WIN32) && defined(_MSC_VER)
   /* so one won't need to link explicitly to gmp.lib...: */
#  if defined(MPIR)
#    pragma comment(lib,"mpir.lib")
#  else
#    pragma comment(lib,"gmp.lib")
#  endif
#  define isnan _isnan
#  define isinf !_finite
#  define USE_ALLOCA 1
#  define inline __inline
#endif

#ifdef __GNUC__
#  define USE_ALLOCA 1
#endif

#ifndef alloca
# ifdef __GNUC__
#  define alloca __builtin_alloca
# else
#   ifdef _MSC_VER
#    include <malloc.h>
#    define alloca _alloca
#   else
#    if HAVE_ALLOCA_H
#     include <alloca.h>
#    else
       char *alloca ();
#    endif
#   endif
# endif
#endif

#define ALLOC_THRESHOLD 8192

#define INDEX_ERROR(msg) PyErr_SetString(PyExc_IndexError, msg)
#define TYPE_ERROR(msg) PyErr_SetString(PyExc_TypeError, msg)
#define VALUE_ERROR(msg) PyErr_SetString(PyExc_ValueError, msg)
#define ZERO_ERROR(msg) PyErr_SetString(PyExc_ZeroDivisionError, msg)
#define SYSTEM_ERROR(msg) PyErr_SetString(PyExc_SystemError, msg)
#define OVERFLOW_ERROR(msg) PyErr_SetString(PyExc_OverflowError, msg)
#define RUNTIME_ERROR(msg) PyErr_SetString(PyExc_RuntimeError, msg)

#define GMPY_DEFAULT -1

/* To prevent excessive memory usage, we don't want to save very large
 * numbers in the cache. The default value specified in the options
 * structure is 128 words (512 bytes on 32-bit platforms, 1024 bytes on
 * 64-bit platforms).
 */
#define MAX_CACHE_LIMBS 16384

/* The maximum number of objects that can be saved in a cache is specified
 * here. The default value is 100.*/
#define MAX_CACHE 1000

#ifdef USE_ALLOCA
#  define TEMP_ALLOC(B, S) \
    if(S < ALLOC_THRESHOLD) { \
        B = alloca(S); \
    } else { \
        if(!(B = malloc(S))) { \
            PyErr_NoMemory(); \
            return NULL; \
        } \
    }
#  define TEMP_FREE(B, S) if(S >= ALLOC_THRESHOLD) free(B)
#else
#  define TEMP_ALLOC(B, S) \
    if(!(B = malloc(S)))  { \
        PyErr_NoMemory(); \
        return NULL; \
    }
#  define TEMP_FREE(B, S) free(B)
#endif

/* Various defs to mask differences between Python versions. */

#define Py_RETURN_NOTIMPLEMENTED\
    return Py_INCREF(Py_NotImplemented), Py_NotImplemented

#ifndef Py_SIZE
#define Py_SIZE(ob)     (((PyVarObject*)(ob))->ob_size)
#endif

#ifndef Py_TYPE
#define Py_TYPE(ob)     (((PyObject*)(ob))->ob_type)
#endif

/* Include the typedefs for all the custom types. They are required to be
 * available even when using gmpy2's C-API.
 */

#include "gmpy2_types.h"

/* Start of the C-API definitions */

#define MPZ_Type_NUM          0
#define XMPZ_Type_NUM         1
#define MPQ_Type_NUM          2
#define XMPQ_Type_NUM         3
#define MPFR_Type_NUM         4
#define XMPFR_Type_NUM        5
#define MPC_Type_NUM          6
#define XMPC_Type_NUM         7
#define CTXT_Type_NUM         8
#define CTXT_Manager_Type_NUM 9
#define RandomState_Type_NUM  10

/* The following functions are found in gmpy2_cache. */

#define GMPy_MPZ_New_NUM            11
#define GMPy_MPZ_New_RETURN         MPZ_Object *
#define GMPy_MPZ_New_PROTO          (CTXT_Object *context)

#define GMPy_MPZ_NewInit_NUM        12
#define GMPy_MPZ_NewInit_RETURN     PyObject *
#define GMPy_MPZ_NewInit_PROTO      (PyTypeObject *type, PyObject *args, PyObject *keywds)

#define GMPy_MPZ_Dealloc_NUM        13
#define GMPy_MPZ_Dealloc_RETURN     void
#define GMPy_MPZ_Dealloc_PROTO      (MPZ_Object *self)

/* The following function is found in gmpy2_convert_mpz. */

#define GMPy_MPZ_ConvertArg_NUM     14
#define GMPy_MPZ_ConvertArg_RETURN  int
#define GMPy_MPZ_ConvertArg_PROTO   (PyObject *arg, PyObject **ptr)

/* The following functions are found in gmpy2_cache. */

#define GMPy_XMPZ_New_NUM           15
#define GMPy_XMPZ_New_RETURN        XMPZ_Object *
#define GMPy_XMPZ_New_PROTO         (CTXT_Object *context)

#define GMPy_XMPZ_NewInit_NUM       16
#define GMPy_XMPZ_NewInit_RETURN    PyObject *
#define GMPy_XMPZ_NewInit_PROTO     (PyTypeObject *type, PyObject *args, PyObject *keywds)

#define GMPy_XMPZ_Dealloc_NUM       17
#define GMPy_XMPZ_Dealloc_RETURN    void
#define GMPy_XMPZ_Dealloc_PROTO     (XMPZ_Object *self)

/* Total number of C-API pointers. */

#define GMPy_API_pointers 18

#ifdef GMPY2_MODULE

/* Import a collection of general purpose macros. */

#include "gmpy2_macros.h"

/* Import the files that complete the definition of the types defined above. */

#include "gmpy2_mpz.h"
#include "gmpy2_xmpz.h"
#include "gmpy2_mpq.h"
#include "gmpy2_mpfr.h"
#include "gmpy2_mpc.h"
#include "gmpy2_context.h"
#include "gmpy2_random.h"

/* Import the header files that provide the various functions. */

/* Support object caching, creation, and deletion. */

#include "gmpy2_cache.h"

/* Suport for miscellaneous functions (ie. version, license, etc.). */

#include "gmpy2_misc.h"

/* Support conversion to/from binary format. */

#include "gmpy2_binary.h"

/* Support for mpz/xmpz specific functions. */

#include "gmpy2_convert.h"
#include "gmpy2_convert_utils.h"
#include "gmpy2_convert_gmp.h"
#include "gmpy2_convert_mpfr.h"
#include "gmpy2_convert_mpc.h"

#include "gmpy2_mpz_divmod.h"
#include "gmpy2_mpz_divmod2exp.h"
#include "gmpy2_mpz_pack.h"
#include "gmpy2_mpz_bitops.h"
#include "gmpy2_mpz_inplace.h"
#include "gmpy2_mpz_misc.h"

#include "gmpy2_xmpz_inplace.h"
#include "gmpy2_xmpz_misc.h"

/* Support for mpq specific functions. */

#include "gmpy2_mpq_misc.h"

/* Support for mpfr specific functions. */

#include "gmpy2_mpfr_misc.h"

/* Support for mpc specific functions. */

#include "gmpy2_mpc_misc.h"

/* Support Lucas sequences. */

#include "gmpy_mpz_lucas.h"

/* Support probable-prime tests. */

#include "gmpy_mpz_prp.h"

/* Support higher-level Python methods and functions; generally not
 * specific to a single typel.
 */

#include "gmpy2_abs.h"
#include "gmpy2_add.h"
#include "gmpy2_divmod.h"
#include "gmpy2_floordiv.h"
#include "gmpy2_minus.h"
#include "gmpy2_mod.h"
#include "gmpy2_mul.h"
#include "gmpy2_plus.h"
#include "gmpy2_pow.h"
#include "gmpy2_sub.h"
#include "gmpy2_truediv.h"
#include "gmpy2_math.h"
#include "gmpy2_const.h"
#include "gmpy2_square.h"
#include "gmpy2_format.h"
#include "gmpy2_hash.h"
#include "gmpy2_fused.h"
#include "gmpy2_muldiv_2exp.h"
#include "gmpy2_predicate.h"
#include "gmpy2_sign.h"
#include "gmpy2_richcompare.h"

#ifdef VECTOR
#include "gmpy2_vector.h"
#endif /* defined(VECTOR) */

#else /* defined(GMPY2_MODULE) */

/* This section is used for other C-coded modules that use gmpy2's API. */

static void **GMPy_C_API;

#define MPZ_Check(op)    ((op)->ob_type == (PyTypeObject*)GMPy_C_API[MPZ_Type_NUM])
#define XMPZ_Check(op)   ((op)->ob_type == (PyTypeObject*)GMPy_C_API[XMPZ_Type_NUM])
#define MPQ_Check(op)    ((op)->ob_type == (PyTypeObject*)GMPy_C_API[MPQ_Type_NUM])
#define XMPQ_Check(op)   ((op)->ob_type == (PyTypeObject*)GMPy_C_API[XMPQ_Type_NUM])
#define MPFR_Check(op)   ((op)->ob_type == (PyTypeObject*)GMPy_C_API[MPFR_Type_NUM])
#define XMPFR_Check(op)  ((op)->ob_type == (PyTypeObject*)GMPy_C_API[XMPFR_Type_NUM])
#define MPC_Check(op)    ((op)->ob_type == (PyTypeObject*)GMPy_C_API[MPC_Type_NUM])
#define XMPC_Check(op)   ((op)->ob_type == (PyTypeObject*)GMPy_C_API[XMPC_Type_NUM])

#define GMPy_MPZ_New        (*(GMPy_MPZ_New_RETURN        (*)GMPy_MPZ_New_PROTO)        GMPy_C_API[GMPy_MPZ_New_NUM])
#define GMPy_MPZ_NewInit    (*(GMPy_MPZ_NewInit_RETURN    (*)GMPy_MPZ_NewInit_PROTO)    GMPy_C_API[GMPy_MPZ_NewInit_NUM])
#define GMPy_MPZ_Dealloc    (*(GMPy_MPZ_Dealloc_RETURN    (*)GMPy_MPZ_Dealloc_PROTO)    GMPy_C_API[GMPy_MPZ_Dealloc_NUM])
#define GMPy_MPZ_ConvertArg (*(GMPy_MPZ_ConvertArg_RETURN (*)GMPy_MPZ_ConvertArg_PROTO) GMPy_C_API[GMPy_MPZ_ConvertArg_NUM])

static int
import_gmpy2(void)
{
    GMPy_C_API = (void **)PyCapsule_Import("gmpy2._C_API", 0);
    return (GMPy_C_API != NULL) ? 0 : -1;
}

#endif /* defined(GMPY2_MODULE) */

#ifdef __cplusplus
}
#endif /* defined(__cplusplus */
#endif /* !defined(Py_GMPYMODULE_H */