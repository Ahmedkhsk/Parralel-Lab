#include <iostream>
#include <omp.h>

using namespace std;

class Q2
{
public:
    inline static int n = 5;
    inline static int arr[5] = {1, 1, 1, 1, 1};
    inline static int globalSum = 0;

    static void run()
    {
        #pragma omp parallel for reduction(+:globalSum)
        for (int i = 0; i < n; i++)
            globalSum += arr[i];

        cout << globalSum;
    }
};

class Q4
{
public:
    inline static string* words;
    inline static char vowles[10];
    inline static int id = 0;
    inline static int n = 5;

    static bool isVowel(char c)
    {
        return (c == 'a' || c == 'u' || c == 'e' || c == 'i' || c == 'o');
    }

    static bool isContains(char c)
    {
        for (int i = 0; i < id; i++)
            if (c == vowles[i])
                return true;
        return false;
    }

    static int maxWordLength()
    {
        int maxEle = 0;
        for (int i = 0; i < n; i++)
            if (words[i].length() > maxEle)
                maxEle = words[i].length();
        return maxEle;
    }

    static void run()
    {
        n = 3;
        words = new string[n];

        words[0] = "banana";
        words[1] = "ahmed";
        words[2] = "disha";

        char matrix[20][20];

        for (int i = 0; i < n; i++)
            for (int j = 0; j < words[i].size(); j++)
                matrix[i][j] = words[i][j];

        #pragma omp parallel for
        for (int i = 0; i < n; i++)
        {
            double sum = 0;

            for (int j = 0; j < words[i].size(); j++)
            {
                if (isVowel(matrix[i][j]))
                {
                    sum++;

                    if (!isContains(matrix[i][j]))
                    {
                        #pragma omp critical
                        {
                            vowles[id] = matrix[i][j];
                            id++;
                        }
                    }
                }
            }

            #pragma omp critical
            {
                cout << words[i] << ": Avg => "
                     << (double)sum / words[i].size() << endl;
            }
        }

        delete[] words;
    }
};

int main()
{
    Q2::run();
    return 0;
}
