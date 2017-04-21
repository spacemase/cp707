#include <cda.h>
#include <enq.h>
#include <tipsp.h>

#define NOT_FOUND      (0)
#define FOUND_EXACT    (1)
#define FOUND_GREATER  (2)

typedef struct receipts_s
{
    ENQ_ITEM_t item;
    double     checkTotal;
    double     tipTotal;
    int        numReceipts;
} RECEIPTS_t, *RECEIPTS_p_t;

static const char *tipListName = "Tip Queue";

static ENQ_ANCHOR_p_t anchor = NULL;

void TIPS_init( void )
{
    CDA_ASSERT( anchor == NULL );
    anchor = ENQ_create_list( tipListName );
}

void TIPS_addReceipt( const char *waitperson, double check, double tip )
{
    RECEIPTS_p_t receipts = NULL;
    RECEIPTS_p_t bucket   = NULL;
    int          result   = NOT_FOUND;
    int          compare  = 0;

    CDA_ASSERT( anchor != NULL );

    receipts = (RECEIPTS_p_t)ENQ_GET_HEAD( anchor );
    while ( (result == NOT_FOUND) && ((ENQ_ANCHOR_p_t)receipts != anchor) )
    {
        compare = strcmp( waitperson, ENQ_GET_ITEM_NAME( (ENQ_ITEM_p_t)receipts ));
        if ( compare == 0 )
            result = FOUND_EXACT;
        else if ( compare < 0 )
            result = FOUND_GREATER;
        else
            receipts = (RECEIPTS_p_t)ENQ_GET_NEXT( (ENQ_ITEM_p_t)receipts );
    }

    switch ( result )
    {
        case FOUND_EXACT:
            receipts->checkTotal += check;
            receipts->tipTotal += tip;
            ++receipts->numReceipts;
            break;
  
        case FOUND_GREATER:
            bucket = (RECEIPTS_p_t)ENQ_create_item( waitperson, sizeof(RECEIPTS_t) );
            bucket->checkTotal = check;
            bucket->tipTotal = tip;
            bucket->numReceipts = 1;
            ENQ_add_before( (ENQ_ITEM_p_t)bucket, (ENQ_ITEM_p_t)receipts );
            break;

        case NOT_FOUND:
            bucket = (RECEIPTS_p_t)ENQ_create_item( waitperson, sizeof(RECEIPTS_t) );
            bucket->checkTotal = check;
            bucket->tipTotal = tip;
            bucket->numReceipts = 1;
            ENQ_add_tail( anchor, (ENQ_ITEM_p_t)bucket );
            break;

        default:
            CDA_ASSERT( CDA_FALSE );
            break;
    }
}

void TIPS_printReceipts( void )
{
    RECEIPTS_p_t receipts = NULL;

    CDA_ASSERT( anchor != NULL );

    receipts = (RECEIPTS_p_t)ENQ_GET_HEAD( anchor );
    while ( receipts != (RECEIPTS_p_t)anchor )
    {
        printf( "%s\n", ENQ_GET_ITEM_NAME( (ENQ_ITEM_p_t)receipts ) );
        printf( "Total receipts: %.2f (Average: %.2f)\n",
                receipts->checkTotal,
                receipts->checkTotal / receipts->numReceipts
              );
        printf( "Total tips: %.2f (Average: %.2f)\n",
                receipts->tipTotal,
                receipts->tipTotal / receipts->numReceipts
              );
        printf( "\n" );
        receipts = (RECEIPTS_p_t)ENQ_GET_NEXT( (ENQ_ITEM_p_t)receipts );
    }
}

void TIPS_close( void )
{
    CDA_ASSERT( anchor != NULL );
    ENQ_destroy_list( anchor );
    anchor = NULL;
}