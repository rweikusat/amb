/*
  recursvie implementation of https://rosettacode.org/wiki/Amb I wrote because
  	- I didn't program anything for weeks
        - to fill the slot for an additional file

  I did this in Perl first but then redid it in C because
  "it can't be so difficult."
*/
#include <alloca.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *articles[] = { "the", "that", "a", 0 };
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
 * every word but the first start with the letter
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
    while (cur = next, cur) {
        next = *++sentence;
        if (!l_eq_f(cur, next)) return 0;
    }

    return 1;
}

/*
 * helper: copy a sentence to heap memory
 */
static char **copy_s(char **sentence)
{
    char **copy;
    unsigned n;

    n = 0;
    while (sentence[n]) ++n;
    if (!n) return 0;

    copy = malloc((n + 1) * sizeof(*sentence));
    n = 0;
    do copy[n] = sentence[n]; while (sentence[++n]);
    copy[n] = 0;

    return copy;
}

/*
 * exhaustive search engine ("amb")
 */
static char **amb(char ***words, int (*pred)(char **), char **sentence)
{
    char **mine, ***remain, **s_work, **res;
    unsigned n, n_mine;

    if (!*words) {
        if (pred(sentence)) return copy_s(sentence);
        return 0;
    }

    /* split words into "my words" and "remaining words" */
    n = 1;
    while (words[n]) ++n;
    remain = alloca(sizeof(*words) * n);
    mine = *words;
    n = 0;
    do remain[n] = words[n + 1]; while (words[++n]);

    /* create "work sentence" as "all words of sentence" + "space for our word" + 0 marker */
    if (!sentence) {
        s_work = alloca(2 * sizeof(*sentence));
        n = 0;
    } else {
        n = 0;
        while (sentence[n]) ++n;
        s_work = alloca((n + 2) * sizeof(*sentence));

        n = 0;
        while (sentence[n]) {
            s_work[n] = sentence[n];
            ++n;
        }
    }
    s_work[n + 1] = 0;

    /* search loop */
    n_mine = 0;
    while (s_work[n] = mine[n_mine], s_work[n]) {
        res = amb(remain, pred, s_work);
        if (res) return res;

        ++n_mine;
    }

    return 0;
}

/*
 * main
 */
int main(void)
{
    char **res;
    
    res = amb(words, chained, NULL);
    if (res) {
        fputs(*res, stdout);
        while (*++res) printf(" %s", *res);
        putchar('\n');
    } else {
        puts("no solution");
    }
    
    return 0;
}
