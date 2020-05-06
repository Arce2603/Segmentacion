#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <list>
#include <utility>      // std::pair
#include <time.h>       /* time */


using namespace std;
using namespace cv;

//void mouseCoordinatesExampleCallback(int event, int x, int y, int flags, void* param);
void setColor(int x, int y, int i);
bool isBlack(const Mat &sourceImage, int x, int y);
void checarPix(int x, int y);
void segmentar(const Mat &sourceImage);
void buscaSemilla(const Mat &sourceImage);
bool isInsideFrame(int x, int y);

Mat currentImage,colorImage;
int numObj;
int posX =-1;
int posY =-1;
int area =0;
list <pair <int,int>> nodeList;
vector <int> objArea;
vector <cv::Scalar> color = {Scalar(0,220,220),Scalar(220,220,0),Scalar(220,0,220), Scalar(0,250,0),Scalar(0,0,250),Scalar(250,0,0)};

/*< Main START >*/
int main(int argc, char *argv[])
{	
	cin >> numObj;
	currentImage = imread("test.jpg", CV_LOAD_IMAGE_COLOR);
	namedWindow("Image");
	imshow("Image", currentImage);
	colorImage = Mat(currentImage.rows,currentImage.cols, CV_8UC3, cv::Scalar(0, 0, 0));
  for(int i = 0; i<numObj; i++){
	buscaSemilla(currentImage);
	segmentar(currentImage);
	cout<< "Obj #"<<i<<" area: "<< area<<endl;	
  }

	imshow("Color",colorImage);
  while (true)
  {               
    char key = waitKey(1);                // Si 'x' o ESC es presionada el programa termina
    if(key == 'x' || key == 27 )        // 27 = ESC
    {
      destroyAllWindows(); //Cierra todas las ventanas
      break;
    }

  }
}
/*< Main END >*/



/*Buscar semilla START*/
void buscaSemilla(const Mat &sourceImage){
	posX=sourceImage.rows/2;
	posY=sourceImage.cols/2;
	while(
	  isBlack(currentImage,posX,posY) || 
		(!isBlack(currentImage,posX,posY) && !isBlack(colorImage,posX,posY))){
			posX= rand() % sourceImage.rows;
			posY= rand() % sourceImage.cols;
	}
}
/*Buscar semilla END*/


/* Segmentación START*/
void segmentar(const Mat &sourceImage){
	area = 0;
	nodeList.push_back(make_pair(posX,posY));

	while (!nodeList.empty()){
		checarPix(posX,posY-1);
		checarPix(posX+1,posY);
		checarPix(posX,posY+1);
		checarPix(posX-1,posY);	
		nodeList.pop_front();
		posX = nodeList.front().first;
		posY = nodeList.front().second;
	}
	objArea.push_back(area);
} 
/* Segmentación END*/

void checarPix(int x, int y){
	if(isInsideFrame(x,y) &&
	   !isBlack(currentImage,x,y) &&
	   isBlack(colorImage,x,y)){
		setColor(x,y,objArea.size()%6);// pinta el color
		area++;
		nodeList.push_back(make_pair(x,y));
	}		
}

/* Verificar valor del pixel START */
bool isBlack(const Mat &sourceImage, int x, int y){
	return int(sourceImage.at<Vec3b>(y, x)[0]) < 100 &&
	   int(sourceImage.at<Vec3b>(y, x)[1]) < 100 &&
	   int(sourceImage.at<Vec3b>(y, x)[2]) < 100 ;
}
/* Verificar valor del pixel END */


/*Dentro de la imagen START*/
bool isInsideFrame(int x, int y){
	return x<currentImage.rows && x>0 && y<currentImage.cols && y>0;
}
/*Dentro de la imagen END*/


/* Pintar START*/
void setColor(int x, int y,int i){
	colorImage.at<Vec3b>(y, x)[0] = color[i][0];
	colorImage.at<Vec3b>(y, x)[1] = color[i][1];
	colorImage.at<Vec3b>(y, x)[2] = color[i][2];
}
/* Pintar END*/

