#include "malloc_free/memory_manager.h"
#include "malloc_free/smartpointor.h"

void smartpointorTest();
int main()
{
    // malloc_memory();
    // access_free_data();
    // malloc_mem_control_block();
    smartpointorTest();
    return 0;
}

void smartpointorTest() {
    smartpointor<int> sptr(new int(1024));
    smartpointor<int> sptr1;
    sptr1  = sptr;
    smartpointor<int> sptr2(sptr);
}