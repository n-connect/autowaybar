#include <cstdio>
#include <iostream>
#include <istream>
#include <sstream>
#include <thread>

using namespace std::chrono_literals;

const std::string WAYBAR_CONFIG = "~/dotfiles/waybar/themes/ml4w/config";
const std::string WAYBAR_STYLE = "~/dotfiles/waybar/themes/ml4w/mixed/style.css";

auto getCursorPos() -> std::pair<int, int> {

    FILE* pipe = popen("hyprctl cursorpos", "r");

    if (!pipe) {
        std::cerr << "Pipe failed to exec hyprctl command";
        return {-1, -1};
    }

    char buffer[128];
    std::string result;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    
    pclose(pipe);

    std::istringstream stream(result);
    int xpos, ypos;
    char basurilla;

    if (stream >> xpos >> basurilla >> ypos)
        return {xpos, ypos};


    return {-1, -1};
}

auto main() -> int {

    bool open = true;
    std::string command = "hyprctl dispatch exec \"waybar -c " + WAYBAR_CONFIG + " -s " + 
    WAYBAR_STYLE + "\"";

    while (true) {
        auto [root_x, root_y] = getCursorPos();

        std::cout << "[+] Found mouse at position at: " << root_x << ", " << root_y << "\n";

        if (open && root_y > 35) {
            std::cout << "[+] It should die \n";
            system("killall waybar");
            open = false;
        }
        else if (!open && root_y < 35) {
            std::cout << "[+] opening it\n";
            system(command.c_str());
            open = true;
        }
        else {
            std::cout << "[+] everything okai \n";
        }


        std::this_thread::sleep_for(50ms);
    }
}
