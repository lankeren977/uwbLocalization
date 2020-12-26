#include "location.h"

vec2d last_result;

//多球交汇原理
vec2d trilateration(const vec2d *anchorArray, const int *radius, const int count)
{
    vec2d result;
    //初始化圆
    circle circle1, circle2, circle3;
    circle1.x = anchorArray[0].x;
    circle1.y = anchorArray[0].y;
    circle1.r = radius[0];

    circle2.x = anchorArray[1].x;
    circle2.y = anchorArray[1].y;
    circle2.r = radius[1];

    circle3.x = anchorArray[2].x;
    circle3.y = anchorArray[2].y;
    circle3.r = radius[2];

    vec2d *insect1, *insect2, *insect3;

    if (count == 3) //一次三角质心
    {
        insect(insect1, circle1, circle2);
        insect(insect2, circle1, circle3);
        insect(insect3, circle2, circle3);

        if (insect1 != NULL && insect2 != NULL && insect3 != NULL)
        {
            vec2d *alter_points = new vec2d[3];
            alter_points[0] = selectPoint(insect1, circle3);
            alter_points[1] = selectPoint(insect2, circle3);
            alter_points[2] = selectPoint(insect3, circle1);

            result = optimizeByCentroid(alter_points);
            last_result = result;
            return result;
        }
        return last_result;
    }
    // else if (count == 4) //两次三角质心
    // {
    //     circle4.x = anchorArray[3].x;
    //     circle4.y = anchorArray[3].y;
    //     circle4.r = radius[3];
    //     vec2d insect4[2];
    // }
    else
    {
        cout << "基站个数异常" << endl;
        return last_result;
    }
}

//确定备选三角顶点
vec2d selectPoint(const vec2d *points, const circle circle)
{
    vec2d alter_point;
    if (!isOutsideCircle(points[0], circle))
    {
        alter_point = points[0];
    }
    else if (!isOutsideCircle(points[1], circle))
    {
        alter_point = points[1];
    }
    else
    {
        double p1_dis_sqr = pow(points[0].x - circle.x, 2) + pow(points[0].y - circle.y, 2);
        double p2_dis_sqr = pow(points[1].x - circle.x, 2) + pow(points[1].y - circle.y, 2);
        alter_point = (p1_dis_sqr <= p2_dis_sqr) ? points[0] : points[1];
    }
    return alter_point;
}

//判断一点是否在圆外
bool isOutsideCircle(const vec2d point, const circle circle)
{
    double dis_sqr = pow(point.x - circle.x, 2) + pow(point.y - circle.y, 2);
    return (dis_sqr > circle.y * circle.y) ? true : false;
}

//两圆求交点
void insect(vec2d* &insect, const circle circle1, const circle circle2)
{
    vec2d *result = new vec2d[2];
    double dis, a, b, c, p, q, r;
    double cos_value[2], sin_value[2];

    //判断是否内含或外离
    dis = sqrt(pow(circle1.x - circle2.x, 2) + pow(circle1.y - circle2.y, 2));
    if (dis < fabs(circle1.r - circle2.r) || dis > circle1.r + circle2.r)
    {
        cout << "两圆没有交点" << endl;
        return;
    }
    //一个内交点为异常
    if (dis == fabs(circle1.r - circle2.r))
    {
        cout << "两圆有一个内交点" << endl;
        return;
    }

    //初始化参数
    a = 2 * circle1.r * (circle1.x - circle2.x);
    b = 2 * circle1.r * (circle1.y - circle2.y);
    c = pow(circle2.r, 2) - pow(circle1.r, 2) - pow(circle1.x - circle2.x, 2) - pow(circle1.y - circle2.y, 2);
    p = a * a + b * b;
    q = -2 * a * c;
    r = c * c - b * b;

    //如果只有一个外交点
    if (fabs(dis - (circle1.r + circle2.r)) < ZERO)
    {
        cos_value[0] = -q / p / 2;
        sin_value[0] = sqrt(1 - pow(cos_value[0], 2));

        result[0].x = circle1.x + circle1.r * cos_value[0];
        result[0].y = circle1.y + circle1.r * sin_value[0];
        //验证sin正负
        if (pow(result[0].x - circle2.x, 2) + pow(result[0].y - circle2.y, 2) - pow(circle2.r, 2) > ZERO)
        {
            result[0].y = circle1.y - circle1.r * sin_value[0];
        }
        result[1] = result[0];
        insect = result;
    }

    //如果有两个交点
    cos_value[0] = (sqrt(q * q - 4 * p * r) - q) / p / 2;
    cos_value[1] = (-sqrt(q * q - 4 * p * r) - q) / p / 2;
    sin_value[0] = sqrt(1 - pow(cos_value[0], 2));
    sin_value[1] = sqrt(1 - pow(cos_value[1], 2));

    result[0].x = circle1.x + circle1.r * cos_value[0];
    result[1].x = circle1.x + circle1.r * cos_value[1];
    result[0].y = circle1.y + circle1.r * sin_value[0];
    result[1].y = circle1.y + circle1.r * sin_value[1];
    //验证解
    if (pow(result[0].x - circle2.x, 2) + pow(result[0].y - circle2.y, 2) - pow(circle2.r, 2) > ZERO)
    {
        result[0].y = circle1.y - circle1.r * sin_value[0];
    }
    if (pow(result[1].x - circle2.x, 2) + pow(result[1].y - circle2.y, 2) - pow(circle2.r, 2) > ZERO)
    {
        result[1].y = circle1.y - circle1.r * sin_value[1];
    }
    //一种特殊情况当已经确定有两个不同的交点，但解出来的cos值只有一个，则sin值必定一正一负
    if (fabs(result[0].x - result[1].x) < ZERO && fabs(result[0].y - result[1].y) < ZERO)
    {
        if (result[0].y > 0)
        {
            result[1].y = -result[1].y;
        }
        else
        {
            result[0].y = -result[0].y;
        }
    }
    insect = result;
}

//三角质心优化
vec2d optimizeByCentroid(const vec2d *points)
{
    vec2d result;
    result.x = (points[0].x + points[1].x + points[2].x) / 3;
    result.y = (points[0].y + points[1].y + points[2].y) / 3;
    return result;
}