/** @file */

#ifndef EMIAS_TIMER_HPP
#define EMIAS_TIMER_HPP


#include <chrono>

//! Namespace containing tools for miscellaneous tasks
namespace NEmias::NTools {

    /**
     * \brief Timer for periodical requests
     */
    class TTimer {
    private:
        const std::chrono::seconds requestPeriod; /**< Fixed period of requests */
        std::chrono::time_point<std::chrono::high_resolution_clock> previousTimepoint; /**< Timepoint of previous request */
    public:
        /**
         * Constructor of timer
         * Previous timepoint is set so that first call to PeriodElapsed() returns True
         * @see PeriodElapsed()
         * @see requestPeriod
         * @param requestPeriod Period of requests in seconds 
         */
        TTimer(std::chrono::seconds requestPeriod) 
            : requestPeriod(requestPeriod)
            , previousTimepoint(std::chrono::high_resolution_clock::now() - requestPeriod)
        { }

        /**
         * Sets previousTimepoint to current time
         */
        void Reset() {
            previousTimepoint = std::chrono::high_resolution_clock::now();
        }

        /**
          * \brief Function checking whether request period elapsed
          * @return true if request period elapsed
          */
        bool PeriodElapsed() const {
            const auto currentTimepoint = std::chrono::high_resolution_clock::now();
            const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTimepoint - previousTimepoint);

            return elapsed.count() > requestPeriod.count();
        }
    };

}  // NEmias::NTools


#endif
