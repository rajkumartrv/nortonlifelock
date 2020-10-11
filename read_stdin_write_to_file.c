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

void unit_test_api(off_t fsize)
{
    char command[100];
    // For UT verification show the copied file
    if (fsize) {
        printf("Given file size is %ld and find copied file detail below \n", fsize);
        snprintf(command, sizeof(command), "ls -l ip_data_copy %s", mdata_file);
    } else {
        printf("\n Copied file 'ip_data_copy' contents are:\n");
        snprintf(command, sizeof(command), "ls -l ip_data_copy \n\n cat ip_data_copy");
    }
    system(command);
}

int main()
{
    char input;
    char buffer[1];
    size_t bytesread = 0;
    struct stat stats;
    FILE *fp = NULL;
    FILE *ifp = stdin;

    // Overwrite if file exist
    fp = fopen("ip_data_copy", "wb");
    if (fp == NULL) {
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
        ifp = fopen(mdata_file, "rb");
        if (ifp == NULL) {
            printf("\n ERROR: Given input file '%s' is not found, errno %d \n",
                    mdata_file, errno);
            perror("fopen");
            // Close already opened descriptor
            fclose(fp);
            return 0;
        }
    } else {
        printf("\n Enter data (press ctr+d if input over):\n");
    }

    while (bytesread = fread(buffer, 1, 1, ifp)) {
        // To check EOF as it contains the number of bytes read
        if (bytesread == 0) {
          break;
        }
        fwrite(buffer, bytesread, 1, fp);
    }
    fclose(fp);
    fclose(ifp);

    //UT
    stat(mdata_file, &stats);
    unit_test_api(stats.st_size);
    return 0;
}
