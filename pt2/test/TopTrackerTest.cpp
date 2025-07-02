
#include "TopTracker.h"

#include <chrono>
#include <gtest/gtest.h>
#include <thread>

using namespace std::chrono_literals;

class TopTrackerTest : public ::testing::Test {
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TopTrackerTest, AddAction_PreservesUntilTTLExpires) {
    TopTracker tracker(10);
    Action a1{.id = "test1"};
    tracker.AddAction(a1, 100ms);

    std::this_thread::sleep_for(50ms);

    auto actions = tracker.GetActions();
    ASSERT_EQ(actions.size(), 1);
    EXPECT_EQ(actions[0].id, "test1");
}

TEST_F(TopTrackerTest, ActionIsRemovedAfterTTL) {
    TopTracker tracker(5);

    tracker.AddAction(Action{"expiring"}, 100ms);
    std::this_thread::sleep_for(150ms);

    auto actions = tracker.GetActions();
    EXPECT_TRUE(actions.empty());

    tracker.AddAction(Action{"second"}, 100ms);
    actions = tracker.GetActions();
    ASSERT_EQ(actions.size(), 1);
    EXPECT_EQ(actions[0].id, "second");
}

TEST_F(TopTrackerTest, OldestItemIsRemovedWhenFull) {
    TopTracker tracker(3);

    tracker.AddAction(Action{"1"}, 1s);
    tracker.AddAction(Action{"2"}, 1s);
    tracker.AddAction(Action{"3"}, 1s);
    tracker.AddAction(Action{"4"}, 1s);

    std::this_thread::sleep_for(10ms);

    auto actions = tracker.GetActions();
    ASSERT_EQ(actions.size(), 3);
    EXPECT_EQ(actions[0].id, "2");
    EXPECT_EQ(actions[1].id, "3");
    EXPECT_EQ(actions[2].id, "4");
}
