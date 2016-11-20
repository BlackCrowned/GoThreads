#pragma once
#include "x86/registers.h"

namespace gothreads {
    namespace detail {
        
        class task;

        struct task_data {
            struct context {
                registers m_registers;  //Always store registers at the end of context structure
            };

            context m_context;

            task* m_task;
        };
    }
}
