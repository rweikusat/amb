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
      In the following, a depth/ level refers to the distance of the
      current position in the words array from the start of it. Level 0
      is on top, followed by level 1 immediately below it and so
      forth.
      
      Basic idea behind this algorithm is to move down in the word
      list set until there's a first possible solution and then, work
      rightward and back up from there. The input set is partitioned
      into two sets:

          - n intermediate lists, n >= 0
          - the final list

      Variables:

          cur:   either the last word assign to a res slot or null pointer
          depth: number of intermediate lists (while inside outmost loop)
          level: current vertical position in the input set
          pos:   array of horizontal positions (as char **) for the intermediate lists
          res:   array storing the current prospective solution
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
        /*
          At this point, there's a conceptual ceiling (at level - 1)
          which is the depth of the result slot which was changed
          last. All result slots from 0 to the current ceiling are
          supposed to retain their present values and all position
          from 0 to the current ceiling to remain as they are. The
          loop resets all positions below the ceiling and above the
          final list to the start and all corresponding result slots
          to the first words of the respective lists.

          For the first iteration of the outer loop, the ceiling is
          virtual and assumed to exist at depth -1.
        */
        while (level < depth) {
            pos[level] = words[level];
            res[level] = *pos[level];

            ++level;
        }

        /*
          Process final list. Set the final result slot to each word
          in turn and check if the current prospective result is a
          solution. If so, return it, otherwise continue with next
          word or exit the loop if there isn't any.
        */
        posl = words[level];
        while (cur = *posl, cur) {
            res[level] = cur;
            if (pred(res)) return res;

            ++posl;
        }

        /*
          Move back up until a list whose supply of words hasn't yet
          been exhausted is found or level 0 has been checked in
          vain. If such a list was found, the result slot for the
          current level is set to the next word on it and the loop
          terminates.
        */
        while (level) {
            --level;

            cur = *++pos[level];
            if (cur) {
                res[level] = cur;
                break;
            }
        }

        /*
          Current level is the ceiling for the next
          iteration. Increase level so that the first inner loop
          starts working below it.
        */
        ++level;                
    } while (cur);              /* no more words -> terminate */

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
