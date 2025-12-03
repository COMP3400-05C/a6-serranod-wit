#include "parta.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // TODO: Complete and document
      if (argc < 3) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }

    char *algo = argv[1];

    int is_fcfs = (strcmp(algo, "fcfs") == 0);
    int is_rr   = (strcmp(algo, "rr") == 0);

    if (!is_fcfs && !is_rr) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }

    int quantum = 0;
    int burst_start_index = 0;
    int plen = 0;

    if (is_fcfs) {
        if (argc < 3) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        burst_start_index = 2;
        plen = argc - burst_start_index;
    } else { 
        if (argc < 4) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        quantum = atoi(argv[2]);
        if (quantum <= 0) {
            printf("ERROR: Missing arguments\n");
            return 1;
        }
        burst_start_index = 3;
        plen = argc - burst_start_index;
    }

    if (plen <= 0) {
        printf("ERROR: Missing arguments\n");
        return 1;
    }

    int *bursts = malloc(sizeof(int) * plen);
    if (!bursts) {
        fprintf(stderr, "ERROR: Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < plen; i++) {
        bursts[i] = atoi(argv[burst_start_index + i]);
    }

    struct pcb *procs = init_procs(bursts, plen);
    if (!procs) {
        fprintf(stderr, "ERROR: Failed to initialize processes\n");
        free(bursts);
        return 1;
    }

    if (is_fcfs) {
        printf("Using FCFS\n\n");
    } else {
        printf("Using RR(%d).\n\n", quantum);
    }

    for (int i = 0; i < plen; i++) {
        printf("Accepted P%d: Burst %d\n", i, bursts[i]);
    }

    int total_time = 0;
    if (is_fcfs) {
        total_time = fcfs_run(procs, plen);
    } else {
        total_time = rr_run(procs, plen, quantum);
    }
    (void)total_time; 
    
    double total_wait = 0.0;
    for (int i = 0; i < plen; i++) {
        total_wait += procs[i].wait;
    }
    double avg_wait = total_wait / plen;

    printf("Average wait time: %.2f\n", avg_wait);

    free(procs);
    free(bursts);

    return 0;
}
