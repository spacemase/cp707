/* $Id: vecp.h,v 1.1 2002/03/17 23:53:46 jstraub Exp $ */

/* Copyright (c) 2002 by Jack Straub.
 */

/******************************************************************************
 *
 * Aleph-1 Vector Module - Private Header File
 *
 * *** FOR PRIVATE USE BY THE VECTOR MODULE ***
 * Other users please use the public header file, vec.h.
 *
 * Module History
 * Date      Author          Reason
 * ========= =============== =================================================
 * 14Mar2002 J Straub        Initiate
 *
 ******************************************************************************/

#ifndef VECP_H
#define VECP_H

        /***************************************************************
         *
         *     INCLUDES
         *
         ***************************************************************/

#include <vec.h>   
#include <cda.h>

        /***************************************************************
         *
         *     PRIVATE MACROS
         *
         ***************************************************************/

/**
 * The default initial capacity used when creating a new vector.
 */
#define VEC__DEF_INITIAL_CAPACITY       (20)

/**
 * The default extend size used when creating a new vector.
 */
#define VEC__DEF_RESIZE_BY              (10)

        /***************************************************************
         *
         *     PRIVATE TYPEDEFS
         *
         ***************************************************************/

/**
 * Used to encapsulate a vector.
 */
typedef struct vec__control_s
{
    CDA_UINT8_t*    buffer;         /* Internal storage buffer                  */
    size_t          next;           /* Index to next unused vector element      */
    size_t          curr_len;       /* Current capacity of the vector           */
    size_t          extend_by;      /* # elements to add to capacity if needed  */
    size_t          element_size;   /* The size of an element in the vector     */
} VEC__CONTROL_t, *VEC__CONTROL_p_t;

        /***************************************************************
         *
         *     PRIVATE ENTRY POINTS
         *
         ***************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Used to extend the buffer when necessary.
 */
CDA_UINT8_t* VEC__extend(
    VEC__CONTROL_p_t vec
);

#ifdef __cplusplus
}
#endif

        /***************************************************************
         *
         *     PRIVATE GLOBAL VARIABLES
         *
         ***************************************************************/

#endif /* #ifndef VECP_H */
