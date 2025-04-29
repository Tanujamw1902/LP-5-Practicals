// ParallelOperations.cpp
#include <iostream> // For input-output operations
#include <vector>   // For using dynamic array (vector)
#include <omp.h>    // For OpenMP parallelization
using namespace std;

// Function to find minimum value in the vector using parallelism
int parallelMin(vector<int> vec)
{
    int min_val = vec[0]; // Initialize with first element

// Parallelize the loop using OpenMP
#pragma omp parallel for
    for (int i = 1; i < vec.size(); i++)
    {
        if (vec[i] < min_val)
        {
            min_val = vec[i]; // Update min if smaller element is found
        }
    }
    return min_val;
}

// Function to find maximum value in the vector using parallelism
int parallelMax(vector<int> vec)
{
    int max_val = vec[0]; // Initialize with first element

// Parallelize the loop using OpenMP
#pragma omp parallel for
    for (int i = 1; i < vec.size(); i++)
    {
        if (vec[i] > max_val)
        {
            max_val = vec[i]; // Update max if larger element is found
        }
    }
    return max_val;
}

// Function to calculate sum of elements using parallelism
int parallelSum(vector<int> vec)
{
    int sum = 0;

// Parallelize the loop using OpenMP
#pragma omp parallel for
    for (int i = 0; i < vec.size(); i++)
    {
        sum += vec[i]; // Add each element to sum
    }
    return sum;
}

// Function to calculate average of elements
float parallelAverage(vector<int> vec)
{
    int sum = parallelSum(vec);          // Reuse parallelSum function
    float avg = float(sum) / vec.size(); // Calculate average
    return avg;
}

int main()
{
    int n; // Number of elements
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> vec(n); // Declare a vector of size n

    cout << "Enter the elements: ";
    for (int i = 0; i < n; ++i)
    {
        cin >> vec[i]; // Input elements
    }

    // Call and display each operation result
    int min_val = parallelMin(vec);
    cout << "Minimum value: " << min_val << endl;

    int max_val = parallelMax(vec);
    cout << "Maximum value: " << max_val << endl;

    int sum = parallelSum(vec);
    cout << "Sum of values: " << sum << endl;

    float avg = parallelAverage(vec);
    cout << "Average of values: " << avg << endl;

    return 0;
}
/*
How to compile and run:
---------------------------------------
g++ -fopenmp Aggregation.cpp -o aggre
./aggre

Sample Input:
---------------------------------------
Enter the number of elements: 5
Enter the elements: 4 8 2 10 6

Expected Output:
---------------------------------------
Minimum value: 2
Maximum value: 10
Sum of values: 30
Average of values: 6
*/