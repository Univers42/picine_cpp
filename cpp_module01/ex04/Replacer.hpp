/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replacer.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 17:30:18 by dlesieur          #+#    #+#             */
/*   Updated: 2026/02/28 17:30:23 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLACER_HPP
# define REPLACER_HPP

# include <string>

class Replacer {
  private:
    std::string inFile;
    std::string outFile;
    std::string s1;
    std::string s2;

    // Helper for the Knuth-Morris-Pratt Algorithm
    void computeLPSArray(int* lps) const;

  public:
    Replacer(const std::string& filename, const std::string& s1, const std::string& s2);
    ~Replacer();

    bool process();
};

#endif
