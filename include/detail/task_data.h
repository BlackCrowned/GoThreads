#pragma once
#include "x86/registers.h"

namespace gothreads {
    namespace detail {
        
        class task;

        struct task_data {
            struct context {
                registers m_registers;  //Always store registers at the end of context structure
            };


            task* m_task;

            context m_context;
        };
    }
}
