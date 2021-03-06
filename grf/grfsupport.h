#pragma once
/*
 *  libgrf
 *  grfsupport.h - commonly used functions
 *  Copyright (C) 2004  Faithful <faithful@users.sf.net>
 *  Copyright (C) 2004  Hongli Lai <h.lai@chello.nl>
 *  Copyright (C) 2004  Rasqual <rasqualtwilight@users.sf.net>
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

/** @file grfsupport.h
 *
 * Various utility functions, to be used in combination with grf.h
 */

#include "grf/grftypes.h"

#ifdef WIN32
/* Windows function names are so... ugghhhh */
#define strcasecmp(a, b) _stricmp(a, b)
#define snprintf _snprintf
#if !defined(__MINGW32__) && !defined(_INC_IO)
#include <io.h>
#define dup(handle) _dup(handle)
#define fileno(stream) _fileno(stream)
#endif /* !defined(__MINGW32__) && !defined(_INC_IO) */
#ifdef _MSC_VER
#define ssize_t SSIZE_T
#endif /* defined(_MSC_VER) */
#endif /* defined(WIN32) */

#define GRF_AlphaSort                                                          \
    ((int (*)(const void *, const void *))grflib_alpha_sort_func)
#define GRF_OffsetSort                                                         \
    ((int (*)(const void *, const void *))grflib_offset_sort_func)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/** Endian support function.
 *
 * Grabs a uint32_t from a 4byte (or more) character array.
 *
 * @warning If the character array is less than 4 bytes long, this function
 *		will access memory outside of the array
 *
 * @param p A uint8_t (char) array holding the bytes
 * @return A uint32_t in Little Endian order
 */
GRFINLINE uint32_t LittleEndian32(uint8_t *p) {
    return ((p[3] * 256 + p[2]) * 256 + p[1]) * 256 + *p;
}

/** Endian support function.
 *
 * Transforms a host uint32_t into a little-endian uint32_t
 *
 * @param hi A host uint32_t value
 * @return A uint32_t in Little Endian order
 */
GRFINLINE uint32_t ToLittleEndian32(uint32_t hi) {
    uint32_t lei;
    uint8_t *p = (uint8_t *)&lei;
    p[0] = hi & 0xFF;
    p[1] = (hi & 0xFF00) >> 8U;
    p[2] = (hi & 0xFF0000) >> 16U;
    p[3] = (hi & 0xFF000000) >> 24U;
    return lei;
}

GRFEXPORT char *grflib_normalize_path(char *out, const char *in);
GRFEXPORT uint32_t grflib_hash_name(const char *name);

GRFEXPORT void grf_sort(Grf *grf, int (*compar)(const void *, const void *));
GRFEXPORT int grflib_alpha_sort_func(const GrfFile *g1, const GrfFile *g2);
GRFEXPORT int grflib_offset_sort_func(const GrfFile *g1, const GrfFile *g2);

GRFEXPORT GrfFile *grf_find(Grf *grf, const char *fname, uint32_t *index);
GRFEXPORT uint32_t grf_find_unused(Grf *grf, uint32_t len);

GRFEXPORT GrfError *grflib_set_error(GrfError *err, GrfErrorType errtype,
                                     uint32_t line, const char *file,
                                     const char *func, void *extra);
GRFEXPORT const char *grflib_strerror(GrfError err);

/*! \brief Macro used internally
 *
 * \sa GRF_SETERR
 * \sa GRF_SETERR_2
 */
#define GRF_SETERR_ADD(a, b, e, f)                                             \
    grflib_set_error(                                                          \
        a, b, __LINE__, __FILE__, #e,                                          \
        (grf_uintptr_t *)f) /* NOTE: ? => grf_uintptr_t* conversion */
/*! \brief Simplification Macro
 *
 * Simplifies setting an error to a GrfError pointer
 */
#define GRF_SETERR(err, type, func) GRF_SETERR_ADD(err, type, func, 0)
/*! \brief Simplificatoin Macro
 *
 * Simplifies setting an error (with extra data) to a GrfError pointer
 */
#define GRF_SETERR_2(err, type, func, extra)                                   \
    GRF_SETERR_ADD(err, type, func, extra)

#ifdef __cplusplus
}
#endif // __cplusplus
