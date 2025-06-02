#include "ast.h"
#include "parser.h"
#include<iostream>
using namespace  std;
extern int yydebug;
extern int yyparse();
extern Func* root;


int main(int argc, char *argv[]) {
    while (true) {
        yydebug = 0;  // Enable debug if needed
        yyparse();
    }
    
    return 0;
}