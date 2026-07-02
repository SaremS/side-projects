#include<iostream>
#include<vector>

using namespace std;


void update_tree(int fenwick_tree[], int n, int index, int val) {
    index = index + 1;

    while (index <= n) {
        fenwick_tree[index] += val;
        index += index & (-index);
    }
}

int get_sum(int fenwick_tree[], int index) {
    int sum = 0;
    index = index + 1;

    while (index > 0) {
        sum += fenwick_tree[index];
        index -= index & (-index);
    }

    return sum;
}

int* construct_tree(int arr[], int n) {
    int* fenwick_tree = new int[n + 1]{0};

    for (int i = 0; i < n; i++) {
        update_tree(fenwick_tree, n, i, arr[i]);
    }

    return fenwick_tree;
}

int main() {
    int freq[] = {2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9};
    int n = sizeof(freq) / sizeof(freq[0]);

    int* fenwick_tree = construct_tree(freq, n);
    cout << "Sum from [0..5]:" << get_sum(fenwick_tree, 5) << endl;
}
