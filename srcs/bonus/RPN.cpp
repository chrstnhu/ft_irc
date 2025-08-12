/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:30:38 by chrhu             #+#    #+#             */
/*   Updated: 2025/08/12 12:10:56 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/bonus/RPN.hpp"

RPN::RPN() { }

RPN::~RPN() { }

// Check if the string is a valid RPN
void RPN::checkValidity(const std::string str) {
    int nbrOperand = 0;
    int nbrOperator = 0;
    
    for(size_t i = 0; i < str.size(); ++i) {
        if (isdigit(str[i])) {
            nbrOperand++;
            continue;
        } else if (str[i] == ' ' || str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/') {
            if (str[i] != ' ') {
                nbrOperator++;
            }
            continue;
        } else {
            throw std::invalid_argument("Error: Invalid number or operator");
        }
    }

    if (nbrOperand < (nbrOperator + 1)) {
        throw std::invalid_argument("Not enough operands.");
    }
    else if (nbrOperand > (nbrOperator + 1)) {
        throw std::invalid_argument("Not enough operators.");
    }

}

// Check if the token is an operand
bool isOperand(char token) {
    return (token == '-' || token == '+' || token == '*' || token == '/');
}

// Calculate the result of the operation
double calculateResult(double nbr1, double nbr2, char token) {
    switch (token) {
        case '+':
            return nbr1 + nbr2;
        case '-':
            return nbr1 - nbr2;
        case '*':
            return nbr1 * nbr2;
        case '/':
            return nbr1 / nbr2;
    }
    return 0;
}

// Evaluation of the Reverse Polish Notation
double RPN::evalRPN(std::string str) {
    checkValidity(str);
    
    std::stack<double> stack;
    
    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == ' ') {
            continue;
        } else if (isdigit(str[i])) {
            stack.push(str[i] - '0');
        } else {
            if (stack.size() < 2) {
                throw std::invalid_argument("Not enough operands for operation");
            }

            double nbr2 = stack.top();
            stack.pop();
            
            double nbr1 = stack.top();
            stack.pop();
            
            if (str[i] == '/' && nbr2 == 0) {
                throw std::runtime_error("Can't divide by zero");
            }
            
            double result = calculateResult(nbr1, nbr2, str[i]);
            stack.push(result);
        }
    }
    double finalResult = stack.top();
    stack.pop();
    
    return finalResult;
}