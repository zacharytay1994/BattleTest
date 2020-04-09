// BattleTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <queue>
#include <vector>
#include "Character.h"
#include "Minimax.h"
#include <string>

std::string ReturnMove(const int& i) {
    switch (i) {
    case 0:
        return "None";
        break;
    case 1:
        return "Attack 1";
        break;
    case 2:
        return "Attack 2";
        break;
    case 3:
        return "Swap Right";
        break;
    case 4:
        return "Swap Left";
        break;
    }
}

int main()
{
    std::vector<Character> characters;
    characters.push_back({ 80, 10, 5, 1, false, 0, 0 });
    characters.push_back({ 80, 10, 2, 1, false, 1, 0 });
    characters.push_back({ 80, 10, 2, 1, true, 2, 0 });
    characters.push_back({ 80, 10, 2, 2, true, 3, 0 });
    Board board = Board(characters, Character::Move::None);

    /*std::vector<Board> boards = board.GenerateBoardSet(6);

    std::cout << "Input Set: ";
    for(Character c : characters) {
        std::cout << "I:" << c.id_ << "H:" << c.health_ << " | ";
    }
    std::cout << std::endl;
    
    for (Board b : boards) {
        std::cout << "Output Set, move = " << static_cast<int>(b.move_) << ": ";
        for (Character c : b.characters_) {
            std::cout << "I:" << c.id_ << "H:" << c.health_ << " | ";
        }
        std::cout << std::endl;
    }*/

    Minimax minimax = Minimax(10);
    std::queue<int> turn_order;
    turn_order.push(0);
    turn_order.push(1);
    turn_order.push(2);
    turn_order.push(3);
    turn_order.push(0);
    turn_order.push(1);
    turn_order.push(2);
    turn_order.push(3);
    turn_order.push(0);
    turn_order.push(1);
    turn_order.push(2);
    turn_order.push(3);
    turn_order.push(0);
    turn_order.push(1);
    turn_order.push(2);
    turn_order.push(3);

    while (turn_order.size() > 0) {
        int score = -1;
        Character::Move move = Character::Move::None;
        if (turn_order.front() > 1) {
            score = minimax.Maximize(board, move, 0, turn_order);
        }
        else {
            score = minimax.Minimize(board, move, 0, turn_order);
        }
        std::cout << "Score is: " << score << std::endl;
        std::cout << "Move is: " << ReturnMove(static_cast<int>(move)) << std::endl;
        // if is attack move, execute attack
        if (static_cast<int>(move) > 0 && static_cast<int>(move) < 3) {
            board = board.Attack(static_cast<int>(move), turn_order.front());
        }
        else if (static_cast<int>(move) >= 3) {
            board = board.Swap(static_cast<int>(move), turn_order.front());
        }
        std::cout << "Output Set, move = " << static_cast<int>(board.move_) << ": ";
        for (Character c : board.characters_) {
            std::cout << "I:" << c.id_ << "H:" << c.health_ << " | ";
        }
        std::cout << std::endl;
        turn_order.pop();
    }
}
