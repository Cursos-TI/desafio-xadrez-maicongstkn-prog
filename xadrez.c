#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define SIZE 8

char board[SIZE][SIZE];

// Inicializa o tabuleiro com posição inicial do xadrez
void initBoard() {
    char initial[8][9] = {
        "rnbqkbnr",
        "pppppppp",
        "        ",
        "        ",
        "        ",
        "        ",
        "PPPPPPPP",
        "RNBQKBNR"
    };
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = initial[i][j];
}

// Converte coordenadas (a-h) e (1-8) para índices do array
int colToIndex(char c) { return c - 'a'; }
int rowToIndex(char r) { return 8 - (r - '0'); }

// Verifica se destino é válido (vazio ou peça do oponente)
int isValidTarget(int toRow, int toCol, char turn) {
    if (toRow < 0 || toRow >= SIZE || toCol < 0 || toCol >= SIZE) return 0;
    char target = board[toRow][toCol];
    if (target == ' ') return 1;
    if (turn == 'w' && islower(target)) return 1;
    if (turn == 'b' && isupper(target)) return 1;
    return 0;
}

// Movimentos das peças
int validPawn(int fr, int fc, int tr, int tc, char turn) {
    int dir = (turn == 'w') ? -1 : 1;
    if (fc == tc && board[tr][tc] == ' ' && tr == fr + dir) return 1;
    if (fc == tc && board[tr][tc] == ' ' && ((turn == 'w' && fr == 6) || (turn == 'b' && fr == 1)) &&
        tr == fr + 2 * dir && board[fr + dir][fc] == ' ') return 1;
    if ((tc == fc + 1 || tc == fc - 1) && tr == fr + dir && board[tr][tc] != ' ') return 1;
    return 0;
}

int validRook(int fr, int fc, int tr, int tc, char turn) {
    if (fr != tr && fc != tc) return 0;
    int rStep = (tr - fr) ? ((tr > fr) ? 1 : -1) : 0;
    int cStep = (tc - fc) ? ((tc > fc) ? 1 : -1) : 0;
    int r = fr + rStep, c = fc + cStep;
    while (r != tr || c != tc) {
        if (board[r][c] != ' ') return 0;
        r += rStep; c += cStep;
    }
    return isValidTarget(tr, tc, turn);
}

int validBishop(int fr, int fc, int tr, int tc, char turn) {
    if (abs(tr - fr) != abs(tc - fc)) return 0;
    int rStep = (tr > fr) ? 1 : -1;
    int cStep = (tc > fc) ? 1 : -1;
    int r = fr + rStep, c = fc + cStep;
    while (r != tr && c != tc) {
        if (board[r][c] != ' ') return 0;
        r += rStep; c += cStep;
    }
    return isValidTarget(tr, tc, turn);
}

int validKnight(int fr, int fc, int tr, int tc, char turn) {
    int dr = abs(tr - fr), dc = abs(tc - fc);
    if ((dr == 2 && dc == 1) || (dr == 1 && dc == 2)) return isValidTarget(tr, tc, turn);
    return 0;
}

int validKing(int fr, int fc, int tr, int tc, char turn) {
    if (abs(tr - fr) <= 1 && abs(tc - fc) <= 1) return isValidTarget(tr, tc, turn);
    return 0;
}

int validQueen(int fr, int fc, int tr, int tc, char turn) {
    return validRook(fr, fc, tr, tc, turn) || validBishop(fr, fc, tr, tc, turn);
}

// Valida o movimento de acordo com a peça
int validMove(int fr, int fc, int tr, int tc, char turn) {
    char piece = board[fr][fc];
    if (turn == 'w' && !isupper(piece)) return 0;
    if (turn == 'b' && !islower(piece)) return 0;

    switch (tolower(piece)) {
        case 'p': return validPawn(fr, fc, tr, tc, turn);
        case 'r': return validRook(fr, fc, tr, tc, turn);
        case 'n': return validKnight(fr, fc, tr, tc, turn);
        case 'b': return validBishop(fr, fc, tr, tc, turn);
        case 'q': return validQueen(fr, fc, tr, tc, turn);
        case 'k': return validKing(fr, fc, tr, tc, turn);
    }
    return 0;
}

// Realiza o movimento se válido
int movePieceValidated(char* move, char turn) {
    int fr = rowToIndex(move[1]), fc = colToIndex(move[0]);
    int tr = rowToIndex(move[3]), tc = colToIndex(move[2]);

    if (!validMove(fr, fc, tr, tc, turn)) {
        printf("Movimento inválido!\n");
        return 0;
    }

    board[tr][tc] = board[fr][fc];
    board[fr][fc] = ' ';
    return 1;
}

// Verifica se o rei do jogador ainda está no tabuleiro
int checkMate(char turn) {
    char king = (turn == 'w') ? 'K' : 'k';
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (board[i][j] == king) return 0;
    return 1;
}

// Imprime o tabuleiro ASCII
void printBoard() {
    printf("\n   a b c d e f g h\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", 8 - i);
        for (int j = 0; j < SIZE; j++) {
            char piece = board[i][j];
            if (piece != ' ') printf("%c ", piece);
            else printf("%c ", ((i + j) % 2 == 0) ? '.' : '#');
        }
        printf("%d\n", 8 - i);
    }
    printf("   a b c d e f g h\n\n");
}

int main() {
    initBoard();
    char move[5];
    char turn = 'w';

    while (1) {
        printBoard();
        printf("Vez do jogador %c. Digite o movimento (ex: e2e4 ou 'exit'): ", turn);
        scanf("%4s", move);

        if (strcmp(move, "exit") == 0) break;

        if (movePieceValidated(move, turn)) {
            if (checkMate(turn)) {
                printf("Jogador %c perdeu! Xeque-mate!\n", turn);
                break;
            }
            turn = (turn == 'w') ? 'b' : 'w';
        }
    }

    printf("Fim do jogo.\n");
    return 0;
}
