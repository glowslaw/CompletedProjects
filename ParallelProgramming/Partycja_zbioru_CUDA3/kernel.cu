#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <iostream>
#include <chrono>

using namespace std;

void scal(int* tab, int p1, int p2, int koniec, int* pom)
{
    int i = p1;
    int j = p2;
    int k = 0;
    while (i != p2 && j != koniec)
    {
        if (tab[i] > tab[j])
        {
            pom[k] = tab[i];
            i++;
            k++;
        }
        else
        {
            pom[k] = tab[j];
            j++;
            k++;
        }
    }
    while (i != p2)
    {
        pom[k] = tab[i];
        i++;
        k++;
    }
    while (j != koniec)
    {
        pom[k] = tab[j];
        j++;
        k++;
    }
    for (int a = 0; a < koniec - p1; a++)
        tab[a + p1] = pom[a];
}

void mergesort(int* tab, int p, int koniec, int* pom)
{
    if (koniec - p < 2)
        return;
    int s = (p + koniec) / 2;
    mergesort(tab, p, s, pom);
    mergesort(tab, s, koniec, pom);
    scal(tab, p, s, koniec, pom);
}

cudaError_t addWithCuda(int* c1, int* c2, int* c3, int* c4, int* c5, int* c6, int* c7, int* c8, int* c9, int* size_of_set, const int* a, const int* b, unsigned int size);

__global__ void ParallelGreedyPartition(int* used2, int* used3, int* used4, int* used5, int* used6, int* used7, int* used8, int* used9, int* used10, int* tab_size_of_set, const int* Set, const int* tab_division)
{
    int id = threadIdx.x;
    int set_size = tab_size_of_set[0];
    int upto_division = tab_division[0];
    int division = id + 2;
    //if (id + 2 <= upto_division)
    //    division = id + 2;
    //else
    //    division = 999999;

    int** used = new int* [9];
    for (int i = 0; i < 9; i++) {
        used[i] = new int[set_size];
    }
    used[0] = used2;
    used[1] = used3;
    used[2] = used4;
    used[3] = used5;
    used[4] = used6;
    used[5] = used7;
    used[6] = used8;
    used[7] = used9;
    used[8] = used10;


    if (division <= upto_division)
    {
        int sum = 0;
        for (int i = 0; i < set_size; i++)
            sum = sum + Set[i];

        bool possible_division = false;

        if (sum % division == 0)
        {
            int subsum = sum / division;

            int** tab = new int* [subsum + 1];
            for (int i = 0; i < subsum + 1; i++) {
                tab[i] = new int[set_size + 1];
            }

            for (int d = 0; d < division - 1; d++)
            {
                for (int i = 0; i < set_size + 1; i++)
                    tab[0][i] = 1;
                for (int i = 1; i < subsum + 1; i++)
                {
                    for (int j = 0; j < set_size + 1; j++)
                        tab[i][j] = 0;
                }
                for (int i = 1; i < subsum + 1; i++)
                {
                    for (int j = 1; j < set_size + 1; j++)
                    {
                        tab[i][j] = tab[i][j - 1];
                        if (tab[i][j] == 0 && i >= Set[j - 1] && used[id][j - 1] == 0)
                            tab[i][j] = tab[i - Set[j - 1]][j - 1];
                    }
                }
                int k = subsum;
                possible_division = false;
                while (k > 0)
                {
                    for (int j = 1; j < set_size + 1; j++)
                    {
                        if (tab[k][j] == 1)
                        {
                            used[id][j - 1] = d + 1;
                            possible_division = true;
                            k = k - Set[j - 1];
                            break;
                        }
                    }
                    if (possible_division == false)
                        break;
                }
                if (possible_division == false)
                    break;
            }
        }


        if (possible_division == false)
        {
            for (int i = 0; i < set_size; i++)
                used[id][i] = -1;
        }
    }
}

int main()
{
    int const number_of_tests = 1000;//2;//1000;
    int const size_of_set = 100;//10;//100
    int const max_value = 30;//4;//30
    int const seed = 1;//2;//1;
    int const division = 10;
    int Set[size_of_set];
    int pom[size_of_set];
    bool display = false;
    double greedy_partition_time[number_of_tests];

    int tab_size_of_set[size_of_set] = { size_of_set };
    int tab_division[size_of_set] = { division };

    int a[size_of_set] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    int b[size_of_set] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 };

    int used2[size_of_set] = { 0 };
    int used3[size_of_set] = { 0 };
    int used4[size_of_set] = { 0 };
    int used5[size_of_set] = { 0 };
    int used6[size_of_set] = { 0 };
    int used7[size_of_set] = { 0 };
    int used8[size_of_set] = { 0 };
    int used9[size_of_set] = { 0 };
    int used10[size_of_set] = { 0 };

    int** used = new int* [9];
    for (int i = 0; i < 9; i++) {
        used[i] = new int[size_of_set];
    }
    used[0] = used2;
    used[1] = used3;
    used[2] = used4;
    used[3] = used5;
    used[4] = used6;
    used[5] = used7;
    used[6] = used8;
    used[7] = used9;
    used[8] = used10;

    srand(seed);


    for (int i = 0; i < number_of_tests; i++)
    {
        for (int k = 0; k < size_of_set; k++)
            Set[k] = rand() % max_value + 1;

        if (display == true)
        {
            cout << "Set"<< i + 1 << ": ";
            for (int k = 0; k < size_of_set; k++)
                cout << Set[k] << " ";
            cout << endl;
            cout << endl;
        }

        auto start = std::chrono::high_resolution_clock::now();

        mergesort(Set, 0, size_of_set, pom);
        cudaError_t cudaStatus = addWithCuda(used2, used3, used4, used5, used6, used7, used8, used9, used10, tab_size_of_set, Set, tab_division, size_of_set);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        greedy_partition_time[i] = elapsed.count();


        //if (cudaStatus != cudaSuccess) {
        //    fprintf(stderr, "addWithCuda failed!");
        //     return 1;
        //}
        if (i == number_of_tests - 1)
        {
            cudaStatus = cudaDeviceReset();
            if (cudaStatus != cudaSuccess) {
                fprintf(stderr, "cudaDeviceReset failed!");
                return 1;
            }
        }

        if (display == true)
        {
            cout << "Sorted_Set: ";
            for (int k = 0; k < size_of_set; k++)
                cout << Set[k] << " ";
            cout << endl;
            cout << endl;
            cout << endl;

            for (int k = 0; k < division - 1; k++)
            {
                cout << "Parallel_Greedy_Partition into " << k + 2 << " subsets: " << endl;
                for (int d = 0; d < k + 2; d++)
                {
                    if (used[k][0] == -1)
                    {
                        cout << "Partition impossible" << endl;
                        break;
                    }
                    cout << "Subset " << d + 1 << ": ";
                    for (int j = 0; j < size_of_set; j++)
                    {
                        if (used[k][j] == d)
                            cout << Set[j] << " ";
                    }
                    cout << endl;
                }
                cout << endl;
            }
            cout << endl;
            cout << endl;
        }
    }

    double greedy_partition_time_sum = 0;
    for (int i = 0; i < number_of_tests; i++)
        greedy_partition_time_sum += greedy_partition_time[i];
    cout << "time_sum: " << greedy_partition_time_sum << endl;

    return 0;
}

cudaError_t addWithCuda(int* c1, int* c2, int* c3, int* c4, int* c5, int* c6, int* c7, int* c8, int* c9, int* size_of_set, const int* a, const int* b, unsigned int size)
{
    int* dev_a = 0;
    int* dev_b = 0;
    int* dev_c1 = 0;
    int* dev_c2 = 0;
    int* dev_c3 = 0;
    int* dev_c4 = 0;
    int* dev_c5 = 0;
    int* dev_c6 = 0;
    int* dev_c7 = 0;
    int* dev_c8 = 0;
    int* dev_c9 = 0;
    int* dev_size_of_set = 0;
    cudaError_t cudaStatus;

    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_c1, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_c2, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_c3, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_c4, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_c5, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_c6, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_c7, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_c8, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_c9, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_size_of_set, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_b, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_size_of_set, size_of_set, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    ParallelGreedyPartition << <1, size >> > (dev_c1, dev_c2, dev_c3, dev_c4, dev_c5, dev_c6, dev_c7, dev_c8, dev_c9, dev_size_of_set, dev_a, dev_b);

    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }

    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    cudaStatus = cudaMemcpy(c1, dev_c1, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(c2, dev_c2, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(c3, dev_c3, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(c4, dev_c4, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(c5, dev_c5, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(c6, dev_c6, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(c7, dev_c7, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(c8, dev_c8, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }
    
    cudaStatus = cudaMemcpy(c9, dev_c9, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

Error:
    cudaFree(dev_c1);
    cudaFree(dev_c2);
    cudaFree(dev_c3);
    cudaFree(dev_c4);
    cudaFree(dev_c5);
    cudaFree(dev_c6);
    cudaFree(dev_c7);
    cudaFree(dev_c8);
    cudaFree(dev_c9); 
    cudaFree(dev_size_of_set);
    cudaFree(dev_a);
    cudaFree(dev_b);

    return cudaStatus;
}

