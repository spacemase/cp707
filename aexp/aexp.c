#include <aexpp.h>
#include <cda.h>
#include <vec.h>
#include <stk.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

static size_t extract_word(
    AEXP__EXPR_p_t  expr,
    size_t          pos,
    AEXP__WORD_p_t  word
);

static size_t extract_number(
    AEXP__EXPR_p_t expr,
    size_t          pos,
    AEXP__WORD_p_t  word
);

static size_t extract_symbol(
    AEXP__EXPR_p_t  expr,
    size_t          pos,
    AEXP__WORD_p_t  word
);

static CDA_BOOL_t is_leading(
    AEXP__EXPR_p_t  expr,
    AEXP__WORD_p_t  pos
);

static CDA_BOOL_t parse_expr(
    AEXP__EXPR_p_t expr
);

static CDA_BOOL_t is_valid(
    AEXP__EXPR_p_t expr
);

static CDA_BOOL_t xlate_to_postfix( 
    AEXP__EXPR_p_t expr
);

static CDA_BOOL_t evaluate_expr(
    AEXP__EXPR_p_t expr
);

AEXP_ID_t AEXP_create_expr( const char *expr_str, size_t len )
{
    AEXP__EXPR_p_t   expr    = CDA_NEW( AEXP__EXPR_t );

    AEXP__token_init();
    expr->expr_str  = CDA_malloc( len + 1 );
    expr->infix = VEC_create( sizeof(AEXP__WORD_t), 0, 0 );
    expr->postfix = VEC_create( sizeof(AEXP__WORD_t), 0, 0 );
    memmove( expr->expr_str, expr_str, len );
    expr->expr_str[len] = '\000';
    expr->len = len;
    expr->bad_str = "";
    expr->bad_str_len = 0;
    expr->value = 0;
    expr->errnum = 0;
    if ( !(expr->valid = parse_expr( expr )) )
        ;
    else if ( !(expr->valid = is_valid( expr )) )
        ;
    else if ( !(expr->valid = xlate_to_postfix( expr )) )
        ;
    else if ( !evaluate_expr( expr ) )
        ;
    else
        ;

    return expr;
}

AEXP_ID_t AEXP_destroy_expr( AEXP_ID_t expr )
{
    if ( expr != AEXP_NULL_ID )
    {
        CDA_free( expr->expr_str );
        VEC_destroy( expr->infix );
        VEC_destroy( expr->postfix );
        CDA_free( expr );
    }

    return AEXP_NULL_ID;
}

const char *AEXP_get_expr_str( AEXP_ID_t expr )
{
    const char  *result = expr->expr_str;
    return result;
}

double AEXP_get_result( AEXP_ID_t expr )
{
    double  result  = expr->value;
    return result;
}

const char *AEXP_get_invalid_token( AEXP_ID_t expr, size_t *len )
{
    const char  *result = expr->bad_str;
    if ( len != NULL )
        *len = expr->bad_str_len;

    return result;
}

CDA_BOOL_t AEXP_is_valid( AEXP_ID_t expr )
{
    CDA_BOOL_t  valid   = expr->valid;
    return valid;
}

int AEXP_get_errno( AEXP_ID_t expr )
{
    int result  = expr->errnum;
    return result;
}

void AEXP_shutdown( void )
{
    AEXP__token_shutdown();
}

static CDA_BOOL_t parse_expr( AEXP__EXPR_p_t expr )
{
    CDA_BOOL_t  rcode       = CDA_TRUE;
    char        *str        = expr->expr_str;
    size_t      pos         = 0;

    while ( pos < expr->len && rcode )
    {
        if ( str[pos] == ' ' )
            pos++;
        else
        {
            size_t          next    = 0;
            AEXP__WORD_t    word;

            word.str = NULL;
            word.len = 0;
            word.token = NULL;
            if ( isalpha( str[pos] ) )
                next = extract_word( expr, pos, &word );
            else if ( isdigit( str[pos] ) || str[pos] == '.' )
                next = extract_number( expr, pos, &word );
            else
                next = extract_symbol( expr, pos, &word );

            if ( word.token == NULL )
            {
                rcode = CDA_FALSE;
                expr->bad_str = &str[pos];
                expr->bad_str_len = expr->len - pos;
            }
            else
            {
                VEC_append( expr->infix, &word );
                pos = next;
            }
        }
    }

    expr->valid = rcode;
    return rcode;
}

static CDA_BOOL_t is_valid( AEXP__EXPR_p_t expr )
{
    CDA_BOOL_t      rcode   = CDA_TRUE;
    int             parens  = 0;
    AEXP__WORD_p_t  first   = VEC_begin( expr->infix );
    AEXP__WORD_p_t  last    = VEC_end( expr->infix );
    AEXP__WORD_p_t  next    = first;

    for ( next = first ; rcode && next != last ; ++next )
    {
        AEXP__TOKEN_p_t      token   = next->token;
        AEXP__TOKEN_TYPE_e_t type    = token->type;
        if ( is_leading( expr, next ) )
        {
            if ( type != AEXP__OPERAND  &&
                 type != AEXP__UNARY_OP &&
                 type != AEXP__LEFT_PAREN
               )
                rcode = CDA_FALSE;
        }
        else if ( type != AEXP__BINARY_OP && type != AEXP__RIGHT_PAREN )
            rcode = CDA_FALSE;
        else
            ;

        if ( rcode )
        {
            if ( type == AEXP__LEFT_PAREN )
                ++parens;
            else if ( type == AEXP__RIGHT_PAREN && --parens < 0 )
                rcode = CDA_FALSE;
            else
                ;
        }

        if ( rcode && next == last - 1 )
        {
            if ( type != AEXP__OPERAND && type != AEXP__RIGHT_PAREN )
                rcode = CDA_FALSE;
            else if ( parens != 0 )
                rcode = CDA_FALSE;
            else
                ;
        }

        if ( !rcode )
        {
            expr->valid = CDA_FALSE;
            expr->bad_str = next->str;
            expr->bad_str_len = expr->len - (expr->bad_str - expr->expr_str);
        }
    }

    return rcode;
}

        

static size_t
extract_word( AEXP__EXPR_p_t expr, size_t pos, AEXP__WORD_p_t word )
{
    size_t      next        = pos;
    const char  *str        = expr->expr_str;
    char        buff[512];
    char        *pbuff      = buff;

    for ( next = pos ; isalpha( str[next] ) || isdigit( str[next] ) ; ++next )
        *pbuff++ = tolower( str[next] );
    *pbuff = '\000';

    word->str = &str[pos];
    word->len = next - pos;
    word->token = AEXP__get_token( buff );
    if ( word->token != NULL )
        word->value = word->token->value;

    return next;
}

static size_t
extract_number( AEXP__EXPR_p_t expr, size_t pos, AEXP__WORD_p_t word )
{
    size_t      next    = pos;
    const char  *str    = expr->expr_str;
    CDA_BOOL_t  done    = CDA_FALSE;
    int         ndp     = 0;

    while ( !done )
        if ( isdigit( str[next] ) )
            ++next;
        else if ( str[next] == '.' )
        {
            ++ndp;
            ++next;
        }
        else
            done = CDA_TRUE;

    if ( ndp > 1 )
        word->token = NULL;
    else
    {
        word->str = &str[pos];
        word->len = next - pos;
        word->token = AEXP__get_token( "x" );
        CDA_ASSERT( word->token != NULL );
        sscanf( word->str, "%lf", &word->value );
    }

    return next;
}

static size_t
extract_symbol( AEXP__EXPR_p_t expr, size_t pos, AEXP__WORD_p_t word )
{
    size_t          next    = pos;
    const char      *str    = expr->expr_str;
    AEXP__TOKEN_p_t token   = NULL;
    char            buff[3];

    /* Some symbols come in one or two character strings; for example, we must
     * be able to distinguish '*' (multiply) from '**' (exponentiate).
     */
    buff[1] = buff[2] = '\000';
    buff[0] = str[next++];
    if ( str[next] != '\000' && !isalnum( str[next] ) )
    {
        buff[1] = str[next++];
        if ( (token = AEXP__get_token( buff )) == NULL )
        {
            --next;
            buff[1] = '\000';
        }
    }

    /* Fall inside here if symbol consists of a single character. Substitute
     * internal names for unary plus and unary minus.
     */
    if ( token == NULL )
    {
        if ( is_leading( expr, VEC_end( expr->infix ) ) )
        {
            if ( *buff == '+' )
                *buff = *AEXP__UP;
            else if ( *buff == '-' )
                *buff = *AEXP__UM;
            else
                ;
        }
        token = AEXP__get_token( buff );
    }

    word->str = &str[pos];
    word->len = next - pos;
    word->token = token;
    if ( token != NULL )
        word->value = token->value;

    return next;
}

static CDA_BOOL_t is_leading( AEXP__EXPR_p_t expr, AEXP__WORD_p_t pos )
{
    CDA_BOOL_t      rcode   = CDA_FALSE;
    AEXP__WORD_p_t  first   = VEC_begin( expr->infix );

    if ( pos == first )
        rcode = CDA_TRUE;
    else
    {
        AEXP__TOKEN_p_t  token   = (--pos)->token;
        if ( token != NULL )
            if ( token->type == AEXP__LEFT_PAREN ||
                 token->type == AEXP__UNARY_OP   ||
                 token->type == AEXP__BINARY_OP
               )
               rcode = CDA_TRUE;
    }

    return rcode;
}

static CDA_BOOL_t xlate_to_postfix( AEXP__EXPR_p_t expr )
{
    CDA_BOOL_t      rcode   = CDA_TRUE;
    AEXP__WORD_p_t  next_in = VEC_begin( expr->infix );
    AEXP__WORD_p_t  last_in = VEC_end( expr->infix );
    AEXP__WORD_p_t  item    = NULL;
    size_t          size    = VEC_get_curr_size( expr->infix );
    STK_ID_t        stack   = STK_create_stack( size );

    for ( ; next_in != last_in ; ++next_in )
    {
        AEXP__TOKEN_p_t  token   = next_in->token;
        CDA_BOOL_t      term    = CDA_FALSE;

        switch ( token->type )
        {
        case AEXP__OPERAND:
            VEC_append( expr->postfix, next_in );
            break;

        case AEXP__LEFT_PAREN:
            STK_push_item( stack, next_in );
            break;

        case AEXP__RIGHT_PAREN:
            CDA_ASSERT( !STK_is_stack_empty( stack ) );
            for ( item = STK_pop_item( stack )         ;
                  item->token->type != AEXP__LEFT_PAREN ;
                  item = STK_pop_item( stack )
                )
                VEC_append( expr->postfix, item );
            break;

        case AEXP__UNARY_OP:
        case AEXP__BINARY_OP:
            term = CDA_FALSE;
            while ( !term && !STK_is_stack_empty( stack ) )
            {
                item = STK_peek_item( stack );
                if ( item->token->type == AEXP__LEFT_PAREN )
                    term = CDA_TRUE;
                else if ( item->token->priority < token->priority )
                    term = CDA_TRUE;
                /*
                else if ( item->token->priority == token->priority &&
                          token->priority == AEXP__MAX_PRIORITY
                        )
                    term = CDA_TRUE;
                    */
                else
                {
                    STK_pop_item( stack );
                    VEC_append( expr->postfix, item );
                }
            }
            STK_push_item( stack, next_in );
            break;

        default:
            CDA_ASSERT( CDA_FALSE );
            break;
        }
    }

    while ( !STK_is_stack_empty( stack ) )
    {
        item = STK_pop_item( stack );
        VEC_append( expr->postfix, item );
    }

    STK_destroy_stack( stack );
    return rcode;
}

static CDA_BOOL_t evaluate_expr( AEXP__EXPR_p_t expr )
{
    CDA_BOOL_t      rcode   = CDA_TRUE;
    AEXP__WORD_p_t  next    = VEC_begin( expr->postfix );
    AEXP__WORD_p_t  end     = VEC_end( expr->postfix );
    size_t          size    = VEC_get_curr_size( expr->postfix );
    STK_ID_t        stack   = STK_create_stack( size );
    double          *temp   = NULL;

    for ( ; rcode && next != end ; ++next )
    {
        AEXP__TOKEN_p_t token   = next->token;
        double          ops[2];
        double          *pops   = ops;

        switch ( token->type )
        {
        case AEXP__OPERAND:
            STK_push_item( stack, &next->value );
            break;
        case AEXP__BINARY_OP:
            temp = STK_pop_item( stack );
            ops[1] = *temp;
            // fall-through
        case AEXP__UNARY_OP:
            temp = STK_pop_item( stack );
            ops[0] = *temp;
            errno = 0;
            next->value = token->evaluate( ops );
            if ( errno != 0 )
            {
                rcode = CDA_FALSE;
                expr->errnum = errno;
            }
            else if ( _isnan( next->value ) || next->value > DBL_MAX )
            {
                rcode = CDA_FALSE;
                expr->errnum = ERANGE;
            }
            else
                STK_push_item( stack, &next->value );
            break;
        default:
            CDA_ASSERT( CDA_FALSE );
            break;
        }
    }

    if ( rcode )
    {
        temp = STK_pop_item( stack );
        expr->value = *temp;
    }

    STK_destroy_stack( stack );
    return rcode;
}
