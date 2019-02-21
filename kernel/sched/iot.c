#include "sched.h"
#include <linux/slab.h>
#include <linux/irq_work.h>

static DEFINE_PER_CPU(cpumask_var_t, local_cpu_mask);

/* Initialize your scheduler specific runqueue */
void init_iot_rq(struct iot_rq *iot_rq)
{
    /*
     * This function is run once per-cpu. It can be
     * use to initalize a runqueue specific to your
     * scheduler class.
     * 
     * i.e. Initaliaze variables, entites, variables, etc.
     */

    struct iot_prio_array *array;
    int i;

    // Get the address of the active run-queues
    array = &iot_rq->active;

    // Initialize bitmap
    for (i = 0; i < MAX_IOT_PRIO; i++) {
        INIT_LIST_HEAD(array->queue + i);
        __clear_bit(i, array->bitmap);
    }

    /* delimiter for bitsearch: */
    __set_bit(MAX_IOT_PRIO, array->bitmap);

    // Set highest priority value
    iot_rq->highest_prio.curr = MAX_IOT_PRIO;

    /* We start in dequeued state, because no IOT tasks are queued */
    iot_rq->iot_queued = 0;
}

/* Initialize scheduler class */
void __init init_sched_iot_class(void)
{
   /*
    * This function is called to initialize your scheduler class. This will only
    * be called once, not per-CPU. Here you can initialize any variables, masks,
    * priorities, etc. 
    */

    unsigned int i;
    for_each_possible_cpu(i) {
        zalloc_cpumask_var_node(&per_cpu(local_cpu_mask, i),GFP_KERNEL, cpu_to_node(i));
    }
}

/* Add task to runqueue */
static void enqueue_task_iot(struct rq *rq, struct task_struct *p, int flags)
{
    /*
     * This function is called when a task is ready to be
     * put on the runqueue. The flags specify the reason
     * the function has been called.
     */

    /*
    * {de,en}queue flags:
    *
    * DEQUEUE_SLEEP  - task is no longer runnable
    * ENQUEUE_WAKEUP - task just became runnable
    *
    * SAVE/RESTORE - an otherwise spurious dequeue/enqueue, done to ensure tasks
    *                are in a known state which allows modification. Such pairs
    *                should preserve as much state as possible.
    *
    * MOVE - paired with SAVE/RESTORE, explicitly does not preserve the location
    *        in the runqueue.
    *
    * ENQUEUE_HEAD      - place at front of runqueue (tail if not specified)
    * ENQUEUE_REPLENISH - CBS (replenish runtime and postpone deadline)
    * ENQUEUE_MIGRATED  - the task was migrated during wakeup
    */
}

/* Remove task from runqueue */
static void dequeue_task_iot(struct rq *rq, struct task_struct *p, int flags)
{
    /*
     * This function is called when a task is to be
     * removed from the runqueue. The flags specify the reason
     * the function has been called.
     */

    /*
    * {de,en}queue flags:
    *
    * DEQUEUE_SLEEP  - task is no longer runnable
    * ENQUEUE_WAKEUP - task just became runnable
    *
    * SAVE/RESTORE - an otherwise spurious dequeue/enqueue, done to ensure tasks
    *                are in a known state which allows modification. Such pairs
    *                should preserve as much state as possible.
    *
    * MOVE - paired with SAVE/RESTORE, explicitly does not preserve the location
    *        in the runqueue.
    *
    * ENQUEUE_HEAD      - place at front of runqueue (tail if not specified)
    * ENQUEUE_REPLENISH - CBS (replenish runtime and postpone deadline)
    * ENQUEUE_MIGRATED  - the task was migrated during wakeup
    */
}

/* Specify on which CPU the process should run */
static int select_task_rq_iot(struct task_struct *p, int cpu, int sd_flag, int flags)
{
    /*
     * In this function, you will choose which CPU the task is assigned to.
     * This can be used to distribute your tasks to multiple CPUs, as there
     * is a per-CPU runqueue. The flags specify the reason the function has
     * been called.
     */

    /*
        SD_LOAD_BALANCE         -   Do load balancing on this domain.
        SD_BALANCE_NEWIDLE      -   Balance when about to become idle
        SD_BALANCE_EXEC         -   Balance on exec
        SD_BALANCE_FORK         -   Balance on fork, clone
        SD_BALANCE_WAKE         -   Balance on wakeup
        SD_WAKE_AFFINE          -   Wake task to waking CPU
        SD_ASYM_CPUCAPACITY     -   Groups have different max cpu capacities
        SD_SHARE_CPUCAPACITY    -   Domain members share cpu capacity
        SD_SHARE_POWERDOMAIN    -   Domain members share power domain
        SD_SHARE_PKG_RESOURCES  -   Domain members share cpu pkg resources
        SD_SERIALIZE            -   Only a single load balancing instance
        SD_ASYM_PACKING         -   Place busy groups earlier in the domain
        SD_PREFER_SIBLING       -   Prefer to place tasks in a sibling domain
        SD_OVERLAP              -   sched_domains of this level overlap
        SD_NUMA                 -   cross-node balancing
    */

    return NULL;
}

/* Preempt the current task with a newly woken task, if needed */
static void check_preempt_curr_iot(struct rq *rq, struct task_struct *p, int flags)
{
    /*
     * In this function, you will be choosing what do when a new
     * task has become runnable. Mainly, you will decide whether
     * this new task will preempt the task that is currently
     * running.
     */

    /*
     *  wake flags
     *
     *  WF_SYNC     -   waker goes to sleep after wakeup
     *  WF_FORK     -   child wakeup after fork
     *  WF_MIGRATED -   internal use, task got migrated
     */
}

/* Choose the task to be run next */
static struct task_struct *pick_next_task_iot(struct rq *rq, struct task_struct *prev, struct rq_flags *rf)
{
    /*
     * In this function, you will be choosing which task will be run next.
     * Which task you choose will depend on how you organize your tasks
     * in the runqueue. Remember that there is a per-CPU runqueue. This
     * function is called primarily from the core schedule() function.
     * 
     * You can look up 'struct rq_flags' for the flags that are passed.
     */
    
    return NULL;
}

/* Assumes rq->lock is held */
static void rq_online_iot(struct rq *rq)
{
    /*
     * This function puts the runqueue online, if it is not already. The
     * function is called:
     *  1) In the early boot process
     *  2) At runtime, if cpuset_cpu_active() fails to rebuild the domains
     * 
     * In basic terms: A cpuset is a mechanism that allows one to assign a
     * set of CPUs and Memory to a set of tasks. 
     * 
     * You can use this function to maintain stats or set masks. 
     * i.e. CPU priority, mask, etc.
     */
}

/* Assumes rq->lock is held */
static void rq_offline_iot(struct rq *rq)
{
    /*
     * This function is called in the event the runqueue is being taken offline.
     * You can free up any resources belonging to the specific CPU runqueue being
     * passed. i.e. any stats or masks being held for the CPU or runqueue
     */
}

/* Task woken up */
static void task_woken_iot(struct rq *rq, struct task_struct *p){
    /*
     * This function is called when a task is woken up. The task is
     * not currently running and has not been called to be scheduled.
     */
}

/* Set the current task */
static void set_curr_task_iot(struct rq *rq) {
    /*
     * This function is mainly called when a task is changing either its
     * policy or group. This function can be used to keep various stats
     * or housekeeping items. Dependant on the implementation of your
     * scheduler. 
     */
}

static void update_curr_iot(struct rq *rq) {
    /*
     * Here you can update any information on the current task relevant to 
     * your scheduler. i.e. runtime statistics
     */
}

/* Task has switched from our scheduler class */
static void switched_from_iot(struct rq *rq, struct task_struct *p){
    /*
     * This function is called when a task has switched scheduler classes
     * and is no longer assigned to our scheduler class. Here you can do 
     * some housekeeping relevant to the task switched from our scheduler.
     */
}

/* Priority of a task has changed */
static void prio_changed_iot(struct rq *rq, struct task_struct *p, int oldprio) {
    /*
     * This function is called when a task has switched priority values. Here you can 
     * perform any relevant functions specific to your scheduler implementation. You
     * are also given the previous priority value of the task.
     */
}

/* Function called on timer interrupt */
static void task_tick_iot(struct rq *rq, struct task_struct *p, int queued) {
    /*
     * This function is called whenever there is an interrupt from the timer.
     * This allows you to perform functions periodically on your scheduler, as 
     * other functions may be called only under certain pre-defined events. You
     * can use this functionality to update various stats pertaining to your
     * scheduler, reschedule a task, maintain any data structures, etc.
     */
}

/* Task has switched to our scheduler class */
static void switched_to_iot(struct rq *rq, struct task_struct *p) {
    /*
     * This function is classed when a task task has been assigned our scheduler
     * class. The task is part of our scheduler class. Here you can perform
     * functions that are relevant to your scheduler implementation.
     */
}

/* Task switched out of CPU */
static void put_prev_task_iot(struct rq *rq, struct task_struct *p) {
    /*
     * This function is called whenever a task is switched out of the
     * CPU. The function that will be running next is chosen by
     * pick_next_task(). You can choose to perform various upkeeping
     * and functions during the task switching process both here and
     * pick_next_task(), or all in pick_next_task(). Again, this will
     * be a decision based upon your scheduler class implementation.
     */
}

/* Give up CPU */
static void yield_task_iot(struct rq *rq) {
    /*
     * This function is called whenever a process voluntarily makes a call
     * to give up CPU. Specifically, this is done via system call by a 
     * user-space process.
     */
}

/* Scheduler class functions */
const struct sched_class iot_sched_class = {
        .next			    = &fair_sched_class,
        .enqueue_task	    = enqueue_task_iot,
        .dequeue_task	    = dequeue_task_iot,
        .yield_task		    = yield_task_iot,
        .check_preempt_curr	= check_preempt_curr_iot,
        .pick_next_task		= pick_next_task_iot,
        .put_prev_task		= put_prev_task_iot,
        .select_task_rq		= select_task_rq_iot,
        .set_cpus_allowed   = set_cpus_allowed_common,
        .rq_online          = rq_online_iot,
        .rq_offline         = rq_offline_iot,
        .task_woken		    = task_woken_iot,
        .switched_from		= switched_from_iot,
        .set_curr_task      = set_curr_task_iot,
        .task_tick		    = task_tick_iot,
        .prio_changed		= prio_changed_iot,
        .switched_to		= switched_to_iot,
        .update_curr		= update_curr_iot,
};