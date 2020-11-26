//
// Created by Matúš on 26. 11. 2020.
//

#ifndef IFJSTACK_PSA_STACK_H
#define IFJSTACK_PSA_STACK_H
typedef enum
{
    OP_EQUAL,         // ==
    OP_NOT_EQUAL,     // !=
    OP_LESS_THAN,       //<
    OP_LESS_OR_EQUAL,  //<=
    OP_MORE_THAN,       //>
    OP_MORE_OR_EQUAL,   //>=

    OP_ASSIGN,  //=
    OP_INIT,    //:=
    OP_ADD,     //+
    OP_SUB,     //-
    OP_MUL,     //*
    OP_DIV,     // /

    OP_L_BRACKET,   // (
    OP_R_BRACKET,   // )
    OP_BLOCK_BEGIN, // {
    OP_BLOCK_END,   // }
    OP_COMMA,       // ,
    OP_SEMICOLON,   // ;
}operator_s;
typedef enum {
    INT,
    FLOAT64,
    STRING,
}Datatype;

typedef struct
{
    enum operator_s operator_ss;
    Datatype datatype;
    union {
        int integer;
        double decimal;
        char *string;
    }utility;
    struct Stack_item *lptr;
    struct Stack_item *rptr;
}Stack_item;



typedef struct
{
    Stack_item *top;
    Stack_item *active;

} Psa_stack;

Psa_stack *s_init();
void s_destroy(Psa_stack *stack);
void s_pop (Psa_stack *stack);
int s_push (Psa_stack *stack, enum operator_s operator_ss);



#endif //IFJSTACK_PSA_STACK_H
