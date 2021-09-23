#include "onegin.h"


static const size_t MAX_FILENAME_SIZE = 64;

static void printIndex(String* Index, size_t indexSize)
{
    for (size_t i = 0; i < indexSize; ++i) {
        char* iter = Index[i].beg;
        while(*iter != '\n') {
            printf("%c", *iter);
            ++iter;
        }
        printf("\n size = %zu\n", Index[i].len);
    }
}

int main(int argc, char *argv[]) 
{
    char fileName[MAX_FILENAME_SIZE + 1];

    int option = 0; 
    while ((option = getopt(argc, argv, "f:lrxh")) != -1) {             //TODO 
        switch(option) {
            case 'i':           //TODO
            case 'l':
            case 'r':
                printf("Options %c\n", option);
                break;

            case 'h':
                printf("This is (a very usefull) help message!\n");
                break;

            case 'f':
                printf("File %s\n", optarg);

                if (strlen(optarg) > MAX_FILENAME_SIZE) {
                    printf("Filename is too long, please try again.\n");
                    return 0;
                }

                strncpy(fileName, optarg, MAX_FILENAME_SIZE);
                break;

            case '?':
                printf("Unknow option, try `-h`\n");
                break;
        }
    }

    
    FILE *inp = fopen(fileName, "r");
    if (!inp) {
        printf("Failed to open the file.\n");
        return 0;
    }

    fseek(inp, 0, SEEK_END);
    size_t bufLen = ftell(inp);
    char *buffer = (char*)calloc(bufLen + 1, sizeof(char));         //TODO free buffer properly
    fseek(inp, 0, 0);
    fread(buffer, sizeof(char), bufLen, inp);
    buffer[bufLen] = 0;

    // printf("%zu\n", wcslen(buffer));
    // printf(" %s \n==========\n", buffer);
    // wprintf(L"%ls \n", L"A wide string");

    size_t indexSize = 0;
    for (size_t i = 0; i < bufLen; ++i) {
        if (buffer[i] == L'\n') {
            ++indexSize;
        }
    }
    printf("indexSize = %zu\n", indexSize);
    if (indexSize == 0) {   //TODO 
        printf("File is empty?\n");
        free(buffer);
        return 0;
    }

    String* Index = (String*)calloc(indexSize, sizeof(String));        //TODO free index properly

    Index[0].beg = buffer;
    size_t indexCnt = 0;

    for (size_t i = 0; i < bufLen && indexCnt < indexSize - 1; ++i) {
        if (buffer[i] == '\n') {
            printf("indexCnt = %zu\n", indexCnt);
            Index[indexCnt].len = dist(Index[indexCnt].beg, &buffer[i]);// / sizeof(char); //TODO find why is this constant 
            ++indexCnt;
            Index[indexCnt].beg = &buffer[i + 1];
        }
    }
    Index[indexCnt].len = dist(Index[indexCnt].beg, buffer + bufLen - 1);

    printf("Here!\n");
    printIndex(Index, indexSize);


    qsort(Index, indexSize, sizeof(char*), straightComp);
    
    printIndex(Index, indexSize);

    free(Index);
    free(buffer);
}
