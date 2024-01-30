/** @file */

#ifndef EMIAS_PROCESS_FULLSTATE_HPP
#define EMIAS_PROCESS_FULLSTATE_HPP


#include <emias/global_instance.hpp>


namespace NEmias {

    /**
     * \brief Function periodically processing requests from full state
     * @return true if full state changed after processing
     */
    bool ProcessFullState();

} // NEmias


#endif
