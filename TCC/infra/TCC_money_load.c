#include <TCC/TCC_money.h>

TCC_Error TCC_moneyNotesLoadTmp(TCC_MoneyNotes* moneyNotesP)
{
    const TCC_Date dateTmp = {.day = 1, .month = 1, .year = 2000};
    const TCC_Money moneyTmp = 1000;
    const size_t capacityTmp = TCC_moneyNotesGetCapacity(moneyNotesP);
    for(size_t i = 0; i < capacityTmp; ++i)
	{
		TCC_moneyNotesInsert(moneyNotesP, moneyTmp, dateTmp);
	}
    return TCC_ERROR_MISSING;    
}