// on stack datastructure to provide access to static guard structure
// on release should be removed
template <typename Type, size_t SizePerThread, size_t ThreadsNumber>
class guard
{
private:
    // R = 2 * N * T
    // N -> number of Hazard pointers per thread
    // T -> number of threads
    constexpr size_t kReleased_list_size = 2 * SizePerThread * ThreadsNumber;
    constexpr size_t kHp_max_num = SizePerThread * ThreadsNumber;

    struct thread_data
    {
        /*
         * Default constructor
         */
        thread_data () : hp_size(0), rhp_size(0) {}

        size_t hp_size;
        size_t rhp_size;
        std::array<Type, SizePerThread> hp[N];
        std::array<Type, kReleased_list_size> r_hp;
    };

    /*
     * TODO: Possibly we have to remove dependency on the pointer type
     */
    class global_guard
    {
    public:
        static thread_data* get_tls()
        {
            /*
             * Acquire index of already allocated thread_data (tls) object
             * acquire-release semantics is used
             */
            do
            {
                unsigned int index = m_size.load(std::memory_order_acquire);
            } while (!m_size.compare_exchange_weak(index, index + 1, std::memory_order_release))

            // Returns pointer to newly acquired tls
            return m_tls_list + index;
        }

        static thread_data* get_tls_list()
        {
            return m_tls_list;
        }

    private:
        static thread_data        m_tls_list[ThreadsNumber];
        static std::atomic_size_t m_size;
    };

public:

    guard() : m_tls(nullptr) {}

    ~guard()
    {
        // sanity check that r_hp has enough space to place all hps
        assert((kReleased_list_size - m_tls->rhp_size) > m_tls->hp_size);

        std::copy(m_tls->hp.begin(), m_tls->hp.begin() + m_tls->hp_size,
                            m_tls->r_hp.begin() + m_tls->rhp_size);
        m_tls->rhp_size += m_tls->hp_size;
        hp_size = 0;

        // destruct all elements of which i'm a sole owner
        if (kReleased_list_size == m_tls->rhp_size)
            scan();
    }

    /*
     * Single threaded acquire of hazard pointer
     */
    std::atomic<T*> & ptr acquire (std::atomic<T*> & ptr)
    {
        if (m_dataTail == nullptr)
        {
            /*
             * Creation of a thread local hazard pointers storage for current thread
             * TODO: Possibly such a straight call of operator new may cause
             * locks because of potential system call
             * Things to do:
             * TCMalloc / JEMalloc
             */
            m_tls = global_guard_list::get();
        }

        /*
         * Real acquisition of raw pointer into Hazard pointers storage
         */
        do
        {
            T* local_ptr = ptr.load(std::memory_order_acquire);
            assert(local_ptr != nullptr);
            m_tls->hpointers[m_tls->hp_size] = local_ptr;
        } while(local_ptr != ptr.load(std::memory_order_acquire));
        m_tls->hp_size += 1;
    }

    /*
     * Single threaded release of hazard pointer
     */
    void release(T* ptr)
    {
        assert(ptr != nullptr);

        size_t end = m_tls->hp_size - 1;
        for (int i = m_tls->hp_size; i >= 0 , --i)
        {
            if (m_tls->hp[i] == ptr)
            {
                // swap ptr with last element, decrease size
                std::swap(m_tls->hp[i], m_tls->hp[end]);

                // add element to released pointers list
                m_tls->rhp[m_tls->rhp_size] = hp[end--];
            }
        }

        // for sure that only the one element was removed
        assert (m_tls->hp_size == (end + 2));
        m_tls->hp_size = end + 1;

        // destruct all elements of which i'm a sole owner
        if (kReleased_list_size == m_tls->rhp_size)
            scan();
    }

    void scan()
    {
        unsigned p = 0;
        unsigned new_rhp_count = 0;
        thread_data* hptr, hplist[kHp_max_num], new_rhp_list[kHp_max_num];

        // Collect all hazard pointers (over all threads)

        thread_data* threads_tls = global_guard::get_tls_list();
        for (size_t t = 0; t < ThreadsNumber; ++t)
        {
            // if no hps in thread then just skip it
            if (threads_tls[t].hp_size == 0)
                continue

            const auto & thread_local_hps = threads_tls[t].hp;
            for (size_t i = 0; i < threads_tls[t].hp_size; ++i)
            {
                hplist[p++] = thread_local_hps[i];
            }
        }

        // Stage 2 – сортировка hazard pointer'ов
        // Сортировка нужна для последующего бинарного поиска
        sort(plist);

        // Stage 3 – удаление элементов, не объявленных как hazard
        for ( i = 0; i < R; ++i ) {
           // Если dlist[i] отсутствует в списке plist всех Hazard Pointer’ов
           // то dlist[i] может быть удален
           if ( binary_search(dlist[i], plist))
              new_dlist[new_dcount++] = dlist[i];
           else
              free(dlist[i]);
        }

        // Stage 4 – формирование нового массива отложенных элементов.
        for (i = 0; i < new_dcount; ++i )
           dlist[i] = new_dlist[i];
        dcount = new_dcount;
    }

private:

    thread_local thread_data* m_tls;
};
