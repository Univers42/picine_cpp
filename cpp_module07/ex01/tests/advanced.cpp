/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   advanced.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlesieur <dlesieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 11:49:33 by dlesieur          #+#    #+#             */
/*   Updated: 2026/03/01 11:49:40 by dlesieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

#include "../iter.hpp"

/* ========================================================================== */
/* 1. CUSTOM STRUCT (Complex Data Type)                                       */
/* ========================================================================== */
struct Player {
  std::string name;
  int health;
};

// Callback to heal the player (Non-const: modifies data)
void healPlayer(Player& p) {
  p.health = 100;
  std::cout << p.name << " was fully healed!" << std::endl;
}

// Callback to print the player (Const: read-only)
void printPlayer(const Player& p) {
  std::cout << "[" << p.name << " | HP: " << p.health << "] ";
}

/* ========================================================================== */
/* 2. LINKED LIST NODE (Demonstrating Array of Pointers)                      */
/* ========================================================================== */
struct Node {
  std::string data;
  Node* next;
};

// Callback to read a linked list node through a pointer
void readNode(Node* const& nodePtr) {
  if (nodePtr) {
    std::cout << "Node Data: " << nodePtr->data << std::endl;
  }
}

/* ========================================================================== */
/* 3. GENERIC TEMPLATE CALLBACK (For any printable type)                      */
/* ========================================================================== */
template <typename T>
void display(const T& item) {
  std::cout << item << " ~ ";
}

/* ========================================================================== */
/* MAIN ROUTINE                                                               */
/* ========================================================================== */
int main(void) {
  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- TEST 1: Array of Custom Structs (Players) ---" << std::endl;
  std::cout << "========================================================\n";

  Player team[] = {{"Cloud", 45}, {"Tifa", 12}, {"Barret", 88}};
  const size_t teamSize = sizeof(team) / sizeof(team[0]);

  std::cout << "Status before healing:\n";
  ::iter(team, teamSize, printPlayer);
  std::cout << "\n\nApplying heal function to all...\n";
  ::iter(team, teamSize, healPlayer);

  std::cout << "\nStatus after healing:\n";
  ::iter(team, teamSize, printPlayer);
  std::cout << "\n";

  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- TEST 2: Array of Strings (Instantiated Template) ---"
            << std::endl;
  std::cout << "========================================================\n";

  std::string inventory[] = {"Buster Sword", "Potion", "Phoenix Down",
                             "Materia"};
  const size_t invSize = sizeof(inventory) / sizeof(inventory[0]);

  // Using an instantiated function template as the callback!
  ::iter(inventory, invSize, display<std::string>);
  std::cout << "\n";

  std::cout << "\n========================================================"
            << std::endl;
  std::cout << "--- TEST 3: Array of Linked List Node Pointers ---"
            << std::endl;
  std::cout << "========================================================\n";

  // We cannot iterate a standard linked list with iter, but we CAN iterate
  // an array that holds pointers to our scattered linked list nodes!
  Node node3 = {"End of List", NULL};
  Node node2 = {"Middle Node", &node3};
  Node node1 = {"Head Node", &node2};

  // Creating an array of pointers to those nodes
  Node* listPointers[] = {&node1, &node2, &node3};
  const size_t ptrSize = sizeof(listPointers) / sizeof(listPointers[0]);

  ::iter(listPointers, ptrSize, readNode);

  std::cout << "\n========================================================"
            << std::endl;
  return 0;
}
