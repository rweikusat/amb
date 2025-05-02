/*
  iterative implementation of https://rosettacode.org/wiki/Amb
*/
#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *articles[] = { "the", "that", "an", 0 };
static char *nouns[] = { "frog", "elephant", "thing", 0};
static char *verbs[] = { "walked", "treaded", "grows", 0 };
static char *adverbs[] = { "slowy", "quickly", 0 };

static char **words[] = {
    articles,
    nouns,
    verbs,
    adverbs,
    0
};

/*
 * condition:
 * every word but the first starts with the letter
 * the word before it ended with
 */
static int l_eq_f(char *w0, char *w1)
{
    unsigned len;

    len = strlen(w0);
    return len ? w0[len - 1] == *w1 : !*w1;
}

static int chained(char **sentence)
{
    char *cur, *next;

    next = *sentence;
    while ((cur = next, cur) && (next = *++sentence, next))
        if (!l_eq_f(cur, next)) return 0;

    return 1;
}

/*
 * exhaustive search engine ("amb")
 */
static char **amb(char ***words, int (*pred)(char **))
{
    char **res, ***posn, **pos, *cur;
    unsigned level, depth;

    depth = 0;
    while (words[depth]) ++depth;
    if (!depth) return 0;

    posn = alloca((depth - 1) * sizeof(*posn));
    res = malloc((depth + 1) * sizeof(*res));
    res[depth] = 0;
    level = 0;
    --depth;

    do {
        while (level < depth) {
            posn[level] = words[level];
            res[level] = *posn[level];

            ++level;
        }

        pos = words[level];
        while (cur = *pos, cur) {
            res[level] = cur;
            if (pred(res)) return res;

            ++pos;
        }

        while (level) {
            --level;

            cur = *++posn[level];
            if (cur) {
                res[level] = cur;
                break;
            }
        }

        ++level;                
    } while (cur);

    free(res);
    return 0;
}

/*
 * main
 */
int main(void)
{
    char **res;
    
    res = amb(words, chained);
    if (res) {
        fputs(*res, stdout);
        while (*++res) printf(" %s", *res);
        putchar('\n');
    } else {
        puts("no solution");
    }
    
    return 0;
}
