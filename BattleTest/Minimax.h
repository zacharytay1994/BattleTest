#pragma once
#include <vector>
#include <queue>
#include "Character.h"

class Minimax {
private:
    int depth_;
public:
    Minimax(const int& depth)
        :
        depth_(depth)
    {

    }
    int Maximize(Board b, Character::Move& move, const int& depth, std::queue<int> turn_order) {
        // if reach the depth
        if (depth >= depth_) {
            // return the cost
            int cost = b.MaxBoardSet(move, turn_order.front());
            return cost;
        }
        if (turn_order.size() <= 1) {
            int cost = b.MaxBoardSet(move, turn_order.front());
            return cost;
        }
        // generate each possible set of next moves
        int maximum_cost = -std::numeric_limits<int>::max();
        int id = turn_order.front();
        turn_order.pop();
        for (Board board : b.GenerateBoardSet(id)) {
            Character::Move temp_move = Character::Move::None;
            // if next in line is friendly, call maximize
            if (b.characters_[b.GetIndex(turn_order.front())].friendly_) {
                int val = Maximize(board, temp_move, depth + 1, turn_order);
                if (val > maximum_cost) {
                    maximum_cost = val;
                    move = board.move_;
                }
            }
            // if next in line is not friendly, call minimize
            else {
                int val = Minimize(board, temp_move, depth + 1, turn_order);
                if (val > maximum_cost) {
                    maximum_cost = val;
                    move = board.move_;
                }
            }
        }
        return maximum_cost;
    }
    int Minimize(Board b, Character::Move& move, const int& depth, std::queue<int> turn_order) {
        // if reach the depth
        if (depth >= depth_) {
            // return the cost
            int cost = b.MinBoardSet(move, turn_order.front());
            return cost;
        }
        if (turn_order.size() <= 1) {
            int cost = b.MinBoardSet(move, turn_order.front());
            return cost;
        }
        // generate each possible set of next moves
        int minimum_cost = std::numeric_limits<int>::max();
        int id = turn_order.front();
        turn_order.pop();
        for (Board board : b.GenerateBoardSet(id)) {
            Character::Move temp_move = Character::Move::None;
            // if next in line is friendly, call maximize
            if (b.characters_[b.GetIndex(turn_order.front())].friendly_) {
                int val = Maximize(board, temp_move, depth + 1, turn_order);
                if (val < minimum_cost) {
                    minimum_cost = val;
                    move = board.move_;
                }
            }
            // if next in line is not friendly, call minimize
            else {
                int val = Minimize(board, temp_move, depth + 1, turn_order);
                if (val < minimum_cost) {
                    minimum_cost = val;
                    move = board.move_;
                }
            }
        }
        return minimum_cost;
    }
};