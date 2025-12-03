#include "parta.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * TODO: Initialize an array of PCBs on the heap 
 */
struct pcb* init_procs(int* bursts, int blen) {
     if (blen <= 0) {
        return NULL;
    }

    struct pcb* procs = malloc(sizeof(struct pcb) * blen);
    if (!procs) {
        return NULL;
    }

    for (int i = 0; i < blen; i++) {
        procs[i].pid        = i;
        procs[i].burst_left = bursts[i];
        procs[i].wait       = 0;
    }

    return procs;
}

/**
 * TODO: print all PCBs for debugging processes
 */
void printall(struct pcb* procs, int plen) {
     if (!procs || plen <= 0) return;

    for (int i = 0; i < plen; i++) {
        printf("PID %d: burst_left=%d wait=%d\n",
               procs[i].pid, procs[i].burst_left, procs[i].wait);
    }
}

/**
 * TODO: Run the process at index 'current' for 'amount' time units
 */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
     if (!procs || plen <= 0 || current < 0 || current >= plen || amount <= 0) {
        return;
    }

    struct pcb *p = &procs[current];

    if (p->burst_left <= 0) {
        return; 
    }

    int executed = amount;
    if (executed > p->burst_left) {
        executed = p->burst_left;
    }

    p->burst_left -= executed;

    for (int i = 0; i < plen; i++) {
        if (i == current) continue;
        if (procs[i].burst_left > 0) {
            procs[i].wait += executed;
        }
    }
}
/**
 * TODO: Run FCFS scheduling on the given PCB array 
 */
int fcfs_run(struct pcb* procs, int plen) {
    // TODO: Complete and document
     if (!procs || plen <= 0) {
        return 0;
    }

    int time = 0;

    for (int i = 0; i < plen; i++) {
        int burst = procs[i].burst_left;
        if (burst > 0) {
            run_proc(procs, plen, i, burst);
            time += burst;
        }
    }

    return time;
}


/**
 * TODO: Round robin helper 
 */
int rr_next(int current, struct pcb* procs, int plen) {
    // TODO: Complete and document
    if (!procs || plen <= 0) {
        return -1;
    }

    int any_left = 0;
    for (int i = 0; i < plen; i++) {
        if (procs[i].burst_left > 0) {
            any_left = 1;
            break;
        }
    }
    if (!any_left) {
        return -1;
    }

    int start;
    if (current < 0 || current >= plen) {
        start = 0;
    } else {
        start = (current + 1) % plen;
    }

    for (int offset = 0; offset < plen; offset++) {
        int idx = (start + offset) % plen;
        if (procs[idx].burst_left > 0) {
            return idx;
        }
    }

    return -1;
}
/**
 * TODO: Run Round Robin scheduling on the given PCB array
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    // TODO: Complete and document
     if (!procs || plen <= 0 || quantum <= 0) {
        return 0;
    }

    int time = 0;
    int current = -1;  

    while (1) {
        current = rr_next(current, procs, plen);
        if (current < 0) {
            break;  
        }

        int remaining = procs[current].burst_left;
        if (remaining <= 0) {
            continue;
        }

        int slice = (remaining < quantum) ? remaining : quantum;

        run_proc(procs, plen, current, slice);
        time += slice;
    }

    return time;
}


