#include "Zombie.hpp"
#include <iostream>

// Create on heap and return pointer - caller must delete
Zombie* makeHeapZombie(const std::string &name)
{
    return new Zombie(name, true);
}

// Return by value (stack-like semantics for the caller's temporary). In C++98 this will
// cause a copy, but for our simple class it demonstrates that the returned object's
// lifetime is managed by the receiver.
Zombie makeStackZombieByValue(const std::string &name)
{
    Zombie z(name, false);
    return z; // copy/move into the caller
}

// Demonstration function that interacts with heap/stack objects across scopes
void scope_demo()
{
    std::cout << Zombie::C_CYAN << "\n--- scope_demo start ---" << Zombie::C_RESET << std::endl;

    // 1) Heap-created object can be controlled from outside its creating scope.
    Zombie *h = makeHeapZombie("heap_outside");
    h->announce();

    // 2) Returned-by-value object: receiver gets its own copy/instance.
    Zombie v = makeStackZombieByValue("value_returned");
    v.announce();

    // 3) Demonstrate that heap object lives until we delete it, even after leaving
    // this function scope (we will delete it later in main to show this behavior).
    // We intentionally do NOT delete 'h' here to show external lifetime control.

    std::cout << Zombie::C_CYAN << "--- scope_demo end (heap_outside still alive) ---\n" << Zombie::C_RESET << std::endl;
}
