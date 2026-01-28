#include <gtest/gtest.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <vector>

// declare the C-style function
std::string megaphone_from_argv(int n, char* args[]);

// Helper function to check if a string is all uppercase (or non-alphabetic)
bool is_uppercase_or_non_alpha(const std::string& s) {
    for (std::size_t i = 0; i < s.length(); ++i) {
        unsigned char ch = static_cast<unsigned char>(s[i]);
        if (std::isalpha(ch) && !std::isupper(ch)) {
            return false;
        }
    }
    return true;
}

// Random string generator class
class RandomStringGenerator {
public:
    RandomStringGenerator() {
        std::srand(static_cast<unsigned int>(std::time(0)));
    }

    // Generate a random character (lowercase, uppercase, digit, or special)
    char random_char() {
        int type = std::rand() % 4;
        switch (type) {
            case 0: // lowercase letter
                return 'a' + (std::rand() % 26);
            case 1: // uppercase letter
                return 'A' + (std::rand() % 26);
            case 2: // digit
                return '0' + (std::rand() % 10);
            case 3: // special character
                {
                    const char specials[] = "!@#$%^&*()_+-=[]{}|;:',.<>?/~ ";
                    return specials[std::rand() % (sizeof(specials) - 1)];
                }
            default:
                return 'a';
        }
    }

    // Generate a random string of given length
    std::string generate(int min_len = 1, int max_len = 20) {
        int len = min_len + (std::rand() % (max_len - min_len + 1));
        std::string result;
        result.reserve(len);
        for (int i = 0; i < len; ++i) {
            result.push_back(random_char());
        }
        return result;
    }

    // Generate multiple random strings
    std::vector<std::string> generate_multiple(int count, int min_len = 1, int max_len = 20) {
        std::vector<std::string> results;
        results.reserve(count);
        for (int i = 0; i < count; ++i) {
            results.push_back(generate(min_len, max_len));
        }
        return results;
    }
};

// Fuzzy test with single random strings
TEST(FuzzyTest, SingleRandomString_1000_Iterations) {
    RandomStringGenerator gen;

    for (int i = 0; i < 1000; ++i) {
        std::string input = gen.generate(1, 30);

        // Convert to char* for function call
        char* args[] = { const_cast<char*>(input.c_str()) };

        // Get result
        std::string result = megaphone_from_argv(1, args);

        // Verify all output is uppercase
        EXPECT_TRUE(is_uppercase_or_non_alpha(result))
            << "Failed on iteration " << i
            << "\nInput: '" << input << "'"
            << "\nOutput: '" << result << "'";
    }
}

// Fuzzy test with multiple random strings
TEST(FuzzyTest, MultipleRandomStrings_1000_Iterations) {
    RandomStringGenerator gen;

    for (int i = 0; i < 1000; ++i) {
        // Generate 2-5 random strings
        int num_strings = 2 + (std::rand() % 4);
        std::vector<std::string> inputs = gen.generate_multiple(num_strings, 1, 15);

        // Convert to char** for function call
        std::vector<char*> args;
        for (std::size_t j = 0; j < inputs.size(); ++j) {
            args.push_back(const_cast<char*>(inputs[j].c_str()));
        }

        // Get result
        std::string result = megaphone_from_argv(args.size(), &args[0]);

        // Verify all output is uppercase
        EXPECT_TRUE(is_uppercase_or_non_alpha(result))
            << "Failed on iteration " << i
            << "\nInputs (" << inputs.size() << "): ";

        if (::testing::Test::HasFailure()) {
            for (std::size_t j = 0; j < inputs.size(); ++j) {
                std::cerr << "  [" << j << "]: '" << inputs[j] << "'\n";
            }
            std::cerr << "Output: '" << result << "'\n";
        }
    }
}

// Fuzzy test with edge cases
TEST(FuzzyTest, EdgeCases_EmptyAndShort_100_Iterations) {
    RandomStringGenerator gen;

    for (int i = 0; i < 100; ++i) {
        // Generate very short or empty strings
        std::string input = gen.generate(0, 3);

        if (input.empty()) {
            char* args[] = { const_cast<char*>(input.c_str()) };
            std::string result = megaphone_from_argv(1, args);
            EXPECT_EQ(result, "");
        } else {
            char* args[] = { const_cast<char*>(input.c_str()) };
            std::string result = megaphone_from_argv(1, args);
            EXPECT_TRUE(is_uppercase_or_non_alpha(result))
                << "Failed on short string iteration " << i;
        }
    }
}

// Fuzzy test with very long strings
TEST(FuzzyTest, LongStrings_100_Iterations) {
    RandomStringGenerator gen;

    for (int i = 0; i < 100; ++i) {
        std::string input = gen.generate(50, 200);

        char* args[] = { const_cast<char*>(input.c_str()) };
        std::string result = megaphone_from_argv(1, args);

        EXPECT_TRUE(is_uppercase_or_non_alpha(result))
            << "Failed on long string iteration " << i
            << "\nInput length: " << input.length()
            << "\nOutput length: " << result.length();
    }
}

// Fuzzy test with only lowercase input
TEST(FuzzyTest, OnlyLowercase_500_Iterations) {
    for (int i = 0; i < 500; ++i) {
        // Generate only lowercase letters
        int len = 1 + (std::rand() % 30);
        std::string input;
        input.reserve(len);
        for (int j = 0; j < len; ++j) {
            input.push_back('a' + (std::rand() % 26));
        }

        char* args[] = { const_cast<char*>(input.c_str()) };
        std::string result = megaphone_from_argv(1, args);

        // Result should be all uppercase
        EXPECT_TRUE(is_uppercase_or_non_alpha(result))
            << "Failed on lowercase-only iteration " << i;

        // Verify length is preserved
        EXPECT_EQ(input.length(), result.length())
            << "Length mismatch on iteration " << i;
    }
}

// Fuzzy test with mixed alphabetic and special characters
TEST(FuzzyTest, MixedAlphaSpecial_500_Iterations) {
    RandomStringGenerator gen;

    for (int i = 0; i < 500; ++i) {
        std::string input = gen.generate(5, 25);

        char* args[] = { const_cast<char*>(input.c_str()) };
        std::string result = megaphone_from_argv(1, args);

        // All alphabetic characters should be uppercase
        EXPECT_TRUE(is_uppercase_or_non_alpha(result))
            << "Failed on mixed alpha/special iteration " << i;

        // Verify length is preserved
        EXPECT_EQ(input.length(), result.length())
            << "Length should be preserved on iteration " << i;
    }
}

// Stress test with many arguments
TEST(FuzzyTest, ManyArguments_100_Iterations) {
    RandomStringGenerator gen;

    for (int i = 0; i < 100; ++i) {
        // Generate 10-50 strings
        int num_strings = 10 + (std::rand() % 41);
        std::vector<std::string> inputs = gen.generate_multiple(num_strings, 1, 10);

        std::vector<char*> args;
        for (std::size_t j = 0; j < inputs.size(); ++j) {
            args.push_back(const_cast<char*>(inputs[j].c_str()));
        }

        std::string result = megaphone_from_argv(args.size(), &args[0]);

        EXPECT_TRUE(is_uppercase_or_non_alpha(result))
            << "Failed on many arguments iteration " << i
            << " with " << num_strings << " arguments";
    }
}

// Test that verifies uppercase conversion specifically
TEST(FuzzyTest, VerifyUppercaseConversion_1000_Iterations) {
    RandomStringGenerator gen;

    for (int i = 0; i < 1000; ++i) {
        std::string input = gen.generate(1, 20);

        char* args[] = { const_cast<char*>(input.c_str()) };
        std::string result = megaphone_from_argv(1, args);

        // For each character in input, verify corresponding output
        for (std::size_t j = 0; j < input.length(); ++j) {
            unsigned char in_ch = static_cast<unsigned char>(input[j]);
            unsigned char out_ch = static_cast<unsigned char>(result[j]);

            if (std::isalpha(in_ch)) {
                // Alphabetic chars should be uppercase
                EXPECT_TRUE(std::isupper(out_ch))
                    << "Character at position " << j
                    << " is not uppercase in iteration " << i
                    << "\nInput char: '" << input[j] << "' (ASCII " << (int)in_ch << ")"
                    << "\nOutput char: '" << result[j] << "' (ASCII " << (int)out_ch << ")";
            } else {
                // Non-alphabetic should be unchanged
                EXPECT_EQ(in_ch, out_ch)
                    << "Non-alphabetic character changed at position " << j
                    << " in iteration " << i;
            }
        }
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    // Set colored output
    ::testing::FLAGS_gtest_color = "yes";

    // Print test info
    std::cout << "\n=== Fuzzy Testing Megaphone ===" << std::endl;
    std::cout << "Running 3600+ iterations with random inputs..." << std::endl;
    std::cout << "Verifying all output is UPPERCASE\n" << std::endl;

    return RUN_ALL_TESTS();
}
