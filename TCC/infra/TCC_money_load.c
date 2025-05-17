// TCC_money_load.c
#include "TCC/TCC_money.h"
#include "DataBase/postgres/tcc_db.h"
#include <stddef.h>

TCC_Error TCC_moneyNotesLoadTmp(TCC_MoneyNotes *moneyNotesP) {
    TCC_Error err = TCC_dbInit();
    if (err != TCC_ERROR_OK) {
        return err;
    }

    err = TCC_dbLoadMoneyNotes(moneyNotesP);
    TCC_dbClose();

    return err;
}
