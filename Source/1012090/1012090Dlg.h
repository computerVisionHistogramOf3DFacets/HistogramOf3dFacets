
// 1012090Dlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "highgui_c.h"
#include "vvImage.h"
#include "opencv2\imgproc\imgproc_c.h"
#include <stdint.h>
#include <unordered_map>


#define nHStatic 18 
#define nSStatic 4
#define nVStatic 4
#define nGStatic 10
#define NUM_CCV_COLOR (nHStatic*nSStatic*nVStatic + nGStatic)

using namespace std;

#define round(a) ((int)(a+0.5))

// CMy1012090Dlg dialog

struct feature_ccv{
	float* m_alpha;
	float* m_beta;
};

union rgb{
	uint32_t m_color32;
	uint8_t m_color8[4];

	bool operator==(const rgb &rhs){
		return m_color32 == rhs.m_color32;
	}
};

struct label_info{
	rgb m_color;
	int m_count;
	uint32_t m_alias;
};

static int t_ccv = 5;
static const int ccv_width = 200;
static const int ccv_height = 300;

inline uint32_t find_label(uint32_t label, unordered_map<uint32_t, label_info> &labels){
	unordered_map<uint32_t, label_info>::iterator it;
	for( it = labels.find(label); label != it->second.m_alias;){
		label = it->second.m_alias;
		it = labels.find(label);
	}
	return label;
}




class CMy1012090Dlg : public CDialogEx
{
// Construction
public:
	CMy1012090Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MY1012090_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonBrowse();
	IplImage* RGB2HSV(IplImage* originalImage, IplImage* &histogramHSVImage);
	IplImage* HSV2RGB(IplImage* originalImage);

	IplImage* RGB2HSVImage(IplImage* rgbImage);
	IplImage* HSV2RGBImage(IplImage* hsvImage);

	CvScalar GRB2HSV(CvScalar rgb);
	CvScalar HSV2RGB(CvScalar hsv);
	CvScalar HqSqVq2HSV(int nH, int nS, int nV, int nG, CvScalar hsvQ);
	CvScalar Index2HSVq(int nH, int nS, int nV,int nG, int i);
	int HSVq2Index(int nH, int nS, int nV, int nG, CvScalar hsvQ);
	CvScalar HSV2HSVq(int nH, int nS, int nV, int nG, CvScalar hsv);
	void get_rgb(rgb &color, IplImage* src, int x, int y);
	void ccv(IplImage* src, feature_ccv &ret, IplImage* &histogramCCVImage);
	IplImage* drawHistogramHSV(float *arrayOfHistogram, int nH, int nS, int nV, int nG);
private:
	IplImage* drawHistogram(IplImage *HSVImage);
	// Location of the selected image file
	CString m_strLocation;
//private:
	IplImage* m_pImage;
	IplImage* m_pRGBImage;
	IplImage* m_pHSVImage;
	IplImage* m_pHistogramImage;
	IplImage* m_pHistogramCCVImage;

	CvvImage m_cShower;
	// gray main image

	CStatic m_ctrMainPic;
	CStatic m_ctrRGBImage;
	CStatic m_ctrHistogramGRBImage;
	CStatic m_ctrHistogramCCVImage;
};

