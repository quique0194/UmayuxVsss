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

QPoint Point2QPoint(const Point& p) {
    return QPoint(p.x, p.y);
}

bool ShowVisionWidget::validPoint(const Point& p) {
    if (!img) {
        return false;
    }
    return p.x >= 0 && p.x < img->width() && p.y >= 0 && p.y < img->height();
}

void ShowVisionWidget::reset()
{
    my_team.assign(3,Point(-1, -1));
    my_team_ori.assign(3,Point(-1, -1));
    op_team.assign(3,Point(-1, -1));
    ball = Point(-1, -1);
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
                q = Point2QPoint(my_team_ori[i]);
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

vector<Point> punto_central_color(Mat imgThresholded, const string& color="") {
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
//    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

    Mat cc, ret;
    int blobs = connectedComponents(imgThresholded, cc, 4);
    vector<Moments> mu(blobs-1);
    vector<Point2f> mc(blobs-1);
    vector<Point> puntos_color;

    for (int i = 1; i < blobs; ++i) {
        inRange(cc, i, i, ret);
        Rect br = boundingRect(ret);
        Point p;
        p.x = br.x + br.width/2;
        p.y = br.y + br.height/2;
        puntos_color.push_back(p);
    }

    return puntos_color;
}


vector<Point> punto_central(const vector<Point>& color_equipo, const vector<Point>& color_jugador){
    vector<Point> puntos_cercanos;
    double dist_min = 20;
    Point p1, p2, p_med;

    for(size_t i=0; i<color_equipo.size(); i++){
        for(size_t j=0; j<color_jugador.size(); j++){
            int a = color_equipo[i].x - color_jugador[j].x;
            int b = color_equipo[i].y - color_jugador[j].y;
            double distancia = sqrt(a*a + b*b);
            if (distancia < dist_min){
                dist_min = distancia;
                p1 = color_equipo[i];
                p2 = color_jugador[j];
                p_med.x = (p1.x+p2.x)/2;
                p_med.y = (p1.y+p2.y)/2;
            }
        }
    }
    if (dist_min == 20) {
        p1 = p2 = p_med = Point(-1,-1);
    }

    puntos_cercanos.push_back(p1);
    puntos_cercanos.push_back(p2);
    puntos_cercanos.push_back(p_med);

    return puntos_cercanos;
}

Point closest_candidate(const vector<Point>& candidates, const Point& last_position) {
    int min_dist_cuad = 10000000;
    Point closest(-1, -1);
    for (int i = 0; i < candidates.size(); ++i) {
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

Point ShowVisionWidget::find_ball(Mat imgThresholded) {
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
    dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

    Mat cc, ret;
    int blobs = connectedComponents(imgThresholded, cc, 4);

    vector<int> areas;
    vector<Point> candidates;
    for (int i = 1; i < blobs; ++i) {
        inRange(cc, i, i, ret);
        int area = countNonZero(ret);
        areas.push_back(area);
        Rect br = boundingRect(ret);
        if (area > 60 && area < 120 && abs(br.width-br.height) < 2) {
            Point p;
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
    Mat imgThresholded;
    Scalar lower_bound, upper_bound;

    /////////////////////////////////////////////////// AMARILLO
    lower_bound = Scalar(ch->yellowCalib.data[0], ch->yellowCalib.data[1], ch->yellowCalib.data[2]);
    upper_bound = Scalar(ch->yellowCalib.data[3], ch->yellowCalib.data[4], ch->yellowCalib.data[5]);
    inRange(*frame, lower_bound, upper_bound, imgThresholded); //Threshold the image
    vector<Point> ptos_amarillo = punto_central_color(imgThresholded, "yellow");

    /////////////////////////////////////////////////// AZUL
    lower_bound = Scalar(ch->blueCalib.data[0], ch->blueCalib.data[1], ch->blueCalib.data[2]);
    upper_bound = Scalar(ch->blueCalib.data[3], ch->blueCalib.data[4], ch->blueCalib.data[5]);
    inRange(*frame, lower_bound, upper_bound, imgThresholded); //Threshold the image
    vector<Point> ptos_azul = punto_central_color(imgThresholded, "blue");

    /////////////////////////////////////////////////// ROJO
    lower_bound = Scalar(ch->redCalib.data[0], ch->redCalib.data[1], ch->redCalib.data[2]);
    upper_bound = Scalar(ch->redCalib.data[3], ch->redCalib.data[4], ch->redCalib.data[5]);
    inRange(*frame, lower_bound, upper_bound, imgThresholded); //Threshold the image
    vector<Point> ptos_rojo = punto_central_color(imgThresholded, "red");

    /////////////////////////////////////////////////// VERDE
    lower_bound = Scalar(ch->greenCalib.data[0], ch->greenCalib.data[1], ch->greenCalib.data[2]);
    upper_bound = Scalar(ch->greenCalib.data[3], ch->greenCalib.data[4], ch->greenCalib.data[5]);
    inRange(*frame, lower_bound, upper_bound, imgThresholded); //Threshold the image
    vector<Point> ptos_verde = punto_central_color(imgThresholded, "green");

    /////////////////////////////////////////////////// CYAN
    lower_bound = Scalar(ch->skyblueCalib.data[0], ch->skyblueCalib.data[1], ch->skyblueCalib.data[2]);
    upper_bound = Scalar(ch->skyblueCalib.data[3], ch->skyblueCalib.data[4], ch->skyblueCalib.data[5]);
    inRange(*frame, lower_bound, upper_bound, imgThresholded); //Threshold the image
    vector<Point> ptos_cyan = punto_central_color(imgThresholded, "skyblue");

    cout << "TIME MEDIUM " << time.elapsed() << endl;
    //reset();

    //// JUGADOR_AMARILLO_ROJO
    vector<Point> jugador_Am_Ro = punto_central(ptos_amarillo,ptos_rojo);
    my_team[0] = jugador_Am_Ro.at(2);
    my_team_ori[0] = jugador_Am_Ro.at(0);

    //// JUGADOR_AMARILLO_VERDE
    vector<Point> jugador_Am_Ve = punto_central(ptos_amarillo,ptos_verde);
    my_team[1] = jugador_Am_Ve.at(2);
    my_team_ori[1] = jugador_Am_Ve.at(0);

    //// JUGADOR_AMARILLO_CYAN
    vector<Point> jugador_Am_Cy = punto_central(ptos_amarillo,ptos_cyan);
    my_team[2] = jugador_Am_Cy.at(2);
    my_team_ori[2] = jugador_Am_Cy.at(0);

    //// JUGADOR_AZUL_ROJO
    vector<Point> jugador_Az_Ro = punto_central(ptos_azul,ptos_rojo);
    op_team[0] = jugador_Az_Ro.at(2);

    //// JUGADOR_AZUL_VERDE
    vector<Point> jugador_Az_Ve = punto_central(ptos_azul,ptos_verde);
    op_team[1] = jugador_Az_Ve.at(2);

    //// JUGADOR_AZUL_CYAN
    vector<Point> jugador_Az_Cy = punto_central(ptos_azul,ptos_cyan);
    op_team[2] = jugador_Az_Cy.at(2);

    //// BALL
    lower_bound = Scalar(ch->orangeCalib.data[0], ch->orangeCalib.data[1], ch->orangeCalib.data[2]);
    upper_bound = Scalar(ch->orangeCalib.data[3], ch->orangeCalib.data[4], ch->orangeCalib.data[5]);
    inRange(*frame, lower_bound, upper_bound, imgThresholded); //Threshold the image
    ball = find_ball(imgThresholded);
    cout << "TIME PROC " << time.elapsed() << endl;

    //// SEND INFO
    /*[x,y,theta] de red, [x,y,theta] de red, [x,y] pelota*/
    float data[] = {
        1.0, 2.0,3.0
    };
    socket.writeDatagram((char*)data, sizeof(data), QHostAddress("127.0.0.1"), 9002);
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
