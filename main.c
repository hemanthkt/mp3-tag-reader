#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-v") == 0)
    {
        return view;
    }
    else if (strcmp(argv[1], "-e") == 0)
    {
        return edit;
    }
    else
    {
        return unsupported;
    }
}

void printDetails(char *tag_buffer, char *tag_data)
{
    if (strcmp("TPE1", tag_buffer) == 0)
    {
        printf("ARTIST: %s\n", tag_data);
    }

    if (strcmp("TIT2", tag_buffer) == 0)
    {
        printf("TITLE: %s\n", tag_data);
    }

    if (strcmp("TALB", tag_buffer) == 0)
    {
        printf("ALBUM: %s\n", tag_data);
    }

    if (strcmp("TYER", tag_buffer) == 0)
    {
        printf("YEAR: %s\n", tag_data);
    }

    if (strcmp("TCON", tag_buffer) == 0)
    {
        printf("MUSIC: %s\n", tag_data);
    }

    if (strcmp("COMM", tag_buffer) == 0)
    {
        printf("COMPOSER: %s\n", tag_data);
    }
}

void viewFile(char filename[])
{
    const char *strline = "----------------------------------------------------";
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Couldnt open %s\n", filename);
        return;
    }
    fseek(fp, 10, SEEK_SET);
    int i = 0;
    printf("\n---------------------SELECTED VIEW DETAILS---------------------\n");
    while (i < 6)
    {
        // store tag
        char tag_buffer[4];
        // to store size
        int size;
        char size_bytes[4];
        fread(tag_buffer, sizeof(char), 4, fp);

        fread(size_bytes, 1, 4, fp);
        size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8) | (size_bytes[3]);
        size = size - 1;

        char flag[3];
        fread(flag, 1, 3, fp);
        char *tag_data = malloc(size);
        fread(tag_data, sizeof(char), size, fp);
        tag_data[size] = '\0';
        printDetails(tag_buffer, tag_data);
        free(tag_data);
        i++;
    }
    printf("\n%s\n", strline);
    printf("\n---------------------DETAILS DISPLAYED---------------------\n");
}

void editFile(char *argv[])
{
    FILE *fp_src = fopen(argv[4], "r");
    FILE *fp_dest = fopen("test.mp3", "w");

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
}

int main(int argc, char *argv[])
{
    const char *strline = "----------------------------------------------------";
    OperationType res = check_operation_type(argv);
    if (res == view)
    {
        printf("\n%s\n", strline);
        printf("MP3 TAG READER AND EDITOR FOR IDV3\n");
        printf("\n%s\n", strline);
        viewFile(argv[2]);
    }
    else if (res == edit)
    {
        editFile(argv);
    }
}
