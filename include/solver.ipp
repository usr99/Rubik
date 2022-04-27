/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Solver.ipp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mamartin <mamartin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/27 16:18:35 by mamartin          #+#    #+#             */
/*   Updated: 2022/04/27 18:10:28 by mamartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOLVER_IPP
# define SOLVER_IPP

# include "CoordCube.hpp"

template <typename T>
std::list<std::string>
CoordCube::_solve()
{
	T root(*this);

	std::list<T>	path;
	int				threshold	= _estimateCost(root);
	bool			found		= false;

	path.push_back(root);
	while (!found)
	{
		// search any solution of length threshold
		const int minimalCost = _search(path, 0, threshold);
		threshold = minimalCost; // new threshold is the smallest branch cost encountered
		found = !minimalCost; // but 0 is returned when the solution has been found
	}

	// convert the path into a readable solution
	path.pop_front(); // remove root node
	std::list<std::string> solution;
	for (
		typename std::list<T>::const_iterator it = path.begin();
		it != path.end();
		it++
	) {
		int			moveIndex	= it->last / 3;
		int			factor		= it->last - moveIndex * 3 + 1;
		std::string	move(1, Rubik::Faces[moveIndex]);

		if (factor == 2)
			move += '2';
		else if (factor == 3)
			move += '\'';
		solution.push_back(move);
	}
	return solution;
}

template <typename T>
int
CoordCube::_search(std::list<T>& path, int cost, int threshold)
{
	const T&	node		= path.back();
	const int	branchCost	= cost + _estimateCost(node);
	int			minimalCost	= -1;

	if (branchCost > threshold)
		return branchCost; // this branch is a dead-end
	if (node.isGoal())
		return 0; // solution found

	std::list<T> nextNodes = _applyAllMoves(node);

	for (
		typename std::list<T>::const_iterator it = nextNodes.begin();
		it != nextNodes.end();
		it++
	) {
		if (std::find(path.begin(), path.end(), *it) == path.end()) // avoid going back to a previously explored state
		{
			path.push_back(*it);

			const int c = _search(path, cost + 1, threshold);
			if (c == 0)
				return c; // solution found
			if (c < minimalCost || minimalCost == -1)
				minimalCost = c;
			path.pop_back();
		}
	}
	return minimalCost;
}

#endif
