#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "view.h"
#include "edit.h"

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
    else if (strcmp(argv[1], "--help"))
    {
        return help;
    }
    else
    {
        return unsupported;
    }
}

int main(int argc, char *argv[])
{
    const char *strline = "----------------------------------------------------";

    if (argc < 3)
    {
        printf("Usage:\n");
        printf("  View: %s -v <mp3file>\n", argv[0]);
        printf("  Edit: %s -e <tag> <new_value> <mp3file>\n", argv[0]);
        printf("  Tags: -t (title), -a (artist), -A (album), -y (year), -m (music), -c (composer)\n");
        return 1;
    }

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
        if (argc < 5)
        {
            printf("Error: Edit mode requires 4 arguments\n");
            printf("Usage: %s -e <tag> <new_value> <mp3file>\n", argv[0]);
            return 1;
        }
        editFile(argv);
    }
    else if (res == help)
    {
        printf("1. -v -> to view mp3 file contents\n");
        printf("2. -e -> to edit mp3 file contents\n");

        printf("   2.1. -t -> to edit song title\n");
        printf("   2.2. -a -> to edit artist name\n");
        printf("   2.3. -A -> to edit album name\n");
        printf("   2.4. -y -> to edit year\n");
        printf("   2.5. -m -> to edit content\n");
        printf("   2.6. -c -> to edit comment\n");
    }
    else
    {
        printf("Error: Unsupported operation\n");
        printf("Use -v for view or -e for edit\n");
        return 1;
    }

    return 0;
}