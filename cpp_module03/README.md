# exercice 0
## variables
## Name handling
- [ ] Construct with an empty string as name
- [ ] Try to modify the name "should not be possible, as it's `const`"
- [ ] construct with a very long name.
### edge cases
- [ ] test if all variables hits can go under 0
- [ ] test is the name of the robot can be edit or not
- [ ] test if the variable can be set to 0.
- [ ] test if the name can be set to NULL.
- [ ] test if the claptrap is well liberated
- [ ] test if there are leaks
- [ ] the attack should be able
- [ ] Test attacking or repairing when energy points are 0 (should not be possible)
- [ ] test taking damage when hit points are 0 (should not revive unless allowed by our logic)
- [ ] test self-assignment (e.g., `a = a`) for copy and move assignment operators
- [ ] test if the destructor is called for all objects (no memory leaks)
- [ ] test if copy/move constructors and assignment operators work as expected (deep copy, no shared state)
- [ ] test if the attack, repair, and takeDamage methods handles every large values
- [ ] test if the class behaves correctly when used in a container
- [ ] test move construction and move assignment from an already-moved object
- [ ] test copy/move assignment when the source and destination have different internal states
- [ ] test creating a large number of ClapTrap objects to check for performance or resource issues
- [ ] test using ClapTrap objects as keys or values in associative containers (e.g., std::map, std::set) if copy/move semantics allow
- [ ] test behavior when passing extremely large values to the constructor (e.g., a name with thousands of characters)
- [ ] test if the class is exception safe (e.g., what happens if memory allocation fails during copy/move)
- [ ] test if the class can be used with standard algorithms (e.g., std::sort, std::find), if applicable
- [ ] test if the class can be used as a base class (i.e., is it designed for inheritance? If not, document it)
- [ ] test if the class can be default-constructed, if you add a default constructor
- [ ] test if the class can be destroyed safely when partially constructed (e.g., if an exception is thrown in the constructor)
 
## comparison

## Method robustness
- [ ] Call `attack`, `takedamage`, and `beRepaired` repeatedly until energy or hit poits are depleted.
- [ ] call `attack`on self(e.g, a.attack("Alpha"));

## Destruction
- [ ] Ensure destructors are called for all objects (no leaks, use valgrind, or similar)
- [ ] Create and destroy many ClapTrap objects in a loop.
## Containers
- [ ] store claptrap objects in a std::vector or std::array, and test copy/move behavior
## Extreme values
- [ ] test with `UINTMAX32_MAX` for hit, nrj, attack, if possible
## invalid input
- [ ] Pass negative values (as large unsigned ints) to `takeDamage`and `beRepaired`