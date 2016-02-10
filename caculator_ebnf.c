/*
 * a simple caculator(using ebnf)
 *
 * expr ::= term { op1 term }
 * term ::= factor { op2 factor }
 * factor ::= '(' expr ')' | num
 * op1 ::= '+' | '-'
 * op2 ::= '*' | '/'
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
int expr();
void error(const char *s);

int expr() {
    int result;
    result = term();
    while (token=='+' || token=='-') {
        if (token == '+') {
            match('+');
            result = result + term();
        } else if (token == '-') {
            match('-');
            result = result - term();
        }
    }
    return result;
}

int term() {
    int result = 0;
    result = factor();
    while (token=='*' || token=='/') {
        if (token == '/') {
            match('/');
            result = result/factor();
        } else if (token == '*') {
            match('*');
            result = result*factor();
        }
    }
    return result;
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
    } else {
        error("factor error\n");
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
