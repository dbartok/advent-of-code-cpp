#include "Day05-SunnyWithAsteroids.h"

#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

__BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <memory>
#include <stdexcept>
#include <string>
__END_LIBRARIES_DISABLE_WARNINGS

namespace AdventOfCode
{

enum class IntcodeProgramExecutionState
{
    RUNNING,
    TERMINATED,
};

struct IntcodeProgamState
{
    std::vector<int> program;
    std::vector<int> inputs;

    std::vector<int> outputs;

    IntcodeProgramExecutionState executionState;
    size_t instructionPointer;
};

enum class IntcodeParameterMode
{
    IMMEDIATE,
    POSITION,
};

class IntcodeParameter
{
public:
    IntcodeParameter(int value, IntcodeParameterMode parameterMode)
        : m_value{value}
        , m_parameterMode{parameterMode}
    {

    }

    int asRvalue(IntcodeProgamState& executionState) const
    {
        if (m_parameterMode == IntcodeParameterMode::IMMEDIATE)
        {
            return m_value;
        }
        else if (m_parameterMode == IntcodeParameterMode::POSITION)
        {
            return executionState.program.at(m_value);
        }

        throw std::runtime_error("Invalid parameter mode for Rvalue resolution: " + std::to_string(static_cast<int>(m_parameterMode)));
    }

    int& asLvalue(IntcodeProgamState& executionState) const
    {
         if (m_parameterMode == IntcodeParameterMode::POSITION)
        {
            return executionState.program.at(m_value);
        }

         throw std::runtime_error("Invalid parameter mode for Lvalue resolution: " + std::to_string(static_cast<int>(m_parameterMode)));
    }

private:
    int m_value;
    IntcodeParameterMode m_parameterMode;
};

class IntcodeInstruction
{
public:
    using SharedPtr = std::shared_ptr<IntcodeInstruction>;

    virtual void execute(IntcodeProgamState& state) const = 0;
    virtual void moveInstructionPointer(size_t& instructionPointer) const = 0;
};

class SingleParameterIntcodeInstruction : public IntcodeInstruction
{
public:
    SingleParameterIntcodeInstruction(IntcodeParameter param) noexcept
        : m_param{std::move(param)}
    {

    }

    void moveInstructionPointer(size_t& instructionPointer) const override
    {
        instructionPointer += 2;
    }

protected:
    IntcodeParameter m_param;
};

class HaltIntcodeInstruction : public IntcodeInstruction
{
public:
    void execute(IntcodeProgamState& state) const override
    {
        state.executionState = IntcodeProgramExecutionState::TERMINATED;
    }

    void moveInstructionPointer(size_t&) const override
    {

    }
};

class InputIntcodeInstruction : public SingleParameterIntcodeInstruction
{
public:
    using SingleParameterIntcodeInstruction::SingleParameterIntcodeInstruction;

    void execute(IntcodeProgamState& state) const override
    {
        m_param.asLvalue(state) = state.inputs.front();
        state.inputs.erase(state.inputs.begin());
    }
};

class OutputIntcodeInstruction : public SingleParameterIntcodeInstruction
{
public:
    using SingleParameterIntcodeInstruction::SingleParameterIntcodeInstruction;

    void execute(IntcodeProgamState& state) const override
    {
        state.outputs.push_back(m_param.asRvalue(state));
    }
};

class TripleParameterIntcodeInstruction : public IntcodeInstruction
{
public:
    TripleParameterIntcodeInstruction(IntcodeParameter param1, IntcodeParameter param2, IntcodeParameter param3) noexcept
        : m_param1{std::move(param1)}
        , m_param2{std::move(param2)}
        , m_param3{std::move(param3)}
    {

    }

    void moveInstructionPointer(size_t& instructionPointer) const override
    {
        instructionPointer += 4;
    }

protected:
    IntcodeParameter m_param1;
    IntcodeParameter m_param2;
    IntcodeParameter m_param3;
};

class AddIntcodeInstruction : public TripleParameterIntcodeInstruction
{
public:
    using TripleParameterIntcodeInstruction::TripleParameterIntcodeInstruction;

    void execute(IntcodeProgamState& state) const override
    {
        m_param3.asLvalue(state) = m_param1.asRvalue(state) + m_param2.asRvalue(state);
    }
};

class MultiplyIntcodeInstruction : public TripleParameterIntcodeInstruction
{
public:
    using TripleParameterIntcodeInstruction::TripleParameterIntcodeInstruction;

    void execute(IntcodeProgamState& state) const override
    {
        m_param3.asLvalue(state) = m_param1.asRvalue(state) * m_param2.asRvalue(state);
    }
};

unsigned numParametersForOpcode(int opcode)
{
    if (opcode == 1 || opcode == 2)
    {
        return 3u;
    }

    if (opcode == 99)
    {
        return 0u;
    }

    if (opcode == 3 || opcode == 4)
    {
        return 1u;
    }

    throw std::runtime_error("Invalid opcode: " + std::to_string(opcode));
}

IntcodeParameterMode createParameterMode(int opcode, size_t parameterPosition)
{
    const int divisor = pow(10, parameterPosition + 1);

    const int modeNumber = (opcode / divisor) % 10;

    if (modeNumber == 0)
    {
        return IntcodeParameterMode::POSITION;
    }
    else if (modeNumber == 1)
    {
        return IntcodeParameterMode::IMMEDIATE;
    }

    throw std::runtime_error("Invalid parameter mode: " + std::to_string(modeNumber));
}

std::vector<IntcodeParameter> createParameters(int numParameters, int opcodeWithParameterModes, const AdventOfCode::IntcodeProgamState& state)
{
    std::vector<IntcodeParameter> parameters;

    for (size_t i = 1; i <= numParameters; ++i)
    {
        const int value = state.program.at(state.instructionPointer + i);
        const IntcodeParameterMode parameterMode = createParameterMode(opcodeWithParameterModes, i);

        IntcodeParameter parameter{value, parameterMode};

        parameters.push_back(std::move(parameter));
    }

    return parameters;
}

IntcodeInstruction::SharedPtr createNextInstruction(const IntcodeProgamState& state)
{
    const int opcodeWithParameterModes = state.program.at(state.instructionPointer);
    const int opcode = opcodeWithParameterModes % 100;
    const unsigned numParameters = numParametersForOpcode(opcode);

    std::vector<IntcodeParameter> parameters = createParameters(numParameters, opcodeWithParameterModes, state);

    switch (opcode)
    {
        case 1:
            return std::make_shared<AddIntcodeInstruction>(parameters.at(0), parameters.at(1), parameters.at(2));
        case 2:
            return std::make_shared<MultiplyIntcodeInstruction>(parameters.at(0), parameters.at(1), parameters.at(2));
        case 3:
            return std::make_shared<InputIntcodeInstruction>(parameters.at(0));
        case 4:
            return std::make_shared<OutputIntcodeInstruction>(parameters.at(0));
        case 99:
            return std::make_shared<HaltIntcodeInstruction>();
    }

    throw std::runtime_error("Invalid opcode: " + std::to_string(opcode));
}

class IntcodeExecutor
{
public:
    IntcodeExecutor(std::vector<int> program, std::vector<int> inputs)
        : m_state{std::move(program), std::move(inputs), {}, IntcodeProgramExecutionState::RUNNING, 0}
    {

    }

    void execute()
    {
        while (m_state.executionState != IntcodeProgramExecutionState::TERMINATED)
        {
            IntcodeInstruction::SharedPtr nextInstruction = createNextInstruction(m_state);

            nextInstruction->execute(m_state);
            nextInstruction->moveInstructionPointer(m_state.instructionPointer);
        }
    }

    const std::vector<int>& getOutputs() const
    {
        return m_state.outputs;
    }

private:
    IntcodeProgamState m_state;
};

int diagnosticCodeProducedByProgram(const std::vector<int>& intcodeProgram)
{
    IntcodeExecutor ie{intcodeProgram, {1}};

    ie.execute();

    return ie.getOutputs().back();
}

}
