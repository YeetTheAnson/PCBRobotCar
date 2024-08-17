#include <Wire.h>
#include <MPU6050.h>

#define SDA_PIN 8
#define SCL_PIN 18

MPU6050 mpu;

void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  } else {
    Serial.println("MPU6050 connection successful");
  }
}

void loop() {
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  int16_t temp;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  temp = mpu.getTemperature();

  Serial.print(ax); Serial.print(";");
  Serial.print(ay); Serial.print(";");
  Serial.print(az); Serial.print(";");
  Serial.print(gx); Serial.print(";");
  Serial.print(gy); Serial.print(";");
  Serial.print(gz); Serial.print(";");
  Serial.print(temp); Serial.println();

  delay(100);
}
