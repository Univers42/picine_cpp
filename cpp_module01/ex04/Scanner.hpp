#ifndef SCANNER_HPP
# define SCANNER_HPP

#include <string>
#include <fstream>
#include <cstddef>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 8192
# endif

class Scanner
{
	public:
		Scanner(std::ifstream& stream, std::size_t bufSize = BUFFER_SIZE);
		~Scanner( void );
		int				get();		// reading: returns 0..255 or -1 for EOF
		int				unget();	// push back one byte (guaranteed for one byte)
		std::size_t		write(const void *data, std::size_t n);
		std::size_t		write(const std::string& s);
		void			flush(std::ostream& out);
	private:
		std::ifstream&	in;
		char*			rbuf;		// raw buffer (no STL containers)
		std::size_t		rcap;		// capacity of rbuf
		std::size_t		rpos;
		std::size_t		rend;
		std::string		wbuf;
		bool			refill();	// return true if bytes available

		// non-copyable
		Scanner(const Scanner &);
		Scanner& operator=(const Scanner &);
};

#endif