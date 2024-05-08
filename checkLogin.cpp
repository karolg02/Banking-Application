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

bool getBalance(int userid, int& balance) {
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

bool connect_for_transaction(int& userid, std::string& adres_odbiorcy, int& balance_adresata, int& balance_odbiorcy) {
    MYSQL mysql;
    mysql_init(&mysql);

    if (!mysql_real_connect(&mysql, "127.0.0.1", "root", "", "bazad", 0, NULL, 0)) {
        std::cerr << "Błąd połączenia z bazą danych: " << mysql_error(&mysql) << std::endl;
        return false;
    }

    // Rozpoczęcie transakcji
    if (mysql_query(&mysql, "START TRANSACTION")) {
        std::cerr << "Błąd rozpoczęcia transakcji: " << mysql_error(&mysql) << std::endl;
        mysql_close(&mysql);
        return false;
    }

    // Aktualizacja salda adresata
    std::string update_balance_query = "UPDATE balance SET balance = " + std::to_string(balance_adresata) + " WHERE user_id = " + std::to_string(userid);

    if (mysql_query(&mysql, update_balance_query.c_str())) {
        std::cerr << "Błąd aktualizacji salda adresata: " << mysql_error(&mysql) << std::endl;
        mysql_query(&mysql, "ROLLBACK");
        mysql_close(&mysql);
        return false;
    }

    // Pobranie id odbiorcy na podstawie adresu email
    std::string select_id_query = "SELECT id FROM users WHERE email = '" + adres_odbiorcy + "'";
    if (mysql_query(&mysql, select_id_query.c_str())) {
        std::cerr << "Błąd pobierania id odbiorcy: " << mysql_error(&mysql) << std::endl;
        mysql_query(&mysql, "ROLLBACK");
        mysql_close(&mysql);
        return false;
    }
    MYSQL_RES* id_result = mysql_store_result(&mysql);
    if (!id_result) {
        std::cerr << "Błąd pobierania wyników: " << mysql_error(&mysql) << std::endl;
        mysql_query(&mysql, "ROLLBACK");
        mysql_close(&mysql);
        return false;
    }
    MYSQL_ROW id_row = mysql_fetch_row(id_result);
    if (!id_row) {
        std::cerr << "Nie znaleziono użytkownika o adresie email: " << adres_odbiorcy << std::endl;
        mysql_free_result(id_result);
        mysql_query(&mysql, "ROLLBACK");
        mysql_close(&mysql);
        return false;
    }
    int odbiorca_id = std::stoi(id_row[0]);
    mysql_free_result(id_result);

    // Aktualizacja salda odbiorcy
    std::string update_balance_odbiorcy_query = "UPDATE balance SET balance = balance + " + std::to_string(balance_odbiorcy) + " WHERE user_id = " + std::to_string(odbiorca_id);

    if (mysql_query(&mysql, update_balance_odbiorcy_query.c_str())) {
        std::cerr << "Błąd aktualizacji salda odbiorcy: " << mysql_error(&mysql) << std::endl;
        mysql_query(&mysql, "ROLLBACK");
        mysql_close(&mysql);
        return false;
    }

    // Zatwierdzenie transakcji
    if (mysql_query(&mysql, "COMMIT")) {
        std::cerr << "Błąd zatwierdzenia transakcji: " << mysql_error(&mysql) << std::endl;
        mysql_close(&mysql);
        return false;
    }

    std::string data = "CURDATE()";
    std::string godzina = "CURTIME()";
    std::string history_add = "INSERT INTO history (id, email_odbiorcy, kwota, data, godzina) VALUES (" + std::to_string(userid) + ", '" + adres_odbiorcy + "', " + std::to_string(balance_odbiorcy) + ", " + data + ", " + godzina + ")";
    mysql_query(&mysql, history_add.c_str());

    mysql_close(&mysql);
    return true;
}

bool history_load(int& userid, int& total_transactions){
    MYSQL mysql;
    mysql_init(&mysql);
    
    if (!mysql_real_connect(&mysql, "127.0.0.1", "root", "", "bazad", 0, NULL, 0)) {
        std::cerr << "Błąd połączenia z bazą danych: " << mysql_error(&mysql) << std::endl;
        return false;
    }
    std::string count_transactions = "SELECT COUNT(*) FROM history WHERE id = " + std::to_string(userid) + ";";

    mysql_query(&mysql, count_transactions.c_str());

    MYSQL_RES* count_result = mysql_store_result(&mysql);

    MYSQL_ROW count_row = mysql_fetch_row(count_result);
    total_transactions = std::stoi(count_row[0]);

    mysql_free_result(count_result);

    std::string get_history = "SELECT * FROM history WHERE id = " + std::to_string(userid) + ";";

    //tu kod zapisujacy dane do wektora i guess
    
    mysql_close(&mysql);
    return true;
}