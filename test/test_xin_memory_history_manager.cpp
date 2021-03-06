/***************************************************************************
* Copyright (c) 2016, Johan Mabille, Sylvain Corlay and Wolf Vollprecht    *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include "gtest/gtest.h"

#include <vector>
#include <array>
#include <string>

#include "nlohmann/json.hpp"

#include "xeus/xhistory_manager.hpp"

namespace nl = nlohmann;

namespace xeus
{
    using history_manager_ptr = std::unique_ptr<xhistory_manager>;
    using history_type = std::vector<std::array<std::string, 3>>;

    TEST(xin_memory_history_manager, get_tail)
    {
        history_manager_ptr hist = xeus::make_in_memory_history_manager();
        hist->store_inputs(1, "print(3)");
        hist->store_inputs(2, "a = 3");
        hist->store_inputs(3, "print(a)");
        hist->store_inputs(4, "a");

        nl::json tail1 = hist->get_tail(1000, true, false);
        ASSERT_EQ(tail1["status"], "ok");
        auto history1 = tail1["history"].get<history_type>();
        ASSERT_EQ(history1.size(), std::size_t(4));
        ASSERT_EQ(history1[0][1], "1");
        ASSERT_EQ(history1[0][2], "print(3)");
        ASSERT_EQ(history1[1][1], "2");
        ASSERT_EQ(history1[1][2], "a = 3");
        ASSERT_EQ(history1[2][1], "3");
        ASSERT_EQ(history1[2][2], "print(a)");
        ASSERT_EQ(history1[3][1], "4");
        ASSERT_EQ(history1[3][2], "a");

        nl::json tail2 = hist->get_tail(2, true, false);
        ASSERT_EQ(tail2["status"], "ok");
        auto history2 = tail2["history"].get<history_type>();
        ASSERT_EQ(history2.size(), std::size_t(2));
        ASSERT_EQ(history2[0][1], "1");
        ASSERT_EQ(history2[0][2], "print(3)");
        ASSERT_EQ(history2[1][1], "2");
        ASSERT_EQ(history2[1][2], "a = 3");
    }

    TEST(xin_memory_history_manager, get_range)
    {
        history_manager_ptr hist = xeus::make_in_memory_history_manager();
        hist->store_inputs(1, "print(3)");
        hist->store_inputs(2, "a = 3");
        hist->store_inputs(3, "print(a)");
        hist->store_inputs(4, "a");

        nl::json range1 = hist->get_range(0, 1, 2, true, false);
        ASSERT_EQ(range1["status"], "ok");
        auto history1 = range1["history"].get<history_type>();
        ASSERT_EQ(history1.size(), std::size_t(1));
        ASSERT_EQ(history1[0][1], "2");
        ASSERT_EQ(history1[0][2], "a = 3");

        nl::json range2 = hist->get_range(0, 1, 3, true, false);
        ASSERT_EQ(range2["status"], "ok");
        auto history2 = range2["history"].get<history_type>();
        ASSERT_EQ(history2.size(), std::size_t(2));
        ASSERT_EQ(history2[0][1], "2");
        ASSERT_EQ(history2[0][2], "a = 3");
        ASSERT_EQ(history2[1][1], "3");
        ASSERT_EQ(history2[1][2], "print(a)");

        nl::json range3 = hist->get_range(0, 1000, 2, true, false);
        ASSERT_EQ(range3["status"], "error");
    }
}
