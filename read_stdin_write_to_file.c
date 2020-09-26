#include <stdio.h>
#include <errno.h>

int main() {
    char input;
    FILE *fp = NULL;

    printf("\n....Enter input ...\n");
    fp = fopen("input_data.txt", "w");
    if (fp == NULL) {
        printf("\n write input to file has failed, error %d \n", errno);
        return -1;
    }

    while ((input = fgetc(stdin)) != EOF) {
        fputc(input, fp);
    }
    fclose(fp);

    return(0);
}
