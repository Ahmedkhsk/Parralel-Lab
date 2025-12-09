#include <iostream>
#include <pthread.h>

using namespace std;
int N;
int numberOfThread = 7;
int range = N / numberOfThread;
pthread_mutex_t myMutex;
long globalSum = 0;

struct ThreadData
{
    int start,endThread;
};

void* f (void* arg)
{
    ThreadData* data = (ThreadData*) arg;
    //int start = id*range+1;
    //int end =  (id==numberOfThread-1) ? N : (id+1)*range;

    long localSum = 0;

    for(int i = data->start ; i <= data->endThread ; i++)
        localSum += i;

    pthread_mutex_lock(&myMutex);
    globalSum += localSum;
    pthread_mutex_unlock(&myMutex);

}

int main()
{
    cout << "Entre N: ";
    cin >> N;

    pthread_mutex_init(&myMutex,NULL);
    pthread_t threads[numberOfThread];
    ThreadData data[numberOfThread];

    for(int i = 0 ; i < numberOfThread ; i++)
    {
        data[i].start = i*range+1;
        data[i].endThread = (i==numberOfThread) ? N : (range*i)+1;
        pthread_create(&threads[i],NULL,f,&data[i]);
    }

    for(int i = 0 ; i < numberOfThread ; i++)
        pthread_join(threads[i],NULL);

    cout<<endl<<"Sum => "<<globalSum;

    return 0;
}
