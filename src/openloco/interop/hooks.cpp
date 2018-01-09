#include "../ui.h"
#include "../windowmgr.h"
#include "interop.hpp"


void register_hooks()
{
    using namespace openloco::ui::windows;

    // Replace ui::update() with our own
    register_hook(0x004524C1,
        [](registers &regs) -> uint8_t
        {
            openloco::ui::update();
            return 0;
        });

    register_hook(0x00445AB9,
        [](registers &regs) -> uint8_t
        {
            auto result = prompt_load_game(
                regs.al,
                (char *)regs.ecx,
                (const char *)regs.edx,
                (const char *)regs.ebx);
            regs.eax = result ? 1 : 0;
            return 0;
        });

    // Remove the set window pos function, we do not want it as it
    // keeps moving the process window to 0, 0
    // Can be removed when windowmgr:update() is hooked
    write_ret(0x00406520);
}
