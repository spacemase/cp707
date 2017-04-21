#ifndef AEXP_H
#define AEXP_H

/**
  * \defgroup aexp AEXP
  * \brief    Documentation for the public API of the 
  *           Arithmetic Expression (AEXP) module.
  * \{
  */

/**
  * \mainpage
  *
  * \section toc Table of Contents
  * <ol>
  *     <li>\ref Introduction
  *     <li>\ref aops
  *     <li>\ref Functions
  *     <li>\ref Constants
  * </ol>
  *
  * <hr>
  *
  * \section Introduction
  * The Arithmetic Expression (AEXP) module provides a means to
  * evaluate expressions represented as strings, such as "3 + 4 x 12."
  * Normal operator precedence is applied, and parentheses are 
  * recognized as grouping subexpressions, for example:
  *
  * \par
  * 3 + 4 x 12 evaluates to 51
  * \par
  * (3 + 4) x 12 evaluates to 84
  *
  * In addition to the usual arithmetic operaters a number of functions
  * are recognized, such as sine and cosine, as are certain arithmetic
  * constants, such as pi.
  *
  * In the context of this module, an arithmetic expression is an array of
  * type char that may or may not be null-terminated; this allows evaluation
  * of expressions that are embedded in another string. All expressions
  * are assumed to use infix notation. All values are are internally represented
  * as double precision floating point types.
  *
  * \section aops Arithmetic Operators
  * The AEXP module recognizes the following arithmetic operators:
  * <table>
  *     <tr>
  *         <th>Operator
  *         <th>Description
  *     <tr>
  *         <td align="center">+
  *         <td>Unary plus; has no effect on the value of an expression.
  *     <tr>
  *         <td align="center">-
  *         <td>Unary minus; negates the value of an expression.
  *     <tr>
  *         <td align="center">+
  *         <td>Addition; computes the sum of two operands.
  *     <tr>
  *         <td align="center">-</td>
  *         <td>Subtraction; computes the difference of two operands.
  *     <tr>
  *         <td align="center">*
  *         <td>Multiplication; computes the product of two operands.
  *     <tr>
  *         <td align="center">/
  *         <td>Division; produces a quotient from two operands.
  *     <tr>
  *         <td align="center">**
  *         <td>Exponentiation; raises one operand to the power of another.
  * </table>
  *
  * \section Functions
  * The AEXP module recognizes the following functions:
  * <table>
  *     <tr>
  *         <th>Function
  *         <th>Description
  *     <tr>
  *         <td align="center">abs
  *         <td>Computes the absolute value of an expression.
  *     <tr>
  *         <td align="center">sqrt
  *         <td>Computes the square root of a value.
  *     <tr>
  *         <td align="center">sqrt
  *         <td>Computes the square root of a value.
  *     <tr>
  *         <td align="center">exp
  *         <td>Computes the value <i>e<sup>x</sup></i>
  *             where <i>e</i> is the natural logarithm base.
  *     <tr>
  *         <td align="center">ln
  *         <td>Computes the natural logarithm of a value.
  *     <tr>
  *         <td align="center">log10
  *         <td>Computes the logarithm base 10 of a value.
  *     <tr>
  *         <td align="center">sin
  *         <td>Computes the sine of an angle represented in radians.
  *     <tr>
  *         <td align="center">asin
  *         <td>Computes the inverse sine, or arcsine, of a value;
  *             the result is an angle represented in radians.
  *     <tr>
  *         <td align="center">cos
  *         <td>Computes the cosine of an angle represented in radians.
  *     <tr>
  *         <td align="center">acos
  *         <td>Computes the inverse cosine, or arccosine, of a value;
  *             the result is an angle represented in radians.
  *     <tr>
  *         <td align="center">tan
  *         <td>Computes the tangent of an angle represented in radians.
  *     <tr>
  *         <td align="center">atan
  *         <td>Computes the inverse tangent, or arctagent, of a value;
  *             the result is an angle represented in radians.
  *     <tr>
  *         <td align="center">csc
  *         <td>Computes the cosecant of an angle represented in radians.
  *     <tr>
  *         <td align="center">acsc
  *         <td>Computes the inverse cosecant, or arccosecant, of a value;
  *             the result is an angle represented in radians.
  *     <tr>
  *         <td align="center">sec
  *         <td>Computes the secant of an angle represented in radians.
  *     <tr>
  *         <td align="center">asec
  *         <td>Computes the inverse secant, or arcsecant, of a value;
  *             the result is an angle represented in radians.
  *     <tr>
  *         <td align="center">ctn
  *         <td>Computes the cotangent of an angle represented in radians.
  *     <tr>
  *         <td align="center">actn
  *         <td>Computes the inverse tangent, or arccotagent, of a value;
  *             the result is an angle represented in radians.
  *     <tr>
  *         <td align="center">radians
  *         <td>Converts degrees to radians.
  *     <tr>
  *         <td align="center">degrees
  *         <td>Converts radians to degrees.
  * </table>
  *
  * \section Constants
  * The AEXP module recognizes the following constants:
  * <table>
  *     <tr>
  *         <th>Constant
  *         <th>Description
  *     <tr>
  *         <td align="center">e
  *         <td>The base of the natural logarithm.
  *     <tr>
  *         <td align="center">pi
  *         <td>The ratio of the circumference of a circle to its diameter.
  * </table>
  *
  * \example test.c
  * Following is the source code for a simple program to test the functionality
  * of the AEXP module.
  */

/**
  * \file aexp.h
  * \brief Public header file for the AEXP module.
  *
  * This is the public header file for the Arithmetic Expression (AEXP)
  * module.
  */

#include <cda.h>

/**
  * \brief NULL-initializer for the AEXP_ID_t type.
  *
  * This macro should be treated as an <i>opaque</i> value.
  */
#define AEXP_NULL_ID (NULL)

/**
  * \brief Type of the ID that represents an expression.
  *
  * In the context of this module, all arithmetic expressions are
  * represented by an object of this type.
  * 
  * \sa AEXP_create_expr
  */
typedef struct aexp__expr_s *AEXP_ID_t;


/**
  * \brief Creates an object to represent an arithmetic expression.
  * \param   expr_str an arithmetic expression represented as a string;
  *          need not be null-terminated
  * \param   len the length of <i>expr_str</i> not including the
  *          null-terminator (if any)
  * \return  the ID of an object representing the expression
  *
  * This method creates an object representing an arithmetic expression.
  * The expression is constructed by the caller using infix notation
  * (e.g. 3 * 13 + 24). The expression string is copied into the object,
  * parsed, and evaluated (if possible). The caller may use ::AEXP_is_valid
  * to determine whether the submitted expression is valid, and
  * ::AEXP_get_errno to determine whether an error occurred 
  * during expression evaluation. When the expression object is no longer
  * needed it should be disposed by calling ::AEXP_destroy_expr.
  *
  * \sa AEXP_destroy_expr, AEXP_is_valid, AEXP_get_errno
  * 
  * \b Example:
  * \code
  * static CDA_BOOL_t evaluate_expr( const char *str, size_t len )
  * {
  *     CDA_BOOL_t  rcode   = CDA_FALSE;
  *     AEXP_ID_t   expr    = AEXP_create_expr( str, len );
  *     CDA_BOOL_t  valid   = AEXP_is_valid( expr );
  *     int         errnum  = AEXP_get_errno( expr );
  * 
  *     if ( !valid )
  *         dump_bad_expr( expr );
  *     else if ( errnum != 0 )
  *         dump_errno( expr );
  *     else
  *     {
  *         double  result  = AEXP_get_result( expr );
  *         printf( "\t%.*s = %g\n", len, str, result );
  *         rcode = CDA_TRUE;
  *     }
  * 
  *     AEXP_destroy_expr( expr );
  *     return rcode;
  * }
  * \endcode
  * \b Output:
  * \code
  *     5 * 3 = 75
  * \endcode
  */
AEXP_ID_t AEXP_create_expr( 
    const char *expr_str, 
    size_t     len
);

/**
  * \brief Destroys an AEXP object.
  * \param  expr    the ID of the object to destroy
  * \return AEXP_NULL_ID
  *
  * This method destroys an object previously created by 
  * ::AEXP_create_expr. The user may pass ::AEXP_NULL_ID,
  * in which case no action will be taken.
  *
  * \b Example: 
  * \code
  * const char  *str    = "10 * (5 + 4)"
  * size_t      len     = strlen( str );
  * AEXP_ID_t   expr    = AEXP_create_expr( str, len );
  * . . .
  * expr = AEXP_destroy_expr( expr );
  * \endcode
  */
AEXP_ID_t AEXP_destroy_expr( 
    AEXP_ID_t expr
);

/**
  * \brief
  * Frees miscellaneous resources allocated by the AEXP module.
  *
  * The AEXP module allocates miscellaneous resources for global use
  * during processing. Calling this method will free those resources
  * (note: this method does not destroy any AEXP object instances; the user
  * should destroy all such instances by calling AEXP_destroy_expr).
  * It is generally not necessary to call this method. However resource 
  * monitors (such as testing tools that look for heap leaks) will
  * identify the module's outstanding allocations as possible leaks;
  * disposing of such allocations prior to program termination will
  * eliminate such warnings.
  */
void AEXP_shutdown(
    void
);

/**
  * \brief  Executes a diagnostic to validate implementation-specific 
  *         facilities.
  * \param  print if set to CDA_TRUE, the diagnostic will dump results to 
  *         stdout
  * \return CDA_TRUE if diagnostic is successful, CDA_FALSE otherwise.
  *
  *
  * The AEXP module maintains internal tables used to parse tokens from an
  * expression string. This method validates the integrity of those tables.
  * The method is public so that it can be called from a black-box test
  * program; as a rule, such a program should verify that the diagnostic
  * passes before proceeding with an exhaustive test.
  * 
  * \sa AEXP_parse_test
  */
CDA_BOOL_t AEXP_token_test( 
    CDA_BOOL_t print
);


/**
  * \brief  Executes a minimal parsing diagnostic.
  * \param  print if set to CDA_TRUE, the diagnostic will dump results to 
  *         stdout
  * \return CDA_TRUE if diagnostic is successful, CDA_FALSE otherwise.
  *
  *
  * This facility executes a minimal diagnostic to test the module's
  * parsing logic. It is <i>not</i> exhaustive. Its main utility is to
  * provide a means for diagnosing problems detected by an external
  * test program. 
  * The method is public so that it can be called from a black-box test
  * program; as a rule, such a program should verify that the diagnostic
  * passes before proceeding with an exhaustive test.
  * 
  * \sa AEXP_token_test
  */
CDA_BOOL_t AEXP_parse_test( 
    CDA_BOOL_t print
);

/**
  * \brief Returns the original expression string used to create the 
  *        expression object.
  * \param  expr   expression to interrogate
  * \return pointer to expression string
  *
  * This method returns the expression string embedded in the expression
  * object. The string is null-terminated, even if the string used to
  * create the object was not.
  *
  * \sa
  * AEXP_create_expr
  */
const char *AEXP_get_expr_str(
    AEXP_ID_t   expr
);

/**
  * \brief Returns the result of the expression evaluation.
  * \param  expr   expression to interrogate
  * \return expression value
  * 
  * This method returns the value of the expression embedded in the
  * object, assuming the expression is syntactically valid, and no
  * arithmetic exceptions were encountered during evaluation.
  *
  * \sa
  * AEXP_is_valid,
  * AEXP_get_errno
  */
double AEXP_get_result(
    AEXP_ID_t   expr
);

/**
  * \brief Returns a pointer to the start of a token in which a syntax
  *        error was encountered.
  * \param  expr    expression to interrogate
  * \param  len     used to return string length; may be NULL
  * \return pointer to token
  *
  * This method returns a pointer to a token inside a syntactically invalid
  * expression string. The token will be the token being parsed when
  * when the syntax error was detected, however it is possible that the
  * syntax error was initiated earlier in the string. The length of the 
  * string is returned via parameter <i>len</i>; the parameter may be NULL,
  * in which case it is ignored.
  *
  * \sa
  * AEXP_is_valid
  *
  * \b Example:
  * \code
  * static void dump_bad_expr( AEXP_ID_t expr )
  * {
  *     const char  *str        = AEXP_get_expr_str( expr );
  *     const char  *bad_str    = AEXP_get_invalid_token( expr, NULL );
  *     ptrdiff_t   pos         = bad_str - str;
  *     ptrdiff_t   inx         = 0;
  * 
  *     printf( "\tInvalid expression:\n" );
  *     printf( "\t%s\n\t", str );
  *     for ( inx = 0 ; inx < pos ; ++inx )
  *         putchar( ' ' );
  *     puts( "^" );
  * }
  * \endcode
  * Output:
  * \code
  *    Invalid expression:
  *    3 + (5 ** pii)
  *              ^
  * \endcode
  */
const char *AEXP_get_invalid_token(
    AEXP_ID_t   expr,
    size_t      *len
);

/**
  * \brief Indicates whether an expression is syntactically valid.
  * \param  expr   expression to interrogate
  * \return CDA_TRUE if expression is valid, CDA_FALSE otherwise
  *
  * This method returns a Boolean value indicating whether or not an
  * expression is syntactically valid.
  *
  * \sa
  * AEXP_get_invalid_token,
  * AEXP_get_errno
  */
CDA_BOOL_t AEXP_is_valid(
    AEXP_ID_t   expr
);

/**
  * \brief Returns the error number obtained from evaluating an expression.
  * \param  expr   expression to interrogate
  * \return error number
  *
  * The method will return the error number generated during expression
  * evaluation. The error number corresponds to the Standard C errno 
  * facility. If no error was encountered the return value will be 0.
  *
  * \b Example:
  * \code
  * static void dump_errno( AEXP_ID_t expr )
  * {
  *     const char  *expr_str   = AEXP_get_expr_str( expr );
  *     int         errnum      = AEXP_get_errno( expr );
  *     const char  *err_str    = strerror( errnum );
  *     printf( "\t%s\n", expr_str );
  *     printf( "\tArithmetic error: %d, \"%s\"\n", errnum, err_str );
  * }
  * \endcode
  *
  * Output:
  * \code
  *    10 ** (10 ** 10)
  *    Arithmetic error: 34, "Result too large"
  * \endcode
  */
int AEXP_get_errno(
    AEXP_ID_t   expr
);

/**
  * \}
  */

#endif
