#include <iostream>
#include <vector>
#include <omp.h>     // OpenMP for parallelization
#include <ctime>     // For measuring time

using namespace std;

// Parallel Bubble Sort using OpenMP
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped = true;

    // Keep running until no swaps are done in a pass
    while (swapped) {
        swapped = false;

        // Parallel for loop to compare and swap adjacent elements
        #pragma omp parallel for
        for (int i = 0; i < n - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]); // Swap if elements are in wrong order
                swapped = true;           // Set flag if any swap happens
            }
        }
    }
}

// Merge function to combine two sorted halves
void merge(vector<int>& arr, int l, int m, int r) {
    vector<int> temp;    // Temporary array to store merged elements
    int left = l;        // Starting index of left subarray
    int right = m + 1;   // Starting index of right subarray

    // Merge the two halves
    while (left <= m && right <= r) {
        if (arr[left] <= arr[right]) {
            temp.push_back(arr[left]);
            left++;
        } else {
            temp.push_back(arr[right]);
            right++;
        }
    }

    // Copy remaining elements of left subarray
    while (left <= m) {
        temp.push_back(arr[left]);
        left++;
    }

    // Copy remaining elements of right subarray
    while (right <= r) {
        temp.push_back(arr[right]);
        right++;
    }

    // Copy merged elements back to original array
    for (int i = l; i <= r; i++) {
        arr[i] = temp[i - l];
    }
}

// Parallel Merge Sort using OpenMP sections
void mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2; // Find the middle point

        // Parallelize the recursive calls using sections
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSort(arr, l, m);       // Sort the first half

            #pragma omp section
            mergeSort(arr, m + 1, r);   // Sort the second half
        }

        // Merge the two halves
        merge(arr, l, m, r);
    }
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter the elements: ";
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    // Measure time for Bubble Sort
    clock_t bubbleStart = clock();
    bubbleSort(arr);
    clock_t bubbleEnd = clock();

    // Display sorted array from Bubble Sort
    cout << "Sorted array using Bubble Sort: ";
    for (int num : arr)
        cout << num << " ";
    cout << endl;

    // Measure time for Merge Sort
    clock_t mergeStart = clock();
    mergeSort(arr, 0, n - 1);
    clock_t mergeEnd = clock();

    // Display sorted array from Merge Sort
    cout << "Sorted array using Merge Sort: ";
    for (int num : arr)
        cout << num << " ";
    cout << endl;

    // Calculate and display execution times
    double bubbleDuration = double(bubbleEnd - bubbleStart) / CLOCKS_PER_SEC;
    double mergeDuration = double(mergeEnd - mergeStart) / CLOCKS_PER_SEC;

    cout << "Bubble sort time in seconds: " << bubbleDuration << endl;
    cout << "Merge sort time in seconds: " << mergeDuration << endl;

    return 0;
}
/*
How to compile and run:
---------------------------------------
g++ -fopenmp MergeBubbleSort.cpp -o merge
./merge

Sample Input:
---------------------------------------
Enter the number of elements: 5
Enter the elements: 5 2 9 1 5

Expected Output:
---------------------------------------
Sorted array using Merge Sort: 1 2 5 5 9
Merge sort time in seconds: (some small number)
*/
