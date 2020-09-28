#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>


void copy_text_n_binary_data(FILE *fp)
{   
    char input;
    printf("\n....Enter text or binary input (press ctr+d if input over)...\n");
    while ((input = fgetc(stdin)) != EOF) {
        fputc(input, fp);
    }

}

void copy_image_video_data(FILE *fp)
{
    char *buffer = NULL;
    char mdata_file[1024];
    size_t bytesread = 0;
    struct stat stats;
    FILE *ifp = NULL; 

    printf("\n Please enter path of image or video file to copy:");
    scanf("%s", mdata_file);
    ifp = fopen(mdata_file, "rb");
    if (ifp == NULL) {
        printf("\n FIle %s is not found, error %d \n", mdata_file, errno);
        return;
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
    fclose(ifp);
    free(buffer);
}


int main()
{
    FILE *fp = NULL;
    char input;

    fp = fopen("input_data", "wb");
    if (fp == NULL) {
        printf("\n write input to file has failed, error %d \n", errno);
        return -1;
    }
    
    printf("\n Is input image or video file(y/N):");
    scanf("%c", &input);
    // Distinguish input to read the text, binary or multimedia data 
    if (input == 'y') {
        copy_image_video_data(fp);
    } else {
        copy_text_n_binary_data(fp);
    }
    fclose(fp);
    
    return(0);
}
