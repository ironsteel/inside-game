/*
 *
 */

#ifndef SINGLETON_H
#define SINGLETON_H

template <class T>
class Singleton
{

	
public:
	static T& getInstance()
	{
		static T instance;
		return instance;
	}
	
	
protected:
	Singleton<T>() {};                   
	
	Singleton<T>(Singleton<T> const&);
	
	void operator=(Singleton<T> const&); 
	
};

#endif // SINGLETON_H
