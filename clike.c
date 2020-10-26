#include <stdio.h>
#include <stdlib.h>

typedef struct token
{
    int  Id;
    char Lexeme[32];
} token;

int IsLetterOr_(char CurrentCharacter)
{
    if(
        ('a' <= CurrentCharacter && CurrentCharacter <= 'z') ||
        ('A' <= CurrentCharacter && CurrentCharacter <= 'Z') ||
        ('1' <= CurrentCharacter && CurrentCharacter <= '9') ||
        ('_' == CurrentCharacter)
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
        
        else if(PreviousCharacter == '=' && CurrentCharacter == '=')
        {
            LexemeIndex++;
            Tokens[TokenIndex].Lexeme[LexemeIndex] = CurrentCharacter;
        }

        // First character
        else if(!IsLetterOr_(PreviousCharacter) && IsLetterOr_(CurrentCharacter))
        {
            //printf("%c\n", CurrentCharacter);
            TokenIndex++;
            LexemeIndex = 0;
            Tokens[TokenIndex].Lexeme[LexemeIndex] = CurrentCharacter;

            Tokens[TokenIndex].Id = IdCounter++;
        }
        
        else if(CurrentCharacter != ' ' && CurrentCharacter != '\n' && CurrentCharacter != '\r')
        {
            TokenIndex++;
            LexemeIndex = 0;
            Tokens[TokenIndex].Lexeme[LexemeIndex] = CurrentCharacter;

            Tokens[TokenIndex].Id = IdCounter++;
            //printf("%c %d\n", CurrentCharacter, TokenIndex);
        }

        PreviousCharacter = CurrentCharacter; 
    }

    for(TokenIndex = 0; TokenIndex < 20; TokenIndex++)
    {
        printf("%2d - %s\n", Tokens[TokenIndex].Id, Tokens[TokenIndex].Lexeme);
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
