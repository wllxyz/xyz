/*
 * Strategy.h
 *
 *  Created on: 2016年8月31日
 *      Author: wll
 */

#ifndef INCLUDE_STRATEGY_H_
#define INCLUDE_STRATEGY_H_

template <typename T>
class Strategy
{
public:
	T* Get() { return strategy; }
	void Set(T* strategy) { this->strategy = strategy; }
private:
	T* strategy;
};

#endif /* INCLUDE_STRATEGY_H_ */
