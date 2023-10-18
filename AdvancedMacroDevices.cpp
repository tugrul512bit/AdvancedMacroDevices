#include"MainWindow.h"

/*

This is a processor design game where players create competitive CPUs 
by connecting different modules to beat other players, NPCs and win with total market domination by selling more CPUs.

Gain skills by experience, distribute new stat/skill points to unique stats and skills. Unlock technologies. Design better CPUs. 
Making a CPU design game is not an easy task. It is planned to be done this way:

- test driven development
- complete all tests until release
- continuously add new tests
- optimize most important tests first
- leave ultra-hard problems as "feature" to be played against by players (i.e. "Deadlock Tycoon")
- but still give them opportunities like Banker's Algorithm, Round-Robin Scheduling, Auto-load-balancing & fully pipelined bidirectional bus framework

Game type: RPG/Simulation/RTS elements

CPU architecture:

- Harward Architecture: optional
- Von Neumann Architecture: optional
- Shared-Distributed Memory Model: optional
- Software-Based Cache Coherence: optional
- Snoop-Cache Coherence: optional
- Directory-Based Cache Coherence: optional
- Out-of-Order-Execution: optional
- Branch-Prediction: optional
- SIMD: optional
- SIMT: optional

Author: Huseyin Tugrul Buyukisik
*/
int main(int argc, char** argv) {
    {
        Window::App app = Window::InitApp();
        app.Loop();
    }
    std::cout << "app closed" << std::endl;
    return 0;
}