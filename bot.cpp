#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <assert.h>
#include <forward_list>
#include <algorithm>

std::vector<char> byFrequency {'e', 's', 'a', 'r', 'o', 'l', 'i', 'c', 't', 'd', 'n', 'b', 'u', 'h', 'p', 'y', 'm', 'f', 'g', 'k', 'w', 'v', 'x', 'z', 'j', 'q'};

enum colors {gray = 48, yellow, green};

class word
{
private:
    std::vector<char> byOrder;
    std::set<char> byLetters;
    long score = 0;

public:
    word(std::string input)
    {
        assert(input.length() == 5);
        
        for (auto c : input)
        {
            byOrder.push_back(c);
            byLetters.insert(c);
        }

        for (char c : byLetters)
        {
            for (size_t i = 0; i < 26; i++)
            {
                if (c == byFrequency.at(i))
                {
                    score += 26-i;
                    break;
                }
            }
        }
        score *= byLetters.size();  
    }

    const bool holds(char c)
    {
        if (byLetters.find(c) == byLetters.end())
            return false;
        return true;
    }

    const int at(int i){
        return byOrder.at(i);
    }

    const bool holdsFromList(const std::vector<char> &letterList) 
    {
        for (char c : letterList)
        {
            if (this->holds(c))
            {
                return true;
            }
        }
        return false;
    }

    const bool holdsFromList(const std::set<char> &letterList) 
    {
        for (char c : letterList)
        {
            if (this->holds(c))
            {
                return true;
            }
        }
        return false;
    }

    const bool confirmedWrongOrder(const std::vector<char> givenOrder)
    {
        for (int i = 0; i < 5; i++)
        {
            if (givenOrder.at(i) == 'X')
                continue;
            
            if (givenOrder.at(i) != this->at(i))
            {
                return true;
            }
        }  

        return false;      
    }

    const bool fitsInPattern(const std::vector<char> &givenOrder, const std::set<char> &forbidden, const std::set<char> &givenLetters)
    {
        if (this->holdsFromList(forbidden))
            return false; // test gray

        for (char c : givenLetters)
        {
            if (this->byLetters.find(c) == byLetters.end())
                return false; // test yellow
        }

        if (this->confirmedWrongOrder(givenOrder))
            return false; //test green

        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const word &w);
};

std::ostream& operator<<(std::ostream& os, const word &w)
{
        for (size_t i = 0; i < 5; i++)
        {
            os << w.byOrder[i];
        }        
        return os;
}

int main(int argc, char const *argv[])
{
    // initialize: read words from file, put them in c style array -> better sorting
    std::ifstream input;
    input.open("5_letter_words.txt");
    std::string w;
    std::getline(input, w);
    
    // das hier vllt auslagern und abspeichern für schnelleres starten, brauchen ja eh ne wordlist
    std::vector<std::pair<word, long>> wordlist;
    while (w != "")
    {
        long score = 0;
        for (char c : w)
        {
            for (size_t i = 0; i < 26; i++)
            {
                if (c == byFrequency[i])
                {
                    score += 26-i;
                    break;
                }
            }
        }
        wordlist.push_back(std::make_pair(word(w), score));
        std::getline(input, w);
    }
    //maybe change the metric used to calculate word score

    std::sort(wordlist.begin(), wordlist.end(), [&](std::pair<word, long> a, std::pair<word, long> b){return a.second >= b.second;} );
    
    // convert to list -> fast deletion, order is kept at deletions
    //sort descending -> taking last element, pushing it keeps descending order, keeping most important words upfront 

    std::forward_list<word> linear_wordlist;
    while (wordlist.empty() == false)
    {
        linear_wordlist.push_front(wordlist.back().first);
        wordlist.pop_back();
    }    
    input.close();

    std::set<char> forbidden;
    std::vector<char> givenOrder {'X','X','X','X','X'};
    std::set<char> givenLetters;

    for (int i = 0; i < 6; i++)
    {
        std::string buf;

        std::cout << "Enter guess: ";
        std::cin >> buf;
        //std::cout << std::endl;
        word guess = word(buf);
        
        std::cout << "Enter response: ";
        std::cin >> buf;
        std::cout << std::endl;

        int j = 0;
        for (char c : buf) 
        {
            switch (c)
            {
            case gray:
                forbidden.insert(guess.at(j));
                break;
            case green:
                givenLetters.insert(guess.at(j));
                givenOrder[j] = guess.at(j);

                break;
            case yellow:
                // TODO: given order at j not c
                givenLetters.insert(guess.at(j));
                break;
            
            default:
                break;
            }
            j++;
        }

        for (char c : forbidden)
        {
            std::cout << c << " ";
        }
        std::cout << "<< Letters known not to be in the word." << std::endl;
        for (char c : givenLetters)
        {
            std::cout << c << " ";
        }
        std::cout << "<< Letters known to be in word." << std::endl;
        for (char c : givenOrder)
        {
            std::cout << c << " ";
        }
        std::cout << "<< Known order." << std::endl << std::endl;

        //check logic: 
        for (char c : givenLetters)
        {
            if (forbidden.find(c) != forbidden.end())
            {
                std::cout << "There appears to be an logic error" << std::endl;
                return 1;
            }
        }

        int num = linear_wordlist.remove_if([&](word w){ return !w.fitsInPattern(givenOrder, forbidden, givenLetters); }); // compile with cpp 20
        std::cout << "Removed " << num << " entries from list."<< std::endl << std::endl;

        char c;
        do
        {   
            if (linear_wordlist.empty())
                std::cout <<" hm ungut" << std::endl;
            else 
            {
                std::cout << "Suggestion: " << linear_wordlist.front() << std::endl <<  "Different suggestion? (y/n) ";
                linear_wordlist.pop_front();
            }
            std::cin >> c;
            std::cout << std::endl;
        } while (c != 'n');
        
        
    }
}
