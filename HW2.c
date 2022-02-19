#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void reddir(char *, int tabs);
void Size(char *, int tabs);
void Size_filter(char *temp, int flags, int tabs);
void type_filter(char *temp, int flags, int tabs);
void type_filter_D(char *temp, int tabs);
void type_filter_R(char *temp, int tabs);

char *filetype(unsigned char type)
{
    char *str;
    switch (type)
    {
    case DT_BLK:
        str = "block device";
        break;
    case DT_CHR:
        str = "character device";
        break;
    case DT_DIR:
        str = "directory";
        break;
    case DT_FIFO:
        str = "named pipe (FIFO)";
        break;
    case DT_LNK:
        str = "symbolic link";
        break;
    case DT_REG:
        str = "regular file";
        break;
    case DT_SOCK:
        str = "UNIX domain socket";
        break;
    case DT_UNKNOWN:
        str = "unknown file type";
        break;
    default:
        str = "UNKNOWN";
    }
    return str;
}
int main(int argc, char **argv)
{
    struct dirent *dirent;
    DIR *parentDir;
    int count = 1;
    int c;
    if (argc < 2)
    {
        printf("Usage: %s <dirname>\n", argv[0]);
        exit(-1);
    }
    parentDir = opendir(argv[1]);
    if (parentDir == NULL)
    {
        printf("Error opening directory '%s'\n", argv[1]);
        exit(-1);
    }
    else
    {
        int svalue = 0;
        int fvalue = 0;
        while ((c = getopt(argc, argv, "Ss:f:t")) != -1)
        {

            switch (c)
            {
            case 'S':
                if (argc < 2)
                {
                    printf("Usage: %s <dirname>\n");
                    exit(-1);
                }
                parentDir = opendir(argv[1]);
                if (parentDir == NULL)
                {
                    printf("Error opening directory '%s'\n", argv[1]);
                    exit(-1);
                }
                else
                {
                    Size(argv[1], 0);
                }
                break;

            case 's':
                svalue = atoi(optarg);
                if (argc < 2)
                {
                    printf("Usage: %s <dirname>\n");
                    exit(-1);
                }
                parentDir = opendir(argv[1]);
                if (parentDir == NULL)
                {
                    printf("Error opening directory '%s'\n", argv[1]);
                    exit(-1);
                }

                else
                {
                    Size_filter(argv[1], svalue, 0);
                }
                break;

            case 'f':
                if (argc < 2)
                {
                    printf("Usage: %s <dirname>\n");
                    exit(-1);
                }
                parentDir = opendir(argv[1]);
                if (parentDir == NULL)
                {
                    printf("Error opening directory '%s'\n", argv[1]);
                    exit(-1);
                }

                else
                {
                    type_filter(argv[1], fvalue, 0);
                }

                break;
            case 't':
                if (argc < 2)
                {
                    printf("Usage: %s <dirname>\n");
                    exit(-1);
                }
                parentDir = opendir(argv[1]);
                if (parentDir == NULL)
                {
                    printf("Error opening directory '%s'\n", argv[1]);
                    exit(-1);
                }

                else
                {
                    if (strcmp(optarg, argv[1]) == 0)
                    {
                        type_filter_D(argv[1], 0);
                    }
                    else
                    {
                        type_filter_R(argv[1], 0);
                    }
                }
                break;
            }
        }
        reddir(argv[1], 0);
    }
}
void reddir(char *temp, int tabs)
{
    char functemp[100];
    struct dirent *dirent2;
    DIR *parentDir2 = opendir(temp);
    int count = 1;

    while ((dirent2 = readdir(parentDir2)) != NULL)
    {
        if (strcmp(dirent2->d_name, ".") != 0 && strcmp(dirent2->d_name, "..") != 0)
        {
            int i = 0;
            for (i = 0; i < tabs; i++)
            {
                printf("\t");
            }
            printf("[%d] %s (%s)\n", count, dirent2->d_name, filetype(dirent2->d_type));
            if (dirent2->d_type == DT_DIR)
            {
                strcpy(functemp, temp);
                strcat(functemp, "/");
                strcat(functemp, dirent2->d_name);
                tabs++;
                reddir(functemp, tabs);
                tabs--;
            }
        }
    }
    closedir(parentDir2);
}

void Size(char *temp, int tabs)
{
    char functemp[100];
    int result;
    struct dirent *dirent2;
    DIR *parentDir2 = opendir(temp);
    int count = 1;
    struct stat buf;
    while ((dirent2 = readdir(parentDir2)) != NULL)
    {
        if (strcmp(dirent2->d_name, ".") != 0 && strcmp(dirent2->d_name, "..") != 0)
        {
            int i = 0;
            for (i = 0; i < tabs; i++)
            {
                printf("\t");
            }
            strcpy(functemp, temp);
            strcat(functemp, "/");
            strcat(functemp, dirent2->d_name);
            lstat(functemp, &buf);
            printf("[%d] %s (%s) %d\n", count, dirent2->d_name, filetype(dirent2->d_type), buf.st_size);
            if (dirent2->d_type == DT_DIR)
            {
                tabs++;
                reddir(functemp, tabs);
                tabs--;
            }
        }
    }
    closedir(parentDir2);
}

void Size_filter(char *temp, int flags, int tabs)
{
    char functemp[100];
    int result;
    struct dirent *dirent2;
    DIR *parentDir2 = opendir(temp);
    int count = 1;
    struct stat buf;

    while ((dirent2 = readdir(parentDir2)) != NULL)
    {
        if (strcmp(dirent2->d_name, ".") != 0 && strcmp(dirent2->d_name, "..") != 0)
        {
            int i = 0;
            for (i = 0; i < tabs; i++)
            {
                printf("\t");
            }
            strcpy(functemp, temp);
            strcat(functemp, "/");
            strcat(functemp, dirent2->d_name);
            lstat(functemp, &buf);
            if (buf.st_size > flags)
            {
                printf("[%d] %s (%s) %d\n", count, dirent2->d_name, filetype(dirent2->d_type), buf.st_size);
            }
            if (dirent2->d_type == DT_DIR)
            {
                tabs++;
                Size_filter(functemp, flags, tabs);
                tabs--;
            }
        }
    }
    closedir(parentDir2);
}
void type_filter(char *temp, int flags, int tabs)
{
    char functemp[100];
    int result;
    struct dirent *dirent2;
    DIR *parentDir2 = opendir(temp);
    int count = 1;
    struct stat buf;

    while ((dirent2 = readdir(parentDir2)) != NULL)
    {
        if (strcmp(dirent2->d_name, ".") != 0 && strcmp(dirent2->d_name, "..") != 0)
        {
            int i = 0;
            for (i = 0; i < tabs; i++)
            {
                printf("\t");
            }
            strcpy(functemp, temp);
            strcat(functemp, "/");
            strcat(functemp, dirent2->d_name);
            lstat(functemp, &buf);
            int fvalue = buf.st_mode;
            if (fvalue = flags)
            {
                printf("[%d] %s (%s) %s\n", count, dirent2->d_name, filetype(dirent2->d_type), buf.st_mode);
            }
            if (dirent2->d_type == DT_DIR)
            {
                tabs++;
                type_filter(functemp, flags, tabs);
                tabs--;
            }
        }
    }
}
closedir(parentDir2);
}
void type_filter_D(char *temp, int tabs)
{
    char functemp[100];
    int result;
    struct dirent *dirent2;
    DIR *parentDir2 = opendir(temp);
    int count = 1;
    struct stat buf;

    while ((dirent2 = readdir(parentDir2)) != NULL)
    {
        if (strcmp(dirent2->d_name, ".") != 0 && strcmp(dirent2->d_name, "..") != 0)
        {
            int i = 0;
            for (i = 0; i < tabs; i++)
            {
                printf("\t");
            }
            strcpy(functemp, temp);
            strcat(functemp, "/");
            strcat(functemp, dirent2->d_name);
            lstat(functemp, &buf);
            if (buf.st_mode == S_IFDIR)
            {
                printf("[%d] %s (%s) %s\n", count, dirent2->d_name, filetype(dirent2->d_type), buf.st_mode);
            }
            if (dirent2->d_type == DT_DIR)
            {
                tabs++;
                type_filter_D(functemp, tabs);
                tabs--;
            }
        }
    }
    closedir(parentDir2);
}

void type_filter_R(char *temp, int tabs)
{
    char functemp[100];
    int result;
    struct dirent *dirent2;
    DIR *parentDir2 = opendir(temp);
    int count = 1;
    struct stat buf;

    while ((dirent2 = readdir(parentDir2)) != NULL)
    {
        if (strcmp(dirent2->d_name, ".") != 0 && strcmp(dirent2->d_name, "..") != 0)
        {
            int i = 0;
            for (i = 0; i < tabs; i++)
            {
                printf("\t");
            }
            strcpy(functemp, temp);
            strcat(functemp, "/");
            strcat(functemp, dirent2->d_name);
            lstat(functemp, &buf);
            if (buf.st_mode == S_IFREG)
            {
                printf("[%d] %s (%s) %s\n", count, dirent2->d_name, filetype(dirent2->d_type), buf.st_mode);
            }
            if (dirent2->d_type == DT_DIR)
            {
                tabs++;
                type_filter_R(functemp, tabs);
                tabs--;
            }
        }
    }
    closedir(parentDir2);
}
