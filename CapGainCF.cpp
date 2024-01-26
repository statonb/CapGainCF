#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <getopt.h>

const char *SW_VERSION =    "1.00";
const char *SW_DATE =       "2024-01-26";

#define MAX_CAP_LOSS_CF     (-3000)

void usage(const char *prog, const char *extraLine = (const char *)(NULL));

void usage(const char *prog, const char *extraLine)
{
    fprintf(stderr, "%s Ver %s %s\n", prog, SW_VERSION, SW_DATE);
    fprintf(stderr, "usage: %s <options>\n", prog);
    fprintf(stderr, "-g gain/(loss)                         Current year gain (or loss as a negative)\n");
    fprintf(stderr, "-p previous year's loss carry forward  Enter as a negative\n");
    if (extraLine) fprintf(stderr, "\n%s\n", extraLine);
}

int main(int argc, char *argv[])
{
    int                 opt;
    bool                usageError = false;
    int32_t             currentYearGain = 0;
    int32_t             previousYearCarryForward = 0;
    int32_t             adjustment = 0;

    struct option longOptions[] =
    {
        {"gain",            required_argument,  0,      'g'}
        ,{"previous",       required_argument,  0,      'p'}
        ,{0,0,0,0}
    };

    while (1)
    {
        int optionIndex = 0;
        opt = getopt_long(argc, argv, "g:p:qh?", longOptions, &optionIndex);

        if (-1 == opt) break;

        switch (opt)
        {
        case 'g':
            currentYearGain = strtol(optarg, NULL, 10);
            break;
        case 'p':
            previousYearCarryForward = strtol(optarg, NULL, 10);
            if  (previousYearCarryForward > 0)
            {
                usageError = true;
            }
            break;
        case 'h':
        case '?':
        default:
            usageError = true;
            break;
        }
    }

    // ------------ TEST CODE ---------
    #if 0
    currentYearGain = -4000;
    previousYearCarryForward = -3500;
    usageError = false;
    // ----------- END OF TEST CODE
    #endif

    if (usageError)
    {
        usage(basename(argv[0]));
        return -1;
    }

    printf("This year's capital gains                 = %d\n", currentYearGain);
    printf("Capital loss carry forward from last year = %d\n", previousYearCarryForward);

    if (currentYearGain > (-previousYearCarryForward))
    {
        adjustment = previousYearCarryForward;
        previousYearCarryForward = 0;
    }
    else
    {
        adjustment = -currentYearGain;
        previousYearCarryForward += currentYearGain;
        if (previousYearCarryForward >= MAX_CAP_LOSS_CF)
        {
            adjustment += previousYearCarryForward;
            previousYearCarryForward = 0;
        }
        else
        {
            adjustment += MAX_CAP_LOSS_CF;
            previousYearCarryForward -= MAX_CAP_LOSS_CF;
        }
    }

    printf("Capital Gain Adjustment                   = %d\n", adjustment);
    printf("Next Year Capital Loss Carry Forward      = %d\n", previousYearCarryForward);

    return 0;
}
