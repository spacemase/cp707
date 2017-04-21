#include <stkx.h>
#include <stdlib.h>
#include <stdio.h>

/* Macro must be invoked in the global area of the source file.
 * This will create a a stack of 20 ints; the push and pop operators
 * will be static functions named PUSH_joe and POP_joe.
 */
STKX_NEW( static, int, 20, joe );

int main( int argc, char **argv )
{
    int inx = 0;

    for ( inx = 0 ; inx < 10 ; ++inx )
    {
        int item    = inx * 100;
        printf( "Pushing %d\n", item );
        PUSH_joe( item );
    }

    puts( "\n***************\n" );

    for ( inx = 0 ; inx < 10 ; ++inx )
    {
        int item    = POP_joe();
        printf( "Popping %d\n", item );
    }

    return EXIT_SUCCESS;
}
