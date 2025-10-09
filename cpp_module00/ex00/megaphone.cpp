#include <iostream>
#include <string>
#include <cctype>

# define FALLBACK "* LOUD AND UNBEARABLE FEEDBACK NOISE *"

void	ft_toupper_buf(std::string &fb)
{
    for (std::size_t i = 0; i < fb.size(); ++i)
        fb[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(fb[i])));
}

int	main(int argc, char **argv)
{
    if (argc == 1)
    {
        std::string fb = FALLBACK;
        ft_toupper_buf(fb);
        std::cout << fb << '\n';
        return 0;
    }

    std::string out;
    for (int i = 1; i < argc; ++i)
    {
        if (i > 1) out += ' ';
        out += argv[i];
    }
    ft_toupper_buf(out);
    std::cout << out << '\n';
    return 0;
}