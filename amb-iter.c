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

    /*
      Without loss of generality, there are three possible cases for
      algorithm below.

      1. There's only one list.

          In this case, the first inner loop will do nothing as

          level == depth == 0

          The second inner loop will assign all words of the single
          list to the final result slot and call pred to determine if
          the current result vector is a solution. If so, it's
          returned.

          As level == 0, the third inner loop will again do nothing
          and as cur == NULL (termination condition for second inner
          loop), the outer loop terminates and 'no solution' is
          returned.

      2. There are two lists.

         Initially level == 0 and depth == 1. The first loop will thus
         set the position for the first list to start and its result
         slot to the first word on it.

         The second inner loop proceeds as described in (1).

         As level is set, the third inner loop will decrease it and
         increase the position for the first list. If there's still a
         word on it, the result slot will be set to it and the loop
         terminates. Level is then increased again.If there was no more
         words, cur will be NULL and cause the outer loop to terminate.

         On the next and all subsequent iterations of the outer loop,
         the first inner loop will do nothing as level == depth. The
         second and third inner loops will continue as already
         described.

      3. There are three lists.

         Initially, level == 0 and depth == 2. The first iteration of
         the first inner loop will set the positions and results slots
         for the first two lists to their respective start values.

         The second inner loop proceeds as described in (1).

         For as long as there's still a word on the second list,
         everything continues as described in (2). When the third inner
         loop has exhausted the second list, it decreases the level
         again and thus, gets to the first list. It increases its
         position and looks for a word there. If none is found, the
         third inner loop terminates with cur == NULL, leading to 'no
         solution' as already described. Otherwise, the (2)
         processing happens again until the second list has been
         exhausted again.
    */
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
