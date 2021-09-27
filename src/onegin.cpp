#include "onegin.h"


int main(int argc, char *argv[]) 
{
    char fileName[MAX_FILENAME_SIZE + 1];

    int option = 0; 
    while ((option = getopt(argc, argv, "f:lrxh")) != -1) {             //TODO 
        switch(option) {
            case 'h':
                printf("sort [-h; -f fileName]\n");
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

    Text_dumpIndex(&text);


    gqsort(text.Index, text.indexLen, sizeof(String), straightComp);
    Text_printIndex(&text);

    qsort(text.Index, text.indexLen, sizeof(String), reverseComp);
    Text_printIndex(&text);

    Text_printBuf(&text);

    Text_dtor(&text);
}
