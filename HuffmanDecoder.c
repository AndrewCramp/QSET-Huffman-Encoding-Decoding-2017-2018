#include <stdio.h>
#include <string.h>
#define MAXFILESIZE 500
#define MAXSTRING 500

void readCodes(char codes[][10]);
void readTransmission(char transmission[]);
void decode(char codes[][10], char encoded_transmission[], char decoded_transmission[]);

int main(int argc, char **argv){
    char codes[36][10]; //Huffman codes calculated by the encoding algorithm
    char transmission[MAXSTRING]; //Transmission string received from the satellite
    char decoded_transmission[MAXSTRING]; //Decoded transmission
    readCodes(codes);
    readTransmission(transmission);
    decode(codes,transmission,decoded_transmission);
    printf("%s",decoded_transmission);
}

//*************************************************************
//
// Function: decode
// 
// \brief: Iterate through list of characters, and their know 
//         codes checking if they are part of the transmitted 
//         sequence. Once a match is found it is then stored in
//         the decoded_transmission string.
// 
// \Param: codes: List of charcters and their corresponding
//         character codes generated from the huffman algorithm
//          
//         encoded_transmission: encoded transmission from satellite
//                               1s correspond to dashes, and 0s
//                               correspond to dots.
//
//         decoded_transmission: string to store decoded sequence. 
//                        
//*************************************************************                  

void decode(char codes[][10], char encoded_transmission[], char decoded_transmission[]){
    int n = 0;
    int count = 0;
    int found = 0;
    while(n < strlen(encoded_transmission)){
        found = 0;
        for(int i = 0; i < 36;i++){
            //Isolate code from it's character value
            char code[10];
            char sequence[10];
            for(int j = 1; j < strlen(codes[i]); j++){
                code[j-1] = codes[i][j]; 
            }
            code[strlen(codes[i])-1] = '\0';
            //Isolate sequence from transmission to cross refrence
            for(int j = n; j < n + strlen(codes[i])-1; j++){
                sequence[j-n] = encoded_transmission[j]; 
            }
            sequence[strlen(codes[i])-1] = '\0';
            //If code matches sequence from transmission add corresponding charcter to decoded message
            if(!strcmp(sequence,code)){
                decoded_transmission[count] = codes[i][0];
                count++;
                found = 1;
                n += strlen(code);
            }
        }
        if(!found){
            decoded_transmission[count] = 63;
            count++;
            n++;
        }
    }
    decoded_transmission[count] = '\0';
}

//*************************************************************
//
// Function: readCodes
//
// \brief: Read characters from text file 
//         with their corresponding encodded strings.
//
// \Param: codes: double index character array that stores the 
//                characters, and their corresponding codes
//                after they have been read in from file.
//
//************************************************************

void readCodes(char codes[][10]){
    FILE * code_file;
    int line_count = 0;
    code_file = fopen("./codes.txt","r");
    if(code_file == NULL){
        printf("ERROR: File not found.");
    }
    while(line_count < MAXFILESIZE && !feof(code_file)){
        fscanf(code_file,"%s",codes[line_count]);
        line_count++;
    }
    fclose(code_file);
}

//*************************************************************
//
// Function: readTransmission
//
// \brief: Read string of 0s, and 1s from file corresponding
//         to the dots and dashs trasmitted from the satellite
//
// \Param: codes: Character array (String) containing the
//                transmitted message from the satellite 
//                once it has been read from the file. 
//
//************************************************************

void readTransmission(char transmission[]){
    FILE * transmission_file;
    transmission_file = fopen("./transmission.txt","r");
    if(transmission_file == NULL){
        printf("ERROR: File not found.");
    }
    fscanf(transmission_file,"%s",transmission);
    fclose(transmission_file);
}


