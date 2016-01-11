#pragma once

#include <string>

class ImageContext
{
public:
	virtual ~ImageContext(){}
	virtual const std::string&	getImageFilePath()const = 0;
};

class CompareResult
{
public:
	virtual ~CompareResult() {}
	virtual const std::string&	getFirstImgFilePath()const = 0;
	virtual const std::string&	getSecondImgFilePath()const = 0;
};

class  ImageOperation
{
public:
	enum compareFlag{ cmp_default = 0x0000, cmp_markDifference = 0x0001 };

	static CompareResult*	compare(const ImageContext* imgSrc, const ImageContext* imgNew, const compareFlag& cmpFlag = cmp_default);

	static ImageContext*	openImage(const std::wstring& strFile);
	static void				closeImage(const ImageContext* img);

	static void				markDifference(const ImageContext* img, const CompareResult* cmpResult);

	static void				showImage(const ImageContext* img, HDC hDC, int x = 0, int y = 0, unsigned long uWidth = -1, unsigned long uHeight = -1);

	static void				writeResultToXml(const CompareResult* cmpResult, const std::string& strResultFile);
	
	static CompareResult*	loadResultFormXml(const std::string& strResultFile);
};
