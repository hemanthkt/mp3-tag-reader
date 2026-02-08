#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "view.h"

static void printDetails(char *tag_buffer, char *tag_data)
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

    fclose(fp);
}