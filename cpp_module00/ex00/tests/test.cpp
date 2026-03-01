#include <cstring>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../megaphone_solutions.hpp"

// Define function pointer type for solutions
typedef int (*SolutionFn)(int, char**);

// ── C++ Scalable Test Framework ──────────────────────────────────────────────

class MegaphoneTester {
 private:
  SolutionFn target_func;
  int passed;
  int failed;

  // Helper to strip ANSI color codes (e.g., \033[33m) so tests compare pure
  // text
  std::string strip_ansi_and_newlines(const std::string& str) {
    std::string clean = "";
    bool in_escape = false;

    for (size_t i = 0; i < str.length(); ++i) {
      if (str[i] == '\033') {
        in_escape = true;  // Start of color code
      } else if (in_escape && str[i] == 'm') {
        in_escape = false;                        // End of color code
      } else if (!in_escape && str[i] != '\n') {  // Ignore newlines and escapes
        clean += str[i];
      }
    }
    return clean;
  }

  std::string capture_output(int ac, char** av) {
    std::streambuf* old_cout = std::cout.rdbuf();
    std::ostringstream oss;
    std::cout.rdbuf(oss.rdbuf());

    target_func(ac, av);

    std::cout.rdbuf(old_cout);
    return strip_ansi_and_newlines(oss.str());
  }

 public:
  explicit MegaphoneTester(SolutionFn func)
      : target_func(func), passed(0), failed(0) {}

  // Highly scalable test runner
  void run_test(const std::string& label, const std::vector<std::string>& args,
                const std::string& expected) {
    std::vector<char*> argv;

    char prog_name[] = "./megaphone";
    argv.push_back(prog_name);

    for (size_t i = 0; i < args.size(); ++i) {
      char* buf = new char[args[i].length() + 1];
      std::memcpy(buf, args[i].c_str(), args[i].length() + 1);
      argv.push_back(buf);
    }

    std::string got = capture_output(argv.size(), &argv[0]);

    bool ok = (got == expected);
    std::cout << (ok ? "[PASS]" : "[FAIL]") << " " << label << '\n';
    if (!ok) {
      std::cout << "       expected : \"" << expected << "\"\n";
      std::cout << "       got      : \"" << got << "\"\n";
    }
    ok ? ++passed : ++failed;

    for (size_t i = 1; i < argv.size(); ++i) {
      delete[] argv[i];
    }
  }

  void run_performance(const std::string& label, const std::string& pattern,
                       int num_args, size_t expected_length) {
    std::cout << "\n── Performance: " << label << " ───────────\n";
    std::vector<char*> argv;
    char prog_name[] = "./megaphone";
    argv.push_back(prog_name);

    for (int i = 0; i < num_args; ++i) {
      char* buf = new char[pattern.length() + 1];
      std::memcpy(buf, pattern.c_str(), pattern.length() + 1);
      argv.push_back(buf);
    }

    std::clock_t start = std::clock();
    std::string got = capture_output(argv.size(), &argv[0]);
    std::clock_t end = std::clock();

    double elapsed_ms = 1000.0 * (end - start) / CLOCKS_PER_SEC;
    bool ok = (got.length() == expected_length);

    std::cout << (ok ? "[PASS]" : "[FAIL]") << " Processed in " << elapsed_ms
              << " ms.\n";
    if (!ok) {
      std::cout << "       expected length : " << expected_length << "\n";
      std::cout << "       got length      : " << got.length() << "\n";
    }

    for (size_t i = 1; i < argv.size(); ++i) {
      delete[] argv[i];
    }
  }

  void print_summary() {
    std::cout << "───────────────────────────────────────────\n";
    std::cout << "  Results: " << passed << " passed, " << failed
              << " failed\n";
    std::cout << "───────────────────────────────────────────\n";
  }
};

// ── Test Execution ───────────────────────────────────────────────────────────

// C++98 helper to easily create vectors from arrays
std::vector<std::string> make_vec(const char* a = NULL, const char* b = NULL,
                                  const char* c = NULL) {
  std::vector<std::string> v;
  if (a) v.push_back(a);
  if (b) v.push_back(b);
  if (c) v.push_back(c);
  return v;
}

int main() {
  std::cout << "═══════════════════════════════════════════\n";
  std::cout << "  Scalable Megaphone Test Suite  \n";
  std::cout << "═══════════════════════════════════════════\n\n";

  MegaphoneTester tester(optimized_solution);  // Pass your function here

  // ── basic tests ──
  tester.run_test("simple word", make_vec("hello"), "HELLO");
  tester.run_test("mixed case", make_vec("HeLLo WoRLd"), "HELLO WORLD");

  // NOTE: 42 megaphone standard behavior joins these WITHOUT spaces
  tester.run_test("multiple args", make_vec("foo", "bar", "baz"), "FOOBARBAZ");

  // ── whitespace tests ──
  tester.run_test("leading spaces", make_vec("   hello"), "HELLO");
  tester.run_test("trailing spaces", make_vec("hello   "), "HELLO");
  tester.run_test("internal spaces", make_vec("hello   world"), "HELLO WORLD");

  // ── boundary tests ──
  tester.run_test("boundary across args", make_vec("hello ", " world"),
                  "HELLO WORLD");

  // ── noise fallback tests ──
  std::string noise =
      "* LOUD AND UNBEARABLE FEEDBACK NOISE *";  // Check your macro text!
  tester.run_test("empty string", make_vec(""), noise);
  tester.run_test("spaces only", make_vec("   ", "  "), noise);

  // ── performance test ──
  // 10,000 args of "   heLLo woRLd   " -> "HELLO WORLD" (11 chars * 10,000 =
  // 110,000) Plus 9,999 spaces separating the arguments = 119,999 pure
  // characters.
  tester.run_performance("10k Heavy Whitespace", "   heLLo woRLd   ", 10000,
                         119999);

  tester.print_summary();

  return 0;
}
