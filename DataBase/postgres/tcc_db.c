// DataBase/postgres/tcc_db.c
#include "tcc_db.h"
#include "db_config.h"
#include <stdio.h>
#include <string.h>

static PGconn* conn = NULL;

TCC_Error TCC_dbInit(void) {
    char conninfo[256];
    snprintf(conninfo, sizeof(conninfo),
             "host=%s port=%s dbname=%s user=%s password=%s",
             DB_HOST, DB_PORT, DB_NAME, DB_USER, DB_PASSWORD);

    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        conn = NULL;
        return TCC_ERROR_DB_CONNECTION;
    }

    // Создание таблицы, если она не существует
    const char* create_table =
        "CREATE TABLE IF NOT EXISTS money_notes ("
        "id SERIAL PRIMARY KEY,"
        "amount DECIMAL(15,2) NOT NULL,"
        "note_date DATE NOT NULL,"
        "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
        ")";

    PGresult* res = PQexec(conn, create_table);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        PQclear(res);
        return TCC_ERROR_DB_QUERY;
    }
    PQclear(res);

    return TCC_ERROR_OK;
}

void TCC_dbClose(void) {
    if (conn) {
        PQfinish(conn);
        conn = NULL;
    }
}

TCC_Error TCC_dbInsertMoneyNote(TCC_Money amount, TCC_Date date) {
    if (!conn) return TCC_ERROR_DB_CONNECTION;

    char query[256];
    snprintf(query, sizeof(query),
             "INSERT INTO money_notes (amount, note_date) VALUES (%llu, '%04d-%02d-%02d')",
             amount, date.year, date.month, date.day);

    PGresult* res = PQexec(conn, query);
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        PQclear(res);
        return TCC_ERROR_DB_QUERY;
    }
    PQclear(res);

    return TCC_ERROR_OK;
}

TCC_Error TCC_dbLoadMoneyNotes(TCC_MoneyNotes* moneyNotesP) {
    if (!conn) return TCC_ERROR_DB_CONNECTION;

    const char* query = "SELECT amount, note_date FROM money_notes ORDER BY note_date";
    PGresult* res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return TCC_ERROR_DB_QUERY;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        TCC_Money amount = atol(PQgetvalue(res, i, 0));
        const char* date_str = PQgetvalue(res, i, 1);

        TCC_Date date;
        sscanf(date_str, "%d-%d-%d", &date.year, &date.month, &date.day);

        TCC_moneyNotesInsert(moneyNotesP, amount, date);
        // Уберите проверку на TCC_ERROR_OK, так как функция void
    }

    PQclear(res);
    return TCC_ERROR_OK;
}

void TCC_dbShowAllRecords(void) {
    if (!conn) return;

    const char* query = "SELECT id, amount, note_date, created_at FROM money_notes ORDER BY note_date";
    PGresult* res = PQexec(conn, query);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Query failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    printf("\nЗаписи в таблице money_notes (%d строк):\n", rows);
    if (rows > 0) {
        printf("%-5s %-15s %-12s %-20s\n", "ID", "Сумма", "Дата", "Создано");
        printf("--------------------------------------------------\n");

        for (int i = 0; i < rows; i++) {
            printf("%-5s %-15s %-12s %-20s\n",
                PQgetvalue(res, i, 0),  // id
                PQgetvalue(res, i, 1),  // amount
                PQgetvalue(res, i, 2),  // note_date
                PQgetvalue(res, i, 3)); // created_at
        }
    } else {
        printf("Таблица пуста\n");
    }

    PQclear(res);
}