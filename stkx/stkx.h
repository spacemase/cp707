#ifndef STKX_H
#define STKX_H

/**
  * \defgroup stkx STKX
  * \brief    Documentation for the STKX module.
  * \{
  */

/**
  * \mainpage
  *
  * This module allows for the automatic creation of type-specific (and
  * type-safe) stacks.
  */

/**
  * \brief Creates a new type-specific stack.
  * \param  sc
  *         the storage class of the generated PUSH and POP operators;
  *         must be either "static" or "extern"
  * \param  type
  *         the type of the stack, e.g. "int" or "REC_t"
  * \param  size
  *         the size of the stack
  * \param  suffix
  *         the suffix to append to the names of push and pop operators
  *
  * This macro creates a type-specific stack. It must be invoked in the 
  * global section of a source file. It generates two global variables
  * and two functions. The global variables will be static, and will have
  * unique names beginning with STKX_. The two functions will have the
  * storage class specified by the 'sc' argument, and will have names
  * beginning with PUSH_ and POP_ followed * by the suffix provided; use
  * the PUSH_suffix function to push a value onto the stack, and the
  * POP_suffix to pop a value off the stack.
  *
  * \par
  * \b Important: the generated functions do not check for stack underflow
  * or overflow.
  *
  * \par
  * The following example creates a stack with one thousand locations of type
  * RECORD_p_t; the push and pop operators will be static, and will be named
  * PUSH_rec and POP_rec:
  *
  * \code
  *     STKX_NEW( static, REC_p_t, 1000, rec );
  * \endcode
  *
  * \par
  * To push a value onto the stack, use:
  *
  * \code
  *     REC_p_t record = get_record();
  *     PUSH_rec( record );
  * \endcode
  *
  * \par
  * To pop a value off the stack, use:
  *
  * \code
  *     REC_p_t record = POP_rec();
  * \endcode
  */
#define STKX_NEW( sc, type, size, suffix ) \
    static type STKX_ ## suffix [ size ]; \
    static type *STKX_sp_ ## suffix = STKX_ ## suffix ; \
    sc void PUSH_ ## suffix ( item ) { *STKX_sp_ ## suffix ++ = (item); }\
    sc type POP_ ## suffix ( void ) { return *--STKX_sp_ ## suffix ; }

/**
  * \}
  */

#endif
