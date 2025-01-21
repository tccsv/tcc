#include <TCC/TCC_STS.h>


TCC_Money TCC_STSprofitsTax(const TCC_MoneyNotes * profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);
    const TCC_Money potentialRes = TCC_STS_PROFITS_TAX_RATE * profits;
    const TCC_Money minTax = TCC_STS_PROFITS_MIN_TAX_RATE * profits;

    return potentialRes > minTax ? potentialRes : minTax;
}

//УСН Доходы минус расходы
#define TCC_STS_PROFITS_MINUS_LOSSES_TAX_RATE 0.15f
//TODO Проверить минималку и её условия
#define TCC_STS_PROFITS_MINUS_LOSSES_MIN_TAX_RATE 0.01f
TCC_Money TCC_STSprofitsMinusLossesTax(const TCC_MoneyNotes * profitsHistoryPtr, 
    const TCC_MoneyNotes * losesHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);
    const TCC_Money loses = TCC_moneyYearSum(profitsHistoryPtr, year);
    const TCC_Money potentialRes = TCC_STS_PROFITS_TAX_RATE * (profits-loses);
    const TCC_Money minTax = TCC_STS_PROFITS_MIN_TAX_RATE * profits;

    return potentialRes > minTax ? potentialRes : minTax;    
}