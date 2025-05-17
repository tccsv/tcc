#pragma once
#include <pthread.h>
#include <stdlib.h>
#include "TCC/TCC_money.h"
#ifndef TCC_MONEY_H
#define TCC_MONEY_H

#include "TCC_error.h"



#ifdef __cplusplus
extern "C"
{
#endif

typedef uint_least64_t TCC_Money;

typedef struct TCC_MoneyNotes TCC_MoneyNotes;

typedef uint_least8_t TCC_DateDay;
typedef uint_least8_t TCC_DateMonth;
typedef uint_least16_t TCC_DateYear;

typedef struct TCC_Date
{
    TCC_DateDay day;
    TCC_DateMonth month;
    TCC_DateYear year; 
} TCC_Date;

TCC_Error TCC_moneyNotesCreate(TCC_MoneyNotes** moneyNoterPP, const size_t length);
void TCC_moneyNotesDel(TCC_MoneyNotes** moneyNoterPP);

TCC_Error TCC_moneyNotesInsert(TCC_MoneyNotes *moneyNotesP, const TCC_Money money,
                               const TCC_Date date);
size_t TCC_moneyNotesGetCapacity(const TCC_MoneyNotes* moneyNotesP);

TCC_Money TCC_moneyYearSum(const TCC_MoneyNotes * notesPtr, const TCC_DateYear year);

TCC_Error TCC_moneyNotesLoadTmp(TCC_MoneyNotes* moneyNotesP);



#ifdef __cplusplus
}
#endif


#endif