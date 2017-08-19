#include <atomic>

#ifdef NBA_SPIN_LOCK
#define NBA_SPIN_LOCK

namespace lock
{

class SpinLock
{
private:
    std::atomic_flag m_flag;

public:
    SpinLock(): m_flag(ATOMIC_FLAG_INIT) {}
    ~SpinLock() { assert(!m_flag); }
    Lock() { while (m_flag.test_and_set(std::memory_order_acquire)); }
    Unlock { m_flag.clear(); }
};

} // lock

#endif // NBA_SPIN_LOCK
