/* File Name: map_utility.h
 * Copyright: (c) Kayne Ruse, all rights reserved.
 * Author: Kayne Ruse
 * Date: 
 * Description: A stipped down version of Sketch's map utility.
*/
#ifndef KR_MAPUTILITY_H_
#define KR_MAPUTILITY_H_

class MapUtility {
public:
	/* Public access members */
	MapUtility	();
	~MapUtility	();

	void Load	(const char* szFileName);
	void Save	(const char* szFileName);
	void New	(int x, int y, int w, int h);
	void Clear	();

	/* Accessors and mutators */
	int GetT(int x, int y);
	int GetX();
	int GetY();
	int GetW();
	int GetH();
	int** GetArray();

private:
	/* Private access members */
	void GenArray();
	void ClrArray();

	int m_iTileXCount;
	int m_iTileYCount;
	int m_iTileW;
	int m_iTileH;

	int** m_pArray;
};

#endif
