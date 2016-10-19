#pragma once

namespace gothreads {
    namespace detail {
        namespace assembler {
            
            extern "C" {
                extern void _cdecl setup_context(void* task_data);

                extern void _cdecl swap_context(void* task_data_next, void* task_data_current);

                extern void* _cdecl get_current_context();
            }
        }
    }
}