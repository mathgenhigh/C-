#include <iostream>
#include <vector>
#include <chrono>
#include <cmath>
#include <stdexcept>

class Heap {
private:
    std::vector<int> heap;
    bool isMinHeap;

    void heapifyUp(int index) {
        int current = index;
        while (current > 0 && compare(heap[parent(current)], heap[current])) {
            std::swap(heap[current], heap[parent(current)]);
            current = parent(current);
        }
    }

    void heapifyDown(int index) {
        int current = index;
        while (true) {
            int left = leftChild(current);
            int right = rightChild(current);
            int smallestOrLargest = current;

            if (left < static_cast<int>(heap.size()) && compare(heap[smallestOrLargest], heap[left])) {
                smallestOrLargest = left;
            }
            if (right < static_cast<int>(heap.size()) && compare(heap[smallestOrLargest], heap[right])) {
                smallestOrLargest = right;
            }

            if (smallestOrLargest == current) break;

            std::swap(heap[current], heap[smallestOrLargest]);
            current = smallestOrLargest;
        }
    }

    int parent(int index) const { return (index - 1) / 2; }
    int leftChild(int index) const { return 2 * index + 1; }
    int rightChild(int index) const { return 2 * index + 2; }

    bool compare(int a, int b) const {
        return isMinHeap ? a > b : a < b;
    }

public:
    Heap(bool minHeap = false) : isMinHeap(minHeap) {
        heap.reserve(1000);
    }

    void push(int element) {
        heap.push_back(element);
        heapifyUp(heap.size() - 1);
    }

    void pop() {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
    }

    int top() const {
        if (heap.empty()) {
            throw std::runtime_error("Heap is empty");
        }
        return heap[0];
    }

    bool empty() const {
        return heap.empty();
    }

    size_t size() const {
        return heap.size();
    }
};

class MedianQueue {
private:
    Heap maxHeap;
    Heap minHeap;

public:
    MedianQueue() : maxHeap(false), minHeap(true) {}

    void insert(int num) {
        if (maxHeap.empty() || num <= maxHeap.top()) {
            maxHeap.push(num);
        } else {
            minHeap.push(num);
        }

        if (maxHeap.size() > minHeap.size() + 1) {
            minHeap.push(maxHeap.top());
            maxHeap.pop();
        } else if (minHeap.size() > maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }
    }

    void batchInsert(const std::vector<int>& nums) {
        for (int num : nums) {
            insert(num);
        }
    }

    int deleteMedian() {
        if (maxHeap.empty()) throw std::runtime_error("Median Queue is empty");

        int median;
        if (maxHeap.size() >= minHeap.size()) {
            median = maxHeap.top();
            maxHeap.pop();
        } else {
            median = minHeap.top();
            minHeap.pop();
        }

        if (minHeap.size() < maxHeap.size()) {
            maxHeap.push(minHeap.top());
            minHeap.pop();
        }

        return median;
    }

    void batchDeleteMedian() {
        while (!maxHeap.empty() || !minHeap.empty()) {
            std::cout << "Deleted median: " << deleteMedian() << std::endl;
        }
    }

    bool empty() const {
        return maxHeap.empty() && minHeap.empty();
    }

    double getMedian() const {
        if (maxHeap.empty()) {
            throw std::runtime_error("MedianQueue is empty");
        }

        if (maxHeap.size() == minHeap.size()) {
            return (maxHeap.top() + minHeap.top()) / 2.0;
        } else {
            return maxHeap.top();
        }
    }
};

int main() {
    int size;
    std::cout << "Enter the number of elements: ";
    std::cin >> size;

    std::vector<int> numbers(size);
    for (int i = 1; i <= size; ++i) {
        numbers[i - 1] = i;
    }

    MedianQueue mq;

    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "Is MedianQueue empty before insert? " << (mq.empty() ? "Yes" : "No") << std::endl;

    mq.batchInsert(numbers);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    double logSize = std::log2(size);

    std::cout << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Elapsed time for batch insert: " << elapsed.count() << " seconds" << std::endl;
    std::cout << "Logarithm of size (base 2): " << logSize << std::endl;
    std::cout << "Time per log(n): " << elapsed.count() / logSize << " seconds" << std::endl;

    if (!mq.empty()) {
        std::cout << "Median after batch insert: " << mq.getMedian() << std::endl;
    } else {
        std::cout << "MedianQueue is now empty." << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;

    std::cout << "Batch deleting medians:" << std::endl;

    start = std::chrono::high_resolution_clock::now();

    mq.batchDeleteMedian();

    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;

    std::cout << "Elapsed time for batch delete: " << elapsed.count() << " seconds" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    std::cout << "Is MedianQueue empty after all deletions? " << (mq.empty() ? "Yes" : "No") << std::endl;

    return 0;
}
