#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    INVALIED,
    IDENTIFIER,
    INT_KEYWORD,      // int
    OPENPARENTHESIS,  // (
    CLOSEPARENTHESIS, // )
    OPENBRACE,        // {
    CLOSEBRACE,       // }
    //OPENBRACKET,      // [
    //CLOSEBRACKET,     // ]
    //OPENCHEVRON,      // <
    //CLOSECHEVRON,     // >
    RETURN_KEYWORD,   // return
    NUMBER,           // [-Max, Max]
    SEMICOLON,        // ;
    END_OF_FILE       // End of file
} token_type;

typedef struct TOKEN_STRUCT
{
    int  Id;
    token_type TokenType;
    char Lexeme[32];
    struct TOKEN_STRUCT *NextToken;
} token;

token *AllocateNewToken()
{
    token *Token = (token *)calloc(1, sizeof(struct TOKEN_STRUCT));
    
    Token->Id         = 0;
    Token->TokenType  = 0;
    //Token->Lexeme     = (void*)0;
    Token->NextToken  = (void*)0;
    
    return Token;
}

typedef enum
{
    AST_NOP,
    AST_PROGRAM,
    AST_FUNCTION_DEFINITION,
    AST_STATEMENT,
    AST_EXPRESSION
} ast_type;

typedef struct AST_STRUCT
{
    ast_type ASTType;
    struct AST_STRUCT *Child;
} ast;

ast *AllocateNewAST(ast_type ASTType)
{
    ast *AST = (ast *)calloc(1, sizeof(struct AST_STRUCT));

    AST->ASTType = ASTType;
    AST->Child   = (void*)0;

    return AST;
}

int IsDigit(char Character)
{
    return ('1' <= Character && Character <= '9') ? 1 : 0;
}

int IsLowerCase(char Character)
{
    return ('a' <= Character && Character <= 'z') ? 1 : 0;
}

int IsUpperCase(char Character)
{
    return ('A' <= Character && Character <= 'Z') ? 1 : 0;
}

int IsAlphabet(char Character)
{
    return (IsLowerCase(Character) || IsUpperCase(Character)) ? 1 : 0;
}

int IsLetter(char Character)
{
    return (IsAlphabet(Character) || IsDigit(Character)) ? 1 : 0;
}

int IsLetterOr_(char Character)
{
    return (IsLetter(Character) || '_' == Character) ? 1 : 0;
}

int StringComparison(char *FirstString, char* SecondString)
{
    int Index;
    for(Index = 0; SecondString[Index]; Index++)
    {
        if(FirstString[Index] != SecondString[Index])
        {
            return 0;
        }
    }
    /* 
    `for` block return 0 means not valid i.e. string didn't match
    `for` block no return means "i" and "in" are not valid but "int" and "inte" are valid
    so FirstString[Index] is used to eliminate "inte"
    */
    return (FirstString[Index]) ? 0 : 1;
}

int IsStringANumber(char *String)
{
    for(int LexemeIndex = 0; String[LexemeIndex]; LexemeIndex++)
    {
        if(!IsDigit(String[LexemeIndex]))
        {
            return 0;
        }
    }
    return 1;
}

char * TokenTypeToString(int Type)
{
    switch (Type)
    {
        case INVALIED         : return "INVALIED";         
        case IDENTIFIER       : return "IDENTIFIER";      
        case INT_KEYWORD          : return "INT_KEYWORD";         
        case OPENPARENTHESIS  : return "OPENPARENTHESIS"; 
        case CLOSEPARENTHESIS : return "CLOSEPARENTHESIS";
        case OPENBRACE        : return "OPENBRACE";       
        case CLOSEBRACE       : return "CLOSEBRACE";      
        //case OPENBRACKET      : return "OPENBRACKET";     
        //case CLOSEBRACKET     : return "CLOSEBRACKET";    
        //case OPENCHEVRON      : return "OPENCHEVRON";     
        //case CLOSECHEVRON     : return "CLOSECHEVRON";    
        case RETURN_KEYWORD           : return "RETURN_KEYWORD";          
        case NUMBER           : return "NUMBER";        
        case SEMICOLON        : return "SEMICOLON";      
        case END_OF_FILE        : return "END_OF_FILE";        
    }
}

char * ASTTypeToString(int Type)
{
    switch (Type)
    { 
        case AST_NOP                 : return "AST_NOP";
        case AST_PROGRAM             : return "AST_PROGRAM";
        case AST_FUNCTION_DEFINITION : return "AST_FUNCTION_DEFINITION";
        case AST_STATEMENT           : return "AST_STATEMENT";
        case AST_EXPRESSION          : return "AST_EXPRESSION";
    }
}

token *Lexer(char *SourceCode, int SourceCodeSize)
{
    char *SourceCodePointer = SourceCode;

    int IdCounter = 0;

    char PreviousCharacter = ' ';
    char CurrentCharacter;

    int LexemeIndex = 0;

    token *TokensHead = AllocateNewToken();
    token *TokenIndex = TokensHead;

    for (int Index = 0; Index < SourceCodeSize; Index++)
    {   
        CurrentCharacter = SourceCodePointer[Index];

        // Middle character
        if(IsLetterOr_(PreviousCharacter) && IsLetterOr_(CurrentCharacter))
        {
            LexemeIndex++;
            TokenIndex->Lexeme[LexemeIndex] = CurrentCharacter;
        }
        
        /*else if(PreviousCharacter == '=' && CurrentCharacter == '=')
        {
            LexemeIndex++;
            Tokens[TokenIndex].Lexeme[LexemeIndex] = CurrentCharacter;
        }*/

        // First character
        else if(!IsLetterOr_(PreviousCharacter) && IsLetterOr_(CurrentCharacter))
        {
            //printf("%c\n", CurrentCharacter);
            LexemeIndex = 0;
            TokenIndex->NextToken = AllocateNewToken();
            TokenIndex->NextToken->Id = IdCounter++;
            TokenIndex->NextToken->TokenType = IDENTIFIER;
            TokenIndex->NextToken->Lexeme[LexemeIndex] = CurrentCharacter;
            TokenIndex = TokenIndex->NextToken;                
        }
        
        else if(CurrentCharacter != ' ' && CurrentCharacter != '\n' && CurrentCharacter != '\r' && CurrentCharacter != '\t')
        {
            LexemeIndex = 0;
            TokenIndex->NextToken = AllocateNewToken();
            TokenIndex->NextToken->Id = IdCounter++;
            TokenIndex->NextToken->Lexeme[LexemeIndex] = CurrentCharacter;

            if(CurrentCharacter == ';')
            {
                TokenIndex->NextToken->TokenType = SEMICOLON;
            }
            else if(CurrentCharacter == '(')
            {
                TokenIndex->NextToken->TokenType = OPENPARENTHESIS;
            }
            else if(CurrentCharacter == ')')
            {
                TokenIndex->NextToken->TokenType = CLOSEPARENTHESIS;
            }
            else if(CurrentCharacter == '{')
            {
                TokenIndex->NextToken->TokenType = OPENBRACE;
            }
            else if(CurrentCharacter == '}')
            {
                TokenIndex->NextToken->TokenType = CLOSEBRACE;
            }

            TokenIndex = TokenIndex->NextToken;
            //printf("%c %d\n", CurrentCharacter, TokenIndex);
        }

        PreviousCharacter = CurrentCharacter;
    }
    token *Temp = AllocateNewToken();
    Temp->Id = IdCounter++;
    Temp->TokenType = END_OF_FILE;
    TokenIndex->NextToken = Temp;
    TokenIndex = TokenIndex->NextToken;
    // Tokens[TokenIndex].Lexeme       // No Lexeme for EOF(End of File)

    char *Int    = "int";
    char *Return = "return";

    //char Keyworlds[][6] = { "int", "return" };

    for(token *TokenIndex = TokensHead->NextToken; TokenIndex->TokenType != END_OF_FILE; TokenIndex = TokenIndex->NextToken)
    {
        if(TokenIndex->TokenType == IDENTIFIER)
        {
            char *CurrentLexeme = TokenIndex->Lexeme;
            if(StringComparison(CurrentLexeme, Int))
            {
                TokenIndex->TokenType = INT_KEYWORD;
            }
            else if(StringComparison(CurrentLexeme, Return))
            {
                TokenIndex->TokenType = RETURN_KEYWORD;
            }
            else if(IsStringANumber(CurrentLexeme))
            {
                TokenIndex->TokenType = NUMBER;
            }
        }
    }

    /*for(token *TokenIndex = TokensHead->NextToken; TokenIndex->TokenType != END_OF_FILE; TokenIndex = TokenIndex->NextToken)
    {
        char *TypeString = TokenTypeToString(TokenIndex->TokenType);
        printf("[Lexer]:   Id=>`%d`,   type=>%16s,   value=> `%s`\n", 
               TokenIndex->Id, TypeString, TokenIndex->Lexeme);
    }*/

    return TokensHead;
}

void Parser_AdvanceAndVerifyToken(token **Token, token_type TokenType)
{
    *Token = (*Token)->NextToken;
    if((*Token)->TokenType != TokenType)
    {
        printf("[Parser]: Expected token type: %s and it is: %s\n", 
               TokenTypeToString(TokenType), TokenTypeToString((*Token)->TokenType));
        exit(-1);
    }
}

ast *Parser_ParseExpression(token **Tokens)
{
    Parser_AdvanceAndVerifyToken(Tokens, NUMBER);

    ast *AST = AllocateNewAST(AST_EXPRESSION);

    return AST;
}

ast *Parser_ParseStatement(token **Tokens)
{
    Parser_AdvanceAndVerifyToken(Tokens, RETURN_KEYWORD);

    ast *AST = AllocateNewAST(AST_STATEMENT);
    AST->Child = Parser_ParseExpression(Tokens);

    Parser_AdvanceAndVerifyToken(Tokens, SEMICOLON);

    return AST;
}

ast *Parser_ParseFunction(token **Tokens)
{
    Parser_AdvanceAndVerifyToken(Tokens, INT_KEYWORD);
    Parser_AdvanceAndVerifyToken(Tokens, IDENTIFIER);
    Parser_AdvanceAndVerifyToken(Tokens, OPENPARENTHESIS);
    Parser_AdvanceAndVerifyToken(Tokens, CLOSEPARENTHESIS);
    Parser_AdvanceAndVerifyToken(Tokens, OPENBRACE);

    ast *AST = AllocateNewAST(AST_FUNCTION_DEFINITION);
    AST->Child = Parser_ParseStatement(Tokens);
    
    Parser_AdvanceAndVerifyToken(Tokens, CLOSEBRACE);

    return AST;
}

ast *Parser(token *Tokens)
{
    ast *ASTRoot = AllocateNewAST(AST_PROGRAM);
    ASTRoot->Child = Parser_ParseFunction(&Tokens);

    return ASTRoot;
}

int main(int ArgumentCount, char** ArgumentValues) 
{
    if(ArgumentCount > 2)
    {
        printf("Invalid Argument Count i.e. %d\n", ArgumentCount);
        return 0;
    }

    char* SourceFileName = ArgumentValues[1];

    FILE * SourceFile = fopen(SourceFileName, "r");

    fseek(SourceFile, 0L, SEEK_END);
    int SourceFileSize = ftell(SourceFile);
    fseek(SourceFile, 0L, SEEK_SET);

    char* SourceCode = (char*)calloc(SourceFileSize, sizeof(char));

    fread(SourceCode, sizeof(char), SourceFileSize, SourceFile);
    
    fclose(SourceFile);

    // --------------------------- Lexer ---------------------------
    token *TokensHead;
    TokensHead = Lexer(SourceCode, SourceFileSize);
#if 0//DEBUG
    for(token *TokenIndex = TokensHead->NextToken; TokenIndex->TokenType != END_OF_FILE; TokenIndex = TokenIndex->NextToken)
    {
        char *TypeString = TokenTypeToString(TokenIndex->TokenType);
        printf("[Lexer]:   Id=>`%d`,   type=>%16s,   value=> `%s`\n", 
               TokenIndex->Id, TypeString, TokenIndex->Lexeme);
    }
#endif
    // --------------------------- Lexer ---------------------------

    // --------------------------- Parser ---------------------------
    ast *ASTRoot;
    ASTRoot = Parser(TokensHead);
#if 1//DEBUG
    for(ast *ASTIndex = ASTRoot; ASTIndex; ASTIndex = ASTIndex->Child)
    {
        char *TypeString = ASTTypeToString(ASTIndex->ASTType);
        printf("[Parser]: %s\n", TypeString);
    }
#endif
    // --------------------------- Parser ---------------------------

    free(SourceCode);

    return 0;
}
