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

int main()
{
    Q1::run();
    return 0;
}
