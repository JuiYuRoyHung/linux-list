#include <assert.h>
#include <stdlib.h>
#include "list.h"

#include "common.h"
#include "common_time_analysis.h"

static uint16_t values[256];

/**
 * split_half - split linked list into half
 * @head: pointer to the head of list to be split
 * @first_half: pointer to the head of list which reveive the first half
 * @second_half: pointer to the head of list which reveive the second half
 *
 * At first, use forward and backward stragegy to find the middle of the list.
 * Cut it by list_cut_position and use list_splice for trasfering
 * two half to first_half and second_half
 */
static void split_half(struct list_head *head,
                       struct list_head *first_half,
                       struct list_head *second_half)
{
    struct list_head *forward_move, *backward_move;

    forward_move = head->next;
    backward_move = head->prev;

    while (forward_move != backward_move) {
        forward_move = forward_move->next;

        // Even number of the list, so forward_move move back for equal cut.
        if (forward_move == backward_move) {
            forward_move = forward_move->prev;
            break;
        }

        backward_move = backward_move->prev;
    }

    list_cut_position(first_half, head, forward_move);
    list_splice(head, second_half);
}

/**
  * merge_compare - merge first_half & second_half and do sorting
  * @head: pointer to the head of list which receive sorted list
  * @first_half: pointer to the head of first half list to be merged
  * @second_half: pointer to the head of second half list to be merged
  *
  * merge the two lists and sort by entry's element
  */
static void merge_compare(struct list_head *head,
                          struct list_head *first_half,
                          struct list_head *second_half)
{
    struct listitem *first_element, *second_element;
    // After list_splice, must initialize head before use again
    INIT_LIST_HEAD(head);

    first_element = list_first_entry(first_half, struct listitem, list);
    second_element = list_first_entry(second_half, struct listitem, list);

    while (!list_empty(first_half) && !list_empty(second_half)) {
        if (cmpint(&first_element->i, &second_element->i) < 0) {
            list_move_tail(&first_element->list, head);
            first_element = list_first_entry(first_half, struct listitem, list);
        } else {
            list_move_tail(&second_element->list, head);
            second_element =
                list_first_entry(second_half, struct listitem, list);
        }
    }
    list_splice_tail(!list_empty(first_half) ? first_half : second_half, head);
}

/**
  * list_mergesort - using merge sort algorithm to sort the entry
  * @head: pointer to the head of list to be sorted
  *
  * Recursive method to implement the function
  * Need to check head is NULL, empty or singular(one node only)
  *
  */
static void list_mergesort(struct list_head *head)
{
    if (head == NULL || list_empty(head) || list_is_singular(head))
        return;

    struct list_head first_half, second_half;

    INIT_LIST_HEAD(&first_half);
    INIT_LIST_HEAD(&second_half);

    split_half(head, &first_half, &second_half);

    list_mergesort(&first_half);
    list_mergesort(&second_half);

    merge_compare(head, &first_half, &second_half);
}

int main(void)
{
    struct list_head testlist;
    struct listitem *item, *is = NULL;
    size_t i;
    struct timespec start, end;

    random_shuffle_array(values, (uint16_t) ARRAY_SIZE(values));

    INIT_LIST_HEAD(&testlist);

    assert(list_empty(&testlist));

    for (i = 0; i < ARRAY_SIZE(values); i++) {
        item = (struct listitem *) malloc(sizeof(*item));
        assert(item);
        item->i = values[i];
        list_add_tail(&item->list, &testlist);
    }

    assert(!list_empty(&testlist));

    qsort(values, ARRAY_SIZE(values), sizeof(values[0]), cmpint);

    clock_gettime(CLOCK_MONOTONIC, &start);
    list_mergesort(&testlist);
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("%lf\n", time_difference(&start, &end));

    i = 0;
    list_for_each_entry_safe (item, is, &testlist, list) {
        assert(item->i == values[i]);
        list_del(&item->list);
        free(item);
        i++;
    }

    assert(i == ARRAY_SIZE(values));
    assert(list_empty(&testlist));

    return 0;
}
