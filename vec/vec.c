/* $Id: vec.c,v 1.2 2002/03/15 18:37:20 jstraub Exp $ */

/* Copyright (c) 2002 by Jack Straub.
 */

/******************************************************************************
 *
 * Aleph-1 Vector Module -  Module Main Source File
 *
 * Module History
 * Date      Author          Reason
 * ========= =============== =================================================
 * 14Mar2002 J Straub        Initiate
 *
 ******************************************************************************/

        /***************************************************************
         *
         *     LOCAL INCLUDES
         *
         ***************************************************************/

#include <vecp.h>
#include <cda.h>
#include <string.h>
#include <signal.h>

        /***************************************************************
         *
         *     LOCAL MACROS
         *
         ***************************************************************/

        /***************************************************************
         *
         *     LOCAL TYPEDEFS
         *
         ***************************************************************/

        /***************************************************************
         *
         *     LOCAL PROTOTYPES
         *
         ***************************************************************/

        /***************************************************************
         *
         *     LOCAL VARIABLES
         *
         ***************************************************************/

        /***************************************************************
         *
         *     EXTERNAL ENTRY POINTS
         *
         ***************************************************************/

const void*
VEC_append( VEC_ID_t vec, const void* item )
{
    CDA_UINT8_t*    target  = NULL;

    if ( vec->next >= vec->curr_len )
        VEC__extend( vec );

    CDA_ASSERT( vec->next < vec->curr_len );

    target = vec->buffer + vec->next * vec->element_size;
    memmove( target, item, vec->element_size );
    ++vec->next;

    return item;
}

void*
VEC_begin( VEC_ID_t vec )
{
    return vec->buffer;
}

VEC_ID_t
VEC_clear( VEC_ID_t vec )
{
    vec->next = 0;

    return vec;
}

VEC_ID_t
VEC_create( size_t element_size, size_t initial_capacity, size_t resize_by )
{
	VEC__CONTROL_p_t    vec = CDA_NEW( VEC__CONTROL_t );
	size_t              cap = initial_capacity;
    size_t              res = resize_by;

    if ( cap == 0 )
        cap = VEC__DEF_INITIAL_CAPACITY;
    if ( res == 0 )
        res = VEC__DEF_RESIZE_BY;

	vec->curr_len       = cap;
    vec->extend_by      = res;
    vec->element_size   = element_size;
    vec->buffer         = CDA_calloc( cap, element_size );
    vec->next           = 0;

    return vec;
}

VEC_ID_t
VEC_destroy( VEC_ID_t vec )
{
    if ( vec != VEC_NULL_ID )
    {
        CDA_free( vec->buffer );
        CDA_free( vec );
    }

    return VEC_NULL_ID;
}

void*
VEC_end( VEC_ID_t vec )
{
    CDA_UINT8_t*   target  = vec->buffer + vec->next * vec->element_size;
    return target;
}

void*
VEC_get( VEC_ID_t vec, size_t inx )
{
    CDA_UINT8_t*    target  = vec->buffer + inx * vec->element_size;

    if ( inx >= vec->next )
        raise( SIGABRT );

    return target;
}

size_t
VEC_get_curr_size( VEC_ID_t vec )
{
    return vec->next;
}

size_t
VEC_get_item_size( VEC_ID_t vec )
{
    return vec->element_size;
}

const void*
VEC_put( VEC_ID_t vec, size_t inx, const void* item )
{
    CDA_UINT8_t*    target  = vec->buffer + inx * vec->element_size;

    if ( inx >= vec->next )
        abort();

    memmove( target, item, vec->element_size );
    return item;
}

        /***************************************************************
         *
         *     PRIVATE ENTRY POINTS
         *
         ***************************************************************/

CDA_UINT8_t*
VEC__extend( VEC__CONTROL_p_t vec )
{
    vec->curr_len += vec->extend_by;
    vec->buffer = CDA_realloc( vec->buffer, vec->curr_len * vec->element_size );

    return vec->buffer;
}
