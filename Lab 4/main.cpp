#include <bits/stdc++.h>

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



int main()
{
    srand(time(NULL));

    int n;
    cout<<"Entre Size of cigarette: ";
    cin>>n;

    Q1::setSize(n);
    Q1::run();

    return 0;
}
