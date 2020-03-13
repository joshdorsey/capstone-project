#ifndef TIMING_H
#define TIMING_H

#include <chrono>
#include <string>   // std::string
#include <map>      // std::map
#include <ostream>  // std::ostream
#include <iomanip>  // std::setw, std::setprecision, std::fixed

#if __GNUC__
#define _INLINEFUNC __inline__
#elif _MSC_VER && !__INTEL_COMPILER
#define _INLINEFUNC __force_inline
#elif __INTEL_COMPILER
#define _INLINEFUNC __inline
#else
#define _INLINEFUNC inline
#endif

namespace timing {
    using namespace std::chrono;
    using std::map;
    using std::string;
    using std::pair;
    using std::make_pair;

    // Output/Formatting
    using std::ostream;
    using std::setw;
    using std::setprecision;
    using std::fixed;

    namespace {
        typedef pair<time_point<high_resolution_clock>,
                     time_point<high_resolution_clock>> time_pair;

        map<string, time_pair> timing_map;
        double error_val = -1.;
        size_t longestName = 0;

        bool timer_exists(const string &timer) {
            return timing_map.find(timer) != timing_map.end();
        }

        double get_time(const time_pair &pair) {
            auto diff = pair.second - pair.first;
            double secs = duration<double, seconds::period>(diff).count();
            if(secs >= 0) {
                return secs;
            }

            return error_val;
        }
    }

    void start(const string &timer) {
        auto now = high_resolution_clock::now();
        timing_map[timer] = make_pair(now, now - seconds(1));
        
        if (timer.length() > longestName) {
            longestName = timer.length();
        }
    }

    double end(const string &timer) {
        auto now = high_resolution_clock::now();
        if(timer_exists(timer)) {
            timing_map[timer].second = now;

            return get_time(timing_map[timer]);
        }

        return error_val;
    }
    
    double __forceinline stop(const string &timer) {
        return end(timer);
    }

    double get(const string &timer) {
        if(timer_exists(timer)) {
            return get_time(timing_map[timer]);
        }

        return error_val;
    }
    
    void printSummary(ostream &stream) {
        for(auto i = timing_map.rbegin(); i != timing_map.rend(); i++) {
            double time = get_time(i->second);
            if(time != error_val) {
                stream << setw(longestName) << i->first << ": ";
                stream << fixed << setprecision(6) << time << " s\n";
            }
        }
        stream.flush();
    }
}

#undef _INLINEFUNC
#endif // TIMING_H
