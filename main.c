#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

char *find_next_char(char *str, char c)
{
    return strchr(str, c);
}

int find_chars(char *str, char p1, char p2)
{
    int n1 = -1;
    int j;
    for (j = 0; j < strlen(str); ++j)
    {
        if (p1 == str[j])
        {
            n1 = j;
            break;
        }

        if (p2 == str[j])
        {
            return -1;
        }
    }

    int n2 = -1;
    for (int i = j + 1; i < strlen(str); ++i)
    {
        if (p2 == str[i])
        {
            n2 = i;
            break;
        }

        if (p1 == str[i])
        {
            return -1;
        }
    }

    if (n1 == -1 && n2 == -1)
    {
        return 0;
    }

    if (n2 == -1)
    {
        return 1;
    }

    return 2;
}

int count_pattern(char *str, char *pattern, int count)
{
    // printf("pattern name: %s\n", pattern);

    if (str == NULL || strlen(str) == 0)
    {
        return 0;
    }

    // printf("str name: %s\n", str);

    int found_char = find_chars(str, pattern[0], pattern[1]);

    // printf("%d\n", found_char);

    if (found_char == -1)
    {
        return count * -1;
    }

    count = count + found_char;

    if (found_char == 0)
    {
        return found_char;
    }

    char *c1 = find_next_char(str, pattern[0]);
    char *c2 = find_next_char(++c1, pattern[1]);

    // printf("c2 = %s\n", c2);

    if (c2 == NULL)
    {
        return found_char;
    }
    else
    {
        return count_pattern(++c2, pattern, count) + found_char;
    }
}

typedef struct object
{
    char *str;
    char *pattern;
    int count;

} Object;

void *count_patterns_thread(void *input)
{

    char *str = ((Object *)input)->str;
    char *pattern = ((Object *)input)->pattern;
    int count = ((Object *)input)->count;

    int c = count_pattern(str, pattern, 0);

    ((Object *)input)->count = c;

    pthread_exit(NULL);
}

/* char *full_pattern(char *str, char *pattern)
{
    printf("pattern name: %s\n", pattern);
    printf("str name: %s\n", str);
    char *c1 = find_next_char(str, pattern[0]);

    if (c1 == NULL)
    {
        return "";
    }

    char *c2 = find_next_char(++c1, pattern[1]);

    if (c2 == NULL)
    {
        puts("puts");
        char *tmp = calloc(2, sizeof(char));
        tmp[0] = pattern[0];
        tmp[1] = '\0';
        return "b";
    }

    char buf[256];
    snprintf(buf, sizeof(buf), "%s", full_pattern(++c2, pattern));

    // strcat(pattern, full_pattern(++c2, pattern));

    return buf;
} */

typedef struct array
{
    // input
    int *array;
    int start;
    int end;

    // output
    int largest;
} Array;

void *find_max_thread(void *args)
{
    Array *params = (Array *)args;
    int *array = params->array;
    int start = params->start;
    int end = params->end;
    int largest = array[start];

    for (int i = start; i < end; i++)
    {
        if (array[i] > largest)
        {
            largest = array[i];
        }
    }

    params->largest = largest;

    // printf("lar = %d\n", largest);
    // printf("i = %d\n", end - start);

    pthread_exit(NULL);
}

int alternate(char *s)
{
    // int max = 0;
    const int len = strlen(s);
    const int l = len * len;

    // int *retvals[len * len + 40];
    int *retvals = calloc(l, sizeof(int));

    // int retvals[l];

    /* for (size_t i = 0; i < l; i++)
    {
        retvals[i] = 0;
    } */

    int counter = 0;
    int i;
    int step = 0;
    int top = len / 10;
    for (i = 0; i < 10; i++)
    {
        // printf("top = %d\n", top);
        // printf("step = %d\n", step);
        // printf("i = %d\n", i);
        pthread_t p_thread_1[1000];

        for (int k = step; k < top; ++k)
        {
            for (size_t j = k + 1; j < len; j++)
            {
                char *pattern = malloc(3);
                pattern[0] = s[k];
                pattern[1] = s[j];
                pattern[2] = '\0';

                if (s[k] == s[j])
                {
                    continue;
                }

                Object o;
                o.str = s;
                o.pattern = pattern;
                o.count = 0;

                if (pthread_create(&p_thread_1[k], NULL, count_patterns_thread, &o) != 0)
                {
                    fprintf(stderr, "error: Cannot create thread # %d\n", k);
                    break;
                }

                if (pthread_join(p_thread_1[k], NULL) != 0)
                {
                    fprintf(stderr, "error: Cannot join thread # %d\n", k);
                }

                // printf("num = %d str = %s\n", max, pattern);
                // printf("pattern = %s\n", o.pattern);
                // printf("counter = %d\n", counter);
                retvals[counter] = o.count;
                counter++;
                free(pattern);
            }
        }
        step = top;
        top = len * (i + 2) / 10;

        // printf("top = %d\n", top);
        // printf("step = %d\n", step);
        // printf("i = %d\n", i);
    }

    /* for (size_t i = 0; i < l; i++)
    {
        printf("r = %d\n", retvals[i]);
        if (max < retvals[i])
        {
            max = retvals[i];
        }
    }

    return 0; */

    /* for (size_t i = 0; i < (10); i++)
    {
        printf("r = %d\n", *(&retvals[i]));
        if (&retvals[i] != NULL && max < *(&retvals[i]))
        {
            max = *(&retvals[i]);
        }
    } */

    // printf("max length: %d\n", max);

    int largest;

    int THREAD_COUNT = 100;

    int ARRAY_SIZE = l;

    // declare an array of threads and associated parameter instances
    pthread_t threads[THREAD_COUNT];
    Array thread_parameters[THREAD_COUNT];

    // smallest and largest needs to be set to something
    largest = retvals[0];

    // start all the threads
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        thread_parameters[i].array = retvals;
        thread_parameters[i].start = i * (ARRAY_SIZE / THREAD_COUNT);
        thread_parameters[i].end = (i + 1) * (ARRAY_SIZE / THREAD_COUNT);
        thread_parameters[i].largest = 0;
        pthread_create(&threads[i], NULL, find_max_thread, &thread_parameters[i]);
    }

    // wait for all the threads to complete
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Now aggregate the "smallest" and "largest" results from all thread runs
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        if (thread_parameters[i].largest > largest)
        {
            largest = thread_parameters[i].largest;
        }
    }
    // printf("Largest is %d\n", largest);

    free(retvals);

    return largest;
}

int main(void)
{
    // char *str = "asdcbsdcagfsdbgdfanfghbsfdab";

    char *str = "beabeefeab";

    // char *str = "abaacdabd";

    // char *str = "asvkugfiugsalddlasguifgukvsa";

    // char *str = "ab";

    // char *str = "bleinlirvdllgpqysgejejaptjrnmuvsulpwwofocahzbdngybbqdfleycicnpbrdkzmzraayiqzwplgnnmirnddadidsyftrezectrelgwzegsrvzyjramgqvwwotacyurxrpfyrvkrqpcjrezpbizwkzwtucohtcwqktiyrlvxtwuilwvjdhoqaiiqjpkosjkolpgojlfabpperqqtmnjowynwmxavicjdknpgnmpktovtssynavflaxqbxygryjbfymjfcemqgnhrhyunopicpsskhzkvdnedrweuneshcuoegxzlmcuvojbnqcyapqvnwpfufqpekjvxxujogguhhtvwlrrvctqdllpdegtwwmfnceaiqfpfoqggkqpbmdzhdpsrklllsssazidvcpsipxhucgcqxpekijfijqblnvbrubgqohwpqrngilierbzjrjozslmibpocyzeqrtfenkcklvtajhrzumyjgdkzdaztytogvrncqgcutwdpvnuesbadhfgijjcjygonhvkhlypwnexumozowkfnykdovqjrwnwsudufhvcikaedsfiyzoqyvodmwixpmdpxtveinykvdrjdbmandgzcouzdlpiynwlhcwqafaqpqjdkbouelfbmztbqshzlgedbduhgcerrbqnqzfvgpfheqrnwlsduxklrfjjnkmvetkuzagkdmkaugptrdenqfiavgqzfubybmjcgoqlmvgcdmddwigtqmvjpkwlkuyxdycuriyrvlbghvyagxulvqmrkxlqfpxblnwdctznlrbbactsrbubcaayntkjmhzjzuyruejekcorvtbglaccnzxhutfqzjrfadgpgubqynmbxziudhmzcpmpx";

    int l = alternate(str);

    printf("Max length of two characters chain is: %d", l);

    return 0;
}
