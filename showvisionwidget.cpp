#include "showvisionwidget.h"

ShowVisionWidget::ShowVisionWidget(QWidget* parent): QLabel(parent)
{

}

ShowVisionWidget::~ShowVisionWidget()
{

}

QSize ShowVisionWidget::sizeHint() const
{
    if (img) {
        return QSize(img->width(), img->height());
    } else {
        return QWidget::sizeHint();
    }
}

void ShowVisionWidget::setCalibrationHandler(CalibrationHandler* _ch)
{
    ch = _ch;
}

void ShowVisionWidget::paintEvent(QPaintEvent*)
{
    if (img) {
        QPainter paint(this);
        paint.drawImage(img->rect(), *img, img->rect());
    }
}

vector<Point> punto_central_color(Mat imgThresholded){

    int thresh = 100;

    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    vector<int> puntos;
    vector<Point> puntos_color;

    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(6, 6)) );

    Canny( imgThresholded, canny_output, thresh, thresh*2, 3 );

    findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    vector<Moments> mu(contours.size() );
    for( size_t i = 0; i < contours.size(); i++ ){
        mu[i] = moments( contours[i], false );
    }

    vector<Point2f> mc( contours.size() );
    for( size_t i = 0; i < contours.size(); i++ ){
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
    }

    for( size_t i = 0; i< contours.size(); i++ ){
        if (i%2!=0){
            Point p;
            p.x = (int)mc[i].x;
            p.y = (int)mc[i].y;
            puntos_color.push_back(p);
        }
    }

    return puntos_color;
}


vector<Point> punto_central(vector<Point> color_equipo, vector<Point> color_jugador){
    vector<Point> puntos_cercanos;
    int distancia;
    int dist_min = 1000;
    Point p1, p2, p_med;


    for(size_t i=0; i<color_equipo.size(); i++){
        for(size_t j=0; j<color_jugador.size(); j++){
            distancia = sqrt (pow(color_equipo.at(i).x-color_jugador.at(j).x, 2) + pow(color_equipo.at(i).y-color_jugador.at(j).y, 2));
            if (distancia<dist_min){
                dist_min = distancia;
                p1 = color_equipo.at(i);
                p2 = color_jugador.at(j);
                p_med.x = (p1.x+p2.x)/2;
                p_med.y = (p1.y+p2.y)/2;
            }
        }
    }

    puntos_cercanos.push_back(p1);
    puntos_cercanos.push_back(p2);
    puntos_cercanos.push_back(p_med);

    return puntos_cercanos;
}

vector<Point> ShowVisionWidget::proc(Mat* frame) {
    Mat imgThresholded;
    /////////////////////////////////////////////////// AMARILLO
    Scalar lower_bound(ch->yellowCalib.data[0], ch->yellowCalib.data[1], ch->yellowCalib.data[2]);
    Scalar upper_bound(ch->yellowCalib.data[3], ch->yellowCalib.data[4], ch->yellowCalib.data[5]);
    inRange(*frame, lower_bound, upper_bound, imgThresholded); //Threshold the image

    vector<Point> ptos_amarillo = punto_central_color(imgThresholded);

//    Mat drawing = Mat::zeros(imgThresholded.size(), CV_8UC3 );

//    for (int k = 0; k<ptos_amarillo.size(); k++){
//        //cout<<"AM_"<<k+1<<": "<<ptos_amarillo.at(k).x<<" ; "<<ptos_amarillo.at(k).y<<endl;
//        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//        circle( drawing, Point(ptos_amarillo.at(k).x,ptos_amarillo.at(k).y), 1, color, -1, 8, 0 );
//    }

    //imshow("Thresholded AMARILLO", imgThresholded);

    /////////////////////////////////////////////////// AZUL
    lower_bound = Scalar(ch->blueCalib.data[0], ch->blueCalib.data[1], ch->blueCalib.data[2]);
    upper_bound = Scalar(ch->blueCalib.data[3], ch->blueCalib.data[4], ch->blueCalib.data[5]);
    inRange(*frame, lower_bound, upper_bound, imgThresholded); //Threshold the image
    vector<Point> ptos_azul = punto_central_color(imgThresholded);

//    for (int k = 0; k<ptos_azul.size(); k++){
//        //cout<<"AZ_"<<k+1<<": "<<ptos_azul.at(k).x<<" ; "<<ptos_azul.at(k).y<<endl;
//        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//        circle( drawing, Point(ptos_azul.at(k).x,ptos_azul.at(k).y), 1, color, -1, 8, 0 );
//    }

    //imshow("Thresholded AZUL", imgThresholded);


    /////////////////////////////////////////////////// ROJO
    lower_bound = Scalar(ch->redCalib.data[0], ch->redCalib.data[1], ch->redCalib.data[2]);
    upper_bound = Scalar(ch->redCalib.data[3], ch->redCalib.data[4], ch->redCalib.data[5]);
    inRange(*frame, lower_bound, upper_bound, imgThresholded); //Threshold the image
    vector<Point> ptos_rojo = punto_central_color(imgThresholded);

//    for (int k = 0; k<ptos_rojo.size(); k++){
//        //cout<<"RO_"<<k+1<<": "<<ptos_rojo.at(k).x<<" ; "<<ptos_rojo.at(k).y<<endl;
//        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//        circle( drawing, Point(ptos_rojo.at(k).x,ptos_rojo.at(k).y), 1, color, -1, 8, 0 );
//    }


    /////////////////////////////////////////////////// VERDE
    lower_bound = Scalar(ch->greenCalib.data[0], ch->greenCalib.data[1], ch->greenCalib.data[2]);
    upper_bound = Scalar(ch->greenCalib.data[3], ch->greenCalib.data[4], ch->greenCalib.data[5]);
    inRange(*frame, lower_bound, upper_bound, imgThresholded); //Threshold the image
    vector<Point> ptos_verde = punto_central_color(imgThresholded);

//    for (int k = 0; k<ptos_verde.size(); k++){
//        //cout<<"VE_"<<k+1<<": "<<ptos_verde.at(k).x<<" ; "<<ptos_verde.at(k).y<<endl;
//        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//        circle( drawing, Point(ptos_verde.at(k).x,ptos_verde.at(k).y), 1, color, -1, 8, 0 );
//    }

//    imshow("Thresholded VERDE", imgThresholded);

    /////////////////////////////////////////////////// CYAN

    lower_bound = Scalar(ch->redCalib.data[0], ch->redCalib.data[1], ch->redCalib.data[2]);
    upper_bound = Scalar(ch->redCalib.data[3], ch->redCalib.data[4], ch->redCalib.data[5]);
    inRange(*frame, lower_bound, upper_bound, imgThresholded); //Threshold the image
    vector<Point> ptos_cyan = punto_central_color(imgThresholded);

//    for (int k = 0; k<ptos_cyan.size(); k++){
//        //cout<<"CY_"<<k+1<<": "<<ptos_cyan.at(k).x<<" ; "<<ptos_cyan.at(k).y<<endl;
//        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//        circle( drawing, Point(ptos_cyan.at(k).x,ptos_cyan.at(k).y), 1, color, -1, 8, 0 );
//    }

    //imshow("Thresholded CYAN", imgThresholded);

    //// JUGADOR_AMARILLO_ROJO
    vector<Point> jugador_Am_Ro = punto_central(ptos_amarillo,ptos_rojo);
    //line(drawing, jugador_Am_Ro.at(0), jugador_Am_Ro.at(1),Scalar( 110, 220, 0 ),  2, 8);
//    circle(drawing, jugador_Am_Ro.at(2),3,Scalar( 110, 220, 0 ),  2, 8);
    cout<<"P1_AM_RO: "<<jugador_Am_Ro.at(2).x<<" ; "<<jugador_Am_Ro.at(2).y<<endl;

    //// JUGADOR_AMARILLO_VERDE
    vector<Point> jugador_Am_Ve = punto_central(ptos_amarillo,ptos_verde);
    //line(drawing, jugador_Am_Ve.at(0), jugador_Am_Ve.at(1),Scalar( 110, 220, 0 ),  2, 8);
//    circle(drawing, jugador_Am_Ve.at(2),3,Scalar( 110, 220, 0 ),  2, 8);
    cout<<"P2_AM_VE: "<<jugador_Am_Ve.at(2).x<<" ; "<<jugador_Am_Ve.at(2).y<<endl;

    //// JUGADOR_AMARILLO_CYAN
    vector<Point> jugador_Am_Cy = punto_central(ptos_amarillo,ptos_cyan);
    //line(drawing, jugador_Am_Cy.at(0), jugador_Am_Cy.at(1),Scalar( 110, 220, 0 ),  2, 8);
//    circle(drawing, jugador_Am_Cy.at(2),3,Scalar( 110, 220, 0 ),  2, 8);
    cout<<"P3_AM_CY: "<<jugador_Am_Cy.at(2).x<<" ; "<<jugador_Am_Cy.at(2).y<<endl;

    //// JUGADOR_AZUL_ROJO
    vector<Point> jugador_Az_Ro = punto_central(ptos_azul,ptos_rojo);
    //line(drawing, jugador_Am_Ro.at(0), jugador_Am_Ro.at(1),Scalar( 110, 220, 0 ),  2, 8);
//    circle(drawing, jugador_Az_Ro.at(2),3,Scalar( 110, 220, 0 ),  2, 8);
    cout<<"P1_AZ_RO: "<<jugador_Az_Ro.at(2).x<<" ; "<<jugador_Az_Ro.at(2).y<<endl;

    //// JUGADOR_AZUL_VERDE
    vector<Point> jugador_Az_Ve = punto_central(ptos_azul,ptos_verde);
    //line(drawing, jugador_Am_Ve.at(0), jugador_Am_Ve.at(1),Scalar( 110, 220, 0 ),  2, 8);
//    circle(drawing, jugador_Az_Ve.at(2),3,Scalar( 110, 220, 0 ),  2, 8);
    cout<<"P2_AZ_VE: "<<jugador_Az_Ve.at(2).x<<" ; "<<jugador_Az_Ve.at(2).y<<endl;

    //// JUGADOR_AZUL_CYAN
    vector<Point> jugador_Az_Cy = punto_central(ptos_azul,ptos_cyan);
    //line(drawing, jugador_Am_Cy.at(0), jugador_Am_Cy.at(1),Scalar( 110, 220, 0 ),  2, 8);
//    circle(drawing, jugador_Az_Cy.at(2),3,Scalar( 110, 220, 0 ),  2, 8);
    cout<<"P3_AZ_CY: "<<jugador_Az_Cy.at(2).x<<" ; "<<jugador_Az_Cy.at(2).y<<endl;

//    imgOriginal = imgOriginal + drawing;
//    imshow("Original", imgOriginal); //show the original image
    return vector<Point>();
}


void ShowVisionWidget::setFrame(Mat *frame)
{
    QImage qimg((uchar*)frame->data, frame->cols, frame->rows, frame->step, QImage::Format_RGB888);
    qimg = qimg.copy();
    if (img) {
        delete img;
    }
    img = new QImage(qimg);
    repaint();
    if (width() != img->width()) {
        resize(img->width(), img->height());
        updateGeometry();
    }
}
