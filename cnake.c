#include <stdio.h> // printf
#include <unistd.h> // usleep
#include <sys/ioctl.h> // ioctl
#include <termios.h> // tcgetattr/tcsetattr
#include <fcntl.h> // fcntl
#include <signal.h> // signal

void clear_screen() {
    printf("\033[2J");
}

void print_frame(int rows, int cols) {
    int i;

    for (i = 1; i <= cols; i++) {
        // Top row
        printf("\033[0;%dH-", i);
        // Bottom row
        printf("\033[%d;%dH-", rows, i);
    }

    for (i = 1; i <= rows; i++) {
        // Left column
        printf("\033[%d;0H|", i);
        // Right column
        printf("\033[%d;%dH|", i, cols);
    }
}

enum dir {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

struct pos {
    int row;
    int col;
    enum dir dir;
    struct pos *next;
};

void print_snake(struct pos *snake) {
    struct pos *next = snake;
    while ((next = next->next) != NULL)
        printf("\033[%d;%dH+", next->row, next->col);

    char head;
    switch (snake->dir) {
        case LEFT:
            head = '<';
            break;
        case RIGHT:
            head = '>';
            break;
        case UP:
            head = 'A';
            break;
        case DOWN:
            head = 'V';
            break;
    }
    printf("\033[%d;%dH%c", snake->row, snake->col, head);
}

void print_candy(struct pos *candy, int len) {
    while (len-- > 0) {
        struct pos c = candy[len];
        if (c.row > 0) {
            printf("\033[%d;%dHX", c.row, c.col);
        }
    }
}

void handle_input(char input, struct pos *snake) {
    switch (input) {
        case 'j':
            if (snake->dir != UP)
                snake->dir = DOWN;
            break;
        case 'k':
            if (snake->dir != DOWN)
                snake->dir = UP;
            break;
        case 'h':
            if (snake->dir != RIGHT)
                snake->dir = LEFT;
            break;
        case 'l':
            if (snake->dir != LEFT)
                snake->dir = RIGHT;
            break;
    }
}

void move_snake(struct pos *snake, struct pos *prev) {
    // Recursive move to start with tail
    if (snake->next != NULL)
        move_snake(snake->next, snake);
    else
        // Clear tail
        printf("\033[%d;%dH ", snake->row, snake->col);

    if (prev != NULL) {
        snake->row = prev->row;
        snake->col = prev->col;
        return;
    }

    switch (snake->dir) {
       case LEFT:
           snake->col--;
           break;
       case RIGHT:
           snake->col++;
           break;
       case UP:
           snake->row--;
           break;
       case DOWN:
           snake->row++;
           break;
   }
}

int running = 1;

void stop() {
    running = 0;
}

int main() {
    // Turn off output buffer
    setbuf(stdout, NULL);

    // Turn off input buffer and echo
    struct termios ctrl;
    tcgetattr(STDIN_FILENO, &ctrl);
    ctrl.c_lflag &= ~ICANON;
    ctrl.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &ctrl);

    // Make stdin non-blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    // Get terminal window size
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    // Hide cursor
    printf("\033[?25l");

    // Catch sigterm and stop loop
    signal(SIGINT, stop);

    // Create snake at the center
    struct pos snake = { .row = size.ws_row / 2, .col = size.ws_col / 2 };

    struct pos rrr = { .row = snake.row, .col = snake.col+1 };
    struct pos rrr2 = { .row = snake.row, .col = snake.col+2 };
    struct pos rrr3 = { .row = snake.row, .col = snake.col+3 };
    rrr2.next = &rrr3;
    rrr.next = &rrr2;
    snake.next = &rrr;

    // Bucket of candy
    struct pos candy[5];
    for (int i = 0; i < 5; i++)
        candy[i].row = 0;

    // TEMP
    candy[0].row = 3;
    candy[0].col = 20;

    clear_screen();
    print_frame(size.ws_row, size.ws_col);

    char buf[1]; int n;
    while (running) {
        move_snake(&snake, NULL);
        print_candy(candy, 5);
        print_snake(&snake);
        if ((n = read(0, buf, 1))) {
            handle_input(buf[0], &snake);
        }
        usleep(100000);
    }

    clear_screen();

    // Reset cursor hidden state
    printf("\033[?25h");

    // Reset terminal controls
    printf("\033c");
    fcntl(STDIN_FILENO, F_SETFL, flags);
    ctrl.c_lflag |= ICANON;
    ctrl.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &ctrl);

}
