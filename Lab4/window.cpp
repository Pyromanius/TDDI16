#include "window.h"
#include "gui_window.h"
#include "text_window.h"
#include <string>

WindowPtr create_window(int argc, const char **argv) {
    if (argc == 2 && std::string(argv[1]) == "--text") {
        return WindowPtr{new Text_Window{}};
    } else if (argc == 1) {
        return WindowPtr{new Gui_Window{}};
    } else {
        std::cout << "Usage:" << std::endl;
        std::cout << argv[0] << " < input.txt" << std::endl;
        std::cout << "or:" << std::endl;
        std::cout << argv[0] << " --text < input.txt" << std::endl;
        return WindowPtr{};
    }
}
