#pragma once

class Character {
public:
    enum class Move {
        None,
        AttackOne,
        AttackTwo,
        SwapRight,
        SwapLeft
    };
public:
    int health_ = 0;
    int speed_ = 0;
    int attack_ = 0;
    int range_ = 0;
    bool friendly_ = true;
    bool dead_ = false;
    int id_ = -1;
    int defence_ = 0;
public:
    Character() = default;
    Character(const int& health, const int& speed, const int& attack, const int& range, const bool& friendly, const int& id, const int& defence)
        :
        health_(health),
        speed_(speed),
        attack_(attack),
        range_(range),
        friendly_(friendly),
        id_(id),
        defence_(defence)
    {}
    void TakeDamage(const int& damage) {
        health_ -= damage;
        health_ += defence_;
        if (health_ <= 0) {
            dead_ = true;
        }
    }
    bool Friendly() {
        return friendly_;
    }
    int Health() {
        return health_;
    }
};

struct BoardData {
    std::vector<Character> characters_;
    Character::Move move_;
    BoardData(std::vector<Character> characters, Character::Move move)
        :
        characters_(characters),
        move_(move)
    {}
};

class Board {
public:
    int board_size_ = 2;
    std::vector<Character> characters_;
    // the move the caused this board formation
    Character::Move move_;
public:
    Board(std::vector<Character> characters, Character::Move move)
        :
        characters_(characters),
        move_(move)
    {}
    Board(const Board& board) 
        :
        characters_(board.characters_),
        move_(board.move_)
    {}
    std::vector<Board> GenerateBoardSet(const int& id) {
        std::vector<Board> new_set;
        int index = GetIndex(id);
        if (index >= board_size_) {
            // friendly
            // generate attack set
            if ((index - characters_[index].range_) < board_size_) {
                // if in range to hit an enemy
                for (int i = board_size_-1; i >= index - characters_[index].range_; i--) {
                    // create copy for new set and push boarddata into new set
                    std::vector<Character> copy = characters_;
                    copy[i].TakeDamage(characters_[index].attack_);
                    new_set.push_back({ copy, static_cast<Character::Move>(board_size_ - i) });
                }
            }
            // generate swap set
            // if can swap left
            if (index >= board_size_+1) {
                std::vector<Character> copy = characters_;
                copy[index] = characters_[index - 1];
                copy[index - 1] = characters_[index];
                new_set.push_back({ copy, Character::Move::SwapLeft });
            }
            // if can swap right
            if (index <= board_size_*2-2) {
                std::vector<Character> copy = characters_;
                copy[index] = characters_[index + 1];
                copy[index + 1] = characters_[index];
                new_set.push_back({ copy, Character::Move::SwapRight });
            }
            // do nothing
            new_set.push_back({ characters_, Character::Move::None });
        }
        else {
            // unfriendly
            // generate attack set
            if ((index + characters_[index].range_) > board_size_-1) {
                // if in range to hit an enemy
                for (int i = board_size_; i <= index + characters_[index].range_; i++) {
                    // create copy for new set and push boarddata into new set
                    std::vector<Character> copy = characters_;
                    copy[i].TakeDamage(characters_[index].attack_);
                    new_set.push_back({ copy, static_cast<Character::Move>(i - (board_size_-1)) });
                }
            }
            // generate swap set
            // if can swap left
            if (index >= 1) {
                std::vector<Character> copy = characters_;
                copy[index] = characters_[index - 1];
                copy[index - 1] = characters_[index];
                new_set.push_back({ copy, Character::Move::SwapLeft });
            }
            // if can swap right
            if (index <= board_size_-2) {
                std::vector<Character> copy = characters_;
                copy[index] = characters_[index + 1];
                copy[index + 1] = characters_[index];
                new_set.push_back({ copy, Character::Move::SwapRight });
            }
            // do nothing
            new_set.push_back({ characters_, Character::Move::None });
        }
        return new_set;
    }

    int GetIndex(const int& id) {
        for (int i = 0; i < board_size_*2; i++) {
            if (characters_[i].id_ == id) {
                return i;
            }
        }
    }

    int GenerateCost() {
        // friendly cost
        int friendly = 0;
        for (int i = board_size_; i < board_size_*2; i++) {
            friendly += characters_[i].health_;
        }
        // unfriendly cost
        int unfriendly = 0;
        for (int i = 0; i < board_size_; i++) {
            unfriendly += characters_[i].health_;
        }
        return friendly - unfriendly;
    }

    Board Attack(const int& i, const int& id) {
        int index = GetIndex(id);
        std::vector<Character> copy = characters_;
        if (index >= board_size_) {
            // friendly
            copy[board_size_ - i].TakeDamage(characters_[index].attack_);
        }
        else {
            // unfriendly
            copy[(board_size_-1) + i].TakeDamage(characters_[index].attack_);
        }
        return { copy, Character::Move::None };
    }

    Board Swap(const int& i, const int& id) {
        int index = GetIndex(id);
        std::vector<Character> copy = characters_;
        // if swap right
        if (i == 3) {
            copy[index] = characters_[index + 1];
            copy[index + 1] = characters_[index];
        }
        else {
            copy[index] = characters_[index - 1];
            copy[index - 1] = characters_[index];
        }
        return { copy, Character::Move::None };
    }

    int MaxBoardSet(Character::Move& move, const int& id) {
        int max = -std::numeric_limits<int>::max();
        for (Board b : GenerateBoardSet(id)) {
            int cost = b.GenerateCost();
            if (cost > max) {
                max = cost;
                move = b.move_;
            }
        }
        return max;
    }

    int MinBoardSet(Character::Move& move, const int& id) {
        int min = std::numeric_limits<int>::max();
        for (Board b : GenerateBoardSet(id)) {
            int cost = b.GenerateCost();
            if (cost < min) {
                min = cost;
                move = b.move_;
            }
        }
        return min;
    }
};