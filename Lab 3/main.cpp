#include <iostream>
#include <pthread.h>
#include <stdint.h>

using namespace std;

class Q1
{
public:
    inline static int N;
    inline static int numberOfThread = 10;
    inline static int range;
    inline static int* arr;
    inline static pthread_mutex_t myMutex;
    inline static int globalCountEven = 0;

    static void* f(void* arg)
    {
        int id = (intptr_t)arg;

        int start = id * range;
        int endThread = (id == numberOfThread - 1) ? N : (id + 1) * range;

        int localEven = 0;

        for (int i = start; i < endThread; i++)
            if (arr[i] % 2 == 0)
                localEven++;

        pthread_mutex_lock(&myMutex);
        globalCountEven += localEven;
        pthread_mutex_unlock(&myMutex);

        pthread_exit(NULL);
        return NULL;
    }

    static void run()
    {
        cout << "Entre N: ";
        cin >> N;

        range = N / numberOfThread;
        arr = new int[N];

        for (int i = 0; i < N; i++)
            arr[i] = rand() % 10;

        pthread_mutex_init(&myMutex, NULL);

        pthread_t threads[numberOfThread];

        for (int i = 0; i < numberOfThread; i++)
            pthread_create(&threads[i], NULL, f, (void*)(intptr_t)i);

        for (int i = 0; i < numberOfThread; i++)
            pthread_join(threads[i], NULL);

        for (int i = 0; i < N; i++)
            cout << arr[i] << " ";

        cout << endl << "Total Even Count " << globalCountEven << endl;
        cout << "Total Odd Count " << N - globalCountEven << endl;

        delete[] arr;
    }
};

class Q2
{
public:
    inline static int N;
    inline static int numberOfThread = 10;
    inline static int range;
    inline static int* arr;
    inline static int* evenArray;
    inline static int* oddArray;
    inline static pthread_mutex_t myMutex;

    static void* f(void* arg)
    {
        int id = (intptr_t)arg;
        int start = id * range;
        int endThread = (id == numberOfThread - 1) ? N : (id + 1) * range;

        for (int i = start; i < endThread; i++)
        {
            int e = arr[i];
            int localEven = 0;
            int localOdd = 0;
            int localSumEven = 0;
            int localSumOdd = 0;

            while (e != 0)
            {
                int temp = e % 10;
                if (temp % 2 == 0)
                {
                    localEven++;
                    localSumEven += temp;
                }
                else
                {
                    localOdd++;
                    localSumOdd += temp;
                }
                e /= 10;
            }

            evenArray[i] = localEven;
            oddArray[i] = localOdd;

            pthread_mutex_lock(&myMutex);
            evenArray[N] += localSumEven;
            oddArray[N] += localSumOdd;
            pthread_mutex_unlock(&myMutex);
        }

        pthread_exit(NULL);
        return NULL;
    }

    static void run()
    {
        cout << "Entre N: ";
        cin >> N;

        range = N / numberOfThread;

        arr = new int[N];
        evenArray = new int[N + 1];
        oddArray = new int[N + 1];

        evenArray[N] = 0;
        oddArray[N] = 0;

        for (int i = 0; i < N; i++)
            arr[i] = rand() % 10000;

        pthread_mutex_init(&myMutex, NULL);

        pthread_t threads[numberOfThread];

        for (int i = 0; i < numberOfThread; i++)
            pthread_create(&threads[i], NULL, f, (void*)(intptr_t)i);

        for (int i = 0; i < numberOfThread; i++)
            pthread_join(threads[i], NULL);

        cout << "Array: ";
        for (int i = 0; i < N; i++)
            cout << arr[i] << " ";

        cout << endl << "Even Array: ";
        for (int i = 0; i <= N; i++)
            cout << evenArray[i] << " ";

        cout << endl << "Odd Array: ";
        for (int i = 0; i <= N; i++)
            cout << oddArray[i] << " ";

        delete[] arr;
        delete[] evenArray;
        delete[] oddArray;
    }
};

class Q4
{
public:
    struct Data
    {
        int n1, n2;
    };

    inline static int N;
    inline static int numberOfThread = 10;
    inline static int range;
    inline static pthread_mutex_t myMutex;
    inline static Data* arr;

    static int GCD_Fun(int a, int b)
    {
        if (b == 0)
            return a;
        return GCD_Fun(b, a % b);
    }

    static void* f(void* arg)
    {
        int id = (intptr_t)arg;
        int start = id * range;
        int endThread = (id == numberOfThread - 1) ? N : (id + 1) * range;

        for (int i = start; i < endThread; i++)
        {
            pthread_mutex_lock(&myMutex);
            cout << "( " << arr[i].n1 << " , " << arr[i].n2 << " ) => "
                 << GCD_Fun(arr[i].n1, arr[i].n2) << endl;
            pthread_mutex_unlock(&myMutex);
        }

        pthread_exit(NULL);
        return NULL;
    }

    static void run()
    {
        cout << "Entre N: ";
        cin >> N;

        range = N / numberOfThread;

        arr = new Data[N];

        for (int i = 0; i < N; i++)
        {
            arr[i].n1 = rand() % 10 + 2;
            arr[i].n2 = rand() % 10 + 2;
        }

        pthread_mutex_init(&myMutex, NULL);

        pthread_t threads[numberOfThread];

        for (int i = 0; i < numberOfThread; i++)
            pthread_create(&threads[i], NULL, f, (void*)(intptr_t)i);

        for (int i = 0; i < numberOfThread; i++)
            pthread_join(threads[i], NULL);

        delete[] arr;
    }
};

int main()
{
    Q1::run();
    Q2::run();
    Q4::run();
    
    return 0;
}
