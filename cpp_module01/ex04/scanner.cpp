#include "Scanner.hpp"
#include <fstream>
#include <ostream>
#include <cstddef>

Scanner::Scanner(std::ifstream& stream, std::size_t bufSize)
: in(stream), rbuf(NULL), rcap(0), rpos(0), rend(0), wbuf()
{
    if (bufSize == 0) bufSize = BUFFER_SIZE;
    rbuf = new char[bufSize];
    rcap = bufSize;
}

Scanner::~Scanner( void )
{
    delete[] rbuf;
    rbuf = NULL;
    rcap = 0;
}

bool Scanner::refill()
{
    if (!in.good()) return false;
    in.read(rbuf, static_cast<std::streamsize>(rcap));
    std::streamsize n = in.gcount();
    if (n <= 0) return false;
    rpos = 0;
    rend = static_cast<std::size_t>(n);
    return true;
}

int Scanner::get()
{
    if (rpos >= rend) {
        if (!refill()) return -1; // EOF
    }
    unsigned char c = static_cast<unsigned char>(rbuf[rpos++]);
    return static_cast<int>(c);
}

int Scanner::unget()
{
    if (rpos == 0) {
        // single-byte pushback only supported within current buffer
        return -1;
    }
    --rpos;
    return 0;
}

std::size_t Scanner::write(const void *data, std::size_t n)
{
    if (n == 0 || data == NULL) return 0;
    const char *cdata = reinterpret_cast<const char*>(data);
    wbuf.append(cdata, n);
    return n;
}

std::size_t Scanner::write(const std::string& s)
{
    wbuf.append(s);
    return s.size();
}

void Scanner::flush(std::ostream& out)
{
    if (!wbuf.empty()) {
        out.write(wbuf.data(), static_cast<std::streamsize>(wbuf.size()));
        wbuf.clear();
    }
    out.flush();
}