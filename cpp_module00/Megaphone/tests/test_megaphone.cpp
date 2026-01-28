#include <gtest/gtest.h>
#include <string>

// declare the C-style function
std::string megaphone_from_argv(int n, char* args[]);

// Basic functionality tests
TEST(MegaphoneTest, NoArgsReturnsFeedback) {
  EXPECT_EQ(megaphone_from_argv(0, (char**)0), "* LOUD AND UNBEARABLE FEEDBACK NOISE *");
}

TEST(MegaphoneTest, SingleWord) {
  char* a[] = { (char*)"hello" };
  EXPECT_EQ(megaphone_from_argv(1, a), "HELLO");
}

TEST(MegaphoneTest, MultipleWords) {
  char* a[] = { (char*)"hello", (char*)"world" };
  EXPECT_EQ(megaphone_from_argv(2, a), "HELLO WORLD");
}

// Edge cases
TEST(MegaphoneTest, EmptyString) {
  char* a[] = { (char*)"" };
  EXPECT_EQ(megaphone_from_argv(1, a), "");
}

TEST(MegaphoneTest, AlreadyUppercase) {
  char* a[] = { (char*)"HELLO" };
  EXPECT_EQ(megaphone_from_argv(1, a), "HELLO");
}

TEST(MegaphoneTest, MixedCase) {
  char* a[] = { (char*)"HeLLo", (char*)"WoRLd" };
  EXPECT_EQ(megaphone_from_argv(2, a), "HELLO WORLD");
}

TEST(MegaphoneTest, SpecialCharacters) {
  char* a[] = { (char*)"hello!", (char*)"world?" };
  EXPECT_EQ(megaphone_from_argv(2, a), "HELLO! WORLD?");
}

TEST(MegaphoneTest, NumbersAndSymbols) {
  char* a[] = { (char*)"test123", (char*)"@#$" };
  EXPECT_EQ(megaphone_from_argv(2, a), "TEST123 @#$");
}

TEST(MegaphoneTest, SingleCharacter) {
  char* a[] = { (char*)"a" };
  EXPECT_EQ(megaphone_from_argv(1, a), "A");
}

TEST(MegaphoneTest, MultipleEmptyStrings) {
  char* a[] = { (char*)"", (char*)"", (char*)"" };
  EXPECT_EQ(megaphone_from_argv(3, a), "  ");
}

TEST(MegaphoneTest, LongSentence) {
  char* a[] = { (char*)"this", (char*)"is", (char*)"a", (char*)"very", (char*)"long", (char*)"sentence" };
  EXPECT_EQ(megaphone_from_argv(6, a), "THIS IS A VERY LONG SENTENCE");
}

TEST(MegaphoneTest, Sensibility) {
  char* a[] = { (char*)"tHiS", (char*)"IS", (char*)"a", (char*)"vErY", (char*)"LonG", (char*)"SeNtEnCe" };
  EXPECT_EQ(megaphone_from_argv(6, a), "THIS IS A VERY LONG SENTENCE");
}

// Custom main with colored output
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);

  // Set colored output explicitly
  ::testing::FLAGS_gtest_color = "yes";

  return RUN_ALL_TESTS();
}
