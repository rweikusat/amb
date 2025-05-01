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
    char **res, ***pos, **posl, *cur;
    unsigned level, depth;

    /*
      Basic idea behind this algorithm is to move down in the word
      list set until there's a first possible solution and then, work
      back up from there. The input set is partitioned into two sets:

          - n intermediate lists, n >= 0
          - the final list

      Each intermediate list has a current position in the array
      pos. The variable level is the start level for the current
      iteration, initially 0. A variable named cur points to the
      current word.

      The first inner loop moves from start level to the last
      intermediate level, initializing the pos of the current level to
      the start of the corresponding word list and the result slot for
      the current depth to the first word from that.

      The second inner loop works through the final word list: It sets
      the last result slot to each word on that in turn and invokes
      the predicate function to determine if the prospective result is
      good. If so, the function returns it.

      The third inner loop moves back up through the intermediate lists
      in order to find one whose supply of words hasn't yet been
      exhausted. It decreases the level, increases the pos for the
      new level and looks for a word there. If one is found, it's
      put into the res slot for the current level and the loop
      terminates. Otherwise, it continues if the top level hadn't been
      reached yet.

      The outer loop terminates if cur doesn't point to a word after
      all the inner processing took place. This can either happen
      because only one list was provided and the second inner loop
      worked throught it without finding a solution. Or because the
      third inner loop determined that the search space has been
      exhausted, because it couldn't find a word to continue with at
      any intermediate level.
    */

    depth = 0;
    while (words[depth]) ++depth;
    if (!depth) return 0;

    pos = alloca((depth - 1) * sizeof(*pos));
    res = malloc((depth + 1) * sizeof(*res));
    res[depth] = 0;
    level = 0;
    --depth;

    do {
        while (level < depth) {
            pos[level] = words[level];
            res[level] = *pos[level];

            ++level;
        }

        posl = words[level];
        while (cur = *posl, cur) {
            res[level] = cur;
            if (pred(res)) return res;

            ++posl;
        }

        while (level) {
            --level;

            cur = *++pos[level];
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
