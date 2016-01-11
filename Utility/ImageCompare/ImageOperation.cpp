#include "stdafx.h"
#include "ImageOperation.h"

#include <io.h>
#include <vector>
#include <algorithm>
//GTEST=gtest-1.3.0;TINYXML=tinyxml2-master;

#include "tinyxml2-master/tinyxml2.h"

#pragma comment( lib, "../../ThirdLibrary/tinyxml2-master/tinyxml2/bin/Win32-Debug-Lib/tinyxml2.lib" )

#include "../ScopeGuard/ScopeGuard.h"
using namespace scopeguard;

class  CCompareResult : public CompareResult
{
public:
	CCompareResult()
	{
		memset(&m_rcBoundingBox, 0, sizeof(m_rcBoundingBox));
	}
	bool		isValidResult()
	{
		if (m_vecDifferentPoint.size() == 0)
			return false;
		if (m_rcBoundingBox.left >= m_rcBoundingBox.right || m_rcBoundingBox.top >= m_rcBoundingBox.bottom)
			return false;
		return true;
	}
	RECT		m_rcBoundingBox;
	std::vector<POINT>	m_vecDifferentPoint;
	std::string		m_strFirstImgFile;
	std::string		m_strSecondImgFile;

	virtual const std::string&	getFirstImgFilePath()const
	{
		return m_strFirstImgFile;
	}
	virtual const std::string&	getSecondImgFilePath()const
	{
		return m_strSecondImgFile;
	}

};

class CImageContext : public ImageContext
{
public:
	Gdiplus::Image*		m_pImg;
	std::string			m_strImgFile;//文件的路径
	CImageContext()
		: m_pImg(nullptr)
	{}
	~CImageContext()
	{
		if (m_pImg)
			delete m_pImg;
		m_pImg = nullptr;
	}
	virtual const std::string&	getImageFilePath()const
	{
		return m_strImgFile;
	}

	Gdiplus::Image*		openImage(const std::wstring& strFile)
	{
		m_pImg = Gdiplus::Bitmap::FromFile(strFile.c_str());
		if (m_pImg)
			m_strImgFile = wcs2mbs(strFile);
		return m_pImg;
	}
};

tinyxml2::XMLElement* makeResult(tinyxml2::XMLDocument* pXMLDoc, const CCompareResult* pResult)
{
	namespace xml = tinyxml2;
	xml::XMLElement* pRoot = pXMLDoc->RootElement();
	xml::XMLElement* pNewEle = pXMLDoc->NewElement("result");

	xml::XMLElement* pBoxEle = pXMLDoc->NewElement("boundingbox");
	pBoxEle->SetAttribute("left", pResult->m_rcBoundingBox.left);
	pBoxEle->SetAttribute("top", pResult->m_rcBoundingBox.top);
	pBoxEle->SetAttribute("right", pResult->m_rcBoundingBox.right);
	pBoxEle->SetAttribute("bottom", pResult->m_rcBoundingBox.bottom);

	pNewEle->LinkEndChild(pBoxEle);

	xml::XMLElement* pPointsEle = pXMLDoc->NewElement("points");
	auto it = pResult->m_vecDifferentPoint.begin();
	while (it != pResult->m_vecDifferentPoint.end())
	{
		xml::XMLElement* pPointEle = pXMLDoc->NewElement("point");
		pPointEle->SetAttribute("x", it->x);
		pPointEle->SetAttribute("y", it->y);

		pPointsEle->LinkEndChild(pPointEle);
		it++;
	}
	pNewEle->LinkEndChild(pPointsEle);

	xml::XMLElement* pFirstImageEle = pXMLDoc->NewElement("FirstImage");
	pFirstImageEle->SetAttribute("File", pResult->m_strFirstImgFile.c_str());
	pNewEle->LinkEndChild(pFirstImageEle);

	xml::XMLElement* pSecondImageEle = pXMLDoc->NewElement("SecondImage");
	pSecondImageEle->SetAttribute("File", pResult->m_strSecondImgFile.c_str());
	pNewEle->LinkEndChild(pSecondImageEle);

	return pNewEle;
}

tinyxml2::XMLDocument* result2XML(CCompareResult const* pResult)
{
	namespace xml = tinyxml2;
// 	if (!pResult->isValidResult())
// 		return;
	xml::XMLDocument* pXmlDoc = new xml::XMLDocument(true);

	xml::XMLElement* pRoot = pXmlDoc->RootElement();
	if (!pRoot)
	{
		pRoot = pXmlDoc->NewElement("Results");
		pXmlDoc->LinkEndChild(pRoot);
	}
	xml::XMLElement* pNewEle = makeResult(pXmlDoc, pResult);
	if (pNewEle)
	{
		pRoot->LinkEndChild(pNewEle);
	}

	return pXmlDoc;
}

CCompareResult* xml2Result(tinyxml2::XMLDocument* pXMLDoc)
{
	namespace xml = tinyxml2;
	CCompareResult* pResult = new CCompareResult();
	CScopeGuard resultGuard([&pResult](){delete pResult; });
	xml::XMLElement* pRoot = pXMLDoc->RootElement();

	auto pxmlResult = pRoot->FirstChildElement("result");
	if (!pxmlResult)
		return nullptr;

	//boundingbox
	{
		auto pxmlBBox = pxmlResult->FirstChildElement("boundingbox");
		if (!pxmlBBox)
			return nullptr;

		pResult->m_rcBoundingBox.left = pxmlBBox->IntAttribute("left");
		pResult->m_rcBoundingBox.right = pxmlBBox->IntAttribute("right");
		pResult->m_rcBoundingBox.top = pxmlBBox->IntAttribute("top");
		pResult->m_rcBoundingBox.bottom = pxmlBBox->IntAttribute("bottom");
	}

	//image file
	{
		auto pxmlFirstImg = pxmlResult->FirstChildElement("FirstImage");
		auto pxmlSecondImg = pxmlResult->FirstChildElement("SecondImage");
		if (!pxmlFirstImg || !pxmlSecondImg)
			return nullptr;
		pResult->m_strFirstImgFile = pxmlFirstImg->Attribute("File");
		pResult->m_strSecondImgFile = pxmlSecondImg->Attribute("File");
	}

	//points, may be not exist
	{
		auto pxmlPoints = pxmlResult->FirstChildElement("points");
		if (pxmlPoints)
		{
			xml::XMLElement* pxmlPoint = pxmlPoints->FirstChildElement("point");
			while (pxmlPoint)
			{
				POINT pt;
				pt.x = pxmlPoint->IntAttribute("x");
				pt.y = pxmlPoint->IntAttribute("y");
				pResult->m_vecDifferentPoint.push_back(pt);
				pxmlPoint = pxmlPoint->NextSiblingElement("point");
			}
		}
	}

	resultGuard.Dismiss();
	return pResult;
}

CompareResult* ImageOperation::compare(const ImageContext* img1, const ImageContext* img2, const compareFlag& cmpFlag/* = cmp_default*/)
{
	assert(img1);
	assert(img2);
	CCompareResult* pResult = new CCompareResult;
	if (img1 && img2)
	{
		CImageContext* pImgContext1 = (CImageContext*)img1;
		CImageContext* pImgContext2 = (CImageContext*)img2;

		pResult->m_strFirstImgFile = pImgContext1->m_strImgFile;
		pResult->m_strSecondImgFile = pImgContext2->m_strImgFile;

		Gdiplus::Bitmap* pImg1 = (Gdiplus::Bitmap*)pImgContext1->m_pImg;
		Gdiplus::Bitmap* pImg2 = (Gdiplus::Bitmap*)pImgContext2->m_pImg;
		Gdiplus::BitmapData bmpData1;
		pImg1->LockBits(nullptr, Gdiplus::ImageLockModeRead, pImg1->GetPixelFormat(), &bmpData1);
		Gdiplus::BitmapData bmpData2;
		pImg2->LockBits(nullptr, Gdiplus::ImageLockModeRead, pImg2->GetPixelFormat(), &bmpData2);

		LPBYTE pBuffer1 = (LPBYTE)bmpData1.Scan0;
		LPBYTE pBuffer2 = (LPBYTE)bmpData2.Scan0;
		for (unsigned int y = 0, h = bmpData1.Height; y < h; ++y)
		{
			LPBYTE pLine1 = pBuffer1 + bmpData1.Stride * y;
			LPBYTE pLine2 = pBuffer2 + bmpData2.Stride * y;
			if (memcmp(pLine1, pLine2, bmpData1.Stride) != 0)
			{
				//说明这一行数据有不同的像素点，具体是哪个点再循环查找
				LPBYTE pPixel_1 = pLine1;
				LPBYTE pPixel_2 = pLine2;
				for (unsigned int x = 0; x < bmpData1.Width; ++x)
				{
					//由于是自己使用，会保证保存的图片都是24bpp的，所以此处不做其它可能性的处理了
					if (memcmp(pPixel_1, pPixel_2, 3) != 0)
					{
						//说明这个像素点不同，需要记录下来
						POINT pt;
						pt.x = x;
						pt.y = y;
						pResult->m_vecDifferentPoint.push_back(pt);
					}
					pPixel_1 += 3;
					pPixel_2 += 3;
				}
			}
		}
		pImg1->UnlockBits(&bmpData1);
		pImg2->UnlockBits(&bmpData2);

		auto it = pResult->m_vecDifferentPoint.begin();
		if (it != pResult->m_vecDifferentPoint.end())
		{
			//先获得第一个点，
			pResult->m_rcBoundingBox.left = it->x;
			pResult->m_rcBoundingBox.top = it->y;
			pResult->m_rcBoundingBox.right = it->x;
			pResult->m_rcBoundingBox.bottom = it->y;

			std::for_each(++it, pResult->m_vecDifferentPoint.end(), [pResult](const POINT& pt)
			{
				pResult->m_rcBoundingBox.left = min(pResult->m_rcBoundingBox.left, pt.x);
				pResult->m_rcBoundingBox.top = min(pResult->m_rcBoundingBox.top, pt.y);
				pResult->m_rcBoundingBox.right = max(pResult->m_rcBoundingBox.right, pt.x);
				pResult->m_rcBoundingBox.bottom = max(pResult->m_rcBoundingBox.bottom, pt.y);
			}
			);

			if (cmpFlag&cmp_markDifference)
			{
				markDifference(img2, pResult);
			}
		}
	}
	return pResult;
}

ImageContext* ImageOperation::openImage(const std::wstring& strFile)
{
	CImageContext* pImgContext = new CImageContext;
	pImgContext->openImage(strFile);

	return pImgContext;
}
void ImageOperation::closeImage(const ImageContext* img)
{
	if (img)
	{
		CImageContext* pImgContext = (CImageContext*)img;
		delete pImgContext;
	}
}

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	using namespace Gdiplus;
	UINT num = 0;
	UINT size = 0;

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
	{
		return -1;
	}
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
	{
		return -1;
	}

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j< num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}

	free(pImageCodecInfo);
	return -1;
}

void ImageOperation::markDifference(const ImageContext* img, const CompareResult* cmpResult)
{
	using namespace Gdiplus;
	if (img && cmpResult)
	{
		CImageContext* pImgContext = (CImageContext*)img;
		CCompareResult* pResult = (CCompareResult*)cmpResult;
		auto& vecDiffPoint = pResult->m_vecDifferentPoint;
		auto& itPoint = vecDiffPoint.begin();
		if (itPoint == vecDiffPoint.end())
			return;

		struct	DifferenceData
		{
			DifferenceData(const int& x, const int& y)
				: rc(x, y, 1, 1)
			{}
			Gdiplus::Rect  rc;
			bool checkPoint(const int& x, const int& y)const
			{
				return (((rc.GetRight() + 5) >= x) && ((rc.GetBottom() + 5) >= y))
					|| (x < rc.X && y < rc.Y && (rc.X - 5 <= x) && rc.Y - 5 <= y);
			}
			void addPoint(const int& x, const int& y)
			{
				if (rc.GetRight() < x)
					rc.Width += (x - rc.GetRight());
				else if (rc.X > x)
				{
					rc.Width += (rc.X - x);
					rc.X = x;
				}
				if (rc.GetBottom() < y)
					rc.Height += (y - rc.GetBottom());

				auto x1 = rc.X;
				auto y1 = rc.Y;
				auto w = rc.Width;
				auto h = rc.Height;
			}
		};


		Gdiplus::Bitmap* pImg = (Gdiplus::Bitmap*)pImgContext->m_pImg;
		Gdiplus::Graphics* pGraphics = Gdiplus::Graphics::FromImage(pImg);
		Pen newPen((Color(255, 0, 0)));


		std::vector<DifferenceData*> vecDiff;

		vecDiff.push_back(new DifferenceData(itPoint->x, itPoint->y));
		itPoint++;

		while (itPoint != vecDiffPoint.end())
		{
			bool bDealed = false;
			auto& itDiffData = vecDiff.begin();
			while (itDiffData != vecDiff.end())
			{
				if ((*itDiffData)->checkPoint(itPoint->x, itPoint->y))
				{
					(*itDiffData)->addPoint(itPoint->x, itPoint->y);

					bDealed = true;
					break;
				}
				itDiffData++;
			}
			if (!bDealed)
			{
				vecDiff.push_back(new DifferenceData(itPoint->x, itPoint->y));
			}
			itPoint++;
		}

		auto& itDiffData = vecDiff.begin();
		while (itDiffData != vecDiff.end())
		{
			pGraphics->DrawRectangle(&newPen, (*itDiffData)->rc);
			delete *itDiffData;
			itDiffData++;
		}
// 		Gdiplus::Rect rc(pResult->m_rcBoundingBox.left, pResult->m_rcBoundingBox.top
// 			, pResult->m_rcBoundingBox.right - pResult->m_rcBoundingBox.left
// 			, pResult->m_rcBoundingBox.bottom - pResult->m_rcBoundingBox.top);
// 		pGraphics->DrawRectangle(&newPen, rc);

// 		CLSID bmpClsid;
// 		GetEncoderClsid(L"image/bmp", &bmpClsid);
// 		pImg->Save(L"C:\\Result.bmp", &bmpClsid);
	}
}

void ImageOperation::showImage(const ImageContext* img, HDC hDC, int x/* = 0*/, int y/* = 0*/, unsigned long uWidth/* = -1*/, unsigned long uHeight/* = -1*/)
{
	using namespace Gdiplus;
	if (img)
	{
		CImageContext* pImgContext = (CImageContext*)img;
		Gdiplus::Bitmap* pImg = (Gdiplus::Bitmap*)pImgContext->m_pImg;
		Gdiplus::Graphics* pGraphics = Gdiplus::Graphics::FromHDC(hDC);

		if (uWidth > 0 && uWidth < -1 && uHeight > 0 && uHeight < -1)
			pGraphics->DrawImage(pImg, Gdiplus::Rect(x, y, uWidth, uHeight));
		else
			pGraphics->DrawImage(pImg, x, y);
	}
}

void ImageOperation::writeResultToXml(const CompareResult* cmpResult, const std::string& strResultFile)
{
	namespace xml = tinyxml2;
	CCompareResult* pResult = (CCompareResult*)cmpResult;
	xml::XMLDocument* pXmlDoc = result2XML(pResult);
	pXmlDoc->SaveFile(strResultFile.c_str());

	delete pXmlDoc;
	pXmlDoc = nullptr;
}

CompareResult* ImageOperation::loadResultFormXml(const std::string& strResultFile)
{
	namespace xml = tinyxml2;
	xml::XMLDocument* pxmlDoc = new xml::XMLDocument();
	ON_SCOPE_EXIT([&pxmlDoc]() {delete pxmlDoc; pxmlDoc = nullptr; });
	if (pxmlDoc->LoadFile(strResultFile.c_str()) == xml::XML_NO_ERROR)
	{
		auto pResult = xml2Result(pxmlDoc);
		return pResult;
	}
	return nullptr;
}

