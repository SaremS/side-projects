#include<iostream>
#include<deque>
#include<string>
#include<sstream>

int main() {
    std::string line;

    std::getline(std::cin, line);
    std::istringstream iss(line);
    int x;

    std::deque<int> q;
    while (iss >> x) {
        while (!q.empty() && q.back() > x)
            q.pop_back();
        q.push_back(x);
    }

    int minimum = q.front();

    std::cout << minimum << std::endl;
    
    return 0;
}
