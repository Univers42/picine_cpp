#include "../PhoneBook.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <cctype>

#define PAT_SPACE "\t\n\r\v\f "

std::string strtolower(const std::string& in)
{
  std::string out;
  for (std::size_t i = 0; i < in.size(); ++i) {
    unsigned char ch = static_cast<unsigned char>(in[i]);
    out.push_back(static_cast<char>(std::tolower(ch)));
  }
  return out;
}

void trim_inplace(std::string& s) {
  s.erase(0, s.find_first_not_of(PAT_SPACE));
  size_t end = s.find_last_not_of(PAT_SPACE);
  if (end != std::string::npos)
    s.erase(end + 1);
  else
    s.clear();
}

std::string trim(const std::string& s) {
  size_t start = s.find_first_not_of(PAT_SPACE);
  size_t end = s.find_last_not_of(PAT_SPACE);

  if (start == std::string::npos)
    return ("");
  return (s.substr(start, end - start + 1));
}

int main( void )
{
	std::string input;
	PhoneBook pb;

  input = "";
  while (true) {
      std::cout << "$> " << std::flush;
      if (!std::getline(std::cin, input))
          break;
      else if (strtolower(trim(input)) == "add")
          pb.add();
      else if (strtolower(trim(input)) == "search")
        pb.search();
      else if (strtolower(trim(input)) == "seed")
        pb.seed();
      else if (strtolower(trim(input)) == "exit")
          break;
  }
  return 0;
}
