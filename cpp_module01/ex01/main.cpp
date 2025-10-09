#include "Zombie.hpp"

Zombie* zombieHorde(int N, std::string name);
void destroyHorde(Zombie *horde, int N);

int	main(void)
{
	int N = 5;
	Zombie *horde = zombieHorde(N, "Zombie");
	if (horde)
	{
		for (int i = 0; i < N; ++i)
			horde[i].announce();
		destroyHorde(horde, N);
	}
	return (0);
}