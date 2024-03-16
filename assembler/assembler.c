/*

Simple One Step Assembler for Generating Machine Code for 3 Instructions
MOVLW, MOVF, MOVWF, ADDLW, ADDWF, SUBLW, and SUBWF

Developed by Dali Fethi Abdellatif
Date: 16/03/2024

Instructions for Use:
1. Create a text file containing the assembly instructions and save it.
2. Open an empty file.
3. Provide the paths for both created files when prompted.
4. Review the generated machine code for correctness.

The assembler follows these steps:
1. Retrieve one line from the input file.
2. Parse the instruction and its operand.
3. Check if the instruction is written correctly.
4. If correct, generate the corresponding machine code.
5. If incorrect, increment the error count and saves the syntax error.
6. Repeat the process for each line in the input file.

Note: 
If there are no syntax errors, the machine code is generated. 
Otherwise, the number of errors encountered during text analysis is provided.



*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>

#define max 200
#define number 5
#define length 10
#define size 5
#define offset 0x02

struct counting     // USED TO COUNT THE NUMBER OF ERRORS
{
    int counter;
    int k;
}errors;
struct begin        // USED TO SAVE THE ADDRESSES
{
    char operand[length];
}valer;
struct err
{
    char string[length];
}*val;

void message(void);
void val_allocation(void);
void function(char *charac, char *operands, char *paths);    // function for storging the machinde code of instruction
char* getOperand(char *inst);                      //function to retrieve the operand form the instruction
char* getInstruction(char *inst);                   // function to get the instruction syntax
void get_path(char path[max]);                  // function to get the path of our asm source file
bool test_file(char path[max]);                 // function to test if the file exist
bool read_file(char path[max], char *paths);                 // reading from the file
char converting(char *instr, char *operand, char *paths);    //converting the instruction by opcode
void function_1(char *poit, char *paths);                    // function for placing the address of each instruction 
void check(char *pointer);                      // function to replace the 'x' by '0'
void check_errors(char *path);                  // function to check if errors occured during the process
bool check_machin_file(char *path);             // function to test if the machine file exists or not
void change(char *operand);                     // function to shift the number from right to left

/*main function*/
int main(void)
{
    val_allocation();
    char path[max];
    char path_mac[max];
    int i = 0;
    while(true)
    {
        printf("machine code file path : ");
        fgets(path_mac, max, stdin);
        i = 0;
        while(path_mac[i] != '\0')
        {
            if(path_mac[i] == '\n')
                path_mac[i] = '\0';
            i+=1;
        }
        bool state = check_machin_file(path_mac);
        if(state == true)
            break;
    }
    while(true)
    {
        get_path(path);
        bool state = test_file(path);
        if(state == true)
            break;
    }
    bool state = read_file(path, path_mac);
    if(state == false)
        check_errors(path_mac);
}
/*function to get the path of the asm source file*/
void get_path(char path[max])
{
    printf("enter the file path : ");
    fgets(path, max, stdin);
    int i = 0;
    while(path[i] != '\0')
    {
        if(path[i] == '\n')
            path[i] = '\0';
        i+=1;
    }
    i = 0;
}
/*function test if the asm source file exists or not*/
bool test_file(char path[max])
{
    FILE *pointer;
    pointer = fopen(path, "r");
    if(pointer != NULL)
    {
        fclose(pointer);
        return true;
    }
    else
    {
        fclose(pointer);
        printf("the file does not exists\n");   
        return false;
    }
}
/*function to test if the mnemonic entered by user is supported or not */
bool instr_test(char *inst)
{
    if(strcmp(inst, "MOVLW") == 0)
        return true;
    else if(strcmp(inst, "MOVWF") == 0)
        return true;
    else if(strcmp(inst, "ORG") == 0)
        return true;
    else if(strcmp(inst, "END") == 0)
        return true;
    else if(strcmp(inst, "MOVF") == 0)
        return true;
    else if(strcmp(inst, "ADDLW") == 0)
        return true;
    else if(strcmp(inst, "ADDWF") == 0)
        return true;
    else if(strcmp(inst, "SUBLW") == 0)
        return true;
    else if(strcmp(inst, "SUBWF") == 0)
        return true;
    else
        return false;
}
/*function to read each line from file*/
bool read_file(char *path, char *paths)
{
    errors.k = 0;
    static int si = 1;
    FILE *pointer;
    static bool state = false;
    char *instr = NULL;
    char *oper = NULL;
    pointer = fopen(path, "r");
    char ligne[max];
    int i = 0;
    while(fgets(ligne, max, pointer))
    {
        i = 0;
        if(strcmp(ligne, "\n") == 0)            // SKIP EMPTY LINE FROM THE FILE 
            continue;   
        while(ligne[i] != '\0')
        {
            if(ligne[i] == '\n')
                ligne[i] = '\0';
            i+=1;
        }
        if(strcmp(ligne, "END") == 0)
        {
            --si;
            val = realloc(val, sizeof(struct err)*si);
            break;
        }
        instr = getInstruction(ligne);
        state = instr_test(instr);
        if(state == false)
        {
            strcpy((val+errors.k)->string, instr);
            ++(errors.k);
            ++si;
            val = realloc(val, sizeof(struct err)*si);
            ++(errors.counter);
        }
        else
        {
            oper = getOperand(ligne);
            converting(instr, oper, paths);
            state = false;
        }
    }
    free(instr);
    free(oper);
    return false;
}
/*function to get the operand form each line*/
char* getOperand(char *inst)
{
    int i = 0, j = 0;
    char *string;
    while(true)
    {
        string = calloc(size, sizeof(char));
        if(string != NULL)
            break;
        else
            message();
    }
    while(i<strlen(inst))
    {
        if(inst[i] == ' ')
        {
            ++i;
            while(i<strlen(inst))
            {
                *(string+j) = inst[i];
                ++i;
                ++j;
            }
            break;
        }
        i+=1;
    }
    return string;
}
/*function to get the mnemonic from each  line*/
char* getInstruction(char *inst)
{
    int i = 0;
    char *string;
    while(true)
    {
        string = calloc(length, sizeof(char));
        if(string != NULL)
            break;
        else
            message();
    }
    while(inst[i] != ' ')
    {
        *(string+i) = inst[i];
        if(i>7)
        {
            i = 0;
            while(inst[i] != '\0')
            {
                *(string+i) = inst[i];
                i+=1;
            }
            break;
        }
        i+=1;
    }
    i = 0;
    return string;
}
/*function to convert instruction to its opcode and also increment the address by two*/
char converting(char *instr, char *operand, char *paths)   
{
    check(operand);
    static bool state = false;
    if(state == true)
    {
        if(strcmp(instr, "MOVLW") == 0) 
            function("7E", operand, paths);
        else if(strcmp(instr, "MOVWF") == 0)
            function("F1", operand, paths);
        else if(strcmp(instr, "MOVF") == 0)  
            function("EB", operand, paths);
        else if(strcmp(instr, "ADDLW") == 0)  
            function("5E", operand, paths);
        else if(strcmp(instr, "ADDWF") == 0)  
            function("E3", operand, paths);
        else if(strcmp(instr, "SUBLW") == 0)  
            function("3A", operand, paths);
        else if(strcmp(instr, "SUBWF") == 0)  
            function("9B", operand, paths);
        for(int i = 0; i<strlen(valer.operand); ++i)
        {
            if(i == 3)
                valer.operand[i] += offset;
        }
        function_1(valer.operand, paths);
    }
    else
    {
        if(strcmp(instr, "ORG") == 0)
        {
            strcpy(valer.operand, operand);
            function_1(valer.operand, paths);   
            state = true;
        }
    }
}

void function(char *charac, char *operands, char *paths)    // function to put the machine code into file
{
    change(operands);
    FILE *pointer;
    pointer = fopen(paths, "a");
    if(pointer != NULL)
    {
        fputs(charac, pointer);
        fputs(operands, pointer);
        fputs("\n", pointer);
        fclose(pointer);
    }
    else
        printf("file did not opened in mode apend\n");
}

void function_1(char *poit, char *paths)        // function to put adress of each instruction into file
{
    FILE *pointer;
    pointer = fopen(paths, "a");
    if(pointer != NULL)
    {
        fputs(poit, pointer);
        fputs("\t\t", pointer);
        fclose(pointer);
    }
    else
        printf("the file did not opened\n");
}

void check(char *pointer)       // function to replace the 'x' by '0'
{
    for(int i = 0; i<strlen(pointer); ++i)
    {
        if(*(pointer+i) == 'x')
            *(pointer+i) = '0';
    }
}

void check_errors(char *path)       // function to check if there are any syntax error and display the errors
{
    if(errors.counter != 0)
    {
        FILE *pointer;
        pointer = fopen(path, "w");
        if(pointer != NULL)
            fclose(pointer);
        printf("number of errors = %d\n", errors.counter);
        printf("syntax errors occured in :\n");
        for(int i = 0; i<errors.k; ++i)
            printf("\t%s\n", ((val+i)->string));
    }
    free(val);
}
bool check_machin_file(char *path)  // function to check if the machine file exists or not
{
    FILE *pointer;
    pointer = fopen(path, "r");
    if(pointer != NULL)
    {
        fclose(pointer);
        return true;
    }
    else
    {
        printf("file does not exist\n");
        return false;
    }
}
void val_allocation(void)   // allocate memory for the structure pointer (val)
{
    while(true)
    {
        val = calloc(1, sizeof(struct err));
        if(val != NULL)
            break;
        else
            message();
    } 
}
void change(char *operand)  // function to shift the number from right to left
{
    int j = 0;
    char ch = 0;
    while(j<2)
    {
        for(int i = 0; i<strlen(operand); ++i)
        {
            ch = operand[i];
            operand[i] = operand[i+1];
            operand[i+1] = ch;
        }
        j+=1;
    }
}
void message(void)  //function to display if any memory fails to be allocated
{
    printf("fail to allocate memory\n");
}