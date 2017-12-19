#pragma once

#include "RegisterValueType.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/optional.hpp>

#include <vector>
#include <deque>
#include <memory>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

class DuetMessageQueue
{
public:
    using SharedPtr = std::shared_ptr<DuetMessageQueue>;

    DuetMessageQueue() noexcept;
    void sendMessage(unsigned queueID, RegisterValueType value);
    boost::optional<RegisterValueType> tryReceiveMessage(unsigned queueID);

private:
    std::vector<std::deque<RegisterValueType>> m_programIDToQueue;

    void validateQueueID(unsigned queueID);
};

}
