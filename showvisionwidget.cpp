#include "showvisionwidget.h"

ShowVisionWidget::ShowVisionWidget(QWidget* parent): QLabel(parent), socket(this)
{
    socket.bind(QHostAddress("0.0.0.0"), 0);
    img = 0;
    reset();
}

ShowVisionWidget::~ShowVisionWidget()
{
    if (img) {
        delete img;
    }
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

QPoint Point2QPoint(const Point2f& p) {
    return QPoint(round(p.x), round(p.y));
}

bool ShowVisionWidget::validPoint(const Point2f& p) {
    if (!img) {
        return false;
    }
    return p.x >= 0 && p.x < img->width() && p.y >= 0 && p.y < img->height();
}

void ShowVisionWidget::reset()
{
    my_team.assign(3,Point2f(-1, -1));
    my_team_ori.assign(3,Point2f(-1, -1));
    op_team.assign(3,Point2f(-1, -1));
    ball = Point2f(-1, -1);
}

void ShowVisionWidget::paintEvent(QPaintEvent*)
{
    if (img) {
        QPainter paint(this);
        paint.drawImage(img->rect(), *img, img->rect());
        QPoint p, q;
        int r = 14;
        for (int i = 0; i < 3; ++i) {
            if (validPoint(my_team[i])) {
                paint.setPen(Qt::green);
                p = Point2QPoint(my_team[i]);
                paint.drawEllipse(p, r, r);
                Point2f dir = my_team_ori[i] - my_team[i];
                Point2f ori = my_team[i] + 4*dir;
//                cout << "pos " << my_team[i] << endl;
                q = Point2QPoint(ori);
                paint.drawLine(p, q);
            }
            if (validPoint(op_team[i])) {
                p = Point2QPoint(op_team[i]);
                paint.setPen(Qt::red);
                paint.drawEllipse(p, r, r);
            }
        }
        if (validPoint(ball)){
            int r = 7;
            p = Point2QPoint(ball);
            paint.setPen(Qt::blue);
            paint.drawEllipse(p, r, r);
        }
    }
}
/*
int myFloodFill(const Mat& I, Mat& output, int label, int i, int j) {
    // Return how many pixels it filled """
    if (i < 0 || j < 0 || i >= output.rows || j >= output.cols)
        return 0;
    if (output.at<uchar>(i, j) != 0)
        return 0;
    if (I.at<uchar>(i, j) == 0)
        return 0;
    output.at<uchar>(i, j) = label;
    int ret = 1;
    ret += myFloodFill(I, output, label, i+1, j);
    ret += myFloodFill(I, output, label, i-1, j);
    ret += myFloodFill(I, output, label, i, j+1);
    ret += myFloodFill(I, output, label, i, j-1);
    return ret;
}

int myConnectedComponents(const Mat& I, Mat& output, int type) {
    // Return number of components
    // accept only char type matrices
    CV_Assert(I.depth() == CV_8U);
    CV_Assert(type==4);
    int label = 1;

    for(int i = 0; i < I.rows; ++i)
    {
        for (int j = 0; j < I.cols; ++j)
        {
            int filled = myFloodFill(I, output, label, i, j);
            if (filled) {
                label += 1;
            }
        }
    }
}
*/
vector<Point2f> punto_central_color(Mat imgThresholded, const string& color="") {
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
//    if (color=="blue") {
//        imshow("blue", imgThresholded);
//    }
//    if (color=="green") {
//        imshow("green", imgThresholded);
//    }
//    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));


    Mat cc = Mat::zeros(imgThresholded.size(), CV_8U);
    Mat ret;
    QTime time;
    time.start();
    int blobs = connectedComponents(imgThresholded, cc, 4);
//    cout << "CONNECTED COMPONENTS " << time.elapsed() << endl;
    vector<Point2f> puntos_color;
    if (blobs > 4) {
//        cout << "WARNING:" << "Mala calibracion, tengo mas de 3 de color " << color << endl;
        return puntos_color;
    }
    vector<Moments> mu(blobs-1);
    vector<Point2f> mc(blobs-1);

    for (int i = 1; i < blobs; ++i) {
        inRange(cc, i, i, ret);
        Moments m = moments(ret, true);
        Point2f p(m.m10/m.m00 , m.m01/m.m00);
        puntos_color.push_back(p);
    }

    return puntos_color;
}


vector<Point2f> punto_central(const vector<Point2f>& color_equipo, const vector<Point2f>& color_jugador){
    vector<Point2f> puntos_cercanos;
    double dist_min = 20;
    Point2f p1, p2, p_med;

    for(size_t i=0; i<color_equipo.size(); i++){
        for(size_t j=0; j<color_jugador.size(); j++){
            int a = color_equipo[i].x - color_jugador[j].x;
            int b = color_equipo[i].y - color_jugador[j].y;
            double distancia = sqrt(a*a + b*b);
            if (distancia < dist_min){
                dist_min = distancia;
                p1 = color_equipo[i];
                p2 = color_jugador[j];
                p_med.x = (p1.x+p2.x)/2.0;
                p_med.y = (p1.y+p2.y)/2.0;
            }
        }
    }
    if (dist_min == 20) {
        p1 = p2 = p_med = Point2f(-1,-1);
    }

    puntos_cercanos.push_back(p1);
    puntos_cercanos.push_back(p2);
    puntos_cercanos.push_back(p_med);

    return puntos_cercanos;
}

Point2f closest_candidate(const vector<Point2f>& candidates, const Point2f& last_position) {
    int min_dist_cuad = 10000000;
    Point2f closest(-1, -1);

    for (unsigned int i = 0; i < candidates.size(); ++i) {
        int a = last_position.x-candidates[i].x;
        int b = last_position.y-candidates[i].y;
        int dist_cuad = a*a + b*b;
        if (dist_cuad < min_dist_cuad) {
            closest = candidates[i];
            min_dist_cuad = dist_cuad;
        }
    }
    return closest;
}

Point2f ShowVisionWidget::find_ball(Mat imgThresholded) {
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

    Mat cc, ret;
    int blobs = connectedComponents(imgThresholded, cc, 4);

    vector<int> areas;
    vector<Point2f> candidates;
    for (int i = 1; i < blobs; ++i) {
        inRange(cc, i, i, ret);
        int area = countNonZero(ret);
        areas.push_back(area);
        Rect br = boundingRect(ret);
        if (area > 60 && area < 120 && abs(br.width-br.height) < 2) {
            Point2f p;
            p.x = br.x + br.width/2;
            p.y = br.y + br.height/2;
            candidates.push_back(p);
        }
    }

    return closest_candidate(candidates, ball);
}

void ShowVisionWidget::proc(Mat* frame) {
    if (!isVisible()) {
        return;
    }
    QTime time;
    time.start();
    Mat input = *frame;
    Mat imgThresholded;
    Scalar lower_bound, upper_bound;

    vector<Point2f> ptos_amarillo, ptos_azul, ptos_cyan, ptos_rojo, ptos_verde;

    //// I already tried to use threads here (openmp) it makes it 1 ms slower due to memory issues

    /////////////////////////////////////////////////// AMARILLO
    lower_bound = Scalar(ch->yellowCalib.data[0], ch->yellowCalib.data[1], ch->yellowCalib.data[2]);
    upper_bound = Scalar(ch->yellowCalib.data[3], ch->yellowCalib.data[4], ch->yellowCalib.data[5]);
    inRange(input, lower_bound, upper_bound, imgThresholded); //Threshold the image
    ptos_amarillo = punto_central_color(imgThresholded, "yellow");

    /////////////////////////////////////////////////// AZUL
    lower_bound = Scalar(ch->blueCalib.data[0], ch->blueCalib.data[1], ch->blueCalib.data[2]);
    upper_bound = Scalar(ch->blueCalib.data[3], ch->blueCalib.data[4], ch->blueCalib.data[5]);
    inRange(input, lower_bound, upper_bound, imgThresholded); //Threshold the image
    ptos_azul = punto_central_color(imgThresholded, "blue");

    /////////////////////////////////////////////////// ROJO
    lower_bound = Scalar(ch->redCalib.data[0], ch->redCalib.data[1], ch->redCalib.data[2]);
    upper_bound = Scalar(ch->redCalib.data[3], ch->redCalib.data[4], ch->redCalib.data[5]);
    inRange(input, lower_bound, upper_bound, imgThresholded); //Threshold the image
    ptos_rojo = punto_central_color(imgThresholded, "red");

    /////////////////////////////////////////////////// VERDE
    lower_bound = Scalar(ch->greenCalib.data[0], ch->greenCalib.data[1], ch->greenCalib.data[2]);
    upper_bound = Scalar(ch->greenCalib.data[3], ch->greenCalib.data[4], ch->greenCalib.data[5]);
    inRange(input, lower_bound, upper_bound, imgThresholded); //Threshold the image
    ptos_verde = punto_central_color(imgThresholded, "green");

    /////////////////////////////////////////////////// CYAN
    lower_bound = Scalar(ch->skyblueCalib.data[0], ch->skyblueCalib.data[1], ch->skyblueCalib.data[2]);
    upper_bound = Scalar(ch->skyblueCalib.data[3], ch->skyblueCalib.data[4], ch->skyblueCalib.data[5]);
    inRange(input, lower_bound, upper_bound, imgThresholded); //Threshold the image
    ptos_cyan = punto_central_color(imgThresholded, "skyblue");

//    cout << "TIME MEDIUM " << time.elapsed() << endl;
    reset();

    if (false) { // if my team is yellow
        //// JUGADOR_AMARILLO_ROJO
        vector<Point2f> jugador_Am_Ro = punto_central(ptos_amarillo,ptos_rojo);
        my_team[0] = jugador_Am_Ro.at(2);
        my_team_ori[0] = jugador_Am_Ro.at(0);

        //// JUGADOR_AMARILLO_VERDE
        vector<Point2f> jugador_Am_Ve = punto_central(ptos_amarillo,ptos_verde);
        my_team[1] = jugador_Am_Ve.at(2);
        my_team_ori[1] = jugador_Am_Ve.at(0);

        //// JUGADOR_AMARILLO_CYAN
        vector<Point2f> jugador_Am_Cy = punto_central(ptos_amarillo,ptos_cyan);
        my_team[2] = jugador_Am_Cy.at(2);
        my_team_ori[2] = jugador_Am_Cy.at(0);

        //// JUGADOR_AZUL_ROJO
        vector<Point2f> jugador_Az_Ro = punto_central(ptos_azul,ptos_rojo);
        op_team[0] = jugador_Az_Ro.at(2);

        //// JUGADOR_AZUL_VERDE
        vector<Point2f> jugador_Az_Ve = punto_central(ptos_azul,ptos_verde);
        op_team[1] = jugador_Az_Ve.at(2);

        //// JUGADOR_AZUL_CYAN
        vector<Point2f> jugador_Az_Cy = punto_central(ptos_azul,ptos_cyan);
        op_team[2] = jugador_Az_Cy.at(2);
    }
    else {
        //// JUGADOR_AMARILLO_ROJO
        vector<Point2f> jugador_Am_Ro = punto_central(ptos_amarillo,ptos_rojo);
        op_team[0] = jugador_Am_Ro.at(2);

        //// JUGADOR_AMARILLO_VERDE
        vector<Point2f> jugador_Am_Ve = punto_central(ptos_amarillo,ptos_verde);
        op_team[1] = jugador_Am_Ve.at(2);

        //// JUGADOR_AMARILLO_CYAN
        vector<Point2f> jugador_Am_Cy = punto_central(ptos_amarillo,ptos_cyan);
        op_team[2] = jugador_Am_Cy.at(2);

        //// JUGADOR_AZUL_ROJO
        vector<Point2f> jugador_Az_Ro = punto_central(ptos_azul,ptos_rojo);
        my_team[0] = jugador_Az_Ro.at(2);
        my_team_ori[0] = jugador_Az_Ro.at(0);

        //// JUGADOR_AZUL_VERDE
        vector<Point2f> jugador_Az_Ve = punto_central(ptos_azul,ptos_verde);
        my_team[1] = jugador_Az_Ve.at(2);
        my_team_ori[1] = jugador_Az_Ve.at(0);

        //// JUGADOR_AZUL_CYAN
        vector<Point2f> jugador_Az_Cy = punto_central(ptos_azul,ptos_cyan);
        my_team[2] = jugador_Az_Cy.at(2);
        my_team_ori[2] = jugador_Az_Cy.at(0);
    }


    //// BALL
    lower_bound = Scalar(ch->orangeCalib.data[0], ch->orangeCalib.data[1], ch->orangeCalib.data[2]);
    upper_bound = Scalar(ch->orangeCalib.data[3], ch->orangeCalib.data[4], ch->orangeCalib.data[5]);
    inRange(*frame, lower_bound, upper_bound, imgThresholded); //Threshold the image
    ball = find_ball(imgThresholded);
//    cout << "TIME PROC " << time.elapsed() << endl;

    //// SEND INFO
    /*[x,y,theta] de red, [x,y,theta] de red, [x,y] pelota*/
    float data[] = {
        1.0, 2.0,3.0
    };
    socket.writeDatagram((char*)data, sizeof(data), QHostAddress("127.0.0.1"), 9002);
    repaint();
}


void ShowVisionWidget::setFrame(Mat *frame)
{
    QImage qimg((uchar*)frame->data, frame->cols, frame->rows, frame->step, QImage::Format_RGB888);
    qimg = qimg.copy();
    if (img) {
        delete img;
    }
    img = new QImage(qimg);
    if (width() != img->width()) {
        resize(img->width(), img->height());
        updateGeometry();
    }
}
