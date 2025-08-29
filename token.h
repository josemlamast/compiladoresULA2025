#pragma once

typedef enum
{
    TOKEN_EOF,
    TOKEN_IF,
    TOKEN_ELSE,

    TOKEN_CONCAT,
    TOKEN_RTOS,
    TOKEN_ETOS,
    TOKEN_ETOR,
    TOKEN_RTOE,
      
    TOKEN_EMPTY,
    TOKEN_HEAD,
    TOKEN_TAIL,
      
    TOKEN_ASIG, 
    TOKEN_ADD,
    TOKEN_SUBSTRACT,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE, 
    TOKEN_MOD,

    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_XOR,

    TOKEN_VAL,
    TOKEN_LET,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_INT,
    TOKEN_REAL,
    TOKEN_STRING,
    TOKEN_FUN,
    TOKEN_IN,
    TOKEN_IDENTIFIER,
    TOKEN_UNKNOWN,

    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LCORCH,
    TOKEN_RCORCH,

    TOKEN_PAIR,
    TOKEN_FST,
    TOKEN_SND,

    TOKEN_LESS,
    TOKEN_GREAT,
    TOKEN_LESSEQL,
    TOKEN_GREATEQL,
    TOKEN_NOTEQUAL,
    TOKEN_EQUAL,

    TOKEN_CONMMENT,
    TOKEN_LCONMEN,
    TOKEN_RCONMEN,

    TOKEN_PRINT
}
token_t;

/*#define MAX_BUFFER_LENGTH 1024

typedef char buffer_t[MAX_BUFFER_LENGTH];

typedef struct
{
    token_t  token;
    buffer_t value;
}
Token;*/

inline const char* to_str(token_t t)
{
    switch(t)
    {
        case TOKEN_EOF: return "EOF";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";


        case TOKEN_CONCAT: return "CONCAT";
        case TOKEN_RTOS: return "RTOS";
        case TOKEN_ETOS: return "ETOS";
        case TOKEN_ETOR: return "ETOR";
        case TOKEN_RTOE: return "RTOE";

        case TOKEN_EMPTY: return "EMPTY";
        case TOKEN_HEAD: return "HEAD";
        case TOKEN_TAIL: return "TAIL";

        case TOKEN_ASIG: return "ASIG";
        case TOKEN_ADD: return "ADD";
        case TOKEN_SUBSTRACT: return "SUBSTRACT";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_MOD: return "MOD";

        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_NOT: return "NOT";
        case TOKEN_XOR: return "XOR";


        case TOKEN_VAL: return "VAL";
        case TOKEN_LET: return "LET";
        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";
        case TOKEN_INT: return "INT";
        case TOKEN_REAL: return "REAL";
        case TOKEN_STRING: return "STRING";
        case TOKEN_FUN: return "FUN";
        case TOKEN_IN: return "IN";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_UNKNOWN: return "UNKNOWN";

        case TOKEN_PAIR: return "PAIR";
        case TOKEN_FST: return "FST";
        case TOKEN_SND: return "SND";

        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LCORCH: return "LCORCH";
        case TOKEN_RCORCH: return "RCORCH";

        case TOKEN_LESS: return "LESS";
        case TOKEN_GREAT: return "GREAT";
        case TOKEN_LESSEQL: return "LESSEQL";
        case TOKEN_GREATEQL: return "GREATEQL";
        case TOKEN_NOTEQUAL: return "NOTEQUAL";
        case TOKEN_EQUAL: return "EQUAL";

        case TOKEN_CONMMENT: return "CONMMENT";
        case TOKEN_LCONMEN: return "LCONMEN";
        case TOKEN_RCONMEN: return "RCONMEN";

        case TOKEN_PRINT: return "PRINT";

    }
}