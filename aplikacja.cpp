#include "headers.h"
#include "baza_connect.cpp"

enum State {
    Login,
    Rejected,
    Register,
    MyAccount,
    Transfers,
    History,
};

int main() {
    // Stworzenie okna
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bank Kar SA");

    window.setFramerateLimit(60);
    
    std::cout << "SFML Version: " << SFML_VERSION_MAJOR << "." << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;

    // Początkowy stan aplikacji
    State currentState = State::Login;

    int userid;
    int balance;
    int kwotaInput = 0;
    int total_transactions;
    int strona = 1;
    int strona_help = 0;

    // Zmienne wskazujące, które pole jest aktualnie aktywne
    bool emailActive = false;
    bool emailActiveAgain = false;
    bool passwordActive = false;
    bool passwordActiveAgain = false;
    bool kwotaActive = false;
    bool odbiorcaActive = false;
    bool isHoveredButton = false;
    bool wrongEmail = false;
    bool transaction_accepted = false;
    bool not_connected = false;
    bool koszty_clicked = false;
    bool przychody_clicked = false;
    bool koszty_or_przychody = true;
    bool saldo_cant_be_on_debit = false;
    bool show_exit = false;
    bool quit_yes_or_no = false;
    bool show_checkmark = false;
    bool password_not_equal = false;
    bool email_not_equal = false;
    bool registered = false;
    bool register_error = false;

    std::string* historia = nullptr;

    std::string przelewy;

    std::string latest_inc;

    sf::Color firstmain(0, 120, 52);
    sf::Color secondmain(241, 178, 86);
    sf::Color thirdmain(255,204,153);

    // Pobranie domyślnej czcionki systemowej
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        std::cerr << "Błąd ładowania czcionki." << std::endl;
        return 1;
    }

    //logowanie poczatek

        sf::Texture logoTexture;
        if (!logoTexture.loadFromFile("bank.jpg")) {
            // Obsługa błędu wczytywania obrazu
            std::cerr << "Błąd wczytywania obrazu bank.jpg" << std::endl;
        }

        sf::Sprite minilogo;
        minilogo.setTexture(logoTexture);
        minilogo.setScale(0.3, 0.3f);
        minilogo.setPosition(650, 16);

        // Tekst na przycisku
        sf::Text buttonText("Zaloguj sie!", font, 20);
        buttonText.setPosition(340, 315);
        buttonText.setFillColor(sf::Color::Black);

        // Pole tekstowe dla email
        sf::Text emailText("Email", font, 16);

        sf::RectangleShape emailBox(sf::Vector2f(250, 20));

        sf::Text passwordText("Haslo", font, 16);

        sf::RectangleShape passwordBox(sf::Vector2f(250, 20));

        if(currentState==Login || currentState==Rejected){
            emailText.setPosition(276, 180);
            emailText.setFillColor(sf::Color::Black);
            emailBox.setPosition(275, 180);
            emailBox.setFillColor(sf::Color::White);
            emailBox.setOutlineThickness(1);
            emailBox.setOutlineColor(sf::Color::Black);
            passwordText.setPosition(276, 240);
            passwordText.setFillColor(sf::Color::Black);
            passwordBox.setPosition(275, 240);
            passwordBox.setFillColor(sf::Color::White);
            passwordBox.setOutlineThickness(1);
            passwordBox.setOutlineColor(sf::Color::Black);
        }

        sf::RectangleShape button(sf::Vector2f(150, 50));
        button.setPosition(325, 300);
        button.setFillColor(sf::Color(250, 184, 86));

        sf::RectangleShape container(sf::Vector2f(400,500));
        container.setPosition(200,50);
        container.setFillColor(firstmain);

        sf::Text textRegistration("Nie masz konta? Zarejestruj sie ponizej", font,12);
        textRegistration.setPosition(285,380);
        textRegistration.setFillColor(sf::Color::Black);

        sf::Text bank("Bank Kar SA",font,30);
        bank.setPosition(315,100);
        bank.setFillColor(sf::Color::Black);

        sf::Text RejectedText("Bledne dane, nie zalogowano",font,12);
        RejectedText.setPosition(315,150);
        RejectedText.setFillColor(sf::Color::Black);

        sf::Text not_connected_text("Brak polaczenia z baza danych!",font,12);
        not_connected_text.setPosition(315,150);
        not_connected_text.setFillColor(sf::Color::Black);
    //logowanie koniec

    //register poczatek

        sf::RectangleShape doRegistration(sf::Vector2f(200,50));
        doRegistration.setPosition(300, 400);
        doRegistration.setFillColor(sf::Color::Red);

        sf::Text doRegistrationText("Zarejestruj sie!",font,20);
        doRegistrationText.setPosition(320,415);
        doRegistrationText.setFillColor(sf::Color::Black);

        sf::Text rejestracja_text("Formularz rejestracyjny",font ,24);
        rejestracja_text.setPosition(265,150);
        rejestracja_text.setFillColor(sf::Color::Black);

        sf::Text emailTextAgain("Powtorz Email", font, 16);
        emailTextAgain.setPosition(276, 230);
        emailTextAgain.setFillColor(sf::Color::Black);
        
        sf::RectangleShape emailBoxAgain(sf::Vector2f(250, 20));
        emailBoxAgain.setOutlineThickness(1);
        emailBoxAgain.setOutlineColor(sf::Color::Black);
        emailBoxAgain.setPosition(275, 230);
        emailBoxAgain.setFillColor(sf::Color::White);

        sf::Text passwordTextAgain("Powtorz haslo", font, 16);
        passwordTextAgain.setPosition(276, 290);
        passwordTextAgain.setFillColor(sf::Color::Black);

        sf::RectangleShape passwordBoxAgain(sf::Vector2f(250, 20));
        passwordBoxAgain.setOutlineThickness(1);
        passwordBoxAgain.setOutlineColor(sf::Color::Black);
        passwordBoxAgain.setPosition(275, 290);
        passwordBoxAgain.setFillColor(sf::Color::White);

        sf::RectangleShape registerButton(sf::Vector2f(200,50));
        registerButton.setPosition(300,400);
        registerButton.setFillColor(secondmain);
        registerButton.setOutlineThickness(1);
        registerButton.setOutlineColor(sf::Color::Black);

        sf::Text registerButton_text("Stworz konto",font,24);
        registerButton_text.setPosition(320,410);
        registerButton_text.setFillColor(sf::Color::Black);

        sf::Text text_to_checkmark("Potwierdzam, ze przeczytalem regulamin",font,12);
        text_to_checkmark.setPosition(260,350);
        text_to_checkmark.setFillColor(sf::Color::Black);

        sf::RectangleShape checkbox(sf::Vector2f(20, 20));
        checkbox.setPosition(520, 347);
        checkbox.setFillColor(sf::Color::White);
        checkbox.setOutlineThickness(1);
        checkbox.setOutlineColor(sf::Color::Black);

        sf::Text checkmark("x", font, 20);
        checkmark.setPosition(524, 343);
        checkmark.setFillColor(sf::Color::Black);

        sf::Text password_not_equal_text("Pola hasel nie sa rowne!",font,20);
        password_not_equal_text.setPosition(280,320);
        password_not_equal_text.setFillColor(secondmain);

        sf::Text email_not_equal_text("Pola email nie sa rowne!",font,20);
        email_not_equal_text.setPosition(280,320);
        email_not_equal_text.setFillColor(secondmain);

        sf::RectangleShape registered_arrow_back(sf::Vector2f(100,50));
        registered_arrow_back.setPosition(0,550);
        registered_arrow_back.setFillColor(secondmain);
        registered_arrow_back.setOutlineThickness(1);
        registered_arrow_back.setOutlineColor(sf::Color::Black);

        sf::Text registered_arrow_back_text("Wroc",font,24);
        registered_arrow_back_text.setPosition(19,561);
        registered_arrow_back_text.setFillColor(sf::Color::Black);

        sf::Text registered_succes("Zarejestrowano pomyslnie",font,24);
        registered_succes.setPosition(250,320);
        registered_succes.setFillColor(sf::Color::Green);

        sf::Text register_error_text("Nie udalo sie zarejestrowac",font,20);
        register_error_text.setPosition(260,320);
        register_error_text.setFillColor(sf::Color::Red);

    //register koniec

    //bar poczatek

        sf::RectangleShape bar(sf::Vector2f(800, 80));
        bar.setPosition(0, 0);
        bar.setFillColor(firstmain);

        sf::RectangleShape myAcc(sf::Vector2f(180,50));
        myAcc.setPosition(16,15);
        myAcc.setFillColor(secondmain);
        myAcc.setOutlineThickness(1);
        myAcc.setOutlineColor(sf::Color::Black);

        sf::RectangleShape myPayments(sf::Vector2f(180,50));
        myPayments.setPosition(212,15);
        myPayments.setFillColor(secondmain);
        myPayments.setOutlineThickness(1);
        myPayments.setOutlineColor(sf::Color::Black);

        sf::RectangleShape myHistory(sf::Vector2f(180,50));
        myHistory.setPosition(408,15);
        myHistory.setFillColor(secondmain);
        myHistory.setOutlineThickness(1);
        myHistory.setOutlineColor(sf::Color::Black);

        sf::RectangleShape quit(sf::Vector2f(180,50));
        quit.setPosition(604,15);
        quit.setFillColor(secondmain);
        quit.setOutlineThickness(1);
        quit.setOutlineColor(sf::Color::Black);

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

        sf::RectangleShape exit_box(sf::Vector2f(300,200));
        exit_box.setPosition(250,200);
        exit_box.setFillColor(firstmain);

        sf::RectangleShape exit_box_shadow(sf::Vector2f(304,204));
        exit_box_shadow.setPosition(248,198);
        exit_box_shadow.setFillColor(sf::Color::Black);

        sf::Text exit_box_text("Czy na pewno chcesz wyjsc?", font ,20);
        exit_box_text.setPosition(260,220);
        exit_box_text.setFillColor(sf::Color::White);

        sf::RectangleShape exit_left_box(sf::Vector2f(120,70));
        exit_left_box.setPosition(270,290);
        exit_left_box.setFillColor(secondmain);
        exit_left_box.setOutlineThickness(1);
        exit_left_box.setOutlineColor(sf::Color::Black);

        sf::Text exit_left_box_text("Tak",font,24);
        exit_left_box_text.setPosition(310,310);
        exit_left_box_text.setFillColor(sf::Color::Black);

        sf::RectangleShape exit_right_box = exit_left_box;
        exit_right_box.setPosition(410,290);
        exit_right_box.setFillColor(secondmain);
        exit_right_box.setOutlineThickness(1);
        exit_right_box.setOutlineColor(sf::Color::Black);

        sf::Text exit_right_box_text("Nie",font,24);
        exit_right_box_text.setPosition(450,310);
        exit_right_box_text.setFillColor(sf::Color::Black);

    //bar koniec

    //myacc poczatek

        sf::RectangleShape left_box(sf::Vector2f(400,520));
        left_box.setPosition(0,80);
        left_box.setFillColor(thirdmain);

        sf::Sprite right_logo;
        right_logo.setTexture(logoTexture);
        right_logo.setScale(0.7, 0.7f);
        right_logo.setPosition(450,150);


        //najnowsza transakcja

        //najnoszy przychod

    //transakcje poczatek

        sf::RectangleShape middle_box(sf::Vector2f(600,425));
        middle_box.setPosition(100,80);
        middle_box.setFillColor(sf::Color(thirdmain));
        middle_box.setOutlineThickness(1);
        middle_box.setOutlineColor(sf::Color::Black);

        sf::RectangleShape middle_boxShadow(sf::Vector2f(600,427));
        middle_boxShadow.setPosition(102,80);
        middle_boxShadow.setFillColor(sf::Color::Black);

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
        przelejButton.setFillColor(sf::Color(secondmain));//255,102,102
        przelejButton.setOutlineThickness(1);
        przelejButton.setOutlineColor(sf::Color::Black);

        sf::Text przelejButtonText("Przelej", font, 24);
        przelejButtonText.setPosition(360,413);
        przelejButtonText.setFillColor(sf::Color::Black);

        sf::RectangleShape przelejButtonShadow = przelejButton;
        przelejButtonShadow.setPosition(300+2,400+2);
        przelejButtonShadow.setFillColor(sf::Color(30, 30, 30));

        sf::Text doesntExists("Nie istnieje uzytkownik o takim adresie!",font, 16);
        doesntExists.setPosition(250,200);
        doesntExists.setFillColor(sf::Color::Red);

        sf::Text transAccepted("Transakcja przebiegla pomyslnie!",font, 16);
        transAccepted.setPosition(240,200);
        transAccepted.setFillColor(firstmain);

        sf::Text saldo_cant_be_on_debit_text("Brak srodkow do sfinalizowania operacji!",font,16);
        saldo_cant_be_on_debit_text.setPosition(240,200);
        saldo_cant_be_on_debit_text.setFillColor(sf::Color::Red);

    //transakcje koniec

    //historia poczatek

        sf::RectangleShape history_background(sf::Vector2f(800,520));
        history_background.setPosition(0,80);
        //history_background.setFillColor(sf::Color(228,214,214));
        history_background.setFillColor(sf::Color::White);

        sf::RectangleShape history_window(sf::Vector2f(700,400));
        history_window.setPosition(50,145);
        history_window.setFillColor(sf::Color(thirdmain));
        history_window.setOutlineThickness(1);
        history_window.setOutlineColor(sf::Color::Black);

        sf::RectangleShape history_window_shader(sf::Vector2f(700,400));
        history_window_shader.setPosition(54,148);
        history_window_shader.setFillColor(sf::Color::Black);

        sf::RectangleShape koszty_click(sf::Vector2f(400,50));
        koszty_click.setPosition(0,80);
        koszty_click.setFillColor(sf::Color(255,102,102));
        koszty_click.setOutlineThickness(1);
        koszty_click.setOutlineColor(sf::Color::Black);

        sf::RectangleShape przychody_click = koszty_click;
        przychody_click.setPosition(400,80);
        przychody_click.setFillColor(sf::Color(102,255,102));
        przychody_click.setOutlineThickness(1);
        przychody_click.setOutlineColor(sf::Color::Black);

        sf::Text przychody("Przelewy przychodzace",font,24);
        przychody.setPosition(470,90);
        przychody.setFillColor(sf::Color::Black);
        
        sf::Text koszty("Przelewy wychodzace",font,24);
        koszty.setPosition(70,90);
        koszty.setFillColor(sf::Color::Black);

        sf::RectangleShape right_arrow(sf::Vector2f(70,30));
        right_arrow.setPosition(700,560);
        right_arrow.setFillColor(secondmain);
        right_arrow.setOutlineThickness(2);
        right_arrow.setOutlineColor(sf::Color::Black);

        sf::Text right_arrow_text(">>",font,20);
        right_arrow_text.setPosition(720,562);
        right_arrow_text.setFillColor(sf::Color::Black);

        sf::RectangleShape left_arrow = right_arrow;
        left_arrow.setPosition(30,560);
        left_arrow.setFillColor(secondmain);
        left_arrow.setOutlineThickness(2);
        left_arrow.setOutlineColor(sf::Color::Black);

        sf::Text left_arrow_text("<<",font,20);
        left_arrow_text.setPosition(50,562);
        left_arrow_text.setFillColor(sf::Color::Black);

    //historia koniec

    // Zmienne przechowujące wprowadzone dane
    std::string emailInput = "";
    std::string emailInputAgain= "";
    std::string passwordInput = "";
    std::string passwordInputAgain = "";
    std::string odbiorcaInput = "";

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

                if(!show_exit){
                    if (myAcc.getGlobalBounds().contains(mousePosF)) {
                        myAcc.setFillColor(sf::Color(39, 54, 85));
                        myHistory.setFillColor(secondmain);
                        myPayments.setFillColor(secondmain);
                        quit.setFillColor(secondmain);
                    } else if(myHistory.getGlobalBounds().contains(mousePosF)){
                        myHistory.setFillColor(sf::Color(39, 54, 85));
                        myAcc.setFillColor(secondmain);
                        myPayments.setFillColor(secondmain);
                        quit.setFillColor(secondmain);
                    } else if(myPayments.getGlobalBounds().contains(mousePosF)){
                        myPayments.setFillColor(sf::Color(39, 54, 85));
                        myAcc.setFillColor(secondmain);
                        myHistory.setFillColor(secondmain);
                        quit.setFillColor(secondmain);
                    } else if(quit.getGlobalBounds().contains(mousePosF)){
                        quit.setFillColor(sf::Color::Red);
                        myAcc.setFillColor(secondmain);
                        myHistory.setFillColor(secondmain);
                        myPayments.setFillColor(secondmain);
                    }
                    else {
                        myAcc.setFillColor(secondmain);
                        myHistory.setFillColor(secondmain);
                        myPayments.setFillColor(secondmain);
                        quit.setFillColor(secondmain);
                    }
                }

                if(currentState==Transfers){
                    if(przelejButton.getGlobalBounds().contains(mousePosF)){
                        przelejButton.setFillColor(sf::Color(39, 54, 85));
                    }
                    else{
                        przelejButton.setFillColor(sf::Color(secondmain));
                    }
                }

                if(currentState==History){
                    if(koszty_click.getGlobalBounds().contains(mousePosF)){
                        koszty_click.setFillColor(sf::Color(39, 54, 85));
                        koszty.setFillColor(sf::Color::Red);
                        przychody_click.setFillColor(sf::Color(102,255,102));
                        przychody.setFillColor(sf::Color::Black);
                        right_arrow.setFillColor(secondmain);
                        left_arrow.setFillColor(secondmain);
                    } else if(przychody_click.getGlobalBounds().contains(mousePosF)){
                        przychody_click.setFillColor(sf::Color(39,54,85));
                        przychody.setFillColor(sf::Color::Green);
                        koszty_click.setFillColor(sf::Color(255,102,102));
                        koszty.setFillColor(sf::Color::Black);
                        right_arrow.setFillColor(secondmain);
                        left_arrow.setFillColor(secondmain);
                    }else if(left_arrow.getGlobalBounds().contains(mousePosF)){
                        left_arrow.setFillColor(sf::Color(39, 54, 85));
                        right_arrow.setFillColor(secondmain);
                        koszty_click.setFillColor(sf::Color(255,102,102));
                        przychody_click.setFillColor(sf::Color(102,255,102));
                        koszty.setFillColor(sf::Color::Black);
                        przychody.setFillColor(sf::Color::Black);
                    }else if(right_arrow.getGlobalBounds().contains(mousePosF)){
                        right_arrow.setFillColor(sf::Color(39, 54, 85));
                        left_arrow.setFillColor(secondmain);
                        koszty_click.setFillColor(sf::Color(255,102,102));
                        przychody_click.setFillColor(sf::Color(102,255,102));
                        koszty.setFillColor(sf::Color::Black);
                        przychody.setFillColor(sf::Color::Black);
                    }
                    else{
                        koszty_click.setFillColor(sf::Color(255,102,102));
                        przychody_click.setFillColor(sf::Color(102,255,102));
                        koszty.setFillColor(sf::Color::Black);
                        przychody.setFillColor(sf::Color::Black);
                        right_arrow.setFillColor(secondmain);
                        left_arrow.setFillColor(secondmain);
                    }
                }

                if(show_exit){
                    if(exit_left_box.getGlobalBounds().contains(mousePosF)){
                        exit_left_box.setFillColor(sf::Color::Red);
                        exit_right_box.setFillColor(secondmain);
                    }else if(exit_right_box.getGlobalBounds().contains(mousePosF)){
                        exit_right_box.setFillColor(sf::Color(39, 54, 85));
                        exit_left_box.setFillColor(secondmain);
                    }
                    else{
                        exit_left_box.setFillColor(secondmain);
                        exit_right_box.setFillColor(secondmain);
                    }
                }

                }

                    // Obsługa kliknięcia myszą
                    if (event.type == sf::Event::MouseButtonPressed) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

                        if(currentState==Login || currentState==Rejected){
                            if (emailBox.getGlobalBounds().contains(mousePosF)) {
                                emailActive = true;
                                passwordActive = false;
                            } else {
                                emailActive = false;
                            }

                            if (passwordBox.getGlobalBounds().contains(mousePosF)) {
                                passwordActive = true;
                                emailActive = false;
                            } else {
                                passwordActive = false;
                            }
                            if (doRegistration.getGlobalBounds().contains(mousePosF)){
                                emailInput = "";
                                passwordInput = "";
                                emailText.setString("Email");
                                passwordText.setString("Haslo");
                                currentState = Register;
                            }
                        }

                        if(currentState==Register){
                            if (emailBox.getGlobalBounds().contains(mousePosF)) {
                                emailActive = true;
                                emailActiveAgain = false;
                                passwordActive = false;
                                passwordActiveAgain = false;
                            } else {
                                emailActive = false;
                            }

                            if (emailBoxAgain.getGlobalBounds().contains(mousePosF)) {
                                emailActiveAgain = true;
                                emailActive = false;
                                passwordActive = false;
                                passwordActiveAgain = false;
                            } else {
                                emailActiveAgain = false;
                            }

                            if (passwordBox.getGlobalBounds().contains(mousePosF)) {
                                passwordActive = true;
                                emailActive = false;
                                emailActiveAgain = false;
                                passwordActiveAgain = false;
                            } else {
                                passwordActive = false;
                            }

                            if (passwordBoxAgain.getGlobalBounds().contains(mousePosF)) {
                                passwordActiveAgain = true;
                                passwordActive = false;
                                emailActive = false;
                                emailActiveAgain = false;
                            } else {
                                passwordActiveAgain = false;
                            }
                            
                            if (checkbox.getGlobalBounds().contains(mousePosF)){
                                if(!show_checkmark){
                                    show_checkmark = true;
                                }
                                else{
                                    show_checkmark = false;
                                }
                            }

                            if(passwordInput!=passwordInputAgain){
                                password_not_equal = true;
                            }else{
                                password_not_equal = false;
                            }

                            if(emailInput!=emailInputAgain){
                                email_not_equal = true;
                            }else{
                                email_not_equal = false;
                            }

                            if((registerButton.getGlobalBounds().contains(mousePosF)) && (password_not_equal == false) && (email_not_equal == false) && (show_checkmark==true) && (!emailInput.empty()) && (!passwordInput.empty())){
                                if(sign_to_database(emailInput,passwordInput)){
                                    registered = true;
                                    register_error = false;
                                }
                                else{
                                    register_error = true;
                                }
                            }
                            if(registered_arrow_back.getGlobalBounds().contains(mousePosF)){
                                    register_error = false;
                                    emailInput = "";
                                    passwordInput = "";
                                    emailText.setString("Email");
                                    passwordText.setString("Haslo");
                                    registered = false;
                                    show_checkmark = false;
                                    currentState = Login;
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

                        if (currentState == History) {
                            if (koszty_click.getGlobalBounds().contains(mousePosF)) {
                                strona = 1;
                                strona_help = 0;
                                przelewy.clear();
                                koszty_or_przychody = true;
                                get_history(emailInput, userid, total_transactions, historia, koszty_or_przychody);
                                for (int i = total_transactions - 10; i < total_transactions; i++) {
                                    if (i >= 0 && i < total_transactions) {
                                        przelewy += "$" + historia[i] + "\n\n";
                                    }
                                }
                                koszty_clicked = true;
                                przychody_clicked = false;
                            }
                            if (przychody_click.getGlobalBounds().contains(mousePosF)) {
                                strona = 1;
                                strona_help = 0;
                                przelewy.clear();
                                koszty_or_przychody = false;
                                get_history(emailInput, userid, total_transactions, historia, koszty_or_przychody);
                                for (int i = total_transactions - 10; i < total_transactions; i++) {
                                    if (i >= 0 && i < total_transactions) {
                                        przelewy += "$" + historia[i] + "\n\n";
                                    }
                                }
                                przychody_clicked = true;
                                koszty_clicked = false;
                            }
                            if (strona < total_transactions / 10+1) {
                                if (right_arrow.getGlobalBounds().contains(mousePosF)) {
                                    strona++;
                                    strona_help += 10;
                                    przelewy.clear();
                                    int max_przelewy = 10;
                                    int start_index = std::max(0, total_transactions - strona_help - max_przelewy); // Indeks początkowy
                                    int end_index = std::max(0, total_transactions - strona_help); // Indeks końcowy

                                    for (int i = start_index; i < end_index; i++) {
                                        if (i >= 0 && i < total_transactions) {
                                            przelewy += historia[i] + "\n\n";
                                        }
                                    }
                                }
                            }
                            if (strona > 1) {
                                if (left_arrow.getGlobalBounds().contains(mousePosF)) {
                                    strona--;
                                    strona_help -= 10;
                                    przelewy.clear();
                                    int max_przelewy = 10;
                                    int start_index = std::max(0, total_transactions - strona_help - max_przelewy); // Indeks początkowy
                                    int end_index = std::max(0, total_transactions - strona_help); // Indeks końcowy

                                    for (int i = start_index; i < end_index; i++) {
                                        if (i >= 0 && i < total_transactions) {
                                            przelewy += historia[i] + "\n\n";
                                        }
                                    }
                                }
                            }
                        } else {
                            przychody_clicked = false;
                            koszty_clicked = false;
                        }
                        // Sprawdź, czy naciśnięto
                        if((currentState==Login || currentState==Rejected)){
                            if (button.getGlobalBounds().contains(mousePosF)) {
                            if (checkLogin(emailInput, passwordInput, userid, not_connected)) {
                                currentState = State::MyAccount; 
                                std::cout<< "Nr id to: " << userid << std:: endl;
                                getBalance(userid, balance);
                                koszty_or_przychody = false;
                                get_history(emailInput, userid, total_transactions, historia, koszty_or_przychody);
                                if(total_transactions!=0){
                                    latest_inc = historia[total_transactions-1];
                                }
                            } else {
                                currentState = State::Rejected;
                            }
                        }
                        }

                        if(!(currentState == Login || currentState == Rejected || currentState == Register || show_exit == true)){
                            if(myAcc.getGlobalBounds().contains(mousePosF)){
                                currentState = MyAccount;
                            }

                            if(myPayments.getGlobalBounds().contains(mousePosF)){
                                currentState = Transfers;
                            }

                            if(myHistory.getGlobalBounds().contains(mousePosF)){
                                currentState = History;
                            }
                            
                            // Sprawdź czy quit jest nacisniety
                            if (quit.getGlobalBounds().contains(mousePosF)){
                                show_exit = true;
                            }
                        }
                        if(!(currentState == Login || currentState == Rejected) && show_exit==true){
                            if(exit_left_box.getGlobalBounds().contains(mousePosF)){
                            window.close();
                        }
                        else if(exit_right_box.getGlobalBounds().contains(mousePosF)){
                            exit_right_box.setFillColor(secondmain);
                            show_exit = false;
                        }
                        }
                        
                        if(przelejButton.getGlobalBounds().contains(mousePosF)){
                            if(!odbiorcaInput.empty()){
                                if(!kwotaInput==0){
                                    if(balance - kwotaInput >0){
                                        balance = balance - kwotaInput;
                                        if(connect_for_transaction(userid,emailInput,odbiorcaInput,balance, kwotaInput)==false){
                                            wrongEmail = true;
                                            saldo_cant_be_on_debit = false;
                                            transaction_accepted = false;
                                            balance = balance + kwotaInput;
                                        }else{
                                            saldo_cant_be_on_debit = false;
                                            wrongEmail = false;
                                            transaction_accepted = true;
                                        }
                                    }else{
                                        saldo_cant_be_on_debit = true;
                                        wrongEmail = false;
                                        transaction_accepted = false;
                                    }
                                }
                            }
                            else{
                                //window.draw(emptyEmail);
                            }
                        }

                    }
                    // Obsługa wprowadzania tekstu
                    if (event.type == sf::Event::TextEntered) {
                        // Email
                        if (emailActive) {
                            if (event.text.unicode == 8 && !emailInput.empty()) { // Backspace
                                emailInput.pop_back();
                            } else if (event.text.unicode < 128 && emailInput.size() < 50 && event.text.unicode != 8) {
                                emailInput += static_cast<char>(event.text.unicode);
                            }
                            emailText.setString(emailInput);
                            if(emailInput.empty()){
                                emailText.setString("Email");
                            }
                        }
                        // Ponownie Email
                        if (emailActiveAgain) {
                            if (event.text.unicode == 8 && !emailInputAgain.empty()) { // Backspace
                                emailInputAgain.pop_back();
                            } else if (event.text.unicode < 128 && emailInputAgain.size() < 50 && event.text.unicode != 8) {
                                emailInputAgain += static_cast<char>(event.text.unicode);
                            }
                            emailTextAgain.setString(emailInputAgain);
                            if(emailInputAgain.empty()){
                                emailTextAgain.setString("Powtorz email");
                            }
                        }
                        // Haslo
                        if (passwordActive) {
                            if (event.text.unicode == 8 && !passwordInput.empty()) { // Backspace
                                passwordInput.pop_back();
                            } else if (event.text.unicode < 128 && passwordInput.size() < 50 && event.text.unicode != 8) {
                                passwordInput += static_cast<char>(event.text.unicode);
                            }
                            passwordText.setString(std::string(passwordInput.size(), '*')); // Zamień hasło na gwiazdki
                            if(passwordInput.empty()){
                                passwordText.setString("Haslo");
                            }
                        }
                        // Ponownie Haslo
                        if (passwordActiveAgain) {
                            if (event.text.unicode == 8 && !passwordInputAgain.empty()) { // Backspace
                                passwordInputAgain.pop_back();
                            } else if (event.text.unicode < 128 && passwordInputAgain.size() < 50 && event.text.unicode != 8) {
                                passwordInputAgain += static_cast<char>(event.text.unicode);
                            }
                            passwordTextAgain.setString(std::string(passwordInputAgain.size(), '*')); // Zamień hasło na gwiazdki
                            if(passwordInputAgain.empty()){
                                passwordTextAgain.setString("Powtorz haslo");
                            }
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
        // register
        if(currentState==Register){
            emailText.setPosition(276, 200);
            emailText.setFillColor(sf::Color::Black);

            emailBox.setPosition(275, 200);
            emailBox.setFillColor(sf::Color::White);
            emailBox.setOutlineThickness(1);
            emailBox.setOutlineColor(sf::Color::Black);

            passwordText.setPosition(276, 260);
            passwordText.setFillColor(sf::Color::Black);

            passwordBox.setPosition(275, 260);
            passwordBox.setFillColor(sf::Color::White);
            passwordBox.setOutlineThickness(1);
            passwordBox.setOutlineColor(sf::Color::Black);
        }

        // history
        sf::RectangleShape between_arrow(sf::Vector2f(160,30));
        between_arrow.setPosition(320,560);
        between_arrow.setFillColor(secondmain);
        if(koszty_or_przychody==true){
            between_arrow.setFillColor(sf::Color(255,102,102));
        }else{
            between_arrow.setFillColor(sf::Color(102,255,102));
        }
        between_arrow.setOutlineColor(sf::Color::Black);
        between_arrow.setOutlineThickness(2);

        sf::Text between_arrow_text("Strona " + std::to_string(strona),font,24);
        between_arrow_text.setPosition(355,562);
        between_arrow_text.setFillColor(sf::Color::Black);

        // myAcc
        sf::Text myBalance("Stan konta: $" + std::to_string(balance), font, 24);
        myBalance.setFillColor(sf::Color::Black);
        if(currentState==MyAccount){
            myBalance.setPosition(20,100);
        }else if(currentState==Transfers){
            myBalance.setPosition(300,125);
        }

        sf::Text latest_income_text("Ostatni przychod:",font,24);
        latest_income_text.setPosition(20,540);
        latest_income_text.setFillColor(sf::Color::Black);

        sf::Text latest_income(latest_inc,font,16);
        latest_income.setPosition(20,570);
        latest_income.setFillColor(firstmain);

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

        //historia
        sf::Text kosztyTXT(przelewy,font ,16);
        kosztyTXT.setPosition(90,162);
        kosztyTXT.setFillColor(sf::Color::Black);

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
        } else if (currentState == State::Rejected){
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
            if(not_connected){
                window.draw(not_connected_text);
            }
            else{
                window.draw(RejectedText);
            }
        } else if (currentState == State::Register){
            //dodac warunek do bar
            window.draw(container);
            window.draw(minilogo);
            window.draw(bank);
            window.draw(rejestracja_text);
            window.draw(emailBox);
            window.draw(passwordBox);
            window.draw(emailText);
            window.draw(passwordText);
            window.draw(emailBoxAgain);
            window.draw(passwordBoxAgain);
            window.draw(emailTextAgain);
            window.draw(passwordTextAgain);
            window.draw(registerButton);
            window.draw(registerButton_text);
            window.draw(text_to_checkmark);
            window.draw(checkbox);
            if(show_checkmark){
                window.draw(checkmark);
            }
            if(email_not_equal){
                window.draw(email_not_equal_text);
            }else{
                if(password_not_equal){
                    window.draw(password_not_equal_text);
                }
            }
            if(registered){
                window.draw(registered_succes);
            }   
            window.draw(registered_arrow_back);
            window.draw(registered_arrow_back_text);
            if(register_error){
                window.draw(register_error_text);
            }
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
            window.draw(latest_income_text);
            window.draw(latest_income);      
            if(show_exit){
                window.draw(exit_box_shadow);
                window.draw(exit_box);
                window.draw(exit_box_text);
                window.draw(exit_left_box);
                window.draw(exit_right_box);
                window.draw(exit_right_box_text);
                window.draw(exit_left_box_text);
            }
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
            if(wrongEmail){
                window.draw(doesntExists);
            }
            if(transaction_accepted){
                window.draw(transAccepted);
            }
            if(saldo_cant_be_on_debit){
                window.draw(saldo_cant_be_on_debit_text);
            }
            if(show_exit){
                window.draw(exit_box_shadow);
                window.draw(exit_box);
                window.draw(exit_box_text);
                window.draw(exit_left_box);
                window.draw(exit_right_box);
                window.draw(exit_right_box_text);
                window.draw(exit_left_box_text);
            }
        } else if (currentState == State::History) {
            window.draw(history_background);
            window.draw(koszty_click);
            window.draw(przychody_click);
            window.draw(bar);
            window.draw(myAcc);
            window.draw(myAccText);
            window.draw(myPayments);
            window.draw(myPaymentsText);
            window.draw(myHistory);
            window.draw(myHistoryText);
            window.draw(quit);
            window.draw(quitText);
            window.draw(przychody);
            window.draw(koszty);
            if(koszty_clicked){
                window.draw(history_window_shader);
                window.draw(history_window);
                window.draw(kosztyTXT);
                if(strona < total_transactions / 10+1){
                    window.draw(right_arrow);
                    window.draw(right_arrow_text);
                }
                window.draw(between_arrow);
                window.draw(between_arrow_text);
                if(strona>1){
                    window.draw(left_arrow);
                    window.draw(left_arrow_text);
                }
            }else if(przychody_clicked){
                window.draw(history_window_shader);
                window.draw(history_window);
                window.draw(kosztyTXT);
                if(strona < total_transactions / 10+1){
                    window.draw(right_arrow);
                    window.draw(right_arrow_text);
                }
                window.draw(between_arrow);
                window.draw(between_arrow_text);
                if(strona>1){
                    window.draw(left_arrow);
                    window.draw(left_arrow_text);
                }
            }
            if(show_exit){
                window.draw(exit_box_shadow);
                window.draw(exit_box);
                window.draw(exit_box_text);
                window.draw(exit_left_box);
                window.draw(exit_right_box);
                window.draw(exit_right_box_text);
                window.draw(exit_left_box_text);
            }
        }
        window.display();
    }

    delete[] historia;

    return 0;
}