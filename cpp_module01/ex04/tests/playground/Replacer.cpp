/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replacer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 17:30:16 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 17:30:29 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Replacer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

Replacer::Replacer(const std::string& filename, const std::string& s1, const std::string& s2)
    : inFile(filename), outFile(filename + ".replace"), s1(s1), s2(s2) {}

Replacer::~Replacer() {}

// Pre-processes the pattern to calculate the LPS (Longest Prefix Suffix) array
void Replacer::computeLPSArray(int* lps) const {
    int len = 0;
    lps[0] = 0;
    int i = 1;
    int M = this->s1.length();

    while (i < M) {
        if (this->s1[i] == this->s1[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

bool Replacer::process() {
    if (this->s1.empty()) {
        std::cerr << "Error: The search string cannot be empty.\n";
        return false;
    }

    std::ifstream ifs(this->inFile.c_str(), std::ios::in | std::ios::binary);
    if (!ifs.is_open()) {
        std::cerr << "Error: Cannot open input file '" << this->inFile << "'.\n";
        return false;
    }

    std::ofstream ofs(this->outFile.c_str(), std::ios::out | std::ios::binary);
    if (!ofs.is_open()) {
        std::cerr << "Error: Cannot create output file '" << this->outFile << "'.\n";
        ifs.close();
        return false;
    }

    // Bulk-load file into memory for maximum speed
    std::ostringstream oss;
    oss << ifs.rdbuf();
    std::string text = oss.str();

    int N = text.length();
    int M = this->s1.length();

    // Dynamically allocate the LPS array because <vector> is forbidden!
    int* lps = new int[M];
    computeLPSArray(lps);

    int i = 0; // Index for text
    int j = 0; // Index for s1 (pattern)
    int last_match_end = 0; // Tracks the last index we safely wrote to the file

    // Knuth-Morris-Pratt Search Loop O(N)
    while (i < N) {
        if (this->s1[j] == text[i]) {
            j++;
            i++;
        }

        if (j == M) {
            // MATCH FOUND!
            // 1. Write the untouched text before the match
            ofs << text.substr(last_match_end, i - M - last_match_end);
            // 2. Write the replacement string
            ofs << this->s2;

            last_match_end = i;
            j = 0; // Reset pattern index to prevent overlapping replacements
        }
        else if (i < N && this->s1[j] != text[i]) {
            // Mismatch after j matches. Use the LPS array to jump!
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    // Write any remaining text left over at the end of the file
    if (last_match_end < N) {
        ofs << text.substr(last_match_end);
    }

    // Free the dynamically allocated array to prevent memory leaks
    delete[] lps;

    ifs.close();
    ofs.close();

    return true;
}
