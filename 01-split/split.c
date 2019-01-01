#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define TRUE 1
#define FALSE 0

#define USE_CUSTOM_POOL 0

char **str_split(char *str, char *separator, int *cnt_token, char **custom_pool)
{
    unsigned int i = 0;
    unsigned int j = 0;
    unsigned int k = 0;
    int last_pos = 0;
    char **tokens = NULL;
    char allocation_needed = TRUE;

    if (custom_pool != NULL)
    {
        tokens = custom_pool;
        allocation_needed = FALSE;
    }
    *cnt_token = 0;

    while (TRUE)
    {
        j = 0;
        k = i;
        while (k < strlen(str) && j < strlen(separator) && str[k] == separator[j] && k++ && j++)
            ;

        if (j == strlen(separator) || k == strlen(str))
        {
            if (allocation_needed)
            {
                tokens = (char **)realloc(tokens, sizeof(char *) * (*cnt_token + 1));
                tokens[*cnt_token] = (char *)malloc(sizeof(char) * (i - last_pos + 1));
            }
            strncpy(tokens[*cnt_token], str + last_pos, i - last_pos);
            tokens[*cnt_token][i - last_pos] = '\0';
            (*cnt_token)++;
            last_pos = k;

            if (k == strlen(str))
            {
                break;
            }
            else
            {
                i = k;
                continue;
            }
        }

        i++;
    }

    return tokens;
}

int main(int argc, char **argv)
{
    struct timeval tv_start, tv_end;
    double ms;

    int num_of_tokens = 0, j = 0;
    FILE *fp = fopen(argv[1], "r");

    char line[1024];
    char **tokens = NULL;
#if USE_CUSTOM_POOL
    char **custom_pool = (char **)malloc(sizeof(char *) * 255);
#else
    char **custom_pool = NULL;
#endif

    long all_lines = 0;

    if (!fp)
    {
        printf("Can't find file: %s\n", argv[1]);
        return -1;
    }

    /* Initalizing the custom memory pool to avoid */
    if (custom_pool != NULL)
    {
        for (j = 0; j < 255; j++)
        {
            custom_pool[j] = (char *)malloc(sizeof(char) * 255);
        }
    }

    gettimeofday(&tv_start, NULL);
    while (fgets(line, 1024, fp))
    {
        num_of_tokens = 0;
        tokens = str_split(line, " ", &num_of_tokens, custom_pool);
        all_lines += num_of_tokens;

        if (custom_pool == NULL)
        {
            for (j = 0; j < num_of_tokens; j++)
            {
                free(tokens[j]);
            }
            free(tokens);
        }
    }
    fclose(fp);

    gettimeofday(&tv_end, NULL);
    ms = (tv_end.tv_usec + ((tv_end.tv_sec - tv_start.tv_sec) * 1000 * 1000) - tv_start.tv_usec) / 1000;
    printf("Number of lines processed: %li\n", all_lines);
    printf("Elapsed time: %.2f ms\n", ms);

    if (custom_pool != NULL)
    {
        for (j = 0; j < 255; j++)
        {
            free(custom_pool[j]);
        }
        free(custom_pool);
    }

    return 0;
}
