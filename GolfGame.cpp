#include <iostream>
#include <map>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <random>
#include<windows.h>

using namespace std;

const int max_players = 8;
const int max_rounds = 9;
bool skip_bots = false;
bool bot_sim = true;
int cur_turn = 0;
int num_games = 1;
int game = 0;
map<char, int> scores;

const string std_deck[] = {
                     "AS", "AH", "AD", "AC", 
                     "KS", "KH", "KD", "KC", 
                     "QS", "QH", "QD", "QC", 
                     "JS", "JH", "JD", "JC",  
                     "2S", "2H", "2D", "2C", 
                     "3S", "3H", "3D", "3C", 
                     "4S", "4H", "4D", "4C", 
                     "5S", "5H", "5D", "5C",
                     "6S", "6H", "6D", "6C",
                     "7S", "7H", "7D", "7C",
                     "8S", "8H", "8D", "8C",
                     "9S", "9H", "9D", "9C",
                     "1S", "1H", "1D", "1C", 
                     "jo", "jo"};

vector <string> cur_deck;
vector <string> discard_pile;

string card_temp;

struct player{
    int score;
    bool isturn;
    bool bot = false;
    string hand[6];
    int cards_revealed[6];

    player(int _score, bool _isturn){
        score = _score;
        isturn = _isturn;
    }
};

vector<player> players;

void dealhands(int num_players){
    for (int i = 0; i < 6; i++){
        for (int p = 0; p < num_players; p++){
            string card_temp;
            card_temp =  cur_deck[0];
            cur_deck.erase(cur_deck.begin());
            //cout << card_temp;
            players[p].hand[i] = card_temp;
            players[p].cards_revealed[i] = 0;
        }
    }

    /* ### Debug Staring Hand ###

    for (player p : players){
        cout << "\n";
        for (string card : p.hand){
            cout << card << " ";
        }
    }

    ###### Debug Staring Hand #### */
    

}

void draw(){
    card_temp = cur_deck[0];
    cur_deck.erase(cur_deck.begin());
}

void draw_discard(){
    card_temp = discard_pile[0];
    if (sizeof(discard_pile)/sizeof(string) == 1){
        discard_pile.clear();
    }
    else if (sizeof(discard_pile)/sizeof(string) > 1){
        discard_pile.erase(discard_pile.begin());
    }
}

void replace(int p, int card_replaced){
    string temp = players[p].hand[card_replaced];
    players[p].hand[card_replaced] = card_temp;
    card_temp = temp;

    if (players[p].cards_revealed[card_replaced] == 0){
        players[p].cards_revealed[card_replaced] = 1;
    }

    discard_pile.insert(discard_pile.begin(), temp);
    card_temp;

    //cout << "\n" << "original card: " << temp << " new card: " << players[p].hand[card_replaced] << " revealed?: " << players[p].cards_revealed[card_replaced]; 
}

void flip(int p, int card_flipped){
    players[p].cards_revealed[card_flipped] = 1;
}

int score(player p){
    //cout << "\n";
    int _score;
    char token;
    char token_top;
    char token_bottom;

    //ADD UP ALL REVEALED SCORES
    for (int c = 0; c < 6; c++){
        if (p.cards_revealed[c] != 0){
            token = p.hand[c][0];
            if (int(token) >= 48 && int(token) <= 57){
                _score += (token - '0');
            }
            else{
                _score += scores[token];
            }
        }
    }

    //CHECK COLUMS
    for (int c = 0; c < 3; c++){
        token_top = p.hand[c][0];
        token_bottom = p.hand[c+3][0];
        //CHECK JOKERS
        if ((token_bottom == 'j' && p.cards_revealed[c+3] != 0) || (token_top == 'j' && p.cards_revealed[c] != 0)){
            if (int(token_bottom) >= 48 && int(token_bottom) <= 57){
                    _score -= (token_top - '0');
            }
            else{
                    _score -= scores[token_top];
            }
            if (int(token_bottom) >= 48 && int(token_bottom) <= 57){
                    _score -= (token_bottom - '0');
            }
            else{
                    _score -= scores[token_bottom];
            }
        }
        //CHECK MATCHES
        else if (p.cards_revealed[c] != 0 && p.cards_revealed[c+3] != 0){
            if (token_top == token_bottom){
                if (int(token_bottom) >= 48 && int(token_bottom) <= 57){
                    _score -= (token_bottom - '0')*2;
                }
                else{
                    _score -= scores[token_bottom]*2;
                }
            }
        }
    }

    return _score;
}

void display_board(int _players_num, int _round, int _cur_turn){
    //clear text
    cout << "\x1B[2J\x1B[H" << "\n";

    //### TITLE ####
    for (int p = 0; p < _players_num/2; p++){
        cout << "##########";
    }
    cout << "  " << "ROUND " << _round << "  ";
    for (int p = _players_num/2; p < _players_num; p++){
        cout << "######";
    }
    cout << "\n";
    for (int p = 0; p < _players_num/2; p++){
        cout << "###########";
    }
    cout << "  GOLF  ";
    for (int p = _players_num/2; p < _players_num; p++){
        cout << "#######";
    }
    cout << "\n\n";
    //### END OF TITLE ####

    //### DISPLAY HANDS ###
    for (int p = 0; p < _players_num; p++){
        cout << "player " << p+1 << " (" << score(players[p])  << ")";
        if (p != _players_num - 1){
            cout << " | ";
        }
    }

    cout << "\n";

    for (int p = 0; p < _players_num; p++){
        cout << "__________";
    }
    cout << "_";

    cout << "\n";

    cout << "\n" << " ";

    for (int p = 0; p < _players_num; p++){

        cout << " ";

        for (int c = 0; c < 3; c++){
            if (players[p].cards_revealed[c] == 0){
                cout << "XX" << " ";
            }
            else{
                cout << players[p].hand[c] << " ";
            }
        }
        if (p != _players_num - 1){
            cout << " |  ";
        }
    }

    cout << "\n" << " ";

    for (int p = 0; p < _players_num; p++){

        cout << " ";

        for (int c = 3; c < 6; c++){
            if (players[p].cards_revealed[c] == 0){
                cout << "XX" << " ";
            }
            else{
                cout << players[p].hand[c] << " ";
            }
        }
        if (p != _players_num - 1){
            cout << " |  ";
        }
    }
    //### END OF DISPLAY HANDS ###

    //### DISPLAY PILES ###
    cout << "\n\n" ;
    cout << "Number of Cards in Deck: " << cur_deck.size();
    cout << "\n" << "Top Discard Pile: " << discard_pile[0]; 
    if (discard_pile.size() > 1){
        cout << " | ";
        for (int c = 1; c < discard_pile.size(); c++){
            cout << discard_pile[c] << " ";
        }
    }
    cout << "\n\n";
    for (int p = 0; p < _players_num/2; p++){
        cout << "###########";
    }
    cout << "  GOLF  ";
    for (int p = _players_num/2; p < _players_num; p++){
        cout << "#######";
    }
    cout << "\n";
    for (int p = 0; p < _players_num/2; p++){
        cout << "##########";
    }
    cout << "  " << "PLAYER " << _cur_turn + 1 << "  ";
    for (int p = _players_num/2; p < _players_num; p++){
        cout << "######";
    }
    //### END OF DISPLAY PILES ###
}

void display_score(int _num_players){
    cout << "\x1B[2J\x1B[H" << "\n";
    cout << "##### TOTAL SCORES #####" << "\n";
    for (int i = 0; i < _num_players; i++){
        cout << "player " << i+1 << ": " << players[i].score << "\n";
    }
    cout << "##### TOTAL SCORES #####" << "\n";
}

void show_deck(){
    for(string card : cur_deck){
        cout << card << " ";
    }

    cout << "\n";
}

int new_turn(int _cur_turn, int _player_num){
    int new_turn = 0;
    if (_cur_turn == _player_num - 1){
        new_turn = 0;
    }
    else{
        new_turn += _cur_turn + 1;
    }
}

void end_round(int _num_players, int& _round){

    //get last player
    int last_player = 0;
    if (cur_turn == 0){
        last_player = _num_players - 1;
    }
    else{
        last_player = cur_turn - 1;
    }

    //check if one hand is completly revealed
    bool flagged = false;
    for (int r : players[last_player].cards_revealed){
        if (r == 0){
            flagged = true;
        }
    }

    if (flagged == false){
        //reveal all hands
        for (int i = 0; i < _num_players; i++){
            fill(begin(players[i].cards_revealed), end(players[i].cards_revealed), 1);
            players[i].score += score(players[i]);
        }

        if (bot_sim == false){
            display_board(_num_players, _round, cur_turn);
            cout << "\n" << "ROUND OVER!" << "\n";
            Sleep(2000);
            display_score(_num_players);
            Sleep(2000);
        }

        //reset player hands
        for (int i = 0; i < _num_players; i++){
            fill(begin(players[i].cards_revealed), end(players[i].cards_revealed), 0);
        }

        //reset piles
        discard_pile.clear();
        cur_deck.clear();
        cur_deck.insert(cur_deck.end(), &std_deck[0], &std_deck[52]);

        //shuffle deck
        show_deck();
        auto rng = default_random_engine{};
        shuffle(cur_deck.begin(), cur_deck.end(), rng);
        show_deck();

        //deal hands
        dealhands(_num_players);

        //turn over first card
        discard_pile.push_back(cur_deck[0]);
        cur_deck.erase(cur_deck.begin());

        cur_turn = 0;
        _round += 1;
    }

}

//card choice must be within range 0-5
void flip_logic(int card_choice, int num_players){

    flip(cur_turn, card_choice);

    if (cur_turn == num_players - 1){
        cur_turn = 0;
    }
    else{
        cur_turn += 1;
    }
}
//card choice must be within range 0-6
void draw_logic(int card_choice, int num_players){
    draw();
    if (card_choice == 6){
        discard_pile.insert(discard_pile.begin(), card_temp);
        card_temp;
    }
    else{
        replace(cur_turn, card_choice);
    }

    if (cur_turn == num_players - 1){
        cur_turn = 0;
    }
    else{
        cur_turn += 1;
    }
}
//card choice must be within range 0-5
void draw_discard_logic(int card_choice, int num_players){
    draw_discard();

    replace(cur_turn, card_choice);
    if (cur_turn == num_players - 1){
        cur_turn = 0;
    }
    else{
        cur_turn += 1;
    }
}

void bot_template_turn(int num_players){
    int choice = 2;//(rand() % 3) + 1;
    int card_choice = 0;
    vector <int> open_choices;
    for (int i = 0; i < 6; i++){
        if (players[cur_turn].cards_revealed[i] == 0){
            open_choices.push_back(i);
        }
    }
    switch (choice){
        case 1:
            card_choice = open_choices[rand() % open_choices.size()];

            flip_logic(card_choice, num_players);

            break;
            
        case 2:
            card_choice = rand() % 7;

            draw_logic(card_choice, num_players);

            break;
        case 3:
            card_choice = rand() % 7;

            draw_discard_logic(card_choice, num_players);

            break;
    }
}

void user_turn(int num_players, int round){
    bool turn_over = false;
    while (turn_over == false){
        int choice = 0;
        int card_choice = 0;
        display_board(num_players, round, cur_turn);
        cout << "\n\n" << "1. Reveal Card" << "\n";
        cout << "2. Draw Card From Deck" << "\n";
        cout << "3. Draw Card From Discard Pile" << "\n";
        cout << "4. Quit" << "\n";
        cout << "Please Choose One Option: ";

        cin >> choice;

        if (choice > 0 && choice < 5){
            switch (choice){
                case 1:
                    display_board(num_players, round, cur_turn);
                    cout << "\n\n" << "1 2 3" << "\n";
                    cout << "4 5 6" << "\n";
                    cout << "7. Back" << "\n";
                    cout << "Please Choose One Option: ";

                    cin.clear();
                    fflush(stdin);
                    cin >> card_choice;
                    
                    if (card_choice == 7 || players[cur_turn].cards_revealed[card_choice] == 1){
                        break;
                    }

                    flip(cur_turn, card_choice-1);

                    if (cur_turn == num_players - 1){
                        cur_turn = 0;
                    }
                    else{
                        cur_turn += 1;
                    }
                    turn_over = true;

                    break;
                case 2:
                    draw();
                    display_board(num_players, round, cur_turn);
                    cout << "\n\n" << "Card Drawn: " << card_temp << "\n";
                    cout << "1 2 3" << "\n";
                    cout << "4 5 6" << "\n";
                    cout << "7. Discard Pile" << "\n";
                    cout << "Please Choose One Option: ";

                    cin.clear();
                    fflush(stdin);
                    cin >> card_choice;

                    if (card_choice == 7){
                        discard_pile.insert(discard_pile.begin(), card_temp);
                        card_temp;
                    }
                    else{
                        replace(cur_turn, card_choice-1);
                    }

                    if (cur_turn == num_players - 1){
                        cur_turn = 0;
                    }
                    else{
                        cur_turn += 1;
                    }
                    turn_over = true;
                    break;
                case 3:
                    draw_discard();
                    display_board(num_players, round, cur_turn);
                    cout << "\n\n" << "Card Taken: " << card_temp << "\n";
                    cout << "1 2 3" << "\n";
                    cout << "4 5 6" << "\n";
                    cout << "7. Back" << "\n";
                    cout << "Please Choose One Option: ";

                    cin.clear();
                    fflush(stdin);
                    cin >> card_choice;

                    if (card_choice == 7){
                        break;
                    }

                    replace(cur_turn, card_choice-1);
                    if (cur_turn == num_players - 1){
                        cur_turn = 0;
                    }
                    else{
                        cur_turn += 1;
                    }
                    turn_over = true;
                    break;
                case 4:
                    round = max_rounds+1;
                    turn_over = true;
                    break;
            }
        }
    }
    
}

int main(){
    int round = 1;
    int choice;
    int card_choice;

    //initlize scores and deck
    scores['A'] = 1; 
    scores['K'] = 0;
    scores['J'] = 11;
    scores['Q'] = 12;

    cur_deck.insert(cur_deck.end(), &std_deck[0], &std_deck[52]);

    //bot sim check
    int bot_sim_choice;
    cout << "Bot sim? 1 = yes, 2 = no: ";
    cin >> bot_sim_choice;

    while (bot_sim_choice < 1 || bot_sim_choice > 2){
        cout << "Sorry, invalid choice" << "\n";
        cout << "Bot sim? 1 = yes, 2 = no: ";
        cin >> bot_sim_choice;
        cout << "\n";
    }

    if (bot_sim_choice == 1){
        bot_sim = true;
    }
    else if(bot_sim_choice == 2){
        bot_sim = false;
    }

    //get number of players from user
    int num_players = 0;
    cout << "Numbers of players: ";
    cin >> num_players;

    while (num_players > max_players || num_players < 2){
        cout << "Sorry, invalid number of players" << "\n";
        cout << "Numbers of players: ";
        cin >> num_players;
        cout << "\n";
    }

    //initialize players
    for (int i = 0; i < num_players; i++){
        players.emplace_back(player{0, false});
    }

    if (bot_sim == false){
        int num_bots = 0;
        cout << "Numbers of bots: ";
        cin >> num_bots;

        while (num_bots > num_players || num_bots < 1){
            cout << "Sorry, invalid number of bots" << "\n";
            cout << "Numbers of bots: ";
            cin >> num_players;
            cout << "\n";
        }

        if (num_bots == num_players){
            for (int i = 0; i < num_players; i++){
                players[i].bot = true;
            }
        }
        else{
            for (int i = num_players - 1; i > num_players - num_bots - 1; i--){
                players[i].bot = true;
            }
        }
    }
    else{
        for (int i = 0; i < num_players; i++){
            players[i].bot = true;
        }

        cout << "number of games: ";
        cin >> num_games;

    }
    

    //shuffle deck
    show_deck();
    auto rng = default_random_engine{};
    shuffle(cur_deck.begin(), cur_deck.end(), rng);
    show_deck();

    //deal hands
    dealhands(num_players);

    //turn over first card
    discard_pile.push_back(cur_deck[0]);
    cur_deck.erase(cur_deck.begin());
    
    while (game < num_games){
        while (round < max_rounds){
            if (players[cur_turn].bot != true){
                user_turn(num_players, round);
            }
            else if(skip_bots == false){
                bot_template_turn(num_players);
            }
            else{
                if (cur_turn == num_players - 1){
                    cur_turn = 0;
                }
                else{
                    cur_turn += 1;
                }
            }

            //shuffle discard pile into deck if deck is empty
            if (cur_deck.size() == 0){
                cur_deck = discard_pile;
                discard_pile.clear();
                shuffle(cur_deck.begin(), cur_deck.end(), rng);
                discard_pile.push_back(cur_deck[0]);
                cur_deck.erase(cur_deck.begin());

                //cout << "Shuffled Empty Deck!";
            }

            end_round(num_players, round);
        }
    }
    return 0;
}


/*
####### RULES ########
    each player is dealt 6 cards from a 52 card deck
    these 6 cards are randomly assigned face down in 2 rows of 3
    before play, players choose two cards to turn face up
    a card is then flipped face up from the top of the deck 
    in order, each players choose from __ options
        1. flip a card
        2. draw a card from the deck and replace one of their cards
        3. draw a card from the discard pile and replace a card
    the game ends when a player has revealed all six of their cards

    scoring:
        1. for non face cards, the score of each card is equal to its value
        2. for face cards, the scores are as follows: ace = 1, king = 0, queen = 12, jack = 11 
        3. if the top and bottom card of a col are the same, the scores are cancelled
        4. if a joker is in a col, the score of the col is cancelled

*/

