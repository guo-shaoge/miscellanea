/*
 * a simple lisp parser
 * op ::= '+' | '-' | '*' | '/'
 * expr ::= num | '(' expr ')' | '(' op expr expr ')'
 * num ::= digit*
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int token ;
int token_val;
char buf[128];
char *src;
enum {OP, NUM, L_PAREN, R_PAREN};

int main();
void error(const char *s);
int expr();
void next();

void next()
{
    while (*src == ' ' || *src == '\n')
        src++;

    token = *src++;
    if (token == '+' || token == '-' || token == '*' || \
            token == '/') {
        token_val = token;
        token = OP;
    } else if (token >= '0' && token <= '9') {
        token_val = token - '0';
        token = NUM;
        while (*src >= '0' && *src <= '9') {
            token_val = token_val * 10 + *src - '0';
            src++;
        }
    } else if (token == '(') {
        token = L_PAREN;
    } else if (token == ')') {
        token = R_PAREN;
    }
}

int expr()
{
    next();
    if (token == NUM) {
        return token_val;
    } else if (token == L_PAREN) {
        next();
        if (token == OP) {
            int l, r, ret;
            if (token_val == '+') {
                l = expr();
                r = expr();
                ret = l+r;
            } else if (token_val == '-') {
                l = expr();
                r = expr();
                ret = l-r;
            } else if (token_val == '*') {
                l = expr();
                r = expr();
                ret = l*r;
            } else if (token_val == '/') {
                l = expr();
                r = expr();
                ret = l/r;
            }
            next();
            if (token != R_PAREN)
                error("expected right parenthese");
            return ret;
        } else {
            expr();
        }
    } else {
        error("unexpected char");
    }
}

void error(const char *s)
{
    printf("%s(src: %s)\n", s, src);
    exit(-1);
}

int main()
{
    src = buf;

    while (1) {
        memset(buf, 0, sizeof(buf));
        printf("input: ");
        if (fgets(buf, sizeof(buf), stdin) == NULL) 
            error("fgets error");
        printf("debug: %s\n", src);
        printf("%s --> %d\n", buf, expr());
    }
}
