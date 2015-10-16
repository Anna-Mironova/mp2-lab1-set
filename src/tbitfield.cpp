// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len<0) 
		throw "Negative Length";
	else
	{
		BitLen=len;
		MemLen=(len+16-1)/16;
		pMem=new TELEM[MemLen];
		for (int i=0;i<MemLen;i++)
			pMem[i]=0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen=bf.BitLen;
	MemLen=bf.MemLen;
	pMem=new TELEM[MemLen];
	memcpy(pMem,bf.pMem,sizeof(TELEM)*MemLen);
}

TBitField::~TBitField()
{
	if (pMem!=NULL)
		delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n/16;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1<<(n%16);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0)
		throw "Negative Index";
	else if (n > BitLen)
		throw "Index more than length of bit field";
	else 
		pMem[GetMemIndex(n)]|=GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0)
		throw "Negative Index";
	else if ( n > BitLen)
		throw "Index more than length of bit field";
	else
		pMem[GetMemIndex(n)]&=~ GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n <0)
		throw "Negative Index";
	else if (n >BitLen)
		throw "Index more than length of bit field";
	else 
		return pMem[GetMemIndex(n)]& GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	BitLen=bf.BitLen;
	if(MemLen!=bf.MemLen)
	{
		MemLen=bf.MemLen;
		if(pMem!=NULL)
			delete pMem;
		pMem=new TELEM[MemLen];
	}
	if (pMem!=NULL)
		memcpy(pMem,bf.pMem,sizeof(TELEM)*MemLen);
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int res=1;
	if (BitLen!=bf.BitLen)
		res=0;
	else 
	{
		for (int i=0; i<MemLen;i++)
			if (pMem[i]!=bf.pMem[i])
			{
				res=0;
				break;
			}
	}
	return res;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  int res=1;
	if (BitLen!=bf.BitLen)
		return res;
	else 
	{
		for (int i=0; i<MemLen;i++)
			if (pMem[i]!=bf.pMem[i])
				break;
			else
				res=0;
	}
	return res;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int Len=BitLen;
	if (bf.BitLen>BitLen)
		Len=bf.BitLen;
	TBitField res(Len);
	for (int i=0;i<MemLen;i++)
		res.pMem[i]=pMem[i];
	for (int i=0;i<bf.MemLen;i++)
		res.pMem[i]|=bf.pMem[i];
	return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int Len=BitLen;
	if (bf.BitLen>BitLen)
		Len=bf.BitLen;
	TBitField res(Len);
	for (int i=0;i<MemLen;i++)
		res.pMem[i]=pMem[i];
	for (int i=0;i<bf.MemLen;i++)
		res.pMem[i]&=bf.pMem[i];
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i=0;i<BitLen;i++)
	{
		if (this->GetBit(i)==0)
			res.SetBit(i);
	}
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	int i=0;
	char ch;
	do 
	{
		istr>>ch;
	}
	while( ch!=' ');
	while(1)
	{
		istr>>ch;
		if(ch=='0')
			bf.ClrBit(i++);
		else if (ch =='1')
			bf.SetBit(i++);
		else break;
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i=0;i<bf.BitLen;i++)
		ostr << bf.GetBit(i);
	return ostr;
}
