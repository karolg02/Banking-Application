#include "headers.h"

bool checkLogin(const std::string& email, const std::string& password, int &userid) {
    // Połączenie z bazą danych
    MYSQL mysql;
    mysql_init(&mysql);
    if (!mysql_real_connect(&mysql, "127.0.0.1", "root", "", "bazad", 0, NULL, 0)) {
        std::cerr << "Błąd połączenia z bazą danych: " << mysql_error(&mysql) << std::endl;
        return false;
    }

    // Zapytanie SQL
    std::string query = "SELECT id FROM users WHERE email = '" + email + "' AND password = '" + password + "'";

    // Wykonanie zapytania
    if (mysql_query(&mysql, query.c_str())) {
        std::cerr << "Błąd zapytania: " << mysql_error(&mysql) << std::endl;
        mysql_close(&mysql);
        return false;
    }

    // Pobranie wyników zapytania
    MYSQL_RES* result = mysql_store_result(&mysql);
    if (!result) {
        std::cerr << "Błąd pobierania wyników: " << mysql_error(&mysql) << std::endl;
        mysql_close(&mysql);
        return false;
    }

    // Sprawdzenie liczby rekordów
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) {
        userid = std::stoi(row[0]);
    }
    else {
        userid = -1; // Jeśli nie znaleziono użytkownika, ustaw ID na -1
    }

    // Zwolnienie pamięci po wynikach
    mysql_free_result(result);

    // Zamknięcie połączenia z bazą danych
    mysql_close(&mysql);

    // Zwrócenie wartości logicznej na podstawie wyników zapytania
    return userid != -1;
}

bool getBalance(int userid, double& balance) {
    MYSQL mysql;
    mysql_init(&mysql);

    if (!mysql_real_connect(&mysql, "127.0.0.1", "root", "", "bazad", 0, NULL, 0)) {
        std::cerr << "Błąd połączenia z bazą danych: " << mysql_error(&mysql) << std::endl;
        return false;
    }

    std::string query = "SELECT balance FROM balance WHERE id = " + std::to_string(userid);

    if (mysql_query(&mysql, query.c_str())) {
        std::cerr << "Błąd zapytania: " << mysql_error(&mysql) << std::endl;
        mysql_close(&mysql);
        return false;
    }

    MYSQL_RES* result = mysql_store_result(&mysql);
    if (!result) {
        std::cerr << "Błąd pobierania wyników: " << mysql_error(&mysql) << std::endl;
        mysql_close(&mysql);
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) {
        balance = std::stoi(row[0]);
    } else {
        std::cerr << "Brak rekordu dla podanego id" << std::endl;
        mysql_free_result(result);
        mysql_close(&mysql);
        return false;
    }

    mysql_free_result(result);
    mysql_close(&mysql);
    return true;
}
