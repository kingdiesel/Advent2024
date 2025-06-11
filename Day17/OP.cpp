#include "OP.h"
#include "CPU.h"
#include <cassert>
#include <cmath>
#include <iostream>
/*
	Combo operands 0 through 3 represent literal values 0 through 3.
	Combo operand 4 represents the value of register A.
	Combo operand 5 represents the value of register B.
	Combo operand 6 represents the value of register C.
	Combo operand 7 is reserved and will not appear in valid programs.
*/
int OpCode::GetOperand(class CPU& cpu) const
{
	int operand = cpu.GetOperand();
	if (IsComboOperand())
	{
		switch (operand)
		{
		case 1:
		case 2:
		case 3:
			break;
		case 4:
			operand = cpu.GetRegisterA();
			break;
		case 5:
			operand = cpu.GetRegisterB();
			break;
		case 6:
			operand = cpu.GetRegisterC();
			break;
		default:
			assert(false && "Invalid combo operand");
			break;
		}

	}
	return operand;
}

void OpCode_ADV::Execute(CPU& cpu)
{
	int numerator = cpu.GetRegisterA();
	int denominator = static_cast<int>(std::pow(2, GetOperand(cpu)));
	int truncated_result = numerator / denominator;
	cpu.SetRegisterA(truncated_result);
}

bool OpCode_ADV::IncrementsProgramCounter() const
{
	return true;
}

bool OpCode_ADV::IsComboOperand() const
{
	return true;
}

void OpCode_BXL::Execute(CPU& cpu)
{
	int xor_left = cpu.GetRegisterB();
	int xor_right = GetOperand(cpu);
	int xor_result = xor_left ^ xor_right;
	cpu.SetRegisterB(xor_result);
}

bool OpCode_BXL::IncrementsProgramCounter() const
{
	return true;
}

bool OpCode_BXL::IsComboOperand() const
{
	return false;
}

void OpCode_BST::Execute(CPU& cpu)
{
	int operand = GetOperand(cpu);
	int modulo_8 = operand % 8;
	cpu.SetRegisterB(modulo_8);
}

bool OpCode_BST::IncrementsProgramCounter() const
{
	return true;
}

bool OpCode_BST::IsComboOperand() const
{
	return true;
}

void OpCode_BXC::Execute(CPU& cpu)
{
	int xor_left = cpu.GetRegisterB();
	int xor_right = cpu.GetRegisterC();
	int xor_result = xor_left ^ xor_right;
	cpu.SetRegisterB(xor_result);
}

bool OpCode_BXC::IncrementsProgramCounter() const
{
	return true;
}

bool OpCode_BXC::IsComboOperand() const
{
	return false;
}

void OpCode_BDV::Execute(CPU& cpu)
{
	int numerator = cpu.GetRegisterA();
	int denominator = static_cast<int>(std::pow(2, GetOperand(cpu)));
	int truncated_result = numerator / denominator;
	cpu.SetRegisterB(truncated_result);
}

bool OpCode_BDV::IncrementsProgramCounter() const
{
	return true;
}

bool OpCode_BDV::IsComboOperand() const
{
	return true;
}

void OpCode_CDV::Execute(CPU& cpu)
{
	int numerator = cpu.GetRegisterA();
	int denominator = static_cast<int>(std::pow(2, GetOperand(cpu)));
	int truncated_result = numerator / denominator;
	cpu.SetRegisterC(truncated_result);
}

bool OpCode_CDV::IncrementsProgramCounter() const
{
	return true;
}

bool OpCode_CDV::IsComboOperand() const
{
	return true;
}

void OpCode_OUT::Execute(CPU& cpu)
{
	int operand = GetOperand(cpu);
	int modulo_8 = operand % 8;
	cpu.PackValue(modulo_8);
	//std::cout << modulo_8;
	m_output_counter++;
}

bool OpCode_OUT::IncrementsProgramCounter() const
{
	return true;
}

bool OpCode_OUT::IsComboOperand() const
{
	return true;
}

void OpCode_JNZ::Execute(CPU& cpu)
{
	m_performed_jump = false;
	if (cpu.GetRegisterA() != 0)
	{
		int jump_location = GetOperand(cpu);
		cpu.SetProgramCounter(jump_location);
		m_performed_jump = true;
	}
}

bool OpCode_JNZ::IncrementsProgramCounter() const
{
	return !m_performed_jump;
}

bool OpCode_JNZ::IsComboOperand() const
{
	return false;
}
