/**
Author: Dan Fodor
Description: Sorting algorithm (given an array of numbers, return it in increasing order).

Solution: The main idea is to make use of
increasing and decreasing sequences in the given array.
There are 2 phases:
Phase 1. Read and split
In this phase, while reading the array, we also keep track of maximal increasing and
decreasing sequences. (also, after getting the sequences we revert the decreasing ones)
Phase 2. Merge
At this step, we make use of the sequences and merge them as merge sort does.

For example, given the array:
{1, 5, 7, 10, 16, 21, 20, 18, 15, 14, 3, 4, 8, 9, 11, 12, 25, 24, 23, 19, 6, 2, 0}

Phase 1.
We can split in 4 sequences:
{1, 5, 7, 10, 16, 21}, {20, 18, 15, 14}, {3, 4, 8, 9, 11, 12, 25}, and {24, 23, 19, 6, 2, 0}
We also reverse the ones that are decreasing
{1, 5, 7, 10, 16, 21}, {14, 15, 18, 20}, {3, 4, 8, 9, 11, 12, 25}, {0, 2, 6, 19, 23, 24}

Phase 2.
We merge them 2 by 2:
Step 0:
{1, 5, 7, 10, 16, 21}, {14, 15, 18, 20}, {3, 4, 8, 9, 11, 12, 25}, {0, 2, 6, 19, 23, 24}
Step 1:
{1, 5, 7, 10, 14, 15, 16, 18, 20, 21}, {0, 2, 3, 4, 6, 8, 9, 11, 12, 19, 23, 24, 25}
Step 2:
{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 18, 19, 20, 21, 23, 24, 25}

Difference from merge sort: it stars the merge sort from increasing arrays of size that may be different than 0.

Trade-off: Compared to merge sort, it requires extra processing while reading, but it may be much faster in the merge
phase for some data sets (when there are not too many sequences in one array)
*/

#include <cstdio>
#include <queue>

using namespace std;

struct Bounds {
    int lower, upper;
};

int a[2][500012];
int n;

FILE *inFile;
FILE *outFile;


void reverse_between(int *v, int lower, int upper)
{
    int mid = (lower + (upper - lower + 1) / 2);
    for (int j = lower; j < mid; ++j)
    {
        swap(v[j], v[lower + upper - j]);
    }
}

void read_and_split(int *v, int &n, queue <Bounds> &seq_queue)
{
    inFile = fopen ("algsort.in","r");

    int current, next, previous;
    bool increasing, changed = false;

    Bounds seq_bounds;
    seq_bounds.lower = 0;
    seq_bounds.upper = 0;


    fscanf(inFile, "%d", &n);
    fscanf(inFile, "%d %d", &current, &next);

    v[0] = current;
    v[1] = next;

    increasing = (current <= next);

    previous = next;
    seq_bounds.upper = 1;

    for (int i = 2; i < n; i++)
    {
        fscanf(inFile, "%d", &current);
        v[i] = current;

        if (changed == true)
        {
            increasing = (previous < current);
            changed = false;
        }

        if ((previous < current) == increasing)
        {
            ++seq_bounds.upper;
        }
        else
        {
            seq_queue.push(seq_bounds);

            if (increasing == false)
            {
                reverse_between(v, seq_bounds.lower, seq_bounds.upper);
            }

            changed = true;

            seq_bounds.lower = i;
            seq_bounds.upper = i;
        }

        previous = current;
    }

    seq_queue.push(seq_bounds);

    if (increasing == false)
    {
        reverse_between(v, seq_bounds.lower, n-1);
    }

    fclose(inFile);
}

void merging(int v[][500012], int n, queue <Bounds> seq_queue , int &merging_stage)
{
    int len = seq_queue.size();
    int take = (len + 1) / 2;

    bool even = (len % 2 == 0);


    while (len != 1)
    {
        if (even == false)
            --take;

        for (int i = 0; i < take; ++i)
        {
            Bounds first_bounds, second_bounds;

            first_bounds = seq_queue.front();
            seq_queue.pop();

            second_bounds = seq_queue.front();
            seq_queue.pop();

            Bounds new_seq;
            new_seq.lower = first_bounds.lower;
            new_seq.upper = second_bounds.upper;
            seq_queue.push(new_seq);

            int first_lower = first_bounds.lower;
            int first_upper = first_bounds.upper;

            int second_lower = second_bounds.lower;
            int second_upper = second_bounds.upper;

            int new_lower = first_lower;
            int new_upper = second_upper;

            for (int l = new_lower; l <= new_upper; ++l)
            {
                if (first_lower <= first_upper && second_lower <= second_upper)
                {
                    if (v[merging_stage % 2][first_lower] < v[merging_stage % 2][second_lower])
                    {
                        v[(merging_stage + 1) % 2][l] = v[merging_stage % 2][first_lower];
                        ++first_lower;
                    }
                    else
                    {
                        v[(merging_stage + 1) % 2][l] = v[merging_stage % 2][second_lower];
                        ++second_lower;
                    }
                }
                else
                {
                    if (first_lower > first_upper)
                    {
                        while (l <= new_upper)
                        {
                            v[(merging_stage + 1) % 2][l] = v[merging_stage % 2][second_lower];
                            ++second_lower;
                            ++l;
                        }
                    }
                    else
                    {
                        while (l <= new_upper)
                        {
                            v[(merging_stage + 1) % 2][l] = v[merging_stage % 2][first_lower];
                            ++first_lower;
                            ++l;
                        }
                    }
                }
            }
        }

        if (even == false)
        {
            Bounds remaining_seq = seq_queue.front();
            int lower_bound, upper_bound;
            lower_bound = remaining_seq.lower;
            upper_bound = remaining_seq.upper;
            for (lower_bound; lower_bound <= upper_bound; ++lower_bound)
                a[(merging_stage + 1) % 2][lower_bound] = a[merging_stage % 2][lower_bound];
            seq_queue.pop();
            seq_queue.push(remaining_seq);
        }

        len = seq_queue.size();
        take = (len + 1) / 2;

        even = (len % 2 == 0);

        ++merging_stage;
    }
}

void print_result(int *v, int n)
{
    outFile = fopen ("algsort.out", "w+");

    for (int i = 0; i < n; ++i)
        fprintf(outFile, "%d ", v[i]);
    fprintf(outFile, "\n");

    fclose(outFile);

}

int main()
{
    queue <Bounds> seq_queue;
    int merging_stage = 0;

    read_and_split(a[0], n, seq_queue);

    merging(a, n, seq_queue, merging_stage);

    print_result(a[merging_stage % 2], n);

    return 0;
}
