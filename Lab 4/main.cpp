#include <bits/stdc++.h>
#include <semaphore.h>
#include <unistd.h>
#include <windows.h>

using namespace std;

class Q1
{
private:
    static inline bool isFinshed = false;
    static inline bool isTobacco = false;
    static inline bool isPaper = false;
    static inline bool isMatches = false;
    static inline bool isTaken = true;
    static inline int counter = 0;
    static inline int sizeOfCigrate = 0;

public:
    static void setSize(int n)
    {
        sizeOfCigrate = n;
    }

    static void* agentFun(void* args)
    {
        while(!isFinshed)
        {
            if(isTaken)
            {
                int i = rand() % 3;

                if(i==0)
                {
                    cout<<"generate tobacco and paper"<<endl;
                    isPaper = true;
                    isTobacco = true;
                    isTaken = false;
                }
                else if(i == 1)
                {
                    cout<<"generate paper and matches"<<endl;
                    isPaper = true;
                    isMatches = true;
                    isTaken = false;
                }
                else
                {
                    cout<<"generate tobacco and matches"<<endl;
                    isTobacco = true;
                    isMatches = true;
                    isTaken = false;
                }
            }
        }
        return nullptr;
    }

    static void* tobacoSmoker(void* args)
    {
        while(!isFinshed)
        {
            if(isPaper && isMatches && !isTaken)
            {
                counter++;
                cout<<counter<<": collected paper and matches to make cigarette"<<endl;
                isPaper = false;
                isMatches = false;
                isTaken = true;

                if(counter == sizeOfCigrate)
                    isFinshed = true;
            }
        }
        return nullptr;
    }

    static void* paperSmoker(void* args)
    {
        while(!isFinshed)
        {
            if(isMatches && isTobacco && !isTaken)
            {
                counter++;
                cout<<counter<<": collected tobacco and matches to make cigarette"<<endl;
                isMatches = false;
                isTobacco = false;
                isTaken = true;

                if(counter == sizeOfCigrate)
                    isFinshed = true;
            }
        }
        return nullptr;
    }

    static void* matcheSmoker(void* args)
    {
        while(!isFinshed)
        {
            if(isPaper && isTobacco && !isTaken)
            {
                counter++;
                cout<<counter<<": collected paper and tobacco to make cigarette"<<endl;
                isPaper = false;
                isTobacco = false;
                isTaken = true;

                if(counter == sizeOfCigrate)
                    isFinshed = true;
            }
        }
        return nullptr;
    }

    static void run()
    {
        pthread_t smoker1, smoker2, smoker3, agent;

        pthread_create(&agent, NULL, agentFun, NULL);
        pthread_create(&smoker1, NULL, tobacoSmoker, NULL);
        pthread_create(&smoker2, NULL, paperSmoker, NULL);
        pthread_create(&smoker3, NULL, matcheSmoker, NULL);

        pthread_join(smoker1, NULL);
        pthread_join(smoker2, NULL);
        pthread_join(smoker3, NULL);
        pthread_join(agent, NULL);
    }
};

class Q2
{
private:
    static inline sem_t VegetableSem, BeefSem, BreadSem, AssemblerSem;
    static inline bool isFinished = false;
    static inline int sizeOfHamburger = 0;
    static inline int counter = 0;

public:
    static void setSize(int n)
    {
        sizeOfHamburger = n;
    }

    static void* VegetableFun(void* arg)
    {
        while(!isFinished)
        {
            sem_wait(&VegetableSem);

            if(isFinished)
                break;

            cout<<"vegetable can be used"<<endl;
            sem_post(&AssemblerSem);
        }
        return nullptr;
    }

    static void* BeefFun(void* arg)
    {
        while(!isFinished)
        {
            sem_wait(&BeefSem);

            if(isFinished)
                break;

            cout<<"beef can be used"<<endl;
            sem_post(&AssemblerSem);

        }
        return nullptr;
    }

    static void* BreadFun(void* arg)
    {
        while(!isFinished)
        {
            sem_wait(&BreadSem);

            if(isFinished)
                break;
            cout<<"Bread can be used"<<endl;
            sem_post(&AssemblerSem);
        }
        return nullptr;
    }

    static void* AssemblerFun(void* arg)
    {
        while(!isFinished)
        {
            sem_wait(&AssemblerSem);
            sem_wait(&AssemblerSem);
            sem_wait(&AssemblerSem);

            if(isFinished)
                break;

            counter++;
            cout<<counter<<": assembling the hamburger"<<endl;

            if(counter == sizeOfHamburger)
                isFinished = true;

            sem_post(&VegetableSem);
            sem_post(&BeefSem);
            sem_post(&BreadSem);
        }
        return nullptr;
    }

    static void run()
    {
        pthread_t VegetableWorker, BeefWorker, BreadWorker, Assembler;

        sem_init(&VegetableSem, 0, 1);
        sem_init(&BeefSem, 0, 1);
        sem_init(&BreadSem, 0, 1);
        sem_init(&AssemblerSem, 0, 0);

        pthread_create(&VegetableWorker, NULL, VegetableFun, NULL);
        pthread_create(&BeefWorker, NULL, BeefFun, NULL);
        pthread_create(&BreadWorker, NULL, BreadFun, NULL);
        pthread_create(&Assembler, NULL, AssemblerFun, NULL);

        pthread_join(VegetableWorker, NULL);
        pthread_join(BeefWorker, NULL);
        pthread_join(BreadWorker, NULL);
        pthread_join(Assembler, NULL);
    }
};

class Q3
{
private:
    static inline int countEven = 0;
    static inline int countOdd  = 0;
    static inline int sharedNumber = 0;
    static inline bool isFinished = false;
    static inline bool numCollected = true;
    static inline pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;

public:
    static void* AgentFun(void* args)
    {
        string name = *(string*) args;

        while(!isFinished)
        {
            pthread_mutex_lock(&myMutex);

            if(numCollected && !isFinished)
            {
                sharedNumber = rand() % 10;
                cout << name << " Generate " << sharedNumber << endl;
                numCollected = false;
            }

            pthread_mutex_unlock(&myMutex);
        }
        return nullptr;
    }

    static void* CollectorFun(void* args)
    {
        while(!isFinished)
        {
            pthread_mutex_lock(&myMutex);

            if(!numCollected && !isFinished)
            {
                if(sharedNumber % 2 == 0)
                    countEven++;
                else
                    countOdd++;

                numCollected = true;

                if(countEven >= 100 || countOdd >= 100)
                    isFinished = true;
            }

            pthread_mutex_unlock(&myMutex);
        }
        return nullptr;
    }

    static void run()
    {
        pthread_t Agent1, Agent2, Collector;

        string Ag1 = "Agent1";
        string Ag2 = "Agent2";

        pthread_create(&Agent1, NULL, AgentFun, &Ag1);
        pthread_create(&Agent2, NULL, AgentFun, &Ag2);
        pthread_create(&Collector, NULL, CollectorFun, NULL);

        pthread_join(Agent1, NULL);
        pthread_join(Agent2, NULL);
        pthread_join(Collector, NULL);

        if(countEven >= 100)
            cout << endl << "even counter reacher 100 first" << endl;
        else
            cout << endl << "odd counter reacher 100 first" << endl;
    }
};

class Q4
{
private:
    static const int N = 5;
    static inline pthread_mutex_t forks[N];
    static inline pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;

public:
    static void* philosopher(void* arg)
    {
        int id = intptr_t(arg);
        int left = id;
        int right = (id + 1) % N;

        while(true)
        {
            pthread_mutex_lock(&mymutex);
            cout << "left fork " << id << endl;
            pthread_mutex_unlock(&mymutex);

            pthread_mutex_lock(&forks[left]);
            Sleep(5);

            pthread_mutex_lock(&forks[right]);
            cout << "right fork " << id << endl;

            cout << "Philosopher " << id << " is eating" << endl;

            pthread_mutex_unlock(&forks[left]);
            pthread_mutex_unlock(&forks[right]);
        }
        return NULL;
    }

    static void run()
    {
        for(int i = 0; i < N; i++)
            pthread_mutex_init(&forks[i], NULL);

        pthread_t philosophers[N];

        for(int i = 0; i < N; i++)
        {
            pthread_create(&philosophers[i], NULL, philosopher, (void*)i);
        }

        for(int i = 0; i < N; i++)
            pthread_join(philosophers[i], NULL);
    }
};

int main()
{
    srand(time(NULL));

    /*

    int n;
    cout<<"Entre Size: ";
    cin>>n;


    cout<<"Main of Q1"<<endl;

    Q1::setSize(n);
    Q1::run();


    cout<<endl<<"Main of Q2"<<endl;
    Q2::setSize(n);
    Q2::run();

    cout<<endl<<"Main of Q3"<<endl;
    Q3::run();

    */

    cout<<endl<<"Main of Q4"<<endl;
    Q4::run();

    return 0;
}
