# include "calcable.hpp"
# include <iostream>
//------------------------------------------------------------------------------
int Calcable::set_state_read(){
  std::lock_guard<std::mutex> lk_guard(mtx);
  auto old_state = state;
  if (state != 1) state = 3;
  return old_state;
}
//------------------------------------------------------------------------------
void Calcable::set_state_sync_required(){
  std::lock_guard<std::mutex> lk_guard(mtx);
  if (state != 1) std::cerr<<"Wrong state change from "<<state<<" to " << 2<<std::endl;
  state = 2;
}
//------------------------------------------------------------------------------
void Calcable::set_state_synced(){
  std::lock_guard<std::mutex> lk_guard(mtx);
  if (state != 3) std::cerr<<"Wrong state change from "<<state<<" to " << 0<<std::endl;
  state = 0;
}
//------------------------------------------------------------------------------
void Calcable::set_state_write(){
  std::unique_lock<std::mutex> lk(mtx);
  cv.wait(lk, [this](){ return state==0; });
  state = 1;
}
//------------------------------------------------------------------------------
void Calcable::try_join(){
  if (calc_thread.get() != nullptr){
    if (calc_thread->joinable()) calc_thread->join();
  }
}
//------------------------------------------------------------------------------
int Calcable::get_state(){
  std::lock_guard<std::mutex> lk_guard(mtx);
  return (state);
}
//------------------------------------------------------------------------------
