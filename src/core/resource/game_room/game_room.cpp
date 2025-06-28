#include "core/resource/game_room/game_room.hpp"

std::condition_variable GameRoom::cv;
std::mutex GameRoom::cv_mtx;