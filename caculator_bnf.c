/*
 * a simple caculator.
 *
 * expr ::= expr op1 term | term
 * term ::= term op2 factor | factor
 * factor ::= ( expr ) | num
 *
 * ----after removing left recursion----
 *
 * expr ::= term expr_tail
 * expr_tail ::= op1 term expr_tail | EMPTY
 * term ::= factor term_tail 
 * term_tail ::= op2 factor term_tail | EMPTY
 * factor ::= (expr) | num
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int token = 0;
int token_val = 0;
char *src;
enum {Num};

int main();
void get_next_token();
int match(int expected_tk);
int factor();
int term();
int term_tail(int v_so_far);
int expr();
int expr_tail(int v_so_far);
void error(const char *s);

int expr() {
    int v_so_far =  term();
    return expr_tail(v_so_far);
}

int expr_tail(int v_so_far) {
    if (token == '+') {
        match('+');
        v_so_far = v_so_far + term();
    } else if (token == '-') {
        match('-');
        v_so_far = v_so_far - term();
    } else {
        return v_so_far;
    }
    return expr_tail(v_so_far);
}

int term() {
    int v_so_far = factor();
    return term_tail(v_so_far);
}

int term_tail(int v_so_far) {
    if (token == '*') {
        match('*');
        v_so_far = v_so_far * factor();
    } else if (token == '/') {
        match('/');
        v_so_far = v_so_far / factor();
    } else {
        return v_so_far;
    }
    return term_tail(v_so_far);
}

int factor() {
    int result = 0;
    if (token == '(') {
        match('(');
        result = expr();
        match(')');
    } else if (token == Num) {
        match(Num);
        result = token_val;
    }
    return result;
}

int match(int expected_tk) {
    if (expected_tk != token) {
        printf("expected: %d(%c), but get %d(%c)\n", expected_tk,\
                 expected_tk, token, token);
        exit(-1);
    }
    get_next_token();
}

void get_next_token() {
   while (*src == ' ' || *src == '\t' || *src == '\n')
      src++;
    token = *src++;
    if (token <= '9' && token >= '0') {
        //only base dec
        token_val = token - '0';
        token = Num;
        while (*src <= '9' && *src >= '0') {
            token_val = token_val * 10 + *src - '0';
            src++;
        }
        return ;
    }
}


int main() {
    char line[100] = {0};
    while (scanf("%s", line)) {
        line[strlen(line)] = 0;
        src = line;
        get_next_token();
        printf("%d\n", expr());
    }
    return 0;
}


//-------------------------------------------------

void error(const char *s) {
    fprintf(stderr, "token:%d(%c) %s", token, s);
    exit(-1);
}
