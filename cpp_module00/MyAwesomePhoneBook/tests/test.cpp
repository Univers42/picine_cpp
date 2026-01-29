// filepath: c:\Users\dylan\Shared\picine_cpp\cpp_module00\MyAwesomePhoneBook\tests\test.cpp
#include <gtest/gtest.h>
#include "../PhoneBook.hpp"
#include "../Contact.hpp"

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

// Test that PhoneBook::seed sets up contacts as expected
TEST(PhoneBookTest, SeedPopulatesContacts) {
  PhoneBook pb;
  pb.seed();
  ASSERT_GE(pb.getCount(), 5);  // seed() loads at least 5 contacts
  EXPECT_EQ(pb.getContact(0).getField(Contact::FIELD_NAME), "Alice");
  EXPECT_EQ(pb.getContact(1).getField(Contact::FIELD_NAME), "Bob");
  EXPECT_EQ(pb.getContact(2).getField(Contact::FIELD_NAME), "Carol");
  EXPECT_EQ(pb.getContact(3).getField(Contact::FIELD_NAME), "Dave");
  EXPECT_EQ(pb.getContact(4).getField(Contact::FIELD_NAME), "Eve");
}

// Test that fields are preserved after seed
TEST(PhoneBookTest, SeedPreservesFields) {
  PhoneBook pb;
  pb.seed();
  EXPECT_EQ(pb.getContact(1).getField(Contact::FIELD_LASTNAME), "Martin");
  EXPECT_EQ(pb.getContact(1).getField(Contact::FIELD_NICKNAME), "Bobby");
  EXPECT_EQ(pb.getContact(1).getField(Contact::FIELD_PHONE), "+33123456789");
  EXPECT_EQ(pb.getContact(1).getField(Contact::FIELD_SECRET), "afraid of heights");
}

// Test that pushContact adds and preserves fields
TEST(PhoneBookTest, PushContactAddsAndPreservesFields) {
  PhoneBook pb;
  Contact c = make_contact("John", "Doe", "JD", "000111222", "none");
  bool ok = pb.pushContact(c);
  EXPECT_TRUE(ok);
  EXPECT_EQ(pb.getCount(), 1);
  EXPECT_EQ(pb.getContact(0).getField(Contact::FIELD_NAME), "John");
  EXPECT_EQ(pb.getContact(0).getField(Contact::FIELD_PHONE), "000111222");
}

// Test that pushContact rejects empty fields
TEST(PhoneBookTest, PushContactRejectsEmptyFields) {
  PhoneBook pb;
  Contact bad = make_contact("", "Doe", "JD", "000", "sec");
  EXPECT_FALSE(pb.pushContact(bad));
  EXPECT_EQ(pb.getCount(), 0);
  Contact bad2 = make_contact("A", "", "B", "1", "s");
  EXPECT_FALSE(pb.pushContact(bad2));
  EXPECT_EQ(pb.getCount(), 0);
}

// Test that overwrites the oldest contact when capacity is exceeded
TEST(PhoneBookTest, OverwriteOldestWhenCapacityExceeded) {
  PhoneBook pb;
  // push 8 contacts
  for (int i = 0; i < 8; ++i) {
    Contact c = make_contact("N"+std::to_string(i), "L"+std::to_string(i),
                             "nick"+std::to_string(i), "ph"+std::to_string(i), "s"+std::to_string(i));
    EXPECT_TRUE(pb.pushContact(c));
  }
  EXPECT_EQ(pb.getCount(), 8);
  // push 9th contact -> should overwrite index 0
  Contact c9 = make_contact("OVER", "WRITE", "ow", "999", "x");
  EXPECT_TRUE(pb.pushContact(c9));
  EXPECT_EQ(pb.getCount(), 8);
  EXPECT_EQ(pb.getContact(0).getField(Contact::FIELD_NAME), "OVER");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
