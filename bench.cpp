#include <chrono>
#include <queue>
#include <random>
#include <stdio.h>
#include <vector>

#include "fishspear.h"

struct S {
    static int comparison_count;
    static int move_count;
    static int assign_count;
    int value_;

    explicit S(int v) : value_(v) {}

    S(S&& rhs) : value_(rhs.value_) {
        move_count += 1;
    }

    S& operator=(S&& rhs) {
        value_ = rhs.value_;
        assign_count += 1;
        return *this;
    }

    struct Less {
        bool operator()(const S& a, const S& b) const {
            comparison_count += 1;
            return a.value_ < b.value_;
        }
    };
};
int S::comparison_count = 0;
int S::move_count = 0;
int S::assign_count = 0;

template<class PQ>
void bench(PQ& pq)
{
    std::mt19937 g;
    constexpr int INITIALSIZE = 10'000;
    constexpr int OPERATIONS = 200'000;
    constexpr int MAXVALUE = 100'000;

    int pushcount = 0;
    int popcount = 0;

    for (int i=0; i < INITIALSIZE; ++i) {
        pq.emplace(g() % MAXVALUE);
    }

    S::comparison_count = 0;
    S::move_count = 0;
    S::assign_count = 0;

    auto t1 = std::chrono::high_resolution_clock::now();

    for (int i=0; i < OPERATIONS; ++i) {
        if (g() % 2 == 0 || pq.empty()) {
            pq.emplace(g() % MAXVALUE);
            pushcount += 1;
        } else {
            pq.pop();
            popcount += 1;
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();

    printf("%d operations (%d push, %d pop) ", OPERATIONS, pushcount, popcount);
    printf("required %d comparisons, %d moves, %d assignments, ", S::comparison_count, S::move_count, S::assign_count);
    printf("%d milliseconds\n", int(std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()));
}

int main()
{
    std::priority_queue<S, std::vector<S>, S::Less> pq;
    Fishspear<S, S::Less> fs;

    printf("Heap:      "); bench(pq);
    printf("Fishspear: "); bench(fs);
}
