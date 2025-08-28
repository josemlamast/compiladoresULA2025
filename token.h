#pragma once

typedef enum
{
    TOKEN_EOF,
    TOKEN_IF,
    TOKEN_ELSE,

   TOKEN_CONCAT,
   TOKEN_CONVER_NRTOS,
   TOKEN_CONVER_NETOS,
   TOKEN_CONVER_NETONR,
   TOKEN_CONVER_NRTONE,
      
    TOKEN_EMPTY,
    TOKEN_HEAD,
    TOKEN_TAIL,
      
    TOKEN_ASIG, 
    TOKEN_ADD,
    TOKEN_SUBSTRACT,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE, 
    TOKEN_RESTODIV,

    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,
    TOKEN_XOR,

    TOKEN_VAL,
    TOKEN_LET,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NENTERO,
    TOKEN_NREAL,
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

    TOKEN_MEN,
    TOKEN_MAY,
    TOKEN_MENEQL,
    TOKEN_MAYEQL,
    TOKEN_DIF,
    TOKEN_EQUAL,

    TOKEN_CONMEN,
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
        case TOKEN_CONVER_NRTOS: return "CONVER_NRTOS";
        case TOKEN_CONVER_NETOS: return "CONVER_NETOS";
        case TOKEN_CONVER_NETONR: return "CONVER_NETONR";
        case TOKEN_CONVER_NRTONE: return "CONVER_NRTONE";

        case TOKEN_EMPTY: return "EMPTY";
        case TOKEN_HEAD: return "HEAD";
        case TOKEN_TAIL: return "TAIL";

        case TOKEN_ASIG: return "ASIG";
        case TOKEN_ADD: return "ADD";
        case TOKEN_SUBSTRACT: return "SUBSTRACT";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_RESTODIV: return "RESTODIV";

        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_NOT: return "NOT";
        case TOKEN_XOR: return "XOR";


        case TOKEN_VAL: return "VAL";
        case TOKEN_LET: return "LET";
        case TOKEN_TRUE: return "TRUE";
        case TOKEN_FALSE: return "FALSE";
        case TOKEN_NENTERO: return "NENTERO";
        case TOKEN_NREAL: return "NREAL";
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

        case TOKEN_MEN: return "MEN";
        case TOKEN_MAY: return "MAY";
        case TOKEN_MENEQL: return "MENEQL";
        case TOKEN_MAYEQL: return "MAYEQL";
        case TOKEN_DIF: return "DIF";
        case TOKEN_EQUAL: return "EQUAL";

        case TOKEN_CONMEN: return "CONMEN";
        case TOKEN_LCONMEN: return "LCONMEN";
        case TOKEN_RCONMEN: return "RCONMEN";

        case TOKEN_PRINT: return "PRINT";

    }
}