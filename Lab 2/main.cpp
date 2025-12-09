#include <iostream>
#include <pthread.h>

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

int main()
{
    Q1andQ2::run();
    return 0;
}
