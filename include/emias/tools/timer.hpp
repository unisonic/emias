#include <chrono>

namespace NEmias::NTools {

    class TTimer {
    private:
        std::chrono::seconds requestPeriod;
        std::chrono::time_point<std::chrono::high_resolution_clock> previousTimepoint;
    public:
        TTimer(std::chrono::seconds requestPeriod) 
            : requestPeriod(requestPeriod)
            , previousTimepoint(std::chrono::high_resolution_clock::now() - requestPeriod)
        { }

        void Reset() {
            previousTimepoint = std::chrono::high_resolution_clock::now();
        }

        bool PeriodElapsed() const {
            const auto currentTimepoint = std::chrono::high_resolution_clock::now();
            const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTimepoint - previousTimepoint);

            return elapsed.count() > requestPeriod.count();
        }
    };

}  // NEmias::NTools
