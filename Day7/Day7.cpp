// https://adventofcode.com/2024/day/7
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cassert>
#include <map>
#include <sstream>
//#define SAMPLE_INPUT

enum OperatorType
{
	INVALID,
	ADD,
	MULTIPLY,
	CONCAT
};
class EquationFragment
{
public:
	EquationFragment() = default;
	virtual ~EquationFragment() = default;
	virtual bool IsConstant() = 0;
	virtual bool IsOperator() = 0;

	virtual int GetValue() = 0;
	virtual OperatorType GetOperatorType() = 0;
	virtual void SetValue(int value) = 0;
	virtual void SetOperatorType(OperatorType type) = 0;
	virtual void SwapOperator() {}
};

class EquationConstant : public EquationFragment
{
public:
	// Inherited via EquationFragment
	bool IsConstant() override
	{
		return true;
	}
	bool IsOperator() override
	{
		return false;
	}
	int GetValue() override
	{
		return m_value;
	}
	OperatorType GetOperatorType() override
	{
		return OperatorType::INVALID;
	}
	void SetValue(int value) override
	{
		m_value = value;
	}
	void SetOperatorType(OperatorType type) override
	{
		// Do nothing, this is a constant
		assert(false && "Cannot set operator type on a constant");
	}
private:
	int m_value = 0;
};



class EquationOperator : public EquationFragment
{
public:
	// Inherited via EquationFragment
	bool IsConstant() override
	{
		return false;
	}
	bool IsOperator() override
	{
		return true;
	}
	int GetValue() override
	{
		assert(false && "Cannot get value from an operator");
		return 0; // Should never be called
	}
	OperatorType GetOperatorType() override
	{
		return m_operator_type;
	}

	void SwapOperator() override
	{
		if (m_operator_type == OperatorType::ADD)
		{
			m_operator_type = OperatorType::MULTIPLY;
		}
		else if (m_operator_type == OperatorType::MULTIPLY)
		{
			m_operator_type = OperatorType::CONCAT;
		}
		else if (m_operator_type == OperatorType::CONCAT)
		{
			m_operator_type = OperatorType::ADD; // Reset to ADD for simplicity, could be any other logic as needed
		}
		else
		{
			assert(false && "Invalid operator type");
		}
	}

	void SetValue(int value) override
	{
		assert(false && "Cannot set value on an operator");
	}
	void SetOperatorType(OperatorType type) override
	{
		m_operator_type = type;
	}

private:
	OperatorType m_operator_type = OperatorType::ADD;
};

class Equation
{
public:
	Equation() = default;

	void AddFragment(class EquationFragment* fragment)
	{
		m_fragments.push_back(fragment);
	}

	void GetOperators(std::vector<EquationFragment*>& operators) const
	{
		for (const auto& fragment : m_fragments)
		{
			if (fragment->IsOperator())
			{
				operators.push_back(fragment);
			}
		}
	}

	void SetDesiredResult(std::uint64_t result)
	{
		m_desired_result = result;
	}

	std::uint64_t GetDesiredResult() const
	{
		return m_desired_result;
	}

	const std::vector<class EquationFragment*>& GetFragments() const
	{
		return m_fragments;
	}

	// compute result based on fragments
	std::uint64_t GetComputedResult()
	{
		assert(m_fragments.size() > 0);
		assert(m_fragments[0]->IsConstant() && "First fragment must be a constant");
		std::uint64_t working_result = m_fragments[0]->GetValue();
		OperatorType cached_operator = OperatorType::INVALID;
		for (int i = 1; i < m_fragments.size(); ++i)
		{
			if (m_fragments[i]->IsOperator())
			{
				cached_operator = m_fragments[i]->GetOperatorType();
			}
			else if (m_fragments[i]->IsConstant())
			{
				if (cached_operator == OperatorType::ADD)
				{
					working_result += m_fragments[i]->GetValue();
				}
				else if (cached_operator == OperatorType::MULTIPLY)
				{
					working_result *= m_fragments[i]->GetValue();
				}
				else if (cached_operator == OperatorType::CONCAT)
				{
					std::string current_result = std::to_string(working_result);
					current_result += std::to_string(m_fragments[i]->GetValue());
					working_result = std::stoull(current_result);
				}
				else
				{
					assert(false && "Invalid operator type");
				}
			}
			else
			{
				assert(false && "Invalid fragment type");
			}
		}
		return working_result;
	}

	void GenerateCombinations(int depth, int length, std::vector<std::vector<int>>& combinations, std::vector<int> current_combination)
	{
		if (depth == 0)
		{
			combinations.push_back(current_combination);
			return;
		}
		for (int i = 0; i < length; ++i)
		{
			current_combination[depth-1] = i;
			GenerateCombinations(depth - 1, length, combinations, current_combination);
		}
	}

	bool CouldDesiredMatchComputedResult()
	{
		if (GetComputedResult() == GetDesiredResult())
		{
			return true;
		}

		std::vector<EquationFragment*> operators;
		GetOperators(operators);
		int num_operators = static_cast<int>(operators.size());

		if (m_combinations_cache.find(num_operators) == m_combinations_cache.end())
		{
			std::vector<std::vector<int>> combinations;
			std::vector<int> combination;
			combination.resize(num_operators);
			GenerateCombinations(num_operators, 3, combinations, combination);
			m_combinations_cache[num_operators] = combinations;
		}

		for (int i = 0; i < operators.size(); ++i)
		{
			// Try all combinations of operators
			for (const auto& combination : m_combinations_cache[num_operators])
			{
				// Swap the operator type based on the combination
				for (int j = 0; j < combination.size(); ++j)
				{
					if (combination[j] == 0)
					{
						operators[j]->SetOperatorType(OperatorType::ADD);
					}
					else if (combination[j] == 1)
					{
						operators[j]->SetOperatorType(OperatorType::MULTIPLY);
					}
					else if (combination[j] == 2)
					{
						operators[j]->SetOperatorType(OperatorType::CONCAT);
					}
				}

				if (GetComputedResult() == GetDesiredResult())
				{
					return true;
				}
			}

			// Reset operator types to default for next iteration
			for (auto& op : operators)
			{
				op->SetOperatorType(OperatorType::ADD);
			}
		}

		return false;
	}

	std::string ToString() const
	{
		std::string result;
		for (const auto& fragment : m_fragments)
		{
			if (fragment->IsConstant())
			{
				result += std::to_string(fragment->GetValue());
			}
			else if (fragment->IsOperator())
			{
				if (fragment->GetOperatorType() == OperatorType::ADD)
				{
					result += " + ";
				}
				else if (fragment->GetOperatorType() == OperatorType::MULTIPLY)
				{
					result += " * ";
				}
				else
				{
					assert(false && "Invalid operator type");
				}
			}
			else
			{
				assert(false && "Invalid fragment type");
			}
		}
		return result;
	}
private:
	std::vector<class EquationFragment*> m_fragments;
	std::uint64_t m_desired_result = 0;
	// Cache for combinations of operators
	// 1 operator will be a list
	std::map<int, std::vector<std::vector<int>>> m_combinations_cache;
};

int main()
{
#ifdef SAMPLE_INPUT
	std::ifstream input_file("sample_input.txt");
#else
	std::ifstream input_file("input.txt");
#endif

	if (!input_file)
	{
		std::cerr << "Cannot open file.";
		return 1;
	}

	std::string line;
	std::map<std::uint64_t, std::vector<int>> data;
	while (std::getline(input_file, line))
	{
		if (line.empty())
			continue;

		auto pos = line.find(':');
		if (pos == std::string::npos)
			continue;

		std::uint64_t key = std::stoull(line.substr(0, pos));
		std::vector<int> values;
		std::istringstream iss(line.substr(pos + 1));
		int num;
		while (iss >> num)
		{
			values.push_back(num);
		}

		data[key] = std::move(values);
	}

	std::vector<Equation> equations;
	for(const auto& [key, values] : data)
	{
		Equation equation;
		for (int i = 0; i < values.size(); ++i)
		{
			EquationConstant* constant = new EquationConstant();
			constant->SetValue(values[i]);
			equation.AddFragment(constant);
			if (i != values.size() - 1)
			{
				EquationOperator* op = new EquationOperator();
				equation.AddFragment(op);
			}
		}
		equation.SetDesiredResult(key);
		equations.push_back(equation);
	}

	//for(Equation& equation : equations)
	//{
	//	std::cout << equation.ToString() << " = " << equation.GetComputedResult() << std::endl;
	//}

	std::vector<Equation> possibly_true_equations;
	for (Equation& equation : equations)
	{
		if (equation.CouldDesiredMatchComputedResult())
		{
			possibly_true_equations.push_back(equation);
		}
	}

	std::uint64_t result = 0;
	for(Equation& equation : possibly_true_equations)
	{
		//std::cout << equation.ToString() << " = " << equation.GetComputedResult() << " (" << equation.GetDesiredResult() << ")" << std::endl;
		result += equation.GetDesiredResult();
	}

	std::cout << "Result: " << result << std::endl;
}
