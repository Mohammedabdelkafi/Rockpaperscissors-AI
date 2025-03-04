#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_MOVES 3
#define MAX_HISTORY 100

const char *moves[MAX_MOVES] = {"rock", "paper", "scissors"};
const char* get_winner(const char *user_move, const char *ai_move) {
    if (strcmp(user_move, ai_move) == 0) {
        return "tie";
    } else if ((strcmp(user_move, "rock") == 0 && strcmp(ai_move, "scissors") == 0) ||
               (strcmp(user_move, "paper") == 0 && strcmp(ai_move, "rock") == 0) ||
               (strcmp(user_move, "scissors") == 0 && strcmp(ai_move, "paper") == 0)) {
        return "user";
    } else {
        return "ai";
    }
}

const char* predict_next_move(char user_history[MAX_HISTORY][10], int history_length, int markov_chain[MAX_MOVES][MAX_MOVES]) {
    if (history_length < 2) {
        return moves[rand() % MAX_MOVES];
    }
    
    const char *last_move = user_history[history_length - 1];
    int last_move_index = -1;
    for (int i = 0; i < MAX_MOVES; ++i) {
        if (strcmp(last_move, moves[i]) == 0) {
            last_move_index = i;
            break;
        }
    }

    if (last_move_index != -1) {
        int max_count = 0;
        int predicted_move_index = rand() % MAX_MOVES;
        for (int i = 0; i < MAX_MOVES; ++i) {
            if (markov_chain[last_move_index][i] > max_count) {
                max_count = markov_chain[last_move_index][i];
                predicted_move_index = i;
            }
        }
        if (strcmp(moves[predicted_move_index], "rock") == 0) {
            return "paper";
        } else if (strcmp(moves[predicted_move_index], "paper") == 0) {
            return "scissors";
        } else {
            return "rock";
        }
    } else {
        return moves[rand() % MAX_MOVES];
    }
}

void update_markov_chain(char user_history[MAX_HISTORY][10], int history_length, int markov_chain[MAX_MOVES][MAX_MOVES]) {
    if (history_length < 2) {
        return;
    }
    
    const char *prev_move = user_history[history_length - 2];
    const char *next_move = user_history[history_length - 1];
    
    int prev_move_index = -1;
    int next_move_index = -1;
    for (int i = 0; i < MAX_MOVES; ++i) {
        if (strcmp(prev_move, moves[i]) == 0) {
            prev_move_index = i;
        }
        if (strcmp(next_move, moves[i]) == 0) {
            next_move_index = i;
        }
    }
    
    if (prev_move_index != -1 && next_move_index != -1) {
        markov_chain[prev_move_index][next_move_index]++;
    }
}

void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int main() {
    srand(time(0));
    printf("Welcome to Rock-Paper-Scissors with Markov Chain Prediction!\n");
    
    char user_history[MAX_HISTORY][10];
    int history_length = 0;
    int markov_chain[MAX_MOVES][MAX_MOVES] = {0};
    
    while (1) {
        printf("Enter your move (rock, paper, scissors) or 'quit' to exit: ");
        char user_move[10];
        scanf("%9s", user_move);
        to_lowercase(user_move);
        
        if (strcmp(user_move, "quit") == 0) {
            break;
        }
        if (strcmp(user_move, "rock") != 0 && strcmp(user_move, "paper") != 0 && strcmp(user_move, "scissors") != 0) {
            printf("Invalid move! Please enter rock, paper, or scissors.\n");
            continue;
        }
        
        const char *ai_move = predict_next_move(user_history, history_length, markov_chain);
        strcpy(user_history[history_length++], user_move);
        update_markov_chain(user_history, history_length, markov_chain);
        const char *winner = get_winner(user_move, ai_move);
        
        if (strcmp(winner, "tie") == 0) {
            printf("It's a tie!\n");
        } else if (strcmp(winner, "user") == 0) {
            printf("You win!\n");
        } else {
            printf("AI wins!\n");
        }
    }
    
    return 0;
}
