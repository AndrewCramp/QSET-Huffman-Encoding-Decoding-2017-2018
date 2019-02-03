#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h" //Include header containing node structure for nodes
#define MAXTREESIZE 71
#define DELTA 2
#define ASCIIA 65
#define ASCIIZ 90
#define ASCII0 48
#define ASCII9 57
#define ASCIISPACE 32 

void readFile(char messages[], int maxSize);
float probability(char totalString[],char letter);
void sortNodes(struct node nodes[], int size);
void genTree(struct node nodes[],int n);
void minProbability(struct node nodes[],int min[2],int n);
int searchTree(char code[][10], struct node currentNode, char target, int count);

int main(int argc, char **argv){
    struct node nodes[MAXTREESIZE]; //Create array of nodes;
    char messages[500]; //String for holding all messages
    char codes[36][10];
    for(int i = 0; i < 500;i++){//Initialize array with NULL values
        messages[i] = NULL;   
    }
    for(int i = 0; i < 36;i++){//Initialize array with NULL values
        for(int j = 0; j < 10;j++){
            codes[i][j] = NULL;   
        }
    }
    readFile(messages,500);
    for(int i = ASCIIA; i <= 90;i++){//Determine probability for each letter and store in base nodes
        nodes[i-ASCIIA].characterValue = i;
        nodes[i-ASCIIA].value = probability(messages,i); 
        nodes[i-ASCIIA].hasParent = 0;
        nodes[i-ASCIIA].number = i-ASCIIA;
        nodes[i-ASCIIA].leftChild = NULL; //Initialize node's children as NULL
        nodes[i-ASCIIA].rightChild = NULL;
    }
    for(int i = ASCII0; i <= 57;i++){//Determine probability for each letter and store in base nodes
        nodes[i-ASCII0+26].characterValue = i;
        nodes[i-ASCII0+26].value = DELTA/36.0/10.0;
        nodes[i-ASCII0+26].hasParent = 0;
        nodes[i-ASCII0+26].number = i-ASCII0+26;
        nodes[i-ASCII0+26].leftChild = NULL; //Initialize node's children as NULL
        nodes[i-ASCII0+26].rightChild = NULL;
    }
    sortNodes(nodes,36); //Sorts nodes in descending order of probability
    for(int i = 0; i < 36; i++){
        printf("Probablity of %c: %f\n",nodes[i].characterValue,nodes[i].value);
    }
    genTree(nodes,36); //Generate bianary Huffman tree
    for(int i = ASCIIA; i <= ASCIIZ; i++){
        searchTree(codes,nodes[MAXTREESIZE-1],i,0); //Search tree for a letter, and store code 
        printf("%c: %s\n",i,codes[i-ASCIIA]);
    }
    for(int i = ASCII0; i <= ASCII9; i++){
        searchTree(codes,nodes[MAXTREESIZE-1],i,0); //Search tree for a letter, and store code 
        printf("%c: %s\n",i,codes[i-ASCII0+26]);
    }
    outputCodes(codes);
    return 0;
}

void readFile(char messages[], int maxSize){ 
        int n = 0;
        char tempMessage[maxSize]; //temp string
        for(int i = 0; i < maxSize;i++){//Initialize array with NULL values
            tempMessage[i] = NULL;   
        }
        FILE *infile;
        infile = fopen("./testfile.txt","r");
        if(infile == NULL){ //If file not found exit program
            printf("File not Found");
            exit(1);
        }
        while(n < maxSize && !feof(infile)){
            fgets(tempMessage,maxSize,infile);
            if(tempMessage[strlen(tempMessage)-1] == '\n') //Remove new line character
                tempMessage[strlen(tempMessage)-1] = '\0'; 
            strcat(messages,tempMessage); //Append next line from file to messages String
            n++;
        }
        fclose(infile);
}

void outputCodes(char codes[][10]){
    FILE *fileOut;
    fileOut = fopen("../codes.txt","w");
    for(int i = 0; i < 36; i++){
        if(i < 26){
            fprintf(fileOut,"%c %s\n",i+ASCIIA,codes[i]);
        }else{
            fprintf(fileOut,"%c %s\n",i-26+ASCII0,codes[i]);
        }
    }
    
}

float probability(char totalString[],char letter){
    int count = 0; //number of instances letter appears in the totalString array
    for(int i = 0; i < strlen(totalString);i++){ //Search arrray for instances of letter
        if(totalString[i] == letter)
            count++;
    }
    return (float)count/(strlen(totalString)+DELTA); //Calculate probability
}

void sortNodes(struct node nodes[], int size){ //Sort structures in descending order of probability using bubblesort
    for(int i = 0; i < size;i++){
        for(int k = 0; k < size-1; k++){
            if(nodes[k].value < nodes[k+1].value){
                struct node temp;
                temp = nodes[k];
                nodes[k] = nodes[k+1];
                nodes[k+1] = temp;
            }
        }
    } 
}

void genTree(struct node nodes[],int n){ //n = number of base nodesl
        int min[2] = {0,0};
        while(n < MAXTREESIZE){ 
            minProbability(nodes,min,n); //find two unused nodes with lowest probability
            n++;
            nodes[n-1].hasParent = 0; //new node has not been integrated into the tree yet
            nodes[n-1].characterValue = NULL; //Internal nodes do not contain character values
            nodes[n-1].number = n-1;
            nodes[n-1].leftChild = &nodes[min[0]]; //link left child to new parent node
            nodes[n-1].rightChild = &nodes[min[1]]; //link right child to new parent node
            nodes[n-1].value = nodes[min[0]].value + nodes[min[1]].value; //probability value of new node is equal to the sum of it's childrens probabilities
            printf("parent probability \t lchild probability \t rchild probability\n");
            printf("%d %f \t\t %c %d %f \t\t %c %d %f\n\n",n-1, nodes[n-1].value,nodes[min[0]].characterValue,nodes[min[0]].number,nodes[min[0]].value,nodes[min[1]].characterValue,nodes[min[1]].number,nodes[min[1]].value);
        }
}

void minProbability(struct node nodes[],int min[2], int n){ // after excecution min[] will hold the indicies of the lowest two unused nodes
        min[0] = 0;
        min[1] = 0;
        int count = 0;
        struct node nodesSort[MAXTREESIZE]; //Temporary node array
        for(int i = 0; i < n;i++){
            if(!nodes[i].hasParent){ //add nodes to temporary array that do not have a parent(have not already been integrated in array)
                nodesSort[count] = nodes[i];
                count++;
            }
        }
        sortNodes(nodesSort,count); //Sort temporary array
        for(int j = 0; j< 2;j++){ //nodes in last two indicies have the lowest probability
            for(int i = 0; i < n;i++){
                if(nodes[i].number == nodesSort[count-1-j].number){
                    min[j] = i;
                    break;
                }
            }   
        }
        nodes[min[0]].hasParent =1; //Indicates this nodes have been placed in the tree
        nodes[min[1]].hasParent =1; 
}

int searchTree(char code[][10], struct node currentNode, char target, int count){ //Search tree for target value and place message in encoded message array
    if(!currentNode.leftChild){ //Is the current node terminal
        if(currentNode.characterValue == target){ //Return true if character node contains target
            return 1;
        }else{
            return 0;
        }
    }else{
        if(searchTree(code, *currentNode.rightChild, target,count+1)){ //If value has been found message at count+1 is 0
            if(ASCIIA <= target && target <= ASCIIZ){
                code[target-ASCIIA][count] = '0';
            }else if(ASCII0 <= target && target <= ASCII9){
                code[target-ASCII0+26][count] = '0';
            }
            return 1;
        }else if(searchTree(code, *currentNode.leftChild, target,count+1)){ //If value has been found message at count+1 is 1
            if(ASCIIA <= target && target <= ASCIIZ){
                code[target-ASCIIA][count] = '1';
            }else if(ASCII0 <= target && target <= ASCII9){
                code[target-ASCII0+26][count] = '1';
            }
            return 1;
        }else{
            return 0;
        }
    }
}



