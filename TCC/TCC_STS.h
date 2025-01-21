#ifndef TCC__STS_H
#define TCC__STS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

//УСН Доходы
//TODO проверить минимальный процент
#define TCC_STS_PROFITS_TAX_RATE 0.06f
//TODO проверить иинималку
#define TCC_STS_PROFITS_MIN_TAX_RATE 0.01f


TCC_Money TCC_STSprofitsTax(const TCC_MoneyNotes * profitsHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

