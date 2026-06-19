#include<iostream>
#include<sstream>
#include<stack>
#include<string>
#include<utility>

int main() {
    std::string line;

    std::getline(std::cin, line);
    std::istringstream iss(line);
    int x;

    std::stack<std::pair<int, int>> st;
    while (iss >> x) {
        int new_min = st.empty() ? x : std::min(x, st.top().second);
        st.push({x, new_min});
    }

    int minimum = st.top().second;

    std::cout << minimum << std::endl;
    
    return 0;
}
