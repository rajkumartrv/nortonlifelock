/*
 * In this program, handled arbitrary input data via STDIN,
 *  and copied them into file "ip_data_copy".
 *
 * If the input is only text then read them via STDIN otherwise get the file path of
 *  binary, image, video, or music file and copy into ip_data_copy.
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

char mdata_file[512];

int main()
{
    char input;
    char buffer[1];
    register size_t bytesread = 0;
    struct stat stats;
    FILE *writer = NULL;
    FILE *reader = stdin;

    // Overwrite if file exist
    writer = fopen("ip_data_copy", "wb");
    if (writer == NULL) {
        printf("\n ERROR: File 'ip_data_copy' open has failed to copy input data,"
               " errno %d \n", errno);
        perror("fopen");
        return 0;
    }

    printf("\n Is input data binary, image, video, or music file(y/N):");
    scanf("%c", &input);
    if (input == 'y') {
        // Get file path of input data
        printf("\n Enter file path (path length should not > 512):");
        scanf("%s", mdata_file);
        reader = fopen(mdata_file, "rb");
        if (reader == NULL) {
            printf("\n ERROR: Given input file '%s' is not found, errno %d \n",
                    mdata_file, errno);
            perror("fopen");
            // Close already opened descriptor
            fclose(writer);
            return 0;
        }
    } else {
        printf("\n Enter data (If input over, press ctr+d or "
                "(ctr+z if your system is windows)):\n");
    }

    while ((bytesread = fread(buffer, 1, 1, reader)) != 0) {
        fwrite(buffer, bytesread, 1, writer);
    }
    fclose(writer);
    fclose(reader);

    // For UT verification show the copied file
    stat(mdata_file, &stats);
    char command[128];
    if (stats.st_size) {
        printf("Given file size is %ld and find copied file details below \n", stats.st_size);
        snprintf(command, sizeof(command), "ls -l ip_data_copy %s", mdata_file);
    } else {
        printf("\n Copied file 'ip_data_copy' contents are:\n");
        snprintf(command, sizeof(command), "ls -l ip_data_copy \n\n cat ip_data_copy");
    }
    system(command);
    return 0;
}
