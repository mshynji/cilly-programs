#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

#ifdef _WIN32
#define os_name "Win32"
#else
#define os_name "Unix_Based"
#endif

void clear_screen() {
    if(os_name == "Win32") {
        system("cls");
    } else {
        system("clear");
    }
}

void pause() {
    std::cout << "Press enter to continue...";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void set_text_color(uint16_t r, uint16_t g, uint16_t b) {
    std::cout << "\033[38;2;" << r << ";" << g << ";" << b << "m";
}

void reset_text_color() {
    std::cout << "\033[0m";
}

class hGame {
private:
    uint32_t   score;
    uint16_t **board;
    bool       board_collisions[4][4];
    
public:
    hGame();
    ~hGame();
    
    bool completed();
    void start_game();
    void add_square();
    void display_board();
    
    bool is_playable();
    bool can_move_up();
    bool can_move_down();
    bool can_move_left();
    bool can_move_right();
    bool has_empty_squares();
    
    void move_up();
    void move_down();
    void move_left();
    void move_right();
    void empty_collisions();
    
    uint32_t   get_score() { return score; }
    uint16_t** get_board() { return board; }
    uint16_t   get_highest_square();
    
    void set_score(uint32_t score);
    void set_board(uint16_t **board);
};

hGame::hGame() : score(0) {
    board = new uint16_t*[4]();
    for(int i = 0; i < 4; i++)
        board[i] = new uint16_t[4]();
        
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++)
            board_collisions[i][j] = false;
    }
}

hGame::~hGame() {
    for(int i = 0; i < 4; i++)
        delete board[i];
        
    delete board;
}

bool hGame::completed() {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(board[i][j] == 2048)
                return true;
        }
    }
    
    return false;
}

void hGame::start_game() {
    score = 0;
    
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++)
            board[i][j] = 0;
    }
    
    add_square();
}

void hGame::add_square() {
    if(!has_empty_squares())
        return;
        
    int i, j;
    int chance = std::rand()%4;
    do {
        i = std::rand()%4;
        j = std::rand()%4;
    } while(board[i][j] != 0);
    
    board[i][j] = (chance == 3) ? 4 : 2;
}

void hGame::display_board() {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            uint16_t r, g, b;
            if(board[i][j] == 0)          { r = 127; g = 127; b = 127; }
            else if (board[i][j] == 2)    { r = 255; g = 255; b = 255; }
            else if (board[i][j] == 4)    { r = 237; g = 224; b = 200; }
            else if (board[i][j] == 8)    { r = 242; g = 177; b = 121; }
            else if (board[i][j] == 16)   { r = 245; g = 149; b = 99;  }
            else if (board[i][j] == 32)   { r = 246; g = 124; b = 96;  }
            else if (board[i][j] == 64)   { r = 246; g = 94;  b = 59;  }
            else if (board[i][j] == 128)  { r = 237; g = 207; b = 115; }
            else if (board[i][j] == 256)  { r = 237; g = 204; b = 98;  }
            else if (board[i][j] == 512)  { r = 237; g = 200; b = 80;  }
            else if (board[i][j] == 1024) { r = 237; g = 197; b = 63;  }
            else if (board[i][j] == 2048) { r = 237; g = 194; b = 45;  }
            
            set_text_color(r, g, b);
            printf("%04d ", board[i][j]);
            reset_text_color();
        }
        std::cout << "\n";
    }
}

bool hGame::is_playable() {
    if(has_empty_squares())
        return true;
    
    if(can_move_up() || can_move_down() || can_move_left() || can_move_right())
        return true;
    
    return false;
}

bool hGame::can_move_up() {
    for(int i = 1; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            int k = i;
            
            while(k > 0) {
                if(board[i-1][j] == 0 || board[i-1][j] == board[i][j])
                    return true;
                k--;
            }
        }
    }
    
    return false;
}

bool hGame::can_move_down() {
    for(int i = 2; i >= 0; i--) {
        for(int j = 0; j < 4; j++) {
            int k = i;
            
            while(k < 3) {
                if(board[i+1][j] == 0 || board[i+1][j] == board[i][j])
                    return true;
                k++;
            }
        }
    }
    
    return false;
}

bool hGame::can_move_left() {
    for(int j = 1; j < 4; j++) {
        for(int i = 0; i < 4; i++) {
            int k = j;
            
            while(k > 0) {
                if(board[i][j-1] == 0 || board[i][j-1] == board[i][j])
                    return true;
                k--;
            }
        }
    }
    
    return false;
}

bool hGame::can_move_right() {
    for(int j = 2; j >= 0; j--) {
        for(int i = 0; i < 4; i++) {
            int k = j;
            
            while(k < 3) {
                if(board[i][j+1] == 0 || board[i][j+1] == board[i][j])
                    return true;
                k++;
            }
        }
    }
    
    return false;
}

bool hGame::has_empty_squares() {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(board[i][j] == 0)
                return true;
        }
    }
    
    return false;
}

uint16_t hGame::get_highest_square() {
    uint16_t highest_square = board[0][0];
    
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(board[i][j] > highest_square)
                highest_square = board[i][j];
        }
    }
    
    return highest_square;
}

void hGame::set_score(uint32_t new_score) {
    score = new_score;
}

void hGame::set_board(uint16_t **new_board) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++)
            board[i][j] = new_board[i][j];
    }
}

void hGame::move_up() {
    if(!can_move_up()) { return; }
    
    bool has_moved = false;
    
    for(int i = 1; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(board[i][j] == 0) { continue; }
            
            int k = i;
            while(k > 0) {
                if(board[k][j] != 0) {
                    if((board[k-1][j] == 0 || board[k-1][j] == board[k][j]) && !board_collisions[k-1][j] && !board_collisions[k][j]) {
                        if(board[k-1][j] == board[k][j])
                            board_collisions[k-1][j] = true;
                        
                        board[k-1][j] += board[k][j];
                        board[k][j] = 0;
                        
                        has_moved = true;
                    }
                }
                
                k--;
            }
        }
    }
    
    if(has_moved) { add_square(); }
    empty_collisions();
}

void hGame::move_down() {
    if(!can_move_down()) { return; }
    
    bool has_moved = false;
    
    for(int i = 2; i >= 0; i--) {
        for(int j = 0; j < 4; j++) {
            if(board[i][j] == 0) { continue; }
            
            int k = i;
            while(k < 3) {
                if(board[k][j] != 0) {
                    if((board[k+1][j] == 0 || board[k+1][j] == board[k][j]) && !board_collisions[k+1][j] && !board_collisions[k][j]) {
                        if(board[k+1][j] == board[k][j])
                            board_collisions[k+1][j] = true;
                        
                        board[k+1][j] += board[k][j];
                        board[k][j] = 0;
                        
                        has_moved = true;
                    }
                }
                
                k++;
            }
        }
    }
    
    if(has_moved) { add_square(); }
    empty_collisions();
}

void hGame::move_left() {
    if(!can_move_left()) { return; }
    
    bool has_moved = false;
    
    for(int j = 1; j < 4; j++) {
        for(int i = 0; i < 4; i++) {
            if(board[i][j] == 0) { continue; }
            
            int k = j;
            while(k > 0) {
                if(board[i][k] != 0) {
                    if((board[i][k-1] == 0 || board[i][k-1] == board[i][k]) && !board_collisions[i][k-1] && !board_collisions[i][k]) {
                        if(board[i][k-1] == board[i][k])
                            board_collisions[i][k-1] = true;
                        
                        board[i][k-1] += board[i][k];
                        board[i][k] = 0;
                        
                        has_moved = true;
                    }
                }
                
                k--;
            }
        }
    }
    
    if(has_moved) { add_square(); }
    empty_collisions();
}

void hGame::move_right() {
    if(!can_move_right()) { return; }
    
    bool has_moved = false;
    
    for(int j = 2; j >= 0; j--) {
        for(int i = 0; i < 4; i++) {
            if(board[i][j] == 0) { continue; }
            
            int k = j;
            while(k < 3) {
                if(board[i][k] != 0) {
                    if((board[i][k+1] == 0 || board[i][k+1] == board[i][k]) && !board_collisions[i][k+1] && !board_collisions[i][k]) {
                        if(board[i][k+1] == board[i][k])
                            board_collisions[i][k+1] = true;
                        
                        board[i][k+1] += board[i][k];
                        board[i][k] = 0;
                        
                        has_moved = true;
                    }
                }
                
                k++;
            }
        }
    }
    
    if(has_moved) { add_square(); }
    empty_collisions();
}

void hGame::empty_collisions() {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(board_collisions[i][j])
                score += board[i][j];
            
            board_collisions[i][j] = false;
        }
    }
}

int main() {
    std::srand(std::time(NULL));
    
    bool     run = true;
    hGame    game_handler;
    uint32_t delta = 0;
    
    while(run) {
        char choice;
        
        clear_screen();
        set_text_color(237, 194, 45);
        std::cout << "2048!\n\n";
        reset_text_color();
        std::cout << "(P) Play\n";
        std::cout << "(E) Exit\n\n";
        std::cout << ":: "; 
        std::cin >> choice;
        
        switch(choice){
            case 'p':
            case 'P':
                game_handler.start_game();
                
                while(game_handler.is_playable() && !game_handler.completed()) {
                    clear_screen();
                    
                    uint32_t score = game_handler.get_score();
                    std::cout << "Score: " << score;
                    if(delta > 0)
                        std::cout << " (+" << delta << ")";
                    std::cout << "\n\n";
                    
                    game_handler.display_board();
                    
                    std::cout << "\nMove:\n";
                    std::cout << "(u) Up     (d) Down     (l) Left     (r) Right\n\n";
                    std::cout << ":: ";
                    std::cin >> choice;
                    switch(choice) {
                        case 'u':
                        case 'U':
                            game_handler.move_up();
                            break;
                        case 'd':
                        case 'D':
                            game_handler.move_down();
                            break;
                        case 'l':
                        case 'L':
                            game_handler.move_left();
                            break;
                        case 'r':
                        case 'R':
                            game_handler.move_right();
                            break;
                        default:
                            clear_screen();
                            std::cout << "Invalid choice :-(\n";
                            pause();
                            
                            break;
                    }
                    
                    delta = game_handler.get_score() - score;
                }
                
                clear_screen();
                if(game_handler.completed()) { std::cout << "Congratulations!!\n\n"; }
                else                         { std::cout << "Better luck next time\n\n"; }
                
                std::cout << "Score: " << game_handler.get_score() << "\n";
                std::cout << "Highest square: " << game_handler.get_highest_square() << "\n\n";
                pause();
                
                break;
                
            case 'e':
            case 'E':
                clear_screen();
                std::cout << "Thank you for playing :-)\n";
                
                run = false;
                
                break;
            
            default:
                clear_screen();
                std::cout << "Invalid choice :-(\n";
                pause();
                
                break;
        }
    }
    
    return 0;
}