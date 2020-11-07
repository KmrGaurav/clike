#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    LEX_INVALIED,
    LEX_IDENTIFIER,
    LEX_INT_KEYWORD,        // int
    LEX_OPENPARENTHESIS,    // (
    LEX_CLOSEPARENTHESIS,   // )
    LEX_OPENBRACE,          // {
    LEX_CLOSEBRACE,         // }
    //LEX_OPENBRACKET,        // [
    //LEX_CLOSEBRACKET,       // ]
    //LEX_OPENCHEVRON,        // <
    //LEX_CLOSECHEVRON,       // >
    LEX_RETURN_KEYWORD,     // return
    LEX_NUMBER,             // [-Max, Max]
    LEX_NEGATION,           // -
    LEX_BITWISE_COMPLEMENT, // ~
    LEX_LOGICAL_NEGATION,   // !
    LEX_SEMICOLON,          // ;
    LEX_END_OF_FILE         // End of file
} token_type;

typedef struct TOKEN_STRUCT
{
    int  Id;
    int LineNumber;
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
    AST_NOP_TYPE,
    AST_PROGRAM_TYPE,
    AST_FUNCTION_DEFINITION_TYPE,
    AST_STATEMENT_TYPE,
    AST_EXPRESSION_TYPE,
    AST_NUMBER_TYPE,
    AST_NEGATION_TYPE,           // -
    AST_BITWISE_COMPLEMENT_TYPE, // ~
    AST_LOGICAL_NEGATION_TYPE    // !
} ast_type;

typedef struct AST_FUNCTION_DEFINITION_STRUCT
{
    char *FunctionName;
} ast_function_definition_struct;

typedef struct AST_NUMBER_STRUCT
{
    int Number;
} ast_number_struct;

typedef struct AST_STRUCT
{
    ast_type ASTType;
    union
    {
        ast_function_definition_struct ASTFuntionDefinitionStruct; 
        ast_number_struct ASTNumberStruct;
    };
    struct AST_STRUCT *Child;
} ast;

ast *AllocateNewAST(ast_type ASTType)
{
    ast *AST = (ast *)calloc(1, sizeof(struct AST_STRUCT));

    AST->ASTType = ASTType;
    if(AST->ASTType == AST_FUNCTION_DEFINITION_TYPE)
    {
        //AST->ASTFuntionDefinitionStruct.FunctionName = "hello"; 
    }

    AST->Child   = (void*)0;

    return AST;
}

int IsDigit(char Character)
{
    return ('0' <= Character && Character <= '9') ? 1 : 0;
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

int StringToNumber(int *Number, char *String)
{
    *Number = 0;
    int IntMax32 = 0x7fffffff; // 2147483647 4294967295

    for(int Index = 0; String[Index]; Index++)
    {
        char CurrentCharacter = String[Index];
        int CurrentDigit = CurrentCharacter - '0';
        if(*Number*10 > IntMax32 - CurrentDigit)
        {
            return 0;
        }
        *Number = *Number*10 + CurrentDigit;
    }

    return 1;
}

void StringConcatenate(char *String1, char *String2)
{
    strcat(String1, String2);
}

char * TokenTypeToString(int Type)
{
    switch (Type)
    {
        case LEX_INVALIED            : return "LEX_INVALIED";         
        case LEX_IDENTIFIER          : return "LEX_IDENTIFIER";      
        case LEX_INT_KEYWORD         : return "LEX_INT_KEYWORD";         
        case LEX_OPENPARENTHESIS     : return "LEX_OPENPARENTHESIS"; 
        case LEX_CLOSEPARENTHESIS    : return "LEX_CLOSEPARENTHESIS";
        case LEX_OPENBRACE           : return "LEX_OPENBRACE";       
        case LEX_CLOSEBRACE          : return "LEX_CLOSEBRACE";      
        //case OPENBRACKET             : return "OPENBRACKET";     
        //case CLOSEBRACKET            : return "CLOSEBRACKET";    
        //case OPENCHEVRON             : return "OPENCHEVRON";     
        //case CLOSECHEVRON            : return "CLOSECHEVRON";    
        case LEX_RETURN_KEYWORD      : return "LEX_RETURN_KEYWORD";          
        case LEX_NUMBER              : return "LEX_NUMBER";     
        case LEX_NEGATION            : return "LEX_NEGATION";
        case LEX_BITWISE_COMPLEMENT  : return "LEX_BITWISE_COMPLEMENT";
        case LEX_LOGICAL_NEGATION    : return "LEX_LOGICAL_NEGATION";   
        case LEX_SEMICOLON           : return "LEX_SEMICOLON";      
        case LEX_END_OF_FILE         : return "LEX_END_OF_FILE"; 
#if 1//DEBUG
        default: printf("Lexer Type not defined.\n");exit(-1);
#endif       
    }
}

char * ASTTypeToString(int Type)
{
    switch (Type)
    { 
        case AST_NOP_TYPE                 : return "AST_NOP_TYPE";
        case AST_PROGRAM_TYPE             : return "AST_PROGRAM_TYPE";
        case AST_FUNCTION_DEFINITION_TYPE : return "AST_FUNCTION_DEFINITION_TYPE";
        case AST_STATEMENT_TYPE           : return "AST_STATEMENT_TYPE";
        case AST_EXPRESSION_TYPE          : return "AST_EXPRESSION";
        case AST_NUMBER_TYPE              : return "AST_NUMBER";
        case AST_NEGATION_TYPE            : return "AST_NEGATION_TYPE";
        case AST_BITWISE_COMPLEMENT_TYPE  : return "AST_BITWISE_COMPLEMENT_TYPE";
        case AST_LOGICAL_NEGATION_TYPE    : return "AST_LOGICAL_NEGATION_TYPE";
#if 1//DEBUG
        default: printf("AST Type not defined.\n");exit(-1);
#endif
    }
}

token *Lexer(char *SourceCode, int SourceCodeSize)
{
    char *SourceCodePointer = SourceCode;

    int IdCounter = 0;
    int SourceCodeLineNumber = 1;

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
            TokenIndex->NextToken->LineNumber = SourceCodeLineNumber;
            TokenIndex->NextToken->TokenType = LEX_IDENTIFIER;
            TokenIndex->NextToken->Lexeme[LexemeIndex] = CurrentCharacter;
            TokenIndex = TokenIndex->NextToken;                
        }
        else if(CurrentCharacter == '\n')
        {
            SourceCodeLineNumber++;
        }
        else if(CurrentCharacter != ' ' && CurrentCharacter != '\r' && CurrentCharacter != '\t')
        {
            LexemeIndex = 0;
            TokenIndex->NextToken = AllocateNewToken();
            TokenIndex->NextToken->Id = IdCounter++;
            TokenIndex->NextToken->LineNumber = SourceCodeLineNumber;
            TokenIndex->NextToken->Lexeme[LexemeIndex] = CurrentCharacter;

            if(CurrentCharacter == ';')
            {
                TokenIndex->NextToken->TokenType = LEX_SEMICOLON;
            }
            else if(CurrentCharacter == '(')
            {
                TokenIndex->NextToken->TokenType = LEX_OPENPARENTHESIS;
            }
            else if(CurrentCharacter == ')')
            {
                TokenIndex->NextToken->TokenType = LEX_CLOSEPARENTHESIS;
            }
            else if(CurrentCharacter == '{')
            {
                TokenIndex->NextToken->TokenType = LEX_OPENBRACE;
            }
            else if(CurrentCharacter == '}')
            {
                TokenIndex->NextToken->TokenType = LEX_CLOSEBRACE;
            }
            else if(CurrentCharacter == '-')
            {
                TokenIndex->NextToken->TokenType = LEX_NEGATION;
            }
            else if(CurrentCharacter == '~')
            {
                TokenIndex->NextToken->TokenType = LEX_BITWISE_COMPLEMENT;
            }
            else if(CurrentCharacter == '!')
            {
                TokenIndex->NextToken->TokenType = LEX_LOGICAL_NEGATION;
            }

            TokenIndex = TokenIndex->NextToken;
            //printf("%c %d\n", CurrentCharacter, TokenIndex);
        }

        PreviousCharacter = CurrentCharacter;
    }
    token *Temp = AllocateNewToken();
    Temp->Id = IdCounter++;
    Temp->TokenType = LEX_END_OF_FILE;
    TokenIndex->NextToken = Temp;
    TokenIndex = TokenIndex->NextToken;
    // Tokens[TokenIndex].Lexeme       // No Lexeme for EOF(End of File)

    char *Int    = "int";
    char *Return = "return";

    //char Keyworlds[][6] = { "int", "return" };

    for(token *TokenIndex = TokensHead->NextToken; TokenIndex->TokenType != LEX_END_OF_FILE; TokenIndex = TokenIndex->NextToken)
    {
        if(TokenIndex->TokenType == LEX_IDENTIFIER)
        {
            char *CurrentLexeme = TokenIndex->Lexeme;
            if(StringComparison(CurrentLexeme, Int))
            {
                TokenIndex->TokenType = LEX_INT_KEYWORD;
            }
            else if(StringComparison(CurrentLexeme, Return))
            {
                TokenIndex->TokenType = LEX_RETURN_KEYWORD;
            }
            else if(IsStringANumber(CurrentLexeme))
            {
                TokenIndex->TokenType = LEX_NUMBER;
            }
        }
    }

    /*for(token *TokenIndex = TokensHead->NextToken; TokenIndex->TokenType != LEX_END_OF_FILE; TokenIndex = TokenIndex->NextToken)
    {
        char *TypeString = TokenTypeToString(TokenIndex->TokenType);
        printf("[Lexer]:   Id=>`%d`,   type=>%16s,   value=> `%s`\n", 
               TokenIndex->Id, TypeString, TokenIndex->Lexeme);
    }*/

    return TokensHead;
}

int IsItAUnaryToken(token_type TokenType)
{
    if(
        TokenType == LEX_NEGATION || 
        TokenType == LEX_BITWISE_COMPLEMENT || 
        TokenType == LEX_LOGICAL_NEGATION)
    {
        return 1;
    }
    return 0;
}

void Parser_AdvanceToken(token **Token)
{
    *Token = (*Token)->NextToken;
}

int Parser_VerifyToken(token **Token, token_type TokenType)
{
    if((*Token)->TokenType != TokenType)
    {
        printf("[Parser]: In file %s at line:%d error at `%s`\n          Expected token type: %s and it is: %s i.e. %s\n",
                __FILE__, (*Token)->LineNumber, (*Token)->Lexeme,
                TokenTypeToString(TokenType), TokenTypeToString((*Token)->TokenType), (*Token)->Lexeme);
        exit(-1);
    }
    return 1;
}

int Parser_AdvanceAndVerifyToken(token **Token, token_type TokenType)
{
    Parser_AdvanceToken(Token);
    return Parser_VerifyToken(Token, TokenType);
}

ast *Parser_ParseNumber(token **Tokens)
{
    int Number;
    if(!StringToNumber(&Number, (*Tokens)->Lexeme))
    {
        printf("[Parser]: In file %s at line:%d error at `%s`\n          Bit overflow: Entered number has exceeded the max value of int.\n",
                __FILE__, (*Tokens)->LineNumber, (*Tokens)->Lexeme);
        exit(-1);
    }

    ast *AST = AllocateNewAST(AST_NUMBER_TYPE);
    AST->ASTNumberStruct.Number = Number;
    return AST;
}

ast *Parser_ParseUnaryOperator(token_type TokenType)
{
    ast *AST;
    if(TokenType == LEX_NEGATION) // -
    {
        AST = AllocateNewAST(AST_NEGATION_TYPE);
    }
    else if(TokenType == LEX_BITWISE_COMPLEMENT) // ~
    {
        AST = AllocateNewAST(AST_BITWISE_COMPLEMENT_TYPE);
    }
    else if(TokenType == LEX_LOGICAL_NEGATION) // !
    {
        AST = AllocateNewAST(AST_LOGICAL_NEGATION_TYPE);
    }
    return AST;  
}

ast *Parser_ParseExpression(token **Tokens)
{
    Parser_AdvanceToken(Tokens);
    token_type TokenType = (*Tokens)->TokenType;
    if(TokenType == LEX_NUMBER)
    {
        Parser_VerifyToken(Tokens, LEX_NUMBER);
        return Parser_ParseNumber(Tokens);
    }
    else if(IsItAUnaryToken(TokenType))
    {
        token_type NextTokenType = (*Tokens)->NextToken->TokenType;
        if(NextTokenType != LEX_NUMBER && !IsItAUnaryToken(NextTokenType))
        {
            
        }
        ast* UnarAST = Parser_ParseUnaryOperator(TokenType);
        UnarAST->Child = Parser_ParseExpression(Tokens);
        return UnarAST;
    }
    else
    {
        printf("[Parser_ParseExpression]: Undefined token type\n");
    }

}

ast *Parser_ParseStatement(token **Tokens)
{
    Parser_AdvanceAndVerifyToken(Tokens, LEX_RETURN_KEYWORD);

    ast *AST = AllocateNewAST(AST_STATEMENT_TYPE);
    ast *ASTE = AllocateNewAST(AST_EXPRESSION_TYPE);
    ASTE = Parser_ParseExpression(Tokens);
    AST->Child = ASTE;

    Parser_AdvanceAndVerifyToken(Tokens, LEX_SEMICOLON);

    return AST;
}

ast *Parser_ParseFunction(token **Tokens)
{
    ast *AST = AllocateNewAST(AST_FUNCTION_DEFINITION_TYPE);

    Parser_AdvanceAndVerifyToken(Tokens, LEX_INT_KEYWORD);
    Parser_AdvanceAndVerifyToken(Tokens, LEX_IDENTIFIER);
    AST->ASTFuntionDefinitionStruct.FunctionName = (*Tokens)->Lexeme;
    Parser_AdvanceAndVerifyToken(Tokens, LEX_OPENPARENTHESIS);
    Parser_AdvanceAndVerifyToken(Tokens, LEX_CLOSEPARENTHESIS);
    Parser_AdvanceAndVerifyToken(Tokens, LEX_OPENBRACE);

    AST->Child = Parser_ParseStatement(Tokens);
    
    Parser_AdvanceAndVerifyToken(Tokens, LEX_CLOSEBRACE);

    return AST;
}

ast *Parser(token *Tokens)
{
    ast *ASTRoot = AllocateNewAST(AST_PROGRAM_TYPE);
    ASTRoot->Child = Parser_ParseFunction(&Tokens);

    return ASTRoot;
}

void PrettyPrintAST(ast *AST, int Indent)
{
    if(AST)
    {
        for(int IndentIndex = 0; IndentIndex < (Indent-1)*4; IndentIndex++)
        {
            printf(" ");
        }
        if(Indent) printf("\u2514\u2500\u2500\u2500");
        if(AST->ASTType == AST_FUNCTION_DEFINITION_TYPE)
        {
            printf("fun: %s\n", AST->ASTFuntionDefinitionStruct.FunctionName);
        }
        else if(AST->ASTType == AST_NUMBER_TYPE)
        {
            printf("num: %d\n", AST->ASTNumberStruct.Number);
        }
        else if(AST->ASTType == AST_NEGATION_TYPE)
        {
            printf("negation: -\n");
        }
        else if(AST->ASTType == AST_BITWISE_COMPLEMENT_TYPE)
        {
            printf("bit comp: ~\n");
        }
        else if(AST->ASTType == AST_LOGICAL_NEGATION_TYPE)
        {
            printf("logical neg: !\n");
        }
        else 
        {
            char *TypeString = ASTTypeToString(AST->ASTType);
            printf("%s\n", TypeString);
        }
        PrettyPrintAST(AST->Child, Indent + 1);
    }

    return;
}

char *Generator(ast *AST)
{
    char *AssemblyCode = (char*)calloc(0, sizeof(char));
    
    if(AST)
    {
        char *FurtherAssemblyCode = (char*)calloc(25, sizeof(char));
        if(AST->ASTType == AST_NUMBER_TYPE)
        {
            sprintf(FurtherAssemblyCode, "    mov $%d, %%eax\n    ret\n", AST->ASTNumberStruct.Number);
            StringConcatenate(AssemblyCode, FurtherAssemblyCode);
        }
        else if(AST->ASTType == AST_EXPRESSION_TYPE)
        {

        }
        else if(AST->ASTType == AST_FUNCTION_DEFINITION_TYPE)
        {
            sprintf(FurtherAssemblyCode, "%s:\n", AST->ASTFuntionDefinitionStruct.FunctionName);
            StringConcatenate(AssemblyCode, FurtherAssemblyCode);
        }
        else if(AST->ASTType == AST_PROGRAM_TYPE)
        {
            sprintf(FurtherAssemblyCode, "    .globl main\n");
            StringConcatenate(AssemblyCode, FurtherAssemblyCode);
        }
        FurtherAssemblyCode = Generator(AST->Child);
        StringConcatenate(AssemblyCode, FurtherAssemblyCode);
        free(FurtherAssemblyCode);
    }

    return AssemblyCode;
}

int main(int ArgumentCount, char** ArgumentValues) 
{
    if(ArgumentCount != 3)
    {
        printf("Invalid Argument Count i.e. %d\n", ArgumentCount);
        return 0;
    }

    char* SourceFileName = ArgumentValues[1];
    FILE * SourceCodeFile = fopen(SourceFileName, "r");

    fseek(SourceCodeFile, 0L, SEEK_END);
    int SourceFileSize = ftell(SourceCodeFile);
    fseek(SourceCodeFile, 0L, SEEK_SET);

    char* SourceCode = (char*)calloc(SourceFileSize, sizeof(char));

    fread(SourceCode, sizeof(char), SourceFileSize, SourceCodeFile);
    
    fclose(SourceCodeFile);

    // --------------------------- Lexer ---------------------------
    token *TokensHead;
    TokensHead = Lexer(SourceCode, SourceFileSize);
#if 1//DEBUG
    for(token *TokenIndex = TokensHead->NextToken; TokenIndex->TokenType != LEX_END_OF_FILE; TokenIndex = TokenIndex->NextToken)
    {
        char *TypeString = TokenTypeToString(TokenIndex->TokenType);
        printf("[Lexer]:   Id: %2d,   Line: %d,   type:%22s,   value: `%s`\n", 
               TokenIndex->Id, TokenIndex->LineNumber, TypeString, TokenIndex->Lexeme);
    }
#endif
    // --------------------------- Lexer ---------------------------
    free(SourceCode);
    // --------------------------- Parser ---------------------------
    ast *ASTRoot;
    ASTRoot = Parser(TokensHead);
#if 1//DEBUG
    printf("[Parser]:\n");
    PrettyPrintAST(ASTRoot, 0);
#endif
    // --------------------------- Parser ---------------------------

    // --------------------------- Code Generator ---------------------------
    char *GeneratedAssemblyCode = Generator(ASTRoot);
#if 0//DEBUG
    printf("[Code Generator]:\n");
    printf("%s", GeneratedAssemblyCode);
#endif
    // --------------------------- Code Generator ---------------------------

    char* AssemblyFileName = ArgumentValues[2];
    FILE *AssemblyCodeFile = fopen(AssemblyFileName, "w");

    fprintf(AssemblyCodeFile, "%s", GeneratedAssemblyCode);

    fclose(AssemblyCodeFile);

    return 0;
}
