#ifndef AEXPP_H
#define AEXPP_H

#include <aexp.h>
#include <cda.h>
#include <vec.h>
#include <stdlib.h>

// The internal string representing unary minus
#define AEXP__UM        ("@")
// The internal string representing unary plus
#define AEXP__UP        ("_")

#define AEXP__PI        (3.14159265358979323846)
#define AEXP__E         (2.71828182845904523536)
#define AEXP__RAD90     (AEXP__PI / 2)
#define AEXP__RAD45     (AEXP__PI / 4)

#define AEXP__MAX_PRIORITY   (7)

typedef double AEXP__EVALUATE_t( double operands[] );
typedef AEXP__EVALUATE_t *AEXP__EVALUATE_p_t;

typedef enum aexp__token_type_e
{
    AEXP__NONE,
    AEXP__END_EXPR,
    AEXP__LEFT_PAREN,
    AEXP__RIGHT_PAREN,
    AEXP__UNARY_OP,
    AEXP__BINARY_OP,
    AEXP__OPERAND
} AEXP__TOKEN_TYPE_e_t;

typedef struct aexp__token_s
{
    const char*          name;
    AEXP__TOKEN_TYPE_e_t type;
    int                  priority;
    double               value;
    AEXP__EVALUATE_p_t   evaluate;
} AEXP__TOKEN_t, *AEXP__TOKEN_p_t;

typedef struct aexp__word_s
{
    const char      *str;
    size_t          len;
    AEXP__TOKEN_p_t token;
    double          value;
} AEXP__WORD_t, *AEXP__WORD_p_t;

typedef struct aexp__expr_s
{
    char        *expr_str;
    size_t      len;
    CDA_BOOL_t  valid;
    VEC_ID_t    infix;
    VEC_ID_t    postfix;
    double      value;
    const char  *bad_str;
    size_t      bad_str_len;
    int         errnum;
} AEXP__EXPR_t, *AEXP__EXPR_p_t;

AEXP__TOKEN_p_t AEXP__get_token( 
    const char *name 
);

void AEXP__token_init( 
    void
);

void AEXP__token_shutdown( 
    void
);

const char *AEXP__get_token_type_as_string(
    AEXP__TOKEN_TYPE_e_t type
);

#endif
