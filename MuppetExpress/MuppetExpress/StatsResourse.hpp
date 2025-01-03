#pragma once

#include <memory_resource>
#include <cstdint>
#include <iostream>

class StatsResource : public std::pmr::memory_resource
{
public:
    // Constructor allows specifying a parent resource, defaults to the global default PMR
    explicit StatsResource(std::pmr::memory_resource* parent = std::pmr::get_default_resource())
        : parent_(parent)
    {
    }

    // Simple function to dump statistics
    void printStats() const
    {
        std::cout << "Bytes currently allocated: " << bytesAlloc_ << '\n';
        std::cout << "High-watermark allocation: " << highWatermarkBytesAlloc_ << '\n';
    }

private:
    // The parent resource that actually does the allocations
    std::pmr::memory_resource* parent_;

    // Track total currently allocated bytes
    int32_t bytesAlloc_{};
    // Track the highest total bytes allocated so far
    int32_t highWatermarkBytesAlloc_{};

protected:
    // Allocate memory, add to our running total, track high-watermark
    void* do_allocate(std::size_t bytes, std::size_t alignment) override
    {
        bytesAlloc_ += static_cast<int32_t>(bytes);

        if (bytesAlloc_ > highWatermarkBytesAlloc_)
        {
            highWatermarkBytesAlloc_ = bytesAlloc_;
        }
        printStats();
        return parent_->allocate(bytes, alignment);
    }

    // Deallocate memory, reduce our running total
    void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override
    {
        bytesAlloc_ -= static_cast<int32_t>(bytes);
        parent_->deallocate(p, bytes, alignment);
        printStats();
    }

    // Used by polymorphic memory resources to check equality
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override
    {
        // We treat this resource as "equal" to another only if the parent is the same
        return parent_->is_equal(other);
    }
};

//// Example usage
//int main()
//{
//    StatsResource stats; // Uses default_resource() internally
//
//    // Create a pmr::vector that uses our stats resource
//    std::pmr::vector<int> vec(&stats);
//
//    // Perform some allocations
//    for (int i = 0; i < 100; ++i) {
//        vec.push_back(i);
//    }
//
//    // Print stats
//    stats.printStats();
//
//    return 0;
//}
