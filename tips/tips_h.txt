#ifndef TIPS_H
#define TIPS_H

void TIPS_addReceipt(
    const char *waitress,
    double     check,
    double     tip
);

void TIPS_close(
    void
);

void TIPS_init(
    void
);

void TIPS_printReceipts(
    void
);


#endif /* #ifndef TIPS_H */
