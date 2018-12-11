/*
 * Tencent is pleased to support the open source community by making
 * WCDB available.
 *
 * Copyright (C) 2017 THL A29 Limited, a Tencent company.
 * All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use
 * this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 *       https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _WCDB_MIGRATIONQUEUE_HPP
#define _WCDB_MIGRATIONQUEUE_HPP

#include <WCDB/AsyncQueue.hpp>
#include <WCDB/Lock.hpp>
#include <set>

namespace WCDB {

class MigrationEvent : public AsyncQueue::Event {
public:
    virtual ~MigrationEvent();

protected:
    virtual std::pair<bool, bool> databaseShouldMigrate(const String& path) = 0;
    friend class MigrationQueue;
};

class MigrationQueue final : public AsyncQueue {
public:
    MigrationQueue(const String& name, MigrationEvent* event);

    void put(const String& path);

    static constexpr const double timeIntervalForMigration = 1.0f;
    static constexpr const int toleranceFailedCount = 3;

protected:
    void loop() override final;

    std::mutex m_mutex;
    std::condition_variable m_cond;
    std::set<String> m_migratings;
    std::map<String, int> m_faileds;
};

} // namespace WCDB

#endif /* _WCDB_MIGRATIONQUEUE_HPP */
