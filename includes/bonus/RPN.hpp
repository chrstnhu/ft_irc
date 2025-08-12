/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrhu <chrhu@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 12:29:51 by chrhu             #+#    #+#             */
/*   Updated: 2025/06/04 16:24:18 by chrhu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef RPN_HPP
# define RPN_HPP

# include <iostream>
# include <string>
# include <fstream>
# include <ctime>
# include <sstream>
# include <cmath>
# include <algorithm>
# include <stack>
# include <climits> 

# include "../Colors.hpp"

class RPN {
    public :
        RPN();
        RPN(std::stack<double> stack);
        ~RPN();
        
        // Function
        static void checkValidity(const std::string str);
        static double evalRPN(std::string str);
        
};

# endif