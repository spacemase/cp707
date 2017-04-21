/* Copyright (c) 2002 by Jack Straub.
 */

/******************************************************************************
 *
 * Aleph-1 Vector Module - Public Header File
 *
 * Module History
 * Date      Author          Reason
 * ========= =============== =================================================
 * 14Mar2002 J Straub        Initiate
 *
 ******************************************************************************/

/**
  * \defgroup vec VEC
  * \brief Documentation for the public API 
  *        of the Vector (VEC) module.
  * \{
  */

/**
  * \mainpage
  * A <i>vector</i> is essentially an open-ended array. Once a vector
  * has been created elements can be appended to it indefinitely; when 
  * necessary the vector will resize itself to accomodate all elements.
  *
  * The user creates a vector by calling the VEC_create method; this method
  * returns an ID that is then used in all subsequent operations. At the time
  * of creation the user must specifiy the size of an element of the vector.
  * Optionally, the user may also specify the initial capacity of the vector
  * and the number of elements by which to extend the vector should it be
  * necessary to do so. If the user chooses not to specify the initial
  * capacity and/or extend value suitable defaults will be applied.
  *
  * \p
  * Example: Create a vector that will be used to store int values. It will
  * have an initial capacity of 20 ints and will be extended (if necessary) in
  * increments of 10 ints.
  * \code
  * VEC_ID_t    vec = VEC_create( sizeof(int), 20, 10 );
  * \endcode
  *
  * \p
  * Example: Create a vector that will be used to store RECORD_t values. The
  * initial capacity and extend values will be determined by internal defaults.
  * \code
  * VEC_ID_t    vec = VEC_create( sizeof(RECORD_t), 0, 0 );
  * \endcode
  *
  * Items are appended to the vector using the VEC_append method. Item values
  * are copied into the vector; each item is assumed to be the size specified
  * at the time of vector creation.
  *
  * \p
  * Example: Create a vector and populate it with values of type RECORD_t.
  * \code
  * VEC_ID_t   vec = VEC_create( sizeof(RECORD_t), 0, 0 );
  * RECORD_t   *rec = get_next_record();
  *
  * while ( rec != NULL )
  * {
  *     VEC_append( vec, rec );
  *     rec = get_next_record();
  * }
  * \endcode
  *
  * Items in the vector can be interrogated with the VEC_get method, and 
  * modified with the VEC_put method.
  *
  * \p
  * Example: Create and populate a vector. Retrieve values from the vector
  * using VEC_get, and modify them using VEC_put. Note that the initial 
  * population must be done with VEC_append.
  * \code
  * VEC_ID_t    vec = VEC_create( sizeof(int), 0, 0 );
  * int         inx = 0;
  *
  * for ( inx = 0 ; inx < 10 ; ++inx )
  *     VEC_append( vec, &inx );
  *
  * for ( inx = 0 ; inx < 10 ; ++inx )
  * {
  *     int *tmp = VEC_get( vec, inx );
  *         printf( "%d\n", *tmp );
  * }
  *
  * for ( inx = 0 ; inx < 10 ; ++inx )
  * {
  *     int tmp = -inx;
  *     VEC_put( vec, inx, &tmp );
  * }
  *
  * for ( inx = 0 ; inx < 10 ; ++inx )
  * {
  *     int *tmp = VEC_get( vec, inx );
  *         printf( "%d\n", *tmp );
  * }
  * \endcode
  *
  * Alternatively, iterators may be used. VEC_begin returns a pointer to
  * the first element of the vector; VEC_end returns a pointer to the 
  * end of the vector (that is, one address beyond the end of the vector's
  * internal array). The contents of the vector are guaranteed to be
  * contiguous, so pointer arithmetic using the iterators yields appropriate
  * results.
  *
  * \p
  * Example: Use iterators to traverse an integer vector.
  * \code
  * static void traverse_int_vector( VEC_ID_t vec )
  * {
  *     int *begin  = VEC_begin( vec );
  *     int *end    = VEC_end( vec );
  *     while ( begin < end )
  *         printf( "%d\n", *begin++ );
  * }
  * \endcode
  */

/**
  * 
  * \file vec.h
  * \brief Public header file for the VEC module,
  */

#ifndef VEC_H
#define VEC_H

        /***************************************************************
         *
         *     INCLUDES
         *
         ***************************************************************/

#include <cda.h>
#include <stdlib.h>

        /***************************************************************
         *
         *     MACROS
         *
         ***************************************************************/

/**
  * \brief NULL-initializer for the VEC_ID_t type.
  *
  * This macro should be treated as an <i>opaque</i> value.
  *
  * \sa VEC_ID_t
  */
#define VEC_NULL_ID     ((VEC_ID_t)NULL)

        /***************************************************************
         *
         *     TYPEDEFS
         *
         ***************************************************************/

/*
 * Forward reference for VEC_ID_t. This should never be used directly
 * by public users; always use VEC_ID_t, instead.
 */
struct vec__control_s;

/**
 * \brief The type of the ID that represents a vector.
 *
 * In the context of this module, all vectors are represented
 * by an object of this type. A VEC_ID_t is obtained by calling
 * VEC_create_item.
 *
 * \sa VEC_create_item
 * \sa VEC_NULL_ID
 */
typedef struct vec__control_s* VEC_ID_t;

        /***************************************************************
         *
         *     ENTRY POINTS
         *
         ***************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief   Appends a value to the end of a vector.
 * \param   vec   the ID of a vector object
 * \param   item  pointer to the value to append
 * \return  item
 *
 * Append an item to the end of a vector. This is a destructive operation;
 * following an append, all iterators and other references to internal
 * storage should be assumed to be invalidated. <i>item</i> is assumed
 * to be a pointer to an object of the size specified when the vector is
 * created; its value is copied into the vector, after which <i>item</i>
 * may be reused.
 */
const void* VEC_append(
    VEC_ID_t    vec,
    const void* item
);

/**
 * \brief   Returns an iterator for a vector.
 * \param   vec     the vector to interrogate
 * \return  pointer to the vector's first member
 *
 * A pointer to the vector's first member in internal storage is returned.
 * The vector is guaranteed to be stored contiguously, so the user may
 * then increment the pointer to obtain the address of the next member.
 * Note that any destructive operation, such as an append, will invalidate
 * the iterator. The user may determine when the last member of the vector
 * has been traversed by calling VEC_end.
 *
 * \sa VEC_end
 */
void* VEC_begin(
    VEC_ID_t vec
);

/**
 * \brief   Resets a vector to an initial state.
 * \param   vec     the ID of the vector to be reset
 * \return  vec
 *
 * Reset a vector to an initial state. This is a destructive operation,
 * and will invalidate all iterators and other references to internal
 * storage.
 */
VEC_ID_t VEC_clear(
    VEC_ID_t vec
);

/**
 * \brief   Creates a new vector.
 * \param   element_size       
 *          the size of a vector element
 * \param   initial_capacity   
 *          the number of elements initially allocated for the vector
 * \param   resize_by          
 *          the number of elements to add to the vector in case 
 *          of overflow
 * \return  ID of new vector
 *
 * Create a new vector. The user supplies the initial capacity of the vector
 * and the number of items to extend the vector by should
 * it be necessary to do so. The user may specify 0 for these values, in 
 * which case defaults will be applied. When the vector is no longer needed,
 * it must be destroyed by calling VEC_destroy.
 *
 * \sa  VEC_destroy
 */
VEC_ID_t VEC_create(
    size_t element_size,
    size_t initial_capacity,
    size_t resize_by
);

/**
 * \brief   destroy a vector
 * \param   vec     the ID of the vecotr to be destroyed
 * \return  VEC_NULL_ID
 *
 * Destroy a vector. This is a destructive operation, and will invalidate
 * all iterators and other references to internal storage.
 *
 * \sa VEC_create
 */
VEC_ID_t VEC_destroy(
    VEC_ID_t vec
);

/**
 * \brief   Returns a pointer to the end of the vector.
 * \param   vec     the vector to interrogate
 * \return  pointer to the end of a vector
 *
 * Used to iterate through a vector. A pointer to the first address
 * past the vector's last member in internal storage is returned. The
 * vector is guaranteed to be stored contiguously, so the user may
 * decrement the pointer to obtain the address of previous members of
 * the vector. Note that any destructive operation, such as an append,
 * will invalidate the iterator. The user may determine when the first
 * member of the vector has been reached by calling VEC_begin.
 *
 * \sa VEC_begin
 */
void* VEC_end(
    VEC_ID_t vec
);

/**
 * \brief   Returns the address of an item in a vector.
 * \param   vec     the vector to interrogate
 * \param   inx     the index of the desired item
 * \return  the address of the desired item
 *
 * Return the address of an item in the vector. The returned address is a
 * pointer to internal storage, and may be invalidated by a subsequent 
 * destructive operation, such as an append, therefore the user may need 
 * to transfer the value of the item to local storage. So long as the
 * returned address remains valid, the user is free to use it to modify
 * the value of the addressed item.
 *
 * \sa VEC_put
 * \sa VEC_get_curr_size
 * \sa VEC_get_item_size
 */
void* VEC_get(
    VEC_ID_t vec,
    size_t   inx
);

/**
 * \brief   Returns the current size of a vector.
 * \param   vec     the ID of the vector to interrogate
 * \return  the current size of the vector
 *
 * Return the number of items currently stored in a vector.
 */
size_t VEC_get_curr_size(
    VEC_ID_t vec
);

/**
 * \brief   Returns the size of a vector element.
 * \param   vec     the ID of the vector to interrogate
 * \return  the vector's element size
 *
 * Return the size of an element stored in the vector.
 */
size_t VEC_get_item_size(
    VEC_ID_t vec
);

/**
 * \brief   Transfers the contents of an item to internal vector storage.
 * \param   vec     the ID of the target vector
 * \param   inx     the index to an item within the vector
 * \param   item    pointer to the item to be stored
 * \return  item
 *
 * Transfer the contents of an item to internal vector storage. The user
 * supplies the index of the element in the vector to modify; if the index
 * is beyond the end of the vector, SIGABRT is raised.
 *
 * \sa VEC_get
 * \sa VEC_get_curr_size
 */
const void* VEC_put(
    VEC_ID_t    vec,
    size_t      inx, 
    const void* item
);

        /***************************************************************
         *
         *     GLOBAL VARIABLES
         *
         ***************************************************************/

/**
  * \}
  */

#endif /* #ifndef VEC_H */
