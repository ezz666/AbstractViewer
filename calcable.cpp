# include "calcable.hpp"
# include <iostream>
# include <string>
# include <array>
//------------------------------------------------------------------------------
int Calcable::set_state_read(){
  std::lock_guard<std::mutex> lk_guard(mtx);
  auto old_state = state;
  //std::cout<< "Changing state from " << state_as_string(state) << " to " << state_as_string(3)<<std::endl;
  if (state != 1) state = 3;
  return old_state;
}
//------------------------------------------------------------------------------
void Calcable::set_state_sync_required(){
  std::lock_guard<std::mutex> lk_guard(mtx);
  //std::cout<< "Changing state from " << state_as_string(state) << " to " << state_as_string(2)<<std::endl;
  if (state != 1) std::cerr<<"Wrong state change from "<<state_as_string(state)<<" to " << state_as_string(2)<<std::endl;
  state = 2;
}
//------------------------------------------------------------------------------
std::string Calcable::state_as_string(int state){
  constexpr const std::array<const char *, 4> state_names{"synced", "writing", "sync_required", "reading"};
  if (state<0 || state>=int(state_names.size())){
    std::cerr << "Unknown state " << state <<std::endl;
    std::exit(-1);
  } 
  return state_names[state];
}
//------------------------------------------------------------------------------
std::string Calcable::state_as_string() const {
  return Calcable::state_as_string(get_state());
}
//------------------------------------------------------------------------------
void Calcable::set_state_synced(){
  std::lock_guard<std::mutex> lk_guard(mtx);
  //std::cout<< "Changing state from " << state_as_string(state) << " to " << state_as_string(0)<<std::endl;
  if (state != 3) std::cerr<<"Wrong state change from "<<state_as_string(state)<<" to " << state_as_string(0)<<std::endl;
  state = 0;
}
//------------------------------------------------------------------------------
void Calcable::set_state_write(){
  std::unique_lock<std::mutex> lk(mtx);
  //std::cout<< "Changing state from " << state_as_string(state) << " to " << state_as_string(1)<<std::endl;
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
int Calcable::get_state() const {
  std::lock_guard<std::mutex> lk_guard(mtx);
  return (state);
}
//------------------------------------------------------------------------------
