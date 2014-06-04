#ifndef SRC_TRACK_RANDOMWALKS_HPP
#define SRC_TRACK_RANDOMWALKS_HPP

#include <memory>
#include <string>
#include "BaseRandomWalk.hpp"

namespace car { namespace track {

namespace randomWalk {

std::shared_ptr<BaseRandomWalk> getRandomWalk(const std::string& name);
std::string getHelpString();


} // namespace randomWalk

}} /* namespace car::track */


#endif /* SRC_TRACK_RANDOMWALKS_HPP */
