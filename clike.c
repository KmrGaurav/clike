#include <stdio.h>
#include <stdlib.h>

typedef enum 
{
    INVALIED,
    IDENTIFIER,
    INTEGER,          // int
    OPENPARENTHESIS,  // (
    CLOSEPARENTHESIS, // )
    OPENBRACE,        // {
    CLOSEBRACE,       // }
    //OPENBRACKET,      // [
    //CLOSEBRACKET,     // ]
    //OPENCHEVRON,      // <
    //CLOSECHEVRON,     // >
    RETURN,           // return
    CONSTANT,         // [-Max, Max]
    SEMICOLON         // ;
} token_type;

typedef struct 
{
    int  Id;
    token_type TokenType;
    char Lexeme[32];
} token;

int IsNumber(char CurrentCharacter)
{
    if('1' <= CurrentCharacter && CurrentCharacter <= '9')
    {
        return 1;
    }
    return 0;
}

int IsLetterOr_(char CurrentCharacter)
{
    if(
        ('a' <= CurrentCharacter && CurrentCharacter <= 'z') ||
        ('A' <= CurrentCharacter && CurrentCharacter <= 'Z') ||
        IsNumber(CurrentCharacter) || 
        '_' == CurrentCharacter
      )
    {
        return 1;
    }
    return 0;
}

void Lexer(token* Tokens, char *SourceCode, int SourceCodeSize)
{
    char *SourceCodePointer = SourceCode;

    int Index;    
    int IdCounter = 0;

    char PreviousCharacter = ' ';
    char CurrentCharacter;

    int TokenIndex = -1;
    int LexemeIndex = 0;

    for (Index = 0; Index < SourceCodeSize; Index++)
    {   
        CurrentCharacter = SourceCodePointer[Index];

        // Middle character
        if(IsLetterOr_(PreviousCharacter) && IsLetterOr_(CurrentCharacter))
        {
            LexemeIndex++;
            Tokens[TokenIndex].Lexeme[LexemeIndex] = CurrentCharacter;
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
            TokenIndex++;
            LexemeIndex = 0;
            Tokens[TokenIndex].Lexeme[LexemeIndex] = CurrentCharacter;

            Tokens[TokenIndex].Id = IdCounter++;
            Tokens[TokenIndex].TokenType = IDENTIFIER;
        }
        
        else if(CurrentCharacter != ' ' && CurrentCharacter != '\n' && CurrentCharacter != '\r' && CurrentCharacter != '\t')
        {
            TokenIndex++;
            LexemeIndex = 0;
            Tokens[TokenIndex].Lexeme[LexemeIndex] = CurrentCharacter;

            Tokens[TokenIndex].Id = IdCounter++;

            if(CurrentCharacter == ';')
            {
                Tokens[TokenIndex].TokenType = SEMICOLON;
            }
            else if(CurrentCharacter == '(')
            {
                Tokens[TokenIndex].TokenType = OPENPARENTHESIS;
            }
            else if(CurrentCharacter == ')')
            {
                Tokens[TokenIndex].TokenType = CLOSEPARENTHESIS;
            }
            else if(CurrentCharacter == '{')
            {
                Tokens[TokenIndex].TokenType = OPENBRACE;
            }
            else if(CurrentCharacter == '}')
            {
                Tokens[TokenIndex].TokenType = CLOSEBRACE;
            }
            //printf("%c %d\n", CurrentCharacter, TokenIndex);
        }

        PreviousCharacter = CurrentCharacter; 
    }

    char *Int    = "int";
    char *Return = "return";

    char Keyworlds[][6] = { "int", "return" }; 

    for(TokenIndex = 0; TokenIndex < 20; TokenIndex++)
    {
        if(Tokens[TokenIndex].TokenType == IDENTIFIER)
        {
            int IsKeywordMinMatched = 1;
            for(LexemeIndex = 0; Int[LexemeIndex]; LexemeIndex++)
            {
                if(Tokens[TokenIndex].Lexeme[LexemeIndex] != Int[LexemeIndex])
                {
                    IsKeywordMinMatched = 0;
                }
            }
            /* 
            (IsKeywordMinMatched == 0) means not valid i.e. string didn't match
            (IsKeywordMinMatched == 1) means "i" is not valid but "int" and "inte" are valid
            so !Tokens[TokenIndex].Lexeme[LexemeIndex] is used to eliminate "inte"
            */
            if(IsKeywordMinMatched && !Tokens[TokenIndex].Lexeme[LexemeIndex])
            {
                Tokens[TokenIndex].TokenType = INTEGER;
                continue;
            }

            IsKeywordMinMatched = 1;
            for(LexemeIndex = 0; Return[LexemeIndex]; LexemeIndex++)
            {
                if(Tokens[TokenIndex].Lexeme[LexemeIndex] != Return[LexemeIndex])
                {
                    IsKeywordMinMatched = 0;
                }
            }
            if(IsKeywordMinMatched && !Tokens[TokenIndex].Lexeme[LexemeIndex])
            {
                Tokens[TokenIndex].TokenType = RETURN;
                continue;
            }

            // ----------------------------- Constant----------------------------------
            int IsCurrentCharacterANumber = 1;
            for(LexemeIndex = 0; Tokens[TokenIndex].Lexeme[LexemeIndex]; LexemeIndex++)
            {
                if(!IsNumber(Tokens[TokenIndex].Lexeme[LexemeIndex]))
                {
                    IsCurrentCharacterANumber = 0;
                }
                //printf("%s\n", Tokens[TokenIndex].Lexeme);
            }
            if(IsCurrentCharacterANumber)
            {
                Tokens[TokenIndex].TokenType = CONSTANT;
            }
        }
    }

    for(TokenIndex = 0; TokenIndex < 20; TokenIndex++)
    {
        printf("%2d - %2d - %s\n", 
               Tokens[TokenIndex].Id, Tokens[TokenIndex].TokenType, Tokens[TokenIndex].Lexeme);
    }
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

    // ---------------------------

    token Tokens[20] = {};
    Lexer(Tokens, SourceCode, SourceFileSize);

    // ---------------------------

    free(SourceCode);

    return 0;
}
