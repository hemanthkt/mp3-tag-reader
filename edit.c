#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "edit.h"

void editFile(char *argv[])
{
    FILE *fp_src = fopen(argv[4], "r");
    FILE *fp_dest = fopen("test.mp3", "w");

    if (fp_src == NULL)
    {
        printf("Couldnt open source file %s\n", argv[4]);
        return;
    }

    if (fp_dest == NULL)
    {
        printf("Couldnt create destination file\n");
        fclose(fp_src);
        return;
    }

    char header[10];
    fread(header, 1, 10, fp_src);
    fwrite(header, 1, 10, fp_dest);

    char target_tag[5];

    if (strcmp(argv[2], "-t") == 0)
        strcpy(target_tag, "TIT2");
    else if (strcmp(argv[2], "-a") == 0)
        strcpy(target_tag, "TPE1");
    else if (strcmp(argv[2], "-A") == 0)
        strcpy(target_tag, "TALB");
    else if (strcmp(argv[2], "-y") == 0)
        strcpy(target_tag, "TYER");
    else if (strcmp(argv[2], "-m") == 0)
        strcpy(target_tag, "TCON");
    else if (strcmp(argv[2], "-c") == 0)
        strcpy(target_tag, "COMM");

    for (int i = 0; i < 6; i++)
    {
        char tag[5];
        tag[4] = '\0';

        char size_bytes[4];
        char flag[2];

        fread(tag, 1, 4, fp_src);
        fread(size_bytes, 1, 4, fp_src);
        fread(flag, 1, 2, fp_src);

        int size = (size_bytes[0] << 24) |
                   (size_bytes[1] << 16) |
                   (size_bytes[2] << 8) |
                   (size_bytes[3]);

        if (strcmp(tag, target_tag) == 0)
        {
            // WRITE NEW FRAME
            fwrite(tag, 1, 4, fp_dest);

            int new_size = strlen(argv[3]) + 1;
            unsigned char new_size_bytes[4];

            new_size_bytes[0] = (new_size >> 24) & 0xFF;
            new_size_bytes[1] = (new_size >> 16) & 0xFF;
            new_size_bytes[2] = (new_size >> 8) & 0xFF;
            new_size_bytes[3] = new_size & 0xFF;

            fwrite(new_size_bytes, 1, 4, fp_dest);
            fwrite(flag, 1, 2, fp_dest);

            fseek(fp_src, size, SEEK_CUR); // skip old data

            char encoding = 0;
            fwrite(&encoding, 1, 1, fp_dest);
            fwrite(argv[3], 1, strlen(argv[3]), fp_dest);
        }
        else
        {
            fwrite(tag, 1, 4, fp_dest);
            fwrite(size_bytes, 1, 4, fp_dest);
            fwrite(flag, 1, 2, fp_dest);

            char *data = malloc(size);
            fread(data, 1, size, fp_src);
            fwrite(data, 1, size, fp_dest);
            free(data);
        }
    }

    // copy remaining audio
    char buffer[1024];
    int bytes;
    while ((bytes = fread(buffer, 1, 1024, fp_src)) > 0)
        fwrite(buffer, 1, bytes, fp_dest);

    fclose(fp_src);
    fclose(fp_dest);

    printf("File edited successfully. Output saved to test.mp3\n");
}