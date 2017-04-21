#include <stdio.h>
#include <stdlib.h>
#include <cda.h>
#include <stack_calc.h>

typedef struct expr_descr_s
{
    const char  *expr;
    long        expectedResult;
} EXPR_DESC_t, *EXPR_DESC_p_t;

static EXPR_DESC_t exprDescs_[] =
{
    { "add( 5, 10 )",
      15
    },
    { "add( -10, -20 )",
      -30
    },
    { "add( 20, add( 30, 35 ) )",
      85
    },
    { "add( add( 20, 15 ), 35 )",
      70
    },
    { "add( add( 5, -3 ), add( 10, 35 ) )",
      47
    },
    { "lor( 0x10, 0x20, 0x200 )",
       0x230
    },
    { "add( 10, lor( 0x10, 0x20, 0x200 ) )",
       570
    },
    { "sub( 5, 10 )",
       -5
    },
    { "mul( 100, 32 )",
      3200
    },
    { "div( 50, 25 )",
      2
    },
    { "xor( 0x100, 0x200, 0x400, 0x200 )",
      0x500
    },
    { "and( 0xffff, 0x500, 0x600 )",
      0x400
    },
    { "lsh( 0x201, 2 )",
      0x804
    },
    { "rsh( 0x804, 2 )",
      0x201
    },
    { "sum( 10, 15, 20, 25, -5, -10 )",
      55
    },
    { "and( 0xff, sum( mul(10, 50 ), div( 24, 3 ), add( mul( 2, 4 ), 2 ) ) )",
      6
    }
};

int main( int *argc, char **argv )
{
    int rcode   = EXIT_SUCCESS;
    int inx     = 0;

    for ( inx = 0 ; inx < CDA_CARD( exprDescs_ ) ; ++inx )
    {
        long    result  = 0;
        printf( "Testing (%d) ... \n", inx );
        printf( "\t%s\n", exprDescs_[inx].expr );
        printf( "\texpected result = %ld\n", exprDescs_[inx].expectedResult );
        result = StackCalc( exprDescs_[inx]. expr, NULL );
        printf( "\tactual result   = %ld\n", result );
        if ( result == exprDescs_[inx].expectedResult )
        {
            printf( "\tsuccess\n" );
        }
        else
        {
            printf( "\tfailure\n" );
            rcode = EXIT_FAILURE;
        }
    }

    return rcode;
}
