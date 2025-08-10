#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <numeric>
#include <omp.h>
#include <sstream>
#include <chrono>

using namespace std;

bool display = false;

bool subsetSum(vector<int> const& S, vector<int>& path, int n, vector<int>& subsum)
{
    int division = subsum.size();
    bool finish = true;
    for (int i = 0; i < division; i++)
    {
        if (subsum[i] != 0)
            finish = false;
    }
    if (finish == true)
        return true;

    if (n < 0)
        return false;

    for (int i = 0; i < division; i++)
    {
        bool element_fits = false;
        if (subsum[i] - S[n] >= 0)
        {
            path.push_back(i + 1);
            subsum[i] = subsum[i] - S[n];
            element_fits = subsetSum(S, path, n - 1, subsum);
            if (element_fits == false)
            {
                path.pop_back();
                subsum[i] = subsum[i] + S[n];
            }
            else
                return true;
        }
    }
    return false;
}

bool partition(vector<int> const& S, vector<int>& path, int division)
{
    int n = S.size();

    if (n < division) {
        return false;
    }

    int sum = accumulate(S.begin(), S.end(), 0);
    if (sum % division != 0)
        return false;
    vector<int> subsum;
    subsum.resize(division);
    for (int i = 0; i < division; i++)
        subsum[i] = sum / division;

    return subsetSum(S, path, n - 1, subsum);
}

bool Partition(vector<int> const& Set, int division)
{
    int siz = Set.size();
    vector<int> path;

    bool result = false;
    stringstream ss;
    if (division > 0 && partition(Set, path, division) == true)
    {
        if (display == true)
        {
            ss << "Partition into " << division << " subsets" << endl;
            ss << "Bruteforce Partition:" << endl;
        }
        for (int i = 0; i < division; i++)
        {
            if (display == true)
                ss << "Set " << i + 1 << ": ";
            for (int j = 0; j < path.size(); j++)
            {
                if (path[j] == i + 1)
                    ss << Set[siz - j - 1] << " ";
            }
            ss << endl;
        }
        ss << endl;
        result = true;
    }
    else
    {
        if (display == true)
        {
            ss << "Partition into " << division << " subsets" << endl;
            ss << "Bruteforce Partition:" << endl;
            ss << "Set cannot be partitioned" << endl;
            ss << endl;
        }
        result = false;
    }

    if (display == true)
    {
        #pragma omp critical
        {
            cout << ss.str();
            cout.flush();
        }
    }

    return result;
}



void scal(vector<int>& tab, int p1, int p2, int koniec, vector<int>& pom)
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

void mergesort(vector<int>& tab, int p, int koniec, vector<int>& pom)
{
    if (koniec - p < 2)
        return;
    int s = (p + koniec) / 2;
    mergesort(tab, p, s, pom);
    mergesort(tab, s, koniec, pom);
    scal(tab, p, s, koniec, pom);
}

bool ParallelGreedyPartition(vector<int>& Set, int division)
{
    vector<int> used;
    used.resize(Set.size());
    for (int i = 0; i < used.size(); i++)
        used[i] = 0;
    vector<int> pom;
    pom.resize(Set.size());
    mergesort(Set, 0, Set.size(), pom);

    int sum = 0;
    for (int i = 0; i < Set.size(); i++)
        sum = sum + Set[i];

    stringstream ss;
    bool possible_division = false;
    if (sum % division != 0)
    {
        if (display == true)
        {
            ss << "Partition into " << division << " subsets" << endl;
            ss << "Greedy Partition:" << endl;
            ss << "Set cannot be partitioned" << endl;
            ss << endl;
        }
    }
    else
    {
        int subsum = sum / division;
        vector<vector<int>> tab;
        tab.resize(subsum + 1);
        for (int i = 0; i < subsum + 1; i++)
            tab[i].resize(Set.size() + 1);

        for (int d = 0; d < division - 1; d++)
        {
            for (int i = 0; i < Set.size() + 1; i++)
                tab[0][i] = 1;
            for (int i = 1; i < subsum + 1; i++)
            {
                for (int j = 0; j < Set.size() + 1; j++)
                    tab[i][j] = 0;
            }
            for (int i = 1; i < subsum + 1; i++)
            {
                for (int j = 1; j < Set.size() + 1; j++)
                {
                    tab[i][j] = tab[i][j - 1];
                    if (tab[i][j] == 0 && i >= Set[j - 1] && used[j - 1] == 0)
                        tab[i][j] = tab[i - Set[j - 1]][j - 1];
                }
            }
            int k = subsum;
            possible_division = false;
            while (k > 0)
            {
                for (int j = 1; j < Set.size() + 1; j++)
                {
                    if (tab[k][j] == 1)
                    {
                        used[j - 1] = d + 1;
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
        
        if (display == true)
        {
            ss << "Partition into " << division << " subsets" << endl;

            if (possible_division == false)
            {
                ss << "Greedy Partition:" << endl;
                ss << "Set cannot be partitioned" << endl;
            }
            else
            {
                ss << "Greedy Partition:" << endl;
                for (int d = 0; d < division; d++)
                {
                    ss << "Set " << d + 1 << ": ";
                    for (int i = 0; i < used.size(); i++)
                    {
                        if (used[i] == d)
                            ss << Set[i] << " ";
                    }
                    ss << endl;
                }
            }
            ss << endl;
        }
    }

    if (display == true)
    {
        #pragma omp critical
        {
            cout << ss.str();
            cout.flush();
        }
    }


    if (possible_division == false)
        return false;
    else
        return true;
}

int main()
{
    bool own_set;
    char command = 'a';
    cout << "Do you want to test your own set or test random sets (o or r)?" << endl;
    while (command != 'o' && command != 'r')
    {
        cin >> command;
        if (command == 'o')
            own_set = true;
        else if (command == 'r')
            own_set = false;
        else
            cout << "Wrong command" << endl;
    }

    command = 'a';
    cout << "Do you want to display tested sets (y or n)?" << endl;
    while (command != 'y' && command != 'n')
    {
        cin >> command;
        if (command == 'y')
            display = true;
        else if (command == 'n')
            display = false;
        else
            cout << "Wrong command" << endl;
    }


    if (own_set == false)
    {
        bool specific_seed;
        command = 'a';
        cout << "Do you want to use your own seed (y or n)?" << endl;
        while (command != 'y' && command != 'n')
        {
            cin >> command;
            if (command == 'y')
                specific_seed = true;
            else if (command == 'n')
                specific_seed = false;
            else
                cout << "Wrong command" << endl;
        }
        if(specific_seed == false)
            srand(time(NULL));
        else
        {
            int seed;
            cout << "What is the value of seed?" << endl;
            cin >> seed;
            srand(seed);
        }
        int test_number;
        cout << "How many tests?" << endl;
        cin >> test_number;
        int elements_number;
        cout << "How many elements in set?" << endl;
        cin >> elements_number;
        int max_value;
        cout << "Maximum value of element?" << endl;
        cin >> max_value;
        int division;
        cout << "Up to how many subsets?" << endl;
        cin >> division;
        bool comparison;
        command = 'a';
        cout << "Do you want to confirm correctness of Greedy Partition (y or n)?" << endl;
        while (command != 'y' && command != 'n')
        {
            cin >> command;
            if (command == 'y')
                comparison = true;
            else if (command == 'n')
                comparison = false;
            else
                cout << "Wrong command" << endl;
        }
        cout << endl;

        vector<int> Set;
        Set.resize(elements_number);
        vector<int> sum_of_correct;
        sum_of_correct.resize(division);

        vector<double> greedy_partition_time;
        greedy_partition_time.resize(test_number);
        for (int j = 0; j < test_number; j++)
        {
            if (display == true)
            {
                cout << "Test: " << j + 1 << endl;
                cout << "Set: ";
            }
            for (int i = 0; i < elements_number; i++)
            {
                Set[i] = rand() % max_value + 1;
                if (display == true)
                    cout << Set[i] << " ";
            }
            if (display == true)
            {
                cout << endl;
                cout << endl;
                cout << endl;
            }


            omp_set_num_threads(division - 1);
            auto start = std::chrono::high_resolution_clock::now();
            #pragma omp parallel
            {
                int id = omp_get_thread_num() + 2;
                int d = id;

                vector<int> Set_copy = Set;
                bool possible_Partition;
                if (comparison == true)
                    possible_Partition = Partition(Set_copy, d);
                bool possible_GreedyPartition = ParallelGreedyPartition(Set_copy, d);
                if (comparison == true)
                    if (possible_GreedyPartition == possible_Partition)
                        sum_of_correct[d - 2]++;
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            greedy_partition_time[j] = elapsed.count();

            if (display == true)
            {
                cout << endl;
                cout << endl;
                cout << endl;
                cout << endl;
            }
        }
        if (comparison == true)
        {
            cout << endl;
            cout << "Correctness of Greedy Partition" << endl;
            for (int d = 2; d <= division; d++)
            {
                float percentage_of_correct = float(sum_of_correct[d - 2]) / float(test_number);
                cout << "Percentage of correct partitions when dividing into " << d << " subsets: " << percentage_of_correct * 100 << endl;
            }
            cout << endl;
            cout << endl;
            cout << endl;
            cout << endl;
        }

        /*
        cout << "Time: " << endl;
        for (int i = 0; i < greedy_partition_time.size(); i++)
            cout << i << ": " << greedy_partition_time[i] << endl;
        */

        double greedy_partition_time_sum = 0;
        for (int i = 0; i < greedy_partition_time.size(); i++)
            greedy_partition_time_sum += greedy_partition_time[i];
        cout << "time_sum: " << greedy_partition_time_sum << endl;
    }
    else
    {
        vector<int> Set;
        int elements_number;
        cout << "How many elements in set?" << endl;
        cin >> elements_number;
        Set.resize(elements_number);
        cout << "Enter set" << endl;
        for (int i = 0; i < elements_number; i++)
        {
            cin >> Set[i];
        }
        int division;
        cout << "Up to how many subsets?" << endl;
        cin >> division;

        bool comparison;
        command = 'a';
        cout << "Do you want to confirm correctness of Greedy Partition (y or n)?" << endl;
        while (command != 'y' && command != 'n')
        {
            cin >> command;
            if (command == 'y')
                comparison = true;
            else if (command == 'n')
                comparison = false;
            else
                cout << "Wrong command" << endl;
        }
        if (display == true)
        {
            cout << "Set: ";
            for (int i = 0; i < elements_number; i++)
                cout << Set[i] << " ";
            cout << endl;
            cout << endl;
            cout << endl;
        }

        vector<int> sum_of_correct;
        sum_of_correct.resize(division - 1);
        omp_set_num_threads(division - 1);
        #pragma omp parallel
        {
            int id = omp_get_thread_num() + 2;
            int d = id;

            vector<int> Set_copy = Set;
            bool possible_Partition;
            if (comparison == true)
                possible_Partition = Partition(Set_copy, d);
            bool possible_GreedyPartition = ParallelGreedyPartition(Set_copy, d);
            if (comparison == true)
                if (possible_GreedyPartition == possible_Partition)
                    sum_of_correct[d - 2]++;
        }


        if (comparison == true)
        {
            for (int i = 0; i < sum_of_correct.size(); i++)
            {
                if (sum_of_correct[i] == 1)
                    cout << "Partition into " << i + 2 << " subsets was correct" << endl;
                else
                    cout << "Partition into " << i + 2 << " subsets was incorrect" << endl;
            }
        }

        cout << endl;
        cout << endl;
        cout << endl;
        cout << endl;
    }
}
///7 3 2 1 5 4 8 1 8 5 7 2 4 3

//1 1 2 2 2 3 3 4 //8 4 2 1 5 6 4 5 6 4 1 8 6 8 1 4 2 8 1 6





/*
#include <iostream>
#include <omp.h>

int main() {
    // Ustawienie liczby w¹tków
    omp_set_num_threads(4);

    // Dyrektywa parallel do równoleg³ego wykonania bloku kodu
#pragma omp parallel
    {
        int id = omp_get_thread_num();
        std::cout << "Hello from thread " << id << std::endl;
    }

    return 0;
}
*/