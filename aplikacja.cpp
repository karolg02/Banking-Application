#include "headers.h"
#include "checkLogin.cpp"
#include "ksztalty.cpp"

enum State {
    Login,
    Rejected,
    MyAccount,
    Transfers,
    History,
};

int main() {
    // Stworzenie okna
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bank Kar SA");

    // Początkowy stan aplikacji
    State currentState = State::Login;

    int userid;
    int balance;
    int kwotaInput = 0;
    int total_transactions;

    // Stworzenie przycisku
    sf::RectangleShape button(sf::Vector2f(150, 50));
    button.setPosition(325, 300);
    button.setFillColor(sf::Color(250, 184, 86));

    sf::RectangleShape container(sf::Vector2f(400,500));
    container.setPosition(200,50);
    container.setFillColor(sf::Color(0, 120, 52));

    // Pobranie domyślnej czcionki systemowej
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Błąd ładowania czcionki." << std::endl;
        return 1;
    }

    // Tekst na przycisku
    sf::Text buttonText("Zaloguj sie!", font, 20);
    buttonText.setPosition(340, 315);
    buttonText.setFillColor(sf::Color::Black);

    // Pole tekstowe dla email
    sf::Text emailText("Email", font, 16);
    emailText.setPosition(276, 180);
    emailText.setFillColor(sf::Color::Black);

    sf::RectangleShape emailBox(sf::Vector2f(250, 20));
    emailBox.setPosition(275, 180);
    emailBox.setFillColor(sf::Color::White);
    emailBox.setOutlineThickness(1);
    emailBox.setOutlineColor(sf::Color::Black);

    // Pole tekstowe dla hasła
    sf::Text passwordText("Haslo", font, 16);
    passwordText.setPosition(276, 240);
    passwordText.setFillColor(sf::Color::Black);

    sf::RectangleShape passwordBox(sf::Vector2f(250, 20));
    passwordBox.setPosition(275, 240);
    passwordBox.setFillColor(sf::Color::White);
    passwordBox.setOutlineThickness(1);
    passwordBox.setOutlineColor(sf::Color::Black);

    sf::Text odbiorcaText("Odbiorca: ",font,24);
    odbiorcaText.setPosition(115,220);
    odbiorcaText.setFillColor(sf::Color::Black);

    sf::RectangleShape odbiorcaBox(sf::Vector2f(400, 28));
    odbiorcaBox.setPosition(235, 220);
    odbiorcaBox.setFillColor(sf::Color::White);
    odbiorcaBox.setOutlineThickness(1);
    odbiorcaBox.setOutlineColor(sf::Color::Black);

    sf::Text odbiorcaTEKST("",font,24);
    odbiorcaTEKST.setPosition(236,220);
    odbiorcaTEKST.setFillColor(sf::Color::Black);

    sf::RectangleShape odbiorcaBoxShadow = odbiorcaBox;
    odbiorcaBoxShadow.setPosition(235+2, 220+2);
    odbiorcaBoxShadow.setFillColor(sf::Color(30, 30, 30));

    sf::Text kwotaTxt("Kwota: ", font, 24);
    kwotaTxt.setPosition(115,280);
    kwotaTxt.setFillColor(sf::Color::Black);

    sf::RectangleShape kwotaBox(sf::Vector2f(200,28));
    kwotaBox.setPosition(235, 280);
    kwotaBox.setFillColor(sf::Color::White);
    kwotaBox.setOutlineThickness(1);
    kwotaBox.setOutlineColor(sf::Color::Black);

    sf::Text kwotaTEKST((std::ostringstream{} << std::fixed << std::setprecision(2) << kwotaInput).str(), font, 24);
    kwotaTEKST.setPosition(236,280);
    kwotaTEKST.setFillColor(sf::Color::Black);
    
    sf::RectangleShape kwotaBoxShadow = kwotaBox;
    kwotaBoxShadow.setPosition(235 + 2, 280 + 2);
    kwotaBoxShadow.setFillColor(sf::Color(30, 30, 30));

    sf::RectangleShape przelejButton(sf::Vector2f(200,50));
    przelejButton.setPosition(300,400);
    przelejButton.setFillColor(sf::Color(250, 184, 86));
    przelejButton.setOutlineThickness(1);
    przelejButton.setOutlineColor(sf::Color::Black);

    sf::Text przelejButtonText("Przelej", font, 24);
    przelejButtonText.setPosition(360,413);
    przelejButtonText.setFillColor(sf::Color::Black);

    sf::RectangleShape przelejButtonShadow = przelejButton;
    przelejButtonShadow.setPosition(300+2,400+2);
    przelejButtonShadow.setFillColor(sf::Color(30, 30, 30));

    sf::RectangleShape bar(sf::Vector2f(800, 80));
    bar.setPosition(0, 0);
    bar.setFillColor(sf::Color(0, 120, 52));

    sf::RectangleShape myAcc(sf::Vector2f(180,50));
    myAcc.setPosition(16,15);
    myAcc.setFillColor(sf::Color(241, 178, 86));
    myAcc.setOutlineThickness(1);
    myAcc.setOutlineColor(sf::Color::Black);

    sf::RectangleShape myPayments(sf::Vector2f(180,50));
    myPayments.setPosition(212,15);
    myPayments.setFillColor(sf::Color(241, 178, 86));
    myPayments.setOutlineThickness(1);
    myPayments.setOutlineColor(sf::Color::Black);

    sf::RectangleShape myHistory(sf::Vector2f(180,50));
    myHistory.setPosition(408,15);
    myHistory.setFillColor(sf::Color(241, 178, 86));
    myHistory.setOutlineThickness(1);
    myHistory.setOutlineColor(sf::Color::Black);

    sf::RectangleShape quit(sf::Vector2f(180,50));
    quit.setPosition(604,15);
    quit.setFillColor(sf::Color::Red);
    quit.setOutlineThickness(1);
    quit.setOutlineColor(sf::Color::Black);

    sf::RectangleShape doRegistration(sf::Vector2f(200,50));
    doRegistration.setPosition(300, 400);
    doRegistration.setFillColor(sf::Color::Red);

    sf::RectangleShape left_box(sf::Vector2f(400,520));
    left_box.setPosition(0,80);
    left_box.setFillColor(sf::Color(228,214,214));
    
    sf::RectangleShape middle_box(sf::Vector2f(600,425));
    middle_box.setPosition(100,80);
    middle_box.setFillColor(sf::Color(228,214,214));
    middle_box.setOutlineThickness(1);
    middle_box.setOutlineColor(sf::Color::Black);

    sf::RectangleShape middle_boxShadow(sf::Vector2f(600,425));
    middle_boxShadow.setPosition(102,82);
    middle_boxShadow.setFillColor(sf::Color::Black);

    sf::Text doRegistrationText("Zarejestruj sie!",font,20);
    doRegistrationText.setPosition(320,415);
    doRegistrationText.setFillColor(sf::Color::Black);

    sf::Text textRegistration("Nie masz konta? Zarejestruj sie ponizej", font,12);
    textRegistration.setPosition(285,380);
    textRegistration.setFillColor(sf::Color::Black);

    sf::Text bank("Bank Kar SA",font,30);
    bank.setPosition(315,100);
    bank.setFillColor(sf::Color::Black);

    sf::Text doesntExists("Nie istnieje uzytkownik o takim adresie!",font, 16);
    doesntExists.setPosition(240,200);
    doesntExists.setFillColor(sf::Color::Red);

    sf::Text transAccepted("Transakcja przebiegla pomyslnie!",font, 16);
    transAccepted.setPosition(240,200);
    transAccepted.setFillColor(sf::Color(0, 120, 52));

    sf::Text myAccText("Moje konto",font,24);
    myAccText.setPosition(45,26);
    myAccText.setFillColor(sf::Color::Black);

    sf::Text myPaymentsText("Platnosci",font,24);
    myPaymentsText.setPosition(250,26);
    myPaymentsText.setFillColor(sf::Color::Black);

    sf::Text myHistoryText("Historia", font,24);
    myHistoryText.setPosition(450,26);
    myHistoryText.setFillColor(sf::Color::Black);

    sf::Text quitText("Wyjdz",font,24);
    quitText.setPosition(655,26);
    quitText.setFillColor(sf::Color::Black);

    sf::Text RejectedText("Bledne dane, nie zalogowano",font,12);
    RejectedText.setPosition(315,150);
    RejectedText.setFillColor(sf::Color::Black);

    // Zmienne przechowujące wprowadzone dane
    std::string emailInput = "";
    std::string passwordInput = "";
    std::string odbiorcaInput = "";

    // Zmienne wskazujące, które pole jest aktualnie aktywne
    bool emailActive = false;
    bool passwordActive = false;
    bool kwotaActive = false;
    bool odbiorcaActive = false;
    bool isHoveredButton = false;
    bool wrongEmail = false;
    bool transaction_accepted = false;

    // Pętla główna programu
    while (window.isOpen()) {
        // Obsługa zdarzeń
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Sprawdza czy najechany zostal element
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                if (myAcc.getGlobalBounds().contains(mousePosF)) {
                    myAcc.setFillColor(sf::Color(39, 54, 85));
                } else if(myHistory.getGlobalBounds().contains(mousePosF)){
                    myHistory.setFillColor(sf::Color(39, 54, 85));
                } else if(myPayments.getGlobalBounds().contains(mousePosF)){
                    myPayments.setFillColor(sf::Color(39, 54, 85));
                } else if(przelejButton.getGlobalBounds().contains(mousePosF)){
                    przelejButton.setFillColor(sf::Color(39, 54, 85));
                }
                else {
                    myAcc.setFillColor(sf::Color(250, 184, 86));
                    myHistory.setFillColor(sf::Color(250, 184, 86));
                    myPayments.setFillColor(sf::Color(250, 184, 86));
                    przelejButton.setFillColor(sf::Color(250, 184, 86));
                }


                }

                    // Obsługa kliknięcia myszą
                    if (event.type == sf::Event::MouseButtonPressed) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                        if(currentState==Login || currentState==Rejected){
                            // Sprawdź, czy kliknięcie nastąpiło w polu email
                            if (emailBox.getGlobalBounds().contains(mousePosF)) {
                                emailActive = true;
                                passwordActive = false;
                            } else {
                                emailActive = false;
                            }

                            // Sprawdź, czy kliknięcie nastąpiło w polu hasła
                            if (passwordBox.getGlobalBounds().contains(mousePosF)) {
                                passwordActive = true;
                                emailActive = false;
                            } else {
                                passwordActive = false;
                            }
                        }

                        if(currentState==Transfers){
                            if (odbiorcaBox.getGlobalBounds().contains(mousePosF)) {
                                odbiorcaActive = true;
                                kwotaActive =false;
                            } else {
                                odbiorcaActive = false;
                            }

                            if (kwotaBox.getGlobalBounds().contains(mousePosF)) {
                                kwotaActive = true;
                                odbiorcaActive = false;
                            } else {
                                kwotaActive = false;
                            }
                        }

                        // Sprawdź, czy naciśnięto
                        if (button.getGlobalBounds().contains(mousePosF)) {
                            if (checkLogin(emailInput, passwordInput, userid)) {
                                currentState = State::MyAccount; 
                                std::cout<< "Nr id to: " << userid << std:: endl;
                                getBalance(userid, balance);
                            } else {
                                std::cout << "Błedny email lub haslo!" << std::endl;
                                currentState = State::Rejected;
                            }
                        }

                        if(myAcc.getGlobalBounds().contains(mousePosF)){
                            currentState = MyAccount;
                        }

                        if(myPayments.getGlobalBounds().contains(mousePosF)){
                            currentState = Transfers;
                        }

                        if(myHistory.getGlobalBounds().contains(mousePosF)){
                            currentState = History;
                            history_load(userid, total_transactions);
                            std::cout << total_transactions << std::endl;
                        }
                        
                        // Sprawdź czy quit jest nacisniety
                        if (quit.getGlobalBounds().contains(mousePosF)){
                            window.close();
                        }

                        if(przelejButton.getGlobalBounds().contains(mousePosF)){
                            if(!odbiorcaInput.empty()){
                                if(!kwotaInput==0){
                                    balance = balance - kwotaInput;
                                    if(connect_for_transaction(userid,odbiorcaInput,balance, kwotaInput)==false){
                                        wrongEmail = true;
                                        transaction_accepted = false;
                                        balance = balance + kwotaInput;
                                    }else{
                                        wrongEmail = false;
                                        transaction_accepted = true;
                                        odbiorcaInput = "";
                                        kwotaInput = 0;
                                    }
                                }else{
                                    //kwota nie moze byc 0
                                }
                            }
                            else{
                                //window.draw(emptyEmail);
                            }
                        }

                    }
                    // Obsługa wprowadzania tekstu
                    if (event.type == sf::Event::TextEntered) {
                        // Pole email
                        if (emailActive) {
                            if (event.text.unicode == 8 && !emailInput.empty()) { // Backspace
                                emailInput.pop_back();
                            } else if (event.text.unicode < 128 && emailInput.size() < 50 && event.text.unicode != 8) {
                                emailInput += static_cast<char>(event.text.unicode);
                            }
                            emailText.setString(emailInput);
                        }
                        // Pole hasla
                        if (passwordActive) {
                            if (event.text.unicode == 8 && !passwordInput.empty()) { // Backspace
                                passwordInput.pop_back();
                            } else if (event.text.unicode < 128 && passwordInput.size() < 50 && event.text.unicode != 8) {
                                passwordInput += static_cast<char>(event.text.unicode);
                            }
                            passwordText.setString(std::string(passwordInput.size(), '*')); // Zamień hasło na gwiazdki
                        }
                        // Pole odbiorcy
                        if (odbiorcaActive) {
                            if (event.text.unicode == 8 && !odbiorcaInput.empty()) { // Backspace
                                odbiorcaInput.pop_back();
                            } else if (event.text.unicode < 128 && odbiorcaInput.size() < 50 && event.text.unicode != 8) {
                                odbiorcaInput += static_cast<char>(event.text.unicode);
                            }
                            odbiorcaTEKST.setString(odbiorcaInput);
                        }
                        // Pole kwoty
                        if (kwotaActive) {
                            if (event.text.unicode == 8 && !std::to_string(kwotaInput).empty()) { // Backspace
                                // Usuwanie ostatniego znaku z tekstu
                                kwotaInput /= 10;
                            } else if ((event.text.unicode >= '0' && event.text.unicode <= '9') || event.text.unicode == '.') {
                                // Sprawdzanie czy wprowadzany znak to cyfra lub kropka
                                kwotaInput = kwotaInput * 10 + (event.text.unicode - '0'); // Dodawanie cyfry lub kropki do kwoty
                            }
                            // Aktualizacja tekstu wyświetlanego na ekranie
                            kwotaTEKST.setString(std::to_string(kwotaInput));
                        }
                    }

                }

        sf::Text myBalance("Stan konta: $" + std::to_string(balance), font, 24);
        myBalance.setFillColor(sf::Color::Black);
        if(currentState==MyAccount){
            myBalance.setPosition(50,100);
        }else if(currentState==Transfers || currentState==wrongEmail){
            myBalance.setPosition(300,125);
        }

        sf::Texture logoTexture;
        if (!logoTexture.loadFromFile("bank.jpg")) {
            // Obsługa błędu wczytywania obrazu
            std::cerr << "Błąd wczytywania obrazu bank.jpg" << std::endl;
        }

        sf::Sprite minilogo;
        minilogo.setTexture(logoTexture);
        minilogo.setScale(0.3, 0.3f);
        minilogo.setPosition(650, 16);

        sf::Sprite right_logo;
        right_logo.setTexture(logoTexture);
        right_logo.setScale(0.7, 0.7f);
        right_logo.setPosition(450,150);

        // Wyczyszczenie okna
        window.clear(sf::Color::White);
        if (currentState == State::Login) {
            window.draw(container);
            window.draw(button);
            window.draw(minilogo);
            window.draw(buttonText);
            window.draw(emailBox);
            window.draw(passwordBox);
            window.draw(emailText);
            window.draw(passwordText);
            window.draw(doRegistration);
            window.draw(doRegistrationText);
            window.draw(textRegistration);
            window.draw(bank);
        }else if(currentState == State::Rejected){
            window.draw(container);
            window.draw(button);
            window.draw(minilogo);
            window.draw(buttonText);
            window.draw(emailBox);
            window.draw(passwordBox);
            window.draw(emailText);
            window.draw(passwordText);
            window.draw(doRegistration);
            window.draw(doRegistrationText);
            window.draw(textRegistration);
            window.draw(bank);
            window.draw(RejectedText);
        } else if (currentState == State::MyAccount) {
            window.draw(bar);
            window.draw(myAcc);
            window.draw(myAccText);
            window.draw(myPayments);
            window.draw(myPaymentsText);
            window.draw(myHistory);
            window.draw(myHistoryText);
            window.draw(quit);
            window.draw(quitText);
            window.draw(right_logo);
            window.draw(left_box);
            window.draw(myBalance);

        } else if (currentState == State::Transfers) {
            window.draw(middle_boxShadow);
            window.draw(middle_box);
            window.draw(bar);
            window.draw(myAcc);
            window.draw(myAccText);
            window.draw(myPayments);
            window.draw(myPaymentsText);
            window.draw(myHistory);
            window.draw(myHistoryText);
            window.draw(quit);
            window.draw(quitText);
            window.draw(myBalance);
            window.draw(odbiorcaText);
            window.draw(odbiorcaBoxShadow);
            window.draw(odbiorcaBox);
            window.draw(kwotaTxt);
            window.draw(kwotaBoxShadow);
            window.draw(kwotaBox);
            window.draw(odbiorcaTEKST);
            window.draw(kwotaTEKST);
            window.draw(przelejButtonShadow);
            window.draw(przelejButton);
            window.draw(przelejButtonText);
            if(wrongEmail==true){
                window.draw(doesntExists);
            }
            if(transaction_accepted==true){
                window.draw(transAccepted);
            }
        } else if (currentState == State::History) {
            window.draw(bar);
            window.draw(myAcc);
            window.draw(myAccText);
            window.draw(myPayments);
            window.draw(myPaymentsText);
            window.draw(myHistory);
            window.draw(myHistoryText);
            window.draw(quit);
            window.draw(quitText);
        }
        
        window.display();
    }

    return 0;
}