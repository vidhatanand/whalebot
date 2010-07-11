#pragma once


namespace korm {
	
//map old plain data types
template<class T>
class CMapper {
public:
	CMapper( const T& opd )
	: m_pPtr((const char*)(&opd))
	{}
	
	const char* dataPointer() const
	{
		return m_pPtr;
	}
	
	size_t dataSize() const
	{
		return sizeof(T);
	}
	
private:
	const char*	m_pPtr;
};

//unmap old plain data types
template<class T>
class CUnMapper {
public:
	CUnMapper( T& target )
	: m_tData(target)
	{}
	
	void unMap( const char *dataPtr, size_t dataSize )
	{
		memcpy(&m_tData, dataPtr, dataSize);
	}
	
	
private:
	T&				m_tData;
};

template<class T>
class CTraits {
public:
	typedef CMapper<T>		CDefaultMapper;
	typedef CUnMapper<T>	CDefaultUnMapper;
};

}//korm - kyoto orm
