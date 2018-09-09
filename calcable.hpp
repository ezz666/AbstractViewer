# ifndef CALCABLE_HPP
# define CALCABLE_HPP
# include <thread>
# include <mutex>
# include <condition_variable>
# include <memory>
class Calcable { 
  protected:
    // State is 0 if everything is in sync
    // 1 --- data updating
    // 2 --- data updated, sync required
    // 3 --- read in progress
    // It's assumed that we never have more than one reader and one writer,
    // so it's not necessary to count readers.
    std::unique_ptr<std::thread> calc_thread;
    int state;
    mutable std::mutex mtx;
    std::unique_lock<std::mutex> lock();
    std::condition_variable cv;
  public:
    Calcable(): calc_thread(nullptr), state(0){}
    ~Calcable(){}
    int set_state_read();
    static std::string state_as_string(int);
    std::string state_as_string() const;
    void set_state_write();
    void set_state_sync_required();
    void set_state_synced();
    void try_join();
    int get_state() const;
};
# endif // CALCABLE_HPP
