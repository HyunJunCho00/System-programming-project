#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#define MAX_WORD_LENGTH 20
#define TOTAL_WORDS 30
#define DISPLAYED_WORDS 10
#define GAME_DURATION 180 // 게임 지속 시간: 3분
int score = 0;
typedef struct {
 char word[MAX_WORD_LENGTH];
 int x, y;
 int type;
 int score;
 bool visible;
 time_t last_update;
} Word;
Word words[TOTAL_WORDS];
Word displayed_words[DISPLAYED_WORDS];
void initialize_words() {
 const char* word_list[TOTAL_WORDS] = {
 "programming", "keyboard", "developer", "practice", "challenge",
 "language", "computer", "learning", "typing", "software",
 "algorithm", "function", "variable", "debugging", "execution",
 "statement", "condition", "iteration", "efficiency", "memory",
 "interface", "compiler", "framework", "application", "database",
 "security", "network", "operating", "version", "integration"
 };
 for (int i = 0; i < TOTAL_WORDS; i++) {
 strcpy(words[i].word, word_list[i]);
 words[i].x = 0;
 words[i].y = 0;
 words[i].type = 0;
 words[i].score = 0;
 words[i].visible = false;
 words[i].last_update = 0;
 }
}
void init_game() {
 srand(time(NULL));
 initialize_words();
 // 10개의 랜덤 단어 선택
 for (int i = 0; i < DISPLAYED_WORDS; i++) {
 int index;
 do {
 index = rand() % TOTAL_WORDS;
 } while (words[index].visible);
 words[index].visible = true;
 words[index].x = rand() % (COLS - strlen(words[index].word));
 words[index].y = rand() % LINES;
 words[index].last_update = time(NULL);
 // 단어 타입 및 점수 설정
 if (i < 3) {
 words[index].type = 1;
 words[index].score = 5;
 } else if (i >= 3 && i < 6) {
 words[index].type = 2;
 words[index].score = 3;
 } else {
 words[index].type = 3;
 words[index].score = 10;
 }
 displayed_words[i] = words[index];
 }
}
bool should_display(int type, time_t last_update) {
 time_t current_time = time(NULL);
 double elapsed = difftime(current_time, last_update);
 switch (type) {
 case 2: // 2초 주기로 1.5초 동안 보이고, 0.5초 동안 보이지 않음
 case 3: // 2초 주기로 1.5초 동안 보이고, 0.5초 동안 보이지 않음 (하강 포함)
 if (elapsed >= 2.0) {
 words->last_update=current_time;
 }
 return elapsed < 1.0;
 default:
 return true;
 }
}
void display_word(Word *word) {
 if (word->visible) {
 mvprintw(word->y, word->x, "%s", word->word);
 }
}
void update_word_position(Word *word) {
 time_t current_time = time(NULL);
 // 타입 1과 3의 단어는 2초마다 하강
 if (word->visible && (word->type == 1 || word->type == 3)) {
 if (difftime(current_time, word->last_update) >= 2) {
 word->y += 1;
 word->last_update = current_time;
 }
 }
 // 타입 2의 단어는 움직이지 않음
}
void handle_input(char *input) {
 for (int i = 0; i < DISPLAYED_WORDS; i++) {
 if (displayed_words[i].visible && strcmp(input, displayed_words[i].word) == 0) 
{
 score += displayed_words[i].score;
 displayed_words[i].visible = false; // 단어 제거
 break;
 }
 }
}
bool all_words_gone() {
 for (int i = 0; i < DISPLAYED_WORDS; i++) {
 if (displayed_words[i].visible) {
 return false;
 }
 }
 return true;
}
int main() {
 bool replay;
 do {
 replay = false;
 initscr();
 cbreak();
 noecho();
 curs_set(0);
 nodelay(stdscr, TRUE);
 score = 0;
 init_game();
 time_t start_time = time(NULL);
 time_t current_time;
 char input[MAX_WORD_LENGTH] = {0};
 int len = 0;
 while (true) {
 clear();
 for (int i = 0; i < DISPLAYED_WORDS; i++) {
 update_word_position(&displayed_words[i]);
 display_word(&displayed_words[i]);
 }
 int ch = getch();
 if (ch != ERR) {
 if (ch == '\n') { // Enter key
 if (len > 0) {
 handle_input(input);
 memset(input, 0, sizeof(input)); // 입력 버퍼 초기화
 len = 0;
 }
 } else if (ch == '\b' || ch == 127) { // Backspace key
 if (len > 0) {
 input[--len] = '\0'; // 마지막 문자 제거
 }
 } else if (len < MAX_WORD_LENGTH - 1) {
 input[len++] = ch;
 input[len] = '\0';
 }
 }
 mvprintw(LINES - 1, 0, "Input: %s", input); // 입력 표시
 current_time = time(NULL);
 mvprintw(0, 0, "Score: %d", score);
 mvprintw(0, COLS - 20, "Time: %ld", GAME_DURATION - (current_time - start_time));
 refresh();
 if ((current_time - start_time) > GAME_DURATION || all_words_gone()) {
 break;
 }
 usleep(100000); // 0.1초 대기
 }
 nodelay(stdscr, FALSE);
 clear();
 mvprintw(LINES / 2, (COLS - strlen("Game Over. Play again? (y/n)")) / 2, 
"Game Over. Play again? (y/n)");
 refresh();
 while (true) {
 int response = getch();
 if (response == 'y' || response == 'Y') {
 replay = true;
 break;
 } else if (response == 'n' || response == 'N') {
 break;
 }
 }
 endwin();
 } while (replay);
 return 0;
}