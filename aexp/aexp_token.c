#include <cda.h>
#include <aexpp.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <float.h>
#include <errno.h>

#define FIRST_IDX( name ) ((*(name) + *((name) + 1) * 2) % hash_size_)
#define NEXT_IDX( idx, iter ) (((idx) + (iter) * (iter)) % hash_size_)
#define IS_ZERO( x ) (!_isnan( (x) ) && fabs( (x) ) < DBL_EPSILON)

static AEXP__EVALUATE_t null_eval;
static AEXP__EVALUATE_t negate_eval;
static AEXP__EVALUATE_t pos_eval;
static AEXP__EVALUATE_t abs_eval;
static AEXP__EVALUATE_t sqrt_eval;
static AEXP__EVALUATE_t exp_eval;
static AEXP__EVALUATE_t ln_eval;
static AEXP__EVALUATE_t log10_eval;
static AEXP__EVALUATE_t sin_eval;
static AEXP__EVALUATE_t asin_eval;
static AEXP__EVALUATE_t cos_eval;
static AEXP__EVALUATE_t acos_eval;
static AEXP__EVALUATE_t tan_eval;
static AEXP__EVALUATE_t atan_eval;
static AEXP__EVALUATE_t sec_eval;
static AEXP__EVALUATE_t asec_eval;
static AEXP__EVALUATE_t csc_eval;
static AEXP__EVALUATE_t acsc_eval;
static AEXP__EVALUATE_t ctn_eval;
static AEXP__EVALUATE_t actn_eval;
static AEXP__EVALUATE_t degrees_eval;
static AEXP__EVALUATE_t radians_eval;
static AEXP__EVALUATE_t add_eval;
static AEXP__EVALUATE_t sub_eval;
static AEXP__EVALUATE_t mul_eval;
static AEXP__EVALUATE_t div_eval;
static AEXP__EVALUATE_t mod_eval;
static AEXP__EVALUATE_t raise_eval;

static int get_prime(
    int min
);

static void hash_insert(
    AEXP__TOKEN_p_t token
);

static AEXP__TOKEN_p_t hash_retrieve(
     const char *name
);

static void init(
    void
);

static AEXP__TOKEN_t tokens_[] =
{
    { "(",       AEXP__LEFT_PAREN,  0, 0,        null_eval   },
    { ")",       AEXP__RIGHT_PAREN, 0, 0,        null_eval   },
    { AEXP__UM,  AEXP__UNARY_OP,    7, 0,        negate_eval    }, // unary -
    { AEXP__UP,  AEXP__UNARY_OP,    7, 0,        pos_eval       }, // unary +
    { "abs",     AEXP__UNARY_OP,    7, 0,        abs_eval       },
    { "sqrt",    AEXP__UNARY_OP,    7, 0,        sqrt_eval      },
    { "exp",     AEXP__UNARY_OP,    7, 0,        exp_eval       },
    { "ln",      AEXP__UNARY_OP,    7, 0,        ln_eval        },
    { "log10",   AEXP__UNARY_OP,    7, 0,        log10_eval     },
    { "sin",     AEXP__UNARY_OP,    7, 0,        sin_eval       },
    { "asin",    AEXP__UNARY_OP,    7, 0,        asin_eval      },
    { "cos",     AEXP__UNARY_OP,    7, 0,        cos_eval       },
    { "acos",    AEXP__UNARY_OP,    7, 0,        acos_eval      },
    { "tan",     AEXP__UNARY_OP,    7, 0,        tan_eval       },
    { "atan",    AEXP__UNARY_OP,    7, 0,        atan_eval      },
    { "sec",     AEXP__UNARY_OP,    7, 0,        sec_eval       },
    { "asec",    AEXP__UNARY_OP,    7, 0,        asec_eval      },
    { "csc",     AEXP__UNARY_OP,    7, 0,        csc_eval       },
    { "acsc",    AEXP__UNARY_OP,    7, 0,        acsc_eval      },
    { "ctn",     AEXP__UNARY_OP,    7, 0,        ctn_eval       },
    { "actn",    AEXP__UNARY_OP,    7, 0,        actn_eval      },
    { "degrees", AEXP__UNARY_OP,    7, 0,        degrees_eval   },
    { "radians", AEXP__UNARY_OP,    7, 0,        radians_eval   },
    { "+",       AEXP__BINARY_OP,   4, 0,        add_eval       },
    { "-",       AEXP__BINARY_OP,   4, 0,        sub_eval       },
    { "*",       AEXP__BINARY_OP,   5, 0,        mul_eval       },
    { "/",       AEXP__BINARY_OP,   5, 0,        div_eval       },
    { "**",      AEXP__BINARY_OP,   6, 0,        raise_eval     },
    { "x",       AEXP__OPERAND,     0, 0.0,      null_eval      },
    { "pi",      AEXP__OPERAND,     0, AEXP__PI, null_eval      },
    { "e",       AEXP__OPERAND,     0, AEXP__E,  null_eval      }
};

static int primes_[] =
{ 13, 17, 19, 23, 29, 31, 33, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83 };

static AEXP__TOKEN_p_t  *hash_      = NULL;
static int              hash_size_  = 0;

void AEXP__token_init( void )
{
    if ( hash_ == NULL )
        init();
}

void AEXP__token_shutdown()
{
    if ( hash_ != NULL )
    {
        CDA_free( hash_ );
        hash_ = NULL;
    }
}

static void init( void )
{
    int inx     = 0;

    hash_size_ = get_prime( 2 * CDA_CARD( tokens_ ) );
    hash_ = CDA_calloc( hash_size_, sizeof(*hash_) );
    for ( inx = 0 ; inx < hash_size_ ; ++inx )
        hash_[inx] = NULL;

    for ( inx = 0 ; inx < CDA_CARD( tokens_ ) ; ++inx )
        hash_insert( &tokens_[inx] );
}

AEXP__TOKEN_p_t AEXP__get_token( const char *name )
{
    AEXP__TOKEN_p_t token   = hash_retrieve( name );

    return token;
}

const char *AEXP__get_token_type_as_string( AEXP__TOKEN_TYPE_e_t type )
{
    const char  *str    = NULL;

#define tcase( t ) case t: str = #t; break
    switch ( type )
    {
        tcase( AEXP__END_EXPR );
        tcase( AEXP__LEFT_PAREN );
        tcase( AEXP__RIGHT_PAREN );
        tcase( AEXP__UNARY_OP );
        tcase( AEXP__BINARY_OP );
        tcase( AEXP__OPERAND );
    default:
        str = NULL;
    }
#undef tcase

    return str;
}

static void hash_insert( AEXP__TOKEN_p_t token )
{
    int idx = FIRST_IDX( token->name );
    int inx = 1;

    for ( inx = 1 ; hash_[idx] != NULL && inx <= hash_size_ ; ++inx )
        idx = NEXT_IDX( idx, inx );

    CDA_ASSERT( hash_[idx] == NULL );
    hash_[idx] = token;
}

static AEXP__TOKEN_p_t hash_retrieve( const char *name )
{
    AEXP__TOKEN_p_t token   = NULL;
    CDA_BOOL_t      found   = CDA_FALSE; 
    int             idx     = FIRST_IDX( name );
    int             inx     = 1;

    while ( !found && inx <= hash_size_ )
        if ( (token = hash_[idx]) == NULL )
            found = CDA_TRUE;
        else if ( strcmp( name, token->name ) == 0 )
            found = CDA_TRUE;
        else
        {
            idx = NEXT_IDX( idx, inx );
            ++inx;
        }

    return token;
}

static int get_prime( int min )
{
    int inx   = 0;
    int prime = 0;

    for ( inx = 0 ; prime == 0 && inx < CDA_CARD( primes_ ) ; inx++ )
        if ( primes_[inx] >= min )
            prime = primes_[inx];

    CDA_ASSERT( prime > 0 );
    return prime;
}

static double null_eval( double operands[] )
{
    abort();
    return 0;
}

static double negate_eval( double operands[] )
{
    double  result  = -operands[0];
    return result;
}

static double pos_eval( double operands[] )
{
    double  result  = operands[0];
    return result;
}

static double abs_eval( double operands[] )
{
    double  result  = operands[0] >= 0 ? operands[0] : -operands[0];
    return result;
}
static double exp_eval( double operands[] )
{
    
    double  result  = exp( operands[0] );
    return result;
}

static double sqrt_eval( double operands[] )
{
    double  result  = pow( operands[0], .5 );
    return result;
}

static double ln_eval( double operands[] )
{
    double  result  = log( operands[0] );
    return result;
}

static double log10_eval( double operands[] )
{
    double  result  = log10( operands[0] );
    return result;
}

static double sin_eval( double operands[] )
{
    double  result  = sin( operands[0] );
    return result;
}

static double asin_eval( double operands[] )
{
    double  result  = asin( operands[0] );
    return result;
}

static double cos_eval( double operands[] )
{
    double  result  = cos( operands[0] );
    return result;
}

static double acos_eval( double operands[] )
{
    double  result  = acos( operands[0] );
    return result;
}

static double tan_eval( double operands[] )
{
    double  result  = tan( operands[0] );
    return result;
}

static double atan_eval( double operands[] )
{
    double  result  = atan( operands[0] );
    return result;
}

static double sec_eval( double operands[] )
{
    double  result  = 0;
    double  dcos    = cos( operands[0] );
    if ( errno == 0 )
    {
        if ( IS_ZERO( dcos ) )
            errno = EDOM;
        else
            result = 1 / dcos;
    }

    return result;
}

static double asec_eval( double operands[] )
{
    double  result  = 0;

    if ( IS_ZERO( operands[0] ) )
        errno = EDOM;
    else
        result = acos( 1 / operands[0] );

    return result;
}

static double csc_eval( double operands[] )
{
    double  result  = 1 / sin( operands[0] );
    double  dsin    = sin( operands[0] );

    if ( errno == 0 )
    {
        if ( IS_ZERO( dsin ) )
            errno = EDOM;
        else
            result = 1 / dsin;
    }

    return result;
}

static double acsc_eval( double operands[] )
{
    double  result  = 0;

    if ( IS_ZERO( operands[0] ) )
        errno = EDOM;
    else
        result = asin( 1 / operands[0] );

    return result;
}

static double ctn_eval( double operands[] )
{
    double  result  = 0;
    double  dtan    = tan( operands[0] );

    if ( errno == 0 )
    {
        if ( IS_ZERO( dtan ) )
            errno = EDOM;
        else
            result = 1 / dtan;
    }

    return result;
}

static double actn_eval( double operands[] )
{
    double  result  = 0;

    if ( IS_ZERO( operands[0] ) )
        result = AEXP__RAD90;
    else
        result = atan( 1 / operands[0] );

    return result;
}

static double add_eval( double operands[] )
{
    double  result  = operands[0] + operands[1];
    return result;
}

static double sub_eval( double operands[] )
{
    double  result  = operands[0] - operands[1];
    return result;
}

static double mul_eval( double operands[] )
{
    double  result  = operands[0] * operands[1];
    return result;
}

static double div_eval( double operands[] )
{
    double  result  = operands[0] / operands[1];
    return result;
}

static double mod_eval( double operands[] )
{
    double  result  = (double)((int)operands[0] % (int)operands[1]);
    return result;
}

static double raise_eval( double operands[] )
{
    double  result  = pow( operands[0], operands[1] );
    return result;
}

static double degrees_eval( double operands[] )
{
    double  result  = operands[0] * (180 / AEXP__PI);
    return result;
}

static double radians_eval( double operands[] )
{
    double  result  = operands[0] * (AEXP__PI / 180);
    return result;
}
