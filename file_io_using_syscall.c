/*
 * In this program, handled arbitrary input data via STDIN,
 * and copied them into file "ip_data_copy".
 *
 * If the input is only text then read them via STDIN otherwise get the
 * file path of binary, image, video, or music file and copy into ip_data_copy.
 *
 * Before run the program make sure, login as super user so that
 * avoid any error on create and setting file permission
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>

/*
 * Desc:
 *  This function copy all other format file like mp3, exe, mp4, jpg, etc
 *  and write into file "ip_data_copy"
 *
 * Arguments:
 *  reader - File descriptor of input file to be copy
 *  writer - File descriptor of new file to copied data into it
 *  file_size - size of input file
 */
void copy_input_file(int reader, int writer, size_t file_size)
{
    size_t bytesread = 0;
    char *buffer = NULL;
    size_t alloc_size = 0;

    buffer = (char *)malloc(file_size);
    alloc_size = malloc_usable_size((void *)buffer);
    if ((buffer == NULL) || (alloc_size < file_size)) {
        printf("\n Failed: memory required %ld but"
                " allocated  %ld\n", file_size, alloc_size);
        if (alloc_size < file_size) {
            free(buffer);
        }
        return;
    }

    while((bytesread = read(reader, buffer, file_size)) < 0 && (errno == EINTR))
        ;//Retry in case of CPU cycle not available
    while((write(writer, buffer, bytesread)) < 0 && (errno == EINTR))
        ;//Retry in case of CPU cycle not available
    free(buffer);
}

/*
 * Desc:
 *  This function copy data from STDIN and write into file "ip_data_copy"
 *
 * Arguments:
 *  reader - File descriptor of input file to be copy
 *  writer - File descriptor of new file to copied data into it
 */
void copy_stdin(int reader, int writer)
{
    size_t bytesread = 0;
    char buffer[1];
    while ((bytesread = read(reader, buffer, 1)) != 0) {
        if (buffer[0] == EOF) {
            break;
        }
        write(writer, buffer, bytesread);
    }
}

int main()
{
    char input;
    char buffer[1];
    char input_file[512];
    int writer, reader=0;
    struct stat stat_rd, stat_wr;

    // Overwrite if file exist
    writer = open("ip_data_copy", O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (writer == -1) {
        printf("\n ERROR: File 'ip_data_copy' open has failed to "
                "copy input data, errno %d \n", errno);
        perror("open");
        return 0;
    }

    printf("\n Is input data binary, image, video, or music file(y/N):");
    scanf("%c", &input);
    if (input == 'y') {
        printf("\n Enter file path (path length should not > 512):");
        scanf("%s", input_file);
        reader = open(input_file, O_RDONLY);
        if (reader == -1) {
            printf("\n ERROR: Given input file '%s' is not found, "
                    "errno %d \n", input_file, errno);
            perror("open");
            // Close already opened descriptor
            close(writer);
            return 0;
        }
        stat(input_file, &stat_rd);
        copy_input_file(reader, writer, stat_rd.st_size);
    } else {
        printf("\n Enter data (If input over, press ctr+d or "
               "(ctr+z if your system is windows)):\n");
        copy_stdin(reader, writer);
    }
    close(reader);
    close(writer);

    // For UT verification show the copied file
    char command[128];
    if (reader != 0) {
        stat("ip_data_copy", &stat_wr);
        printf("\nSize of input file %s is %ld ", input_file, stat_rd.st_size);
        printf("\nFile copy was successful, with %ld byte copied\n", stat_wr.st_size);
        snprintf(command, sizeof(command), "ls -lh ip_data_copy %s", input_file);
    } else {
        printf("\n Copied file 'ip_data_copy' contents are:\n");
        snprintf(command, sizeof(command), "ls -l ip_data_copy \n\n cat ip_data_copy");
    }
    system(command);
    printf("\n");
    return 0;
}
