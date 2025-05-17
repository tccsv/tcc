// DataBase/postgres/tcc_db.h
#ifndef TCC_DB_H
#define TCC_DB_H

#include <libpq-fe.h>
#include "TCC/TCC_money.h"
#include "TCC/TCC_error.h"


// Инициализация и закрытие соединения с БД
TCC_Error TCC_dbInit(void);

void TCC_dbClose(void);

// Операции с денежными записями
TCC_Error TCC_dbInsertMoneyNote(TCC_Money amount, TCC_Date date);

TCC_Error TCC_dbLoadMoneyNotes(TCC_MoneyNotes *moneyNotesP);

TCC_Error TCC_dbUpdateMoneyNote(size_t id, TCC_Money amount, TCC_Date date);

TCC_Error TCC_dbDeleteMoneyNote(size_t id);

#endif

void TCC_dbShowAllRecords(void);
