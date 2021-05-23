#pragma once

#include <cassert>
#include <functional>
#include <iostream>
#include <list>
#include <utility>
#include <vector>

template<class T, class Compare = std::less<T>>
struct Fishspear {
    static constexpr double Beta = 0.7034;

    explicit Fishspear() {
        bs_.push_back(1);
    }

    size_t size() const {
        size_t n = u_.size();
        for (const auto& vi : v_) n += vi.size();
        for (const auto& wi : w_) n += wi.size();
        return n;
    }

    template<class F>
    void for_each_element(const F& f) const {
        for (const auto& x : u_) f(x);
        for (const auto& vi : v_) for (const auto& x : vi) f(x);
        for (const auto& wi : w_) for (const auto& x : wi) f(x);
    }

    bool empty() const {
        return u_.empty();
    }

    const T& top() const {
        assert(!u_.empty());
        return u_.front();
    }

    void pop() {
        delete_sharp();
        if (u_.empty()) {
            resume_push_loop();
        }
    }

    template<class... Args>
    void reemplace_top(Args&&... args) {
        assert(!u_.empty());
        u_.front() = T(static_cast<Args&&>(args)...);
        auto& vk = v_.emplace_front();
        vk.splice(vk.begin(), u_, u_.begin());
        w_.push_front(std::move(u_));
        u_.clear();
        start_push_loop();
    }

    template<class... Args>
    void emplace(Args&&... args) {
        barb_create(static_cast<Args&&>(args)...);
        start_push_loop();
    }

    void push(const T& x) {
        emplace(x);
    }

    void push(T&& x) {
        emplace(std::move(x));
    }

    friend std::ostream& operator<<(std::ostream& os, const Fishspear& fs) {
        os << "U =";
        for (auto& x : fs.u_) os << " " << x;
        for (int k = fs.v_.size(); k >= 1; --k) {
            auto&& vk = *std::prev(fs.v_.end(), k);
            auto&& wk = *std::prev(fs.w_.end(), k);
            os << "\nV[" << k << "] =";
            for (auto& x : vk) os << " " << x;
            os << "\nW[" << k << "] =";
            for (auto& x : wk) os << " " << x;
        }
        return os;
    }

private:
    void start_push_loop() {
        while (!w_.front().empty()) {
            size_t u = u_.size();
            if (v_.front().size() < u && u < bs_.back()) {
                bs_.push_back(1 + Beta * u);
                return;
            } else {
                pmerge();
            }
        }
        barb_dispose();
        resume_push_loop();
    }

    void resume_push_loop() {
        bs_.pop_back();
        if (bs_.empty()) {
            bs_.push_back(1 + Beta * u_.size());
        } else {
            start_push_loop();
        }
    }

    void delete_sharp() {
        invariants();
        assert(!u_.empty());
        u_.pop_front();
        invariants();
    }

    template<class... Args>
    void barb_create(Args&&... args) {
        invariants();
        v_.emplace_front();
        v_.front().emplace_front(static_cast<Args&&>(args)...);
        w_.push_front(std::move(u_));
        u_.clear();
        invariants();
    }

    void pmerge() {
        invariants();
        assert(!v_.empty() && !w_.empty());
        auto& vk = v_.front();
        auto& wk = w_.front();
        assert(!wk.empty());
        if (vk.empty() || greater()(wk.front(), vk.front())) {
            u_.splice(u_.end(), wk, wk.begin());
        } else {
            u_.splice(u_.end(), vk, vk.begin());
        }
        invariants();
    }

    void barb_dispose() {
        invariants();
        assert(!v_.empty() && !w_.empty());
        int k = v_.size();
        auto& vk = v_.front();
        auto& wk = w_.front();
        assert(wk.empty());
        if (k == 1) {
            u_.splice(u_.end(), std::move(vk));
        } else {
            auto& vkminus1 = *std::next(v_.begin());
            vkminus1.merge(std::move(vk), greater());
        }
        v_.pop_front();
        w_.pop_front();
        invariants();
    }

    void invariants() {
#ifdef FISHSPEAR_TEST_INVARIANTS
        assert(v_.size() == w_.size());
        assert(std::is_sorted(u_.begin(), u_.end(), greater()));
        for (auto&& v : v_) {
            assert(std::is_sorted(v.begin(), v.end(), greater()));
        }
        for (auto&& w : w_) {
            assert(std::is_sorted(w.begin(), w.end(), greater()));
        }
#endif
    }

    auto greater() const {
        return [&less = less_](const T& x, const T& y) { return less(y, x); };
    }

    Compare less_;
    std::vector<size_t> bs_;
    std::list<T> u_;
    std::deque<std::list<T>> v_;  // vk, vk-1, ... v2, v1
    std::deque<std::list<T>> w_;  // wk, wk-1, ... w2, w1
};
