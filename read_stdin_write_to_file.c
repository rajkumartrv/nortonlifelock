#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char mdata_file[1024];

void copy_text_n_binary_data(FILE *fp)
{
    char input;
    printf("\n....Enter text or binary input (press ctr+d if input over)...\n");
    while ((input = fgetc(stdin)) != EOF) {
        fputc(input, fp);
    }
}


off_t copy_image_video_data(FILE *fp)
{
    char *buffer = NULL;
    size_t bytesread = 0;
    struct stat stats;
    FILE *ifp = NULL;

    printf("\n Enter file path (path length should not > 1024) :");
    scanf("%s", mdata_file);
    ifp = fopen(mdata_file, "rb");
    if (ifp == NULL) {
        printf("\nERROR: File %s is not found, error %d \n", mdata_file, errno);
        return 0;
    }
       // Get file size to setup buffer
    stat(mdata_file, &stats);
    buffer = (char *)malloc(stats.st_size);
    while (bytesread = fread(buffer, 1, stats.st_size, ifp)) {
        // bytesread contains the number of bytes actually read
        if (bytesread == 0) {
          break;
        }
        fwrite(buffer, bytesread, 1, fp);
    }
    free(buffer);
    fclose(ifp);
    return stats.st_size;
}


unit_test_api(off_t fsize)
{   
    char command[100];
    // For UT verifcation show the copied file
    if (fsize) {
        printf("Given file sie %ld\n", fsize);
        sprintf(command, "ls -l ip_data_copy %s", mdata_file);
    } else {
        printf("\n Copied data file 'ip_data_copy' detail and contents are:\n");
        sprintf(command, "ls -l ip_data_copy \n\n cat ip_data_copy");
    }
    system(command);

}

int main()
{
    FILE *fp = NULL;
    char input;
    struct stat stats;
    off_t fsize;
    
    fp = fopen("ip_data_copy", "wb");
    if (fp == NULL) {
        printf("\nERROR: write input to file has failed, errno %d \n", errno);
        return -1; 
    }

    printf("\n Is input image or video file(y/N):");
    scanf("%c", &input);
    // Distinguish input to read the text, binary or multimedia data
    if (input == 'y') {
        fsize = copy_image_video_data(fp);
        fclose(fp);
        if (fsize) {
            unit_test_api(fsize);
        }
    } else {
        copy_text_n_binary_data(fp);
        fclose(fp);
        unit_test_api(0);
    }
    return(0);
}
