#include <TCC/TCC_money.h>
#include <TCC/TCC_STS.h>

//для printf
#include <stdio.h>

// //для таймера
// #include <TCC/utils/TCC_timer.h>

int main(void)
{
	size_t initLength = 100000;
	TCC_MoneyNotes * moneyNotes;
	TCC_moneyNotesCreate(&moneyNotes, initLength);
	TCC_moneyNotesLoadTmp(moneyNotes);
	TCC_Money res = TCC_STSprofitsTax(moneyNotes, 2000);
	printf("\n res: %llu\n", res);
	TCC_moneyNotesDel(&moneyNotes);

	return 0;
}