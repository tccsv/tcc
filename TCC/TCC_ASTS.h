#ifndef TCC__ASTS_H
#define TCC__ASTS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

//УСН Доходы
#define TCC_ASTS_PROFITS_TAX_RATE 0.08f
//TODO проверить иинималку
#define TCC_ASTS_PROFITS_MIN_TAX_RATE 0.01f

TCC_Money TCC_ASTSprofitsTax(const TCC_MoneyNotes * profitsHistoryPtr, 
    const TCC_DateYear year);
TCC_Money TCC_ASTSprofitsMinusLosesTax(const TCC_MoneyNotes * profitsHistoryPtr, 
    const TCC_DateYear year);



#ifdef __cplusplus
}
#endif


#endif