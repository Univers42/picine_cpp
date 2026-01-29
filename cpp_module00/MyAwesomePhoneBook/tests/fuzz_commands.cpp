#include <gtest/gtest.h>
#include "../PhoneBook.hpp"
#include "../Contact.hpp"
#include <sstream>
#include <streambuf>
#include <string>

// Redirect helpers
struct CinRedirect {
  std::streambuf* old;
  std::istringstream in;
  CinRedirect(const std::string& s) : old(std::cin.rdbuf()), in(s) { std::cin.rdbuf(in.rdbuf()); }
  ~CinRedirect() { std::cin.rdbuf(old); }
};
struct CoutRedirect {
  std::streambuf* old;
  std::ostringstream out;
  CoutRedirect() : old(std::cout.rdbuf()) { std::cout.rdbuf(out.rdbuf()); }
  ~CoutRedirect() { std::cout.rdbuf(old); }
  std::string str() const { return out.str(); }
};

// helper to create a contact with given fields
static Contact make_contact(const std::string& n, const std::string& ln,
                            const std::string& nn, const std::string& ph,
                            const std::string& sec) {
  Contact c;
  c.setField(Contact::FIELD_NAME, n);
  c.setField(Contact::FIELD_LASTNAME, ln);
  c.setField(Contact::FIELD_NICKNAME, nn);
  c.setField(Contact::FIELD_PHONE, ph);
  c.setField(Contact::FIELD_SECRET, sec);
  return c;
}

// 1) Whitespace-only fields should be rejected by interactive add().
TEST(FuzzCommands, AddRejectsWhitespaceOnlyFields) {
  PhoneBook pb;
  // simulate entering whitespace-only name then other fields
  std::string input = "   \nDoe\nNick\n012345\nsecret\n";
  CinRedirect in(input);
  pb.add();
  EXPECT_EQ(pb.getCount(), 0);
}

// 2) SEARCH handles invalid index (out-of-range) and prints "Invalid index."
TEST(FuzzCommands, SearchInvalidIndexHandled) {
  PhoneBook pb;
  pb.seed();
  CoutRedirect coutr;
  CinRedirect incr("999\n\n"); // invalid then cancel
  pb.search();
  std::string out = coutr.str();
  EXPECT_NE(std::string::npos, out.find("Invalid index."));
}

// 3) SEARCH valid index prints one-field-per-line output for that contact.
TEST(FuzzCommands, SearchValidIndexPrintsFields) {
  PhoneBook pb;
  pb.seed();
  CoutRedirect coutr;
  CinRedirect incr("1\n"); // valid index
  pb.search();
  std::string out = coutr.str();
  EXPECT_NE(std::string::npos, out.find("First Name:"));
  EXPECT_NE(std::string::npos, out.find("Last Name:"));
  EXPECT_NE(std::string::npos, out.find("Phone:"));
}

// 4) pushContact accepts very long strings without crashing and preserves data.
TEST(FuzzCommands, PushContactLongStringsNoCrash) {
  PhoneBook pb;
  std::string longs(5000, 'x');
  Contact c = make_contact(longs, longs, longs, longs, longs);
  EXPECT_TRUE(pb.pushContact(c));
  EXPECT_EQ(pb.getCount(), 1);
  EXPECT_EQ(pb.getContact(0).getField(Contact::FIELD_NAME), longs);
}

// 5) Mixed-case/trailing spaces in fields: pushContact stores values as provided.
TEST(FuzzCommands, PushContactMixedCasePreserved) {
  PhoneBook pb;
  Contact c = make_contact("  Alice  ", "DuPont", "aLiCe", "0123", "secret");
  EXPECT_TRUE(pb.pushContact(c));
  EXPECT_EQ(pb.getCount(), 1);
  EXPECT_EQ(pb.getContact(0).getField(Contact::FIELD_LASTNAME), "DuPont");
}

// 6) SEARCH output: columns are 10 chars wide, right-aligned, truncated with dot if too long.
TEST(FuzzCommands, SearchColumnFormat_TruncationAndAlignment) {
  PhoneBook pb;
  // Add a contact with long fields
  Contact c = make_contact("FirstnameIsWayTooLong", "LastnameIsAlsoLong", "NickIsLong", "0123456789", "secret");
  pb.pushContact(c);
  CoutRedirect coutr;
  CinRedirect incr("\n"); // cancel after listing
  pb.search();
  std::string out = coutr.str();

  // Check for 10-char columns, right-aligned, truncated with dot
  std::string expected = " Firstnam."; // right-aligned in 10 chars
  EXPECT_NE(std::string::npos, out.find(expected)) << "First name not truncated/formatted as expected";

  // Check for pipe separators and column width
  size_t line_pos = out.find('|');
  ASSERT_NE(line_pos, std::string::npos) << "No pipe separator found in output";
  // Find the first data line (skip header)
  std::istringstream iss(out);
  std::string line;
  while (std::getline(iss, line)) {
    if (line.find('|') != std::string::npos && line.find("index") == std::string::npos) {
      // Should be data line
      std::vector<std::string> cols;
      size_t prev = 0, pos;
      while ((pos = line.find('|', prev)) != std::string::npos) {
        cols.push_back(line.substr(prev, pos - prev));
        prev = pos + 1;
      }
      cols.push_back(line.substr(prev));
      // All columns should be 10 chars
      for (size_t i = 0; i < cols.size(); ++i) {
        EXPECT_EQ(static_cast<int>(cols[i].size()), 10) << "Column " << i << " is not 10 chars wide: '" << cols[i] << "'";
      }
      break;
    }
  }
}

// 7) SEARCH output: short fields are right-aligned in their columns.
TEST(FuzzCommands, SearchColumnFormat_RightAlignedShortFields) {
  PhoneBook pb;
  Contact c = make_contact("A", "B", "C", "0123", "secret");
  pb.pushContact(c);
  CoutRedirect coutr;
  CinRedirect incr("\n");
  pb.search();
  std::string out = coutr.str();

  // Should see right-aligned single-letter fields
  std::istringstream iss(out);
  std::string line;
  while (std::getline(iss, line)) {
    if (line.find('|') != std::string::npos && line.find("index") == std::string::npos) {
      // Data line
      size_t idx = line.find('|');
      ASSERT_NE(idx, std::string::npos);
      std::string first_col = line.substr(0, idx);
      std::string second_col = line.substr(idx + 1, 10);
      // Should be 9 spaces then 'A'
      EXPECT_EQ(second_col, "         A") << "Short field not right-aligned: '" << second_col << "'";
      break;
    }
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
