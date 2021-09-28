#include "onegin.h"

void Text_ctor(Text *this_, char* fileName) {
    FILE *inp = fopen(fileName, "r");
    if (!inp) {
        fprintf(stderr, "Failed to open the file.\n");
        exit(0);
    }

    fseek(inp, 0, SEEK_END);
    this_->dataLen = ftell(inp);
    

    #ifndef NDEBUG
    if (this_->data != (char*)POINTER_POISON) {
        fprintf(stderr, "WARNING: Trying to allocate memory for in-use pointer.\n");
        assert(false);
    }
    #endif

    // this_->data = (char*)calloc(this_->dataLen + 1, sizeof(char));         
    this_->dataWrapper = (char*)calloc(this_->dataLen + 2, sizeof(char));         
    
    if (!this_->dataWrapper) { 
        fprintf(stderr, "Failed to allocate buffer memory.\n");
        assert(false);
    }

    this_->dataWrapper[0]                  = '\n';
    this_->dataWrapper[this_->dataLen + 1] = '\n';

    this_->data = this_->dataWrapper + 1;
   
    fseek(inp, 0, 0);
    fread(this_->data, sizeof(char), this_->dataLen, inp);
    this_->data[this_->dataLen] = 0;

    this_->indexLen = 0;
    for (size_t i = 0; i < this_->dataLen; ++i) {
        if (this_->data[i] == '\n') {
            ++this_->indexLen;
        }
    }

    if (this_->indexLen == 0) {   
        fprintf(stderr, "File is empty?\n");
        free(this_->dataWrapper);
        this_->data        = (char*)POINTER_POISON;
        this_->dataWrapper = (char*)POINTER_POISON;
        exit(0);
    }

    
    #ifndef NDEBUG
    if (this_->Index != (String*)POINTER_POISON) {
        fprintf(stderr, "WARNING: Trying to allocate memory for in-use pointer.\n");
        assert(false);
    }
    #endif

    this_->Index = (String*)calloc(this_->indexLen, sizeof(String));
    
    if (!this_->Index) { 
        fprintf(stderr, "Failed to allocate buffer memory.\n");
        assert(false);
    }

    #ifndef NDEBUG
    for (size_t i = 0; i < this_->indexLen; ++i) {
        this_->Index->beg = (char*)POINTER_POISON;
        this_->Index->len = -1;
    }
    #endif



    this_->Index[0].beg = this_->data;
    size_t indexCnt = 0;

    for (size_t i = 0; i < this_->dataLen - 1 && indexCnt < this_->indexLen - 1; ++i) {     //TODO check if dataLen can be 0
        if (this_->data[i] == '\n') {
            this_->Index[indexCnt].len = dist(this_->Index[indexCnt].beg, &this_->data[i]);
            ++indexCnt;
            this_->Index[indexCnt].beg = &this_->data[i + 1];
        }
    }
    this_->Index[indexCnt].len = dist(this_->Index[indexCnt].beg, this_->data + this_->dataLen - 1);
}

void Text_dumpIndex(Text *this_)         //TODO add cout/cerr pipes support
{
    printf("Index:\nindexLen = %zu\n\n", this_->indexLen);
    for (size_t i = 0; i < this_->indexLen; ++i) {
        char* iter = this_->Index[i].beg;
        while(*iter != '\n') {  
            printf("%c", *iter);
            ++iter;
        }
        printf("\n size = %zu\n", this_->Index[i].len);
    }
}

void Text_printIndex(Text *this_)         //TODO add cout/cerr pipes support
{
    for (size_t i = 0; i < this_->indexLen; ++i) {
        char* iter = this_->Index[i].beg;
        while(*iter != '\n') {  
            printf("%c", *iter);
            ++iter;
        }
        printf("\n");
    }
}

void Text_printBuf(Text *this_)
{
    char tmp = this_->data[this_->dataLen];
    this_->data[this_->dataLen] = '\0';

    printf("%s", this_->data);

    this_->data[this_->dataLen] = tmp;
}

void Text_dtor(Text *this_)
{
    #ifndef NDEBUG
    if (this_->data == (char*)POINTER_POISON || this_->Index == (String*)POINTER_POISON) {
        fprintf(stderr, "WARNING: Trying to destroy Text with poisoned pointer.\n");
        assert(false);
    }
    #endif

    free(this_->dataWrapper);
    free(this_->Index);

    this_->data        = (char*)POINTER_POISON;
    this_->dataWrapper = (char*)POINTER_POISON;
    this_->Index     = (String*)POINTER_POISON;
}

int continuoslyCompare(char* firstIter, char* secondIter, int direction) 
{
    assert(direction == 1 || direction == -1);

    while (*firstIter != '\n' && *secondIter != '\n') {
        if (!isalpha(*(firstIter))) 
            firstIter += direction;
        else if (!isalpha(*(secondIter)))
            secondIter += direction;
        else { 
            if (*(secondIter) < *(firstIter)) {
                #ifdef  VERBOSE_COMP
                fprintf(stderr, "First diff chars: %c < %c\n", *secondIter, *firstIter);
                #endif
                return 1;
            }
            if (*(secondIter) > *(firstIter)) { 
                #ifdef  VERBOSE_COMP
                fprintf(stderr, "First diff chars: %c > %c\n", *secondIter, *firstIter);
                #endif
                return -1;
            }
            firstIter += direction;
            secondIter += direction;
        }
    }
    while (!isalpha(*firstIter) && *firstIter != '\n')
        firstIter += direction;

    while (!isalpha(*secondIter) && *secondIter != '\n')
        secondIter += direction;

    if (*firstIter < *secondIter) {
        #ifdef  VERBOSE_COMP
        fprintf(stderr, "Same letters diff lens: %zu < %zu\n", first->len, second->len);
        #endif
        return -1;
    }
    else if (*firstIter > *secondIter) {
        #ifdef  VERBOSE_COMP
        fprintf(stderr, "Same letters diff lens: %zu %zu\n", first->len, second->len);
        #endif
        return 1;
    }
    
    #ifdef  VERBOSE_COMP
    fprintf(stderr, "Equal strings\n");
    #endif

    return 0;
}

int straightComp(const void *first_inp, const void *second_inp)         
{
    String* first = (String*)first_inp;
    String* second = (String*)second_inp;

    char* firstIter  = first->beg;
    char* secondIter = second->beg;
    
    assert(firstIter  != (char*)POINTER_POISON);
    assert(secondIter != (char*)POINTER_POISON);
    
    return continuoslyCompare(firstIter, secondIter, 1);
}


int reverseComp(const void *first_inp, const void *second_inp)         
{
    String* first = (String*)first_inp;
    String* second = (String*)second_inp;

    assert(first->beg  != (char*)POINTER_POISON);
    assert(second->beg != (char*)POINTER_POISON);

    char* firstIter  = first->beg + first->len - 1;
    char* secondIter = second->beg + second->len - 1;
    

    return continuoslyCompare(firstIter, secondIter, -1);
}
