# Advanced Macro Devices: Deadlock Tycoon

Have you ever wondered how it feels to be CEO of a processor vendor? Do you have the guts to be a processor architect? Do you compute? If yes, then wait no more. Dive into the action from the times of 8088, develop yourself until cpu market is dominated by your advanced vector processors (even integrated gpus) and reach new goals on every play. See [wiki](https://github.com/tugrul512bit/AdvancedMacroDevices/wiki) for details.


- Design your unique modular CPU on a uniform grid of cells
- Reprogram modules (like "control unit") for more performance or leave them at their default setting and focus on more fun
- Try to evade deadlock as much as possible
- Pass as many benchmarks as possible
- Beat Imtel, Nvideo and IMB in CPU performance, quality, power and sell more CPUs
- Level-up your character and distribute skill points
- Unlock better technologies
- Buy factories to produce more CPUs
- watch millions of NPCs use your processors, profit
- Iterate your next CPU design

![[https://github.com/tugrul512bit/AdvancedMacroDevices/blob/master/wiki/decoder-alu-controller.png](https://github.com/tugrul512bit/AdvancedMacroDevices/blob/master/wiki/decoder-alu-controller.png)]


# Dependencies

- C++14
- vcpkg: for installing other dependencies
- opencv: image processing
- imgui: corporation management interface, character specs, cpu design
- opencl: gpgpu for simulated-annealing to train custom neural networks (millions of NPC) that will make them act like gamers, datacenter owners & cpu architects
- opengl: imgui's opengl bindings are used
- various libraries from my profile (LruClockCache, DirectMappedCache, Ultra Fast Simulated Annealing, etc)

# Features

- millions of simulated consumers in a free (cpu) market simulation.
- run benchmarks (with assembler specific to your cpu) on designed cpus & have influence on gamers, data-centers and miners, to sell more. 
- tune production costs to optimize profit
- manage R&D to win the tech race
- optionally fine-tune pipelines of cores for optimum price/performance
- hire tech forums/channels to do advertisement, even cheat on benchmark charts
- upgrade with integrated gpu & custom gpu designs
- sell designs to other corporations
- hire test engineers, developers and architects to optimize your cpus in a better way when you dont want to fiddle with modules
- hold 99% of market for 20 years to win
- select your ceo with a unique skill tree
- research new technologies like out of order execution, speculative execution, SIMD, multiple threads per core (from 2-way to 32-way per core), multiple cores, multiple cpus on same motherboard, different types of caches & complex instruction sets, all used either in an automated way or your own way.
- fight economic crisis and other difficult situations, survive.

All simulation computations will be in GPU to enable bigger populations on high end systems.

# Gameplay

- player picks main character class with hundreds of unique skills to choose from tree, ranging from logic mastery to physical optimization.
- selects background talents as a balancing factor against other corporations
- given only a small corporation building with empty rooms
- world is also initialized with random conditions
- game starts first turn
- all players make plans for what to build (factory, cpu production, etc)
- all players hire developers, testers, etc
- all players develop software
- all players develop hardware
- first turn ends
- game runs in realtime until second turn begins while players can only watch what is happening (video gamers buying cpus, corporations giving opportunities to work with, earthquakes, cryptocoin bubbles, all the chaotic world requirements from cpu vendors from simple game benchmarks to expensive CGI in movie industry, all kinds of disasters & design errors)
- realtime ends with income report & r&d report (r&d will also have random outcomes like failures or quantum-leaps)
- second turn starts...

# Skill System

All skills are connected to at least one other skill in a graph structure. 

Each skill requires skill points to spend.

Players gain skill points when they level-up. 10+dice skill points per level, with higher experience requirement for every new level.

Players can start choosing only from base skills:

- electronics (logical optimizations, ...)
- economics (investing, optimizing income)
- physics (better & smaller transistors)
- sociology (better r&d staff, ads, ...)
- maths (everywhere)
- algorithm


Base skills are connected to more advanced skills like these:

- logic optimization (algorithm & math)
- logic gate optimization (physics, maths, electronics)
- supercomputing (electronics, physics, economics)
- storage (physics, electronics, math)
- etc

Every skill can take multiple skill points for boosting its effects higher or unlocking new skills.

Every player class starts with a fixed set of skills.

When a specific player level is reached, a second class can be chosen to balance weaknesses or help improve certain advantages.

Sample skill requirement for 128MB 64-way integrated level-3 cache:

- storage 44
- caching 15

Sample requirement for 64-wide SIMD:

- Computing 22
- Parallel computing 5

Players get experience from everything:

- 1 per credit (from selling cpu only)
- 5 per r&d point gained per turn

# R&D

Players invest resources into research to gain points. Points are used for unlocking tech, but only when they are available by skill tree.

Every tech has a success percentage. Spending r&d points on a technology only increases its success percentage and triggers a check. If its successful, the tech is unlocked. This way, lucky players can start earlier. Disadvantage is that it also has a failure ratio with opposite value. If a tech is unlocked at 1%, then it has 99% failure rate (failure means it won't function, function slow, function randomly or can't even pass the tests). If failure is at realtime phase, then customers will not be happy.

Players can hold unlocking until any percentage is achieved, to minimize negative consumer feedback.

Technologies can go beyond their 100% unlock value to have greater effects. Cache at 200% unlock value enables 3D stacked cache design. ALU at 500% unlock value makes ray-tracing cores available (same core can be custom-designed by players too but at the cost of many skill points).

# CPU Simulation

Every detail in cpu design will have impact on benchmark points. Benchmarks will have certain series of instructions and they will pass through pipelines of cpu during simulation. They will just go through decode, load, compute, store, retire. Actual computation will not be shown, just blobs of different colors will pass through pipelines.

Types of instructions:

- load from memory into register
- compute integer
- compute floating point
- compute fp special function
- store to memory
- branch
- custom instruction (ray trace, calculate force, etc) that has multiple microinstructions
- sync (for multiple cores/cpus)

If pipeline has SIMD capability, then multiple instructions will be automatically loaded at once, without the need of recompiling codes. 

Adding more features to a cpu core increases the number of required transistors and power requirement.

Bigger CPUs have bigger production failure rate. Failed products can be recycled as a binned product by disabling some (failed) parts of cpu and selling it as a lower segment product (like 2 cores instead of 4).

Players can order architects to optimize cpus for one or more stats:

- less transistors at the cost of increased software requirement (such as computing square root on software)
- less transistors at the cost of latency (computing square root iterations in a hardware-based loop (state machine) instead of a fully unrolled pipeline)
- less power at the cost of lower frequency
- less power at the cost of better material / lithography
- less power with more design time
- etc

Overall design quality is dictated by the quality of architects and engineers. Sociology skill increases chances of finding better people for the team.

If players want to custom-design every part of cpu, then a lot of skill points (and multi-class) will be required. 

Winning the game adds 10 extra skill points on next game. Reaching certain goals unlock special reserved skills. Inventing time machine (which costs tremendous compute power) passes 10% of skill points onto next game (along with some cash).

# Customers

Customers have randomly initialized budgets, incomes and computational requirements.

Customers can look at benchmarks to know what to buy or try to guess before buying. Some individuals will also influence other customers negatively or positively (popularity stat determines this). Some player skills help on this issue.

Number of total potential customers increase exponentially with time but each one has a total life time (30-40 turns).

Selling top-quality product can make a customer so happy that the next upgrade can take too much time, reducing the income of even player's own corporation.

# Genre

2D RPG/Strategy/Simulation with 8bit pixel art as a mixture of factorio and dungeons & dragons.

Real-time phase events (disasters, new customers, etc) have dice-rolls. Player classes have dice rolls on gaining skill points per level and some other stats.

# Player Stats

- learning: learning the learning increases the learning rate (skill points per level)
- r&d: when player works alone, this is useful in early-game.
- strength: useful only for stealing opponent corporation servers and defending against assassinations
- health: against disasters, assassinations and genetically modified food
- intelligence: for covert ops, hacking devices and placing spies into opponent headquarters (also to stop being hacked)
- advertisement: useful for finding an investor or increaing customers (requires credits)
- leadership: not needed for single-person corporations. Boosts workers.

# Player Classes

All customers and workers also have their own class and any two can be combined as  multiclass.

- Processor Architect
- Electronics Engineer
- Physics Engineer
- Banker

