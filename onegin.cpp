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

    Text text;
    Text_ctor(&text, fileName);

    printf("Here!\n");
    Text_dumpIndex(&text);


    qsort(text.Index, text.indexLen, sizeof(char*), straightComp);
    
    Text_dumpIndex(&text);
}
