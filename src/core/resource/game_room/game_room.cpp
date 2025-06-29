#include "core/game_world/game_room.hpp"

std::condition_variable GameRoom::cv;
std::mutex GameRoom::cv_mtx;