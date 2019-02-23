#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>

#define SCHED_IOT 7

#ifndef SCHED_BATCH
# define SCHED_BATCH 3
#endif

static void show_usage(const char *cmd)
{
    fprintf(stderr, "usage: %s [options] [prio] [pid | cmd [args...]]\n",
            cmd);
    fprintf(stderr, "manipulate real-time attributes of a process\n");
    fprintf(stderr, "-b, --batch                        "
                    "set policy to SCHED_BATCH\n");
    fprintf(stderr, "-f, --fifo                         "
                    "set policy to SCHED_FF\n");
    fprintf(stderr, "-p, --pid                          "
                    "operate on existing given pid\n");
    fprintf(stderr, "-m, --max                          "
                    "show min and max valid priorities\n");
    fprintf(stderr, "-o, --other                        "
                    "set policy to SCHED_OTHER\n");
    fprintf(stderr, "-r, --rr                           "
                    "set policy to SCHED_RR (default)\n");
    fprintf(stderr, "-i, --iot                           "
                    "set policy to SCHED_IOT \n");
    fprintf(stderr, "-h, --help                         "
                    "display this help\n");
    fprintf(stderr, "-v, --verbose                      "
                    "display status information\n");
    fprintf(stderr, "-V, --version                      "
                    "output version information\n\n");
    fprintf(stderr, "You must give a priority if changing policy.\n\n");
    fprintf(stderr, "Report bugs and send patches to <rml@tech9.net>\n");
}

static void show_rt_info(const char *what, pid_t pid)
{
    struct sched_param sp;
    int policy;

    /* don't display "pid 0" as that is confusing */
    if (!pid)
        pid = getpid();

    policy = sched_getscheduler(pid);
    if (policy == -1) {
        perror("sched_getscheduler");
        fprintf(stderr, "failed to get pid %d's policy\n", pid);
        exit(1);
    }

    printf("pid %d's %s scheduling policy: ", pid, what);
    switch (policy) {
        case SCHED_OTHER:
            printf("SCHED_OTHER\n");
            break;
        case SCHED_FIFO:
            printf("SCHED_FIFO\n");
            break;
        case SCHED_RR:
            printf("SCHED_RR\n");
            break;
        case SCHED_BATCH:
            printf("SCHED_BATCH\n");
            break;
        case SCHED_IOT:
            printf("SCHED_IOT\n");
            break;
        default:
            printf("unknown\n");
    }

    if (sched_getparam(pid, &sp)) {
        perror("sched_getparam");
        fprintf(stderr, "failed to get pid %d's attributes\n", pid);
        exit(1);
    }

    printf("pid %d's %s scheduling priority: %d\n",
           pid, what, sp.sched_priority);
}

static void show_min_max(void)
{
    int max, min;

    max = sched_get_priority_max(SCHED_OTHER);
    min = sched_get_priority_min(SCHED_OTHER);
    if (max >= 0 && min >= 0)
        printf("SCHED_OTHER min/max priority\t: %d/%d\n", min, max);
    else
        printf("SCHED_OTHER not supported?\n");

    max = sched_get_priority_max(SCHED_FIFO);
    min = sched_get_priority_min(SCHED_FIFO);
    if (max >= 0 && min >= 0)
        printf("SCHED_FIFO min/max priority\t: %d/%d\n", min, max);
    else
        printf("SCHED_FIFO not supported?\n");

    max = sched_get_priority_max(SCHED_RR);
    min = sched_get_priority_min(SCHED_RR);
    if (max >= 0 && min >= 0)
        printf("SCHED_RR min/max priority\t: %d/%d\n", min, max);
    else
        printf("SCHED_RR not supported?\n");

    max = sched_get_priority_max(SCHED_BATCH);
    min = sched_get_priority_min(SCHED_BATCH);
    if (max >= 0 && min >= 0)
        printf("SCHED_BATCH min/max priority\t: %d/%d\n", min, max);
    else
        printf("SCHED_BATCH not supported?\n");


    max = sched_get_priority_max(SCHED_IOT);
    min = sched_get_priority_min(SCHED_IOT);
    if (max >= 0 && min >= 0)
        printf("SCHED_IOT min/max priority\t: %d/%d\n", min, max);
    else
        printf("SCHED_IOT not supported?\n");

}

int main(int argc, char *argv[])
{
    int i, policy = SCHED_RR, priority = 0, verbose = 0;
    struct sched_param sp;
    pid_t pid = 0;

    struct option longopts[] = {
            { "batch",	0, NULL, 'b' },
            { "fifo",	0, NULL, 'f' },
            { "iot",	0, NULL, 'i' },
            { "pid",	0, NULL, 'p' },
            { "max",        0, NULL, 'm' },
            { "other",	0, NULL, 'o' },
            { "rr",		0, NULL, 'r' },
            { "verbose",	0, NULL, 'v' },
            { "version",	0, NULL, 'V' },
            { NULL,		0, NULL, 0 }
    };



    while((i = getopt_long(argc, argv, "+bifphmorvV", longopts, NULL)) != -1)
    {
        int ret = 1;

        switch (i) {
            case 'b':
                policy = SCHED_BATCH;
                break;
            case 'f':
                policy = SCHED_FIFO;
                break;
            case 'm':
                show_min_max();
                return 0;
            case 'o':
                policy = SCHED_OTHER;
                break;
            case 'p':
                errno = 0;
                pid = strtol(argv[argc - 1], NULL, 10);
                if (errno) {
                    perror("strtol");
                    fprintf(stderr, "failed to parse pid!\n");
                    return 1;
                }
                break;
            case 'r':
                policy = SCHED_RR;
                break;
            case 'v':
                verbose = 1;
                break;
            case 'i':
                policy = SCHED_IOT;
                break;
            default:
                policy = SCHED_OTHER;
                show_usage(argv[0]);
                return ret;
        }

    }
    if ((pid && argc - optind < 1) || (!pid && argc - optind < 2)) {
        show_usage(argv[0]);
        return 1;
    }

    if (pid && (verbose || argc - optind == 1)) {
        show_rt_info("current", pid);
        if (argc - optind == 1)
            return 0;
    }

    errno = 0;
    priority = 98;
    if (errno) {
        perror("strtol");
        fprintf(stderr, "failed to parse priority!\n");
        return 1;
    }

    printf("[FRISK] setting up priority %d\n", priority);

    sp.sched_priority = priority;
    if (sched_setscheduler(pid, policy, &sp) == -1) {
        perror("sched_setscheduler");
        fprintf(stderr, "failed to set pid %d's policy\n", pid);
        return 1;
    }

    if (verbose)
        show_rt_info("new", pid);

    if (!pid) {
        argv += optind + 1;
        execvp(argv[0], argv);
        perror("execvp");
        fprintf(stderr, "failed to execute %s\n", argv[0]);
        return 1;
    }

    return 0;
}
