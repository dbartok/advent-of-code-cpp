#include "DanceMove.h"

#include "../../Common/Utils.h"

#include "../../Common/DisableLibraryWarningsMacros.h"

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/lexical_cast.hpp>
END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

DanceMove::SharedPtr DanceMove::sharedPtrFromString(const std::string& danceMoveString)
{
    if (danceMoveString.empty())
    {
        throw std::runtime_error("Move string is empty.");
    }

    const char moveID = danceMoveString.front();
    const std::string argsString = danceMoveString.substr(1);

    if (moveID == 's')
    {
        unsigned offset = boost::lexical_cast<unsigned>(argsString);

        return std::make_shared<SpinMove>(offset);
    }
    else if (moveID == 'x')
    {
        std::vector<unsigned> args;
        Utils::splitStringIntoTypedVector(argsString, args, '/');
        if (args.size() != 2)
        {
            throw std::runtime_error("Two arguments expected for Exchange move.");
        }

        return std::make_shared<ExchangeMove>(args[0], args[1]);
    }
    else if (moveID == 'p')
    {
        std::vector<std::string> args;
        Utils::splitStringIntoTypedVector(argsString, args, '/');
        if (args.size() != 2)
        {
            throw std::runtime_error("Two arguments expected for Partner move.");
        }

        return std::make_shared<PartnerMove>(args[0], args[1]);
    }
    else
    {
        throw std::runtime_error("Unknown move identifier.");
    }
}


SpinMove::SpinMove(unsigned offset) noexcept
    : m_offset{offset}
{

}

void SpinMove::execute(std::vector<std::string>& namesInOrder) const
{
    if (namesInOrder.empty())
    {
        return;
    }

    auto namesInOrderOriginal{namesInOrder};

    for (size_t i = 0; i < namesInOrder.size(); ++i)
    {
        namesInOrder[(i + m_offset) % namesInOrder.size()] = namesInOrderOriginal[i];
    }
}


ExchangeMove::ExchangeMove(unsigned pos1, unsigned pos2) noexcept
    : m_pos1{pos1}
    , m_pos2{pos2}
{

}

void ExchangeMove::execute(std::vector<std::string>& namesInOrder) const
{
    if (namesInOrder.size() < std::max(m_pos1, m_pos2))
    {
        throw std::runtime_error("Too few elements.");
    }

    std::swap(namesInOrder[m_pos1], namesInOrder[m_pos2]);
}


PartnerMove::PartnerMove(std::string name1, std::string name2) noexcept
    : m_name1{std::move(name1)}
    , m_name2{std::move(name2)}
{

}

void PartnerMove::execute(std::vector<std::string>& namesInOrder) const
{
    const auto name1Iter = std::find(namesInOrder.begin(), namesInOrder.end(), m_name1);
    const auto name2Iter = std::find(namesInOrder.begin(), namesInOrder.end(), m_name2);

    if (name1Iter == namesInOrder.end() || name2Iter == namesInOrder.end())
    {
        throw std::runtime_error("Both named elements need to be present.");
    }

    std::swap(*name1Iter, *name2Iter);
}

}
