#include <iostream>
#include <pthread.h>
#include <stdint.h>

using namespace std;

class Q1andQ2
{
public:
    inline static int N = 0;
    inline static int numberOfThread = 7;
    inline static long globalSum = 0;
    inline static pthread_mutex_t myMutex;

    struct ThreadData
    {
        int start, endThread;
    };

    static void* f(void* arg)
    {
        ThreadData* data = (ThreadData*)arg;

        long localSum = 0;
        for (int i = data->start; i <= data->endThread; i++)
            localSum += i;

        pthread_mutex_lock(&myMutex);
        globalSum += localSum;
        pthread_mutex_unlock(&myMutex);

        pthread_exit(NULL);
        return NULL;
    }

    static void run()
    {
        cout << "Enter N: ";
        cin >> N;

        int range = N / numberOfThread;

        pthread_mutex_init(&myMutex, NULL);

        pthread_t threads[numberOfThread];
        ThreadData data[numberOfThread];

        for (int i = 0; i < numberOfThread; i++)
        {
            data[i].start = i * range + 1;
            data[i].endThread = (i == numberOfThread - 1) ? N : (i + 1) * range;

            pthread_create(&threads[i], NULL, f, &data[i]);
        }

        for (int i = 0; i < numberOfThread; i++)
            pthread_join(threads[i], NULL);

        cout << endl << "Sum => " << globalSum << endl;
    }
};


class Q4
{
public:
    inline static int N = 2;
    inline static int numberOfThread = 2;
    inline static int range = N / numberOfThread;
    inline static int* vec;
    inline static int* result;
    inline static int matrix[20][20];
    inline static pthread_mutex_t myMutex;

    static void* f(void* arg)
    {
        int id = (intptr_t)arg;

        int start = id * range;
        int endThread = (id == numberOfThread - 1) ? N : (id + 1) * range;

        for (int i = start; i < endThread; i++)
        {
            int localSum = 0;

            for (int j = 0; j < N; j++)
                localSum += matrix[i][j] * vec[j];

            result[i] = localSum;
        }

        pthread_exit(NULL);
        return NULL;
    }

    static void run()
    {
        range = N / numberOfThread;

        vec = new int[N];
        result = new int[N];

        vec[0] = 1;
        vec[1] = 2;

        matrix[0][0] = 1;
        matrix[0][1] = 2;
        matrix[1][0] = 3;
        matrix[1][1] = 4;

        pthread_mutex_init(&myMutex, NULL);

        pthread_t threads[numberOfThread];

        for (int i = 0; i < numberOfThread; i++)
            pthread_create(&threads[i], NULL, f, (void*)(intptr_t)i);

        for (int i = 0; i < numberOfThread; i++)
            pthread_join(threads[i], NULL);

        for (int i = 0; i < N; i++)
            cout << result[i] << " ";

        delete[] vec;
        delete[] result;
    }
};



int main()
{
    Q1andQ2::run();
    Q4::run();
    return 0;
}
