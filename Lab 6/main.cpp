#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    #pragma omp parallel
    {
        #pragma omp cretical
        {
            cout << "Thread: " << omp_get_thread_num() << endl;
        }
    }
    return 0;
}
