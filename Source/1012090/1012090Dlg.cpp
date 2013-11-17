
// 1012090Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "1012090.h"
#include "1012090Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy1012090Dlg dialog




CMy1012090Dlg::CMy1012090Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy1012090Dlg::IDD, pParent)
	, m_strLocation(_T(""))
	, m_pImage(NULL)
	, m_pRGBImage(NULL)
	, m_pHSVImage(NULL)
	, m_pHistogramImage(NULL)
	, m_pHistogramCCVImage(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy1012090Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOCATION, m_strLocation);
	DDX_Control(pDX, IDC_STATIC_PIC5, m_ctrMainPic);
	DDX_Control(pDX, IDC_STATIC_PIC2, m_ctrRGBImage);
	DDX_Control(pDX, IDC_STATIC_PIC3, m_ctrHistogramGRBImage);
	DDX_Control(pDX, IDC_STATIC_PIC6, m_ctrHistogramCCVImage);
}

BEGIN_MESSAGE_MAP(CMy1012090Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CMy1012090Dlg::OnBnClickedButtonBrowse)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMy1012090Dlg message handlers

BOOL CMy1012090Dlg::OnInitDialog()
{
	//isCanDo = false;
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMy1012090Dlg::OnPaint()
{

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMy1012090Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMy1012090Dlg::OnBnClickedButtonBrowse()
{
	// TODO: Add your control notification handler code here
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_EXTENSIONDIFFERENT, _T("JPEG (*.jpeg, *.jpg) | *.jpeg; *.jpg|"), this);
	if( fileDlg.DoModal() == IDOK ){
		this->m_strLocation = fileDlg.GetPathName();
		this->UpdateData(FALSE);

		if ( this->m_pImage != NULL ) // --> Can phai thuc hien de giai phong vung nho.
		{
			// Khi Browse den du lieu anh moi thi phai xoa du lieu anh cu
			cvReleaseImage (&(this->m_pImage));
			m_pImage = NULL;
		}
		if( this->m_pRGBImage != NULL ){
			cvReleaseImage(&(this->m_pRGBImage));
			m_pRGBImage = NULL;
		}
		if( this->m_pHistogramImage != NULL ){
			cvReleaseImage(&(this->m_pHistogramImage));
			m_pHistogramImage = NULL;
		}
		if( this->m_pHSVImage != NULL ){
			cvReleaseImage(&(this->m_pHistogramImage));
			m_pHistogramImage = NULL;
		}
		if( this->m_pHistogramCCVImage != NULL ){
			cvReleaseImage(&m_pHistogramCCVImage);
			m_pHistogramCCVImage = NULL;
		}
	
		USES_CONVERSION;

		char* p = W2A( this->m_strLocation);

		this->m_pImage = cvLoadImage(p);


		if( this->m_pImage != NULL ){
			/*IplImage *destination = cvCreateImage(cvSize(100, 100), this->m_pImage->depth, this->m_pImage->nChannels);
			cvResize(this->m_pImage, destination);
			this->m_pImage = destination;*/
			IplImage *destination = cvCreateImage(cvSize(ccv_width, ccv_height), m_pImage->depth, m_pImage->nChannels);
			cvResize(m_pImage, destination);
			m_pImage = destination;

			this->m_pHSVImage =
				this->RGB2HSVImage(m_pImage);
			this->m_pRGBImage =
				this->HSV2RGBImage(m_pHSVImage);

			this->m_pHistogramImage = this->drawHistogram(m_pHSVImage);

				

			RECT rect;
			this->m_ctrMainPic.GetClientRect(&rect);
			this->m_cShower.CopyOf(m_pImage, 1);
			this->m_cShower.DrawToHDC(this->m_ctrMainPic.GetDC()->m_hDC, &rect);
	
			this->m_ctrRGBImage.GetClientRect(&rect);
			this->m_cShower.CopyOf(m_pRGBImage, 1);
			this->m_cShower.DrawToHDC(this->m_ctrRGBImage.GetDC()->m_hDC, &rect);
			
			this->m_ctrHistogramGRBImage.GetClientRect(&rect);
			this->m_cShower.CopyOf(m_pHistogramImage, 1);
			this->m_cShower.DrawToHDC(this->m_ctrHistogramGRBImage.GetDC()->m_hDC, &rect);


			feature_ccv featureCCV;
	
			ccv(this->m_pImage, featureCCV, m_pHistogramCCVImage);

			this->m_ctrHistogramCCVImage.GetClientRect(&rect);
			this->m_cShower.CopyOf(m_pHistogramCCVImage, 1);
			this->m_cShower.DrawToHDC(this->m_ctrHistogramCCVImage.GetDC()->m_hDC, &rect);

			cvReleaseImage( &(this->m_pRGBImage));
			cvReleaseImage( &(this->m_pHistogramImage));
			cvReleaseImage( &(this->m_pImage));
			cvReleaseImage(&m_pHistogramCCVImage);
		}
	}else{
		AfxMessageBox(_T( "Something goes wrong here" ));
	}
}


void CMy1012090Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	if( this->m_pImage != NULL ){
		cvReleaseImage(&m_pImage);
	}
		IplImage* m_pImage;
	IplImage* m_pRGBImage;
	IplImage* m_pHSVImage;
	IplImage* m_pHistogramImage;

	if( this->m_pImage != NULL ){
		cvReleaseImage(&m_pImage);
	}
	if( this->m_pRGBImage != NULL ){
		cvReleaseImage(&m_pRGBImage);
	}
	if( this->m_pHSVImage != NULL ){
		cvReleaseImage(&m_pHSVImage);
	}
	if( this->m_pHistogramImage != NULL ){
		cvReleaseImage(&m_pHistogramImage);
	}
	if( this->m_pHistogramCCVImage != NULL ){
		cvReleaseImage(&m_pHistogramCCVImage);
	}
	// TODO: Add your message handler code here
}

IplImage* CMy1012090Dlg::RGB2HSVImage(IplImage* rgbImage){
	IplImage* hsvImage = cvCreateImage(cvSize(rgbImage->width,rgbImage->height),IPL_DEPTH_32F,3);

	int height    = rgbImage->height;
    int width     = rgbImage->width;

	 for(int i=0;i<height;i++) for(int j=0;j<width;j++){
	  CvScalar s,s1;
	s=cvGet2D(rgbImage,i,j); // get the (i,j) pixel value

	s1 = GRB2HSV(s);

//	cout << hue << "  " << sa << "  " << va << endl;
//	int pos = s1.val[0];
//	histogramValue[pos]++;
	//cout << s1.val[0] << "  ";
 	cvSet2D(hsvImage,i,j,s1); // set the (i,j) pixel value
  }

	return hsvImage;
}

void CMy1012090Dlg::ccv(IplImage* src, feature_ccv &ret, IplImage* &histogramCCVImage){
	uint32_t *labeled;
	unordered_map<uint32_t, label_info> labels;
	
	//IplImage *destination = cvCreateImage(cvSize(ccv_width, ccv_height), src->depth, src->nChannels);
	//cvResize(src, destination);
	//cout << destination->width << " " << destination->height;
	

	//ccv_width = src->width;
	//ccv_height = src->height;

	
	src = RGB2HSVImage(src);



	ret.m_alpha = new float[NUM_CCV_COLOR];
	ret.m_beta = new float[NUM_CCV_COLOR];

	for( int i = 0; i < NUM_CCV_COLOR; i++){
		ret.m_alpha[i] = 0.0f;
		ret.m_alpha[i] = 0.0f;
	}

	label_info info;
	rgb color;
	uint32_t label;
	int size;

	size = ccv_width*ccv_height;

	labeled = new uint32_t[size];
	get_rgb(color, src, 0, 0 );

	label = 0;

	labeled[0] = label;

	info.m_color = color;
	info.m_alias = label;
	info.m_count = 1;

	labels[label] = info;

	int x, y;
	for( x = 1; x < ccv_width; x++ ){
		rgb color_here, color_left;

		get_rgb(color_here, src, x, 0);
		get_rgb(color_left, src, x - 1, 0);

		if( color_here.m_color32 == color_left.m_color32){
			labels[label].m_count++;
			labeled[x] = label;
		}else{
			label++;
			labeled[x] = label;

			info.m_color = color_here;
			info.m_alias = label;
			info.m_count = 1;

			labels[label] = info;

		}
	
	}

	for ( y = 1; y < ccv_height ; y++){
		rgb c_here, c_left, c_above, c_above_l, c_above_r;

		c_left.m_color32 = 0xffffffff;
		c_above_l.m_color32 = 0xffffffff;
		get_rgb(c_above, src, 0, y-1);

		for	(x = 0; x < ccv_width ; x++){
			get_rgb(c_here, src, x, y);
			if( x + 1 < ccv_width){
				get_rgb(c_above_r, src, x+ 1, y - 1);
			}else{
				c_above_r.m_color32 = 0xffffffff;
			}

			uint32_t same[4];
            uint32_t l;
            int      num_same = 0;

            if (c_here == c_left) {
                    l = labeled[x - 1 + y * ccv_width];
                    same[num_same] = find_label(l, labels);
                    num_same++;
            }

            if (c_here == c_above) {
                    l = labeled[x + (y - 1) * ccv_width];
                    same[num_same] = find_label(l, labels);
                    num_same++;
            }

            if (c_here == c_above_l) {
                    l = labeled[x - 1 + (y - 1) * ccv_width];
                    same[num_same] = find_label(l, labels);
                    num_same++;
            }

            if (c_here == c_above_r) {
                    l = labeled[x + 1 + (y - 1) * ccv_width];
                    same[num_same] = find_label(l, labels);
                    num_same++;
            }

            if (num_same > 0) {
                    unordered_map<uint32_t,
                            label_info>::iterator it1, it2;
                    uint32_t label_min = same[0];

                    for (int i = 1; i < num_same; i++) {
                            if (same[i] < label_min) {
                                    label_min = same[i];
                            }
                    }

                    it1 = labels.find(label_min);
                    it1->second.m_count++;

                    for (int i = 0; i < num_same; i++) {
                        if (label_min < same[i]) {
                            it2 = labels.find(same[i]);

                            int count = it2->second.m_count;

                            it2->second.m_alias = label_min;
                            it2->second.m_count = 0;

                            it1->second.m_count += count;
                        }
                    }

                    labeled[x + y * ccv_width] = label_min;
            } else {
                    label++;
                    labeled[x + y * ccv_width] = label;

                    info.m_color = c_here;
                    info.m_alias = label;
                    info.m_count = 1;

                    labels[label] = info;
            }
			if( x == 535){
				
			}

		}
	
		c_left = c_here;
		c_above = c_above_r;
		c_above_l = c_above;
	}

	unordered_map<uint32_t, label_info>::iterator it;
        for (it = labels.begin(); it != labels.end(); ++it) {
                int idx;
				idx = it->second.m_color.m_color32;

                if (it->second.m_count > t_ccv) {
					if(idx < NUM_CCV_COLOR){
                        ret.m_alpha[idx] += it->second.m_count;
					}
                } else {
					if(idx < NUM_CCV_COLOR){
                      ret.m_beta[idx] += it->second.m_count;
					}
                }
        }

		

		delete(labeled);
		delete(ret.m_beta);
		histogramCCVImage = drawHistogramHSV(ret.m_alpha, nHStatic, nSStatic, nVStatic, nGStatic);
}

void CMy1012090Dlg::get_rgb(rgb &color, IplImage* src, int x, int y){
	CvScalar s,s1;
	s=cvGet2D(src, y, x); // get the (i,j) pixel value
	s1 = HSV2HSVq(nHStatic,nSStatic,nVStatic,nGStatic,s);

	uint32_t index = HSVq2Index(nHStatic, nSStatic, nVStatic, nGStatic,s1);
	color.m_color32 = index;
}

IplImage* CMy1012090Dlg::drawHistogramHSV(float *arrayOfHistogram, int nH, int nS, int nV, int nG){
	IplImage* histogram = cvCreateImage(cvSize(1500,500),IPL_DEPTH_8U,3);

	CvPoint  curve1[]={5,5,  10,100,  100,100,  100,10};
CvPoint  curve2[]={30,30,  30,130,  130,130,  130,30,  150,10};
CvPoint* curveArr[2]={curve1, curve2};
int      nCurvePts[2]={4,5};
int      nCurves=2;
int      isCurveClosed=1;
int      lineWidth=1;

cvPolyLine(histogram,curveArr,nCurvePts,nCurves,isCurveClosed,cvScalar(0,255,255),lineWidth);

		cvSet(histogram,CV_RGB(230,230,230));

	CvSize sizeCanvas = cvSize(histogram->width,histogram->height);

	CvPoint corner1,corner2;
			corner1.x = 20;
			corner1.y = 20;
			corner2.x = histogram->width - 20;
			corner2.y = histogram->height - 20;
	CvPoint curve1T[4];//={10,10,  10,100,  100,100,  100,10};
			curve1T[0] = corner1;
			curve1T[1] = cvPoint(corner2.x,corner1.y);
			curve1T[2] = corner2;
			curve1T[3] = cvPoint(corner1.x,corner2.y);
			cvLine(histogram, curve1T[0], curve1T[1], cvScalar(0,0,0), 3);
			cvLine(histogram, curve1T[1], curve1T[2], cvScalar(0,0,0), 3);
			cvLine(histogram, curve1T[2], curve1T[3], cvScalar(0,0,0), 3);
			cvLine(histogram, curve1T[3], curve1T[0], cvScalar(0,0,0), 3);


			int axisX = sizeCanvas.width - 100, axisY = sizeCanvas.height - 100;
	CvPoint coordinateOrigin = cvPoint(50,sizeCanvas.height - 50);
	int f = 0;

	int h,s,v;
	
	 float dentaY = 0;
	 int maxHistogram = arrayOfHistogram[0];
	 for(int i = 1; i < nH*nS*nV + nG; i++ ){
		 if(arrayOfHistogram[i] > maxHistogram )
			 maxHistogram = arrayOfHistogram[i];
	 }
	 dentaY = axisY*1.0/maxHistogram;



	float dentaX = (axisX - coordinateOrigin.x )*1.0/(nH*nS*nV+nG);
	for(int i = 0; i < nH*nS*nV +nG; i++ ){
			//	 cout << r << "  " <<g << "  " << b << endl;
			//	   cout << dentaYi << endl; 

		CvScalar hsvQC = Index2HSVq(nH,nS,nV,nG,i);
		h = hsvQC.val[0];
		s = hsvQC.val[1];
		v = hsvQC.val[2];
				corner1.x = coordinateOrigin.x+ (axisX-coordinateOrigin.x)*f/(nH*nS*nV + nG);
				corner1.y = coordinateOrigin.y;
				corner2.x = corner1.x+ dentaX;
				corner2.y = coordinateOrigin.y - 50;
				  CvPoint  curve1[4];//={10,10,  10,100,  100,100,  100,10};



				  curve1[0] = corner1;
				  curve1[1] = cvPoint(corner2.x,corner1.y);
				  curve1[2] = corner2;
				  curve1[3] = cvPoint(corner1.x,corner2.y);
			CvPoint* curveArr = curve1;
			int      nCurvePts = 4;
			int      nCurves=1;
			//cvFillPoly(histogram,curveArr,nCurvePts,nCurves,cvScalar(0,255,255));
			CvScalar hsvQ = {h,s,v};

			CvScalar hsv = HqSqVq2HSV(nH,nS,nV,nG, hsvQ);

			CvScalar rgb = HSV2RGB(hsv);

			cvFillConvexPoly(histogram,curveArr,nCurvePts,CV_RGB(rgb.val[0],rgb.val[1],rgb.val[2]));


			corner1.x = coordinateOrigin.x+ dentaX*f;
				corner1.y = coordinateOrigin.y-50;
				corner2.x = corner1.x+ dentaX;
				corner2.y = coordinateOrigin.y -  dentaY*arrayOfHistogram[i]-50;
				  curve1[0] = corner1;
				  curve1[1] = cvPoint(corner2.x,corner1.y);
				  curve1[2] = corner2;
				  curve1[3] = cvPoint(corner1.x,corner2.y);
				//  cvFillConvexPoly(histogram,curve1,nCurvePts,CV_RGB(0, 0, 0));

				 cvLine(histogram, curve1[0], curve1[1], cvScalar(0,0,0), 1);
			cvLine(histogram, curve1[1], curve1[2], cvScalar(0,0,0), 1);
			cvLine(histogram, curve1[2], curve1[3], cvScalar(0,0,0), 1);
			cvLine(histogram, curve1[3], curve1[0], cvScalar(0,0,0), 1);
				  				f++;
		}

	delete(arrayOfHistogram);
	return histogram;
}

IplImage* CMy1012090Dlg::RGB2HSV(IplImage* originalImage, IplImage* &histogramHSVImage){
	IplImage* hsvImage = cvCreateImage(cvSize(originalImage->width,originalImage->height),IPL_DEPTH_32F,3);

	int height    = originalImage->height;
    int width     = originalImage->width;

	 for(int i=0;i<height;i++) for(int j=0;j<width;j++){
	  CvScalar s,s1;
	s=cvGet2D(originalImage,i,j); // get the (i,j) pixel value

	s1 = GRB2HSV(s);
//	cout << hue << "  " << sa << "  " << va << endl;
//	int pos = s1.val[0];
//	histogramValue[pos]++;
	//cout << s1.val[0] << "  ";
 	cvSet2D(hsvImage,i,j,s1); // set the (i,j) pixel value
  }

//	 drawHistogram(histogramValue,histogramImage,cvSize(histogramImage->width,histogramImage->height)
//		 ,histogramImage->width*histogramImage->height);

	return hsvImage;
}

float maxFloat(float a, float b, float c ){
	if( a >= b && a >= c )
		return a;
	if( b >= a && b >= c )
		return b;
	return c;
}

float minFloat(float a, float b, float c ){
	if( a <= b && a <= c )
		return a;
	if( b <= a && b <= c )
		return b;
	return c;
}

CvScalar CMy1012090Dlg::GRB2HSV(CvScalar rgb){
	CvScalar hsv;

	float r,g,b;
	float rT,gT,bT;
	bT = rgb.val[0]*1.0;
	gT = rgb.val[1]*1.0;
	rT = rgb.val[2]*1.0;

	b = rgb.val[0]*1.0/255;
	g = rgb.val[1]*1.0/255;
	r = rgb.val[2]*1.0/255;

	float hue = 0,sa = 0,va = 0;
	va = maxFloat(b,g,r);
	if((maxFloat(bT,gT,rT) - minFloat(bT,gT,rT) == 0) || maxFloat(bT,gT,rT) == 0 ){
		sa = 0.0;
	}else{
		sa = (va - minFloat(b,g,r))/va;
	}

	if(va == r && sa){
		hue = 60.0*(g-b)/sa;
	}
	if( va == g && sa){
		hue = 120 + 60.0*(b-r)/sa;
	}
	if( va == b && sa){
		hue = 240 + 60.0*(r-g);
	}
	if( hue < 0 )
		hue += 360;
	if ( sa == 0 )
		hue = -1;
//	s1 = cvGet2D(rgbImage,i,j);
	//printf("B=%f, G=%f, R=%f\n",s.val[0],s.val[1],s.val[2]);
	hsv.val[0] = hue;
	hsv.val[1] = sa;
	hsv.val[2] = va;
	
	return hsv;
}


IplImage* CMy1012090Dlg::HSV2RGBImage(IplImage* hsvImage){
	IplImage* rgbImage = cvCreateImage(cvSize(hsvImage->width,hsvImage->height),IPL_DEPTH_8U,3);

	int height    = hsvImage->height;
    int width     = hsvImage->width;
	int c = 0;
	 for(int i=0;i<height;i++) for(int j=0;j<width;j++){
	  CvScalar s,s1;
	s=cvGet2D(hsvImage,i,j); // get the (i,j) pixel value

	s1 = HSV2RGB(s);

 	cvSet2D(rgbImage,i,j,s1); // set the (i,j) pixel value
  }

	return rgbImage;
}

int CMy1012090Dlg::HSVq2Index(int nH, int nS, int nV, int nG, CvScalar hsvQ){
	int index;

	if(hsvQ.val[0] < 0 ){
		index = nH*nS*nV + hsvQ.val[2];
	}else{
		index = nS*nV*hsvQ.val[0] + nV*hsvQ.val[1] + hsvQ.val[2];
	}

	return index;
}
CvScalar CMy1012090Dlg::HSV2HSVq(int nH, int nS, int nV, int nG, CvScalar hsv){
	int hQ,sQ,vQ;

	hQ = (int)(hsv.val[0]*nH*1.0/360);
	sQ = (int)(hsv.val[1]*nS*1.0);
	vQ = (int)(hsv.val[2]*nV*1.0);

	if(hsv.val[0] < 0 ){
		hQ = -1;
		sQ = -1;
		vQ = (int)(hsv.val[2]*nG/1.0);
	}
	CvScalar hsvQ = {hQ, sQ, vQ};

	return hsvQ;
}

CvScalar CMy1012090Dlg::HSV2RGB(CvScalar hsv){
	CvScalar rgb;

	float hue,sa,va;
	float b = 0,g = 0,r = 0;
	hue = hsv.val[0];
	sa = hsv.val[1];
	va = hsv.val[2];

	if( hue == -1 ){
		r = g = b = va;
	}else{
		int hInt;
		hInt = (int)(hue/60);
	/*	if(hInt == c ){
			cout << hInt << endl;
			c ++;
			if( c == 6 )
				c = 0;
		}*/
		float min = va - sa*va;
		
		switch(hInt){
		case 0:
			r = va;
			b = min;
			g = hue*sa*1.0/60+b;
			break;
		case 1:
			g = va;
			b = min;
			r = b - (hue-120)*sa*1.0/60;
			break;
		case 2:
			g = va;
			r = min;
			b = (hue-120)*sa*1.0/60+r;
			break;
		case 3:
			b = va;
			r = min;
			g = r - (hue-240)*sa*1.0/60;
			break;
		case 4:
			b = va;
			g = min;
			r = (hue-240)*sa*1.0/60 + g;
			break;
		case 5:
			r = va;
			g = min;
			b = g - hue*sa*1.0/60;
			break;
		}
	}
//	cout << hue << "  " << sa << "  " << va << endl;
	rgb.val[0] = b*255;
	rgb.val[1] = g*255;
	rgb.val[2] = r*255;
	return rgb;
}

CvScalar CMy1012090Dlg::HqSqVq2HSV(int nH, int nS, int nV, int nG, CvScalar hsvQ){
	float h = 0,s = 0,v = 0;
	if(hsvQ.val[0] >= 0 ){
		h = (360.0/nH)*hsvQ.val[0]+360.0/(2*nH);
		s = (1.0/nS)*hsvQ.val[1]+1.0/(2*nS);
		v = (1.0/nV)*hsvQ.val[2]+1.0/(2*nV);
	}else{
		h = -1;
		v  = hsvQ.val[2]*1.0/255;
	}
	
	CvScalar hsv = {h,s,v};
//	hsv.val[0] = 
	return hsv;
}

CvScalar CMy1012090Dlg::Index2HSVq(int nH, int nS, int nV,int nG, int i){
	int hQ = 0,sQ = 0,vQ = 0;
	if( i >= nH*nS*nV){
		int gQ = i - nH*nS*nV;
		float h,s,v;
		h = -1;
		v = (256*1.0/nG)*gQ + (256*1.0/(2*nG));
		s = 0;
		CvScalar hsv = {h,s,v};
		return hsv;
	}
	vQ = i%nV;
	int a = i/nV;
	sQ = a%nS;
	hQ = a/nS;
	
	CvScalar hsv = {hQ,sQ,vQ};
//	hsv.val[0] = 
	return hsv;
}




IplImage* CMy1012090Dlg::drawHistogram(IplImage *HSVImage){
	IplImage* histogram = cvCreateImage(cvSize(1500,500),IPL_DEPTH_8U,3);

	CvPoint  curve1[]={5,5,  10,100,  100,100,  100,10};
CvPoint  curve2[]={30,30,  30,130,  130,130,  130,30,  150,10};
CvPoint* curveArr[2]={curve1, curve2};
int      nCurvePts[2]={4,5};
int      nCurves=2;
int      isCurveClosed=1;
int      lineWidth=1;

cvPolyLine(histogram,curveArr,nCurvePts,nCurves,isCurveClosed,cvScalar(0,255,255),lineWidth);

		cvSet(histogram,CV_RGB(230,230,230));

	CvSize sizeCanvas = cvSize(histogram->width,histogram->height);

	CvPoint corner1,corner2;
			corner1.x = 20;
			corner1.y = 20;
			corner2.x = histogram->width - 20;
			corner2.y = histogram->height - 20;
	CvPoint curve1T[4];//={10,10,  10,100,  100,100,  100,10};
			curve1T[0] = corner1;
			curve1T[1] = cvPoint(corner2.x,corner1.y);
			curve1T[2] = corner2;
			curve1T[3] = cvPoint(corner1.x,corner2.y);
			cvLine(histogram, curve1T[0], curve1T[1], cvScalar(0,0,0), 3);
			cvLine(histogram, curve1T[1], curve1T[2], cvScalar(0,0,0), 3);
			cvLine(histogram, curve1T[2], curve1T[3], cvScalar(0,0,0), 3);
			cvLine(histogram, curve1T[3], curve1T[0], cvScalar(0,0,0), 3);


			int axisX = sizeCanvas.width - 100, axisY = sizeCanvas.height - 100;
	CvPoint coordinateOrigin = cvPoint(50,sizeCanvas.height - 50);
	int f = 0;
	//for(int h = 0; h < 12; h++ ){
	//	for(int s = 0; s < 4; s++){
	//		for(int v = 0; v < 4; v++){
	//		//	 cout << r << "  " <<g << "  " << b << endl;
	//		//	   cout << dentaYi << endl; 
	//			corner1.x = coordinateOrigin.x+ (axisX-coordinateOrigin.x)*f/(12*4*4);
	//			corner1.y = coordinateOrigin.y;
	//			corner2.x = corner1.x+ (axisX-coordinateOrigin.x)/255;
	//			corner2.y = coordinateOrigin.y - 100;
	//			f++;
	//			  CvPoint  curve1[4];//={10,10,  10,100,  100,100,  100,10};



	//			  curve1[0] = corner1;
	//			  curve1[1] = cvPoint(corner2.x,corner1.y);
	//			  curve1[2] = corner2;
	//			  curve1[3] = cvPoint(corner1.x,corner2.y);
	//		CvPoint* curveArr = curve1;
	//		int      nCurvePts = 4;
	//		int      nCurves=1;
	//		//cvFillPoly(histogram,curveArr,nCurvePts,nCurves,cvScalar(0,255,255));
	//		CvScalar hsvQ = {h,s,v};
	//		cout << h << "  " << s <<"  " << v << endl;
	//		CvScalar hsv = HqSqVq2HSV(12,4,4,5, hsvQ);
	//		cout << hsv.val[0] << "  " << hsv.val[1] <<"  " << hsv.val[2] << endl;
	//		CvScalar rgb = HSV2RGB(hsv);
	//		cout << rgb.val[0] << "  " << rgb.val[1] <<"  " << rgb.val[2] << endl << endl;

	//		cvFillConvexPoly(histogram,curveArr,nCurvePts,CV_RGB(rgb.val[0],rgb.val[1],rgb.val[2]));

	//		}
	//	}
	//}

	int h,s,v;
	int nH = 18, nS = 4, nV = 4, nG = 10;
	int *arrayOfHistogram = new int[nH*nS*nV+nG];
	for(int i = 0; i < nH*nS*nV + nG; i++){
		arrayOfHistogram[i] = 0;
	}
	int width = HSVImage->width, height = HSVImage->height;
	 for(int i=0;i<height;i++) for(int j=0;j<width;j++){
	  CvScalar s,s1;
	s=cvGet2D(HSVImage,i,j); // get the (i,j) pixel value
	s1 = HSV2HSVq(nH,nS,nV,nG,s);


	int index = HSVq2Index(nH,nS,nV,nG,s1);
	arrayOfHistogram[index]++;
 	//cvSet2D(rgbImage,i,j,s1); // set the (i,j) pixel value
  }

	 float dentaY = 0;
	 int maxHistogram = arrayOfHistogram[0];
	 for(int i = 1; i < nH*nS*nV + nG; i++ ){
		 if(arrayOfHistogram[i] > maxHistogram )
			 maxHistogram = arrayOfHistogram[i];
	 }
	 dentaY = axisY*1.0/maxHistogram;



	float dentaX = (axisX - coordinateOrigin.x )*1.0/(nH*nS*nV+nG);
	for(int i = 0; i < nH*nS*nV +nG; i++ ){
			//	 cout << r << "  " <<g << "  " << b << endl;
			//	   cout << dentaYi << endl; 

		CvScalar hsvQC = Index2HSVq(nH,nS,nV,nG,i);
		h = hsvQC.val[0];
		s = hsvQC.val[1];
		v = hsvQC.val[2];
				corner1.x = coordinateOrigin.x+ (axisX-coordinateOrigin.x)*f/(nH*nS*nV + nG);
				corner1.y = coordinateOrigin.y;
				corner2.x = corner1.x+ dentaX;
				corner2.y = coordinateOrigin.y - 50;
				  CvPoint  curve1[4];//={10,10,  10,100,  100,100,  100,10};



				  curve1[0] = corner1;
				  curve1[1] = cvPoint(corner2.x,corner1.y);
				  curve1[2] = corner2;
				  curve1[3] = cvPoint(corner1.x,corner2.y);
			CvPoint* curveArr = curve1;
			int      nCurvePts = 4;
			int      nCurves=1;
			//cvFillPoly(histogram,curveArr,nCurvePts,nCurves,cvScalar(0,255,255));
			CvScalar hsvQ = {h,s,v};

			CvScalar hsv = HqSqVq2HSV(nH,nS,nV,nG, hsvQ);

			CvScalar rgb = HSV2RGB(hsv);

			cvFillConvexPoly(histogram,curveArr,nCurvePts,CV_RGB(rgb.val[0],rgb.val[1],rgb.val[2]));


			corner1.x = coordinateOrigin.x+ dentaX*f;
				corner1.y = coordinateOrigin.y-50;
				corner2.x = corner1.x+ dentaX;
				corner2.y = coordinateOrigin.y -  dentaY*arrayOfHistogram[i]-50;
				  curve1[0] = corner1;
				  curve1[1] = cvPoint(corner2.x,corner1.y);
				  curve1[2] = corner2;
				  curve1[3] = cvPoint(corner1.x,corner2.y);
				//  cvFillConvexPoly(histogram,curve1,nCurvePts,CV_RGB(0, 0, 0));

				 cvLine(histogram, curve1[0], curve1[1], cvScalar(0,0,0), 1);
			cvLine(histogram, curve1[1], curve1[2], cvScalar(0,0,0), 1);
			cvLine(histogram, curve1[2], curve1[3], cvScalar(0,0,0), 1);
			cvLine(histogram, curve1[3], curve1[0], cvScalar(0,0,0), 1);
				  				f++;
		}

//	delete(arrayOfHistogram);

	return histogram;
}