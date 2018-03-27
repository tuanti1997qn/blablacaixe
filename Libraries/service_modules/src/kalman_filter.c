#include "kalman_filter.h"

//#define RESTRICT_PITCH

/* global value */
static float f_Q_angle = 0.001f;
static float f_Q_bias = 0.003f;
static float f_R_measure = 0.03f;
static float f_angle = 0.0f; // Reset the angle
static float f_bias = 0.0f; // Reset bias
static float f_rate = 0.0f;
static float f_P[2][2];


static float f_angleX_;
static float f_angleY_;

double gyroXangle, gyroYangle; // Angle calculate using the gyro only
double compAngleX, compAngleY; // Calculated angle using a complementary filter
double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter

/* static function */
static void Kalman_Filter_Init(void);
static float Kalman_Get_Angle(float newAngle, float newRate, float dt);
static double ABS(double var);

/*-----------------------------------------------------------------------------------------*/

static double ABS(double var)
{
    if(var < 0){
        return (-var);
    }
    return var;
}

void Kalman_Filter_Process(void)
{
    int16_t i16_Accel[3];
    int16_t i16_Gyro[3];
    double accX, accY, accZ;
    double gyroX, gyroY, gyroZ;

    v_Mpu6050_get_Accel_Raw(i16_Accel);
    v_Mpu6050_get_Gyro_Raw(i16_Gyro);

    accX = i16_Accel[0];
    accY = i16_Accel[1];
    accZ = i16_Accel[2];

    gyroX = i16_Gyro[0];
    gyroY = i16_Gyro[1];
    gyroZ = i16_Gyro[2];

    // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
    // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
    // It is then converted from radians to degrees
  #ifdef RESTRICT_PITCH // Eq. 25 and 26
    double roll  = atan2(accY, accZ) * RAD_TO_DEG;
    double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
  #else // Eq. 28 and 29
    double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
    double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
  #endif

    double gyroXrate = gyroX / 131.0; // Convert to deg/s
    double gyroYrate = gyroY / 131.0; // Convert to deg/s

	#ifdef RESTRICT_PITCH
    // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
    f_angle = f_angleX_;
    if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
        f_angle = roll;
        compAngleX = roll;
        kalAngleX = roll;
        gyroXangle = roll;
    }
    else
    {
        kalAngleX = Kalman_Get_Angle(roll, gyroXrate, dt_update); // Calculate the angle using a Kalman filter
        f_angleX_ = kalAngleX;
    }

    f_angle = f_angleY_;
    if (ABS(kalAngleX) > 90)
    {
        gyroYrate = -gyroYrate; // Invert rate, so it fits the restriced accelerometer reading
    }
    kalAngleY = Kalman_Get_Angle(pitch, gyroYrate, dt_update);
    f_angleY_ = gyroYrate;
#else
    // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
    f_angle = f_angleY_;
    if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) {
        f_angle = pitch;
          compAngleY = pitch;
          kalAngleY = pitch;
          gyroYangle = pitch;
    }
    else
    {
        kalAngleY = Kalman_Get_Angle(pitch, gyroYrate, dt_update); // Calculate the angle using a Kalman filter
        f_angleY_ = kalAngleY;
    }

    f_angle = f_angleX_;
    if (ABS(kalAngleY) > 90)
        gyroXrate = -gyroXrate; // Invert rate, so it fits the restriced accelerometer reading
    kalAngleX = Kalman_Get_Angle(roll, gyroXrate, dt_update); // Calculate the angle using a Kalman filter
    f_angleX_ = kalAngleX;
#endif

    gyroXangle += gyroXrate * dt_update; // Calculate gyro angle without any filter
    gyroYangle += gyroYrate * dt_update;
    //gyroXangle += kalmanX.getRate() * dt; // Calculate gyro angle using the unbiased rate
    //gyroYangle += kalmanY.getRate() * dt;

    compAngleX = 0.93 * (compAngleX + gyroXrate * dt_update) + 0.07 * roll; // Calculate the angle using a Complimentary filter
    compAngleY = 0.93 * (compAngleY + gyroYrate * dt_update) + 0.07 * pitch;

    // Reset the gyro angle when it has drifted too much
    if (gyroXangle < -180 || gyroXangle > 180)
        gyroXangle = kalAngleX;
    if (gyroYangle < -180 || gyroYangle > 180)
        gyroYangle = kalAngleY;
}

void MPU6050_Kalman_Angle(double * d_angle)
{
    d_angle[0] = kalAngleX;
    d_angle[1] = kalAngleY;
}

void MPU6050_Complimentary_Angle(double * d_angle)
{
    d_angle[0] = compAngleX;
    d_angle[1] = compAngleY;
}

static void Kalman_Filter_Init(void) 
{
    /* We will set the variables like so, these can also be tuned by the user */
    f_Q_angle = 0.001f;
    f_Q_bias = 0.003f;
    f_R_measure = 0.03f;

    f_angle = 0.0f; // Reset the angle
    f_bias = 0.0f; // Reset bias

    f_P[0][0] = 0.0f; // Since we assume that the bias is 0 and we know the starting angle (use setAngle), the error covariance matrix is set like so - see: http://en.wikipedia.org/wiki/Kalman_filter#Example_application.2C_technical
    f_P[0][1] = 0.0f;
    f_P[1][0] = 0.0f;
    f_P[1][1] = 0.0f;
}

// The angle should be in degrees and the rate should be in degrees per second and the delta time in seconds
static float Kalman_Get_Angle(float newAngle, float newRate, float dt) 
{
    // KasBot V2  -  Kalman filter module - http://www.x-firm.com/?page_id=145
    // Modified by Kristian Lauszus
    // See my blog post for more information: http://blog.tkjelectronics.dk/2012/09/a-practical-approach-to-kalman-filter-and-how-to-implement-it

    // Discrete Kalman filter time update equations - Time Update ("Predict")
    // Update xhat - Project the state ahead
    /* Step 1 */
    f_rate = newRate - f_bias;
    f_angle += dt * f_rate;

    // Update estimation error covariance - Project the error covariance ahead
    /* Step 2 */
    f_P[0][0] += dt * (dt*f_P[1][1] - f_P[0][1] - f_P[1][0] + f_Q_angle);
    f_P[0][1] -= dt * f_P[1][1];
    f_P[1][0] -= dt * f_P[1][1];
    f_P[1][1] += f_Q_bias * dt;

    // Discrete Kalman filter measurement update equations - Measurement Update ("Correct")
    // Calculate Kalman gain - Compute the Kalman gain
    /* Step 4 */
    float S = f_P[0][0] + f_R_measure; // Estimate error
    /* Step 5 */
    float K[2]; // Kalman gain - This is a 2x1 vector
    K[0] = f_P[0][0] / S;
    K[1] = f_P[1][0] / S;

    // Calculate angle and bias - Update estimate with measurement zk (newAngle)
    /* Step 3 */
    float y = newAngle - f_angle; // Angle difference
    /* Step 6 */
    f_angle += K[0] * y;
    f_bias += K[1] * y;

    // Calculate estimation error covariance - Update the error covariance
    /* Step 7 */
    float P00_temp = f_P[0][0];
    float P01_temp = f_P[0][1];

    f_P[0][0] -= K[0] * P00_temp;
    f_P[0][1] -= K[0] * P01_temp;
    f_P[1][0] -= K[1] * P00_temp;
    f_P[1][1] -= K[1] * P01_temp;

    return f_angle;
}

void Kalman_Set_Angle(float angle) 
{
    f_angle = angle;
}   // Used to set angle, this should be set as the starting angle

float Kalman_Get_Rate(void) {
    return f_rate;
}   // Return the unbiased rate

/* These are used to tune the Kalman filter */
void Kalman_Set_Qangle(float Q_angle) 
{
    f_Q_angle = Q_angle;
}

void Kalman_Set_Qbias(float Q_bias) 
{
    f_Q_bias = Q_bias;
}

void Kalman_Set_Rmeasure(float R_measure) 
{
    f_R_measure = R_measure;
}

float Kalman_Get_Qangle(void) 
{
    return f_Q_angle;
}

float Kalman_Get_Qbias(void)
{
    return f_Q_bias;
}

float Kalman_Get_Rmeasure(void)
{
    return f_R_measure;
}
